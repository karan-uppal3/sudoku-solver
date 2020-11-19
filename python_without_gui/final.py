from scanner import scanner
from solver import sudoku

scan = scanner(
    "/home/zoh/Programming_Data/Projects/Sudoku_Solver/Pics/Train_data/4.jpg")
scan.getDigits()

grid = scan.grid

slv = sudoku(grid)

ifSolved = slv.solve()

if ifSolved == True:
    slv.show()
else:
    print("Couldn't Solve")
