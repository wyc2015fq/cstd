# 配置OpenCV的Qt开发环境 - wishchinYang的专栏 - CSDN博客
2013年10月18日 20:56:26[wishchin](https://me.csdn.net/wishchin)阅读数：1160
QT&openCV系列！链接：http://www.cnblogs.com/emouse/category/449213.html
本文链接：[http://blog.csdn.net/qiurisuixiang/article/details/8665278](http://blog.csdn.net/qiurisuixiang/article/details/8665278)
0 编译、配置OpenCV的工程相当曲折，编译OpenCV不下5次，一开始还以为是自己的配置、编译步骤有问题，后来发现原来是自己电脑装的Qt Creator版本有点低，里面的MingW版本较低，而OpenCV下载的是最新版本，最新版本的OpenCV需要高版本的MingW。最后果断换成高版本Qt Creator，终于编译成功。
1 下载所需工具
（1）Qt Creator 下载地址[http://qt-project.org](http://qt-project.org/) (建议下载最新版本，因为里面带的MinGW版本较高，我最开始使用Qt
 4.7.4编译OpenCV-2.4.4一直报错，后来换成Qt 5.0.1编译成功)
（2）OpenCV 下载地址[http://opencv.org/](http://opencv.org/)（不建议下载最新版本，最新版本的编译需要高版本的MinGW，不然在执行mingw32-make命令会报错）
（3）CMake 下载地址[http://www.cmake.org/cmake/resources/software.html](http://www.cmake.org/cmake/resources/software.html)
2 安装Qt Creator，过程很简单，一直下一步就OK
3 安装OpenCV，设置解压路径就OK
4 安装CMake，过程同样很简单（注意：（1）安装路径不能有中文和空格（2）勾选加入所有境变量中）
5 目前最新的OpenCV版本，安装之后的目录下面有源码和个版本的lib和dll，不过为避免出现问题，还是自己编译
6 将C:\Qt\Qt5.0.1\Tools\MinGW\bin加入系统的Path环境变量（假设Qt Creator安装在C盘）
7 打开CMake，使用CMake编译OpenCV。配置源码路径与生成的目标路径
（1）源码路径：要选择OpenCV安装目录下含有如下文件的文件夹
（2）目标路径：可以随意设置
![](https://img-my.csdn.net/uploads/201303/12/1363092103_5268.jpg)
8 点击Configure配置按钮。在弹出的对话框中选择MinGW makefiles，其余选项保持默认，点击Finish按钮，开始编译
![](https://img-my.csdn.net/uploads/201303/12/1363085166_5181.jpg)
9 CMake编译结束界面如下图，勾选生成列表中的WITH_OPENGL和WITH_QT两项，其他项最好不要改动
![](https://img-my.csdn.net/uploads/201303/12/1363085213_3710.jpg)
10 再次点击Configure按钮，红色消失
![](https://img-my.csdn.net/uploads/201303/12/1363085244_5168.jpg)
11 点击Generate按钮，生成makefile文件，成功后，底下会提示Generating done
![](https://img-my.csdn.net/uploads/201303/12/1363085274_9706.jpg)
12 打开cmd，进入刚才设置的目标路径（C：\OpenCV）,输入mingw32-make命令开始编译
![](https://img-my.csdn.net/uploads/201303/12/1363085299_3423.jpg)
13 此过程较为漫长，建议关掉杀毒软件可能会快些，在我的ThinkPad T420上耗时30分钟
![](https://img-my.csdn.net/uploads/201303/12/1363085394_3918.jpg)
14 执行mingw32-make install,此过程大约需要1到2分钟
![](https://img-my.csdn.net/uploads/201303/12/1363085340_2407.jpg)
15 下面开始配置Qt Creator的OpenCV开发环境，拷贝下面三个文件夹放在同一个文件夹下（只需这三个目录，其余生成的文件和文件夹可以删除，以节省磁盘空间）
（1）OpenCV-2.4.4安装目录下的build/include目录
![](https://img-my.csdn.net/uploads/201303/12/1363089113_8912.jpg)
（2）CMake目标路径下，MinGW编译后的bin目录
![](https://img-my.csdn.net/uploads/201303/12/1363089137_7457.jpg)
（3）CMake目标路径下，MinGW编译后的lib目录
![](https://img-my.csdn.net/uploads/201303/12/1363090368_6672.jpg)
16 拷贝三个目录到同一目录后的结构如下图（我删除了原来的CMake目标目录中的其他目录，拷贝进来了include目录）
![](https://img-my.csdn.net/uploads/201303/12/1363090448_9230.jpg)
17 将bin目录添加到系统环境变量
![](https://img-my.csdn.net/uploads/201303/12/1363090469_9720.jpg)
18 至此Qt Creator下的OpenCV开发环境已经配置好，下面在Qt Creator下测试是否已经配置成功
19 打开Qt Creator，新建一个空工程，然后添加一个cpp文件，任意命名
20 在pro文件中添加如下OpenCV的依赖文件
```cpp
INCLUDEPATH+=C:\OpenCV\include\opencv\
                    C:\OpenCV\include\opencv2\
                    C:\OpenCV\include
LIBS+=C:\OpenCV\lib\libopencv_calib3d244.dll.a\
        C:\OpenCV\lib\libopencv_contrib244.dll.a\
        C:\OpenCV\lib\libopencv_core244.dll.a\
        C:\OpenCV\lib\libopencv_features2d244.dll.a\
        C:\OpenCV\lib\libopencv_flann244.dll.a\
        C:\OpenCV\lib\libopencv_gpu244.dll.a\
        C:\OpenCV\lib\libopencv_highgui244.dll.a\
        C:\OpenCV\lib\libopencv_imgproc244.dll.a\
        C:\OpenCV\lib\libopencv_legacy244.dll.a\
        C:\OpenCV\lib\libopencv_ml244.dll.a\
        C:\OpenCV\lib\libopencv_objdetect244.dll.a\
        C:\OpenCV\lib\libopencv_video244.dll.a
备注：其实有种简单的做法，但是有个缺点是可能包含链接库较多
INCLUDEPATH+=C:\OpenCV\include\opencv\
                    C:\OpenCV\include\opencv2\
                    C:\OpenCV\include
LIBS+=C:\OpenCV\lib
```
21 main.cpp文件中内容如下
```cpp
#include"cv.h"
#include"cxcore.h"
#include"highgui.h"
int main(intargc,char*argv[])
{
    //声明IplImage指针
   IplImage*pImg;
    //载入图片
    pImg=cvLoadImage("E:/OpenCV/FirstOpenCV/home.jpg",1);
    //创建窗口
    cvNamedWindow("Image",1);
    //显示图像
    cvShowImage("Image",pImg);
    //等待按键
    cvWaitKey(0);
    //销毁窗口
    cvDestroyWindow("Image");
    //释放图像
    cvReleaseImage(&pImg);
    return0;
}
```
22 运行程序，按键盘上任意键可以退出
![](https://img-my.csdn.net/uploads/201303/12/1363091124_3969.jpg)
