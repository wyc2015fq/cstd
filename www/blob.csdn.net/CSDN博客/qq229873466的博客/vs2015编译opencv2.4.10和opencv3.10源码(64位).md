# vs2015编译opencv2.4.10和opencv3.10源码(64位) - qq229873466的博客 - CSDN博客

2016年10月25日 10:27:54[qq229873466](https://me.csdn.net/qq229873466)阅读数：3295


**vs2015编译opencv2.4.10：**

1.安装Visual Studio Community 2015、cmake3.70（官网有免安装版）、opencv2.4.10。

2.进入vs2015，工具 -> Visual Studio 命令提示，打开cmd窗口，输入cmake-gui.exe全路径打开（E:\cmake\cmake-3.7.0-rc2-win64-x64\bin\cmake-gui.exe）。

3.进入cmake，点击Browse Sources选择原代码目录（E:/opencv2.4.10/sources），点击Browse Build选择编译后代码存放的目录（E:/opencv2.4.10/mbuild），点击Configure，出现红色栏，再点一次Configure，红色栏全变成白色，点击Generate。

4.进入vs2015，打开解决方案E:\opencv2.4.10\mbuild\OpenCV.sln，选择解决方案配置为Debug，x64，右键ALL_BUILD生成，完成后右键INSTALL生成。到这里源码编译完成，可以开始新建自己的解决方案进行源码调试。

**源码调试：**

1.配置环境变量，在本用户环境变量那里新建Path，输入E:\opencv2.4.10\mbuild\bin\Debug（这里有dll和调试用的pdb，而官方的build没有pdb所以不能调试），重启电脑使环境变量生效。

2.新建解决方案opencv2410，右键属性，配置活动Debug，平台x64，右键VC++目录，包含目录E:\opencv2.4.10\mbuild\install\include;E:\opencv2.4.10\mbuild\install\include\opencv;E:\opencv2.4.10\mbuild\install\include\opencv2，库目录E:\opencv2.4.10\mbuild\lib\Debug，链接器 -> 输入，附加依赖项，输入

opencv_ml2410d.lib

opencv_calib3d2410d.lib

opencv_contrib2410d.lib

opencv_core2410d.lib

opencv_features2d2410d.lib

opencv_flann2410d.lib

opencv_gpu2410d.lib

opencv_highgui2410d.lib

opencv_imgproc2410d.lib

opencv_legacy2410d.lib

opencv_objdetect2410d.lib

opencv_ts2410d.lib

opencv_video2410d.lib

opencv_nonfree2410d.lib

opencv_ocl2410d.lib

opencv_photo2410d.lib

opencv_stitching2410d.lib

opencv_superres2410d.lib

opencv_videostab2410d.lib

3.源文件添加cpp，输入

```cpp
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

int main()
{	
    Mat img = imread("lena.jpg");//注：要在cpp目录下放一张lena的图片	
    imshow("img", img);
    waitKey(0);
    return 0;
}
```
4.设断点，点击调试，可以进入源代码了。


**vs2015编译opencv3.10：**

1.安装Visual Studio Community 2015、cmake3.70（官网有免安装版）、opencv3.10。

2.进入vs2015，工具 -> Visual Studio 命令提示，打开cmd窗口，输入cmake-gui.exe全路径打开（E:\cmake\cmake-3.7.0-rc2-win64-x64\bin\cmake-gui.exe）。

3.进入cmake，点击Browse Sources选择原代码目录（E:/opencv3.10/sources），点击Browse Build选择编译后代码存放的目录（E:/opencv3.10/mbuild），点击Configure，发现需要下载文件，参考[http://blog.csdn.net/qq229873466/article/details/52904710](http://blog.csdn.net/qq229873466/article/details/52904710)中的3。搞定后，再点一次Configure，红色栏全变成白色，点击Generate。

4.进入vs2015，打开解决方案E:\opencv3.10\mbuild\OpenCV.sln，选择解决方案配置为Debug，x64，右键ALL_BUILD生成，完成后右键INSTALL生成。到这里源码编译完成，可以开始新建自己的解决方案进行源码调试。

**源码调试：**

1.配置环境变量，在本用户环境变量那里新建Path，输入E:\opencv3.10\mbuild\bin\Debug（这里有dll和调试用的pdb，而官方的build没有pdb所以不能调试），重启电脑使环境变量生效。

2.新建解决方案opencv310，右键属性，配置活动Debug，平台x64，右键VC++目录，包含目录E:\opencv3.10\mbuild\install\include;E:\opencv3.10\mbuild\install\include\opencv;E:\opencv3.10\mbuild\install\include\opencv2，库目录E:\opencv3.10\mbuild\lib\Debug，链接器 -> 输入，附加依赖项，输入

opencv_calib3d310d.lib

opencv_core310d.lib

opencv_features2d310d.lib

opencv_flann310d.lib

opencv_highgui310d.lib

opencv_imgcodecs310d.lib

opencv_imgproc310d.lib

opencv_ml310d.lib

opencv_objdetect310d.lib

opencv_photo310d.lib

opencv_shape310d.lib

opencv_stitching310d.lib

opencv_superres310d.lib

opencv_ts310d.lib

opencv_video310d.lib

opencv_videoio310d.lib

opencv_videostab310d.lib

3.源文件添加cpp，输入

```cpp
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

int main()
{	
    Mat img = imread("lena.jpg");	
    imshow("img", img);
    waitKey(0);
    return 0;
}
```
4.设断点，点击调试，可以进入源代码了。





