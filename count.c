#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "funcionescount.c"

FILE* vaciar_archivo(char *nombreArchivo);
void calculo(FILE *archivo, char cantidad_caracteres, char cantidad_lineas, int *caracteres, int *lineas);
void resultados(int lineas, int caracteres, char cantidad_lineas, char cantidad_caracteres);

// Manejo de errores y archivo
FILE* vaciar_archivo(char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo, "w");

    // Verificar si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
    }

    return archivo;
}

// Bloque principal
int main(int argc, char *argv[]) {
    // Variables para almacenar las opciones de línea de comandos
    char *archivoentrada = NULL;  // No se usará en este caso si leemos de stdin
    char cantidad_lineas = 0;
    char cantidad_caracteres = 0;
    int caracteres = 0;
    int lineas = 0;
    char *archivosalida = NULL;  // Para la opción -o

    int option;
    while ((option = getopt(argc, argv, "i:o:CL")) != -1) {  // Agregamos 'o:' para la opción de salida
        switch (option) {
            case 'i':
                archivoentrada = optarg; // Nombre del archivo de entrada (no necesario si leemos de stdin)
                break;
            case 'C':
                cantidad_caracteres = 1; // Contar el número de caracteres
                break;
            case 'L':
                cantidad_lineas = 1; // Contar el número de líneas
                break;
            case 'o':
                archivosalida = optarg;  // Nombre del archivo de salida
                break;
            default:
                fprintf(stderr, "Uso: %s [-L] [-C] [-o archivo_salida]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Manejo de errores en el archivo
    if (archivoentrada == NULL) {
        // Si no se especifica el archivo de entrada, leer de stdin
        archivoentrada = "stdin"; // Esto es solo para el flujo de trabajo
    }

    // Limpiar archivo de salida si se especificó
    FILE *salida = stdout;  // Por defecto, escribir en stdout
    if (archivosalida != NULL) {
        salida = vaciar_archivo(archivosalida);  // Limpiar y abrir el archivo de salida
    }

    // Leer desde stdin (si no hay archivo de entrada especificado)
    FILE *entrada;
    if (archivoentrada != NULL && strcmp(archivoentrada, "stdin") == 0) {
        entrada = stdin;  // Si no se especifica archivo, se usa stdin
    } else {
        entrada = fopen(archivoentrada, "r");  // Si se especifica archivo, se intenta abrir
        if (entrada == NULL) {
            perror("Error al abrir el archivo de entrada");
            exit(EXIT_FAILURE);  // Salir si no se puede abrir el archivo
        }
    }

    // Llamado a la función de cálculo
    calculo(entrada, cantidad_caracteres, cantidad_lineas, &caracteres, &lineas);

    // Cierre de archivo
    fclose(entrada);

    // Llamado a la función de resultados
    resultados(lineas, caracteres-1, cantidad_lineas, cantidad_caracteres);

    // Si la salida es a un archivo, redirigir a ese archivo
    if(salida != stdout) {
        fprintf(salida, "%d %d\n", lineas, caracteres-1);
        fclose(salida);
    }

    return 0;
}