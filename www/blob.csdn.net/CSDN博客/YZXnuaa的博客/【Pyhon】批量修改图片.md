# 【Pyhon】批量修改图片 - YZXnuaa的博客 - CSDN博客
2018年03月06日 15:12:04[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：64
个人分类：[图像基本处理](https://blog.csdn.net/YZXnuaa/article/category/7430188)
运用Opencv解决
# !/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import cv2
file_dir = '/home/ubuntu/图片/CAT_VS_DOGS_new/cat_dogs_record/cat'
i = 0
for file in os.listdir(file_dir):
    image = cv2.imread(os.path.join(file_dir, file))
    # 按比例修改图片大小
(x, y, z) = image.shape
    x_s = 1000  # define standard width
y_s = int(y * x_s / x)  # calc height based on standard width
    # x,y 位置相反！
res = cv2.resize(image, (y_s, x_s), interpolation=cv2.INTER_CUBIC)
    i = i+1
n = str(i)
    s = n.zfill(3)
    cv2.imwrite("/home/ubuntu/视频/vryuchuli/"+s+".jpg", res)
