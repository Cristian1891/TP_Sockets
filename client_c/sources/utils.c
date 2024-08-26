#include <stdio.h>
#include "utils.h"

// Función para mostrar el menú y obtener la elección del usuario
int showMenu() {
    int choice;
    printf("Seleccione una opcion:\n");
    printf("1. Generar nombre de usuario\n");
    printf("2. Generar password\n");
    printf("3. Salir\n");
    printf("Ingrese su eleccion (1-3): ");
    scanf("%d", &choice);
    return choice;
}

// Función para manejar la entrada de longitud
int getLength() {
    int length;
    printf("Ingrese la longitud deseada: ");
    scanf("%d", &length);
    return length;
}