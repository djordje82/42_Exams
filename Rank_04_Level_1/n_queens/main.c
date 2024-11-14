#include <stdio.h>
#include <stdlib.h>

int leftDiagonal[30] = { 0 };
int rightDiagonal[30] = { 0 };
int column[30] = { 0 };

void printSolution(int** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf(" %s ", board[i][j] == 1 ? "Q" : ".");
        printf("\n");
    }
}

int solveNQueensUtil(int** board, int col, int n) {
    if (col >= n)
        return 1;

    for (int row = 0; row < n; row++) {
        if ((leftDiagonal[row - col + n - 1] != 1 && rightDiagonal[row + col] != 1) && column[row] != 1) {
            board[row][col] = 1;
            leftDiagonal[row - col + n - 1] = rightDiagonal[row + col] = column[row] = 1;

            if (solveNQueensUtil(board, col + 1, n))
                return 1;

            board[row][col] = 0;
            leftDiagonal[row - col + n - 1] = rightDiagonal[row + col] = column[row] = 0;
        }
    }

    return 0;
}

int solveNQueens(int n) {
    int** board = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        board[i] = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = 0;

    if (solveNQueensUtil(board, 0, n) == 0) {
        printf("Solution does not exist");
        return 0;
    }

    printSolution(board, n);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Usage: ./n_queens <number>");
        return 1;
    }
    solveNQueens(atoi(argv[1]));
    return 0;
}