# OpenCV学习笔记（五）——2.0以上版本和以下版本的数据兼容 - 迭代的是人，递归的是神 - CSDN博客





2011年10月24日 09:07:26[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：11970
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









                
都知道，2.0版本对之前的OpenCV数据结构进行了大幅度的修改。但对之前版本的兼容是一个很重要的事情。这节就主要讨论这个问题

首先来看一下2.0版本对之前版本的进行了哪些修改

1.采用了新的数据结构Mat作为图像的容器，取代了之前的CvMat和lplImage，这个改动不是太复杂，只需适应一下新东西，而且可以自由转换

```cpp
Mat I;
IplImage pI = I;
CvMat mI = I;
```


对于指针的操作要相对复杂一些，而且还要注意内存的释放，我这里不推荐用老版本的数据结构，例如：

```cpp
Mat I;
IplImage* pI = &I.operator IplImage();
CvMat* mI = &I.operator CvMat();
```


2.对library进行了重组，将原来的一个大库根据功能结构分成具体小库，这样包含头文件的时候只需要加入你需要的库，只是原来库的子集

3.使用了cv 这个namespace来防止和其他的library 结构冲突。所以在使用的时候也要预先加上cv::关键字，这也是新版本的函数，数据都省略了cv前缀的原因，一般放在include之后，格式为：

```cpp
using namespace cv; // The new C++ interface API is inside this namespace. Import it.
```






