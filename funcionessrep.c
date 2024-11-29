#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "funcionessrep.h"

// Entradas: Recibe un char* correspondiente a la linea de texto donde se buscara el reemplazo,
//           Un char* correspondiente a la cadena de caracteres dentro de la linea en donde se buscara.
//           Un int correspondiente al indice de la linea en donde empieza a buscar patrones.
// Salida: Retorna un entero. 
//         Retorna 1 si encontro la subcadena dentro de la linea indicada.
//         Retorna 0 si no se encontro lo antes mencionado.
// Descripción: Esta funcion comprueba si dentro de una linea se encuentra una cadena de caracteres especifica para trabajar, dado un indice especifico.
int buscar_cadena_caracteres(const char* linea, const char* subcadena, int indice) {
    int i = 0;
    while (subcadena[i] != '\0') {
        if (linea[indice + i] != subcadena[i]) {
            return 0;  // No coincide
        }
        i++;
    }
    return 1;  // Se encontró la subcadena
}

// Entradas: Recibe un char* correspondiente a la linea de texto donde se buscara el reemplazo,
//           Un char* correspondiente al caracterAntiguo que sera la cadena que se reemplazara y
//           Un char* correspondiente al caracterNuevo que sera la cadena nueva que se reemplazara por la anterior.
// Salida: Retorna un char* que es la nueva linea generada tras el reemplazo entre los caracteres.
// Descripción: Busca en las lineas de texto todas la ocaciones que aparezca el caracter solicitado para realizar un cambio,
//              representando con el caracterAntiguo y las reemplaza por la cadena carecteres nueva representado por caracterNuevo.
//              Este proceso lo realiza linea a linea hasta reemplazar todos los caracteres involucrados.
char* reemplazar_caracter(char *linea, const char* caracterAntiguo, const char* caracterNuevo) {
    static char nuevaLinea[1024];  // Buffer para la nueva línea
    int i = 0, j = 0, k;
    int longitudAntigua = 0;
    int longitudNueva = 0;

    // Calcular la longitud del antiguo caracter
    while (caracterAntiguo[longitudAntigua] != '\0') {
        longitudAntigua++;
    }

    // Calcular la longitud del nuevo caracter
    while (caracterNuevo[longitudNueva] != '\0') {
        longitudNueva++;
    }

    // Procesar la línea para buscar y reemplazar la subcadena
    while (linea[i] != '\0') {
        if (buscar_cadena_caracteres(linea, caracterAntiguo, i)) {
            // Copiar la subcadena nueva en la nueva línea
            for (k = 0; k < longitudNueva; k++) {
                nuevaLinea[j++] = caracterNuevo[k];
            }
            // Saltar la longitud del caracter antiguo en la línea original
            i += longitudAntigua;
        } else {
            // Si no se encuentran los caracteres, copiar el carácter original
            nuevaLinea[j++] = linea[i++];
        }
    }
    nuevaLinea[j] = '\0'; 
    return nuevaLinea;
}

// Entradas: Recibe un char* correspondiente al nombre del archivo de output
// Salida: Retorna un elemento de tipo FILE correspondiente al archivo de salida
// Descripción: Verifica si el archivo de output existe, si existe lo limpia y si no existe lo crea y lo deja abierto
FILE* vaciar_archivo(char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo, "w");

    // Verificar si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
    }

    return archivo;
}

// Entradas: Recibe un char* correspondiente al nombre del archivo de input, un char* correspondiente al nombre del archivo output,
//           un char* correspondiente al caracter original a cambiar y un char* correspondiente al caracter nuevo a escribir.
// Salida: No retorna nada
// Descripción: Verifica si el archivo de entrada existe y se abre correctamente.
//              Luego comprueba si se ha escrito el nombre del archivo de salida por consola y realiza la apertura correspondiente. En caso de que no se escriba se utilizara la consola en todo el proceso.
//              Lee linea a linea el archivo de entrada (input.txt de ejemplo).
//              Llama a la funcion para reemplazar una cadena de caracteres actuales del archivo por otra cadena nueva, tomando en cuenta los valores por comando.
//              Termina la lectura y escribe el resultado en el archivo de salida o por consola (stdout dependiendo de si no se proporciono nombre). Mientras mantiene un contador.
//              Se cierran los archivos e imprime la cantidad de lineas que se leyeron por consola como feedback.
void procesar_archivo(char* filename, char* output_filename, char* caracterAntiguo, char* caracterNuevo) {
    int contador = 0;
    char linea[1024];

    // Abrir el archivo en modo lectura
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo de entrada.\n");
        return;
    }
    // Se escribe en el archivo correspondiente siempre cuando no sea Null su nombre por comando
    FILE* output_file;
    if (output_filename != NULL) {
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            printf("Error al abrir el archivo de salida.\n");
            fclose(file);
            return;
        }
    } else {
        output_file = stdout;  // Si no se proporciona archivo, se usa la consola.
    }
    // Leer línea por línea el archivo de entrada
    while (fgets(linea, sizeof(linea), file)) {
        char *linea_nueva = reemplazar_caracter(linea, caracterAntiguo, caracterNuevo);
        fprintf(output_file, "%s", linea_nueva);  
        contador++; // Contador para la cantidad de lineas leidas.
    }   
    // Cerrar los archivos si es necesario
    fclose(file);
    if (output_file != stdout) {
        fclose(output_file);
    }
    // Feedback por consola, indica si el archivo se proceso y hasta donde se proceso (contador).
    printf("Archivo procesado....\nSe escribieron %d lineas.\n", contador);
}


