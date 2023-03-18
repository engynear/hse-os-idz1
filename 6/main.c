#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 5000

void reverse_words_order(char *str) {
    int i, j;
    char output[BUFFER_SIZE] = "";
    for (i = strlen(str) - 1; i >= 0; i--) {
        if (str[i] == ' ') {
            for (j = i + 1; j < strlen(str); j++) {
                if (str[j] == ' ') {
                    break;
                }
                strncat(output, &str[j], 1);
            }
            strncat(output, " ", 1);
        }
    }
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            break;
        }
        strncat(output, &str[i], 1);
    }

    memset(str, 0, BUFFER_SIZE);
    strcpy(str, output);
}

bool count_arguments(int argc) {
    if (argc != 3) {
        printf("Слишком %s аргументов, укажите только входной и выходной файл\n", argc < 2 ? "мало" : "много");
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];
    int size;
    char *input, *output;
    if (!count_arguments(argc)) {
        exit(-1);
    } else {
        input = argv[1];
        output = argv[2];
    }

    int pipe_one[2];
    int pipe_two[2];

    if (pipe(pipe_one) < 0) {
        printf("Ошибка при открытии пайпа\n");
        exit(-1);
    }
    if (pipe(pipe_two) < 0) {
        printf("Ошибка при открытии пайпа\n");
        exit(-1);
    }
    int process = fork();
    if (process < 0) {
        printf("Ошибка при создании процесса\n");
        exit(-1);
    } else if (process == 0) {
        if (close(pipe_one[1]) < 0) {
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
        size = read(pipe_one[0], buffer, BUFFER_SIZE);
        buffer[size] = '\0';
        if (size < 0) {
            printf("Ошибка при чтении из пайпа\n");
            exit(-1);
        }
        reverse_words_order(buffer);
        if (close(pipe_one[0]) < 0) {
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
        size = write(pipe_two[1], buffer, BUFFER_SIZE);
        if (size != BUFFER_SIZE) {
            printf("Ошибка при записи в пайп\n");
            exit(-1);
        }
        if (close(pipe_two[1]) < 0) {
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
        exit(0);
    } else {
        if (close(pipe_one[0]) < 0) {
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
        int input_file;
        if ((input_file = open(input, O_RDONLY, 0666)) < 0) {
            printf("Ошибка при открытии файла\n");
            exit(-1);
        }
        size = read(input_file, buffer, BUFFER_SIZE);
        buffer[size] = '\0';
        if (close(input_file) < 0) {
            printf("Ошибка при закрытии файла\n");
        }
        size = write(pipe_one[1], buffer, BUFFER_SIZE);
        if (size != BUFFER_SIZE) {
            printf("Ошибка при записи в пайп\n");
            exit(-1);
        }
        if (close(pipe_one[1]) < 0) {
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
    }
    while (wait(NULL) > 0) {
    }
    if (close(pipe_two[1]) < 0) {
        printf("Ошибка при закрытии пайпа\n");
        exit(-1);
    }
    size = read(pipe_two[0], buffer, BUFFER_SIZE);
    if (size < 0) {
        printf("Ошибка при чтении из пайпа\n");
        exit(-1);
    }
    if (close(pipe_two[0]) < 0) {
        printf("Ошибка при закрытии пайпа\n");
        exit(-1);
    }
    int output_file = open(output, O_WRONLY | O_CREAT, 0666);
    if (output_file < 0) {
        printf("Ошибка при открытии файла\n");
        exit(-1);
    }
    size = write(output_file, buffer, strlen(buffer));
    if (size != strlen(buffer)) {
        printf("Ошибка при записи\n");
        exit(-1);
    }
    if (close(output_file) < 0) {
        printf("Ошибка при закрытии файла\n");
    }
    return 0;
}