#include "socket_server.h"
#include <stdio.h>

// Implementación de la estructura ServerSocketConfig
struct ServerSocketConfig {
    WSADATAPtr wsaData;
    SOCKET serverSocket;
    ServerAddressPtr serverAddress;
};

// Función para crear una nueva configuración del socket del servidor
ServerSocketConfigPtr crearServerSocketConfig() {
    ServerSocketConfigPtr config = (ServerSocketConfigPtr)malloc(sizeof(struct ServerSocketConfig));
    if (config == NULL) {
        printf("Error al asignar memoria para ServerSocketConfig\n");
        return NULL;
    }
    // Inicializar el puntero wsaData
    config->wsaData = (WSADATAPtr)malloc(sizeof(WSADATA));
    if (config->wsaData == NULL) {
        printf("Error al asignar memoria para WSADATA\n");
        free(config);  
        return NULL;
    }

    // Inicializar el puntero serverAddress
    config->serverAddress = (ServerAddressPtr)malloc(sizeof(struct sockaddr_in));
    if (config->serverAddress == NULL) {
        printf("Error al asignar memoria para sockaddr_in\n");
        free(config->wsaData);  
        free(config);  
        return NULL;
    }

    // Inicializar el socket a un valor predeterminado
    config->serverSocket = INVALID_SOCKET;

    return config;
}

// Función para liberar la memoria de la configuración del socket del servidor
void liberarServerSocketConfig(ServerSocketConfigPtr config) {
    if (config != NULL) {
        if (config->wsaData != NULL) {
            free(config->wsaData);
        }
        if (config->serverAddress != NULL) {
            free(config->serverAddress);
        }
        free(config);
    }
}

// Getters
WSADATAPtr getWsaData(ServerSocketConfigPtr config) {
    return config->wsaData;
}

SOCKET getServerSocket(ServerSocketConfigPtr config) {
    return config->serverSocket;
}

ServerAddressPtr getServerAddress(ServerSocketConfigPtr config) {
    return config->serverAddress;
}

// Setters
void setWsaData(ServerSocketConfigPtr config, WSADATAPtr wsaData) {
    config->wsaData = wsaData;
}

void setServerSocket(ServerSocketConfigPtr config, SOCKET serverSocket) {
    config->serverSocket = serverSocket;
}

void setServerAddress(ServerSocketConfigPtr config, ServerAddressPtr serverAddress) {
    config->serverAddress = serverAddress;
}


// Inicialización del socket del servidor y configuración de Winsock
int initializeServerSocket(ServerSocketConfigPtr config, int port) {
    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), getWsaData(config)) != 0) {
        printf("Error en WSAStartup: %d\n", WSAGetLastError());
        return 1;
    }

    // Crear el socket del servidor
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Error al crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    setServerSocket(config, serverSocket);

    // Configurar la dirección del servidor
    ServerAddressPtr serverAddress = getServerAddress(config);
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = INADDR_ANY;
    serverAddress->sin_port = htons(port);

    // Enlazar el socket a la dirección y puerto
    if (bind(getServerSocket(config), (struct sockaddr *)serverAddress, sizeof(*serverAddress)) == SOCKET_ERROR) {
        printf("Error al enlazar el socket: %d\n", WSAGetLastError());
        cleanupServerSocket(config);
        return 1;
    }

    // Poner el socket en modo de escucha
    if (listen(getServerSocket(config), SOMAXCONN) == SOCKET_ERROR) {
        printf("Error al poner el socket en modo de escucha: %d\n", WSAGetLastError());
        cleanupServerSocket(config);
        return 1;
    }

    printf("Servidor iniciado en el puerto %d...\n", port);
    return 0;
}

// Función para cerrar el socket y limpiar Winsock
void cleanupServerSocket(ServerSocketConfigPtr config) {
    closesocket(getServerSocket(config));
    WSACleanup();
}

// Función para manejar la solicitud de un cliente
void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error == WSAECONNRESET) {
                printf("Cliente desconectado abruptamente.\n");
            } else {
                printf("Error al recibir datos: %d\n", error);
            }
            break;
        } else if (bytesReceived == 0) {
            printf("Cliente desconectado normalmente.\n");
            break;
        }
        

        buffer[bytesReceived] = '\0'; // Asegurar de que el buffer esté terminado en null
        printf("Mensaje recibido: %s\n", buffer);

        char response[BUFFER_SIZE];
        int length;
        if (sscanf(buffer, "USERNAME %d", &length) == 1) {
            if (length < 5 || length > 15) {
                snprintf(response, sizeof(response), "La longitud del username no debe ser menor a 5 ni mayor a 15");
            } else {
                char* username = generateUsername(length);
                if (username) {
                    snprintf(response, sizeof(response), "Username: %s", username);
                    free(username);
                } else {
                    snprintf(response, sizeof(response), "Error al generar el username");
                }
            }
        } else if (sscanf(buffer, "PASSWORD %d", &length) == 1) {
            if (length < 8 || length >= 50) {
                snprintf(response, sizeof(response), "La longitud de la password debe ser mayor o igual a 8, y menor a 50");
            } else {
                char* password = generatePassword(length);
                if (password) {
                    snprintf(response, sizeof(response), "Password: %s", password);
                    free(password);
                } else {
                    snprintf(response, sizeof(response), "Error al generar la password");
                }
            }
        } else {
            snprintf(response, sizeof(response), "Comando inválido");
        }

        if (send(clientSocket, response, (int)strlen(response), 0) == SOCKET_ERROR) {
            printf("Error al enviar datos: %d\n", WSAGetLastError());
            break;
        }
    }
    closesocket(clientSocket);
}