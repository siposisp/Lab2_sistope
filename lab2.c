#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Función auxiliar para manejo de errores
void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s [comandos]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parsear argumentos
    // Suponemos que los comandos están separados por '|'
    int num_comandos = 1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0) num_comandos++;
    }

    // Espacio para los comandos
    char ***comandos = malloc(num_comandos * sizeof(char **));
    if (!comandos) handle_error("malloc");

    // Dividir los argumentos en comandos
    int cmd_idx = 0;
    int arg_idx = 0;
    comandos[cmd_idx] = malloc((argc + 1) * sizeof(char *));
    if (!comandos[cmd_idx]) handle_error("malloc");

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0) {
            comandos[cmd_idx][arg_idx] = NULL; // Terminar comando actual
            cmd_idx++;
            arg_idx = 0;
            comandos[cmd_idx] = malloc((argc + 1) * sizeof(char *));
            if (!comandos[cmd_idx]) handle_error("malloc");
        } else {
            comandos[cmd_idx][arg_idx++] = argv[i];
        }
    }
    comandos[cmd_idx][arg_idx] = NULL;

    // Crear pipes
    int pipes[num_comandos - 1][2];
    for (int i = 0; i < num_comandos - 1; i++) {
        if (pipe(pipes[i]) == -1) handle_error("pipe");
    }

    // Crear procesos hijos
    for (int i = 0; i < num_comandos; i++) {
        pid_t pid = fork();
        if (pid == -1) handle_error("fork");

        if (pid == 0) { // Proceso hijo
            // Redirigir stdout si no es el último comando
            if (i < num_comandos - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            // Redirigir stdin si no es el primer comando
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            // Cerrar pipes en el hijo
            for (int j = 0; j < num_comandos - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Ejecutar comando
            execvp(comandos[i][0], comandos[i]);
            handle_error("execvp");
        }
    }

    // Cerrar pipes en el padre
    for (int i = 0; i < num_comandos - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Esperar a que terminen los hijos
    for (int i = 0; i < num_comandos; i++) {
        wait(NULL);
    }

    // Liberar memoria
    for (int i = 0; i < num_comandos; i++) {
        free(comandos[i]);
    }
    free(comandos);

    return 0;
}
