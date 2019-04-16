# OpenCV：概述、结构和内容 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月02日 12:32:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：121
个人分类：[OpenCV](https://blog.csdn.net/leixiaohua1020/article/category/1362937)










1、OpenCV的定义：
 OpenCV的英文全称是Open Source Computer Vision Library。它是一个开源的计算机视觉库，它由一系列 C 函数和少量 C++ 类构成，实现了图像处理和计算机视觉方面的很多通用算法。

2、OpenCV的特点：


 （1）OpenCV采用C/C++语言编写，可以运行在Linux/Windows/Mac等操作系统上。

 （2）OpenCV提供了Python、Ruby、MATLAB以及其他语言的接口。

 （3）它采用优化的C代码编写，能够充分利用多核处理器的优势

 （4）具有良好的可移植性

3、OpenCV的设计目标：

 执行速度尽量快，主要关注**实时应用**。如果是希望在Intel平台上得到更快的处理速度，可以购买Intel的高性能多媒体函数库IPP(Integrated Performance Primitives)。IPP库包含许多从底层优化的函数，这些函数涵盖多个应用领域。如果系统已经安装了IPP库，OpenCV会在运行时自动使用相应的IPP库。


 【注】：OpenCV使用优化了的C和C++代码实现，因此它对IPP不存在任何的依赖。

4、OpenCV的应用领域
 (1)人机互动 (2)物体识别 (3)图象分割 (4)人脸识别 (5)动作识别 (6)运动跟踪 (7)机器人 (8)运动分析 (9)机器视觉 (10)结构分析

5、OpenCV的结构和内容





OpenCV主体分为五个模块，其中四个模块如下图所示。

![](https://img-blog.csdn.net/20131002123017500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

OpenCV的CV模块包含基本的图像处理函数和高级的计算机视觉算法。ML是机器学习库，包含一些基于统计的分类和聚类工具。HighGUI包含图像和视频输入/输出的函数。CXCore包含OpenCV的一些基本数据结构和相关函数。
【具体内容如下】：

 （1） cxcore
 基础结构：CvPoint,CvSize,CvScalar等
 数组操作：cvCreateImage,cvCreateMat等
 动态结构：CvMemStorage,CvMemBlock等
 绘图函数：cvLine,cvRectangle等
 数据保存和运行时类型信息：CvFileStorage,cvOpenFileStorage等
 错误处理和系统函数:cvGetErrStatus,cvAlloc,cvFree等


 （2） cv

 图像处理：cvSobel，cvCanny等
 结构分析：ContourArea等
 运动分析与目标跟踪：cvMeanShift等
 模式识别：CvHaarFeature
 摄像头定标与三维重建：cvCalibrateCamer2

 （3）Machine Learning(ML)
 包含许多聚类、分类和数据分析函数。如Bayes分类器，K近邻算法，支持向量机，决策树，神经网络等等。

 （4）HighGUI

 图像界面函数：cvNamedWindow

 读图像和保存图像：cvLoadImage，cvSaveImage

 读视频和写视频：CvCreateFileCapture等


 （5）cvcam

 摄像机接口，在Opencv1.0以后的版本中已经被移除

 （6）cvaux

 该模块中一般存放一些即将被淘汰的算法和函数（如基于嵌入式隐马尔科夫的人脸识别算法），还包含一些实验性的算法和函数（前景检测，背景剔除等）


原文地址：[http://blog.csdn.net/xiaofeilong321/article/details/8841125](http://blog.csdn.net/xiaofeilong321/article/details/8841125)






