#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(int value) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (!newNode) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

TreeNode* insert(TreeNode* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    } else {
        printf("Значение %d уже есть в дереве, дубликаты не добавляются\n", value);
    }
    return root;
}

TreeNode* minValueNode(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

TreeNode* deleteNode(TreeNode* root, int value) {
    if (root == NULL) {
        printf("Узел не найден\n");
        return NULL;
    }

    if (value < root->data) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->data) {
        root->right = deleteNode(root->right, value);
    } else {
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }


        TreeNode* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

void visualize(TreeNode* root, int depth) {
    if (root == NULL) return;
    visualize(root->right, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%d\n", root->data);
    visualize(root->left, depth + 1);
}

int maxDegree(TreeNode* root) {
    if (root == NULL) return 0;

    int degree = 0;
    if (root->left != NULL) degree++;
    if (root->right != NULL) degree++;

    int leftDegree = maxDegree(root->left);
    int rightDegree = maxDegree(root->right);

    int maxChildDegree = (leftDegree > rightDegree) ? leftDegree : rightDegree;
    return (degree > maxChildDegree) ? degree : maxChildDegree;
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    TreeNode* root = NULL;
    int choice, value;

    do {
        printf("\n=== Меню ===\n");
        printf("1. Добавить узел\n");
        printf("2. Удалить узел\n");
        printf("3. Визуализировать дерево\n");
        printf("4. Определить степень дерева \n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите целое число: ");
                scanf("%d", &value);
                root = insert(root, value);
                break;
            case 2:
                printf("Введите значение для удаления: ");
                scanf("%d", &value);
                root = deleteNode(root, value);
                break;
            case 3:
                printf("Визуализация дерева (справа налево, корень слева):\n");
                visualize(root, 0);
                break;
            case 4:
                if (root == NULL) {
                    printf("Дерево пусто, степень = 0\n");
                } else {
                    int deg = maxDegree(root);
                    printf("Степень дерева (макс. число потомков узла) = %d\n", deg);
                }
                break;
            case 0:
                printf("Выход из программы\n");
                break;
            default:
                printf("Неверный выбор, попробуйте снова\n");
        }
    } while (choice != 0);

    freeTree(root);
    return 0;
}
