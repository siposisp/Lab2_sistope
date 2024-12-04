#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "funcionessrep.h"

//Bloque Principal
int main(int argc, char *argv[]) {
    // Variables para almacenar las opciones de línea de comandos
    char *archivoentrada = NULL;
    char *archivosalida = NULL;
    char *caracterAntiguo = NULL;
    char *caracterNuevo = NULL;

    int option;
   while ((option = getopt(argc, argv, "i:o:s:S:")) != -1) {
        switch (option) {
            case 's':
                caracterAntiguo = optarg;  // Strings antiguos a buscar
                break;
            case 'S':
                caracterNuevo = optarg;  //  Strings nuevos para reemplazar
                break;
            case 'i':
                archivoentrada = optarg;  // Nombre del archivo de entrada
                break;
            case 'o':
                archivosalida = optarg;  // Nombre del archivo de salida
                break;
            default:
                fprintf(stderr, "Uso: %s [-i archivoentrada] [-o archivosalida][-s StringsAntiguos] [-S StringsNuevos]\n", argv[0]);
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
    
    // Procesar archivo usando el flujo adecuado (archivo o stdin) 
    procesar_archivo_ajustada(entrada, salida, caracterAntiguo, caracterNuevo);

    // Cerrar los archivos si no son stdin/stdout
    if (entrada != stdin) {
         fclose(entrada);
     } 
    if (salida != stdout){
         fclose(salida);
    }
    return 0;
}