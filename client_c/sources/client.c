#include <stdio.h>
#include <string.h>
#include "socket_client.h" 
#include "utils.h"

int main() {
    // Crear y configurar ClientSocketConfig usando un puntero
    ClientSocketConfigPtr clientConfig = crearClientSocketConfig();
    if (clientConfig == NULL) {
        return 1;
    }

    // Inicializar el socket del cliente
    if (initializeClientSocket(clientConfig, "127.0.0.1", 8080) != 0) {
        liberarClientSocketConfig(clientConfig);
        return 1;
    }

    int choice, length;

    while (1) {
        // Mostrar el menú y obtener la elección del usuario
        choice = showMenu();

        if (choice == 3) {
            break; // Salir del bucle si el usuario elige salir
        }

        // Obtener la longitud deseada
        length = getLength();

        // Crear la solicitud basada en la elección del usuario
        char request[BUFFER_SIZE];
        if (choice == 1) {
            snprintf(request, sizeof(request), "USERNAME %d", length);
        } else if (choice == 2) {
            snprintf(request, sizeof(request), "PASSWORD %d", length);
        } else {
            printf("Opción inválida.\n");
            continue; // Volver a mostrar el menú en caso de opción inválida
        }

        // Enviar solicitud y mostrar la respuesta
        requestAndPrintResult(clientConfig, request);
    }

    // Cerrar el socket y limpiar
    cleanupClientSocket(clientConfig);

    // Liberar memoria de ClientSocketConfig
    liberarClientSocketConfig(clientConfig);

    return 0;
}