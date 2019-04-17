# OpenCV学习笔记（十九）——浅尝contrib - 迭代的是人，递归的是神 - CSDN博客





2011年11月01日 18:37:30[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：11907
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









contrib为最新贡献但不是很成熟的函数库。作为最新的东西，就更有价值进行庖丁解牛了，我来也。

首先介绍一个CvAdaptiveSkinDetector类。该类的功能是自适应的皮肤检测。分析了一下代码，其构造函数的输入参数有两个，samplingDivider样本分类，morphingMethod为变形方法。该类的关键函数为process函数，该函数先将输入图像由RGB转换为HSV空间，Hue的范围是3~33，Intensity（V）的范围为15~250。然后进行必要的腐蚀膨胀，去除噪声，使轮廓更加清晰平滑。具体的使用代码参考sample文件夹中的adaptiveskindetector.cpp






