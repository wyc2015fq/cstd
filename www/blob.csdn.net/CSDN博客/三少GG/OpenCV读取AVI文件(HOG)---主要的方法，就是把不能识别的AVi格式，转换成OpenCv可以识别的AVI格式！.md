# OpenCV读取AVI文件(HOG)---主要的方法，就是把不能识别的AVi格式，转换成OpenCv可以识别的AVI格式！ - 三少GG - CSDN博客
2011年03月23日 20:58:00[三少GG](https://me.csdn.net/scut1135)阅读数：5856

4.   [http://villager5.blog.163.com/blog/static/827363620091066282328/](http://villager5.blog.163.com/blog/static/827363620091066282328/)
opencv写入的视频如何显示出来？
步骤一.     [http://hq.huang.blog.163.com/blog/static/1066821912009758958885/](http://hq.huang.blog.163.com/blog/static/1066821912009758958885/)
OpenCV读取AVI文件
今天看到网上很多人问，cvCreateFileCapture读取avi，为什么总是返回NULL. 我查了查文献，总结如下：
（源程序附在最下）
问题：为什么我的电脑支持AVI或者能够播出AVI，但为什么使用cvCreateFileCapture函数总返回NULL呢？
答案：尽管是AVI文件，但也可能使用了某种codec，例如:MJPEG Decompressor。 需要把它转换OpenCV支持的AVI文件. OpenCV支持的AVI如下：
|Container|FourCC|Name|Description|
|----|----|----|----|
|AVI|'DIB '|RGB(A)|Uncompressed RGB, 24 or 32 bit|
|AVI|'I420'|RAW I420|Uncompressed YUV, 4:2:0 chroma subsampled|
|AVI|'IYUV'|RAW I420|identical to I420|
转换格式解决方法：
解决方法1：[下载mencoder.exe](http://oss.netfarm.it/mplayer-win32.php)， 在window命令行下使用：
 mencoder in.avi -ovc raw -vf format=i420 -o out.avi
（注：我测试了这个方法，没有成功，原因不详，希望有朋友们能够详细讨论一下。）
**解决方法2：[下载VitualDub](http://www.virtualdub.org/download.html), 我使用1.9.4版本**
**a. File->Open Video File;**
**b. Video->Filters->Add->Convert format; 选择4:2:0 Planar YCbCr (YV12)或者 32-Bit RGB。**
**c. Save as AVI. 保存完毕。**
**(注：成功使用。)**
大家可以测试一下。如果还是不行，请回复，共同讨论一下。
我的源代码，(VS C++ 2008 Expression, WinXP sp3, )
//------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace std;
int main(int argc, char** argv[])
{
int key=0;
char* filename="D:/fruit.avi";
CvCapture* capture = cvCreateFileCapture(filename);
double fps=cvGetCaptureProperty(capture, CV_CAP_PROP_FPS );
cout<<"fps="<<fps<<endl;//print the frame rate per second
if(capture==NULL) {
cout<<"NO capture"<<endl;
char a=getchar();
return 1;
};    
IplImage* frame;
cvNamedWindow("PlayAVI", CV_WINDOW_AUTOSIZE);
while(1) {
frame = cvQueryFrame( capture );
if(!frame) break;
cvShowImage("PlayAVI", frame );
key = cvWaitKey(33);// quit when users press 'ESC'
if( key == 27 ) break;
}
cvReleaseCapture(&capture);
cvDestroyWindow("PlayAVI"); 
return 0;
}
步骤二.
# 用openCV 写灰度视频
**cvCreateVideoWriter问题生成视频文件大小为0**
#include "cvaux.h"
#include "highgui.h"
#include <stdio.h>
int main()
{
    IplImage*      current_frame = 0; 
    IplImage*      masked_frame = 0;
CvCapture*      cap = 0;          //读视频
CvVideoWriter*  wrt = 0;          //写视频
cvNamedWindow("image show", 1);  
cap= cvCaptureFromCAM(0);
cvGrabFrame( cap);
current_frame  = cvRetrieveFrame( cap);//cvQueryFrame(cap)
wrt = cvCreateVideoWriter("result.avi", -1, 2.0, cvGetSize(current_frame),1); 
if(!current_frame || !wrt )
    {
        printf("bad video n");
        exit(0);
    }
for( ;;)
{ 
cvGrabFrame( cap);
current_frame  = cvRetrieveFrame( cap);
cvWriteFrame(wrt, current_frame);
cvShowImage( "image show",current_frame );
if( cvWaitKey(20)>= 0 )//等待10毫秒
                break;
}
cvReleaseCapture(&cap);
cvReleaseVideoWriter(&wrt);
cvDestroyWindow("image show"); 
    return 0;
}
把**编码方式改为****-1，也就是出现一个压缩编码方式和参数选择的对话框，就可以实现功能。**
**==================================================================**
**======================================================================**
**R**eference:★★★★★
1.
> 
写视频文件关键在于各参数的设定，如编码器，帧率，彩色or gray 帧编码
通过函数cvCreateVideoWriter () 构造一个“视频文件写手”CvVideoWriter*
> 
## cvCreateVideoWriter
创建视频文件写入器
typedef struct CvVideoWriter CvVideoWriter;
CvVideoWriter* cvCreateVideoWriter( const char* filename, int fourcc, double fps, CvSize frame_size, int is_color=1 );
- filename 
输出视频文件名。- fourcc 
四个字符用来表示压缩帧的codec 例如，CV_FOURCC('P','I','M','1')是MPEG-1 codec， CV_FOURCC('M','J','P','G')是motion-jpeg codec等。 在Win32下，如果传入参数-1，可以从一个对话框中选择压缩方法和压缩参数。- fps 
被创建视频流的帧率。- frame_size 
视频流的大小。- is_color 
如果非零，编码器将希望得到彩色帧并进行编码；否则，是灰度帧（只有在Windows下支持这个标志）。
函数cvCreateVideoWriter创建视频写入器结构。
默认情况下，is_color = 1. 因此当你设为0 时即告诉写手你要构建灰度视频，通过循环中cvWriteFrame() 一帧一帧写入即可。
> 
## cvWriteFrame
写入一帧到一个视频文件中
int cvWriteFrame( CvVideoWriter* writer, const IplImage* image );
- writer 
视频写入器结构。- image 
被写入的帧。
函数cvWriteFrame写入／附加到视频文件一帧。
- 返回：
成功返回1,不成功返回0。
注意：此处image 即帧图像应该和writer 类型（彩色或灰度）保持一致，即
如果写灰度视频，image 需要是单通道，若为多通道图像需做转换：
[view plain](http://blog.csdn.net/zhmyy/archive/2010/03/12/5374746.aspx)[copy
 to clipboard](http://blog.csdn.net/zhmyy/archive/2010/03/12/5374746.aspx)[print](http://blog.csdn.net/zhmyy/archive/2010/03/12/5374746.aspx)[?](http://blog.csdn.net/zhmyy/archive/2010/03/12/5374746.aspx)
- CvVideoWriter* wrVideo = cvCreateVideoWriter("out.AVI",CV_FOURCC_DEFAULT,fps,size,0);  
- 
- 
- 
- while(pFrame =cvQueryFrame(pCapture))  
- 
- {  
- pBkImg=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);  
- 
- cvCvtColor(pFrame,pBkImg,CV_BGR2GRAY);  
- cvWriteFrame(wrVideo,pBkImg);  
- 
- if(cvWaitKey(2)>=0)  
- break;  
- }  
- 
- cvReleaseCapture(&pCapture);  
-  cvReleaseVideoWriter(&wrVideo);  
2.
今天电脑中毒了，杀毒后删了一些软件，然后重启，打开写好的程序，**发现opencv中的cvCreateFileCapture一直是NULL，怎么也打不开，程序没有任何问题，还是原来的程序，郁闷，后来才发现是XviD codec解码器被删除了，**下载后一切OK! 哈哈，网址：http://www.skycn.com/soft/11696.html eagle 2010-02-07
3.
### [恶心的cvCreateVideoWriter](http://www.opencv.org.cn/forum/viewtopic.php?t=6784#p25993)
![帖子](http://www.opencv.org.cn/forum/styles/prosilver/imageset/icon_post_target.gif)由 **[yaos](http://www.opencv.org.cn/forum/memberlist.php?mode=viewprofile&u=5401&sid=b32809033d260cced0eb70c08d061ab1)** »
 2009-06-18 16:53
我的开发环境VC6.0 MFC
第二个参数，我试过了
CV_FOURCC('P','I','M','1'),
CV_FOURCC('M','J','P','G'),
CV_FOURCC('D','I','V','3'),
CV_FOURCC('D','I','V','X'),
CV_FOURCC('X','V','I','D')，
-1
- 代码: [全选](http://www.opencv.org.cn/forum/viewtopic.php?t=6784)
```
frame = cvQueryFrame(pCapture);
cstr.Format("_seg_%03d",videonum);
pos = destDirectoryFile.ReverseFind('.');
destDirectoryFile.Insert(pos,cstr);
//cvSaveImage("test.jpg", frame);
//pVideo = cvCreateVideoWriter(destDirectoryFile, CV_FOURCC('P','I','M','1'), (int)m_framerate, cvGetSize(frame), 1);   
pVideo = cvCreateVideoWriter("c://out.avi", CV_FOURCC('D', 'I', 'V', '3'), m_framerate, cvGetSize(frame), 1);
// 一直pVideo == NULL
if (pVideo == NULL)
{
   MessageBox("Video File Create Error!", "Error", NULL);
   return ;
}
for (i=0; i<nNumFrame && index[i]==0; i++)
{
   cvWriteFrame(pVideo, frame);
   frame = cvQueryFrame(pCapture);
}
if(pVideo != NULL)
{
   cvReleaseVideoWriter(&pVideo);
   pVideo = NULL;
}
```
pVideo = cvCreateVideoWriter("c://out.avi", ('D', 'I', 'V', '3'), m_framerate, cvGetSize(frame), 1);
这行一直返回NULL，不能创建视频写入器结构，谁能给解释一下
**自行了断了：先获取视频编码属性，然后设置写入器编码属性，就能创建成功了outCompressCodec = (int)cvGetCaptureProperty(pCapture, CV_CAP_PROP_FOURCC);pVideo = cvCreateVideoWriter("c://out.avi", outCompressCodec, m_framerate, cvGetSize(frame), 1);**
