# OpenCV 2.4.9 +VS2013 开发环境配置 - zj360202的专栏 - CSDN博客





2014年08月19日 10:00:50[zj360202](https://me.csdn.net/zj360202)阅读数：2016












OpenCV的全称是：Open Source Computer Vision Library。OpenCV是一个基于（开源）发行的跨平台计算机视觉库，可以运行在Linux、Windows和Mac OS操作系统上。它轻量级而且高效——由一系列 C 函数和少量 C++ 类构成，同时提供了Python、Ruby、MATLAB等语言的接口，实现了图像处理和计算机视觉方面的很多通用算法。

现本教程，是在windows平台下使用visual studio2013和opencv2.4.9配置一个开发环境。






## 工具/原料

- 
opencv2.4.9地址：[https://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.4.9/opencv-2.4.9.exe/download](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.4.9/opencv-2.4.9.exe/download)

- 
vs2013请自行百度





## 方法/步骤

- 

安装opencv2.4.9，解压，请务必记住自己解压的路径。宜家以我自己的路径为例D:\Program Files



![](https://img-blog.csdn.net/20140819095533715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



- 

配置环境变量

1、系统变量 Path：添加D:\Program Files\opencv\build\x86\vc12\bin

2、用户变量：添加opencv变量，值D:\Program Files\opencv\build，添加PATH变量（有就不需要添加，但是值需要添加）值D:\Program Files\opencv\build\x86\vc12\bin

**说明：不管你系统是32位还是64位，路径目录均选择X86，因为编译都是使用32位编译；如果选用X64，则程序运行时候会出错。**



![](https://img-blog.csdn.net/20140819095549923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



- 

新建visual C项目

新建 visual C++项目，如下图所示，项目选项注意：如下图。



![](https://img-blog.csdn.net/20140819095559907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140819095425140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



- 

工程目录的配置（Debug）

找到属性管理器，如果找不到，请安装下图方法找到。双击Debug|Win32打开如下窗口，

设置如下：（下图红框项为设置项）

1、包含目录：（VC++目录）

```java
D:\Program Files\opencv\build\include
D:\Program Files\opencv\build\include\opencv
D:\Program Files\opencv\build\include\opencv2
```



2、库目录：（VC++目录）

```java
D:\Program Files\opencv\build\x86\vc12\lib
```




3、连接器->输入->附加依赖项：

```java
opencv_ml249d.lib
opencv_calib3d249d.lib
opencv_contrib249d.lib
opencv_core249d.lib
opencv_features2d249d.lib
opencv_flann249d.lib
opencv_gpu249d.lib
opencv_highgui249d.lib
opencv_imgproc249d.lib
opencv_legacy249d.lib
opencv_objdetect249d.lib
opencv_ts249d.lib
opencv_video249d.lib
opencv_nonfree249d.lib
opencv_ocl249d.lib
opencv_photo249d.lib
opencv_stitching249d.lib
opencv_superres249d.lib
opencv_videostab249d.lib
```



其实以上都是D:\Program Files\opencv\build\x86\vc12\lib下所有的lib文件，你会发现，有的后面带上d，有的没有d，这是因为Debug的就有d，Release则没有d。



![](https://img-blog.csdn.net/20140819095729358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140819095651375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140819095705046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140819095714703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



- 

工程目录的配置（Release）

其他与Debug一样，只是连接器->输入->附加依赖项不一样，设置如下：

```java
opencv_objdetect249.lib
opencv_ts249.lib
opencv_video249.lib
opencv_nonfree249.lib
opencv_ocl249.lib
opencv_photo249.lib
opencv_stitching249.lib
opencv_superres249.lib
opencv_videostab249.lib
opencv_calib3d249.lib
opencv_contrib249.lib
opencv_core249.lib
opencv_features2d249.lib
opencv_flann249.lib
opencv_gpu249.lib
opencv_highgui249.lib
opencv_imgproc249.lib
opencv_legacy249.lib
opencv_ml249.lib
```




- 

最终的测试

1、新建一个cpp文件，如下图：在cpp文件上输入以下代码：

```cpp
#include <cv.h>

#include <highgui.h>

using namespace std;

int main()

{

IplImage * test;

test = cvLoadImage("D:\\Sample_8.bmp");//图片路径

cvNamedWindow("test_demo", 1);

cvShowImage("test_demo", test);

cvWaitKey(0);

cvDestroyWindow("test_demo");

cvReleaseImage(&test);

return 0;

}
```





[](http://jingyan.baidu.com/album/75ab0bcbee4b47d6864db2fc.html?picindex=9)

[](http://jingyan.baidu.com/album/75ab0bcbee4b47d6864db2fc.html?picindex=10)




- 
END





## 注意事项

- 
如果程序退出出错，请检测连接器->输入->附加依赖项有没有出错





错误列表

错误：opencv_core249d.lib(array.obj) : error LNK2038: 检测到“RuntimeLibrary”的不匹配项:  值“MTd_StaticDebug”不匹配值“MDd_DynamicDebug”(first.obj 中)

解决方案：工程-属性-c/c++-代码生成-运行库-多线程调（/MTd) 

错误：无法启动此程序，丢失opencv_core249.dll

解决方案：将opencv_core249.dll,opencv_highgui249.dll复制到exe目录下面即可 









