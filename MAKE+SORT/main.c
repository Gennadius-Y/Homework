#include "list.h"
#include <stdio.h>

int main() {
    list myList;
    list_create(&myList);

    printf("=== Демонстрация работы линейного списка ===\n\n");

    // Добавляем элементы
    printf("1. Добавляем элементы:\n");
    for (int i = 5; i >= 1; i--) {
        data_type d = {i * 10, i * 100};
        list_push_back(&myList, d);
        printf("   Добавлен (%d, %d)\n", d.key, d.value);
    }

    printf("\n2. Текущий список: ");
    list_print(&myList);

    // Проверяем процедуру №4
    printf("\n3. Процедура №4 (поиск и перестановка соседних):\n");
    if (find_and_swap(&myList)) {
        printf("   Перестановка выполнена! Список: ");
        list_print(&myList);
    } else {
        printf("   Перестановка не нужна (список уже упорядочен)\n");
    }

    // Добавляем ещё элементы для наглядной сортировки
    printf("\n4. Добавляем ещё элементы: (30,300) и (20,200)\n");
    data_type d1 = {30, 300};
    data_type d2 = {20, 200};
    list_push_back(&myList, d1);
    list_push_back(&myList, d2);

    printf("   Список до сортировки: ");
    list_print(&myList);

    // Сортировка пузырьком
    printf("\n5. Сортировка пузырьком (рекурсивная):\n");
    bubble_sort(&myList);
    printf("   Список после сортировки: ");
    list_print(&myList);

    // Дополнительная демонстрация других операций
    printf("\n6. Другие операции:\n");
    printf("   Размер спища: %zu\n", list_size(&myList));
    printf("   Пуст ли список? %s\n", list_is_empty(&myList) ? "да" : "нет");

    printf("\n   Удаляем первый элемент: ");
    list_pop_front(&myList);
    list_print(&myList);

    printf("   Удаляем последний элемент: ");
    list_pop_back(&myList);
    list_print(&myList);

    printf("\n=== Программа завершена ===\n");

    return 0;
}