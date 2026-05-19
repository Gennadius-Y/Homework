#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAPACITY 10

// Создание списка
void list_create(list* l) {
    l->data = (data_type*)malloc(INIT_CAPACITY * sizeof(data_type));
    l->size = 0;
    l->capacity = INIT_CAPACITY;
}

// Проверка на пустоту
bool list_is_empty(const list* l) {
    return l->size == 0;
}

// Увеличение вместимости при необходимости
static void ensure_capacity(list* l) {
    if (l->size >= l->capacity) {
        l->capacity *= 2;
        l->data = (data_type*)realloc(l->data, l->capacity * sizeof(data_type));
    }
}

// Добавление в начало
void list_push_front(list* l, data_type d) {
    ensure_capacity(l);
    for (size_t i = l->size; i > 0; i--) {
        l->data[i] = l->data[i-1];
    }
    l->data[0] = d;
    l->size++;
}

// Добавление в конец
void list_push_back(list* l, data_type d) {
    ensure_capacity(l);
    l->data[l->size++] = d;
}

// Удаление из начала
void list_pop_front(list* l) {
    if (l->size == 0) return;
    for (size_t i = 0; i < l->size - 1; i++) {
        l->data[i] = l->data[i+1];
    }
    l->size--;
}

// Удаление из конца
void list_pop_back(list* l) {
    if (l->size > 0) {
        l->size--;
    }
}

// Печать списка
void list_print(const list* l) {
    printf("[");
    for (size_t i = 0; i < l->size; i++) {
        printf("(%d,%d)", l->data[i].key, l->data[i].value);
        if (i < l->size - 1) printf(" ");
    }
    printf("]\n");
}

// Размер списка
size_t list_size(const list* l) {
    return l->size;
}

// Вставка с сохранением упорядоченности (по ключу)
void list_insert(list* l, const data_type d) {
    ensure_capacity(l);
    size_t pos = 0;
    while (pos < l->size && l->data[pos].key < d.key) {
        pos++;
    }
    for (size_t i = l->size; i > pos; i--) {
        l->data[i] = l->data[i-1];
    }
    l->data[pos] = d;
    l->size++;
}

// Удаление по ключу
void list_erase(list* l, const key_type k) {
    size_t pos = 0;
    while (pos < l->size && l->data[pos].key != k) {
        pos++;
    }
    if (pos < l->size) {
        for (size_t i = pos; i < l->size - 1; i++) {
            l->data[i] = l->data[i+1];
        }
        l->size--;
    }
}

// Процедура №4: поиск двух соседних элементов, где первый > второй, и их перестановка
// Возвращает true, если перестановка была сделана
bool find_and_swap(list* l) {
    if (l->size < 2) return false;

    for (size_t i = 0; i < l->size - 1; i++) {
        if (l->data[i].key > l->data[i+1].key) {
            // Перестановка
            data_type temp = l->data[i];
            l->data[i] = l->data[i+1];
            l->data[i+1] = temp;
            return true;
        }
    }
    return false;
}

// Рекурсивная версия пузырьковой сортировки
static void bubble_sort_recursive(list* l, size_t n) {
    if (n <= 1) return;

    // Один проход
    for (size_t i = 0; i < n - 1; i++) {
        if (l->data[i].key > l->data[i+1].key) {
            data_type temp = l->data[i];
            l->data[i] = l->data[i+1];
            l->data[i+1] = temp;
        }
    }
    // Рекурсивный вызов для оставшейся части
    bubble_sort_recursive(l, n - 1);
}

// Обёртка для рекурсивной сортировки пузырьком
void bubble_sort(list* l) {
    if (l->size == 0) return;
    bubble_sort_recursive(l, l->size);
}