#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

// Entradas : Recibe un char* como el nombre del archivo.
// Salida : archivo.
// Descripción : Funcion que permite abrir un archivo en modo "r".
FILE* procesar_archivo(char *archivoentrada) {
    FILE *archivo = fopen(archivoentrada, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }
    return archivo;
}

// Entradas : Recibe el archivo, un char correspondiente a cantidad caracteres, un char correspondiente a cantidad lineas, un int* correspondiente
//            a los caracteres de comando un otro int* correspondiente a las lineas por comando.
// Salida : no retorna nada
// Descripción : Funcion que calcula la cantidad de caracteres de un archivo y la cantidad de lineas que posee el mismo archivo.
void calculo(FILE *archivo, char cantidad_caracteres, char cantidad_lineas, int *caracteres, int *lineas) {
    char c;         
    char limite = 0; //Utilizare este indicador para asegurar que el archivo se lee completo
    while ((c = fgetc(archivo)) != EOF) {
        if (cantidad_caracteres) {
            (*caracteres)++;
        }
        if (cantidad_lineas && c == '\n') {
            (*lineas)++;
        }
        limite = c;
    }
    // Si el último carácter no es '\n', contar la última línea
    if (cantidad_lineas && limite != '\n') {
        (*lineas)++;
    }
}

//Entradas : Recibe un int correspondiente a la linea de comando, un int correspondiente a los caracteres de comando, un char para guardar la cantidad de lineas 
//            y otra char para guardar la cantidad de caracteres.
// Salida : no retorna nada.
// Descripción : Funcion que imprime los resultados.
void resultados(int lineas, int caracteres, char cantidad_lineas, char cantidad_caracteres) {
    if (cantidad_lineas && cantidad_caracteres) {
        printf("%d %d\n", lineas, caracteres);   //Imprime la cantidad de líneas y caracteres
    } else if (cantidad_lineas) {
        printf("%d\n", lineas); // Imprime la cantidad de líneas
    } else if (cantidad_caracteres) {
        printf("%d\n", caracteres); // Imprime la cantidad de caracteres
    }else{
        printf("Error al imprimir los caracteres o lineas\n");
    }
}