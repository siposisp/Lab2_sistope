#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Entradas: Recibe un char* (una palabra).
// Salida: No tiene retorno, ya que es void
// Descripción: Función auxiliar para manejo de errores
void manejo_error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    // Manejo de error en entrada
    // Para que no se reciba "./lab2" sin comandos
    if (argc < 2) {
        fprintf(stderr, "Uso: %s [comandos]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parsear argumentos
    // Se considera el digito "|" como separador de los programas y se cuentan los comandos
    int cant_comandos = 1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0){
            cant_comandos++;
        } 
    }

    // Espacio en memoria para los comandos
    char ***comandos = malloc(cant_comandos * sizeof(char **));

    if (comandos == NULL) {
        manejo_error("malloc");
    }

    int num_comando = 0;             // ./lab2 [comando 1] | [comando 2] | [comando 3]
    int num_argumento = 0;           // ./lab2 ["argumento1" "argumento2" "argumento3"] | ["argumento1" "argumento2"] | ["argumento1" "argumento2" "argumento3"]

    comandos[num_comando] = malloc((argc + 1) * sizeof(char *));

    if (comandos[num_comando] == NULL) {
        manejo_error("malloc");
    }
    
    for (int i = 1; i < argc; i++) { // Se avanza en los comandos

        if (strcmp(argv[i], "|") == 0) {
            comandos[num_comando][num_argumento] = NULL; // Terminar comando actual

            num_comando++;      // Avanza al siguiente comando
            num_argumento = 0;  // Reinicia el contador para los argumentos de un comando

            comandos[num_comando] = malloc((argc + 1) * sizeof(char *));
            if (comandos[num_comando] == NULL) {
                manejo_error("malloc");
            }
        } else {
            comandos[num_comando][num_argumento++] = argv[i];  // Asignar argumento al comando
        }
    }
    comandos[num_comando][num_argumento] = NULL; // Terminar comando actual



    // Crear los pipes
    int pipes[cant_comandos - 1][2];
    for (int i = 0; i < cant_comandos - 1; i++) {
        if (pipe(pipes[i]) == -1) manejo_error("pipe");
    }

    // Crear procesos hijos
    for (int i = 0; i < cant_comandos; i++) {
        pid_t pid = fork();

        if (pid == -1){
            manejo_error("fork");
        } 

        // Proceso hijo
        if (pid == 0) { 
            // Redirigir stdout si no es el último comando
            if (i < cant_comandos - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            // Redirigir stdin si no es el primer comando
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            // Cerrar pipes en el hijo
            for (int j = 0; j < cant_comandos - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Ejecutar programa
            if(strcmp(comandos[i][0], "cut") == 0){
                execvp("./cut", comandos[i]);
            }
            if(strcmp(comandos[i][0], "srep") == 0){
                execvp("./srep", comandos[i]);
            }
            if(strcmp(comandos[i][0], "count") == 0){
                execvp("./count", comandos[i]);
            }
            else{
                execvp(comandos[i][0], comandos[i]);
            }
            
            manejo_error("execvp");
        }
    }

    // Cerrar pipes en el padre
    for (int i = 0; i < cant_comandos - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Esperar que terminen los hijos
    for (int i = 0; i < cant_comandos; i++) {
        wait(NULL);
    }

    // Liberar memoria
    for (int i = 0; i < cant_comandos; i++) {
        free(comandos[i]);
    }
    free(comandos);

    return 0;
}