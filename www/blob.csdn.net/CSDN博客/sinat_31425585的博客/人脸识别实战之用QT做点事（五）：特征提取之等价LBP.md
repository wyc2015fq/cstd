# 人脸识别实战之用QT做点事（五）：特征提取之等价LBP - sinat_31425585的博客 - CSDN博客
2018年06月22日 23:08:51[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：107
    在检测到人脸后，需要对人脸图像进行特征提取，最常见的特征提取方式为LBP，这里采用等价LBP，具体的流程如下：
    1、新建一个features.h文件，内容如下：
```cpp
#ifndef FEATURES_H
#define FEATURES_H
//UniformLBP
Mat UniformLBP(Mat img, double * lbp_hist, Mat &out);
//中心对称LBP
Mat CSLBP(Mat & src, double * lbp_hist, Mat &out);
#endif // FEATURES_H
```
    2、新建一个features.cpp文件，内容如下：
```cpp
//UniformLBP
// 当某个LBP所对应的循环二进制数从0到1或者从1到0最多有两次跳变时，
// 该LBP所对应的二进制就称为一个等价模式。在实际图像中，
// 计算出来的大部分值都在等价模式之中，可达百分之90%以上。
Mat UniformLBP(Mat img, double * lbp_hist, Mat &out)
{
    uchar UTable[256];
    memset(UTable, 0, 256 * sizeof(uchar));
    uchar temp = 1;
    // 总共有256种2进制模式
    // 这里进行分类
    // 对属于等级模式的二进制表示进行统计
    for (int i = 0; i < 256; i++)
    {
        if (getHopCount(i) <= 2)
        {
            UTable[i] = temp;
            ++temp;
        }
    }
    Mat result;
    result.create(img.rows - 2, img.cols - 2, img.type());
    result.setTo(0);
    // 计算二进制表示，并通过查表的方式，获取其对应的等价模式下的值
    for (int i = 1; i < img.rows - 1; i++)
    {
        for (int j = 1; j < img.cols - 1; j++)
        {
            uchar center = img.at<uchar>(i, j);
            uchar code = 0;
            code |= (img.at<uchar>(i - 1, j - 1) >= center) << 7;
            code |= (img.at<uchar>(i - 1, j) >= center) << 6;
            code |= (img.at<uchar>(i - 1, j + 1) >= center) << 5;
            code |= (img.at<uchar>(i, j + 1) >= center) << 4;
            code |= (img.at<uchar>(i + 1, j + 1) >= center) << 3;
            code |= (img.at<uchar>(i + 1, j) >= center) << 2;
            code |= (img.at<uchar>(i + 1, j - 1) >= center) << 1;
            code |= (img.at<uchar>(i, j - 1) >= center) << 0;
            result.at<uchar>(i - 1, j - 1) = UTable[code];
        }
    }
    out = result;
    // 进行直方图统计
    for (int i = 0; i < result.rows; i++)
        for (int j = 0; j < result.cols; j++)
        {
            lbp_hist[result.at<uchar>(i, j)] += 1;
        }
    // 查找最大值
    float max_hist_val = 0;
    for (int i = 0; i < 59; i++)
    {
        if (max_hist_val < lbp_hist[i])
        {
            max_hist_val = lbp_hist[i];
        }
    }
    for (int i = 0; i < 59; i++)
    {
        lbp_hist[i] /= max_hist_val;
    }
    Mat LBP_hist_pic_color;
    Mat LBP_hist_pic = Mat::zeros(59, (59 + 1) * 2, CV_8U);
    cvtColor(LBP_hist_pic, LBP_hist_pic_color, COLOR_GRAY2BGR);
    for (int i = 0; i < 59; i++)
    {
        rectangle(LBP_hist_pic_color,
            Point(i * 2, 59),
            Point((i + 1) * 2, (1 - lbp_hist[i]) * 59),
            Scalar(255, 0, 0));
    }
    /*for (int i = 0; i < 59; i++)
    {
    rectangle(LBP_hist_pic_color,
    Point(i* LBP_hist_pic.cols / 118, LBP_hist_pic.rows),
    Point((i + 1)* LBP_hist_pic.cols /118, (1 - lbp_hist[i]) * LBP_hist_pic.rows),
    Scalar(255, 0, 0), 2);
    }*/
    return LBP_hist_pic_color;
}
// (ni - ni+4)* pow(2, i)
Mat CSLBP(Mat & src, double * lbp_hist, Mat &out)
{
    Mat img;
    src.copyTo(img);
    uchar UTable[16];
    // 计算二进制表示，并通过查表的方式，获取其对应的等价模式下的值
    for (int i = 1; i < img.rows - 1; i++)
    {
        for (int j = 1; j < img.cols - 1; j++)
        {
            uchar code = 0;
            for (int m = 0; m < 4; m++)
            {
                code |= (src.at<uchar>(i - 1, j - 1) >= src.at<uchar>(i + 1, j + 1)) << 0;
                code |= (src.at<uchar>(i, j - 1) >= src.at<uchar>(i, j + 1)) << 1;
                code |= (src.at<uchar>(i + 1, j - 1) >= src.at<uchar>(i - 1, j + 1)) << 2;
                code |= (src.at<uchar>(i + 1, j) >= src.at<uchar>(i - 1, j)) << 3;
            }
            img.at<uchar>(i, j) = code;
        }
    }
     out = img;
    for (int i = 1; i < img.rows - 1; i++)
    {
        for (int j = 1; j < img.cols - 1; j++)
        {
            UTable[img.at<uchar>(i, j)]++;
        }
    }
    // 查找最大值
    float max_hist_val = 0;
    for (int i = 0; i < 16; i++)
    {
        if (max_hist_val < UTable[i])
        {
            max_hist_val = UTable[i];
        }
    }
    //cout << max_hist_val << endl;
    for (int i = 0; i < 16; i++)
    {
        lbp_hist[i] = 1.0 * UTable[i] / max_hist_val;
        //cout << "lbp_hist: " << lbp_hist[i] << endl;
    }
    Mat LBP_hist = Mat::zeros(32, 17 * 2, CV_8U);
    Mat LBP_hist_color;
    cvtColor(LBP_hist, LBP_hist_color, COLOR_GRAY2BGR);
    for (int i = 0; i < 16; i++)
    {
        //cout << 1 - UTable[i] << endl;
        rectangle(LBP_hist_color,
            Point(i * 2, 32),
            Point((i + 1) * 2, int((1 - lbp_hist[i]) * 32)),
            Scalar(255, 0, 0));
    }
    return LBP_hist_color;
}
```
    3、在mainwidow.cpp文件中添加如下文件：
```cpp
double lbp_hist[59] = {0};
```
    在检测人脸之后，添加如下代码：
```cpp
equalizeHist(face_gray, face_gray);
          ULBP = UniformLBP(face_gray, lbp_hist, ULBP_out);
          int width_3 = ui->label_3->width();
          int height_3 = ui->label_3->height();
          cv::resize(ULBP, ULBP, cv::Size(width_3, height_3));
          cvtColor(ULBP, ULBP_show, COLOR_BGR2RGB);
          QImage img_ULBP((const uchar*)ULBP_show.data,
                     ULBP_show.cols, ULBP_show.rows,
                     ULBP_show.cols * ULBP_show.channels(),
                     QImage::Format_RGB888);
          ui->label_3->setPixmap(QPixmap::fromImage(img_ULBP));
```
效果如下：
![](https://img-blog.csdn.net/2018062223084067?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
