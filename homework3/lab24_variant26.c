#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR 256

typedef enum {
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_OPERATOR
} NodeType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW
} Operator;

typedef struct Node {
    NodeType type;
    union {
        double number;
        char variable;
        struct {
            Operator op;
            struct Node* left;
            struct Node* right;
        };
    };
} Node;

typedef struct {
    Node** items;
    int top;
    int capacity;
} Stack;

typedef struct {
    char* items;
    int top;
    int capacity;
} CharStack;

Stack* create_stack(int capacity) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->items = (Node**)malloc(sizeof(Node*) * capacity);
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void push(Stack* s, Node* node) {
    s->items[++s->top] = node;
}

Node* pop(Stack* s) {
    return s->items[s->top--];
}

int is_empty(Stack* s) {
    return s->top == -1;
}

CharStack* create_char_stack(int capacity) {
    CharStack* s = (CharStack*)malloc(sizeof(CharStack));
    s->items = (char*)malloc(sizeof(char) * capacity);
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void push_char(CharStack* s, char c) {
    s->items[++s->top] = c;
}

char pop_char(CharStack* s) {
    return s->items[s->top--];
}

char peek_char(CharStack* s) {
    return s->items[s->top];
}

int is_empty_char(CharStack* s) {
    return s->top == -1;
}

Node* create_number_node(double value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_NUMBER;
    node->number = value;
    return node;
}

Node* create_variable_node(char var) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_VARIABLE;
    node->variable = var;
    return node;
}

Node* create_operator_node(Operator op, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_OPERATOR;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

int get_priority(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

int get_priority_from_node(Node* node) {
    if (node->type != NODE_OPERATOR) return 4;
    switch (node->op) {
        case OP_ADD: case OP_SUB: return 1;
        case OP_MUL: case OP_DIV: return 2;
        case OP_POW: return 3;
        default: return 0;
    }
}

void apply_operator(Stack* stack, char op) {
    Node* right = pop(stack);
    Node* left = pop(stack);
    Operator oper;
    
    switch (op) {
        case '+': oper = OP_ADD; break;
        case '-': oper = OP_SUB; break;
        case '*': oper = OP_MUL; break;
        case '/': oper = OP_DIV; break;
        case '^': oper = OP_POW; break;
        default: return;
    }
    
    push(stack, create_operator_node(oper, left, right));
}

Node* parse_expression(const char* expr) {
    Stack* output = create_stack(strlen(expr) * 2);
    CharStack* operators = create_char_stack(strlen(expr) * 2);
    
    int i = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) {
            i++;
            continue;
        }
        
        if (isdigit(expr[i]) || expr[i] == '.') {
            char num_str[32];
            int j = 0;
            while (isdigit(expr[i]) || expr[i] == '.') {
                num_str[j++] = expr[i++];
            }
            num_str[j] = '\0';
            push(output, create_number_node(atof(num_str)));
            continue;
        }
        
        if (isalpha(expr[i])) {
            push(output, create_variable_node(expr[i]));
            i++;
            continue;
        }
        
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '^') {
            char op = expr[i];
            while (!is_empty_char(operators) && peek_char(operators) != '(') {
                char top_op = peek_char(operators);
                if (get_priority(top_op) >= get_priority(op)) {
                    if (get_priority(top_op) == get_priority(op) && op == '^') {
                        break;
                    }
                    apply_operator(output, pop_char(operators));
                } else {
                    break;
                }
            }
            push_char(operators, op);
            i++;
            continue;
        }
        
        if (expr[i] == '(') {
            push_char(operators, '(');
            i++;
            continue;
        }
        
        if (expr[i] == ')') {
            while (!is_empty_char(operators) && peek_char(operators) != '(') {
                apply_operator(output, pop_char(operators));
            }
            if (!is_empty_char(operators) && peek_char(operators) == '(') {
                pop_char(operators);
            }
            i++;
            continue;
        }
        
        i++;
    }
    
    while (!is_empty_char(operators)) {
        apply_operator(output, pop_char(operators));
    }
    
    Node* result = pop(output);
    
    free(output->items);
    free(output);
    free(operators->items);
    free(operators);
    
    return result;
}

int is_pow_with_subtraction(Node* node, Node** base, Node** left_sub, Node** right_sub) {
    if (node->type != NODE_OPERATOR || node->op != OP_POW) {
        return 0;
    }
    
    Node* exponent = node->right;
    if (exponent->type != NODE_OPERATOR || exponent->op != OP_SUB) {
        return 0;
    }
    
    *base = node->left;
    *left_sub = exponent->left;
    *right_sub = exponent->right;
    
    return 1;
}

Node* copy_node(Node* node) {
    if (node == NULL) return NULL;
    
    if (node->type == NODE_NUMBER) {
        return create_number_node(node->number);
    } else if (node->type == NODE_VARIABLE) {
        return create_variable_node(node->variable);
    } else {
        return create_operator_node(node->op, 
                                    copy_node(node->left), 
                                    copy_node(node->right));
    }
}

Node* transform_pow_subtraction(Node* node) {
    if (node == NULL) return NULL;
    
    if (node->type == NODE_OPERATOR) {
        node->left = transform_pow_subtraction(node->left);
        node->right = transform_pow_subtraction(node->right);
        
        Node* base;
        Node* left_sub;
        Node* right_sub;
        
        if (is_pow_with_subtraction(node, &base, &left_sub, &right_sub)) {
            Node* pow1 = create_operator_node(OP_POW, copy_node(base), copy_node(left_sub));
            Node* pow2 = create_operator_node(OP_POW, copy_node(base), copy_node(right_sub));
            Node* result = create_operator_node(OP_DIV, pow1, pow2);
            
            free(node);
            return result;
        }
    }
    
    return node;
}

void print_tree(Node* node, int depth, const char* prefix) {
    if (node == NULL) return;
    
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    switch (node->type) {
        case NODE_NUMBER:
            printf("%s%.2f\n", prefix, node->number);
            break;
        case NODE_VARIABLE:
            printf("%s%c\n", prefix, node->variable);
            break;
        case NODE_OPERATOR:
            {
                char op_char;
                switch (node->op) {
                    case OP_ADD: op_char = '+'; break;
                    case OP_SUB: op_char = '-'; break;
                    case OP_MUL: op_char = '*'; break;
                    case OP_DIV: op_char = '/'; break;
                    case OP_POW: op_char = '^'; break;
                }
                printf("%s%c\n", prefix, op_char);
            }
            print_tree(node->left, depth + 1, "L:");
            print_tree(node->right, depth + 1, "R:");
            break;
    }
}

void tree_to_string(Node* node, char* buffer, int* pos) {
    if (node == NULL) return;
    
    if (node->type == NODE_NUMBER) {
        char num[32];
        if (node->number == (int)node->number) {
            sprintf(num, "%d", (int)node->number);
        } else {
            sprintf(num, "%.2f", node->number);
            char* dot = strchr(num, '.');
            if (dot) {
                char* end = num + strlen(num) - 1;
                while (end > dot && *end == '0') end--;
                if (end == dot) end--;
                *(end + 1) = '\0';
            }
        }
        strcpy(buffer + *pos, num);
        *pos += strlen(num);
    } else if (node->type == NODE_VARIABLE) {
        buffer[(*pos)++] = node->variable;
    } else {
        int need_paren_left = 0, need_paren_right = 0;
        
        if (node->left->type == NODE_OPERATOR && 
            get_priority_from_node(node->left) < get_priority_from_node(node)) {
            need_paren_left = 1;
        }
        
        if (node->right->type == NODE_OPERATOR) {
            int right_priority = get_priority_from_node(node->right);
            int op_priority = get_priority_from_node(node);
            if (right_priority < op_priority ||
                (right_priority == op_priority && (node->op == OP_SUB || node->op == OP_DIV))) {
                need_paren_right = 1;
            }
        }
        
        if (need_paren_left) buffer[(*pos)++] = '(';
        tree_to_string(node->left, buffer, pos);
        if (need_paren_left) buffer[(*pos)++] = ')';
        
        char op_char;
        switch (node->op) {
            case OP_ADD: op_char = '+'; break;
            case OP_SUB: op_char = '-'; break;
            case OP_MUL: op_char = '*'; break;
            case OP_DIV: op_char = '/'; break;
            case OP_POW: op_char = '^'; break;
        }
        buffer[(*pos)++] = op_char;
        
        if (need_paren_right) buffer[(*pos)++] = '(';
        tree_to_string(node->right, buffer, pos);
        if (need_paren_right) buffer[(*pos)++] = ')';
    }
}

void free_tree(Node* node) {
    if (node == NULL) return;
    if (node->type == NODE_OPERATOR) {
        free_tree(node->left);
        free_tree(node->right);
    }
    free(node);
}

int main() {
    char expr[MAX_EXPR];
    
    printf("Enter expression: ");
    fflush(stdout);
    
    if (fgets(expr, MAX_EXPR, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    expr[strcspn(expr, "\n")] = '\0';
    
    if (strlen(expr) == 0) {
        printf("Empty expression\n");
        return 1;
    }
    
    printf("\nOriginal expression: %s\n", expr);
    
    Node* root = parse_expression(expr);
    if (root == NULL) {
        printf("Parse error\n");
        return 1;
    }
    
    printf("\nTree BEFORE transformation:\n");
    print_tree(root, 0, "");
    
    Node* transformed = transform_pow_subtraction(root);
    
    printf("\nTree AFTER transformation:\n");
    print_tree(transformed, 0, "");
    
    char result_str[MAX_EXPR * 2] = {0};
    int pos = 0;
    tree_to_string(transformed, result_str, &pos);
    printf("\nResult: %s\n", result_str);
    
    free_tree(transformed);
    
    return 0;
}
