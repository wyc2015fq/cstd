
# 交互系统的构建之（四）手掌与拳头检测加盟TLD - zouxy09的专栏 - CSDN博客


2012年08月31日 16:48:36[zouxy09](https://me.csdn.net/zouxy09)阅读数：12410标签：[image																](https://so.csdn.net/so/search/s.do?q=image&t=blog)[palm																](https://so.csdn.net/so/search/s.do?q=palm&t=blog)[tts																](https://so.csdn.net/so/search/s.do?q=tts&t=blog)[object																](https://so.csdn.net/so/search/s.do?q=object&t=blog)[function																](https://so.csdn.net/so/search/s.do?q=function&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=object&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=tts&t=blog)个人分类：[OpenCV																](https://blog.csdn.net/zouxy09/article/category/1218759)[图像处理																](https://blog.csdn.net/zouxy09/article/category/1218762)[计算机视觉																](https://blog.csdn.net/zouxy09/article/category/1218765)[嵌入式																](https://blog.csdn.net/zouxy09/article/category/1218763)[
							](https://blog.csdn.net/zouxy09/article/category/1218765)
[
																								](https://blog.csdn.net/zouxy09/article/category/1218762)
[
				](https://blog.csdn.net/zouxy09/article/category/1218759)
[
			](https://blog.csdn.net/zouxy09/article/category/1218759)
[
	](https://so.csdn.net/so/search/s.do?q=tts&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=palm&t=blog)
**交互系统的构建之（四）手掌与拳头检测加盟****TLD**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
人机交互系统的构建之（一）http://blog.csdn.net/zouxy09/article/details/7919618中提到我的整个交互系统包含以下部分：TLD系统、TTS语音合成、语音识别、手势和语音控制鼠标和键盘、运行前加入手掌的检测（这样就不用鼠标画目标box了）、拳头的检测等等。
**目前已完成：**
1、TLD系统的介绍与编译：
http://blog.csdn.net/zouxy09/article/details/7893022
2、TLD系统工作过程分析：
http://blog.csdn.net/zouxy09/article/details/7893026
3、重写Makefile编译TLD系统:
http://blog.csdn.net/zouxy09/article/details/7919618
4、Linux下鼠标和键盘的模拟控制：
http://blog.csdn.net/zouxy09/article/details/7920253
5、TTS技术简单介绍和Ekho（余音）TTS的安装与编程：
http://blog.csdn.net/zouxy09/article/details/7909154
6、TTS语音合成的添加：
http://blog.csdn.net/zouxy09/article/details/7921032
7、OpenCV中人脸检测Haar分类器分析（本文的手掌和拳头的检测也是基于这个的）
http://blog.csdn.net/zouxy09/article/details/7922923
8、计算机视觉目标检测的框架与过程分析
http://blog.csdn.net/zouxy09/article/details/7928771
**本文将完成：**
运行前加入手掌的检测，检测到手掌作为需要跟踪的目标（这样就不需要用鼠标框选目标box了）、运行时加入拳头的检测，检测到拳头，则表示用户单击。
有关于本系统构建的文章结构都会由三个部分来组织，一是该功能模块的介绍和在Linux下简单应用程序的实现；二是将该功能模块整合到交互系统（先以TLD为地基）中去；三是分析目前存在的问题与未来的解决构思。
**一、手掌和拳头的检测原理和编程**
**1.1****、原理**
该部分使用到的手掌的检测是基于OpenCV的Haar+AdaBoost，而拳头检测是基于OpenCV的LBP+ AdaBoost；
检测的原理见：
http://blog.csdn.net/zouxy09/article/details/7922923
http://blog.csdn.net/zouxy09/article/details/7928771
关于目标检测的图像特征提取的分析，我也做了一个总结：
目标检测的图像特征提取之（一）HOG特征
http://blog.csdn.net/zouxy09/article/details/7929348
目标检测的图像特征提取之（二）LBP特征
http://blog.csdn.net/zouxy09/article/details/7929531
目标检测的图像特征提取之（三）Haar特征
http://blog.csdn.net/zouxy09/article/details/7929570
目前整体交互系统的检测过程我修改如下：
系统启动，语音会进行提示；然后先用拳头放进摄像头区域，然后张开手掌，系统就会检测到手掌，然后我提取检测到的手掌的一个方框，作为TLD需要跟踪的目标Bounding Box（不再需要鼠标去框选了），这样TLD就会跟踪这个手掌，用手掌去控制鼠标。然后再处理帧的过程中，会检测拳头，检测到拳头表示鼠标单击事件。但拳头检测并不是检测整张图像，为了提高检测速度，我这样处理：假设跟踪器跟踪到了手掌Box，那么这时候握拳的地方肯定在手掌box的附近，所以把这个手掌box稍微加大（为了稳定），把这个box框内的图像传给拳头检测器检测。
**1.2****、编程**
在现在的OpenCV版本中，使用级联分类器做人脸检测的时候，有两种选择：一是使用老版本的CvHaarClassifierCascade，一是使用新版本的CascadeClassifier类。老版本的分类器只支持类Haar特征，而新版本的分类器既可以使用Haar，也可以使用LBP特征。
类CascadeClassifier中实际上封装了新旧两种分类器，对于老版本的xml模型文件，CascadeClassifier会用CvHaarClassifierCascade去解析，并用cvHaarDetectObjects去检测。而对于新版本的xml文件，CascadeClassifier会用自己的一套格式去解析。
**使用起来很简单，只需要简单的几步：**
//创建级联分类器对象
CascadeClassifier cascade;
//加载级联分类器
cascade.load( “haarcascade_frontalface_alt.xml” );
//转换成灰度图像
cvtColor( img, smallImg, CV_BGR2GRAY );
//可以先缩放灰度图像，再对图像进行直方图均衡
equalizeHist( smallImg, smallImg );
//检测
cascade.detectMultiScale( smallImg, objects, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30) );
检测函数是detectMultiScale，它的参数比较多。该函数以不同大小的窗口扫描输入图像寻找目标。
函数原型是：void CascadeClassifier::detectMultiScale(const Mat& image, vector<Rect>& objects, double scaleFactor=1.1, int minNeighbors=3, int flags=0, Size minSize=Size(), Size maxSize=Size());
参数image为输入的灰度图像，objects为得到被检测物体的矩形框向量组，scaleFactor参数决定每两个不同大小的窗口之间有多大的跳跃，默认值为1.1，参数minNeighbors控制误检率，现实图像中的目标可能会被多次检测到，因为周围的像素和不同大小的窗口也会检测到目标。默认为3，表示至少有3次检测到目标，才认为是目标。flags对于新的分类器没有用（但目前的haar分类器都是旧版的，CV_HAAR_DO_CANNY_PRUNING利用Canny边缘检测器来排除一些边缘很少或者很多的图像区域，CV_HAAR_SCALE_IMAGE就是按比例正常检测，CV_HAAR_FIND_BIGGEST_OBJECT只检测最大的物体，CV_HAAR_DO_ROUGH_SEARCH只做初略检测），默认为0。minSize和maxSize用来限制得到的目标区域的范围，例如你的目标不会太小，也不会太大，所以太小和太大的区域就不用检测了。
OpenCV带了一个检测的例子：功能是检测人脸和人的双眼
http://opencv.itseez.com/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html
代码如下：

```cpp
#include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include <iostream>
 #include <stdio.h>
 using namespace std;
 using namespace cv;
 /** Function Headers */
 void detectAndDisplay( Mat frame );
 /** Global variables */
 String face_cascade_name = "haarcascade_frontalface_alt.xml";
 String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 string window_name = "Capture - Face detection";
 RNG rng(12345);
 /** @function main */
 int main( int argc, const char** argv )
 {
   CvCapture* capture;
   Mat frame;
   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   //-- 2. Read the video stream
   capture = cvCaptureFromCAM( -1 );
   if( capture )
   {
     while( true )
     {
   frame = cvQueryFrame( capture );
   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }
       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0;
 }
/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  std::vector<Rect> faces;
  Mat frame_gray;
  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );
  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
  for( int i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;
    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    for( int j = 0; j < eyes.size(); j++ )
     {
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
     }
  }
  //-- Show what you got
  imshow( window_name, frame );
```

另外，我的手掌和拳头的分类器都是别人弄好的了，效果也不错。
**二、整合到****TLD****系统中**
在run_tld.cpp中添加的代码如下：

```cpp
//xiaoyi added here
String fist_cascade = "c1_lbp_900_20.xml";
String palm_cascade = "palmcascade2.xml";
……
……
//xiaoyi added here
int detect_object( Mat& img, CascadeClassifier& cascade, vector<Rect> &objects, float scale)
{
	//将图片缩小，加快检测速度
	Mat smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 ); 
	//将尺寸缩小到1/scale,用线性插值
    resize( img, smallImg, smallImg.size(), 0, 0, INTER_LINEAR ); 
    equalizeHist( smallImg, smallImg );//直方图均衡
    cascade.detectMultiScale( smallImg, objects, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30) );
   
    return objects.size();
}
//xiaoyi added here
int detect_fist( Mat& img, CascadeClassifier& cascade, float scale)
{
    vector<Rect> fists;
    detect_object( img, cascade, fists, scale);	
    printf("the fist.size is---------------------------------%d \n", fists.size());
    return fists.size();
}
//xiaoyi added here
int detect_palm_first(Mat &img)
{
    CascadeClassifier cascade;//创建级联分类器对象
    double scale = 1.3;
    vector<Rect> palms;
	Mat gray;
    if( !cascade.load( palm_cascade ) ) //从指定的文件目录中加载级联分类器
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return 0;
    }
	
    //因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
    cvtColor( img, gray, CV_BGR2GRAY );
	detect_object( gray, cascade, palms, scale);	
   
    if (!palms.size())
        return 0;
    box.width = cvRound( palms[0].width * scale * 0.7 );
    box.height = cvRound( palms[0].height * scale * 0.8 );
    box.x = cvRound(palms[0].x * scale + palms[0].width * scale * 0.15 );
    box.y = cvRound(palms[0].y * scale +  palms[0].height * scale * 0.1 );
    gotBB = true;
    return 1;
}
……
……
main()函数
{
……
GETBOUNDINGBOX:
    while(!gotBB)
    {
        if (!fromfile) {
            capture >> frame;
        }
        else
            first.copyTo(frame);
        cvtColor(frame, last_gray, CV_RGB2GRAY);
        //加入手掌检测，作为初始的bounding box
        detect_palm_first(frame); //xiaoyi added here
        drawBox(frame,box);
        imshow("TLD", frame);
        if (cvWaitKey(33) == 'q')
            return 0;
    }
……
	int load_cascade_success = 1;
	Rect detect_fist_box;
	double scale = 1.1;
    CascadeClassifier cascade;  //创建级联分类器对象
    if( !cascade.load( fist_cascade ) ) //从指定的文件目录中加载级联分类器
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        load_cascade_success = 0;
    }
        detect_fist_box.x = pbox.x - 40;
        if (detect_fist_box.x < 0)
            detect_fist_box.x = 0;
        detect_fist_box.y = pbox.y - 20;
        if (detect_fist_box.y < 0)
            detect_fist_box.y = 0;
        detect_fist_box.width = pbox.width + 80;
        if ((detect_fist_box.x + detect_fist_box.width) > 320)
            detect_fist_box.width = 320 - detect_fist_box.x;
        detect_fist_box.height = pbox.height + 40;
        if ((detect_fist_box.y + detect_fist_box.height) > 240)
            detect_fist_box.height = 240 - detect_fist_box.y;
        Mat temp = detect_gray(detect_fist_box);
        if (load_cascade_success )
        {
            if ( (detect_fist(temp, cascade, scale)) > 0 )
            {
                if (continue_fist == 0)
                    simulate_key(fd_mouse, BTN_LEFT);
                continue_fist++;
            }
            else
                continue_fist = 0;
        }
……
```

系统集合后试验了一下，感觉还是挺不错的，呵呵！
**三、目前存在的问题和解决构思**
1、需不需要通过Opencv的手掌检测去加固TLD的手掌检测和跟踪？
2、单击的时候实际上是不属于TLD的跟踪轨迹的了，这对于系统是否存在影响？
3、其他的稳定性问题

[
](https://so.csdn.net/so/search/s.do?q=palm&t=blog)
[
  ](https://so.csdn.net/so/search/s.do?q=image&t=blog)