# OpenCV3.x中UMat对象介绍与使用 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年04月09日 11:03:49[gloomyfish](https://me.csdn.net/jia20003)阅读数：10444








### UMat对象起源

OpenCV3中引入了一个新的图像容器对象UMat，它跟Mat有着多数相似的功能和相同的API函数，但是代表的意义却太不一样。要说到UMat对象的来龙去脉，必须首先从OpenCL来开始说，OpenCL是一个面向异构系统通用的并行编程标准，这个标准最早是苹果公司提出，后来变成了一个国际标准，目的是通过它开发通用的GPU计算软件，中国的华为是该标准的成员之一。说的直白点就是如果CPU或者GPU支持OpenCL标准，就可以通过OpenCL相关编程实现使用GPU计算。OpenCV2.x开始支持它，不过那个时候这个功能很不好用，大致一般正常基于CPU的读写视频一帧图像代码如下：

```bash
cv::Mat inMat, outMat;
vidInput >> inMat;
cv::cvtColor(inMat, outMat, cv::COLOR_RGB2GRAY);
vidOutput << outMat;
```

基于OpenCL的GPU方式读写视频一帧图像代码如下：

```
cv::Mat inMat, outMat;
vidInput >> inMat;
cv::ocl::oclMat inOclMat(inMat);
cv::ocl::oclMat outOclMat;
cv::ocl::cvtColor(inOclMat, outOclMat, cv::COLOR_RGB2GRAY);
outMat = outOclMat;
vidOutput << outMat;
```

上述代码通过添加ocl前缀空间实现OpenCL支持设备的GPU运算能力提高。但是上述代码在不支持OpenCL的平台上还会运行失败，使用起来及其不方便。对开发者来说不是统一API和底层透明。

于是OpenCV在3.0版本中开始引入了T-API的设计理念，即通过设计一套对开发者来说底层透明，接口统一的API调用方式，避免由于系统不支持OpenCL而导致程序运行失败，这个就是UMat图像容器类型。通过使用UMat对象，OpenCV会自动在支持OpenCL的设备上使用GPU运算，在不支持OpenCL的设备仍然使用CPU运算，这样就避免了程序运行失败，而且统一了接口。上述代码在OpenCV3中使用UMat改下如下：

```bash
cv::UMat inMat, outMat;
vidInput >> inMat;
cv::cvtColor(inMat, outMat, cv::COLOR_RGB2GRAY);
vidOutput << outMat;
```

这样就无需像OpenCV2中那样通过显式声明的调用方式。很明显UMat与Mat极其类似。而且两者之间是可以相互转换的。

### Mat与UMat相互转换

#### 从UMat中获取Mat对象使用UMat的get方法UMat::getMat(int access_flags)支持的FLAG如下：
- ACCESS_READ 
- ACCESS_WRITE 
- ACCESS_RW 
- ACCESS_MASK 
- ACCESS_FAST 

最常用的就是读写，注意当使用这种方式的时候UMat对象将会被LOCK直到CPU使用获取Mat对象完成操作，销毁临时Mat对象之后，UMat才可以再被使用。

#### 把Mat转换为UMat

通过Mat::getUMat()之后就获取一个UMat对象，同样在UMat对象操作期间，作为父对象Mat也会被LOCK直到子对象UMat销毁之后才可以继续使用。

OpenCV的官方文档说不鼓励在一个方法和一段代码中同时使用Mat与UMat两种方式，因为这样做真的非常危险。此外Mat与UMat还可以相互拷贝，但是这种方式也不是OpenCV官方提倡与推荐的，所以尽量别用这种方式。

一个同UMat读取视频并灰度化完整的例子

```cpp
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    VideoCapture capture;
    capture.open("D:/vcprojects/images/sample.mp4");
    if (!capture.isOpened()) {
        printf("could not load video data...\n");
        return -1;
    }

    // UMat方式读取视频，转为灰度显示-自动启用GPU计算
    // 如果显卡支持OpenCL
    UMat frame, gray;
    namedWindow("UMat Demo", CV_WINDOW_AUTOSIZE);
    while (capture.read(frame)) {
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        imshow("UMat Demo", gray);
        char c = waitKey(100);
        if (c == 27) {
            break;
        }
    }
    // 释放资源
    capture.release();
    waitKey(0);
    return 0;
}
```

### 特别注意

代码基于VS2015与OpenCV3.1实现，欢迎大家继续关注本人博客！分享有用实用的图像处理技术与OpenCV相关技术文章，本人会用不停止！！！



