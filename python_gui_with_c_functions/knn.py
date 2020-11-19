import cv2
import glob
import numpy as np

class KNearestNeighbors:

    def __init__(self,k=5):
        self.k = k
        self.imgs = []

    def distance (self,img1,img2):
        
        img1 = np.array(img1,dtype=np.float32)    
        img2 = np.array(img2,dtype=np.float32)        

        temp = (img1-img2)
        temp = np.sum(temp**2)
        """for i in range(20):
            for j in range(20):
                x = float(img1[i][j])-float(img2[i][j])
                temp += x*x"""
        
        return np.sqrt(temp)

    def load (self,files,group):
        for f in files:
            temp = cv2.imread(f,0)
            self.imgs.append({'img':temp,'group':group})
    
    def fit_transform(self):
        my_path='Data/'
        ext_path = '/*.png'

        for i in range(0,10):
            files=glob.glob(my_path+str(i)+ext_path)
            self.load(files,i)

    def predict(self,img):
        for i in range(len(self.imgs)):
            self.imgs[i]['distance'] = self.distance(self.imgs[i]['img'],img)

        self.imgs.sort(key=lambda x: x['distance'])

        freq = [0,0,0,0,0,0,0,0,0,0]
        freq[ self.imgs[0]['group'] ] += 1
        first_group = self.imgs[0]['group']

        for i in range(1,self.k):
            freq[ self.imgs[i]['group'] ] += 1

        m = max(freq)
        temp = [i for i, j in enumerate(freq) if j == m]
        if  len(temp) > 1 and freq[first_group] == m:
            return first_group
        else:
            return int(freq.index(m))

"""
classifier = KNearestNeighbors(1)

classifier.fit_transform()

img = cv2.imread('/home/zoh/Programming_Data/Sudoku/2_47.png',0)

print(classifier.predict(img))
"""