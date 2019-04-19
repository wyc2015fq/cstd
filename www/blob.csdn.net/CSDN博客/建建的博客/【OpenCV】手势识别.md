# 【OpenCV】手势识别 - 建建的博客 - CSDN博客
2018年08月31日 14:20:25[纪建](https://me.csdn.net/u013898698)阅读数：1516
手势动态识别（基于opencv的简单实现）
http://blog.csdn.net/anqing715/article/details/17653709
 
 
基于OpenCV的实时手势识别算法研究_许艳旭
http://wenku.baidu.com/view/c7a2c97e998fcc22bdd10d11.html
 
 
基于OpenCV动态手势识别的实现
http://wenku.baidu.com/view/e0c708efad51f01dc281f1dd.html
/************************************************************************/
/* 
Description:    手势检测
                先滤波去噪
                -->转换到HSV空间
                -->根据皮肤在HSV空间的分布做出阈值判断，这里用到了inRange函数，
                然后进行一下形态学的操作，去除噪声干扰，是手的边界更加清晰平滑
                -->得到的2值图像后用findContours找出手的轮廓，去除伪轮廓后，再用convexHull函数得到凸包络
Author:            Yang Xian
History:        
*/
/************************************************************************/
#include <iostream>    // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
 
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
 
using namespace cv;
using namespace std;
 
int main(int argc, char *argv[])
{
    const std::string sourceReference = "test3.avi";
    int delay = 1;
 
    char c;
    int frameNum = -1;            // Frame counter
 
    VideoCapture captRefrnc(sourceReference);
 
    if ( !captRefrnc.isOpened())
    {
        //         cout  << "Could not open reference " << sourceReference << endl;
        return -1;
    }
 
    Size refS = Size( (int) captRefrnc.get(CV_CAP_PROP_FRAME_WIDTH),
        (int) captRefrnc.get(CV_CAP_PROP_FRAME_HEIGHT) );
 
    bool bHandFlag = false;
 
    const char* WIN_SRC = "Source";
    const char* WIN_RESULT = "Result";
 
    // Windows
    namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE );
    namedWindow(WIN_RESULT, CV_WINDOW_AUTOSIZE);
 
    Mat frame;    // 输入视频帧序列
    Mat frameHSV;    // hsv空间
    Mat mask(frame.rows, frame.cols, CV_8UC1);    // 2值掩膜
    Mat dst(frame);    // 输出图像
 
//     Mat frameSplit[4];
 
    vector< vector<Point> > contours;    // 轮廓
    vector< vector<Point> > filterContours;    // 筛选后的轮廓
    vector< Vec4i > hierarchy;    // 轮廓的结构信息
    vector< Point > hull;    // 凸包络的点集
 
    while(true) //Show the image captured in the window and repeat
    {
        captRefrnc >> frame;
 
        if( frame.empty() )
        {
            cout << " < < <  Game over!  > > > ";
            break;
        }
        imshow( WIN_SRC, frame);
 
        // Begin
 
        // 中值滤波，去除椒盐噪声
        medianBlur(frame, frame, 5);
//         GaussianBlur( frame, frameHSV, Size(9, 9), 2, 2 );
//         imshow("blur2", frameHSV);
//        pyrMeanShiftFiltering(frame, frameHSV, 10, 10);
//         imshow(WIN_BLUR, frameHSV);
        // 转换到HSV颜色空间，更容易处理
        cvtColor( frame, frameHSV, CV_BGR2HSV );
 
//         split(frameHSV, frameSplit);
//         imshow(WIN_H, frameSplit[0]);
//         imshow(WIN_S, frameSplit[1]);
//         imshow(WIN_V, frameSplit[2]);
 
        Mat dstTemp1(frame.rows, frame.cols, CV_8UC1);
        Mat dstTemp2(frame.rows, frame.cols, CV_8UC1);
        // 对HSV空间进行量化，得到2值图像，亮的部分为手的形状
        inRange(frameHSV, Scalar(0,30,30), Scalar(40,170,256), dstTemp1);
        inRange(frameHSV, Scalar(156,30,30), Scalar(180,170,256), dstTemp2);
        bitwise_or(dstTemp1, dstTemp2, mask);
//         inRange(frameHSV, Scalar(0,30,30), Scalar(180,170,256), dst);        
 
        // 形态学操作，去除噪声，并使手的边界更加清晰
        Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
        erode(mask, mask, element);
        morphologyEx(mask, mask, MORPH_OPEN, element);
        dilate(mask, mask, element);
        morphologyEx(mask, mask, MORPH_CLOSE, element);
 
        frame.copyTo(dst, mask);
 
        contours.clear();
        hierarchy.clear();
        filterContours.clear();
        // 得到手的轮廓
        findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        // 去除伪轮廓
        for (size_t i = 0; i < contours.size(); i++)
        {
//             approxPolyDP(Mat(contours[i]), Mat(approxContours[i]), arcLength(Mat(contours[i]), true)*0.02, true);
            if (fabs(contourArea(Mat(contours[i]))) > 30000)    //判断手进入区域的阈值
            {
                filterContours.push_back(contours[i]);
            }
        }
        // 画轮廓
        drawContours(dst, filterContours, -1, Scalar(0,0,255), 3/*, 8, hierarchy*/);
        // 得到轮廓的凸包络
        for (size_t j=0; j<filterContours.size(); j++)
        {
            convexHull(Mat(filterContours[j]), hull, true);
            int hullcount = (int)hull.size();
 
            for (int i=0; i<hullcount-1; i++)
            {
                line(dst, hull[i+1], hull[i], Scalar(255,0,0), 2, CV_AA);
            }
            line(dst, hull[hullcount-1], hull[0], Scalar(255,0,0), 2, CV_AA);
        }
        
        imshow(WIN_RESULT, dst);
        dst.release();
        // End
 
        c = cvWaitKey(delay);
        if (c == 27) break;
    }
}
