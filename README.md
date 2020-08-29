# Sudoku Solver

Given an image of a sudoku grid, it performs following steps:
1. Gaussian Blur to remove noise
2. Canny edge detection using Otsu threshold values
3. Extraction of largest contour
4. Perspective transform (homography) to get top view
5. Adaptive thresholding to get binary image
6. Extraction of each cell of 9x9 grid
7. Using KNN to perform OCR
8. Solving the sudoku using backtracking

## C++
Requirements:
```bash
OpenCV >= 4.2.0
```
### Without GUI
Instruction on how to use:
```bash
make final
./final
```
Note: the GUI version uses GTK+2.0 which is no longer in use

## Python
Requirements:
```bash
pip install opencv-python
pip install tkinter
```
Run ```gui.py``` / ```final.py``` to launch with / without GUI respectively.
