#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 5000

// Функция, которая меняет порядок слов в строке на обратный
void reverse_words_order(char *str) {
    int i, j;
    char output[BUFFER_SIZE] = "";

    // Перебираем все символы в строке, если встречаем пробел, то копируем все символы (слово) в новую строку
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

    // Копируем все символы, которые находятся до первого пробела, в новую строку
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

const char *pipe_one = "one.fifo";
const char *pipe_two = "two.fifo";

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
    mknod(pipe_one, S_IFIFO | 0666, 0);
    mknod(pipe_two, S_IFIFO | 0666, 0);

    int first;
    int second;
    int process = fork();
    if (process < 0) {
        printf("Ошибка при создании процесса\n");
        exit(-1);
    } else if (process == 0) {
        int result1 = fork();
        if (result1 < 0) {
            printf("Ошибка при создании процесса\n");
            exit(-1);
        } else if (result1 == 0) {
            first = open(pipe_two, O_RDONLY);
            if (first < 0) {
                printf("Ошибка при открытии пайпа для чтенияg\n");
                exit(-1);
            }
            size = read(first, buffer, BUFFER_SIZE);
            if (size < 0) {
                printf("Ошибка при чтении из пайпа\n");
                exit(-1);
            }
            if (close(first) < 0) {
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
                printf("Ошибка при записи в файл\n");
                exit(-1);
            }
            if (close(output_file) < 0) {
                printf("Ошибка при закрытии файла\n");
            }
            unlink(pipe_one);
            unlink(pipe_two);
        } else {
            if ((second = open(pipe_one, O_RDONLY)) < 0) {
                printf("Ошибка при открытии пайпа для чтенияg\n");
                exit(-1);
            }
            size = read(second, buffer, BUFFER_SIZE);
            if (size < 0) {
                printf("Ошибка при чтении из пайпа\n");
                exit(-1);
            }
            reverse_words_order(buffer);
            if (close(second) < 0) {
                printf("Ошибка при закрытии пайпа\n");
                exit(-1);
            }
            if ((first = open(pipe_two, O_WRONLY)) < 0) {
                printf("Ошибка при открытии пайпа для записи\n");
                exit(-1);
            }
            size = write(first, buffer, BUFFER_SIZE);
            if (size != BUFFER_SIZE) {
                printf("Ошибка при записи в пайп\n");
                exit(-1);
            }
            if (close(first) < 0) {
                printf("Ошибка при закрытии пайпа\n");
                exit(-1);
            }
        }
    } else {
        int to_read = 0;
        if ((to_read = open(input, O_RDONLY, 0666)) < 0) {
            printf("Ошибка при открытии файла\n");
            exit(-1);
        }
        size = read(to_read, buffer, BUFFER_SIZE);
        buffer[size] = '\0';
        if (close(to_read) < 0) {
            printf("Ошибка при закрытии файла\n");
        }
        if ((second = open(pipe_one, O_WRONLY)) < 0) {
            printf("Ошибка при открытии пайпа для записи\n");
            exit(-1);
        }
        size = write(second, buffer, BUFFER_SIZE);
        if (size != BUFFER_SIZE) {
            printf("Ошибка при записи в файл\n");
            exit(-1);
        }
        if (close(second) < 0) {
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
    }
    return 0;
}