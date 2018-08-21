import sys
#sys.path.append('/usr/local/lib/python2.7/dist-packages/')

from PIL import Image
import numpy as np
import cv2
import matplotlib.pyplot as plt

img = cv2.imread('test.jpg')
mser = cv2.MSER_create(_min_area=30, _max_area=500)

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
regions, boxes = mser.detectRegions(gray)

for box in boxes:
    x, y, w, h = box
    cv2.rectangle(img, (x,y),(x+w, y+h), (255, 0, 0), 1)

plt.imshow(img,'brg')
plt.show()
