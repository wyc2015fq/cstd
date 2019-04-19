# 人脸识别实战之用QT做点事（三）：增加人脸关键点检测 - sinat_31425585的博客 - CSDN博客
2018年06月19日 09:16:28[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：363
很简单，只需要在前面代码里面加入几行代码就好了。
1、mainwindow.h头文件中添加#include <opencv2/face.hpp>
2、在private slots:中添加两个函数：
```cpp
void drawPolyline
    (
      Mat &im,
      const vector<Point2f> &landmarks,
      const int start,
      const int end,
      bool isClosed = false
    );
    void drawLandmarks(Mat &im, vector<Point2f> &landmarks);
```
3、添加对应处理函数：
```cpp
// drawPolyLine draws a poly line by joining
// successive points between the start and end indices.
void MainWindow::drawPolyline
(
  Mat &im,
  const vector<Point2f> &landmarks,
  const int start,
  const int end,
  bool isClosed
)
{
    // Gather all points between the start and end indices
    vector <Point> points;
    for (int i = start; i <= end; i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }
    // Draw polylines.
    polylines(im, points, isClosed, COLOR, 2, 16);
}
void MainWindow::drawLandmarks(Mat &im, vector<Point2f> &landmarks)
{
    // Draw face for the 68-point model.
    if (landmarks.size() == 68)
    {
      drawPolyline(im, landmarks, 0, 16);           // Jaw line
      drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
      drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
      drawPolyline(im, landmarks, 27, 30);          // Nose bridge
      drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
      drawPolyline(im, landmarks, 36, 41, true);    // Left eye
      drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
      drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
      drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
    }
    else
    { // If the number of points is not 68, we do not know which
      // points correspond to which facial features. So, we draw
      // one dot per landamrk.
      for(int i = 0; i < landmarks.size(); i++)
      {
        circle(im,landmarks[i],3, COLOR, FILLED);
      }
    }
}
```
4、在private：中添加如下接口：
```cpp
Ptr<face::Facemark> facemark;
```
5、在mainwindow.cpp文件的MainWindow(QWidget *parent)接口中添加：
```cpp
// Create an instance of Facemark
    facemark = face::FacemarkLBF::create();
    // Load landmark detector
    facemark->loadModel("F:\\QT\\data\\lbfmodel.yaml");
```
6、在readFrame接口中添加：
```cpp
vector< vector<Point2f> > landmarks;
    // Run landmark detector
    bool success = facemark->fit(frame,faces,landmarks);
    if(success)
    {
      // If successful, render the landmarks on the face
      for(int i = 0; i < landmarks.size(); i++)
      {
        drawLandmarks(frame, landmarks[i]);
      }
    }
```
效果如下：
![](https://img-blog.csdn.net/20180619091611608?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
