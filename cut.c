#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

// Entradas: Recibe un char* (una palabra) y un char (un caracter).
// Salida: Retorna un char* (una palabra con el char concatenado).
// Descripción: Agrega un caracter a una palabra Ej: "hol"+ "a" resulta "hola".
char* concatenar_caracter(const char* palabra, char caracter) {
    int longitud = strlen(palabra);  // Obtener la longitud de la cadena original

    // Crear una nueva palabra con espacio adicional para el nuevo carácter y el carácter nulo
    char* nueva_palabra = (char*) malloc((longitud + 2) * sizeof(char)); 

    // Copiar la cadena original a la nueva palabra
    strcpy(nueva_palabra, palabra);

    // Añadir el nuevo carácter al final de la nueva palabra
    nueva_palabra[longitud] = caracter;
    nueva_palabra[longitud + 1] = '\0';

    return nueva_palabra; 
}

// Entradas: Recibe un char* correspondiente a una linea del archivo.
// Salida: Retorna un arreglo de char** correspondiente a una linea del archivo.
// Descripción: transforma un linea leida del archivo a un arreglo separado por el delimitador ingresado
char** linea_a_arreglo(char* linea, char simbolo) {
    int i = 0;
    int capacidad = 1;  // Capacidad inicial del arreglo
    int cantidad = 0;   // Cantidad de palabras
    char** arreglo = NULL;  // Arreglo de cadenas
    char* palabra = NULL;   // Palabra temporal

    // Reservar memoria inicial para el arreglo
    arreglo = (char**)malloc(capacidad * sizeof(char*));
    if (arreglo == NULL) {
        printf("Error al reservar memoria.\n");
        exit(1);
    }

    // Inicializar la primera palabra
    palabra = (char*)malloc(1 * sizeof(char));
    if (palabra == NULL) {
        printf("Error al reservar memoria para la palabra.\n");
        free(arreglo);
        exit(1);
    }
    palabra[0] = '\0';  // Arreglo inicial vacío

    while (linea[i] != '\0') {
        if (linea[i] != simbolo) {
            // Concatenar el carácter actual a la palabra
            palabra = concatenar_caracter(palabra, linea[i]);
        } else {
            // Agregar la palabra al arreglo
            if (cantidad == capacidad) {
                capacidad = capacidad * 2;  // Se duplica para no llamar tantas veces a realloc
                arreglo = (char**)realloc(arreglo, capacidad * sizeof(char*));
                if (arreglo == NULL) {
                    printf("Error al redimensionar memoria.\n");
                    for (int k = 0; k < cantidad; k++) {
                        free(arreglo[k]);
                    }
                    free(palabra);  // Liberar la palabra actual
                    exit(1);
                }
            }

            arreglo[cantidad] = palabra;  // Agregar palabra al arreglo
            cantidad++;

            // Reservar memoria para la siguiente palabra
            palabra = (char*)malloc(1 * sizeof(char));
            if (palabra == NULL) {
                printf("Error al reservar memoria para la nueva palabra.\n");
                for (int k = 0; k < cantidad; k++) {
                    free(arreglo[k]);
                }
                free(arreglo);
                exit(1);
            }
            palabra[0] = '\0';  // Inicializar nueva palabra
        }
        i++;
    }


    // Se agregar la última palabra al arreglo
    if (palabra[0] != '\0') {
        if (cantidad == capacidad) {
            capacidad = capacidad + 1;
            arreglo = (char**)realloc(arreglo, capacidad * sizeof(char*));
            if (arreglo == NULL) {
                printf("Error al redimensionar memoria para la última palabra.\n");
                for (int k = 0; k < cantidad; k++) {
                    free(arreglo[k]);
                }
                free(palabra);
                exit(1);
            }
        }
        arreglo[cantidad] = palabra;
        cantidad++;
    } else {
        // Si la última palabra está vacía, liberarla
        free(palabra);
    }

    // Agregar un puntero nulo para marcar el final del arreglo
    arreglo = (char**)realloc(arreglo, (cantidad + 1) * sizeof(char*));
    if (arreglo == NULL) {
        printf("Error al redimensionar memoria para el puntero nulo.\n");
        for (int k = 0; k < cantidad; k++) {
            free(arreglo[k]);
        }
        exit(1);
    }
    arreglo[cantidad] = NULL;

    return arreglo;
}

// Entradas: Recibe un char* correspondiente al nombre del archivo de output.
// Salida: No retorna nada.
// Descripción: Verifica si el archivo de output existe, si existe lo limpia y si no existe lo crea.
void vaciar_archivo(char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo, "w");

    // Verificar si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
    }

    // Cerrar el archivo
    fclose(archivo);
}

// Entradas: Recibe un char* correspondiente al nombre del archivo de output y un char* con el contenido de la linea a guardar en el archivo de salida.
// Salida: No retorna nada.
// Descripción: Abre el archivo y agrega una linea al final del archivo.
void guardar_en_archivo(char *nombreArchivo, char* contenidoLinea){
    FILE *archivo = fopen(nombreArchivo, "a");

    // Verificar si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
    }
    
    // Escribir una nueva línea en el archivo
    fprintf(archivo, "%s", contenidoLinea);

    // Cerrar el archivo
    fclose(archivo);
}

// Entradas : Recibe un char* correspondiente a un puntero de un arreglo de caracteres con el que se trabajara.
//            Un int correspondiente al tamaño del arreglo.  
//            Un char correspondiente al caracter que se va a buscar dentro del arreglo.  
// Salida : Retorna un entero.
//          Retorna 1 si el char esta dentro del arreglo.
//          Retorna 0 si el char no esta dentro del arreglo.
// Descripción : La funcion busca un caracter dado dentro de un arreglo de caracteres, dependiendo si esta retornara un valor u otro. 
int buscar_caracter_en_arreglo(char* arreglo, int tamano, char caracter) {

    for (int i = 0; i < tamano; i++) {
        if (arreglo[i] == caracter) {
            return 1;  // El char está en el arreglo
        }
    }
    return 0;  // El char no está en el arreglo
}

// Entradas : Recibe un char* correspondiente al nombre del archivo de entrada.
//            Un int* correspondiente a un arreglo de enteros que indica los indice de las columnas que se extraeran (comando).
//            Un int correspondiente al numero de elementos en el arreglo columnas (largo del arreglo) que indica cuantas se van a extraer. 
//            Un char* correspondiente al nombre del archivo de salida.   
//            Un char correspondiente al caracter que indica como estan separadas las columnas.  
// Salida : No retorna nada
// Descripción : La funcion procesa el archivo linea a linea.
//               Lee el archivo de entrada. Verificando que se ingrese con comando correspondiente y que el nombre sea correcto (si existe).
//               Lee linea a linea el archivo almacenando en un buffer de 1024 la linea leida.   
//               Si la linea contiene el separador indicado, la funcion divide las columnas usando otra funcion.
//               Extrae las columnas seleccionadas si se cumplen los requisitos previos.
//               Dependiendo de si se proporciona el nombre del archivo de salida o no, se imprime por consola o se guarda en el archivo de salida.
//               Se libera la memoria correspondiente.
//               Se cierra el archivo y se envia mensaje de exito.
void procesar_archivo(char* filename, int* columnas, int largoarreglo, char* archivosalida, char separador){
    char linea[1024];

    // Abrir el archivo en modo lectura
    FILE* file = fopen(filename, "r");  
    if (file == NULL) { // Verificar que el archivo se haya abierto correctamente
        printf("Error al ingresar parametro flag -i. \nVerifique que el nombre sea correcto o si el archivo existe \n");
        return;
    }

    while (fgets(linea, sizeof(linea), file)) {
        // Se verifica que la linea tanga el separador
        if (buscar_caracter_en_arreglo(linea, strlen(linea), separador)) {
            //La linea de texto se transforma en un arreglo
            char ** arreglo_de_linea = linea_a_arreglo(linea, separador);

            // Calcular el largo real del arreglo de palabra para evitar debordarse
            int largo_real = 0;
            while (arreglo_de_linea[largo_real] != NULL) {
                largo_real++;
            }
            
            int i;
            int posicion;
            for(i=0; i < largoarreglo; i++){
                posicion = columnas[i];

                // Se verifica que la columna que se quiere no sea mayor a la cantidad de columnas que hay en el archivo
                if(columnas[i] <= largo_real){
                    if (archivosalida == NULL) {
                        printf("%s", arreglo_de_linea[posicion - 1]); 
                    } else {
                        guardar_en_archivo(archivosalida, arreglo_de_linea[posicion-1]);
                    }

                    if(i < largo_real - 1){
                        if((i + 1) != largoarreglo){
                            // Agregar separador hasta antes del ultimo
                            if (columnas[i+1] < largo_real){
                                if (archivosalida == NULL) {
                                    printf("%c", separador);
                                } else {
                                    char temp_separador[2] = {separador, '\0'};  // Crear un arreglo de un carácter más el terminador nulo
                                    guardar_en_archivo(archivosalida, temp_separador);
                                }
                            }
                        }
                    }
                }
                else{
                    if (archivosalida == NULL) {
                        printf("%c", separador);
                    } else {
                        char temp_separador[2] = {separador, '\0'};
                        guardar_en_archivo(archivosalida, temp_separador);
                    }

                }
            }

            // Agregar salto de línea
            if (archivosalida == NULL) {
                printf("\n");
            } else {
                guardar_en_archivo(archivosalida, "\n");
            }


        // Liberar memoria del arreglo_de_linea
        for (int j = 0; j < largo_real; j++) {
            free(arreglo_de_linea[j]);
        }
        free(arreglo_de_linea);
        }
    }

    printf("Archivo procesado correctamente\n");

}

// Entradas : Recibe un char* correspondiente a un puntero a una cadena de caracteres que contiene numeros separados por comas Ej: 10,4,3,4,5,6. 
// Salida : Retorna un entero.
//          Si es valido retorna la cantidad de numeros enteros en la cadena.
//          Si encuentra un caracter no valido imprime un mensaje de error y retorna -1.
//          Si la cadena contiene un numero que comienza con 0, imprime un error y retorna -1.
// Descripción : La funcion verifica la cadena de caracteres (por columnas la veremos) que se entrega para contar los numeros enteros positivos que estan separados por comas.
int cantidad_de_numeros(char* columnas) {
    int contador = 0;
    int i = 0;
    int en_numero = 0;  // Indicar si estamos en un número

    // Verificar si la cadena está vacía
    if (columnas == NULL || columnas[0] == '\0') {
        return 0; // Retorna 0 si la cadena es vacía
    }

   while (columnas[i] != '\0') {
        // Si el carácter es un dígito
        if (columnas[i] >= '0' && columnas[i] <= '9') {
            if (en_numero == 0) {  // Si no estamos en un número
                // Si el número es '0' y estamos empezando a contar un nuevo número
                if (columnas[i] == '0' && (i == 0 || columnas[i - 1] == ',')) {
                    printf("Error: El carácter '%c' no es válido en la cadena. Solo puede ingresar numeros enteros positivos y mayores a 0\n", columnas[i]);
                    return -1; // Retorna -1 si encontramos un 0
                }
                
                contador++;         // Contamos un número
                en_numero = 1;      // Cambiamos el indicador
            }
        } 
        // Si el carácter es una coma
        else if (columnas[i] == ',') {
            en_numero = 0; // Si es una coma, no estamos en un número
        } 
        // Si encontramos un carácter no válido
        else {
            printf("Error: El carácter '%c' no es válido en la cadena. Solo puede ingresar numeros enteros positivos y mayores a 0\n", columnas[i]);
            return -1; // Retorna -1 para indicar un error
        }
        i++;
    }

    return contador; // Retorna la cantidad total de números
}

// Entradas : Recibe un char* correspondiente a un puntero de una cadena de caracteres que contiene una lista de numeros separados Ej: ["1","2","4"].
// Salida : Retorna un puntero a un arreglo int* quue contiene los numeros convertidos. Ej: ["1","2","4"] -> [1,2,4].
// Descripción : La funcion convierte una lista de numeros que son char* en un arreglo de enteros int*.
int* arreglo_char_to_int(char* lista){

    char **arreglo_aux = linea_a_arreglo(lista, ',');
    
    int cantidad_numeros =  cantidad_de_numeros(lista);
    int* arreglo_numeros = (int*) malloc((cantidad_numeros) * sizeof(int));
    
    int i; //para avanzar en el arreglo de char
    //int j=0; //para avanzar en el arreglo de int
    for(i = 0; i < cantidad_numeros; i++)
    {
        arreglo_numeros[i] = atoi(arreglo_aux[i]);
    }
    
    for (i = 0; arreglo_aux[i] != NULL; i++) { // Liberar el arreglo de cadenas
    free(arreglo_aux[i]);
    }
    free(arreglo_aux);

    return arreglo_numeros;
}

// Entradas : Recibe un char* correspondiente al archivo de entrada y un char* correspondiente al nombre del archivo de salida.
// Salida : No retorna nada.
// Descripción : La funcion lee el archivo de entrada linea a linea.
//               Escribe su contenido en un archivo de salida especifico.
//               Si no se puede abrir el archivo imprime errores correspondientes. Finalmente muestra el mensaje de archivo procesado y cierra el archivo.
void reimprimir_archivo(char* filename, char* archivosalida) {
    char linea[1024];

    // Abrir el archivo en modo lectura
    FILE* file = fopen(filename, "r");  
    if (file == NULL) { // Verificar que el archivo se haya abierto correctamente
        printf("Error al ingresar parametro flag -i. \nVerifique que el nombre sea correcto o si el archivo existe \n");
        return;
    }

    //Leer linea por linea el archivo
    while (fgets(linea, sizeof(linea), file)) {
        guardar_en_archivo(archivosalida, linea);
    }

    printf("Archivo procesado exitosamente\n");

    fclose(file);
}

// Entradas : Recibe un char* correspondiente a un puntero de un arreglo de caracteres con el que se trabajara.
//            Un int correspondiente al tamaño del arreglo.  
//            Un int correspondiente a la cantidad de espacios seguidos son los que se buscan
// Salida : Retorna un entero.
//          Retorna 1 si los espacios están dentro del arreglo.
//          Retorna 0 si los espacios no están dentro del arreglo.
// Descripción : La funcion sirve para ver si en una cadena de caracteres hay una cadena de espacios de la cantidad que se necesita
int buscar_espacios(char* linea, int tamano, int cantidad) {
    int contador = 0;  // Contador para los espacios seguidos

    for (int i = 0; i < tamano; i++) {
        // Si encontramos un espacio, incrementamos el contador
        if (linea[i] == ' ') {
            contador++;
            // Si hemos encontrado suficientes espacios, retornamos 1
            if (contador == cantidad) {
                return 1;  // Se encontraron la cantidad de espacios requeridos
            }
        } else {
            // Reseteamos el contador si encontramos un carácter que no es un espacio
            contador = 0;
        }
    }
    return 0;  // No se encontraron suficientes espacios
}

// Entradas: Recibe un char* correspondiente a una linea del archivo.
// Salida: Retorna un arreglo de char** correspondiente a una linea del archivo.
// Descripción: transforma un linea leida del archivo a un arreglo separado por espacios
char** linea_a_arreglo_con_espacios(char* linea) {
    int i = 0;
    int capacidad = 1;  // Capacidad inicial del arreglo
    int cantidad = 0;   // Cantidad de palabras
    char** arreglo = NULL;  // Arreglo de cadenas
    char* palabra = NULL;   // Palabra temporal

    // Reservar memoria inicial para el arreglo
    arreglo = (char**)malloc(capacidad * sizeof(char*));
    if (arreglo == NULL) {
        printf("Error al reservar memoria.\n");
        exit(1);
    }

    // Inicializar la primera palabra
    palabra = (char*)malloc(1 * sizeof(char));
    if (palabra == NULL) {
        printf("Error al reservar memoria para la palabra.\n");
        free(arreglo);
        exit(1);
    }
    palabra[0] = '\0';  // Arreglo inicial vacío

    while (linea[i] != '\0') {
        // Verificar si el carácter actual es un separador
        if (linea[i] != ' ') {
            // Concatenar el carácter actual a la palabra
            palabra = concatenar_caracter(palabra, linea[i]);
        } else {
            if (strlen(palabra) > 0) {
                if (cantidad == capacidad) {
                    capacidad = capacidad * 2;  // Duplicar la capacidad
                    arreglo = (char**)realloc(arreglo, capacidad * sizeof(char*));
                    if (arreglo == NULL) {
                        printf("Error al redimensionar memoria.\n");
                        for (int k = 0; k < cantidad; k++) {
                            free(arreglo[k]);
                        }
                        free(palabra);
                        exit(1);
                    }
                }

                arreglo[cantidad] = strdup(palabra);  // Duplicar la palabra y agregar al arreglo
                cantidad++;
                free(palabra); // Liberar la memoria de la palabra anterior
                palabra = (char*)malloc(1 * sizeof(char)); // Reservar nueva memoria para la siguiente palabra
                if (palabra == NULL) {
                    printf("Error al reservar memoria para la nueva palabra.\n");
                    for (int k = 0; k < cantidad; k++) {
                        free(arreglo[k]);
                    }
                    free(arreglo);
                    exit(1);
                }
                palabra[0] = '\0';  // Inicializar nueva palabra
            }
        }
        i++;
    }

    // Para almacenar la última palabra si existe
    if (strlen(palabra) > 0) {
        if (cantidad == capacidad) {
            capacidad *= 2;  // Duplicar la capacidad si es necesario
            arreglo = (char**)realloc(arreglo, capacidad * sizeof(char*));
            if (arreglo == NULL) {
                printf("Error al redimensionar memoria.\n");
                for (int k = 0; k < cantidad; k++) {
                    free(arreglo[k]);
                }
                free(palabra);
                exit(1);
            }
        }
        arreglo[cantidad] = strdup(palabra);  
        cantidad++;
    }

    // Liberar memoria
    free(palabra);
    arreglo[cantidad] = NULL; // Finalizar el arreglo con NULL
    return arreglo; 
}

// Entradas : Recibe un char* correspondiente al nombre del archivo de entrada.
//            Un int* correspondiente a un arreglo de enteros que indica los indice de las columnas que se extraeran (comando).
//            Un int correspondiente al numero de elementos en el arreglo columnas (largo del arreglo) que indica cuantas se van a extraer. 
//            Un char* correspondiente al nombre del archivo de salida.   
//            Un char correspondiente al caracter que indica como estan separadas las columnas.  
//            Un int con la cantidad espacios que se están buscando EJ: 5 espacios para el equivalente de "     "
// Salida : No retorna nada
// Descripción : La funcion procesa el archivo linea a linea.
//               Lee el archivo de entrada. Verificando que se ingrese con comando correspondiente y que el nombre sea correcto (si existe).
//               Lee linea a linea el archivo almacenando en un buffer de 1024 la linea leida.   
//               Si la linea contiene el separador indicado, la funcion divide las columnas usando otra funcion.
//               Extrae las columnas seleccionadas si se cumplen los requisitos previos.
//               Dependiendo de si se proporciona el nombre del archivo de salida o no, se imprime por consola o se guarda en el archivo de salida.
//               Se libera la memoria correspondiente.
//               Se cierra el archivo y se envia mensaje de exito.
void procesar_archivo_con_espacios(char* filename, int* columnas, int largoarreglo, char* archivosalida, char* separador, int cantidad_de_espacios){
    char linea[1024];

    // Abrir el archivo en modo lectura
    FILE* file = fopen(filename, "r");  
    if (file == NULL) { // Verificar que el archivo se haya abierto correctamente
        printf("Error al ingresar parametro flag -i. \nVerifique que el nombre sea correcto o si el archivo existe \n");
        return;
    }
    

    while (fgets(linea, sizeof(linea), file)) {
        // Se verifica que la linea tanga el separador
        if (buscar_espacios(linea, strlen(linea), cantidad_de_espacios)) {
            //La linea de texto se transforma en un arreglo
            char ** arreglo_de_linea = linea_a_arreglo_con_espacios(linea);


            // Calcular el largo real del arreglo de palabra para evitar debordarse
            int largo_real = 0;
            while (arreglo_de_linea[largo_real] != NULL) {
                largo_real++;
            }
            
            int i;
            int posicion;
            for(i=0; i < largoarreglo; i++){
                posicion = columnas[i];

                // Se verifica que la columna que se quiere no sea mayor a la cantidad de columnas que hay en el archivo
                if(columnas[i] <= largo_real){
                    if (archivosalida == NULL) {
                        printf("%s", arreglo_de_linea[posicion - 1]); 
                    } else {
                        guardar_en_archivo(archivosalida, arreglo_de_linea[posicion-1]);
                    }

                    if(i < largo_real - 1){
                        if((i + 1) != largoarreglo){
                            // Agregar separador hasta antes del ultimp
                            if (columnas[i+1] < largo_real){
                                if (archivosalida == NULL) {
                                    printf("%s", separador);
                                } else {
                                    guardar_en_archivo(archivosalida, separador);
                                }
                            }
                        }
                    }
                }
                else{
                    if (archivosalida == NULL) {
                        printf("%s", separador);
                    } else {
                        guardar_en_archivo(archivosalida, separador);
                    }

                }
            }

            // Agregar salto de línea
            if (archivosalida == NULL) {
                printf("\n");
            } else {
                guardar_en_archivo(archivosalida, "\n");
            }


        // Liberar memoria del arreglo_de_linea
        for (int j = 0; j < largo_real; j++) {
            free(arreglo_de_linea[j]);
        }
        free(arreglo_de_linea);
        }
    }

    printf("Archivo procesado correctamente\n");

}

//Bloque Principal
int main(int argc, char *argv[])
{
    printf("****************INICIO DEL PROGRAMA****************\n\n");

    // Variables para almacenar las opciones de línea de comandos
    int option;
    char *archivoentrada = NULL;
    char *archivosalida = NULL;
    char *separador = "\t"; // Se deja la tabulación por defecto
    char *columnas = NULL;
    int cantidad_de_espacios;

    //Se utiliza geopt para leer las opciones de línea de comandos
    while ((option = getopt(argc, argv, "i:o:d:c:")) != -1) {
        switch (option) {
            case 'd':
                if (strcmp(optarg, "t") == 0) {
                    separador = "\t";
                }
                //Manejo de espacios
                else if (optarg[0] == 'b') {
                    cantidad_de_espacios = strlen(optarg);
                    separador = malloc((cantidad_de_espacios + 1) * sizeof(char));
                    memset(separador, ' ', cantidad_de_espacios);
                    separador[cantidad_de_espacios] = '\0';
                } 
                else if (strcmp(optarg, ":") == 0 || 
                         strcmp(optarg, ";") == 0 || 
                         strcmp(optarg, ",") == 0 || 
                         strcmp(optarg, " ") == 0) {
                    separador = optarg; // Separador válido
                }
                else {
                    // Mensaje de error si el separador no es válido
                    fprintf(stderr, "Error: Separador no válido. Solo se permiten los caracteres: : ; , t b bbbbbb\n\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'c':
                columnas = optarg; //Indicar la o las columnas 
                break;
            case 'i':
                archivoentrada = optarg; //Nombre del archivo de entrada
                break;
            case 'o':
                archivosalida = optarg; //Nombre del archivo de salida
                break;
            default:
                fprintf(stderr, "Uso: %s [-c columnas]\n", argv[0]);
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
        // Llamado a la funcion vaciar_archivo
        vaciar_archivo(archivosalida); //Se limpia el archivo de salida, si es que existe
    }
    
    
    if(columnas != NULL)
    {
        // Llamado a la funcion cantidad_de_numeros    
        int cantidad_numeros = cantidad_de_numeros(columnas);
        if(cantidad_numeros==-1){
            exit(1);
        }
        // Llamado a la funcion arreglo_char_to_int
        int* arreglo_numeros = arreglo_char_to_int(columnas);

        if (strcmp(separador, "\t") == 0 || 
            strcmp(separador, ":") == 0 || 
            strcmp(separador, ";") == 0 || 
            strcmp(separador, ",") == 0 || 
            strcmp(separador, " ") == 0){
            // Llamado a la función procesar_archivo
            procesar_archivo(archivoentrada, arreglo_numeros, cantidad_numeros, archivosalida, separador[0]);
        }
        else if(cantidad_de_espacios > 1){
            procesar_archivo_con_espacios(archivoentrada, arreglo_numeros, cantidad_numeros, archivosalida, separador, cantidad_de_espacios);
        }

        //procesar_archivo(archivoentrada, arreglo_numeros, cantidad_numeros, archivosalida, separador[0]);
        
        // Liberacion del arreglo
        free(arreglo_numeros);
    }
    
    else{
        // Llamado a la funcion reimprimir_archivo
        reimprimir_archivo(archivoentrada,archivosalida);
    }
    printf("\n*****************FIN DEL PROGRAMA*****************\n");
    return 0;
}