#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "funcionescount.c"

int main(int argc, char *argv[]) {
    // Variables para almacenar las opciones de línea de comandos
    char *archivoentrada = NULL;  // Archivo de entrada (NULL para stdin)
    char cantidad_lineas = 0;     // Bandera para contar líneas
    char cantidad_caracteres = 0; // Bandera para contar caracteres
    int caracteres = 0;           // Conteo de caracteres
    int lineas = 0;               // Conteo de líneas
    char *archivosalida = NULL;   // Archivo de salida
    
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
                fprintf(stderr, "Uso: %s [-i archivo_entrada][-o archivo_salida] [-L] [-C] \n", argv[0]);
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

    // Llamado a la función de resultados, pasando el puntero de salida
    resultados(lineas, caracteres, cantidad_lineas, cantidad_caracteres, salida);
  
    // Si la salida es un archivo, escribir resultados y cerrarlo
    if (salida != stdout) {
        fclose(salida);
    }

    return 0;
}