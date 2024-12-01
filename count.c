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


int main(int argc, char *argv[]) {
    // Variables para almacenar las opciones de línea de comandos
    char *archivoentrada = NULL;  // Archivo de entrada (NULL para stdin)
    char cantidad_lineas = 0;     // Bandera para contar líneas
    char cantidad_caracteres = 0; // Bandera para contar caracteres
    int caracteres = 0;           // Conteo de caracteres
    int lineas = 0;               // Conteo de líneas
    char *archivosalida = NULL;   // Archivo de salida

    // Parsear opciones de línea de comandos
    int option;
    while ((option = getopt(argc, argv, "i:o:CL")) != -1) {
        switch (option) {
            case 'i':
                archivoentrada = optarg; // Archivo de entrada
                break;
            case 'C':
                cantidad_caracteres = 1; // Contar caracteres
                break;
            case 'L':
                cantidad_lineas = 1;     // Contar líneas
                break;
            case 'o':
                archivosalida = optarg; // Archivo de salida
                break;
            default:
                fprintf(stderr, "Uso: %s [-L] [-C] [-o archivo_salida] [-i archivo_entrada]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Manejo del archivo de salida
    FILE *salida = stdout; // Por defecto, escribir en stdout
    if (archivosalida != NULL) {
        salida = fopen(archivosalida, "w"); // Abrir archivo de salida en modo escritura
        if (salida == NULL) {
            perror("Error al abrir el archivo de salida");
            exit(EXIT_FAILURE);
        }
    }

    // Manejo del archivo de entrada
    FILE *entrada = stdin; // Por defecto, leer desde stdin
    if (archivoentrada != NULL) {
        entrada = fopen(archivoentrada, "r"); // Intentar abrir el archivo de entrada
        if (entrada == NULL) {
            perror("Error al abrir el archivo de entrada");
            if (salida != stdout) fclose(salida); // Cerrar archivo de salida si está abierto
            exit(EXIT_FAILURE);
        }
    }

    // Llamado a la función de cálculo
    calculo(entrada, cantidad_caracteres, cantidad_lineas, &caracteres, &lineas);

    // Cerrar el archivo de entrada si no es stdin
    if (entrada != stdin) {
        fclose(entrada);
    }

    // Llamado a la función de resultados
    resultados(lineas, caracteres - 1, cantidad_lineas, cantidad_caracteres);

    // Si la salida es un archivo, escribir resultados y cerrarlo
    if (salida != stdout) {
        fprintf(salida, "%d:%d\n", lineas, caracteres - 1);
        fclose(salida);
    }

    return 0;
}
