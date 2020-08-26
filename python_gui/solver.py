import numpy as np

class sudoku:

    def __init__(self,grid):
        self.arr = grid

    def show(self):
        print(self.arr)

    def findc(self,c):
        if 0 <= c <=2 :
            return 1
        elif 3 <= c <= 5 :
            return 4
        else :    
            return 7

    def isValid (self,x,y,num):
        
        for i in range(0,9):

            if i == x:
                continue

            if self.arr[i][y] == num:
                return False

        for j in range(0,9):
            
            if j == y:
                continue

            if self.arr[x][j] == num:
                return False

        i = self.findc(x)
        j = self.findc(y)

        for k in range(i-1,i+2):
            for l in range(j-1,j+2):
                if k==x and l==y :
                    continue
                if self.arr[k][l] == num:
                    return False

        return True

    def emptyCell (self):
        for i in range(0,9):
            for j in range(0,9):
                if self.arr[i][j] == 0:
                    return (i,j)

        return None,None

    def solve (self):
        x,y = self.emptyCell()

        if x == None:
            return True

        for k in range(1,10):

            if self.isValid(x,y,k) == True:

                self.arr[x][y] = k

                if self.solve() == True:
                    return True
                
                self.arr[x][y] = 0

        return False

"""
s = sudoku(np.array([[0, 0, 0, 9, 0, 2, 0, 0, 0],
 [0, 4, 0, 0, 0, 0, 0, 5, 0],
 [0, 0, 2, 0, 0, 0, 3, 0, 0],
 [2, 0, 0, 0, 0, 0, 0, 0, 7],
 [0, 0, 0, 4, 5, 6, 0, 0, 0],
 [6, 0, 0, 0, 0, 0, 0, 0, 9],
 [0, 0, 7, 0, 0, 0, 8, 0, 0],
 [0, 3, 0, 0, 0, 0, 0, 4, 0],
 [0, 0, 0, 2, 0, 7, 0, 0, 0]]))     
s.solve()  
s.show()
"""