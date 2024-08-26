#ifndef SOCKET_H
#define SOCKET_H
#define BUFFER_SIZE 1024

//Incluye el archivo de encabezado de Winsock para el manejo de sockets en Windows.
#include <winsock2.h>
#include "generator.h"

// Definición de la estructura y punteros
struct ServerSocketConfig;
typedef struct ServerSocketConfig * ServerSocketConfigPtr;
typedef WSADATA* WSADATAPtr;
typedef struct sockaddr_in * ServerAddressPtr;
 
// Constructor y destructor
ServerSocketConfigPtr crearServerSocketConfig();
void liberarServerSocketConfig(ServerSocketConfigPtr config);

// Getters y setters
WSADATAPtr getWsaData(ServerSocketConfigPtr config);
SOCKET getServerSocket(ServerSocketConfigPtr config);
ServerAddressPtr getServerAddress(ServerSocketConfigPtr config);
void setWsaData(ServerSocketConfigPtr config, WSADATAPtr wsaData);
void setServerSocket(ServerSocketConfigPtr config, SOCKET serverSocket);
void setServerAddress(ServerSocketConfigPtr config, ServerAddressPtr serverAddress);

// Funciones para inicializar y cerrar el socket del servidor
int initializeServerSocket(ServerSocketConfigPtr config, int port);
void cleanupServerSocket(ServerSocketConfigPtr config);
void handleClient(SOCKET clientSocket);

#endif // SOCKET_H