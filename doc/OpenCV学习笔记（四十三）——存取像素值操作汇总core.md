# OpenCV学习笔记（四十三）——存取像素值操作汇总core

2012年01月06日 18:36:12

yang_xian521

阅读数：34199

 								标签： 																[iterator																](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)[优化																](https://so.csdn.net/so/search/s.do?q=优化&t=blog)[多线程																](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)[image																](https://so.csdn.net/so/search/s.do?q=image&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog) 							更多

 								个人分类： 																[OpenCV																](https://blog.csdn.net/yang_xian521/article/category/910716) 							

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

在上一讲[OpenCV学习笔记（四十二）——Mat数据操作之普通青年、文艺青年、暴力青年](http://blog.csdn.net/yang_xian521/article/details/7161335)里，对Mat内数据的各种读写操作进行了速度的比较，都是我自己想到的方法，感觉不够系统，这次整理了下思路，参考了文献，把能想到的方法进行了汇总，希望能对大家有所帮助。

### 1.存取单个像素值

最通常的方法就是



```cpp
img.at<uchar>(i,j) = 255;
img.at<Vec3b>(i,j)[0] = 255;
```

 如果你觉得at操作显得太笨重了，不想用Mat这个类，也可以考虑使用轻量级的Mat_类，使用重载操作符()实现取元素的操作。 





```cpp
cv::Mat_<uchar> im2= img; // im2 refers to image
im2(50,100)= 0; // access to row 50 and column 100
```



### 2.用指针扫描一幅图像



对于一幅图像的扫描，用at就显得不太好了，还是是用指针的操作方法更加推荐。先介绍一种上一讲提到过的



```cpp
for (int j=0; j<nl; j++)
{
        uchar* data= image.ptr<uchar>(j);
        for (int i=0; i<nc; i++)
       {                 
                  data[i] = 255;
        }
}
```

 更高效的扫描连续图像的做法可能是把W*H的衣服图像看成是一个1*（w*h）的一个一维数组，这个想法是不是有点奇葩，这里要利用

isContinuous

这个函数判断图像内的像素是否填充满，使用方法如下： 





```cpp
if (img.isContinuous())
{
        nc = img.rows*img.cols*img.channels();
}
uchar* data = img.ptr<uchar>(0);
for (int i=0; i<nc; i++)
{
        data[i] = 255;
}
```

 更低级的指针操作就是使用Mat里的data指针，之前我称之为暴力青年，使用方法如下： 





```cpp
uchar* data = img.data;
// img.at(i, j)
data = img.data + i * img.step + j * img.elemSize();
```



### 3.用迭代器iterator扫描图像



和C++STL里的迭代器类似，Mat的迭代器与之是兼容的。是MatIterator_。声明方法如下：



```cpp
cv::MatIterator_<Vec3b> it;
```

 或者是： 





```cpp
cv::Mat_<Vec3b>::iterator it;
```

 扫描图像的方法如下： 





```cpp
Mat_<Vec3b>::iterator it = img.begin<Vec3b>();
Mat_<Vec3b>::iterator itend = img.end<Vec3b>();
for (; it!=itend; it++)
{
         (*it)[0] = 255;
}
```

### 4.高效的scan image方案总结



还是用我们之前使用过的getTickCount、getTickFrequency函数测试速度。这里我就不一一列举我测试的结果了，直接上结论。测试发现，好的编写风格可以提高50%的速度！要想减少程序运行的时间，必要的优化包括如下几个方面：

（1）内存分配是个耗时的工作，优化之；

（2）在循环中重复计算已经得到的值，是个费时的工作，优化之；举例：



```cpp
int nc = img.cols * img.channels();
for (int i=0; i<nc; i++)
{.......}
//**************************
for (int i=0; i<img.cols * img.channels(); i++)
{......}
```

 后者的速度比前者要慢上好多。 



（3）使用迭代器也会是速度变慢，但迭代器的使用可以减少程序错误的发生几率，考虑这个因素，可以酌情优化



（4）at操作要比指针的操作慢很多，所以对于不连续数据或者单个点处理，可以考虑at操作，对于连续的大量数据，不要使用它

（5）扫描连续图像的做法可能是把W*H的衣服图像看成是一个1*（w*h）的一个一维数组这种办法也可以提高速度。短的循环比长循环更高效，即使他们的操作数是相同的



以上的这些优化可能对于大家的程序运行速度提高并不明显，但它们毕竟是个得到速度提升的好的编程策略，希望大家能多采纳。

还有就是利用多线程也可以高效提高运行速度。OpenMP和TBB是两种流行的APT，不过对于多线程的东西，我是有些迷糊的，呵呵

### 5.整行整列像素值的赋值

对于整行或者整列的数据，可以考虑这种方式处理



```cpp
img.row(i).setTo(Scalar(255));
img.col(j).setTo(Scalar(255));
```

 这节就先介绍这么多攻略吧~希望大家喜欢 