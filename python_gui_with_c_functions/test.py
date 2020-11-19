from ctypes import c_void_p, c_double, c_int, cdll
from numpy.ctypeslib import ndpointer
import numpy as np
import time

matrix = np.array([[5, 3, 0, 0, 7, 0, 0, 0, 0],
                   [6, 0, 0, 1, 9, 5, 0, 0, 0],
                   [0, 9, 8, 0, 0, 0, 0, 6, 0],
                   [8, 0, 0, 0, 6, 0, 0, 0, 3],
                   [4, 0, 0, 8, 0, 3, 0, 0, 1],
                   [7, 0, 0, 0, 2, 0, 0, 0, 6],
                   [0, 6, 0, 0, 0, 0, 2, 8, 0],
                   [0, 0, 0, 4, 1, 9, 0, 0, 5],
                   [0, 0, 0, 0, 8, 0, 0, 7, 9]], dtype=np.float)

# load the compiled C library
lib = cdll.LoadLibrary("./test.so")
c_sum = lib.solve  # c_sum is the name of our C function
c_sum.restype = ndpointer(dtype=c_double, shape=(9, 9))

result = c_sum(c_void_p(matrix.ctypes.data))

print(result)
