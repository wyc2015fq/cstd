# 人脸识别实战之用QT做点事（一）：使用QT5.7.1编译opencv3.4.1+contrib - sinat_31425585的博客 - CSDN博客
2018年06月19日 02:26:04[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：835
   参考博客：https://blog.csdn.net/huihut/article/details/78701814
    其实都差不多，一个套路。首先需要安装QT5.7.1MinGM版本，并将QT路径加入到系统环境变量中去，然后安装3.8.2版本的Cmake，其次下载好Opencv3.4.1+contrib源码，最后打开Cmake的gui界面，并设置source路径和编译二进制文件保存路径。
![](https://img-blog.csdn.net/20180619020522162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
   然后取消BUILD_opencv_python3、BUILD_opencv_python_bingings_generator、WITH_MATLAB、BUILD_opencv_hdf、ENABLE_PRECOMPILED_HEADERS、WITH_IPP，勾选ENABLE_CXX11、WITH_QT、WITH_OPENGL
    点击Configure，设置编译工具，点击Generate按钮。
    在MinGM文件下，按住shift，右键进入cmd窗口，输入命令：
>> mingw32-make
>> mingw32-make install
  编译完成后就会在MinGM下的install文件夹中生成编译好的库文件。
![](https://img-blog.csdn.net/20180619021940268?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
将x86文件夹下面bin路径加到系统path变量中，例如我的bin文件路径为：
D:\Libarary\opencv3.4.1\MinGM\install\x86\mingw\bin
最后，测试一下：
新建一个控制台程序，将opencv的include头文件路径及库文件路径LIBS加入到.pro文件中：
```
INCLUDEPATH += D:\Libarary\opencv3.4.1\MinGW\install\include
INCLUDEPATH += D:\Libarary\opencv3.4.1\MinGW\install\include\opencv
INCLUDEPATH += D:\Libarary\opencv3.4.1\MinGW\install\include\opencv2
 
LIBS += -L D:\Libarary\opencv3.4.1\MinGW\install\x86\mingw\lib\libopencv_*.a
```
然后，在main.cpp文件中添加：
```
#include <opencv2/opencv.hpp>
using namespace cv;
```
以及：
```
Mat src = imread("F:\\QT\\1.jpg");
    imshow("src", src);
    waitKey(0);
```
放一起如下：
![](https://img-blog.csdn.net/20180619022452744?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
运行结果如下：
![](https://img-blog.csdn.net/20180619022547514?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
