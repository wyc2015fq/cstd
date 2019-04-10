# OpenCV学习笔记（六十二）——《OpenCV Computer Version with Python》阅读摘要

 																				2014年08月30日 08:29:19 					[yang_xian521](https://me.csdn.net/yang_xian521) 						阅读数：10521 										

 									

现在python火啊，每次OpenCV自带的ml模块都让我直呼坑爹，索性准备用python来做OpenCV后期的机器学习算法的处理。于是赶紧拿起这本书读读。

适合OpenCV和python都有一定基础的。。。。由于都比较熟悉这两个东西，我阅读之前比较关心的只有几个问题，具体的应用实例没有仔细看。

1.如何在python中安装opencv

2.OpenCV的Mat数据结构能否方便的转换成numpy的array结构

3.OpenCV的GUI模块在python里好用么

4.二者还能擦出什么我想不到的火花么。。。。

书中提到在windows系统中，python-32bit表现的比64bit要好，推荐安装32位的python

第一个问题在windows下很简单，OpenCV安装好之后，找到目录<build_folder>\lib\Release\cv2.pyd(from a Visual Studio build) 这个文件，然后copy到C:\Python2.7\Lib\site-packages。搞定了，就这么简单。毕竟脚本语言，简直无情，\sources\samples下有很多python的例子，跑几个试试就知道是否安装好了。import  cv2这句就可以导入cv2模块了

第二个问题也不用担心了，因为python不用声明变量的类型，实验了一下，发现得到的矩阵的数据类型就是array，稳了，直接拿来用。

第三个问题也超简单，图像显示读写的模块、摄像头模块、鼠标键盘的响应模块都可以，跟c++的版本使用起来也差不多。

第四个问题我简单粗看了一遍书，没发现什么亮点，只是书中提到一个pygame可以用来做hgui效果还行，支持画画和编辑文本，不过好像对CV也没啥帮助，所以就没研究了。

补充几个我学习的时候遇见的问题：

opencv里的Rect数据结构在python里是没有对应类型的，这个要注意调用的时候需要注意。比如rectangle函数输入的就是矩形两个点的坐标，不是Rect。

还有就是opencv里的很多宏在python里需要加上cv2.cv前缀就可以生效了。