from tkinter import *
from tkinter import filedialog
import numpy as np

from scanner import scanner
from solver import sudoku


class gui:

    def __init__(self):
        self.grid = [[0 for i in range(9)] for j in range(9)]
        self.digits = None
        self.err_msg = StringVar()
        self.err_msg.set("Unable to solve")

    def openfile(self):
        root.filename = filedialog.askopenfilename(initialdir=".", title="Select a file", filetypes=(
            ('png files', '*.png'), ('jpg files', '*.jpg')))
        scan = scanner(root.filename)
        scan.getDigits()
        self.digits = scan.grid
        for i in range(9):
            for j in range(9):
                if scan.grid[i][j] != 0:
                    self.grid[i][j].insert(0, int(scan.grid[i][j]))

    def solution(self):

        for i in range(9):
            for j in range(9):
                if self.grid[i][j].get() != "":
                    self.digits[i][j] = int(self.grid[i][j].get())

        slv = sudoku(self.digits)
        ifSolved = slv.solve()

        if ifSolved == True:
            for i in range(9):
                for j in range(9):
                    self.grid[i][j].insert(0, slv.arr[i][j])

        else:
            Label(root, textvariable=self.err_msg).grid(
                row=5, column=10)

    def clear(self):
        for i in range(9):
            for j in range(9):
                self.grid[i][j].delete(0, END)

        self.err_msg.set(" ")


root = Tk()
root.title("Sudoku Solver")
root.geometry("1000x620")

g = gui()

frame = LabelFrame(root)
frame.grid(row=0, column=0, padx=15, pady=15)

for i in range(3):
    for j in range(3):
        frame_temp = LabelFrame(frame, padx=10, pady=10)
        for k in range(3):
            for l in range(3):
                g.grid[i*3+k][j*3 +
                              l] = Entry(frame_temp, width=1, borderwidth=5)
                g.grid[i*3+k][j*3+l].grid(row=i*3+k,
                                          column=j*3+l, padx=5, pady=5)
        frame_temp.grid(row=i, column=j)

frame.grid(row=0, column=0, rowspan=10, columnspan=10)

Label(root, text="This is the Sudoku Solver. Using the \n'Select Image' button, select a picture \nof a sudoku. Then click the 'Solve'\n button to solve the puzzle.",
      anchor=W).grid(row=0, column=10, rowspan=4, columnspan=2, pady=15)

select_img_button = Button(root, text="Select Image",
                           padx=120, pady=5, anchor=E, command=g.openfile)
select_img_button.grid(row=6, column=10, padx=15, columnspan=2)

solve_button = Button(root, text="Solve", padx=170, pady=5, command=g.solution)
solve_button.grid(row=7, column=10, padx=30, columnspan=2)

clear_button = Button(root, text="Clear", padx=170, pady=5, command=g.clear)
clear_button.grid(row=8, column=10, padx=30, columnspan=2)

quit_button = Button(root, text="Quit", padx=180, pady=5, command=root.destroy)
quit_button.grid(row=9, column=10, padx=30, columnspan=2)

root.mainloop()
