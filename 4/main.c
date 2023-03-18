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

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];
    int size;
    char *input, *output;

    if (!count_arguments(argc)) { // считаем количество аргументов
        exit(-1);
    } else {
        input = argv[1];
        output = argv[2];
    }

    int pipe_one[2], pipe_two[2]; // создаём пайпы

    if (pipe(pipe_one) < 0) { // создаем первый пайп
        printf("Ошибка при создании пайпа\n"); 
        exit(-1); 
    }

    if (pipe(pipe_two) < 0) { // создаем второй пайп
        printf("Ошибка при создании пайпа\n"); 
        exit(-1); 
    }

    int process1 = fork(); // создаем первый процесс

    if (process1 < 0) { // проверяем успешность операции создания дочернего процесса
        printf("Ошибка при создании процесса\n"); 
        exit(-1); 
    } else if (process1 == 0) {

        int process2 = fork(); // создаем второй процесс

        if (process2 < 0) {
            printf("Ошибка при создании процесса\n"); 
            exit(-1); 
        } else if (process2 == 0) {

            if (close(pipe_two[1]) < 0) {
                printf("Ошибка при закрытии пайпа\n"); 
                exit(-1); 
            }

            size = read(pipe_two[0], buffer, BUFFER_SIZE); // читаем данные из второго пайпа
            if (size < 0) {
                printf("Ошибка при чтении пайпа\n"); 
                exit(-1); 
            }
            if (close(pipe_two[0]) < 0) { // закрываем читающую сторону второго пайпа
                printf("Ошибка при закрытии пайпа\n"); 
                exit(-1); 
            }
            int output_file = open(output, O_WRONLY | O_CREAT, 0666); // открываем файл для записи результата
            if (output_file < 0) {
                printf("Ошибка при открытии файла\n");
                exit(-1);
            }
            size = write(output_file, buffer, strlen(buffer)); // записываем результат в ыфайл
            if (size != strlen(buffer)) {
                printf("Ошибка при записи в файл\n");
                exit(-1);
            }
            if (close(output_file) < 0) {
                printf("Ошибка при закрытии файла\n");
            }
        } else {
            if (close(pipe_one[1]) < 0) {
                printf("Ошибка при закрытии пайпа\n");
                exit(-1);
            }
            size = read(pipe_one[0], buffer, BUFFER_SIZE); // читаем данные из первого пайпа
            if (size < 0) {
                printf("Ошибка при чтении пайпа\n");
                exit(-1);
            }
            reverse_words_order(buffer); // преобразуем согласные символы в ASCII-коды
            if (close(pipe_one[0]) < 0) {
                printf("Ошибка при закрытии пайпа\n");
                exit(-1);
            }
            size = write(pipe_two[1], buffer, BUFFER_SIZE); // записываем результат второго процесса во второй пайп
            if (size != BUFFER_SIZE) {
                printf("Ошибка при записи в пайп\n");
                exit(-1);
            }
            if (close(pipe_two[1]) < 0) {
                printf("Ошибка при закрытии пайпа\n");
                exit(-1);
            }
        }
    } else {
        if (close(pipe_one[0]) < 0) {
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
        int input_file = open(input, O_RDONLY, 0666); // открываем файл для чтения
        if (input_file < 0) {
            printf("Ошибка при открытии файла\n"); , если файл не удалось открыть
            exit(-1);
        }
        size = read(input_file, buffer, BUFFER_SIZE - 1); // читаем данные из файла
        buffer[size] = '\0'; // добавляем нулевой символ в конец строки
        if (close(input_file) < 0) {
            printf("Ошибка при закрытии файла\n");
        }
        size = write(pipe_one[1], buffer, BUFFER_SIZE); // записываем данные в первый пайп
        if (size != BUFFER_SIZE) { // проверяем успешность записи
            printf("Ошибка при записи в пайп\n");
            exit(-1);
        }
        if (close(pipe_one[1]) < 0) { // закрываем записывающую сторону первого пайпа
            printf("Ошибка при закрытии пайпа\n");
            exit(-1);
        }
    }
    return 0;
}