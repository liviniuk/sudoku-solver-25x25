#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

class SudokuSolver {
// Traverse the board recursively (from left to right, row by row).
// Check AllDiff constraint only for the current cell (in constant time - using lookup tables).
// Recursively proceed or backtrack if AllDiff violated.
public:
    static const int EMPTY = -1;
    static const int N = 25; // board size
    static const int C = sqrt(N); // subgrid size
    
    int N_BITS = (1 << N) - 1; // first N bits
    vector<int> horizontal = vector<int>(N, N_BITS);
    vector<int> vertical = vector<int>(N, N_BITS);
    vector<int> subgrid = vector<int>(N, N_BITS);
    
    vector<vector<int>> bitBoard = vector<vector<int>>(N, vector<int>(N, 0));
    vector<pair<int,int>> que; // queue for choosing the next cell (pair: <countRemainingValues, cell_index>)
    
    void flipBits(int row, int col, int bit) {
        horizontal[row] ^= bit;
        vertical[col] ^= bit;
        subgrid[C * (row / C) + col / C] ^= bit;
        bitBoard[row][col] ^= bit;
    }
    
    int countSetBits(int n) { 
        int count = 0; 
        while (n) { 
            count += n & 1; 
            n >>= 1; 
        } 
        return count; 
    } 

    int countRemainingValues(int a) {
        int row = a / N;
        int col = a % N;
        int s = C * (row / C) + col / C;
        return countSetBits(horizontal[row] & vertical[col] & subgrid[s]);
    }

    static bool comp(pair<int,int> a, pair<int,int> b) {
        return a.first >= b.first;
    }

    bool recursiveSolved(int cells_left) {
        if (cells_left == 0)
            return true; // solved
        for (int i = 0; i < cells_left; ++i) {
            que[i].first = countRemainingValues(que[i].second);
            if (que[i].first == 0) return false;
        }
        if (que[cells_left - 1].first > 1)
            partial_sort(que.begin(), que.begin() + cells_left, que.end(), comp);

        int row = que[cells_left - 1].second / N;
        int col = que[cells_left - 1].second % N;
        
        int s = C * (row / C) + col / C;
        int options = horizontal[row] & vertical[col] & subgrid[s];
        
        while (options) {
            int next = ((options - 1) ^ options) & options; // the smallest (last) bit in options is the next option to try
            options ^= next; // remove next bit from options
            flipBits(row, col, next);
            //cout << cells_left << ',' << row << ',' << col << endl;
            if (recursiveSolved(cells_left - 1))
                return true;
            flipBits(row, col, next);
        }
        
        return false;
    }
    
    bool solveSudoku(vector<vector<int>>& board) {
        que = vector<pair<int,int>>();
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (board[i][j] != EMPTY) {
                    int bit = 1 << board[i][j];
                    flipBits(i, j, bit);
                } else
                    que.push_back(pair<int,int>(25, i * N + j));
        reverse(que.begin(), que.end());
        
        bool solved = recursiveSolved(que.size());
        if (!solved) return false;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                int num = 0; 
                while ((bitBoard[i][j] >> num) > 0) 
                    num++;
                board[i][j] = num - 1;
            }
        return solved;
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
    bool solved = s.solveSudoku(board);
    if (!solved) { cout << "fail" << endl; return 0; }
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << board[i][j] << "\t";
        //cout << endl;
    }
    return 0;
}
