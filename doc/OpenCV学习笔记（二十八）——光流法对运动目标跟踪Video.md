# OpenCV学习笔记（二十八）——光流法对运动目标跟踪Video

2011年11月18日 18:18:07

yang_xian521

阅读数：38230

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

OpenCV配套的教程Tutorials对于Video的部分，没有实例进行说明，我只能摸石头过河啦，之前试过一个camShift做目标检测，这次试一试光流法做运动估计。这里使用的光流法是比较常用的 Lucas-Kanade方法。对于光流法的原理，我就不过多介绍了，主要讲使用OpenCV如何实现。

首先利用goodFeaturesToTrack函数得到图像中的强边界作为跟踪的特征点，接下来要调用calcOpticalFlowPyrLK函数，输入两幅连续的图像，并在第一幅图像里选择一组特征点，输出为这组点在下一幅图像中的位置。再把得到的跟踪结果过滤一下，去掉不好的特征点。再把特征点的跟踪路径标示出来。说着好简单哦~~

程序的效果和代码下载<http://download.csdn.net/detail/yang_xian521/3811478>

![img](http://hi.csdn.net/attachment/201111/18/0_13216040982xIj.gif)