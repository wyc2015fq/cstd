# QT之QImage与Mat（opencv） - sinat_31425585的博客 - CSDN博客
2018年08月29日 00:00:10[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：452
     在QT中，与机器视觉最相关的类可能就是QImage了，QImage是QT中处理图像数据最主要的类，他提供了很多处理图像的函数。而在opencv中，数据存储格式为Mat，Mat取义于matrix，是一个用于存储处理不同数据类型的n维矩阵。举例来说，在机器视觉中，一张图片就是一个由像素组成的二维矩阵，它具有宽（列数）、高（行数）和通道数，每一个位置对应于一个像素强度（灰度图）或多个像素强度（彩色图）。
    直观理解，如下：
![](https://img-blog.csdn.net/20180829000959222?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180829001016830?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
     将图像中一个图像块放大，就可以得到第二个图像。
一、Mat的构造函数
 1、 要创建一个r行，c列，数据格式为F的矩阵M，可以采用：
```cpp
Mat M(r, c, F, Scalar(0));
```
 这里，Scalar(0)表示将矩阵M初始化为0，而F的格式可以为：
```cpp
CV_<bits><type>C(<channels>)
```
  bits表示位数，可以为：
```
8: 无符号或有符号整数
16: 无符号或有符号整数
32: 无符号或有符号整数和浮点数
64: 无符号或有符号的浮点数
```
 type表示数据类型，可以为：
```cpp
U: 无符号整形
S: 有符号整形
F: 有符号的浮点型
```
 channels表示通道数，如果为图像，一般为1或者3
 常见的图像格式有：
```cpp
CV_8U : 8-bit unsigned integers
CV_8S : 8-bit signed integers
CV_16U : 16-bit unsigned integers
CV_16S : 16-bit signed integers
CV_32S : 32-bit signed integers
CV_32F : 32-bit floating-point numbers
CV_64F : 64-bit floating-point numbers
```
2、还可以采用create接口来进行构造：
```cpp
Mat M;
M.create(r, c, F, Scalar(0));
```
3、使用Mat::zeros和Mat::ones来构造：
```cpp
Mat M = Mat::zeros(r, c, F);
// Mat M = Mat::ones(r, c, F);
```
4、使用size来构造：
```cpp
Mat M(Size(r, c), F);
```
二、Mat的ROI选取
1、使用Rect选取：
```cpp
Mat roi(image,Rect(25,25,50,50));
```
2、使用rowRange和colRange：
```cpp
Mat M1 = image.rowRange(image.rows/2 - 10, image.rows/2 + 10);
Mat M2 = image.colRange(image.cols/2 - 10, image.cols/2 + 10);
```
三、Mat的复制
   clone和copyTo
```cpp
Mat imageCopy = image.clone();
src.copyTo(dst, mask);
```
四、QImage的构造函数
 1、给定图像大小和格式，创建一个空的QImage对象：
```cpp
QImage image(320, 240, QImage::Format_RGB888);
```
2、还可以通过QSize代替直接赋值：
```cpp
QImage image(QSize(320,240), QImage::Format_RGB888);
```
五、opencv中Mat类型与QImage类型之间转换
因为opencv中默认图像通道顺序为BGR，而Qt中默认图像颜色通道为RGB，因此转换过程中，需要对通道顺序进行调整：
1、彩色图像对应Mat 转 QImage：
```cpp
Mat mat =imread("c:/dev/test.jpg"); 
cvtColor(mat, mat, CV_BGR2RGB); 
QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
```
    其中cvtColor是opencv提供用于转换Mat类型数据颜色通道的函数，还可以用来将BGR图像转换成HSV，Gray等格式。
  2、 还可以使用QImage的rgbSwapped()属性，直接对RGB三通道进行交换。
```cpp
cv::Mat mat = cv::imread("c:/dev/test.jpg");
QImage image(mat.data,
            mat.cols,
            mat.rows,
            mat.step,
            QImage::Format_RGB888);
ui->label->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
```
参考资料：
Computer Vision with OpenCV 3 and Qt5
