#include <vector>
#include <cmath>
#include <iostream>
#include<fstream>

using namespace std;

class SudokuSolver {
// Traverse the board recursively (from left to right, row by row).
// Check AllDiff constraint only for the current cell (in constant time - using lookup tables).
// Recursively proceed or backtrack if AllDiff violated.
private:
    static const int EMPTY = -1;
    static const int N = 25; // board size
    static const int C = sqrt(N); // subgrid size
    
    int N_BITS = (1 << N) - 1; // first N bits
    vector<int> horizontal = vector<int>(N, N_BITS);
    vector<int> vertical = vector<int>(N, N_BITS);
    vector<int> subgrid = vector<int>(N, N_BITS);
    
    vector<vector<int>> bitBoard = vector<vector<int>>(N, vector<int>(N, 0));
    
    void flipBits(int row, int col, int bit) {
        horizontal[row] ^= bit;
        vertical[col] ^= bit;
        subgrid[C * (row / C) + col / C] ^= bit;
        bitBoard[row][col] ^= bit;
    }
    
    bool recursiveSolved(int row, int col) {
        if (col == N) {
            col = 0;
            ++row;
            
            if (row == N)
                return true;
        }
        
        if (bitBoard[row][col] != 0)
            return recursiveSolved(row, col + 1);
        
        int s = C * (row / C) + col / C;
        int options = horizontal[row] & vertical[col] & subgrid[s];
        
        while (options) {
            int next = ((options - 1) ^ options) & options; // the smallest (last) bit in options is the next option to try
            options ^= next; // remove next bit from options
            flipBits(row, col, next);
            if (recursiveSolved(row, col + 1))
                return true;
            flipBits(row, col, next);
        }
        
        return false;
    }
    
public:
    void solveSudoku(vector<vector<int>>& board) {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (board[i][j] != EMPTY) {
                    int bit = 1 << board[i][j];
                    flipBits(i, j, bit);
                }
        
        recursiveSolved(0, 0);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                int num = 0; 
                while ((bitBoard[i][j] >> num) > 0) 
                    num++;
                board[i][j] = num - 1;
            }
    }
};


int main(int argc, const char *argv[]) {
    const int N = 25;
    vector<vector<int>> board = vector<vector<int>>();
    
    // Read from file
    ifstream f;
    string file_path = argv[1];
    f.open(file_path);
    if (f.is_open()) {
	int num;
        for (int i = 0; i < N; ++i) {
            board.push_back(vector<int>());
            for (int j = 0; j < N; ++j) {
	        f >> num;
                board[i].push_back(num);
            }
	}
    }
    f.close();
    
    SudokuSolver s = SudokuSolver();
    s.solveSudoku(board);
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << board[i][j] << "\t";
        cout << '\n';
    }
    return 0;
}
