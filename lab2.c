#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s \"comandos\"\n", argv[0]);
        return 1;
    }

    // Crear una copia modificable de la cadena
    char *comandos = malloc(strlen(argv[1]) + 1);
    if (!comandos) {
        perror("Error al reservar memoria");
        return 1;
    }

    strcpy(comandos, argv[1]);

    // Contamos el número de tokens
    int num_tokens = 0;
    char *token = strtok(comandos, "|");
    while (token != NULL) {
        num_tokens++;
        token = strtok(NULL, "|");
    }

    // Ahora asignamos memoria para los tokens
    char **programas = malloc(sizeof(char*) * (num_tokens + 1));
    if (!programas) {
        perror("Error al reservar memoria para programas");
        return 1;
    }

    // Re-inicializamos `comandos` para tokenizar nuevamente
    strcpy(comandos, argv[1]);
    token = strtok(comandos, "|");
    int i = 0;
    while (token != NULL) {
        programas[i++] = token;
        token = strtok(NULL, "|");
    }
    programas[i] = NULL; // Finalizamos la lista de programas

    // Mostrar los programas divididos
    for (int j = 0; j < i; j++) {
        printf("Programa %d: %s\n", j + 1, programas[j]);
    }

    // Ahora manejamos el primer programa
    char *args[128];
    char *cmd = strdup(programas[1]);  // Usamos el primer comando como ejemplo
    char *arg = strtok(cmd, " ");
    int arg_index = 0;

    while (arg != NULL) {
        args[arg_index++] = arg;
        arg = strtok(NULL, " ");
    }
    args[arg_index] = NULL; // Terminar el arreglo de argumentos con NULL

    // Mostrar los argumentos del primer comando
    printf("Comando: %s\n", args[0]);
    if (args[1]) {
        printf("Argumento 1: %s\n", args[1]);
    }









    // Pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Error al crear el pipe");
        exit(1);
    }

    // Fork
    int pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(1);
    }

    if (pid == 0) {
        // Proceso hijo
        close(fd[0]); // Cerrar el extremo de lectura del pipe

        // Redirige stdout al extremo de escritura del pipe
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("Error al redirigir stdout");
            exit(1);
        }

        // Ejecutar el comando
        execvp(args[0], args);
        perror("Error al ejecutar el comando");
        exit(1);
    } else {
        // Proceso padre
        close(fd[1]); // Cerrar el extremo de escritura del pipe

        // Abrir el archivo de salida
        FILE *archivo = fopen("out.txt", "a");
        if (archivo == NULL) {
            perror("Error al abrir el archivo de salida");
            exit(1);
        }

        // Leer datos del pipe y escribirlos en el archivo
        char buffer[1024];
        ssize_t bytes_leidos;

        while ((bytes_leidos = read(fd[0], buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, bytes_leidos, archivo);
        }

        if (bytes_leidos == -1) {
            perror("Error al leer del pipe");
        }

        // Cerrar recursos
        fclose(archivo);
        close(fd[0]);

        // Esperar al proceso hijo
        wait(NULL);
    }


    return 0;
}
