# OpenCV 3.1.0中的Mat对象使用 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年09月29日 00:11:54[gloomyfish](https://me.csdn.net/jia20003)阅读数：2916








一： Mat介绍

         OpenCV刚出来的时候图像加载内存之后的对象是IplImage作为数据对象，里面存储了图像的像素数据和宽、高、位图深度、图像大小、通道数等基本属性。IplImage结构是C语言体系下定义出来的接口，使用它时候最大的问题是要自己负责内存管理，控制内存分配和释放，很容易导致内存问题。

         OpenCV从2.0版本开始引入Mat对象，它会自动分配和释放内存，让开发人员把精力放在图像处理问题上面，不被内存问题所困扰，Mat是一个C++的类对象，它有两个部分组成：一个矩阵头部分（包含矩阵大小、存储方法等）另外一部分是一个指针指向矩阵像素的，其中矩阵头部分是固定常量大小。但是整个矩阵的大小跟图像实际大小有关系。

         另外一个改进就是引用计数系统，对Mat对象来说，图像处理可能是一系列算法的组合，会调用多个图像处理的算法，这个时候Mat作为引用被传到各个对应的算法处理函数，除非有必要，一般情况下只会复制Mat的头和指针，不会真正复制像素数据本身。

例子如下:



```cpp
Mat A, C
A = imread(argv[1], IMREAD_COLOR);
Mat B(A); // copy constructor
C = A;
```



上面的三个指针，虽然头部不一样，但是都指向同一个图像的像素数据矩阵。通常我们需要创建ROI区域，可以使用Mat来实现：

```cpp
Mat D (A, Rect(10, 10, 100, 100));
```

如果想跟图像数据一起复制，OpenCV提供了两个相关的API操作：cv::Mat::clone()与cv::Mat::copyTo()。



```cpp
Mat F = A.clone(); 
Mat G; A.copyTo(G);
```

所以关于使用Mat图像像素矩阵要记住如下四点：

1. 输出图像的内存分配是自动的

2. 使用OpenCV的C++接口，不需要考虑内存管理问题

3. 赋值操作和拷贝构造函数操作，只会复制头部分，像素数据部分仍然相同

4. 使用克隆clone和copyTo将会复制图像数据矩阵Mat



二：存储方法

这里是指存储图像像素值，它跟两个因素有关系，一个是色彩空间另外一个数据类型。常见的色彩空间包括：

RGB - 最常见的色彩空间，OpenCV标准显示都是基于RGB色彩空间

HSV/HLS - 三个通道分别为HUE、饱和度、亮度，一个常用的例子就是排出光线干扰，丢弃最后一个通道的值处理。

YCrCb - 在JPEG图像格式中比较流行使用

每一种色彩空间的每个通道值都有它自己的取值范围和应用场景。

创建一个Mat对象

cv::Mat::Mat 构造函数

```cpp
Mat  M(2,2, CV_8UC3, Scalar(0,0,255));     
cout << "M = " << endl << " " << M << endl << endl;
```
![](https://img-blog.csdn.net/20160929000427894)


其中前两个参数分别表示行(row)跟列(column)、第三个参数CV_8UC3中的8表示每个通道占8位、U表示无符号、C表示Char类型、3表示通道数目是3，第四个参数是个向量表示初始化每个像素值是多少，向量长度对应通道数目一致。创建多维数据：

```cpp
int sz[3] = {2,2,2};     
Mat  L(3,sz, CV_8UC(1), Scalar::all(0));
```

上面是一个创建多维（大于2维）数组的例子，只是第二个参数不通，其它都跟第一个例子相似。

cv::Mat::create功能

```cpp
M.create(4, 4, CV_8UC(2));
cout << "M = "<< endl << " "  << M << endl << endl;
```
![](https://img-blog.csdn.net/20160929000559616)


这种构造函数无法初始化每个像素值。此外OpenCV还提供类似Matlab风格的初始化函数：

```cpp
Mat E = Mat::eye(4, 4, CV_64F);     
cout << "E = " << endl << " " << E << endl << endl;     

Mat O = Mat::ones(2, 2, CV_32F);     
cout << "O = " << endl << " " << O << endl << endl;    
 
Mat Z = Mat::zeros(3,3, CV_8UC1);     
cout << "Z = " << endl << " " << Z << endl << endl;
```
![](https://img-blog.csdn.net/20160929000742210)


定义小的数组，可以使用如下方式：

```cpp
Mat C = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);     
cout << "C = " << endl << " " << C << endl << endl;
```
![](https://img-blog.csdn.net/20160929000832978)


克隆定义好的二维数组数据

```cpp
Mat RowClone = C.row(1).clone();     
cout << "RowClone = " << endl << " " << RowClone << endl << endl;
```
![](https://img-blog.csdn.net/20160929000918101)
参考资料：

http://docs.opencv.org/3.1.0/d6/d6d/tutorial_mat_the_basic_image_container.html



