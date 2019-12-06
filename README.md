# sudoku-solver-25x25
AI course project: 25x25 Sudoku Solver.

**Input** is 625 (=25x25) integers in the range [-1,24], corresponding to the game-board serialized row-wise. -1 means the corresponding cell is blank, [0-24] corresponds to the number in the cell.

**Output** is 625 integers in the range [0,24], such that all AllDiff constraints are satisfied, or “fail” if no solution.
