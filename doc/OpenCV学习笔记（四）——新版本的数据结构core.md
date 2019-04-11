# OpenCV学习笔记（四）——新版本的数据结构core

2011年10月26日 09:09:46

yang_xian521

阅读数：20224

 								标签： 																[数据结构																](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)[matlab																](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[扩展																](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog) 							更多

 								个人分类： 																[OpenCV																](https://blog.csdn.net/yang_xian521/article/category/910716) 							

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

2.0新版本对数据结构进行了大幅修改：

定义了DataType类

定义了Point_模板类，取代了之前版本的CvPoint、CvPoint2D32f

定义了Point3_模板类，取代了之前版本的CvPoint2D32f

定义了Size_模板类，取代了之前版本的CvSize和CvSize2D32f

定义了Rect_模板类，取代了之前版本的CvRect

RotatedRect模板类，

TermCriteria模板类，取代了之前的CvTermCriteria，这个类是作为迭代算法的终止条件的，这个类在参考手册里介绍的很简单，我查了些资料，这里介绍一下。该类变量需要3个参数，一个是类型，第二个参数为迭代的最大次数，最后一个是特定的阈值。类型有CV_TERMCRIT_ITER、CV_TERMCRIT_EPS、CV_TERMCRIT_ITER+CV_TERMCRIT_EPS，分别代表着迭代终止条件为达到最大迭代次数终止，迭代到阈值终止，或者两者都作为迭代终止条件。以上的宏对应的c++的版本分别为TermCriteria::COUNT、TermCriteria::EPS，这里的COUNT也可以写成MAX_ITER。

Matx模板类。Matx模板类是对Mat类的一个拓展，从Matx类有派生出Vec类，又Vec类又派生出Scalar_类，取代了CvScalar

定义了Range类指定了一个序列的一个连续的子序列

定义了Ptr类用来对老版本的数据结构进行指针操作，更安全有效，可以防止内存的不正常使用。

最最重要的定义了Mat类来表示矩阵，取代了之前的CvMat和lplImage。Mat结构支持的操作有：

构造析构函数Mat和~Mat

对=、MatExpr、( )、CvMat、IplImage进行了运算符重载

row、col函数

rowRange、colRange

类似matlab的运算操作diag、t、inv、mul、cross、dot、zeros、one、eye

复制转换变形clone、copyTo、convertTo、assignTo、setTo、reshape、create、addref

其中copyTo函数有个妙用，不但可以复制Mat，还可以通过mask提取出感兴趣的部分

数据的操作release、resize、reserve、push_back、pop_back、locateROI、adjustROI

Mat的信息total、isContinuous、elemSize、elemSize1、type、depth、channes、step1、size、empty

其中step1函数返回Mat结构每一行的字节数=列数*通道数，更方便用指针定位特定元素

定位ptr、at、begin、end

还做了几个扩展类Mat_、NAryMatlterator、SparseMat、SparseMat_取代了之前的CvSparseMat。这些类的操作运算与Mat大同小异，类声明参考core的具体头文件

当然，新版本对老版本的数据结构和函数依然支持。

新版本还在这些结构里支持dft、dct变换，我这里讲一下我的新发现PCA类、SVD类

PCA类有构造函数PCA，运算符重载()，project，backProject。SVD类有构造函数SVD，运算符重载()，compute，solveZ，backSubst

这里介绍几个我使用过的实用函数：

inRange函数可以检查Matsrc的内容是否在Matlower、Matupper之间，输出结果是一个uchar型矩阵，1表示在两者之间，否则为0，值得注意的是，Matlower，Matupper也可以用Scalar的格式

bitwise_xxx函数对两个矩阵进行位运算，结果保存在第三个矩阵当中

mixChannels函数可以实现矩阵的指定通道复制到新矩阵的指定通道

总之，新版本支持更多的数学运算，还支持一些画图操作