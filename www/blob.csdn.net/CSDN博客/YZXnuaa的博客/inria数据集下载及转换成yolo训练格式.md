# inria数据集下载及转换成yolo训练格式 - YZXnuaa的博客 - CSDN博客
2018年05月16日 15:25:38[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1184

## 部分转载自：https://zhuanlan.zhihu.com/p/31836357
## **6.[INRIA Person Dataset（INRIA行人数据库）](https://link.zhihu.com/?target=http%3A//pascal.inrialpes.fr/data/human/)**
![](https://pic3.zhimg.com/80/v2-2774950c6e0158ebd8ea227668a2c3e0_hd.jpg)
该数据库是目前使用最多的静态行人检测数据库，提供原始图片及相应的标注文件。训练集有正样本614张（包含2416个行人），负样本1218张；测试集有正样本288张（包含1126个行人），负样本453张。图片中人体大部分为站立姿势且高度大于100个象素，部分标注可能不正确。图片主要来源于GRAZ-01、个人照片及google，因此图片的清晰度较高。在XP操作系统下部分训练或者测试图片无法看清楚，但可用OpenCV正常读取和显示。
**行人检测数据集打包下载链接：**[https://pan.baidu.com/s/1o8aanoQ](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1o8aanoQ)
**密码：**xkka
# coding=UTF-8
import os
import re
from PIL import Image
sets=['train']
image_path = "/data/yolo3/scripts/VOCdevkit/VOC2007/JPEGImages"                          # 图片存放路径，路径固定
annotations_path = "./annotations/" #文件夹目录                                          # INRIA标签存放路径
annotations= os.listdir(annotations_path) #得到文件夹下的所有文件名称 
# 获取文件夹下所有图片的图片名
def get_name(file_dir):
   list_file=[]
   for root, dirs, files in os.walk(file_dir):
      for file in files:
         # splitext()将路径拆分为文件名+扩展名，例如os.path.splitext(“E:/lena.jpg”)将得到”E:/lena“+".jpg"
if os.path.splitext(file)[1] == '.jpg':
            list_file.append(os.path.join(root, file))
   return list_file
# 在labels目录下创建每个图片的标签txt文档
def text_create(name,bnd):
   full_path = "/data/yolo3/scripts/VOCdevkit/VOC2007/labels/%s.txt"%(name)
   size = get_size(name + '.jpg')
   convert_size = convert(size, bnd)
   file = open(full_path, 'a')
   file.write('0 ' + str(convert_size[0]) + ' ' + str(convert_size[1]) + ' ' + str(convert_size[2]) + ' ' + str(convert_size[3]) )
   file.write('\n')
# 获取要查询的图片的w,h
def get_size(image_id):
   im = Image.open('/data/yolo3/scripts/VOCdevkit/VOC2007/JPEGImages/%s'%(image_id))       # 源图片存放路径
size = im.size
   w = size[0]
   h = size[1]
   return (w,h)
# 将Tagphoto的x,y,w,h格式转换成yolo的X,Y,W,H
def convert(size, box):
   dw = 1./size[0]
   dh = 1./size[1]
   x = (box[0] + box[2])/2.0
y = (box[1] + box[3])/2.0
w = box[2] - box[0]
   h = box[3] - box[1]
   x = x*dw
   w = w*dw
   y = y*dh
   h = h*dh
   return (x,y,w,h)
# 将处理的图片路径放入一个ｔｘｔ文件夹中
for image_set in sets:
   if not os.path.exists('/data/yolo3/scripts/VOCdevkit/VOC2007/labels'):
      os.makedirs('/data/yolo3/scripts/VOCdevkit/VOC2007/labels')                     # 生成的yolo3标签存放路径，路径固定
image_names = get_name(image_path)
   list_file = open('2007_%s.txt'%(image_set), 'w')
   for image_name in image_names:
      list_file.write('%s\n'%(image_name))
   list_file.close()
s = []
for file in annotations: #遍历文件夹  
str_name = file.replace('.txt', '')
   if not os.path.isdir(file): #判断是否是文件夹，不是文件夹才打开  
with open(annotations_path+"/"+file) as f : #打开文件  
iter_f = iter(f); #创建迭代器  
for line in iter_f: #遍历文件，一行行遍历，读取文本
str_XY = "(Xmax, Ymax)"
if str_XY in line:
               strlist = line.split(str_XY)
               strlist1 = "".join(strlist[1:])    # 把list转为str
strlist1 = strlist1.replace(':', '')
               strlist1 = strlist1.replace('-', '')
               strlist1 = strlist1.replace('(', '')
               strlist1 = strlist1.replace(')', '')
               strlist1 = strlist1.replace(',', '')
               b = strlist1.split()
               bnd = (float(b[0]) ,float(b[1]) ,float(b[2]) ,float(b[3]))
               text_create(str_name, bnd)
            else:
               continue

