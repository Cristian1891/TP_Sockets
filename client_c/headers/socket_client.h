#ifndef SOCKET_H
#define SOCKET_H
#define BUFFER_SIZE 1024

#include <winsock2.h>

struct ClientSocketConfig;

typedef struct ClientSocketConfig * ClientSocketConfigPtr;
typedef WSADATA* WSADATAPtr;

typedef struct sockaddr_in * ServerAddressPtr;

// Constructor y destructor
ClientSocketConfigPtr crearClientSocketConfig();
void liberarClientSocketConfig(ClientSocketConfigPtr config);

// Getters
WSADATAPtr getWsaData(ClientSocketConfigPtr config);
SOCKET getClientSocket(ClientSocketConfigPtr config);
ServerAddressPtr getServerAddress(ClientSocketConfigPtr config);

// Setters
void setWsaData(ClientSocketConfigPtr config, WSADATAPtr wsaData);
void setClientSocket(ClientSocketConfigPtr config, SOCKET clientSocket);
void setServerAddress(ClientSocketConfigPtr config, ServerAddressPtr serverAddress);

// Funciones para inicializar y cerrar el socket del cliente
int initializeClientSocket(ClientSocketConfigPtr config, const char *ip, int port);
void cleanupClientSocket(ClientSocketConfigPtr config);

// Función para enviar una solicitud al servidor y mostrar la respuesta
void requestAndPrintResult(ClientSocketConfigPtr config, const char *request);

#endif // SOCKET_H