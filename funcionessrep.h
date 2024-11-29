#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

// Entradas : Recibe los comandos de entrada
// Salida : no retorna nada
// Descripción : Recibe los comandos de entrada, [-i archivoentrada] [o- archivosalida][-s] [-S] para realizar la lectura de comandos.
void opciones(int argc, char *argv[], char **archivoentrada, char **archivosalida,char **caracterAntiguo, char **caracterNuevo);

// Entradas: Recibe un char* correspondiente a la linea de texto donde se buscara el reemplazo,
//           Un char* correspondiente a la cadena de caracteres dentro de la linea en donde se buscara.
//           Un int correspondiente al indice de la linea en donde empieza a buscar patrones.
// Salida: Retorna un entero. 
//         Retorna 1 si encontro la subcadena dentro de la linea indicada.
//         Retorna 0 si no se encontro lo antes mencionado.
// Descripción: Esta funcion comprueba si dentro de una linea se encuentra una cadena de caracteres especifica para trabajar, dado un indice especifico.
int buscar_cadena_caracteres(const char* linea, const char* subcadena, int indice);

// Entradas: Recibe un char* correspondiente a la linea de texto donde se buscara el reemplazo,
//           Un char* correspondiente al caracterAntiguo que sera la cadena que se reemplazara y
//           Un char* correspondiente al caracterNuevo que sera la cadena nueva que se reemplazara por la anterior.
// Salida: Retorna un char* que es la nueva linea generada tras el reemplazo entre los caracteres.
// Descripción: Busca en las lineas de texto todas la ocaciones que aparezca el caracter solicitado para realizar un cambio,
//              representando con el caracterAntiguo y las reemplaza por la cadena carecteres nueva representado por caracterNuevo.
//              Este proceso lo realiza linea a linea hasta reemplazar todos los caracteres involucrados.
char* reemplazar_caracter(char *linea, const char* caracterAntiguo, const char* caracterNuevo);

// Entradas: Recibe un char* correspondiente al nombre del archivo de output
// Salida: Retorna un elemento de tipo FILE correspondiente al archivo de salida
// Descripción: Verifica si el archivo de output existe, si existe lo limpia y si no existe lo crea y lo deja abierto
FILE* vaciar_archivo(char *nombreArchivo);

// Entradas: Recibe un char* correspondiente al nombre del archivo de input, un char* correspondiente al nombre del archivo output,
//           un char* correspondiente al caracter original a cambiar y un char* correspondiente al caracter nuevo a escribir.
// Salida: No retorna nada
// Descripción: Verifica si el archivo de entrada existe y se abre correctamente.
//              Luego comprueba si se ha escrito el nombre del archivo de salida por consola y realiza la apertura correspondiente. En caso de que no se escriba se utilizara la consola en todo el proceso.
//              Lee linea a linea el archivo de entrada (input.txt de ejemplo).
//              Llama a la funcion para reemplazar una cadena de caracteres actuales del archivo por otra cadena nueva, tomando en cuenta los valores por comando.
//              Termina la lectura y escribe el resultado en el archivo de salida o por consola (stdout dependiendo de si no se proporciono nombre). Mientras mantiene un contador.
//              Se cierran los archivos e imprime la cantidad de lineas que se leyeron por consola como feedback.
void procesar_archivo(char* filename, char* output_filename, char* caracterAntiguo, char* caracterNuevo);

