
# 图像拼接（二）：OpenCV同时打开两个摄像头捕获视频 - 高科的专栏 - CSDN博客

2018年11月12日 13:35:21[高科](https://me.csdn.net/pbymw8iwm)阅读数：209


使用OpenCV实现同时打开两个USB摄像头，并实时显示视频。如果未检测有两个摄像头，程序会结束并发出“摄像头未安装好”的警告。这里推荐一个小巧的摄像头视频捕捉软件：amcap，使用它可以方便的检查每个摄像头是否能正常工作。
捕获视频：
```python
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
using namespace cv;
using namespace std;
int main()
{
    VideoCapture cap1(2);
    VideoCapture cap2(1);
    bool stop(false);
    Mat frame1;
    Mat frame2;
    namedWindow("cam1", CV_WINDOW_AUTOSIZE);
    namedWindow("cam2", CV_WINDOW_AUTOSIZE);
    if (cap1.isOpened() && cap2.isOpened())
    {
        cout << "*** ***" << endl;
        cout << "摄像头已启动！" << endl;
    }
    else
    {
        cout << "*** ***" << endl;
        cout << "警告：请检查摄像头是否安装好!" << endl;
        cout << "程序结束！" << endl << "*** ***" << endl;
        return -1;
    }
    //cap1.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    //cap1.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    //cap2.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    //cap2.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cap1.set(CV_CAP_PROP_FOCUS, 0);
    cap2.set(CV_CAP_PROP_FOCUS, 0);
    while (!stop)
    {
        if (cap1.read(frame1) && cap2.read(frame2))
        {
            imshow("cam1", frame1);
            imshow("cam2", frame2);
            imwrite("frame1.bmp", frame1);
            imwrite("frame2.bmp", frame2);
            //彩色帧转灰度
            cvtColor(frame1, frame1, CV_RGB2GRAY);
            cvtColor(frame2, frame2, CV_RGB2GRAY);

            if (waitKey(1) == 27)//按ESC键
            {
                stop = true;
                cout << "程序结束！" << endl;
                cout << "*** ***" << endl;
            }
        }
    }
    return 0;
}
```
![](https://img-blog.csdn.net/20170118201446580?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3psMzg5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码说明：
1、VideoCapture cap1(2);这句定义了一个视频捕获对象，参数为摄像头的ID，0是第1个摄像头，请确认每个摄像头的ID。
2、下面两句设置了捕获视频每帧图像的分辨率和焦距，调用的是bool VideoCapture::set(int propId, double value)方法。此外，该方法还可设置视频捕获的帧率、亮度、对比度、饱和度、白平衡等各种信息。可参考OpenCV在线文档。
cap1.set(CV_CAP_PROP_FRAME_WIDTH, 640);
cap1.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
cap1.set(CV_CAP_PROP_FOCUS, 0);
cap2.set(CV_CAP_PROP_FOCUS, 0);
3、if (cap1.read(frame1) && cap2.read(frame2)){}中read()方法的参数代表捕获的帧图像，如果捕获成功，则返回True。

