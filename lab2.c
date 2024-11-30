#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
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

        // Ejecutar el programa "cut" con los argumentos
        char *argumentos[] = {"./cut", "-i", "input.txt", "-d", ":", "-c", "2,4", NULL};
        execv(argumentos[0], argumentos);

        // Si execv falla
        perror("Error al ejecutar execv");
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
