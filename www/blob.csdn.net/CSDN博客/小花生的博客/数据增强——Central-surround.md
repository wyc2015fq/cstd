
# 数据增强——Central-surround - 小花生的博客 - CSDN博客


2018年07月15日 09:57:15[Peanut_范](https://me.csdn.net/u013841196)阅读数：378



###### 参考论文： 《Learning to Compare Image Patches via Convolutional Neural Networks》
**在输入端进行数据增强操作：**
假设我们输入的是大小为64*64的一张图片，那么Central-surround的意思就是把图片64*64的图片，处理成两张32*32图片，然后再输入网络，那么这两张32*32的图片是怎么计算得到的？这就是Central-surround方法，也就是第一张图片是通过以图片中心，进行裁剪出32*32的图片，也就是下图的浅蓝色区域的图片。*
![这里写图片描述](https://img-blog.csdn.net/20180715095949803?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*那么第二张图片是怎么计算的：这张图片是直接通过对整张图片下采样的方法得到的，也就是说直接把64**[64的图片进行下采样得到32](https://img-blog.csdn.net/20180715095949803?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**32的图片。那么为什么要把一张64**[64的图片，拆分成两张32*32的图片。其实这个就像多尺度一样，在图片处理领域经常采用多分辨率、多尺度，比如什么sift、还有什么高斯金字塔什么的。](https://img-blog.csdn.net/20180715095949803?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)*
*总结：*
*让网络既能学到局部的特征变化，又能学到全局的特征，提高模型的泛化能力，提高精度。*
*在这里：第一张图像采用随机裁剪的方式（而非中心裁剪），第二张图像进行下采样操作。融合为6通道的输入。*
*\# -*- coding: utf-8 -*-
"""
Created on Tue Jul  3 09:53:47 2018
数据增强
@author: fancp
"""
import os
import tensorflow as tf
import cv2
import numpy as np
def resize_image(img):
    return cv2.resize(img,(96,112))  
filename = r"aa.jpg"
file_contents = tf.read_file(filename)
image = tf.image.decode_jpeg(file_contents, dct_method="INTEGER_ACCURATE")
image.set_shape((130,110,3))
image = tf.cast(image, tf.float32)
print('*************')
print(image)
\# pic1
image1 = tf.random_crop(image, [112, 96, 3])
image1 = tf.image.random_flip_left_right(image1)
image1 = tf.minimum(255.0, tf.maximum(0.0,tf.image.random_brightness(image1,25.0)))
image1 = tf.minimum(255.0, tf.maximum(0.0,tf.image.random_contrast(image1,0.8,1.2)))
noise1 = tf.random_normal((112, 96, 3), mean=0.0, stddev=3.0, dtype=tf.float32)
image1 = tf.minimum(255.0, tf.maximum(0.0,image1+noise1))
print(image1)
\# pic2
image2 = tf.py_func(resize_image,[image],tf.float32)
image2 = tf.image.random_flip_left_right(image2)
image2 = tf.minimum(255.0, tf.maximum(0.0,tf.image.random_brightness(image2,25.0)))
image2 = tf.minimum(255.0, tf.maximum(0.0,tf.image.random_contrast(image2,0.8,1.2)))
noise2 = tf.random_normal((112, 96, 3), mean=0.0, stddev=3.0, dtype=tf.float32)
image2 = tf.minimum(255.0, tf.maximum(0.0,image2+noise2))
print(image2)
image=tf.concat([image1,image2],axis=2)
image = tf.subtract(image,127.5)
image = tf.multiply(image,0.0078125)
image=tf.reshape(image,[112,96,6])
print(image)

sess = tf.Session()
c = sess.run(image)
a = sess.run(image1)
print(a.shape)
b = sess.run(image2)
cv2.imshow('aaa',a.astype(np.uint8))
cv2.imshow('bbb',b.astype(np.uint8))
cv2.waitKey(0)**输入：*
![这里写图片描述](https://img-blog.csdn.net/20180715100614950?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*输出：*
![这里写图片描述](https://img-blog.csdn.net/20180715101116264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---
*\#\#\#\#\#\#注：博众家之所长，集群英之荟萃。*

