import cv2
import math
import numpy as np
from knn import KNearestNeighbors


class scanner:

    def __init__(self, path, k=1):
        self.img = cv2.imread(path, 0)
        if self.img.shape[0] > 2500 or self.img.shape[1] > 2500:
            self.img = cv2.resize(
                self.img, (self.img.shape[0]//2, self.img.shape[1]//2))
        self.k = KNearestNeighbors(k)
        self.k.fit_transform()
        self.grid = np.zeros((9, 9), dtype=np.int64)

    def getNum(self, digit):
        return self.k.predict(digit)

    def preprocessing(self):

        img_blur = cv2.GaussianBlur(self.img, (3, 3), 0)

        otsu_thresh_val, _ = cv2.threshold(
            img_blur, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
        high_thresh_val = otsu_thresh_val
        lower_thresh_val = otsu_thresh_val * 0.5
        canny_output = cv2.Canny(img_blur, lower_thresh_val, high_thresh_val)

        contours, _ = cv2.findContours(
            canny_output, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        contours = sorted(contours, key=cv2.contourArea, reverse=True)[:5]

        temp = None

        for c in contours:
            peri = cv2.arcLength(c, True)
            approx = cv2.approxPolyDP(c, 0.02 * peri, True)
            if len(approx) == 4:
                temp = approx
                break

        temp = temp.reshape(4, 2)

        inputRect = np.zeros((4, 2), dtype="float32")

        s = temp.sum(axis=1)
        inputRect[0] = temp[np.argmin(s)]
        inputRect[2] = temp[np.argmax(s)]

        diff = np.diff(temp, axis=1)
        inputRect[1] = temp[np.argmin(diff)]
        inputRect[3] = temp[np.argmax(diff)]

        outputRect = np.array([[0, 0], [self.img.shape[0] - 1, 0], [self.img.shape[0] - 1,
                                                                    self.img.shape[1] - 1], [0, self.img.shape[1] - 1]], dtype="float32")

        perspectiveMatrix = cv2.getPerspectiveTransform(inputRect, outputRect)
        warp_output = cv2.warpPerspective(
            self.img, perspectiveMatrix, (self.img.shape[0], self.img.shape[1]))

        size = int(warp_output.shape[0]*warp_output.shape[1]/2188)
        if size % 2 == 0:
            size += 1

        binary_output = cv2.adaptiveThreshold(
            warp_output, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, size, 0)
        square = cv2.resize(binary_output, (900, 900))

        return square

    def getDigits(self):

        square = self.preprocessing()

        for x in range(9):
            for y in range(9):

                s = int(900*0.13)
                elm = np.zeros((s, s))

                for i in range(s):
                    for j in range(s):
                        if i + int(900*x/9) < 900 and j + int(900*y/9) < 900:
                            elm[i][j] = square[i +
                                               int(900*x/9)][j + int(900*y/9)]
                        else:
                            elm[i][j] = 0

                elm = cv2.convertScaleAbs(elm)

                contours, _ = cv2.findContours(
                    elm, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

                largest_area = 0

                for cnt in contours:
                    area = cv2.contourArea(cnt)
                    if area > largest_area:
                        largest_area = area
                        c = cnt

                bounding_rect = cv2.boundingRect(c)

                elm = elm[bounding_rect[1]+5:bounding_rect[1]+bounding_rect[3] -
                          5, bounding_rect[0]+5:bounding_rect[0]+bounding_rect[2]-5]

                fin = cv2.resize(elm, (20, 20))
                self.grid[x][y] = int(self.getNum(fin))


"""
s = scanner(
    "/home/zoh/Programming_Data/sudoku_project_attempts/python/Pics/13.jpg")

s.getDigits()

print(s.grid)
"""
