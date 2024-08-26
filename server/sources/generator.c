#include "generator.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h> 

// Función para generar un nombre de usuario
char* generateUsername(int length) {
    if (length < 5 || length > 15) return NULL;

    static const char vowels[] = "aeiou";
    static const char consonants[] = "bcdfghjklmnpqrstvwxyz";
    char* username = malloc(length + 1);
    if (username == NULL) return NULL;

    srand(time(NULL)); // Semilla para la generación aleatoria

    int isVowel = rand() % 2; // Elige aleatoriamente si empezar con vocal o consonante
    for (int i = 0; i < length; ++i) {
        if (isVowel) {
            username[i] = vowels[rand() % (sizeof(vowels) - 1)];
        } else {
            username[i] = consonants[rand() % (sizeof(consonants) - 1)];
        }
        isVowel = !isVowel; // Alternar entre vocales y consonantes
    }

    username[length] = '\0';
    return username;
}

// Función para generar una contraseña
char* generatePassword(int length) {
    if (length < 8 || length >= 50) return NULL;

    static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char* password = malloc(length + 1);
    if (password == NULL) return NULL;

    srand(time(NULL)); // Semilla para la generación aleatoria

    for (int i = 0; i < length; ++i) {
        password[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    password[length] = '\0';
    return password;
}

