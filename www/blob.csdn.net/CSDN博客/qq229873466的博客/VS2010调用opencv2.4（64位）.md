# VS2010调用opencv2.4（64位） - qq229873466的博客 - CSDN博客

2015年11月30日 11:37:48[qq229873466](https://me.csdn.net/qq229873466)阅读数：377标签：[visual studio 2010																[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)](https://so.csdn.net/so/search/s.do?q=visual studio 2010&t=blog)
个人分类：[VC应用](https://blog.csdn.net/qq229873466/article/category/5990495)


**1.建项目**

文件->新建->项目->Win32(Visual C++)->Win32控制台应用程序->输入项目名字，选择保存路径，填出框选下一步->点击控制台应用程序（默认），选择空项目->完成

在源文件处添加*.cpp文件，并输入如下测试代码：

```cpp
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;
int main()
{
    Mat img = imread("test.jpg");//图片和*.cpp文件位于同一目录
    if(img.empty())
    {
        cout<<"error";
        return -1;
    }
    imshow("test",img);
    waitKey();

    return 0;
}
```

**2.配置**

右键我的电脑（计算机）->属性->高级系统设置->环境变量->系统变量->path后加入opencv2.4中的x64\vc10\bin目录 e.g. (E:\opencv\opencv2.4\build\x64\vc10\bin) ->确定

打开cmd->输入set PATH=C:->回车->关闭cmd->打开cmd->echo %PATH%->回车（令环境变量马上生效）

右键项目名->VC++目录

->包含目录->输入opencv2.4中的include目录 e.g. (E:\opencv\opencv2.4\build\include)

->库目录->输入opencv2.4中的x64\vc10\lib目录 e.g. (E:\opencv\opencv2.4\build\x64\vc10\lib)

->链接器->输入->附加依赖项->输入所有lib中的名字 e.g. Debug(*d.lib) Release(非*b.lib)一般只配置Debug就可以了

->配置管理器->活动方案解决平台->新建->选择x64->关闭

之后就可以编译运行程序了。

附上Debug(*d.lib)

opencv_videostab2410d.lib

opencv_calib3d2410d.lib

opencv_contrib2410d.lib

opencv_core2410d.lib

opencv_features2d2410d.lib

opencv_flann2410d.lib

opencv_gpu2410d.lib

opencv_highgui2410d.lib

opencv_imgproc2410d.lib

opencv_legacy2410d.lib

opencv_ml2410d.lib

opencv_nonfree2410d.lib

opencv_objdetect2410d.lib

opencv_ocl2410d.lib

opencv_photo2410d.lib

opencv_stitching2410d.lib

opencv_superres2410d.lib

opencv_ts2410d.lib

opencv_video2410d.lib


