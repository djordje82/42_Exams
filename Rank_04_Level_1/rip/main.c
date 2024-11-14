#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_solution(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ')
            putchar(str[i]);
        else
            putchar('_');  // Use underscore to represent removed parentheses
    }
    puts("");
}

void backtrack(char *str, int index, int open_rem, int close_rem, int open, char *result, int res_index) {
    if (str[index] == '\0') {
        if (open == 0) {
            result[res_index] = '\0';
            print_solution(result);
        }
        return;
    }

    if (str[index] == '(') {
        // Option to remove '(' by adding a space
        if (open_rem > 0) {
            result[res_index] = ' ';
            backtrack(str, index + 1, open_rem - 1, close_rem, open, result, res_index + 1);
        }
        // Option to keep '('
        result[res_index] = '(';
        backtrack(str, index + 1, open_rem, close_rem, open + 1, result, res_index + 1);
    } else if (str[index] == ')') {
        // Option to remove ')' by adding a space
        if (close_rem > 0) {
            result[res_index] = ' ';
            backtrack(str, index + 1, open_rem, close_rem - 1, open, result, res_index + 1);
        }
        // Option to keep ')' only if it has a matching '('
        if (open > 0) {
            result[res_index] = ')';
            backtrack(str, index + 1, open_rem, close_rem, open - 1, result, res_index + 1);
        }
    } else {
        // Copy other characters as they are
        result[res_index] = str[index];
        backtrack(str, index + 1, open_rem, close_rem, open, result, res_index + 1);
    }
}

void find_min_removals(char *str) {
    int open_rem = 0, close_rem = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '(') open_rem++;
        else if (str[i] == ')') {
            if (open_rem > 0) open_rem--;
            else close_rem++;
        }
    }

    int len = strlen(str);
    char *result = (char *)malloc(len + 1);
    if (!result) exit(1);

    backtrack(str, 0, open_rem, close_rem, 0, result, 0);
    free(result);
}

int main(int argc, char **argv) {
    if (argc != 2) return 1;
    find_min_removals(argv[1]);
    return 0;
}
