# VC 2008 Express下安装OpenCV2.3.1 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月06日 21:59:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35











注意：
- 下列文档以VC2008 Express为例，VC2010下的配置应与本文档类似。
- VC 6.0不被OpenCV 2.3.1支持。
- VC Express是微软提供的免费版，可从此处下载： [http://www.microsoft.com/visualstudio/en-us/products/2010-editions/express](http://www.microsoft.com/visualstudio/en-us/products/2010-editions/express)
- 建议先不要自己编译，如果使用预编译好的库有问题，再尝试自己编译。





## 安装所需要的软件





### 下载OpenCV
- 从本站下载栏目 [http://www.opencv.org.cn/index.php/Download](http://www.opencv.org.cn/index.php/Download) 下载 OpenCV for Windows（也即 OpenCV-2.3.1-win-superpack.exe 文件）。
- 将 OpenCV-2.3.1-win-superpack.exe 解压并放到某个目录下，例如 `D:\Program Files\OpenCV2.3.1` （无需运行setup.exe，解压则可）。解压后的目录结构如下图。


![点击看大图](http://wiki.opencv.org.cn/images/thumb/8/8c/Opencv231-install-1.png/300px-Opencv231-install-1.png)










### 安装CMake(不打算自己编译无需安装)

从 [http://www.cmake.org/cmake/resources/software.html](http://www.cmake.org/cmake/resources/software.html) 下载 Windows (Win32 Installer) 安装。







## 编译OpenCV（非必需步骤）





### 用CMake导出VC++项目文件
- 运行cmake-gui，设置where is the source code路径为OpenCV安装路径（本文档假定安装位置为：`D:\Program Files\OpenCV2.3.1\opencv`），并创建子目录`D:\Program Files\OpenCV2.3.1\opencv\build\my`，并将cmake的"where to build the binaries"设置为这个目录。
- 然后点 configure，在弹出的对话框内选择 Visual Studio 9 2008。
- 你可根据你的系统修改选项，修改后再次选择“Congfigure”，完成后选择“Generate”。
（注： 此处Open CV文件放的目录不能包含汉字，否则“Congfigure”和“Generate”无法成功；同时需要关闭已经打开的VS，否则CMake与VS的宏无法配置。 by：Jackyzzy 赵振阳）
|![2.0版本截图仅供参考，点击看大图](http://wiki.opencv.org.cn/images/thumb/9/90/Opencv2-install-step-1.png/300px-Opencv2-install-step-1.png)|![2.0版本截图仅供参考，点击看大图](http://wiki.opencv.org.cn/images/thumb/d/dc/Opencv2-install-step-3.png/300px-Opencv2-install-step-3.png)||
|----|----|----|











### 

### 编译 OpenCV Debug和Release版本库

完成上一步骤后，将在D:\Program Files\OpenCV2.3.1\opencv\build\my目录下生成OpenCV.sln的VC Solution File，请用VC++ 2008 Express打开OpenCV.sln，然后执行如下操作：
- 在Debug下，选择Solution Explorer(解决方案资源管理器)里的 Solution OpenCV（解决方案“OpenCV”），点右键，运行"Rebuild Solution"；如编译无错误，再选择INSTALL项目，运行"Build"。
- 在Release下，选择Solution Explorer里的 Solution OpenCV，点右键，运行"Rebuild Solution"；如编译无错误，再选择INSTALL项目，运行"Build"。

全部运行完毕后，针对你的系统的OpenCV库就生成了。





## 配置VC





### 配置include路径

也即告诉VC去什么地方寻找OpenCV的头文件，打开VC，选择菜单“工具”->“选项”->“项目和解决方案”->“VC++目录”->“包含文件”，包含 ` D:\Program Files\OpenCV2.3.1\opencv\build\include；D:\Program Files\OpenCV2.3.1\opencv\build\include\opencv；D:\Program Files\OpenCV2.3.1\opencv\build\include\opencv2` 如果是自己编译的则输入D:\Program Files\OpenCV2.3.1\opencv\build\my\install\include


![点击看大图](http://wiki.opencv.org.cn/images/thumb/3/3c/Opencv231-install-2.png/300px-Opencv231-install-2.png)










### 配置lib路径

也即告诉VC去什么地方寻找OpenCV的库文件。

在刚才下载的文件OpenCV-2.3.1-win-superpack.exe 里，已经为VC2008和VC2010预先编译好了动态库和静态库。因此我们不需要如早先版本那样，自己用cmake编译OpenCV。
- 自己编译的库，库目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\my\install\lib `


（注：由于自己编译成功，lib文件在Debug和Release两个文件中，加载库时需到Debug和Release中，
  我配置的库目录为：D:\Program Files\OpenCV2.3.1\opencv\build\my\install\lib\Debug，否则调试时无法进入Open CV代码中。 by：Jackyzzy 赵振阳）


- 32位系统 & VC2008，库目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x86\vc9\lib`
- 32位系统 & VC2010，库目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x86\vc10\lib`
- 64位系统 & VC2008，库目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x64\vc9\lib`
- 64位系统 & VC2010，库目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x64\vc10\lib`

请根据自己的情况四选一，将库目录输入菜单“工具”->“选项”->“项目和解决方案”->“VC++目录”->“库文件” 。如下图所示：
|![点击看大图](http://wiki.opencv.org.cn/images/thumb/8/89/Opencv231-install-3.png/300px-Opencv231-install-3.png)|![点击看大图](http://wiki.opencv.org.cn/images/thumb/1/14/Opencv231-install-5.png/300px-Opencv231-install-5.png)||
|----|----|----|





## 设置环境变量

刚才设置的是动态库，因此还需要将OpenCV的dll文件所在的目录加入Path环境变量。dll文件目录如下，请根据自己情况五选一：
- 自己编译的库，dll目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\my\install\bin`
- 32位系统 & VC2008，dll目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x86\vc9\bin`


（注：此处环境变量应与上边库目录一致，这样能够调试时能够进入代码，我自己的目录是：D:\Program Files\OpenCV2.3.1\opencv\build\x86\vc9\bin\Debug。 by：Jackyzzy 赵振阳）


- 32位系统 & VC2010，dll目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x86\vc10\bin`
- 64位系统 & VC2008，dll目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x64\vc9\bin`
- 64位系统 & VC2010，dll目录为：`D:\Program Files\OpenCV2.3.1\opencv\build\x64\vc10\bin`

由于有些函数需要TBB，所以需要将tbb所在的目录也加入到环境变量Path中。TBB相关的DLL路径为：
- 32位系统 & VC2008：`D:\Program Files\OpenCV2.3.1\opencv\build\common\tbb\ia32\vc9`
- 32位系统 & VC2010：`D:\Program Files\OpenCV2.3.1\opencv\build\common\tbb\ia32\vc10`
- 64位系统 & VC2008：`D:\Program Files\OpenCV2.3.1\opencv\build\common\tbb\intel64\vc9`
- 64位系统 & VC2010：`D:\Program Files\OpenCV2.3.1\opencv\build\common\tbb\intel64\vc10`

如下图所示将OpenCV和TBB的dll文件所在的目录系统环境变量Path中。加入后可能需要注销当前Windows用户（或重启）后重新登陆才生效。
|![点击看大图](http://wiki.opencv.org.cn/images/thumb/5/5d/Opencv2-install-step-15.png/300px-Opencv2-install-step-15.png)|![点击看大图](http://wiki.opencv.org.cn/images/thumb/0/0d/Opencv2-install-step-16.png/300px-Opencv2-install-step-16.png)|
|----|----|





## 使用OpenCV 2.3.1编程
- 打开VC++ 2008 Express，创建一个Win32控制台程序helloopencv；


![点击看大图](http://wiki.opencv.org.cn/images/thumb/d/df/Opencv231-install-6.png/300px-Opencv231-install-6.png)





- 选择Solution Explorer里的opencvhello项目，点击鼠标右键，选择Properties。


![点击看大图](http://wiki.opencv.org.cn/images/thumb/c/c4/Opencv231-install-7.png/300px-Opencv231-install-7.png)





- ，在[链接器 LINKER]的[输入INPUT]中,为项目的Debug配置增加 [附加依赖项 Additional Dependencies]：`opencv_calib3d231d.lib; opencv_contrib231d.lib; opencv_core231d.lib; opencv_features2d231d.lib; opencv_flann231d.lib; opencv_gpu231d.lib; opencv_highgui231d.lib; opencv_imgproc231d.lib; opencv_legacy231d.lib; opencv_ml231d.lib; opencv_objdetect231d.lib; opencv_ts231d.lib; opencv_video231d.lib` （可根据实际需要删减）注意，请打开了新编辑窗口（即点击了“...”按钮）“附加依赖项”，并一条一条分别加入，一条一行（一个回车），（注意行末不要加";"）否则会出现类似以下错误：1>LINK: fatal error LNK1104: 无法打开文件“…….lib”
- 如果下载的OpenCV不是2.3.1版本，请将"XXX231d.lib"中的"231"更改为对应的版本号，例如：下载OpenCV 2.4.0，则统一改为"XXX240d.lib"，否则会出现类似以下错误：1>LINK: fatal error LNK1104: 无法打开文件“XXX.lib”


![点击看大图](http://wiki.opencv.org.cn/images/thumb/e/e7/Opencv231-install-8.png/300px-Opencv231-install-8.png)





- 为项目的Release配置增加[附加依赖项 Additional Dependencies]：`opencv_calib3d231.lib; opencv_contrib231.lib; opencv_core231.lib; opencv_features2d231.lib; opencv_flann231.lib; opencv_gpu231.lib; opencv_highgui231.lib; opencv_imgproc231.lib; opencv_legacy231.lib; opencv_ml231.lib; opencv_objdetect231.lib; opencv_ts231.lib; opencv_video231.lib` （可根据实际需要删减）*


![点击看大图](http://wiki.opencv.org.cn/images/thumb/e/eb/Opencv231-install-9.png/300px-Opencv231-install-9.png)





- 编译运行下面的例程（需要将[lena.jpg文件](http://wiki.opencv.org.cn/images/f/fb/Lena.jpg)放在项目目录下,即与生成的.exe文件同位置）（这里有误。双击exe的话，图片放在.exe同一文件夹内；按F5直接运行程序的话，lena.jpg要放在helloopencv\helloopencv里）。



```cpp
/***********************************************************************
 * OpenCV 2.3.1 测试例程
 * 于仕琪 提供
 ***********************************************************************/
#include "stdafx.h"
 
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;
 
int main(int argc, char* argv[])
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




![程序运行结果图，点击看大图](http://wiki.opencv.org.cn/images/thumb/a/a2/Opencv231-install-10.png/500px-Opencv231-install-10.png)

 程序运行结果图





原文地址：http://wiki.opencv.org.cn/index.php/首页




