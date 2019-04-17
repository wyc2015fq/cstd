# OpenCV学习笔记（四十七）——VideoWriter生成视频流highgui - 迭代的是人，递归的是神 - CSDN博客





2012年04月09日 16:38:09[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：86275
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









首先要先纠正个误区，我见有人用OpenCV做多媒体开发，真的是很搞笑，OpenCV这东西再强大，这方面也不行的，之所以把视频读取写入这部分做的强大一些，也是为了方便大家做视频处理的时候方便些，而且这部分也是基于vfw和ffmpeg二次开发的，功能还是很弱的。一定要记住一点，OpenCV是一个强大的计算机视觉库，而不是视频流编码器或者解码器。希望大家不要走入这个误区，可以把这部分简单单独看待。目前，OpenCV只支持avi的格式，而且生成的视频文件不能大于2GB，而且不能添加音频。如果你想突破这些限制，我建议你最好还是看看ffMpeg，而不是浪费时间在OpenCV上。不过也可以利用视频后期合成工具制作。闲言少叙，进入重点VideoWriter类。

这个类是highgui交互很重要的一个工具类，可以方便我们容易的将图片序列保存成视频文件。类内成员函数有构造函数，open，isOpened，write（也可以用<<），使用还是很简单的。

使用很简单，先调用构造函数确定文件的名称，格式，帧率，帧大小，是否彩色。其中格式作为第二个参数，OpenCV提供的格式是未经过压缩的，目前支持的格式如下：




CV_FOURCC('P', 'I', 'M', '1') = MPEG-1 codec

CV_FOURCC('M', 'J', 'P', 'G') = motion-jpeg codec

CV_FOURCC('M', 'P', '4', '2') = MPEG-4.2 codec 

CV_FOURCC('D', 'I', 'V', '3') = MPEG-4.3 codec 

CV_FOURCC('D', 'I', 'V', 'X') = MPEG-4 codec 

CV_FOURCC('U', '2', '6', '3') = H263 codec 

CV_FOURCC('I', '2', '6', '3') = H263I codec 


CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec

然后就<<不停的把image传进去就行啦，太简单了，我都不好意思写了。上代码：



```cpp
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void main()
{
	VideoCapture capture(0);
	VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));
	Mat frame;
	
	while (capture.isOpened())
	{
		capture >> frame;
		writer << frame;
		imshow("video", frame);
		if (cvWaitKey(20) == 27)
		{
			break;
		}
	}
}
```





