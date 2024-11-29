#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "funcionescount.c"

//Bloque Principal
int main(int argc, char *argv[]) {
    printf("****************INICIO DEL PROGRAMA****************\n");

    // Variables para almacenar las opciones de línea de comandos
    char *archivoentrada = NULL;
    char cantidad_lineas = 0;
    char cantidad_caracteres = 0;
    int caracteres = 0;
    int lineas = 0;

    int option;
    while ((option = getopt(argc, argv, "i:CL")) != -1) {
        switch (option) {
            case 'i':
                archivoentrada = optarg; // Nombre del archivo de entrada
                break;
            case 'C':
                cantidad_caracteres = 1; // Contar el número de caracteres
                break;
            case 'L':
                cantidad_lineas = 1; // Contar el número de líneas
                break;
            default:
                fprintf(stderr, "Uso: %s [-L] [-C]\n", argv[0]);
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

    // Abrir el archivo de entrada
    FILE *archivo = procesar_archivo(archivoentrada);

    // Llamado de la funcion calculo
    calculo(archivo, cantidad_caracteres, cantidad_lineas, &caracteres, &lineas);

    // Cierre de archivo
    fclose(archivo);

    // Llamado de la funcion resultados
    resultados(lineas, caracteres, cantidad_lineas, cantidad_caracteres);

    printf("*****************FIN DEL PROGRAMA*****************\n");
    return 0;
}