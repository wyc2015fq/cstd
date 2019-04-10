# OpenCV学习笔记（一）——安装配置、第一个程序

2011年10月21日 16:16:40

yang_xian521

阅读数：56396

 								标签： 																[image																](https://so.csdn.net/so/search/s.do?q=image&t=blog)[build																](https://so.csdn.net/so/search/s.do?q=build&t=blog)[include																](https://so.csdn.net/so/search/s.do?q=include&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[path																](https://so.csdn.net/so/search/s.do?q=path&t=blog)[cmd																](https://so.csdn.net/so/search/s.do?q=cmd&t=blog) 							更多

 								个人分类： 																[OpenCV																](https://blog.csdn.net/yang_xian521/article/category/910716) 							

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

决心开始研究OpenCV。闲言少叙，sourceforge网站最近的版本是2011年8月的OpenCV2.3.1，下载安装，我这里使用的开发环境是vs2008，网上搜了一下配置的教程，与之前的几个OpenCV版本的配置过程大体相同：（教程网上很多，知之为知之，不知百度之，我这里粗略再讲一下）

\1. 配置电脑的环境变量（Path变量）这里我的是D:\Program Files\OpenCV2.3.1\build\x86\vc9\bin（需要注销才能生效），这里需要注意网上的教程又让增加一个OPENCV变量，值为D:\Program Files\OpenCV2.3.1\build(你安装的路径中的build目录)

\2. 添加包含文件和库文件，这个和前几个版本方法类似，都是去工具->选项->VC目录添加build目录下的include目录及其子目录和你电脑对应版本的lib目录（选择x86 or x64，vc9  or vc10）

下面是第一个实例：

新建一个空项目，添加源文件如下：



```cpp
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
 
using namespace cv;
using namespace std; 
 
int main( int argc, char** argv )
{ 
	if( argc != 2) 
	{
	 cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
	 return -1;
	}
	
	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);	// Read the file
 
	if(! image.data )                              // Check for invalid input
	{
		cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}
 
	namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "Display window", image );                   // Show our image inside it.
 
	waitKey(0);											 // Wait for a keystroke in the window
	return 0;
}
```

这段程序可以在你安装目录下的samples\cpp\tutorial_code\introduction\display_image找到，编译后，将图片test.jpg放到opencvtest.exe相同的目录中去，利用cmd命令行进入的可执行文件所在的目录，



运行opencvtest test.jpg



则会显示一个图片，第一个程序成功。如图

![img](http://hi.csdn.net/attachment/201110/21/0_1319185093OlO0.gif)

如果遇到找不到tbb_debug.dll文件的问题，参考这里<http://www.opencv.org.cn/forum/viewtopic.php?p=52223>，只是我的环境是vs2008，大同小异。祝你成功！（PS：后来我发现这不是最好的处理办法，最好的处理办法是在Path路径里添加环境变量D:\Program  Files\OpenCV2.3.1\build\common\tbb\ia32\vc9）

接下来我觉得应该好好研究一下OpenCV里的doc文件夹下的教程和使用手册，我个人觉得《学习OpenCV》这本书已经远远跟不上OpenCV发展的速度了