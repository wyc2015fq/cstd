import sys
sys.path.append('../12net')
import numpy as np
import cv2
import os
import numpy.random as npr
from utils import *


anno_file = 'E:/data/ew_id/list1.txt'
root = 'E:/data/ew_id/mtcnn/'
im_dir = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/"
pos_save_dir = root+"12/positive"
part_save_dir = root+"12/part"
neg_save_dir = root+'12/negative'
save_dir = root+"12"
mkdir(root)
mkdir(save_dir)
mkdir(neg_save_dir)
mkdir(part_save_dir)
mkdir(pos_save_dir)
f1 = []
f2 = []
f3 = []
with open(anno_file, 'r') as f:
    annotations = f.readlines()
annotations = annotations[0:1]
num = len(annotations)
print("%d pics in total" % num)
p_idx = 0 # positive
n_idx = 0 # negative
d_idx = 0 # dont care

outsize = (12,12)
outsize = (48,48)
max_num = 10
ex_r = 0.2

for annotation in annotations:
    annotation = annotation.strip().split(' ')
    im_path = annotation[0]
    s = annotation[1:]
    pts = []
    for i in range(len(s)//2):
        pts.append((float(s[i*2]), float(s[i*2+1])) )
    #bbox = map(float, annotation[1:])
    if pts[0][0] <0:
        continue

    bbox = bound_box_ex(pts, ex_r)
    #print(im_path, bbox)
    boxes = np.array(bbox, dtype=np.float32).reshape(-1, 4)
    img_org = cv2.imread(os.path.join(im_dir, im_path))
    if img_org is None:
        continue

    #imshow('img', img, bbox,pts)
    na = 10
    for i in range(na):
        #a = npr.randint(0,360)
        a = i*360/na
        img,pts2 = rotate(img_org, a, 1, pts)
        bbox2 = bound_box_ex(pts_ex(pts2), ex_r)
        #imshow('im2', im2, bbox2,pts2)    continue
        height, width, channel = img.shape

        #imshow("adsf", img, bbox)
        #continue
        neg_num = 0

        box = [-1,-1,-1,-1]
        #n_idx = save_rand_image(neg_save_dir,img,box,boxes,0,0,0.3,n_idx,max_num,outsize,f2)

        for box in boxes:
            # box (x_left, y_top, x_right, y_bottom)
            x1, y1, x2, y2 = box
            w = x2 - x1 + 1
            h = y2 - y1 + 1

            # ignore small faces
            # in case the ground truth boxes of small faces are not accurate
            if max(w, h) < 40 or x1 < 0 or y1 < 0:
                continue

            n_idx = save_rand_image(neg_save_dir,img,box,boxes,0,0,0.3,n_idx,max_num,outsize,f2)
            p_idx = save_rand_image(pos_save_dir,img,box,boxes,0,0.65,1,p_idx,max_num,outsize,f1)
            d_idx = save_rand_image(part_save_dir,img,box,boxes,0,0.4,0.65,d_idx,max_num,outsize,f3)
            idx = p_idx + d_idx + n_idx
            print("%s images done, pos: %s part: %s neg: %s"%(idx, p_idx, d_idx, n_idx))


save_list(os.path.join(save_dir, 'pos_12.txt'), f1)
save_list(os.path.join(save_dir, 'neg_12.txt'), f2)
save_list(os.path.join(save_dir, 'part_12.txt'), f3)
