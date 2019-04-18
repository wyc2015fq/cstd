# OpenCV的Python接口 - wishchinYang的专栏 - CSDN博客
2014年01月11日 10:28:07[wishchin](https://me.csdn.net/wishchin)阅读数：2094
Python教程系列：[http://blog.csdn.net/sunny2038/article/details/9057415](http://blog.csdn.net/sunny2038/article/details/9057415)
与C++的不同之处：[http://developer.51cto.com/art/201003/187863_all.htm](http://developer.51cto.com/art/201003/187863_all.htm)
python接口功能的类似功能的介绍其中包括迭代访问、切片方式、如果你对python接口功能想有所了解的话，你可以浏览以下文章，会对你有所帮助。
AD：[2013云计算架构师峰会课程资料下载](http://down.51cto.com/zt/5551)
在正常情况下, OpenCV的python接口和C语言接口基本一致. 但是, 对于python接口实现比较困难的地方, 一些函数和结构体可能会有不同. 下面的描述就是有关此内容的详细介绍。希望会对你有所帮助。
函数接口也尽量保持和C语言一致. 最大的差异是在函数通过参数来返回值的时候. 由于python中有些基本的参数都不可改变的, 因此代替的方法是一次返回多个值. 同样, 大部分的结构和C语言中也有相似的功能, 但是语法可能有些不同.
- [Python socket服务如何进行配置详解](http://developer.51cto.com/art/201003/187727.htm)
- [Python日志需要我们不断的学习](http://developer.51cto.com/art/201003/187728.htm)
- [了解Python多线程的创建程序](http://developer.51cto.com/art/201003/187804.htm)
- [探秘Python对象体系](http://developer.51cto.com/art/201003/187816.htm)
- [用Python标准库修改搜索引擎获取结果](http://developer.51cto.com/art/201003/187833.htm)
**下面描述比较重要的差异, 具体的细节可以参考python接口的代码. **
**没有IplImage **
最大的差异是python接口中没有IplImage! 这主要是为了避免SWIG 在处理IplImage中隐含共享的不足. 以下是具体的替代方法:
原来返回IplImage的函数, 现在返回CvMat 原来读IplImage改为读CvMat ，CvMat中没有的IplImage属性被添加以支持IplImage, 例如height, width, depth, imageDataSize等. ROI和COI相关函数被禁止使用. 不过用户可以用cvGetSubRect/cvSplit/cvMerge来实现类似的功能.
**迭代访问 **
CvMat扩展了python中的2个基本方法__iter__ 和 __getitem__, 来支持元素的简单访问.
通过行的方式迭代
```python
<python>x = cvCreateMat(m, n, type) for row in x:   
     # row is same as that returned by cvGetRowpython>
```
通过列的方式迭代
```python
<python>for col in x.colrange():   
     # col is same as that returned by cvGetColpython>
```
**切片方式**
获取一行
```python
<python>row = x[i]python>
```
获取一列
```python
<python>col = x[:, i]python>
```
获取一个区域
```python
<python>slice = x[0:10, 0:10]python>
```
获取一个元素
```python
<python>elem = x[i, j]   
or   
elem = x[i][j]   
or if x is a vector   
elem = x[i]python>
```
同样的方法可以用来修改元素
```python
<python># x and y are CvMat's x[0:10, 0:5] = y[10:20, 1:6] x[i, j]
 = 1; x[:, :] = 1; x[:, :] = cvScalar(1); x[0:10, i]
 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]python>
```
**CvMatND **
暂时还不支持. 如果您有兴趣自己实现的话, 请联系RomanStanchak.
CvSeq 
python中访问CvSeq的代码:
```python
<python>num_contours, contours = cv.cvFindContours(...)   
hrange iterates through h_next element   
for contour in contours.hrange():   
   # contour is a CvSeq of points  
   for pt in contour:  
       print pt  
   # alternately  
   for pt in contour.vrange():  
       print pt  
   # alternately  
   for i in range(contour.total):  
       print contour[i]python>
```
**交互命令行编程 **
python的最大优点是可以在交互命令行中开发程序. 交互shell编程和Matlab类似. 以下在命令行中显示一副图像的代码:
```python
<python>
from opencv.cv import * 
from opencv.highgui import *
 cvStartWindowThread() 
cvNamedWindow("win") 
im= cvLoadImage("pics/airplane.jpg") 
cvShowImage("win", im)python>
```
需要注意的是cvStartWindowThread函数, 它目前只支持linux系统. cvStartWindowThread 函数的目的是代替cvWaitKey()来实现窗口的自动刷新. 去掉cvWaitKey()函数之后, 程序的输入将不受cvWaitKey阻塞, 更便于shell交互.
**Matlab语法 **
熟悉Matlab或SciPy的朋友, 可以更关注matlab_syntax模块. 该模块采用了和 Matlab/SciPy 类似的方法来创建数组. 很多情况下, 这比用OpenCV的标准函数更加便捷. 切换行号显示：
```python
<python>from opencv.cv import * 
from opencv.highgui import * 
from opencv.matlab_syntax import * 
cvStartWindowThread() 
im = imread("pics/airplane.jpg") 
imshow(im)
python>
```
