# OpenCV 3.1.0编译与添加扩展模块 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年01月17日 15:21:30[gloomyfish](https://me.csdn.net/jia20003)阅读数：5059








# OpenCV 3.1.0编译与添加扩展模块

最近在弄个东西，需要把OpenCV的扩展模块中的xfeatures给包含进来，发现要自己编译OpenCV3.1.0与其扩展模块才可以实现。经过一番实践，终于编译完成，总结了一下，其实很简单，只要如下三步即可实现。在正式开始之前，有些准备工作需要做，就是下载OpenCV3.1.0还有其扩展模块，以及CMake GUI工具。

OpenCV3.1.0下载地址： 
[https://sourceforge.net/projects/opencvlibrary/files/opencv-win/3.1.0/opencv-3.1.0.exe/download](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/3.1.0/opencv-3.1.0.exe/download)

OpenCVcontrib下载地址： 
[https://github.com/opencv/opencv_contrib](https://github.com/opencv/opencv_contrib)

cmake-gui下载地址(3.7.2)：  [https://cmake.org/download/](https://cmake.org/download/)

把OpenCV就解压缩到指定目录，其中我是把OpenCV解压缩到 
![这里写图片描述](https://img-blog.csdn.net/20170117150059251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后安装CMake GUI，默认安装即可。 
![这里写图片描述](https://img-blog.csdn.net/20170117150144333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
好了之后就正式开始。 

第一步 

配置与生成CMake， 打开CMake GUI之后，选择好路径，点击配置，在打开的对话框中一定要选择VS14 Windows 64才可以。然后它就会自动运行得到如下结果： 
![这里写图片描述](https://img-blog.csdn.net/20170117150232486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后找到OPENCV_EXTRA_MODULE_PATH设置扩展模块的路径运行完成显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20170117150357893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170117150542760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第二步： 

配置VS生成installer，首先到CMake的Build输出目录 

D:\opencv3.1\opencv\newbuild下找到OpenCV.sln文件，双击打开之后，右键选择 

->重新生成解决方案，然后在找到CMakeTargets->INSTALL, 右键选择生成installer即可。这样就会在D:\opencv3.1\opencv\newbuild下面多出一个installer的文件夹，到如果能成功生成，编译就结束了。 
![这里写图片描述](https://img-blog.csdn.net/20170117151016286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第三步： 

重新配置OpenCV，新建一个项目打开，到【视图】-》【其它窗口】-》【属性管理器】然后选择64 debug下的User Cpp从属性中打开配置窗口

分别设置好 包含目录 
![这里写图片描述](https://img-blog.csdn.net/20170117151349286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
库目录 
![这里写图片描述](https://img-blog.csdn.net/20170117151438772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 还有附件依赖项中添加如下： 

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

ippicvmt.lib 

opencv_xfeatures2d310d.lib 

opencv_xobjdetect310d.lib
最后千万别忘记把bin目录改过来， 我的编译生成的OpenCV v14/bin的目录如下： 

 D:\opencv3.1\opencv\newbuild\install\x64\vc14\bin

添加到系统的环境变量中即可。把原来的去掉。这样就可以使用OpenCV扩展模块xfeatures2d了。测试代码与运行结果如下：

```cpp
#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main(int argc, char** argv) {
    Mat img_1 = imread("D:/vcprojects/images/test.png", IMREAD_GRAYSCALE);   
    if (img_1.empty()) {
        printf("could not load image...\n");
        return -1;
    }
    imshow("input image", img_1);

    int minHessian = 400;
    Ptr<SURF> detector = SURF::create(minHessian);
    vector<KeyPoint> keypoints;
    detector->detect(img_1, keypoints);

    Mat img_keypoints1;
    drawKeypoints(img_1, keypoints, img_keypoints1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    namedWindow("key points", CV_WINDOW_AUTOSIZE);
    imshow("key points", img_keypoints1);

    waitKey(0);
    return 0;
}
```

基于xfeature实现SURF特征检测的运行结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20170117151912603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
关注微信公众号【OpenCV学堂】获取图像处理相关知识。











