# OpenCV学习笔记（二十）——手势识别

2011年11月02日 10:25:34

yang_xian521

阅读数：43480

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

最近也在玩手势识别，资料找了很多，基本可以分为静态手势识别和动态手势识别，先弄个简单的静态手势识别给大家看看。

基本流程如下：

先滤波去噪-->转换到HSV空间-->根据皮肤在HSV空间的分布做出阈值判断，这里用到了inRange函数，然后进行一下形态学的操作，去除噪声干扰，是手的边界更加清晰平滑-->得到的2值图像后用findContours找出手的轮廓，去除伪轮廓后，再用convexHull函数得到凸包络。

结果如下：

![img](http://hi.csdn.net/attachment/201111/4/0_1320368184UUpQ.gif)

源代码下载位置：<http://download.csdn.net/detail/yang_xian521/3746669>，有点贵哦，当时随便设了个值，呵呵