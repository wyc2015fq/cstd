# OpenCV视频进度播放控制 - wishchinYang的专栏 - CSDN博客
2015年09月09日 08:32:02[wishchin](https://me.csdn.net/wishchin)阅读数：1458
       本来打算把进度条嵌入MFC的PIC空间里面，结果显示进度条消失，看来还是不要这个样子了。
全局变量区域：
```cpp
//2.初始化进度条的位置
int   G_slider_position   =     0;
CvCapture*    G_capture   =  NULL;
//cv::VideoCapture    G_capture   =  NULL;
bool    G_StatePlay = false;
```
全局回调函数：
```cpp
//2.进度条的回调函数，播放进度控制
void onTrackbarSlide(int  Pos)
{ 
	cvSetCaptureProperty(G_capture, CV_CAP_PROP_POS_FRAMES, Pos);//设置视频
	cvSetTrackbarPos("Trackbar Name","VideoBar" , Pos);
	IplImage* frameS=cvQueryFrame(G_capture);
	cv::Mat frameDst(240,320,CV_8UC3);
	//resizeImage( frameS, frameDst);
	cv::Mat  MatSrc( (frameS) ) ;
	cv::resize(MatSrc,frameDst,frameDst.size() );
	cv::imshow("VideoBar",frameDst);
	
	cv::waitKey(0);
	cvReleaseImage(&frameS); 
}
```
控制播放的代码：
```cpp
bool CPicToolsDlg::initTrackBar()
{
	//捕获视频
	//G_capture=cvCreateFileCapture("video.long.raw.avi");
	//获取视频的帧数
	int frames = (int) cvGetCaptureProperty(G_capture,CV_CAP_PROP_FRAME_COUNT);
	//从第N-100帧开始显示
	int Pos =100;
	cvSetCaptureProperty(G_capture,CV_CAP_PROP_POS_FRAMES,Pos);  
	//建立进度条
	if(frames!=0)
	{
		cvNamedWindow("VideoBar",CV_WINDOW_AUTOSIZE);
		cvCreateTrackbar("Trackbar Name","VideoBar" ,&G_slider_position,frames, onTrackbarSlide);
	}
	return true;
}
```
控制效果：
![](https://img-blog.csdn.net/20150909083123633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

