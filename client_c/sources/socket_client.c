#include "socket_client.h"
#include <stdio.h>
#include <string.h>

struct ClientSocketConfig {
    WSADATAPtr wsaData;
    SOCKET clientSocket;
    ServerAddressPtr serverAddress; 
};

ClientSocketConfigPtr crearClientSocketConfig() {
    ClientSocketConfigPtr config = (ClientSocketConfigPtr)malloc(sizeof(struct ClientSocketConfig));
    if (config == NULL) {
        printf("Error al asignar memoria para ClientSocketConfig\n");
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
    config->clientSocket = INVALID_SOCKET;

    return config;
}

void liberarClientSocketConfig(ClientSocketConfigPtr config) {
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
WSADATAPtr getWsaData(ClientSocketConfigPtr config) {
    return config->wsaData;
}

SOCKET getClientSocket(ClientSocketConfigPtr config) {
    return config->clientSocket;
}

ServerAddressPtr getServerAddress(ClientSocketConfigPtr config) {
    return config->serverAddress;
}

// Setters
void setWsaData(ClientSocketConfigPtr config, WSADATAPtr wsaData) {
    config->wsaData = wsaData;
}

void setClientSocket(ClientSocketConfigPtr config, SOCKET clientSocket) {
    config->clientSocket = clientSocket;
}

void setServerAddress(ClientSocketConfigPtr config, ServerAddressPtr serverAddress) {
    config->serverAddress = serverAddress;
}

// Función para inicializar Winsock y configurar el socket del cliente
int initializeClientSocket(ClientSocketConfigPtr config, const char *ip, int port) {
    // Inicializar Winsock
    WSADATAPtr wsaData = getWsaData(config);
    if (WSAStartup(MAKEWORD(2, 2), wsaData) != 0) {
        printf("Error en WSAStartup: %d\n", WSAGetLastError());
        return 1;
    }
    setWsaData(config, wsaData); // Guardar la configuración actualizada en el objeto

    // Crear el socket del cliente
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Error al crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    setClientSocket(config, clientSocket);

    // Configurar la dirección del servidor
    ServerAddressPtr serverAddress = getServerAddress(config);
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = inet_addr(ip);
    serverAddress->sin_port = htons(port);
    setServerAddress(config, serverAddress);

    // Conectar al servidor
    if (connect(getClientSocket(config), (struct sockaddr *)getServerAddress(config), sizeof(*getServerAddress(config))) == SOCKET_ERROR) {
        printf("Error al conectar con el servidor: %d\n", WSAGetLastError());
        cleanupClientSocket(config);
        return 1;
    }

    printf("Conectado al servidor en %s:%d...\n", ip, port);
    return 0;
}

// Función para cerrar el socket y limpiar Winsock
void cleanupClientSocket(ClientSocketConfigPtr config) {
    closesocket(getClientSocket(config));
    WSACleanup();
}

// Función para enviar una solicitud al servidor y mostrar la respuesta
void requestAndPrintResult(ClientSocketConfigPtr clientConfig, const char *request) {
    if (send(clientConfig->clientSocket, request, (int)strlen(request), 0) == SOCKET_ERROR) {
        printf("Error al enviar datos: %d\n", WSAGetLastError());
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientConfig->clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == SOCKET_ERROR) {
        printf("Error al recibir datos: %d\n", WSAGetLastError());
        return;
    }
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Asegurar de que el buffer esté terminado en null
        printf("Respuesta del servidor: %s\n", buffer);
    } else {
        printf("El servidor cerró la conexión.\n");
        return;
    }
}