# 【人脸检测】SSH: Single Stage Headless Face Detector - zj360202的专栏 - CSDN博客





2017年12月28日 10:26:48[zj360202](https://me.csdn.net/zj360202)阅读数：1185









SSH: Single Stage Headless Face Detector 

ICCV2017 
[https://github.com/mahyarnajibi/SSH](https://github.com/mahyarnajibi/SSH)


本文的人脸检测算法走的是又快又好的路子，类似于目标检测中的 SSD算法思路。

时间上面用GPU 800x1200 100ms/f
![这里写图片描述](https://img-blog.csdn.net/20171031085355957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


SSH is designed to decrease inference time, have a low memory foot-print, and be scale-invariant， single-stage detector 

本文设计的人脸检测算法简称为 SSH，希望速度快，占用内存少，尺度不变性。它是一个单步骤检测器


3 Proposed Method 

3.1. General Architecture 
![这里写图片描述](https://img-blog.csdn.net/20171031090059185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

主要是在网络不同深度的卷积层进行人脸检测 

SSH 对网络不同位置上，即不同尺度的特征图上接入了三个检测模块，检测模块由 a convolutional binary classifier and a regressor 构成


这里我们采用了类似 RPN 策略来构建 anchor set，采用滑动窗口的方式，每个位置定义具有不同尺度的 K 个 anchors，这里我们的长宽比是1。如果输入检测模块的特征图大小为 W×H， 那么一共有 W×H×K 个anchors


detection module 
![这里写图片描述](https://img-blog.csdn.net/20171031155420713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


Context Module 

incorporate context by enlarging the window around the candidate proposals， 

SSH mimics this strategy by means of simple convolutional layers 
![这里写图片描述](https://img-blog.csdn.net/20171031155512700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


Scale-Invariance Design 

这里检测人脸 在网络的三个不同卷积层使用了 三个检测模块 M1，M2，M3, 这三个检测模块使用的步长分别为 8, 16，32 ，用于检测 大、中、小人脸


3.4. Training 

不同尺寸的人脸对用不同的检测模块进行训练 

3.4.1 Loss function 

损失函数的定义


3.5. Online hard negative and positive mining 

去除一些简单的负样本


4 Experiments


WIDER face detection benchmark 
![这里写图片描述](https://img-blog.csdn.net/20171031160220432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171031160356884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


检测时间 
![这里写图片描述](https://img-blog.csdn.net/20171031160241768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


输入图像尺寸的影响 
![这里写图片描述](https://img-blog.csdn.net/20171031160308029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


创新点：


1：检测model


2：OHEM方法



