# CUHK 行人数据集　及　转为ｙｏｌｏ训练标签格式 - YZXnuaa的博客 - CSDN博客
2018年05月16日 15:18:05[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1012
部分转载：https://zhuanlan.zhihu.com/p/31836357
## **11.[CUHK Occlusion Dataset](https://link.zhihu.com/?target=http%3A//mmlab.%253C/b%253Eie.cuhk.edu.hk/datasets/cuhk_occlusion/index.html)**
![](https://pic3.zhimg.com/80/v2-0dbc5091544db0f9ffdeee4b3c0d7b5b_hd.jpg)
该数据集出自于香港中文大学，可应用于行为分析和行人检测。包含了1063张行人图片。
**行人检测数据集打包下载链接：**[https://pan.baidu.com/s/1o8aanoQ](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1o8aanoQ)
**密码：**xkka
转换数据格式代码：
# coding=UTF-8
import os
from PIL import Image
from os import listdir, getcwd
image_path = "/data/yolo3/scripts/VOCdevkit/VOC2007/JPEGImages"                          # 图片存放路径，路径固定
# 我们的标签格式的路径
filelist = open("./annotations_CUHK/CUHK.txt", "r")   # 1,我们的标签存放路径
sets=[('train')]
lines = filelist.readlines()
# 将CUHK的x,y,w,h格式转换成yolo的X,Y,W,H
def convert(size, box):
   dw = 1./size[0]
   dh = 1./size[1]
   x = box[0] + box[2]/2.0
y = box[1] + box[3]/2.0
w = box[2]
   h = box[3]
   x = x*dw
   w = w*dw
   y = y*dh
   h = h*dh
   return (x,y,w,h)
# 获取要查询的图片的w,h
def get_size(image_id):
   im = Image.open('./VOCdevkit/VOC2007/JPEGImages/%s'%(image_id))   # 2,源图片存放路径
size = im.size
   w = size[0]
   h = size[1]
   return (w,h)
# 在labels目录下创建每个图片的标签txt文档
def text_create(name,bnd):
   full_path = "./VOCdevkit/VOC2007/labels/%s.txt"%(name)
   size = get_size(name + '.jpg')
   convert_size = convert(size, bnd)
   file = open(full_path, 'a')
   file.write('0 ' + str(convert_size[0]) + ' ' + str(convert_size[1]) + ' ' + str(convert_size[2]) + ' ' + str(convert_size[3]) )
   file.write('\n')
# 获取文件夹下所有图片的图片名
def get_name(file_dir):
   list_file=[]
   for root, dirs, files in os.walk(file_dir):
      for file in files:
         # splitext()将路径拆分为文件名+扩展名，例如os.path.splitext(“E:/lena.jpg”)将得到”E:/lena“+".jpg"
if os.path.splitext(file)[1] == '.jpg':
            list_file.append(os.path.join(root, file))
   return list_file
# 当前工作环境下的绝对路径
wd = getcwd()
for image_set in sets:
   if not os.path.exists('/data/yolo3/scripts/VOCdevkit/VOC2007/labels'):
      os.makedirs('/data/yolo3/scripts/VOCdevkit/VOC2007/labels')                     # 生成的yolo3标签存放路径，路径固定
image_names = get_name(image_path)
   list_file = open('2007_%s.txt'%(image_set), 'w')
   for image_name in image_names:
      list_file.write('%s\n'%(image_name))
   list_file.close()
for line in lines:
   str_names = line.split()
   str_name = str_names[0]
   b = line.split()[1:]
   bnd = (float(b[0]) ,float(b[1]) ,float(b[2]) ,float(b[3]))
   text_create(str_name, bnd)
