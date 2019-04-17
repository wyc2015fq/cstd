# OpenCV学习笔记（十）——图形交互和媒体接口HighGUI - 迭代的是人，递归的是神 - CSDN博客





2011年10月24日 17:11:55[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：21399
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)











OpenCV提供一个功能强大的UI接口，可以在MFC、Qt、WinForms、Cocoa等平台下使用，甚至不需要其他的平台。新版本的HighGUI接口包括：

创建并控制窗口，该窗口可以显示图片并记录其内容

为窗口添加了trackbars控件，可以方便利用鼠标进行控制而不是之前版本的只能利用键盘

读写硬盘和内存的图片

读取摄像头的视频、读写视频文件

先来介绍UI，包括函数createTrackbar、getTrackbarPos、setTrackbarPos、imshow、namedWindow、destroyWindow、destroyAllWindows、MoveWindow、ResizeWindow、SetMouseCallback、waitKey。这些函数保证了图像的基本处理、tarckbar的控制和鼠标键盘的响应

介绍一下读写图像视频的函数：图像相关的函数有imdecode、imencode、imread、imwrite。读取视频相关为VideoCapture类，负责捕捉文件和摄像头的视频，该类内有成员函数VideoCapture、open、isOpened、release、grab、retrieve、read、get、set，写视频的类为VideoWriter，类内有成员函数VideoWriter、open、isOpened、write

新版本还为Qt做了新函数，这里就不介绍了，有兴趣的朋友可以自己看一下参考手册的第四章第三节。

这里介绍几个常用的新功能，首先介绍一下添加滑杆控件Trackbar。调用函数为：

```cpp
createTrackbar( TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar );
```


第一个参数为字符串作为标签，第二个参数为所在窗口的名字，第三个参数为存储滑杆位置的值地址，其范围为0~alpha_slider_max（第四个参数），最后一个参数为移动滑杆时调用的回调函数名。

OpenCV2.0版本加强了对视频处理的支持，不再需要对一组连续的图片进行处理，可以进行实时的图像采集和记录以及存储。视频的操作基本都被封装在VideoCapture类中。打开视频可以可以通过如下代码实现：

```cpp
VideoCapture captRefrnc(sourceReference);
// or
VideoCapture captUndTst;
captUndTst.open(sourceCompareWith);
```


其中sourceReference和sourceCompareWith为string型，为文件名。还可以通过isOpened函数检测视频是否成功打开。也可以调用release函数提前关闭视频。还可以讲VideoCapture放到Mat结构中，因为视频流是一连串的，可以通过read函数或>>操作符逐帧的读取，例如：

```cpp
Mat frameReference, frameUnderTest;
captRefrnc >> frameReference;
captUndTst.open(frameUnderTest);
```


read函数只能逐帧的抓取，如果要抓取某一帧，可以成对的调用grab函数和retrieve函数。get函数可以获取视频相关信息。set函数可以控制视频的一些值，比如是指视频的当前位置或帧数。

可以使用VideoWriter类创建新视频，其open，isOpened函数调用方法类似，write函数或<<运算符向视频写入内容，可以使用split函数和merge函数单独调整RGB通道的值

今日，被一个网友指出，说OpenCV以前提供的读写功能采用VFW，效率低下且有些格式支持不好。而 OpenCV 2.0 内置了videoInput Library，可以自动在VFW和DirectShow间切换。videoInput是老外写的功能强大的开源视频处理库。是一个第三方库，2.0~2.2的版本专门有一个3rdparty对该库进行支持，而在最新的2.3版本中，已经讲videoInput库集成到highgui中了，想使用它的话，只需要在cmake中设置宏WITH_VIDEOiNPUT=OFF/ON即可。

以后有新学到的东西都会陆续补充进来。





