#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Node {
    char op; // Operator (+, *)
    int value; // Only used if node is a number
    struct Node *left;
    struct Node *right;
} Node;

// Skip whitespace in the input
void skipWhitespace(const char **input) {
    while (**input == ' ') (*input)++;
}

// Print an error message and exit
void error(const char *message, char unexpected) {
    printf("%s: '%c'\n", message, unexpected);
    exit(1);
}

// Parse a factor (handles numbers and parentheses)
Node* parseFactor(const char **input) {
    skipWhitespace(input);

    if (**input == '(') {
        (*input)++;
        Node *node = parseExpression(input);

        skipWhitespace(input);
        if (**input != ')') {
            error("Unbalanced parentheses", **input);
            freeAST(node);
        }
        (*input)++; // Consume ')'
        return node;
    } else if (isdigit(**input)) {
        Node *node = malloc(sizeof(Node));
        node->op = '\0';
        node->value = **input - '0';
        node->left = node->right = NULL;
        (*input)++;
        return node;
    } else {
        error("Unexpected character", **input);
    }
    return NULL; // To satisfy the compiler
}

// Parse a term (handles *)
Node* parseTerm(const char **input) {
    Node *left = parseFactor(input);

    while (1) {
        skipWhitespace(input);
        char op = **input;

        if (op == '*') {
            (*input)++;
            Node *right = parseFactor(input);
            Node *node = malloc(sizeof(Node));
            node->op = '*';
            node->left = left;
            node->right = right;
            left = node;
        } else {
            break;
        }
    }

    return left;
}

// Parse an expression (handles +)
Node* parseExpression(const char **input) {
    Node *left = parseTerm(input);

    while (1) {
        skipWhitespace(input);
        char op = **input;

        if (op == '+') {
            (*input)++;
            Node *right = parseTerm(input);
            Node *node = malloc(sizeof(Node));
            node->op = '+';
            node->left = left;
            node->right = right;
            left = node;
        } else {
            break;
        }
    }

    return left;
}

// Evaluate the AST
int evaluateAST(Node *node) {
    if (!node->op) {
        return node->value;
    }

    int leftVal = evaluateAST(node->left);
    int rightVal = evaluateAST(node->right);

    if (node->op == '+') {
        return leftVal + rightVal;
    } else if (node->op == '*') {
        return leftVal * rightVal;
    }

    return 0; // Should never reach here
}

// Free memory used by the AST
void freeAST(Node *node) {
    if (!node) return;
    freeAST(node->left);
    freeAST(node->right);
    free(node);
}

int main() {
    char input[256];
    printf("Enter an arithmetic expression: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input.\n");
        return 1;
    }

    const char *ptr = input;
    Node *ast = parseExpression(&ptr);

    // Check for leftover characters after parsing
    skipWhitespace(&ptr);
    if (*ptr != '\0' && *ptr != '\n') {
        error("Unexpected character", *ptr);
        freeAST(ast);
        return 1;
    }

    // Evaluate and print the result
    int result = evaluateAST(ast);
    printf("Result: %d\n", result);

    // Clean up
    freeAST(ast);
    return 0;
}
