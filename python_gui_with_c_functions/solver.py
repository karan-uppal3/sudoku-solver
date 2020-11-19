from ctypes import c_void_p, c_double, c_int, cdll
from numpy.ctypeslib import ndpointer
import numpy as np


class sudoku:

    def __init__(self, grid):
        self.arr = np.array(grid, dtype=np.float)

    def show(self):
        print(self.arr)

    def solve(self):

        lib = cdll.LoadLibrary("./c_solver.so")
        c_sum = lib.solve  # c_sum is the name of our C function
        c_sum.restype = ndpointer(dtype=c_double, shape=(9, 9))

        try:
            result = c_sum(c_void_p(self.arr.ctypes.data))
            self.arr = result
            return True
        except:
            return False
