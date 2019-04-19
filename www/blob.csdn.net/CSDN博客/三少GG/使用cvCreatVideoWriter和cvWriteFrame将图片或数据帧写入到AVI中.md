# 使用cvCreatVideoWriter和cvWriteFrame将图片或数据帧写入到AVI中 - 三少GG - CSDN博客
2011年03月23日 22:06:00[三少GG](https://me.csdn.net/scut1135)阅读数：7729
[http://ellenberger.blog.163.com/blog/static/88796664200891711120835/](http://ellenberger.blog.163.com/blog/static/88796664200891711120835/)
实现此操作时走了很多错路，两个函数的用法也不是很复杂，关键问题是去了解OpenCV的机制。经过这几天的学习，OpenCV搜索系统编解码器。
使用cvCreateVideoWriter时，在其参数中必须指定编码方式。
cvCreateVideoWriter
创建视频文件写入器
typedef struct CvVideoWriter CvVideoWriter;
CvVideoWriter* cvCreateVideoWriter( const char* filename, int fourcc, double fps, CvSize frame_size, int is_color=1 );
filename
输出视频文件名。
fourcc
四个字符用来表示压缩帧的codec 例如，CV_FOURCC('P','I','M','1')是MPEG-1 codec， CV_FOURCC('M','J','P','G')是motion-jpeg codec等。在Win32下，如果传入参数-1，可以从一个对话框中选择压缩方法和压缩参数。
fps
被创建视频流的帧率。
frame_size
视频流的大小。
is_color
如果非零，编码器将希望得到彩色帧并进行编码；否则，是灰度帧（只有在Windows下支持这个标志）。
函数cvCreateVideoWriter创建视频写入器结构。
使用以下操作
pWriter = cvCreateVideoWriter(strAviName,CV_FOURCC('D','I','V','X'),fps,cvSize(nFrameW,nFrameH),1);
cvWriteFrame(pWriter,pFrame);
在系统中包含DIVX编码器的前提下，可以将pFrame写入AVI文件strAviName中。
======================================================================================
Reference:
1. openCV数据结构
[http://www.cnblogs.com/pangliang/archive/2010/11/21/1883566.html](http://www.cnblogs.com/pangliang/archive/2010/11/21/1883566.html)
2.  [http://fzzl.is-programmer.com/posts/9774.html](http://fzzl.is-programmer.com/posts/9774.html)
先开一个CvCapture及IplImage,用cvCreateCameraCapture()选定视频装置编号,给窗口命名,接着,就是写入AVI文件的重头戏啦,用CvVideoWriter建立一个writer的数据结构,给它AVI文件名,AVI格式,FPS(frame per second)每秒图片播放频率(即帧率),CvSize数据结构及Avi文件的颜色种类,再用cvWriteFrame()写入AVI文件。**最后,记得用 cvReleaseVideoWriter()把CvVideoWriter数据结构释放,cvReleaseVideoWriter()非常重要,因为AVI文件的文件格式收尾就是要用到cvReleaseVideoWriter(),也就是说,如果不用到 cvReleaseVideoWriter()，AVI文件就无法打开.**这边,先从cvCreateCameraCapture()详细叙述.
对于多视频装置的实例,假如,有两Window XP(VFW)的视频装置要控制,则用两个相同的cvCreateCameraCapture()函数及两个相同的CV_CAP_VFW代号,而OpenCV会有接口来选择目标装置.
cvCreateCameraCapture(装置数字代号);
**cvCreateVideoWriter()**它要放的参数分别为AVI文件名,AVI格式,FPS,CvSize数据结构及AVI文件的颜色种类,AVI的格式有很多种,由于影片有很多各个不同的压缩格式,因此,被分为非常多类,许多的种类也只是在学术界采用,real player及media player很多都不适用,这边压缩格式的规格分为:
CV_FOURCC('P','I','M','1') MPEG-1 codec
CV_FOURCC('M','J','P','G') motion-jpeg codec (does not work well)
CV_FOURCC('M', 'P', '4', '2') MPEG-4.2 codec
CV_FOURCC('D', 'I', 'V', '3') MPEG-4.3 codec
CV_FOURCC('D', 'I', 'V', 'X') MPEG-4 codec
CV_FOURCC('U', '2', '6', '3') H263 codec
CV_FOURCC('I', '2', '6', '3') H263I codec
CV_FOURCC('F', 'L', 'V', '1') FLV1 codec
它的压缩格是参数方法被定义在"highgui.h"里的
#define CV_FOURCC(c1,c2,c3,c4)     /
            (((c1)`&`255) + (((c2)`&`255)<<8) + (((c3)`&`255)<<16) + (((c4)`&`255)<<24))
如果要使用MPEG-1则直接使用int AviForamt=CV_FOURCC('P','I','M','1');就好啦,不过光是mpge-1压缩格式real play跟media play就不支持播放,比较建议的是直接用-1,会有GUI接口帮你自己选择文件格式.虽然有些格是很多播放程序也是不支持,不过用起来方便许多.再 来,FPS,每秒Frame的播放速度,frame有很多的翻译,也可以叫它做框页,用来做每秒画面显示的速度.AVI颜色种类,这个其实就很简单啦,只 有0跟1,0是黑白,1是彩色.
cvCreateVideoWriter("AVI档名",AVI格式参数或代号,FPS,AVI大小CvSize数据结构,AVI颜色种类代号);
**cvQueryFrame()**其实这是两个子程序组合而成的,分别是cvGrabFrame()+cvRetrieveFrame(),这两个子程序的实作会在下面的程序代码用出.
IplImage* cvQueryFrame(capture数据结构);
**cvWriteFrame()**
将frame写入writer数据结构.
cvWriteFrame(writer数据结构,frame数据结构);
**cvReleaseVideoWriter()**
释放writer数据结构,这边cvReleaseVideoWriter()不只做了释放数据结构的功能,还包括的AVI文件格式的结尾制作,再做写入AVI档时一定要有cvReleaseVideoWriter()做结尾.
cvReleaseVideoWriter(writer数据结构)
--
附注:opencv1.1 取消了AviForamt=-1的功能,并且要挂上ffopencv的函数库才能使用,关于avi文件的存取,必须要去Yahoo Group下载ffopencv110.dll覆盖到C:/Program Files/OpenCV/bin的目录底下
然后只能在Visual Studio的平台才能使用.
**ffopencv使用的是ffmpeg library,ffmpeg只支持gcc 4.3版以上的编译器,而Dev-C++使用的是gcc 3.4.2版的编译器,因此在Dev-C++使用Opencv 1.1播放avi档还是会有Error.**
