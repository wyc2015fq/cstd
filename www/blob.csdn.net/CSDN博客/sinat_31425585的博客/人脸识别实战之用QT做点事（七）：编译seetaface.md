# 人脸识别实战之用QT做点事（七）：编译seetaface - sinat_31425585的博客 - CSDN博客
2018年08月15日 14:54:33[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：442
     之前使用opencv自带的人脸检测器来进行人脸检测，感觉效果不太好，出现很多误检，于是想能不能换个人脸检测接口，现有最好的人脸检测接口是于仕琪老师开源的libfacedetection接口，但是没有开放源码，也没有提供qt编译库，无法进行直接编译，于是考虑对山世光老师的开源人脸检测引擎进行编译。  
在csdn上搜寻了一下看到一个编译好的seetaface，QT版本，下载地址为：
[https://download.csdn.net/download/qq_18854309/10004013](https://download.csdn.net/download/qq_18854309/10004013)
    下载下来之后一看，用不了，对这个下载文件，我只想说一个字**坑**！完全是来骗积分的，因此，我就尝试自己来尝试编译，结果还真成功了，现在将编译步骤总结一下，分享给大家。
    参考[净无邪](https://blog.csdn.net/naibozhuan3744)的博客：[Qt5.9生成dll详细图文教程](https://blog.csdn.net/naibozhuan3744/article/details/78926110)
    当然，不是完全一样，这篇博客只是参考，具体流程如下：
一、编译库
    文件 ->新建 -> library -> C++ 库：
![](https://img-blog.csdn.net/20180815141512553?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
名称：faceDetection
![](https://img-blog.csdn.net/20180815141735449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择MinGW这个kits
![](https://img-blog.csdn.net/20180815141943603?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
勾选QtGUI
![](https://img-blog.csdn.net/20180815142055298?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后，直接下一步->.......->完成。文件结构如下：
![](https://img-blog.csdn.net/20180815142311564?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后，删除facedetection.h，facedetection_global.h， facedetection.cpp三个文件，将seetafaceEngine文件夹中，对应于include和src两个文件夹复制到当前新建文件夹路径中：
![](https://img-blog.csdn.net/20180815143010165?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
修改common.h文件，这里原谅博主的自私，向大家借一点积分，这里提供一个下载链接：[修改后的common.h](https://download.csdn.net/download/sinat_31425585/10605482)
然后直接生成即可，在debug路径下，会生成对应facedetection.a和facedetection.dll库文件。
二、配置
将facedetection.a和facedetection.dll分别放到配置opencv所在路径的lib和bin文件下
![](https://img-blog.csdn.net/201808151439412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018081514404843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在opencv的include文件夹下新建子文件夹：facedetection
然后将编译库过程中，所有的头文件(.h)复制到这个路径下，你全部复制过来也行，开心就好：
![](https://img-blog.csdn.net/20180815144309762?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
三、测试
新建一个qt console工程：
在.pro文件中添加：
```cpp
INCLUDEPATH += D:\library\opencv\MinGW\install\include\faceDetection
LIBS += -L D:\library\opencv\MinGW\install\x86\mingw\lib\libfacedetection.a
```
测试代码如下：
```cpp
#include <QCoreApplication>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "face_detection.h"
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    const char* img_path = "F:\\QT\\facedetectTest\\faceDetect\\0_1_1.jpg";
      seeta::FaceDetection detector("F:\\QT\\facedetectTest\\faceDetect\\seeta_fd_frontal_v1.0.bin");
      detector.SetMinFaceSize(40);
      detector.SetScoreThresh(2.f);
      detector.SetImagePyramidScaleFactor(0.8f);
      detector.SetWindowStep(4, 4);
      cv::Mat img = cv::imread(img_path, cv::IMREAD_UNCHANGED);
      cv::Mat img_gray;
      if (img.channels() != 1)
        cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
      else
        img_gray = img;
      seeta::ImageData img_data;
      img_data.data = img_gray.data;
      img_data.width = img_gray.cols;
      img_data.height = img_gray.rows;
      img_data.num_channels = 1;
      long t0 = cv::getTickCount();
      std::vector<seeta::FaceInfo> faces = detector.Detect(img_data);
      long t1 = cv::getTickCount();
      double secs = (t1 - t0)/cv::getTickFrequency();
      cout << "Detections takes " << secs << " seconds " << endl;
    #ifdef USE_OPENMP
      cout << "OpenMP is used." << endl;
    #else
      cout << "OpenMP is not used. " << endl;
    #endif
    #ifdef USE_SSE
      cout << "SSE is used." << endl;
    #else
      cout << "SSE is not used." << endl;
    #endif
      cout << "Image size (wxh): " << img_data.width << "x"
          << img_data.height << endl;
      cv::Rect face_rect;
      int32_t num_face = static_cast<int32_t>(faces.size());
      for (int32_t i = 0; i < num_face; i++) {
        face_rect.x = faces[i].bbox.x;
        face_rect.y = faces[i].bbox.y;
        face_rect.width = faces[i].bbox.width;
        face_rect.height = faces[i].bbox.height;
        cv::rectangle(img, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
      }
      cv::namedWindow("Test", cv::WINDOW_AUTOSIZE);
      cv::imshow("Test", img);
      cv::waitKey(0);
      cv::destroyAllWindows();
    return a.exec();
}
```
需要修改测试图像路径及模型所在路径，效果如下：
![](https://img-blog.csdn.net/20180815145140382?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180815145021104?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
将效果图截图到一起的原因是，alt一按下，效果图就不见了，然后点到自带的截图工具不工作了。
facealignment的编译方式与这个一样，就不再赘述了，enjoy！！！！
最后展示一下，我做的一个人脸识别系统界面：
![](https://img-blog.csdn.net/20180815154607687?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
效果：[https://v.youku.com/v_show/id_XMzc4MTk0NTE3Ng==.html?spm=a2h3j.8428770.3416059.1](https://v.youku.com/v_show/id_XMzc4MTk0NTE3Ng==.html?spm=a2h3j.8428770.3416059.1)
