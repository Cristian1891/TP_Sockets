#include <stdio.h>
#include "socket_server.h"

int main() {
    // Crear la configuración del servidor
    ServerSocketConfigPtr serverConfig = crearServerSocketConfig();
    if (serverConfig == NULL) {
        return 1;
    }

    // Inicializar el socket del servidor
    if (initializeServerSocket(serverConfig, 8080) != 0) {
        liberarServerSocketConfig(serverConfig);
        return 1;
    }

    // Lógica para aceptar conexiones y manejar clientes
    while (1) {
        printf("Esperando conexiones...\n");
        SOCKET clientSocket = accept(getServerSocket(serverConfig), NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            printf("Error al aceptar conexión: %d\n", WSAGetLastError());
            break;
        }
        printf("Cliente conectado!\n");
        handleClient(clientSocket);
    }

    // Cerrar el socket y limpiar
    cleanupServerSocket(serverConfig);
    liberarServerSocketConfig(serverConfig);

    return 0;
}