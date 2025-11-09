#include <iostream>
using namespace std;

const int N = 9;

// Check if placing `num` at grid[row][col] is valid
bool isSafe(int grid[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num) return false; // check row
        if (grid[x][col] == num) return false; // check column
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (grid[startRow + r][startCol + c] == num)
                return false; // check 3×3 box

    return true;
}

// Find next empty cell (returns true if found)
bool findEmptyCell(int grid[N][N], int &row, int &col) {
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return true;
    return false;
}

// Solve Sudoku using backtracking
bool solveSudoku(int grid[N][N]) {
    int row, col;
    if (!findEmptyCell(grid, row, col))
        return true; // solved!

    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;

            if (solveSudoku(grid))
                return true;

            grid[row][col] = 0; // backtrack
        }
    }
    return false; // no valid number found
}

// Print the Sudoku grid
void printGrid(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << grid[row][col] << " ";
            if ((col + 1) % 3 == 0) cout << " ";
        }
        cout << endl;
        if ((row + 1) % 3 == 0) cout << endl;
    }
}

int main() {
    int grid[N][N] = {
        {5,3,0, 0,7,0, 0,0,0},
        {6,0,0, 1,9,5, 0,0,0},
        {0,9,8, 0,0,0, 0,6,0},

        {8,0,0, 0,6,0, 0,0,3},
        {4,0,0, 8,0,3, 0,0,1},
        {7,0,0, 0,2,0, 0,0,6},

        {0,6,0, 0,0,0, 2,8,0},
        {0,0,0, 4,1,9, 0,0,5},
        {0,0,0, 0,8,0, 0,7,9}
    };

    cout << "Original Sudoku puzzle:\n\n";
    printGrid(grid);

    if (solveSudoku(grid)) {
        cout << "Solved Sudoku:\n\n";
        printGrid(grid);
    } else {
        cout << "No solution exists.\n";
    }

    return 0;
}
