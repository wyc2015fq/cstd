# VS2012 +OpenCv2.4.4配置 - wishchinYang的专栏 - CSDN博客
2013年08月07日 16:17:44[wishchin](https://me.csdn.net/wishchin)阅读数：3181
                
使用OpenCV少了数据读取、填充、存储的麻烦。
转载于opencv官网：对于2010和2.43的配置可以直接挪用到新配置环境
[http://www.opencv.org.cn/index.php/VC_2010_Express下安装OpenCV2.4.3](http://www.opencv.org.cn/index.php/VC_2010_Express%E4%B8%8B%E5%AE%89%E8%A3%85OpenCV2.4.3)
VC 2012 Express下安装OpenCV2.4.4
注意： 
下列文档基于VC2012 Express。 
VC 6.0不被OpenCV 2.4.4支持。 
VC Express是微软提供的免费版，可从此处下载： VC2010 Express 
建议先不要自己编译，如果使用预编译好的库有问题，再尝试自己编译。 目录 [隐藏]
1 安装所需要的软件 
1.1 下载OpenCV
2 配置VC 
2.1 配置include路径
2.2 配置lib路径
3 设置环境变量
4 使用OpenCV 2.4.4编程
5 作者
安装所需要的软件
下载OpenCV
 从本站下载栏目 http://www.opencv.org.cn/index.php/Download 下载 OpenCV for Windows（也即 OpenCV-2.4.4.exe 文件）。
 将 OpenCV-2.4.4.exe 解压并放到某个目录下，例如 D:\OpenCV 。
配置VC
配置include路径：也即告诉VC去什么地方寻找OpenCV的头文件。 
(1):打开VC++ 2012 Express，创建一个Win32控制台程序opencvtest； 
(2):选择Solution Explorer里的opencvtest项目，点击鼠标右键，选择Properties。 
依次选择“配置属性”->“VC++目录”->“包含目录”，包含 D:\OpenCV\build\include；D:\OpenCV\build\include\opencv；D:\OpenCV\build\include\opencv2  
(切记：包含目录应该分行写，一定不能加分隔号，否则仍然找不到！！！)
(3):配置lib路径
也即告诉VC去什么地方寻找OpenCV的库文件。 
在刚才下载的文件OpenCV-2.4.3.exe 里，已经为VC2010预先编译好了动态库和静态库。因此我们不需要如早先版本那样，自己用cmake编译OpenCV。
32位系统 & VC2010，库目录为：D:\OpenCV\build\x86\vc11\lib 
64位系统 & VC2010，库目录为：D:\OpenCV\build\x64\vc11\lib 
请根据自己的情况二选一，将库目录输入“属性”->“配置属性”->“VC++目录”->“库目录” 。 
(4):设置环境变量
刚才设置的是动态库，因此还需要将OpenCV的dll文件所在的目录加入Path环境变量。dll文件目录如下： 
32位系统 & VC2012，dll目录为：D:\OpenCV\build\x86\vc11\bin 
64位系统 & VC2012，dll目录为：D:\OpenCV\build\x64\vc11\bin 
由于有些函数需要TBB，所以之前的版本需要将tbb所在的目录也加入到环境变量Path中。TBB相关的DLL路径为： 
32位系统 & VC2012：D:\OpenCV\build\common\tbb\ia32\vc11 
64位系统 & VC2012：D:\OpenCV\build\common\tbb\intel64\vc11
因为2.4.4版本的opencv内嵌了TBB的功能，所以取消了TBB直接安装，这样就不需要再配置TBB环境变量了。 
如下图所示将OpenCV的dll文件所在的目录系统环境变量Path中。加入后可能需要注销当前Windows用户（或重启）后重新登陆才生效。 
(对于2.44版本好像不用重启就可以了) 
三：使用OpenCV 2.4.4编程
选择Solution Explorer里的opencvtest项目，点击鼠标右键，选择Properties。 
在配置属性-->[链接器 LINKER]的[输入INPUT]中,为项目的Debug配置增加 [附加依赖项 Additional Dependencies]： 
```cpp
opencv_calib3d244d.lib
opencv_contrib244d.lib
opencv_core244d.lib
opencv_features2d244d.lib
opencv_flann244d.lib
opencv_gpu244d.lib
opencv_highgui244d.lib
opencv_imgproc244d.lib
opencv_legacy244d.lib
opencv_ml244d.lib
opencv_objdetect244d.lib
opencv_ts244d.lib
opencv_video244d.lib
```
（可根据实际需要删减）注意，请打开了新编辑窗口（即点击了“...”按钮）“附加依赖项”，并一条一条分别加入，一条一行（一个回车），（注意行末不要加";"）
否则会出现类似以下错误：1>LINK : fatal error LNK1104: 无法打开文件“…….lib” 
如果下载的OpenCV不是2.4.4版本，请将"XXX244d.lib"中的"244"更改为对应的版本号，例如：下载OpenCV 2.4.0，则统一改为"XXX240d.lib"，否则会出现类似以下错误：1>LINK : fatal error LNK1104: 无法打开文件“XXX.lib”
点击看大图
为项目的Release配置增加[附加依赖项 Additional Dependencies]： 
```cpp
opencv_contrib244.lib
opencv_core244.lib
opencv_features2d244.lib
opencv_flann244.lib
opencv_gpu244.lib
opencv_highgui244.lib
opencv_imgproc244.lib
opencv_legacy244.lib
opencv_ml244.lib
opencv_objdetect244.lib
opencv_ts244.lib
opencv_video244.lib
```
 （可根据实际需要删减）* 
编译运行下面的例程，需要将lena.jpg文件放在项目目录下。如果想双击运行exe的话，图片放在.exe同一文件夹内；按F5直接运行程序的话，lena.jpg要放在opencvtest\opencvtest里）。
/***********************************************************************
 * OpenCV 2.4.4 测试例程
 * 于仕琪 提供
 ***********************************************************************/
```cpp
#include "stdafx.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
 
int _tmain(int argc, _TCHAR* argv[])
{
    const char* imagename = "lena.jpg";
    //从文件中读入图像
    Mat img = imread(imagename);
    //如果读入图像失败
    if(img.empty())
    {
        fprintf(stderr, "Can not load image %s\n", imagename);
        return -1;
    }
    //显示图像
    imshow("image", img); 
    //此函数等待按键，按键盘任意键就返回
    waitKey(); 
    return 0;
}
```
