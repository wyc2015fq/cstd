# Python+OpenCV学习(1) - weixin_33985507的博客 - CSDN博客
2013年05月14日 14:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
今天很happy因为下载到了一本关于用Python调用OpenCV的书
《Programming Computer Vision with Python》，上个月Orielly才出版的，哈哈。happy。
然后写出了第一个OpenCV+Python的“hello world”：展示lena.jpg
代码：
```
1 #!/usr/bin/python
 2 # Filename:show.py
 3 
 4 import cv2
 5 
 6 filename='/home/chris/workspace/python/barcode/lena.jpg'
 7 im = cv2.imread(filename)
 8 
 9 cv2.imshow('haha', im)
10 
11 cv2.waitKey()
```
下载地址：[http://it-ebooks.info/book/836/](http://it-ebooks.info/book/836/)
这书一上来让下载Python Imaging Library ，安装方式是，到官网下载压缩包解压然后chmod再python setup.py build 和install，参看：
[http://www.blogjava.net/stone2083/archive/2010/11/05/299946.html](http://www.blogjava.net/stone2083/archive/2010/11/05/299946.html)
调用摄像头也和C++的版本差不多：
```
1 import cv2
2 
3 cap = cv2.VideoCapture(0)
4 while True:
5     ret,im = cap.read()
6     cv2.imshow('video test',im)
7     key = cv2.waitKey(10)
8     if key == 27:
9         break
```
只是这里的ret,im=cap.read()一句话不懂。去掉ret,后代码不能运行。。要好好学习python阿
学python，其实是继续做条码识别。
