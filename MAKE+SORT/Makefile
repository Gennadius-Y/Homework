# Makefile файлы №25 + №26
# Программа: линейный список + сортировка пузырьком

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g -std=c99
LDFLAGS =

# Целевой исполняемый файл
TARGET = prog

# Объектные файлы
OBJS = main.o list.o

# Правило по умолчанию
all: $(TARGET)

# Компоновка
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Компиляция main.o
main.o: main.c list.h
	$(CC) $(CFLAGS) -c main.c

# Компиляция list.o
list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Полная пересборка
rebuild: clean all

# Объявление "псевдоцелей"
.PHONY: all clean rebuild