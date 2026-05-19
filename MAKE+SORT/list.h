#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Типы для ключа и значения (для примера - целые числа)
typedef int key_type;
typedef int value_type;

// Структура элемента данных
typedef struct {
    key_type key;
    value_type value;
} data_type;

// Структура линейного списка на массиве
typedef struct {
    data_type* data;    // массив элементов
    size_t size;        // текущий размер
    size_t capacity;    // вместимость
} list;

// Базовые операции (по заданию из лабы №26)
void list_create(list* l);
bool list_is_empty(const list* l);
void list_push_front(list* l, data_type d);
void list_push_back(list* l, data_type d);
void list_pop_front(list* l);
void list_pop_back(list* l);
void list_print(const list* l);
size_t list_size(const list* l);
void list_insert(list* l, const data_type d);
void list_erase(list* l, const key_type k);

// Сортировка пузырьком (метод №4)
void bubble_sort(list* l);

// Процедура №4: поиск двух соседних элементов, где первый > второй, и их перестановка
bool find_and_swap(list* l);

#endif