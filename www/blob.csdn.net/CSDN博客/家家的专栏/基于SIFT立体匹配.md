# 基于SIFT立体匹配 - 家家的专栏 - CSDN博客





2010年10月29日 11:16:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1927
个人分类：[图像处理算法](https://blog.csdn.net/yihaizhiyan/article/category/715109)









## [视觉（13）基于SIFT立体匹配的移动机器人自定位](http://www.cnblogs.com/cutepig/archive/2007/07/18/822997.html)


主要思路如下：
1.找特征：
对上一帧拍摄的左右图像分别计算SIFT特征，保存为L1.key,R1.key。
对当前帧拍摄的左右图像分别计算SIFT特征，保存为L2.key,R2.key。
2.特征匹配：
匹配L1.key,R1.key，得到文件K1.match。
匹配L2.key,R2.key，得到文件K2.match。
匹配K1.match，K2.match，得到result.txt，这个文件每一行包含了这四张图像的同一个特征点。
3.计算位姿的相对变换

现在1，2已经写好程序（一个简陋的命令行程序），一般能得到几百个匹配的特征，但是里面有一些特征匹配是错误的。
下一步可以考虑在计算3时采用RANSAC消除掉错误匹配的影响。

![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)转换为pgm文件
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)找特征点
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>siftWin32.exe  0<L1.pgm 1>L1.key
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)Finding keypoints![](http://www.cnblogs.com/Images/dot.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)2473 keypoints found.
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>siftWin32.exe  0<R1.pgm 1>R1.key
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)Finding keypoints![](http://www.cnblogs.com/Images/dot.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)2945 keypoints found.
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>siftWin32.exe  0<L2.pgm 1>L2.key
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)Finding keypoints![](http://www.cnblogs.com/Images/dot.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)2712 keypoints found.
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>siftWin32.exe  0<R2.pgm 1>R2.key
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)Finding keypoints![](http://www.cnblogs.com/Images/dot.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)3536 keypoints found.
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)匹配并显示（1）
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>match -m1 L1.key R1.key k1.match
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)k1:2473,k2:2945
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)Found 928 matches.
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>match -draw k1.match L1.pgm R1.pgm
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)klist:928
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)匹配并显示（1）
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>match -m1 L2.key R2.key k2.match
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)k1:2712,k2:3536
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)Found 1032 matches.
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>match -draw k2.match L2.pgm R2.pgm
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)klist:1032
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)匹配（2）
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)>match -m2 k1.match k2.match result.txt
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)k1:928,k2:1032
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)find 573 matches



转自:http://www.cnblogs.com/cutepig/archive/2007/07/18/822997.html



