# OpenCV（Open Source Computer Vision Library）简介 - Koma Hub - CSDN博客
2018年05月03日 20:03:42[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：393
个人分类：[OpenCV](https://blog.csdn.net/Rong_Toa/article/category/7627742)
##### 定义
OpenCV于1999年由Intel建立，如今由Willow Garage提供支持。OpenCV是一个基于BSD许可（开源）发行的跨平台计算机视觉库，可以运行在Linux、Windows、Android和Mac OS操作系统上。它由一系列 C 函数和少量 C++ 类构成，同时提供了Python、Ruby、MATLAB等语言的接口，实现了图像处理和计算机视觉方面的很多通用算法。OpenCV用C++语言编写，它的主要接口也是C++语言，但是依然保留了大量的C语言接口。如今也提供对于C#、Ch、Ruby的支持。所有新的开发和算法都是用C++接口。一个使用CUDA的GPU接口也于2010年9月开始实现。OpenCV 拥有包括 500 多个C函数的跨平台的中、高层 API。它不依赖于其它的外部库——尽管也可以使用某些外部库。
![](https://img-blog.csdn.net/20180503200706472)
##### 发展历史
1999年1月，CVL项目启动。主要目标是人机界面，能被UI调用的实时计算机视觉库，为Intel处理器做了特定优化。
2000年6月，第一个开源版本OpenCV alpha 3发布。
2000年12月，针对linux平台的OpenCV beta 1发布。
2006年，支持Mac OS的OpenCV 1.0发布。
2009年9月，OpenCV 1.2（beta2.0）发布。
2009年10月1日，Version 2.0发布。
2010年12月6日，OpenCV 2.2发布。
2011年8月，OpenCV 2.3发布。
2012年4月2日，发布OpenCV 2.4。
2014年8月21日，发布OpenCv 3.0 alpha。
2014年11月11日，发布OpenCV 3.0 beta。
2015年6月4日，发布OpenCV 3.0。
2016年12月，发布OpenCV 3.2版（合并969个修补程序，关闭478个问题）
2017年8月3日，发布OpenCV 3.3版（最重要的更新是把DNN模块从contrib里面提到主仓库）
OpenCV 使用类BSDlicense，所以对非商业应用和商业应用都是免费（FREE）的。（细节参考 license）
OpenCV提供的视觉处理算法非常丰富，并且它部分以C语言编写，加上其开源的特性，处理得当，不需要添加新的外部支持也可以完整的编译链接生成执行程序，所以很多人用它来做算法的移植，OpenCV的代码经过适当改写可以正常的运行在DSP系统和ARM嵌入式系统中，这种移植在大学中经常作为相关专业本科生毕业设计或者研究生课题的选题。
##### 为什么用OpenCV
OpenCV致力于成为标准API。OpenCV致力于真实世界的实时应用，通过优化的C代码的编写对其执行速度带来了可观的提升，并且可以通过购买Intel的IPP高性能多媒体函数库（Integrated Performance Primitives）得到更快的处理速度。下图为OpenCV与当前其他主流视觉函数库的性能比较。
![](https://img-blog.csdn.net/2018050319595834)
##### 应用领域
1、人机互动
2、物体识别
3、图像分割
4、人脸识别
5、动作识别
6、运动跟踪
7、机器人
8、运动分析
9、机器视觉
10、结构分析
11、汽车安全驾驶
##### 版本发布时间
2010年12月06日，OpenCV 2.2.0 正式版发布。 
2011年06月25日，OpenCV-2.3.0rc 发布了，新增加了一个stitching拼接模块，Android支持更加方便了，使用Google test框架，其他变化主要是内部性能改进。
2013年07月03日，OpenCV 2.4.6发布。
主要更新是关于手持设备方面的：
首先，OpenCV 2.3的Android build终于变成了官方支持的NDK-Build的方式了。以前的方法相当让人头疼，使用的非Android官方的方法去写JNI接口，tutorial跟着走一遍也不知道所以然。
其次，What’s new里面说道，新的Android支持是NVidia在开发。本来OpenCV就是支持CUDA显卡加速的。
2013年12月31日，OpenCV 2.4.8发布。
2014年4月25日，OpenCV 2.4.9发布。
2014年8月21日，OpenCv 3.0 alpha版本发布。
2014年11月11日，OpenCV 3.0 beta版本发布。
2015年6月4日，OpenCV 3.0版本发布。
2015年7月30号，OpenCV2.4.12版本发布。
2015年12月21日，OpenCV3.1版本发布。
2016年12月23日，OpenCV3.2版本发布。
2017年8月3日，OpenCV3.3版本发布。
