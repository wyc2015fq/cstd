# 计算机视觉库OpenCV初步了解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月10日 11:39:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3776








OpenCV全称是OpenSource Computer Vision Library，是一个开放源代码的计算机视觉库。OpenCV最初由英特尔公司发起并开发，以BSD许可证授权发行，可以在商业和研究领域中免费试用，现在美国Willow Garage为OpenCV提供主要的支持。OpenCV可用于开发实时的图像处理、计算机视觉以及模式识别程序，目前在工业界以及科研领域广泛采用。




OpenCV是一个值得深入研究的库，暂时做初步了解，后续有需要再深入研究，尤其是SVM和ANN的应用。

### 1OpenCV开发环境

1）C/C++开发环境：Eclipse CPP+OpenCV

Project–>Properties–>C/C++Build–>Settings–>Tool Settings：

GCC C++Compiler->Includes中添加OpenCV的头文件目录；

MinGW C++Linker->Libraries中添加OpenCV的库文件目录以及相应的库文件名称；首先要下载安装Opencv。

2）Java开发环境：Eclipse Java

引入opencv-2.4.11-0.11.jar和opencv-2.4.11-0.11-windows-x86_64.jar即可；

稍前版本需要动态加载Opencv的DLL，目前只需引入相应平台的jar包即可，不安装opencv，不加载lib库。稍前版本在引入在opencv-2.4.11-0.11.jar后，选择该jar包的Native library location，输入opencv的/build/lib 文件夹下库目录(lib)的路径，当然要先安装opencv。

### 2OpenCV库基础知识

1）图像的基本操作

计算机数字图像用MXN的矩阵表示，矩阵元素的值表示该位置上像素的亮度，像素值越大该点越亮。

一般来说，灰度图用2维矩阵表示，彩色（多通道）图像用3维矩阵（MXNX3）表示。对于图像显示来说，目前大部分设备都是用无符号8位整数（类型为CV_8U）表示像素亮度。

图像数据在计算机内存中的存储顺序以图像最左上点（也可能是最左下点）开始。如果是多通道图像，比如RGB图像，则每个像素用三个字节表示。在OpenCV中，RGB图像的通道顺序为BGR。

2）Mat类

既然图像是通过矩阵表示，矩阵中每个点上的值表示像素亮度，那么Opencv是通过怎样数据结构来保存一副图像呢？

Mat类是新版OpenCV保存图像的数据结构，自动管理内存。

Mat类的具体操作可以参考OpenCV教程。Mat类存储的图像，主要就是矩阵操作。

Mat对象由两个数据部分组成：矩阵头（包含矩阵尺寸、存储方法、存储地址等信息）和一个指向存储所有像素值的矩阵的指针。

3）图像读写

函数imread()将图像文件读入内存，用Mat数据结构存储表示。将Mat对象以图像文件格式写入文件，可以用imwrite()函数。

imread()函数返回的是Mat对象：

Matimread(const string& filename,int flags=1)

filename是被读取的文件，flag>0函数返回3通道图像，flag=0返回单通道图像，flag<0函数不对图像进行通道转换。

将图像从内存写入文件函数：

boolimwrite(const string& filename,InputArray image,constvector<int>& params=vector<int>())

filename指定存储的文件路径和格式。Bmp格式是无损不压缩格式（文件大）、jpeg是有损压缩，png是无损压缩，建议存储为png。

4）视频读写

视频的格式主要由压缩算法决定。压缩算法称为编码器coder，解压算法称为解码器decoder，编解码算法统称为编解码器codec。OpenCV提供VideoCapture来读视频和VideoWrite来写视频。

VideoCapture既可以从视频文件中按帧读取图像，也可以从摄像头读取图像。

VideoWrite将视频写入文件。

5）总结

上面这些opencv库基础内容，将图像读入后怎么处理就涉及到opencv库中更多组件的处理。

http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/tutorials.html

可以通过该网站学习到具体图像处理技术方法。

### 3SVM和ANN应用

1）支持向量机(SVM) 是一个类分类器，正式的定义是一个能够将不同类样本在样本空间分隔的超平面。 换句话说，给定一些标记好的训练样本 （监督式学习），SVM算法输出一个最优化的分隔超平面。

OpenCV库提供CvSVM类支持SVM分类。通过CvSVM::train 训练一个SVM分类器， 以及用 CvSVM::predict 测试训练结果。

2）OpenCV的ml模块实现了人工神经网络（Artificial Neural Networks， ANN）最典型的多层感知器（multi-layer perceptrons, MLP）模型。CvANN_MLP是OpenCV中提供的一个神经网络的类，正如它的名字一样(multi-layer perceptrons),它是一个多层感知网络，它有一个输入层，一个输出层以及1或多个隐藏层。






