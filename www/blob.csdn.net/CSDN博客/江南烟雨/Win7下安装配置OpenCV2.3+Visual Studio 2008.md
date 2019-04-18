# Win7下安装配置OpenCV2.3+Visual Studio 2008 - 江南烟雨 - CSDN博客
2011年11月18日 15:25:38[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3298
一、下载OpenCV2.3.0
下载地址：[http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.3/](http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.3/)
文件名：OpenCV-2.3.0-win-superpack.exe
二、解压上述文件
双击，解压至C盘根目录下。默认文件名OpenCV2.3。
![](http://hi.csdn.net/attachment/201111/18/0_1321600418ruyk.gif)
三、配置Visual Studio 2008
点击 工具->选项->项目和解决方案->VC++目录。
![](http://hi.csdn.net/attachment/201111/18/0_1321600603vLkq.gif)
在“包含文件”下添加以下两个路径：
C:\OpenCV2.3\opencv\include\opencv
C:\OpenCV2.3\build\include
![](http://hi.csdn.net/attachment/201111/18/0_13216006361v1P.gif)
添加库文件
C:\OpenCV2.3\build\x86\vc9\lib
![](http://hi.csdn.net/attachment/201111/18/0_1321600694DJSR.gif)
四、配置环境变量
右键单击计算机，单击属性，点击高级系统设置。
![](http://hi.csdn.net/attachment/201111/18/0_1321600814MZGf.gif)
点击环境变量，在用户变量中添加：
C:\OpenCV2.3\build\x86\vc9\bin
名称为path，若path环境变量已经存在，则选择编辑。
![](http://hi.csdn.net/attachment/201111/18/0_1321600915K8pu.gif)
五、工程文件属性设置
在新建一个工程时，要在“附加依赖项”中添加一些内容。
debug版本的配置，在“附加依赖库”中添加以下内容：（中间用空格隔开）
opencv_features2d230d.lib opencv_highgui230d.lib opencv_core230d.lib opencv_imgproc230d.lib opencv_ml230d.lib opencv_objdetect230d.lib opencv_video230d.lib opencv_contrib230d.lib opencv_calib3d230d.lib
![](http://hi.csdn.net/attachment/201111/18/0_1321601034wjiJ.gif)
release版本的配置，在“附加依赖库”中添加以下内容：（中间用空格隔开）
opencv_features2d230.lib opencv_highgui230.lib opencv_core230.lib opencv_imgproc230.lib opencv_ml230.lib opencv_objdetect230.lib opencv_video230.lib opencv_contrib230.lib opencv_calib3d230.lib
![](http://hi.csdn.net/attachment/201111/18/0_1321601099r51L.gif)
到此为止，就全部配置好了。
可以测试一下。新建一个空项目，再添加一个cpp文件，粘贴代码：
```cpp
#include <cv.h>
#include <highgui.h>
int main()
{
	const char* filename = "C:\\lena.jpg" ;
	const char* wndname = "Test" ;
	IplImage* pImage = cvLoadImage(filename,CV_LOAD_IMAGE_COLOR) ;
	cvNamedWindow(wndname,CV_WINDOW_AUTOSIZE);
	cvShowImage(wndname,pImage);
	cvWaitKey(0);
	cvReleaseImage(&pImage);
	cvDestroyWindow(wndname);
	return 0;
}
```
安装上面的步骤对项目属性进行设置。
运行：
![](http://hi.csdn.net/attachment/201111/18/0_1321601281uhlU.gif)
参考文章：[http://blog.csdn.net/moc062066/article/details/6676117](http://blog.csdn.net/moc062066/article/details/6676117)
