# OpenCV 2.1.0 with Visual Studio 2010(1) - 三少GG - CSDN博客
2010年11月09日 20:51:00[三少GG](https://me.csdn.net/scut1135)阅读数：1771标签：[2010																[properties																[dialog																[library																[installer																[download](https://so.csdn.net/so/search/s.do?q=download&t=blog)](https://so.csdn.net/so/search/s.do?q=installer&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
如果Debug下不能运行，则切换到Release版本下！！！(以前网上查到，不明真相）
# 实践中可用的方案1：
[http://blog.aguskurniawan.net/post/OpenCV-210-with-Visual-Studio-2010.aspx](http://blog.aguskurniawan.net/post/OpenCV-210-with-Visual-Studio-2010.aspx)
[**OpenCV 2.1.0 with Visual Studio 2010**](http://blog.aguskurniawan.net/post/OpenCV-210-with-Visual-Studio-2010.aspx)
By [Agus Kurniawan](http://blog.aguskurniawan.net/author/Admin.aspx)23. 六月 2010 00:24
OpenCV (Open Source Computer Vision) is a library of programming functions for real time computer vision. Two months ago OpenCV released the new version of OpenCV 2.1. On this article, I would like to share how to use OpenCV with Visual Studio 2010.
# System Requirements
- Download OpenCV 2.1 or the latest, [[V](http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.1/OpenCV-2.1.0-win32-vs2008.exe/download)]
- Download Visual Studio 2010, you can download an express edition [[V](http://www.microsoft.com/express/downloads/)]
I use Windows 7 x64 as development environment.
# OpenCV 2.1 Installation
After you downloaded OpenCV library. Click setup file and then you’ll get a dialog as below
![inst1](http://blog.aguskurniawan.net/image.axd?picture=inst1_thumb.png)
Click **Next** button
![inst2](http://blog.aguskurniawan.net/image.axd?picture=inst2_thumb.png)
Click **I Agree** button
![inst3](http://blog.aguskurniawan.net/image.axd?picture=inst3_thumb.png)
Choose **Do not add OpenCV to the system PATH** (you may choose any option) and then click **Next** button
![inst4](http://blog.aguskurniawan.net/image.axd?picture=inst4_thumb.png)
Choose **Destnation Folder** that you want to store OpenCV files. After that, click **Next** button
![inst5](http://blog.aguskurniawan.net/image.axd?picture=inst5_thumb.png)
Click **Next** button
![inst6](http://blog.aguskurniawan.net/image.axd?picture=inst6_thumb.png)
Select the type of install **Full**. After that click **Install** button. Installer will install OpenCV library on your system.
![inst7](http://blog.aguskurniawan.net/image.axd?picture=inst7_thumb.png)
Click **Finish** button to finish installation process.
Now you can see OpenCV 2.1 on your system. You can check on folder where OpenCV installed
![openCV](http://blog.aguskurniawan.net/image.axd?picture=openCV_thumb.png)
# Visual Studio 2010 Project Configuration
After you created a C/C++ project on Visual Studio 2010 you should configure OpenCV on Visual Studio 2010. Open properties dialog of project. Assume your OpenCV 2.1 installed on C:/OpenCV2.1
On properties dialog, click **C/C++** –>**General **and entry **C:/OpenCV2.1/include/opencv** in **Additional Include Directories**
![conf1](http://blog.aguskurniawan.net/image.axd?picture=conf1_thumb.png)
On properties dialog, click **C/C++** –>**Advanced **and select **Compile As**. If you prefer C compiler, use **Compile as C Code (/TC)**
![conf2](http://blog.aguskurniawan.net/image.axd?picture=conf2_thumb.png)
On properties dialog, click **Linker** –>**General **and entry **C:/OpenCV2.1/lib** in **Additional Library Directories**
![conf3](http://blog.aguskurniawan.net/image.axd?picture=conf3_thumb.png)
On properties dialog, click **Linker** –>**Input **and entry all *.lib files in **Additional Dependencies**. If you’re running on debug mode, try to entry all *d.lib files
![conf4](http://blog.aguskurniawan.net/image.axd?picture=conf4_thumb.png)
![conf5](http://blog.aguskurniawan.net/image.axd?picture=conf5_thumb.png)
After you do all tasks above, click OK button on Propeties dialog.
# Testing
For testing purpose, try to write this code (*note* change image file **c:/temp/monas.jpg**)
#include <stdio.h>
#include <highgui.h>
int main( int argc, char** argv ) 
{	
	IplImage* img = cvLoadImage( "c:/temp/monas.jpg",1 );
	cvNamedWindow("Monas", CV_WINDOW_AUTOSIZE );
	cvShowImage("Monas", img );
	cvWaitKey(0);
	cvReleaseImage( &img );
	cvDestroyWindow("Monas");
} 
Compile and run it. If success, you get image viewer dialog as below
![monas](http://blog.aguskurniawan.net/image.axd?picture=monas_thumb.png)
when you run this code from Visual Studio  and get error as below. Try to copy *.dll from OpenCV (**C:/OpenCV2.1/bin**) on folder where your executable file was located
![error](http://blog.aguskurniawan.net/image.axd?picture=error_thumb.png)
Donate this article: ![](https://www.paypal.com/en_US/i/scr/pixel.gif)
当前评分 4.8 , 共有 4 人参与
