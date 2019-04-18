# Opencv 滑桿(createTrackbar) - YZXnuaa的博客 - CSDN博客
2018年01月23日 15:28:59[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：190
个人分类：[OpenCV](https://blog.csdn.net/YZXnuaa/article/category/7390843)
OpenCV提供createTrackbar()函式，可以在視窗上產生滑桿，讓使用者自己調整輸入，接著用這輸入值執行預計的操作，另外有getTrackbarPos()和setTrackbarPos()函式，讓我們對滑桿進行進一步的操作。
#### OpenCV 產生滑桿
int createTrackbar(const string& trackbarname, const string& winname, int* value, int count, TrackbarCallback onChange=0, void*userdata=0)
- trackbarname：滑桿名稱。
- winname：滑桿的父視窗名稱。
- value：滑桿所在位置的值。
- count：滑桿允許的最大值，最小值為0。
- onChange：自定義函式的名稱，當滑桿值變動時，會呼叫此函式。
當我們創建滑桿時，會搭配一個自定義函式，當使用者改變滑桿的值時，OpenCV會自動呼叫此函式。我們自行決定這個函式名稱和內容，把這名稱作為createTrackbar()裡的onChange參數，當然，通常這函式內部會使用到滑桿輸入值。
#### OpenCV 得到滑桿位置
int getTrackbarPos(const string& trackbarname, const string& winname)
- trackbarname：滑桿名稱。
- winname：滑桿的父視窗名稱。
#### OpenCV 設定滑桿位置
void setTrackbarPos(const string& trackbarname, const string& winname, int pos)
- trackbarname：滑桿名稱。
- winname：滑桿的父視窗名稱。
- pos：滑桿位置。
以下我們程式碼創建滑桿，sliderValue為滑桿的值，初始為0，使用者可透過拉動滑桿更改sliderValue的值，sliderMaxValue為滑桿的最大值，我們這邊設為100，當使用者拉動滑桿時，程式呼叫on_trackbar()函式，此時讀取sliderValue的值當作影像的混和比例：
```
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;
int sliderValue;
Mat src1, src2;
void on_trackbar(int, void*){
    double alpha = (double) sliderValue/100.0 ;
    double beta = ( 1.0 - alpha );
    Mat dst;
    addWeighted( src1, alpha, src2, beta, 0.0, dst);
    imshow("trackbar demo", dst);
}
int main(){
    src1 = imread("beach.jpg",CV_LOAD_IMAGE_UNCHANGED);
    src2 = imread("cat.jpg",CV_LOAD_IMAGE_UNCHANGED);
    sliderValue = 0;
    int sliderMaxValue = 100;
    namedWindow("trackbar demo", 0);
    createTrackbar("Ratio", "trackbar demo", &sliderValue, sliderMaxValue, on_trackbar);
    on_trackbar(sliderValue, 0 );
    waitKey(0);
    return 0;
}
```
![trackbar](http://monkeycoding.com/wp-content/uploads/2016/01/trackbar.jpg)
转自：http://monkeycoding.com/?p=652
            
