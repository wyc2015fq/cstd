# 【OpenCV】CalcBackProject函数 - YZXnuaa的博客 - CSDN博客
2018年02月08日 10:35:17[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：236
# 反向投影
## 目标
本文档尝试解答如下问题:
- 什么是反向投影，它可以实现什么功能？
- 如何使用OpenCV函数 [calcBackProject](http://opencv.willowgarage.com/documentation/cpp/imgproc_histograms.html?#calcBackProject) 计算反向投影？
- 如何使用OpenCV函数 [mixChannels](http://opencv.willowgarage.com/documentation/cpp/core_operations_on_arrays.html?#mixChannels) 组合图像的不同通道？
## 原理
### 什么是反向投影？
- 反向投影是一种记录给定图像中的像素点如何适应直方图模型像素分布的方式。
- 简单的讲， 所谓反向投影就是首先计算某一特征的直方图模型，然后使用模型去寻找图像中存在的该特征。
- 例如， 你有一个肤色直方图 ( Hue-Saturation 直方图 ),你可以用它来寻找图像中的肤色区域:
### 反向投影的工作原理?
- 
我们使用肤色直方图为例来解释反向投影的工作原理:
- 
假设你已经通过下图得到一个肤色直方图(Hue-Saturation)， 旁边的直方图就是 *模型直方图* ( 代表手掌的皮肤色调).你可以通过掩码操作来抓取手掌所在区域的直方图:
|![T0](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection_Theory0.jpg)|![T1](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection_Theory1.jpg)|
下图是另一张手掌图(测试图像) 以及对应的整张图像的直方图:
|![T2](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection_Theory2.jpg)|![T3](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection_Theory3.jpg)|
我们要做的就是使用 *模型直方图* (代表手掌的皮肤色调) 来检测测试图像中的皮肤区域。以下是检测的步骤
- 
对测试图像中的每个像素 ( ![p(i,j)](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/41d6ddd5993c418573ce68e285bb36109da45ff1.png) ),获取色调数据并找到该色调( ![( h_{i,j}, s_{i,j} )](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/21b6b795449bc0a979ae7250bb97b84ce57061ee.png) )在直方图中的bin的位置。
- 
查询 *模型直方图* 中对应的bin - ![( h_{i,j}, s_{i,j} )](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/21b6b795449bc0a979ae7250bb97b84ce57061ee.png) - 并读取该bin的数值。
- 
将此数值储存在新的图像中(*BackProjection*)。 你也可以先归一化 *模型直方图* ,这样测试图像的输出就可以在屏幕显示了。
- 
通过对测试图像中的每个像素采用以上步骤， 我们得到了下面的 BackProjection 结果图:
![../../../../../_images/Back_Projection_Theory4.jpg](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection_Theory4.jpg)
- 
使用统计学的语言, *BackProjection* 中储存的数值代表了测试图像中该像素属于皮肤区域的 *概率* 。比如以上图为例， 亮起的区域是皮肤区域的概率更大(事实确实如此),而更暗的区域则表示更低的概率(注意手掌内部和边缘的阴影影响了检测的精度)。
## 源码
- 
**本程序做什么?**
- 
装载图像
- 
转换原图像到 HSV 格式，再分离出 *Hue* 通道来建立直方图 (使用 OpenCV 函数 [mixChannels](http://opencv.willowgarage.com/documentation/cpp/core_operations_on_arrays.html?#mixChannels))
- - 让用户输入建立直方图所需的bin的数目。
- 计算同一图像的直方图 (如果bin的数目改变则更新直方图) 和反向投影图。
- 
显示反向投影图和直方图。
- 
**下载源码**:
> - 点击 [这里](https://code.ros.org/svn/opencv/trunk/opencv/samples/cpp/tutorial_code/Histograms_Matching/calcBackProject_Demo1.cpp) 获取简单版的源码 (本教程使用简单版)。
- 要尝试更炫的代码 (使用 H-S 直方图和 floodFill 来定义皮肤区域的掩码)你可以点击 [增强版演示](https://code.ros.org/svn/opencv/trunk/opencv/samples/cpp/tutorial_code/Histograms_Matching/calcBackProject_Demo2.cpp)
- 当然你也可以从实例库里下载经典的 [camshiftdemo](https://code.ros.org/svn/opencv/trunk/opencv/samples/cpp/camshiftdemo.cpp) 示例。
- 
**代码一瞥:**
#include "opencv2/imgproc/imgproc.hpp"#include "opencv2/highgui/highgui.hpp"#include <iostream>usingnamespacecv;usingnamespacestd;/// 全局变量Matsrc;Mathsv;Mathue;intbins=;/// 函数申明voidHist_and_Backproj(int,void*);/** @函数 main */intmain(intargc,char**argv){/// 读取图像src=imread(argv[],);/// 转换到 HSV 空间cvtColor(src,hsv,CV_BGR2HSV);/// 分离 Hue 通道hue.create(hsv.size(),hsv.depth());intch[]={,};mixChannels(&hsv,,&hue,,ch,);/// 创建 Trackbar 来输入bin的数目char*window_image="Source image";namedWindow(window_image,CV_WINDOW_AUTOSIZE);createTrackbar("* Hue  bins: ",window_image,&bins,,Hist_and_Backproj);Hist_and_Backproj(,);/// 现实图像imshow(window_image,src);/// 等待用户反应waitKey();return;}/** * @函数 Hist_and_Backproj * @简介：Trackbar事件的回调函数 */voidHist_and_Backproj(int,void*){MatNDhist;inthistSize=MAX(bins,);floathue_range[]={,};constfloat*ranges={hue_range};/// 计算直方图并归一化calcHist(&hue,,,Mat(),hist,,&histSize,&ranges,true,false);normalize(hist,hist,,,NORM_MINMAX,-,Mat());/// 计算反向投影MatNDbackproj;calcBackProject(&hue,,,hist,backproj,&ranges,,true);/// 显示反向投影imshow("BackProj",backproj);/// 显示直方图intw=;inth=;intbin_w=cvRound((double)w/histSize);MathistImg=Mat::zeros(w,h,CV_8UC3);for(inti=;i<bins;i++){rectangle(histImg,Point(i*bin_w,h),Point((i+)*bin_w,h-cvRound(hist.at<float>(i)*h/255.0)),Scalar(,,),-);}imshow("Histogram",histImg);}
## 解释
- 
申明图像矩阵，初始化bin数目:
Matsrc;Mathsv;Mathue;intbins=;
- 
读取输入图像并转换到HSV 格式:
src=imread(argv[],);cvtColor(src,hsv,CV_BGR2HSV);
- 
本教程仅仅使用Hue通道来创建1维直方图 (你可以从上面的链接下载增强版本,增强版本使用了更常见的H-S直方图，以获取更好的结果):
hue.create(hsv.size(),hsv.depth());intch[]={,};mixChannels(&hsv,,&hue,,ch,);
你可以看到这里我们使用 [mixChannels](http://opencv.willowgarage.com/documentation/cpp/core_operations_on_arrays.html?#mixChannels) 来抽取 HSV图像的0通道(Hue)。 该函数接受了以下的实参:
- **&hsv:** 一系列输入图像的数组， 被拷贝的通道的来源
- **1:** 输入数组中图像的数目
- **&hue:** 一系列目的图像的数组， 储存拷贝的通道
- **1:** 目的数组中图像的数目
- **ch[] = {0,0}:** 通道索引对的数组，指示如何将输入图像的某一通道拷贝到目的图像的某一通道。在这里，&hsv图像的Hue(0) 通道被拷贝到&hue图像(单通道)的0 通道。
- **1:** 通道索引对德数目
- 
创建Trackbar方便用户输入bin数目。 Trackbar的任何变动将会调用函数 **Hist_and_Backproj** 。
char*window_image="Source image";namedWindow(window_image,CV_WINDOW_AUTOSIZE);createTrackbar("* Hue  bins: ",window_image,&bins,,Hist_and_Backproj);Hist_and_Backproj(,);
- 
显示并等待用户突出程序:
imshow(window_image,src);waitKey();return;
- 
**Hist_and_Backproj 函数:** 初始化函数 [calcHist](http://opencv.willowgarage.com/documentation/cpp/imgproc_histograms.html?#calcHist) 需要的实参， bin数目来自于 Trackbar:
voidHist_and_Backproj(int,void*){MatNDhist;inthistSize=MAX(bins,);floathue_range[]={,};constfloat*ranges={hue_range};
- 
计算直方图并归一化到范围 ![[0,255]](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/0fb139e8e3c75ef24fe4eac5ba07c800abd7c943.png)
calcHist(&hue,,,Mat(),hist,,&histSize,&ranges,true,false);normalize(hist,hist,,,NORM_MINMAX,-,Mat());
- 
调用函数 [calcBackProject](http://opencv.willowgarage.com/documentation/cpp/imgproc_histograms.html?#calcBackProject) 计算同一张图像的反向投影
MatNDbackproj;calcBackProject(&hue,,,hist,backproj,&ranges,,true);
所有的实参都已经知道了(与计算直方图的实参一样), 仅仅增加了 backproj 矩阵,用来储存原图像(&hue)的反向投影。
- 
显示 backproj:
imshow("BackProj",backproj);
- 
显示1维 Hue 直方图:
intw=;inth=;intbin_w=cvRound((double)w/histSize);MathistImg=Mat::zeros(w,h,CV_8UC3);for(inti=;i<bins;i++){rectangle(histImg,Point(i*bin_w,h),Point((i+)*bin_w,h-cvRound(hist.at<float>(i)*h/255.0)),Scalar(,,),-);}imshow("Histogram",histImg);
## 结果
- 
下面是对一张样本图像(猜猜是什么?又是一掌)进行的测试结果。 你可以改变bin的数目来观察它是如何影响结果图像的:
|![R0](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection1_Source_Image.jpg)|![R1](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection1_Histogram.jpg)|![R2](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/Back_Projection1_BackProj.jpg)|
