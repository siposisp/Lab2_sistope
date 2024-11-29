#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "funcionessrep.h"

//Bloque Principal
int main(int argc, char *argv[]) {
    printf("****************INICIO DEL PROGRAMA****************\n");

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
                fprintf(stderr, "Uso: %s [-s StringsAntiguos] [-S StringsNuevos]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //Manejo de errores en el archivo
    if (archivoentrada == NULL) {
        // Si no se proporciona un archivo de entrada, pedir al usuario que ingrese el nombre
        char nombre_archivo[256];
        printf("Ingrese el nombre del archivo de entrada: ");
        
        if (fscanf(stdin, "%s", nombre_archivo) == 1) {  // Leer desde stdin
            archivoentrada = strdup(nombre_archivo); // Asignar el nombre del archivo a archivoentrada
        } else {
            fprintf(stderr, "Error al leer el nombre del archivo de entrada.\n");
            exit(EXIT_FAILURE);
        }
    }
    if(archivosalida != NULL){
        vaciar_archivo(archivosalida); //Se limpia el archivo de salida, si es que existe
    }
   
    //Llamado a la funcion procesar_archivo
    procesar_archivo(archivoentrada, archivosalida, caracterAntiguo, caracterNuevo);

    printf("*****************FIN DEL PROGRAMA*****************\n");
    return 0;
}
