# OpenCV学习笔记（五十九）——marker检测识别"Master OpenCV"chp.2

 																				2013年01月30日 12:24:12 					[yang_xian521](https://me.csdn.net/yang_xian521) 						阅读数：26991 										

 									

第二章原本是讲如何将基于标定的增强现实在ios平台实现，包括以下4个方面：

1、在ios平台建立opencv工程

2、Marker检测识别

3、摄像机标定及Marker姿态估计

4、在Marker基础上渲染一个3维虚拟物体

这里面第一部分是IOS平台的开发，我不是太关注，略去；第四部分是基于OpenGL的3维虚拟物体建立，也是基于IOS平台，因为第三章里还要用到OpenGL，这里留着第三章再解剖。所以这里主要分析第二部分和第三部分。这一篇介绍第二部分。感觉这个东西有点像二维码识别。不知道2维码是怎么做的哦

MarkerDetection类任务processFrame：图1

![img](https://img-my.csdn.net/uploads/201301/30/1359511535_1008.png)

转为灰度图、2值化（固定阈值法threshold：受光照等影响明显；自适应阈值法adaptiveThreshold：更好）（我这里用OpenCV243里的adaptiveThreshold函数未能实现自适应滤波的效果，效果像边缘检测的算法，很困惑。。最后用threshold函数代替，这个问题未能解决，希望高手指点，ps：网上高手多啊，[这个问题已经解决了](http://www.opencv.org.cn/forum/viewtopic.php?t=3355)），检测后的结果如图threshold（图1左上）

findMarkerContours函数进行轮廓检测findContours（用多边形的顶点最好，去掉小于阈值的点（对小的轮廓不感兴趣），把每个轮廓的点按照逆时针排序，并去掉距离太近的轮廓），结果如图contours（图1中上）

接下来findMarkerCandidates函数对轮廓进行筛选，先用approxPolyDP得到轮廓近似的多边形。进行筛选，为凸多边形且顶点为4的才有可能是marker，并检测这个4边形的边长，最小边长如果小于10pixel，也不认为是一个marker。然后把得到的可能的marker的轮廓点按照逆时针排序。并且检测是否检测到重复的marker，如果检测到重复的marker，去掉周长更短的那个。这步之后效果如下markerCandidate（图1左下）

detectMarkers函数有3个任务，去除投影变换的影响（getPerspectiveTransform得到投影矩阵，warpPerspective得到正面的视角的图像），得到marker正面的视图。

然后对这个marker的正面图进行解码，threshold对marker使用THRESH_OTSU进行2值化。效果图：

![img](https://img-my.csdn.net/uploads/201301/30/1359513529_4427.png)

接下来对这个marker进行识别marker.decode。检测编码marker（对marker解码，marker编码为7*7的栅格，中心5*5为ID，周围一圈为黑色边界，检测的时候先检测周围一圈是否为黑色边界，然后再对中心5*5解码（注意，只有5*5具有旋转不变形才能得到唯一的码），是5bit*5word，每个word中的5bit，2位为id（2位4位），3位为校验码（用来保证旋转），所以5word一共有2^10=1024个不同id，而且第一位要置反，目的是要防止一个word全黑，不易检测。举例，我这里使用的marker的5个word的id分别为10、01、11、11、11。那么如何从刚刚得到的marker图提取出7*7的2值栅格呢，这里用个Mat(Rect)取marker中的小方块，用countNonZero来判断这个方块为0or1。因为marker有4个方向，哪个方向才是我对应的marker的id的，这里用id和我验证用的id的hamming距离来做依据，汉明距最小的即marker的方向。

确认为一个marker后再得到轮廓的细致的corner，使用cornerSubPix，这时才进行细化，是因为这个函数相对耗时，如果之前就对各corner细化，由于候选目标很多，会加重计算负担。）效果图marker（图1右下）。

最后我试了其他的marker编码，都能正确解码出id信息，效果图如下：第一幅图的id为0011010101，第二幅图为第一图的旋转，id相同，第三图id为0011000110，第四图不是一个marker，故没有检测出来。

![img](https://img-my.csdn.net/uploads/201301/30/1359520087_2904.png)

代码的下载地址：<http://download.csdn.net/detail/yang_xian521/5040634>

在下一篇里，将介绍如何用这个marker的轮廓位置，和轮廓（红色）的方向（黄点）来在marker上建立一个3维的虚拟物体。