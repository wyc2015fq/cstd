# 人脸识别实战之用QT做点事（二）：做一个人脸检测界面 - sinat_31425585的博客 - CSDN博客
2018年06月19日 08:21:48[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：1403
    这个其实很简单，可以参考博客：https://blog.csdn.net/shirley3052004/article/details/70162211，界面由两个label和两个pushBotton构成，具体流程如下：
    1、新建一个项目：
    文件 -> 新建文件或项目 -> Qt Widgets Application -> 输入名称："face_detect"  -> 直接点击下一步到最后。
    2、打开界面文件对应mainwindow.ui，并拖四个控件
    双击mainwindow.ui -> 拖两个label和两个Push Button，并调整label大小 -> 修改Push Botton 的objectName 和 text（左边这个Push Botton按钮对应于打开摄像头功能，右边这个Push Botton 按钮对应于关闭摄像头功能，所有将左边的Push Botton的objectName 设置为openCam， text设置为openCamera， 右边Push Button的objectName 设置为closeCam， text设置为closeCamera）
![](https://img-blog.csdn.net/20180619080410503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    3、添加相应的信号处理函数
    分别右键单击左右两个Push Botton 按钮，转到槽 -> 然后修改mainwindow.h文件，修改部分如下：
    （1）添加头文件：
```cpp
#include <QImage>
#include <QTimer>     // 设置采集数据的间隔时间
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;
```
    （2）在private slots中添加成员函数：
`void readFrame();       // 读取当前帧信息`
    （3）在private中添加成员：
```cpp
QTimer    *timer;
QImage    *image;
VideoCapture cap;
Mat frame;
CascadeClassifier face_cascade;
```
     4、修改mainwindow.cpp文件
    （1） 修改mainwindow函数，添加如下部分：
```cpp
if( !face_cascade.load("F:\\QT\\data\\haarcascade_frontalface_alt2.xml") )
        cout << "--(!)Error loading face cascade\n";   
    timer = new QTimer(this);
    image = new QImage();
    // 时间到，读取当前摄像头信息
    connect(timer, SIGNAL(timeout()),
            this, SLOT(readFrame()));
```
        注意：cascade文件载入路径一定要跟自己cascade文件所在实际路径一致。
    （2） 在on_openCam_clicked()函数中添加：
```cpp
cap.open(0);
timer->start(33);
```
    （3） 在on_closeCam_clicked()函数中添加：
```cpp
timer->stop();
cap.release();
```
    （4） 添加readFrame() 函数的处理部分：
```cpp
void MainWindow::readFrame()
{
    cap >> frame;
    Mat gray, frame1, frame2;
    vector<Rect> faces;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    cvtColor(frame, frame1, COLOR_BGR2RGB);
    QImage img((const uchar*)frame1.data,
               frame1.cols, frame1.rows,
               frame1.cols * frame1.channels(),
               QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    equalizeHist(gray, gray);
    //-- Detect faces
    face_cascade.detectMultiScale( gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(60, 60) );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
    cvtColor(frame, frame2, COLOR_BGR2RGB);
    QImage img1((const uchar*)frame2.data,
               frame2.cols, frame2.rows,
               frame2.cols * frame2.channels(),
               QImage::Format_RGB888);
    ui->label_2->setPixmap(QPixmap::fromImage(img1));
}
```
    这样就ok啦，运行一下，效果如下：
![](https://img-blog.csdn.net/20180619081937390?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
