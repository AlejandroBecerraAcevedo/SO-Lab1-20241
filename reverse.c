/*
    Entrega laboratorio Sistemas operativos

    Integrantes:

    Johan Seabastian Henao Cañas    
    Alejandro Becerra Acevedo
    
*/ 

#include <stdio.h>    // Para funciones de entrada/salida
#include <stdlib.h>   // Para funciones generales, como manejo de memoria dinámica
#include <string.h>   // Para funciones de manipulación de cadenas
#include <sys/stat.h> // Para obtener información de archivos, como el número de inodo

#define MAX_LINE_LENGTH 1024  // Define la longitud máxima de una línea

// Función que imprime un mensaje de error en stderr
void printError(const char *message) {
    fprintf(stderr, "%s\n", message);
}

// Función que imprime un mensaje de error en stderr con el nombre del archivo problemático
void printErrorsArg(const char *message, const char *filename) {
    fprintf(stderr, "reverse: %s '%s'\n", message, filename);
}

// Esta función invierte el orden de un arreglo de cadenas de texto
void reverseLines(char **lines, int numLines) {
    int start = 0;             // Índice inicial
    int end = numLines - 1;     // Índice final
    char *temp;

    // Ciclo para intercambiar las líneas de inicio a fin y viceversa
    while (start < end) {
        temp = lines[start];      // Almacena temporalmente la línea inicial
        lines[start] = lines[end]; // Coloca la línea final en la posición inicial
        lines[end] = temp;         // Coloca la línea inicial en la posición final
        start++;
        end--;
    }
}

int main(int argc, char *argv[]) {
    FILE *inputFile;   // Puntero al archivo de entrada
    FILE *outputFile;  // Puntero al archivo de salida

    // Verifica que el número de argumentos no exceda el límite esperado
    if (argc > 3) {
        printError("usage: reverse <input> <output>");
        exit(1);  // Sale del programa si hay demasiados argumentos
    }

    // Manejo de casos según el número de argumentos
    if (argc == 1) {
        // Si no hay argumentos, se leen datos desde stdin (entrada estándar) y se escriben en stdout (salida estándar)
        inputFile = stdin;
        outputFile = stdout;
    } else if (argc == 2) {
        // Si hay un argumento, se lee el archivo de entrada y se escribe en stdout
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {  // Verifica si el archivo de entrada no puede abrirse
            printErrorsArg("cannot open file", argv[1]);
            exit(1);
        }
        outputFile = stdout;
    } else {
        // Si hay dos argumentos, se leen datos del archivo de entrada y se escriben en el archivo de salida
        struct stat input_stat, output_stat;

        // Verifica que el archivo de entrada y salida no sean el mismo comparando sus números de inodo
        if (stat(argv[1], &input_stat) == 0 && stat(argv[2], &output_stat) == 0) {
            if (input_stat.st_ino == output_stat.st_ino) {
                printError("reverse: input and output file must differ");
                exit(1);  // Si los archivos son el mismo, se sale del programa con un error
            }
        }

        // Intenta abrir el archivo de entrada
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {  // Verifica si el archivo no pudo abrirse
            printErrorsArg("cannot open file", argv[1]);
            exit(1);
        }

        // Intenta abrir el archivo de salida
        outputFile = fopen(argv[2], "w");
        if (outputFile == NULL) {  // Verifica si el archivo de salida no puede abrirse
            printError("cannot open output file");
            exit(1);
        }

        // Verifica si los archivos de entrada y salida tienen el mismo nombre
        if (strcmp(argv[1], argv[2]) == 0) {
            printError("reverse: input and output file must differ");
            exit(1);
        }
    }

    // Almacena las líneas leídas en un arreglo dinámico
    char **lines = NULL;
    int numLines = 0;  // Contador de líneas leídas
    char line[MAX_LINE_LENGTH];  // Buffer para almacenar cada línea

    // Leer cada línea del archivo de entrada
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        // Eliminar el carácter de salto de línea del final de la cadena
        line[strcspn(line, "\n")] = '\0';

        // Reservar memoria para almacenar la nueva línea en el arreglo
        lines = realloc(lines, (numLines + 1) * sizeof(char *));
        if (lines == NULL) {  // Verificar si la reserva de memoria falló
            printError("malloc failed");
            exit(1);
        }

        // Copiar la línea leída al arreglo
        lines[numLines] = strdup(line);
        if (lines[numLines] == NULL) {  // Verificar si la duplicación de la cadena falló
            printError("malloc failed");
            exit(1);
        }
        numLines++;  // Incrementar el contador de líneas
    }

    // Invertir el orden de las líneas leídas
    reverseLines(lines, numLines);

    // Escribir las líneas en el archivo de salida en orden inverso
    for (int i = 0; i < numLines; i++) {
        fprintf(outputFile, "%s\n", lines[i]);  // Imprimir cada línea
        free(lines[i]);  // Liberar la memoria asignada para cada línea
    }
    free(lines);  // Liberar la memoria del arreglo de líneas

    // Cerrar los archivos si no se usaron stdin o stdout
    if (inputFile != stdin) fclose(inputFile);
    if (outputFile != stdout) fclose(outputFile);

    return 0;
}
