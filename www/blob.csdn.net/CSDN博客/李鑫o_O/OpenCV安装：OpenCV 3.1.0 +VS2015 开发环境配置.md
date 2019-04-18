# OpenCV安装：OpenCV 3.1.0 +VS2015 开发环境配置 - 李鑫o_O - CSDN博客

2016年03月24日 19:22:44[hustlx](https://me.csdn.net/HUSTLX)阅读数：10595
个人分类：[图像处理](https://blog.csdn.net/HUSTLX/article/category/6120392)



一、准备工作

a、vs2015环境

b、从[http://opencv.org/downloads.html](http://opencv.org/downloads.html)下载opencv3.1.0

二、开始安装

a、双击![](https://img-blog.csdn.net/20160324192413135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)得到如下配置安装路径：

![](https://img-blog.csdn.net/20160324192441901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

b、设置一个路径，点击extract开始安装：

![](https://img-blog.csdn.net/20160324192525167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

三、配置环境变量

上述步骤完成之后，需要配置电脑的环境变量：电脑-》属性-》高级系统设置：

![](https://img-blog.csdn.net/20160324192705345?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击高级系统设置之后如下：

![](https://img-blog.csdn.net/20160324192852455?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

单击环境变量：对系统变量 path 添加:    D:\opencv3.1.0\opencv\build\x64\vc14\bin

注意前面改成自己相应的路径

![](https://img-blog.csdn.net/20160324192944659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击确定之后，重启电脑，一定要重启电脑。

四、配置项目

a、重启电脑之后，新建项目，右击项目名称，选择属性：

![](https://img-blog.csdn.net/20160324193426521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在配置属性-》vc目录，修改包含目录：

D:\opencv3.1.0\opencv\build\include\opencv2

D:\opencv3.1.0\opencv\build\include\opencv

D:\opencv3.1.0\opencv\build\include\

![](https://img-blog.csdn.net/20160324193552353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击确定之后，修改库目录：

D:\opencv3.1.0\opencv\build\x64\vc14\lib

![](https://img-blog.csdn.net/20160324193745241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

确定之后，链接器-》输入-》附加依赖项：

opencv_world310.lib

![](https://img-blog.csdn.net/20160324193726058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

确定之后，用下面代码进行测试：

```cpp
#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;

int main()
{
    // 读入一张图片（游戏原画）  
    Mat img = imread("pic.jpg");
    // 创建一个名为 "游戏原画"窗口  
    cvNamedWindow("游戏原画");
    // 在窗口中显示游戏原画  
    imshow("游戏原画", img);
    // 等待6000 ms后窗口自动关闭  
    waitKey(6000);
}
```

运行出现如下画面：
![](https://img-blog.csdn.net/20160324194015039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

