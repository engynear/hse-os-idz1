## ИДЗ №1 по ОС
----------------------------------------------------------------
## Исполнитель: Миронов Павел Андреевич
## Группа: БПИ212
## Вариант 17

Разработать программу, которая меняет на обратный порядок следование слов в ASCII-строке символов.
### Файлы

В файлах с расширением **.o** хранятся собранные объектные исполняемые файлы

В файлах с расширением **.c** хранится исходный код

## Задание выполнено на 6 баллов
Выполненные условия на каждый из баллов лежат в отдельной папке под номерами от 4 до 6


## Отчет на 4 балла
### Схема
Сначала первый процесс читает данные из заданного аргументом командной строки файла. Данные передаются через неименованный канал №1 во второй процесс. 

Второй процесс обрабатывает данные с помощью функции, выполняющей условие задания и передает результат обработки через неименованный канал №2 в третий процесс.

Третий процесс выводит полученные данные из канала №2 в указанный в аргументе командной строки файл вывода.

### Тесты
Тесты находятся в папке 4/tests/. Файлы входные inputN.txt для N от 1 до 5 и выходные файлы outputN.txt соответственно.

## Отчет на 5 баллов
### Схема
Сначала первый процесс читает данные из заданного аргументом командной строки файла. Данные передаются через именованный канал one.fifo во второй процесс. 

Второй процесс обрабатывает данные с помощью функции, выполняющей условие задания и передает результат обработки через именованный two.fifo в третий процесс.

Третий процесс выводит полученные данные из two.fifo в указанный в аргументе командной строки файл вывода.

### Тесты
Тесты находятся в папке 5/tests/. Файлы входные inputN.txt для N от 1 до 5 и выходные файлы outputN.txt соответственно.

## Отчет на 6 баллов
### Схема
Сначала первый процесс читает данные из заданного аргументом командной строки файла. Данные передаются через неименованный канал №1 во второй процесс. 

Второй процесс, дочерний первому, обрабатывает данные с помощью нужной функции и возвращает результат обратно первому процессу через неименованный канал №2.

Первый процесс получает данные из второго канала и записывает их в указанный в аругменте командной строки файл вывода.

### Тесты
Тесты находятся в папке 6/tests/. Файлы входные inputN.txt для N от 1 до 5 и выходные файлы outputN.txt соответственно.