# OpenCV学习笔记（三）——Mat，图像的新容器

2011年10月21日 20:48:05

yang_xian521

阅读数：37149

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

自从版本2.0，OpenCV采用了新的数据结构，用Mat类结构取代了之前用extended C写的cvMat和lplImage，更加好用啦，最大的好处就是更加方便的进行内存管理，对写更大的程序是很好的消息。

 需要注意的几点：

\1. Mat的拷贝只是复制了Mat的信息头，数据的指针也指向了被拷贝的数据地址，而没有真正新建一块内存来存放新的矩阵内容。这样带来的一个问题就是对其中一个Mat的数据操作就会对其他指向同一块数据的Mat产生灾难性的影响。

 2.建立多维数组的格式是这样的

```cpp
	int sz[3] = {2, 2, 2};
	Mat L(3, sz, CV_8UC(1), Scalar::all(0));
```

 3.传统的lplImage格式也可直接转换为Mat格式

```cpp
	IplImage* img = cvLoadImage("greatwave.png", 1);
	Mat mtx(img); // convert IplImage* -> Mat
```

如果想将新版本的Mat格式转换为老版本，则需要如下调用：



```cpp
Mat I;
IplImage* pI = &I.operator IplImage();
CvMat* mI = &I.operator CvMat();
```

 不过更安全的调用格式为： 





```cpp
Ptr<IplImage> piI = &I.operator IplImage();
```

 4.Mat结构更加友好，很多操作更接近matlab的风格 



 5.也有Point2f，Point3f，vector等数据结构可以使用

6.RNG类可以产生随机数

7.实现颜色通道的分离使用函数split