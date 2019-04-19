# 人脸识别实战之用QT做点事（四）：增加人脸对齐 - sinat_31425585的博客 - CSDN博客
2018年06月19日 14:25:59[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：110
    人脸关键点定位之后，就可以通过眼睛的位置获知人脸的姿态，在大量的论文中都强调过，将人脸对齐之后，能够显著的提升识别的准确率，因此，在前面的工作基础上，这里做一些许的调整，就可以实现对齐功能。
    首先，我们来看一下，68点模型的人脸关键点分布，如图1所示。
![](https://img-blog.csdn.net/20181007144404341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                     图1 68个关键点人脸模型
    可以看到人眼对应关键点分别对应于：
    左眼：37-42                            右眼：43-48
    而68点存储在一个vector中，vector索引是从0开始标记的，所以眼睛对应关键点索引为：
     左眼：36-41                            右眼：42-47
    那么人脸对齐就很简单，这里只贴一下关键代码：
    首先，在mainwindow.h文件中的private slots:中添加成员变量：
```cpp
void preprocess(Mat& img, const vector<Point2f> &p, const Rect & r,
                    Point& e_L, Point& e_R, Point& e_C, Mat & face);
```
    然后，添加preprocess函数的实现部分：
```cpp
void MainWindow::preprocess(Mat& img, const vector<Point2f> & p, const Rect & r,
                Point& e_L, Point& e_R, Point& e_C, Mat & face)
{
    e_L = Point(0, 0);
    e_R = Point(0, 0);
    e_C = Point(0, 0);
    // 获取左右眼的中心坐标
    // 36-41左眼
    for (int m = 36; m < 42; m++)
    {
        e_L.x += (int)p[m].x;
        e_L.y += (int)p[m].y;
        circle(img, p[m], 1, Scalar(0, 255, 0));
    }
    e_L.x /=  6;
    e_L.y /=  6;
    // 42-47右眼
    for (int n = 42; n < 48; n++)
    {
        e_R.x += (int)p[n].x;
        e_R.y += (int)p[n].y;
        circle(img, p[n], 1, Scalar(0, 255, 0));
    }
    e_R.x /=  6;
    e_R.y /=  6;
    circle(img, e_L, 1, Scalar(0, 0, 255), 3);
    circle(img, e_R, 1, Scalar(0, 0, 255), 3);
    e_L -= Point(r.x, r.y);
    e_R -= Point(r.x, r.y);
    // 人眼中心
    e_C.x = (e_L.x + e_R.x) * 0.5f;
    e_C.y = (e_L.y + e_R.y) * 0.5f;
    // 人脸对齐
    // 计算两个眼睛之间的角度
    double dy = (e_R.y - e_L.y);
    double dx = (e_R.x - e_L.x);
    double len = sqrt(dx*dx + dy*dy);
    double angle = atan2(dy, dx) * 180.0 / CV_PI;
    double desiredLen = (DESIRED_RIGHT_EYE_X - 0.16);
    double scale = desiredLen * DESIRED_FACE_WIDTH / len;
    Mat rot_mat = getRotationMatrix2D(e_C, angle, scale);
    // Shift the center of the eyes to be the desired center between the eyes.
    rot_mat.at<double>(0, 2) += DESIRED_FACE_WIDTH * 0.5f - e_C.x;
    rot_mat.at<double>(1, 2) += DESIRED_FACE_HEIGHT * DESIRED_LEFT_EYE_Y - e_C.y;
    Mat warped = Mat(DESIRED_FACE_HEIGHT, DESIRED_FACE_WIDTH, CV_8U, Scalar(128)); // Clear the output image to a default grey.
    Mat roi = img(r);
    warpAffine(roi, warped, rot_mat, warped.size());
    //equalizeHist(warped, warped);
    Mat mask = Mat(warped.size(), CV_8U, Scalar(0)); // Start with an empty mask.
    Point faceCenter = Point( DESIRED_FACE_WIDTH/2, cvRound(DESIRED_FACE_HEIGHT * FACE_ELLIPSE_CY) );
    Size size = Size( cvRound(DESIRED_FACE_WIDTH * FACE_ELLIPSE_W), cvRound(DESIRED_FACE_HEIGHT * FACE_ELLIPSE_H) );
    ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);
    //imshow("mask", mask);
    // Use the mask, to remove outside pixels.
    Mat dstImg = Mat(warped.size(), CV_8U, Scalar(128)); // Clear the output image to a default gray.
    // Apply the elliptical mask on the face.
    warped.copyTo(dstImg, mask);  // Copies non-masked pixels from filtered to dstImg.
    //imshow("dstImg", dstImg);
    face = warped;
}
```
最后，显示部分代码位于readFrame函数中：
```cpp
void MainWindow::readFrame()
{
    cap >> frame;
    Mat gray, frame1, frame2;
    vector<Rect> faces;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    //cvtColor(frame, frame1, COLOR_BGR2RGB);
    //QImage img((const uchar*)frame1.data,
               //frame1.cols, frame1.rows,
               //frame1.cols * frame1.channels(),
               //QImage::Format_RGB888);
    // ui->label->setPixmap(QPixmap::fromImage(img));
    equalizeHist(gray, gray);
    //-- Detect faces
    face_cascade.detectMultiScale( gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(60, 60) );
    vector< vector<Point2f> > landmarks;
    // Run landmark detector
    bool success = facemark->fit(frame,faces,landmarks);
    Mat face;
    if(success)
    {
      // If successful, render the landmarks on the face
      for(int i = 0; i < landmarks.size(); i++)
      {
          // cout << landmarks[i].size() << endl;
        // drawLandmarks(frame, landmarks[i]);
          preprocess(frame, landmarks[i], faces[i],
                         eye_L, eye_R, eye_C, face);
          int width = ui->label_2->width();
          int height = ui->label_2->height();
          cv::resize(face, face, cv::Size(width, height));
          cvtColor(face, frame2, COLOR_BGR2RGB);
          QImage img1((const uchar*)frame2.data,
                     frame2.cols, frame2.rows,
                     frame2.cols * frame2.channels(),
                     QImage::Format_RGB888);
          ui->label_2->setPixmap(QPixmap::fromImage(img1));
          rectangle(frame, faces[i], Scalar(0, 255, 0), 2);
      }
    }
    cvtColor(frame, frame1, COLOR_BGR2RGB);
    QImage img((const uchar*)frame1.data,
               frame1.cols, frame1.rows,
               frame1.cols * frame1.channels(),
               QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
}
```
效果如下：
![](https://img-blog.csdn.net/20180619142549122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
