# EasyPR-Java开源中文车牌识别系统工程部署 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年09月24日 17:17:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8459








EasyPR-Java基于JavaCV（JNI封装OpenCV），支持中文，先车牌检测(应用SVM向量机训练样本)再OCR字符(应用ANN人工神经网络) 。用eclipse搭建起了工程，如下步骤：

1、工程搭建

1)下载zip，import到eclipse工程；

     https://github.com/fan-wenjie/EasyPR-Java

2)下载OpenCV3.0安装并配置环境变量；

     http://opencv.org/downloads.html    

     JavaCV0.8+不需要本地原生库，直接把opencv的代码编译后融合到了jar包里面；因此该步骤免了。

3)http://bytedeco.org/

     下载JavaCV的Jar库，引入工程；

     jar包分为两类，

     一类是如javacpp-0.11.jar、javacv-0.11.jar、opencv-2.4.11-0.11.jar，没有跟具体的硬件平台，这一类是纯java代码生成的，在任何平台上都一样

     另一类是opencv-2.4.11-0.11-windows-x86_64.jar，命名包含具体平台的，这一类包含了本地平台相关的库封装，是包含c++编译成的适合某一个平台的本地代码的。

     Linux平台的就需要opencv-2.4.11-0.11-linux-x86_64.jar。

由于Javacpp和Opencv版本问题，摸索好久，最后下载到了0.11版本相关Jar。部署好的eclipse工程结构如下：

![](https://img-blog.csdn.net/20160924171702953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


4）工程包下载地址：http://download.csdn.net/detail/fjssharpsword/9639024

      工程所需要的Javacv包下载地址：http://download.csdn.net/detail/fjssharpsword/9639029

2、执行结果： 

      1）对于测试库内都能识别；

      2）外部图片一般会提示以下两个错误或者无法识别。错误希望有兴趣的一起研究解决。

错误一：

![](https://img-blog.csdn.net/20160924171429078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


错误二：

![](https://img-blog.csdn.net/20160924172736065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







