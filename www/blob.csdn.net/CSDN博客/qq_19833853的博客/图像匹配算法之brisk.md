# 图像匹配算法之brisk - qq_19833853的博客 - CSDN博客
2017年06月08日 21:51:18[jessie Fei](https://me.csdn.net/qq_19833853)阅读数：245
**单目标匹配**
#include <cv.h>  
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/core/core.hpp> 
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp> 
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/nonfree/nonfree.hpp>  
#include <Windows.h>  
using namespace cv;
using namespace std;
int main()
{
//Load Image  
Mat c_src1 = imread("1-301.bmp");
Mat c_src2 = imread("1-200(1).bmp");
Mat src1 = imread("1-301.bmp", CV_LOAD_IMAGE_GRAYSCALE);
Mat src2 = imread("1-200(1).bmp", CV_LOAD_IMAGE_GRAYSCALE);
if (!src1.data || !src2.data)
{
cout << "Error reading images " << std::endl;
return -1;
}
//feature detect  
BRISK detector;
vector<KeyPoint> kp1, kp2;
double start = GetTickCount();
detector.detect(src1, kp1);
detector.detect(src2, kp2);
//cv::BRISK extractor;  
Mat des1, des2;//descriptor  
detector.compute(src1, kp1, des1);
detector.compute(src2, kp2, des2);
Mat res1, res2;
int drawmode = DrawMatchesFlags::DRAW_RICH_KEYPOINTS;
drawKeypoints(c_src1, kp1, res1, Scalar::all(-1), drawmode);//画出特征点  
drawKeypoints(c_src2, kp2, res2, Scalar::all(-1), drawmode);
cout << "size of description of Img1: " << kp1.size() << endl;
cout << "size of description of Img2: " << kp2.size() << endl;
BFMatcher matcher(NORM_HAMMING);
vector<DMatch> matches;
matcher.match(des1, des2, matches);
double end = GetTickCount();
cout << "耗时：" << (end - start) << "ms" << endl;
Mat img_match;
drawMatches(src1, kp1, src2, kp2, matches, img_match);
cout << "number of matched points: " << matches.size() << endl;
namedWindow("matches", WINDOW_NORMAL);
imshow("matches", img_match);
vector<Point2f> obj;
vector<Point2f> scene;
for (unsigned int i = 0; i < matches.size(); i++)
{
obj.push_back(kp1[matches[i].queryIdx].pt);
scene.push_back(kp2[matches[i].trainIdx].pt);
}
Mat H = findHomography(obj, scene, CV_RANSAC);
//-- Get the corners from the image_1 ( the object to be "detected" )  
std::vector<Point2f> obj_corners(4);
obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(src1.cols, 0);
obj_corners[2] = cvPoint(src1.cols,src1.rows); obj_corners[3] = cvPoint(0, src1.rows);
std::vector<Point2f> scene_corners(4);
perspectiveTransform(obj_corners, scene_corners, H);
//-- Draw lines between the corners (the mapped object in the scene - image_2 )  
line(img_match, scene_corners[0] + Point2f(src1.cols, 0), scene_corners[1] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
line(img_match, scene_corners[1] + Point2f(src1.cols, 0), scene_corners[2] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
line(img_match, scene_corners[2] + Point2f(src1.cols, 0), scene_corners[3] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
line(img_match, scene_corners[3] + Point2f(src1.cols, 0), scene_corners[0] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
//-- Show detected matches  
namedWindow("Good Matches & Object detection", WINDOW_NORMAL);
imshow("Good Matches & Object detection", img_match);
waitKey(0);
return 0;
}
![](https://img-blog.csdn.net/20170608215402048)
![](https://img-blog.csdn.net/20170608215450267)
[OpenCV中feature2D学习——BFMatcher和FlannBasedMatcher](http://blog.csdn.net/holybin/article/details/40926315) 
 暴力匹配和近似匹配的区别
二者的区别在于BFMatcher总是尝试所有可能的匹配，从而使得它总能够找到最佳匹配，这也是Brute Force（暴力法）的原始含义。而FlannBasedMatcher中FLANN的含义是Fast Library forApproximate Nearest Neighbors，从字面意思可知它是一种近似法，[算法](http://lib.csdn.net/base/datastructure)更快但是找到的是最近邻近似匹配，所以当我们需要找到一个相对好的匹配但是不需要最佳匹配的时候往往使用FlannBasedMatcher。当然也可以通过调整FlannBasedMatcher的参数来提高匹配的精度或者提高算法速度，但是相应地算法速度或者算法精度会受到影响。
此外，使用特征提取过程得到的特征描述符（descriptor）数据类型有的是float类型的，比如说SurfDescriptorExtractor，
SiftDescriptorExtractor，有的是uchar类型的，比如说有ORB，BriefDescriptorExtractor。对应float类型的匹配方式有：FlannBasedMatcher，BruteForce<L2<float>>，BruteForce<SL2<float>>，BruteForce<L1<float>>。对应uchar类型的匹配方式有：BruteForce<Hammin>，BruteForce<HammingLUT>。所以ORB和BRIEF特征描述子只能使用BruteForce匹配法。
需要进一部弄清楚的：
BRISK detector;
detector.detect(src1, kp1);
detector.compute(src1, kp1, des1);
- int minHessian = 400;  
- 
- 
-     SurfFeatureDetector detector( minHessian );
detector.detect( img_object, keypoints_object );  
- SurfDescriptorExtractor extractor;  
- 
- 
-     Mat descriptors_object, descriptors_scene;  
- 
- 
-     extractor.compute( img_object, keypoints_object, descriptors_object ); 
extractor.compute与detector.compute的区别
参考的博客：
1.http://blog.csdn.net/jia_zhengshen/article/details/18364735    brisk算法
2.http://blog.csdn.net/yaoweijiao/article/details/54313333     单应性矩阵原理及画出匹配物
3.http://blog.csdn.net/liuphahaha/article/details/50719275       图像矫正    虚拟广告牌  单应性矩阵原理
4.http://blog.csdn.net/chenjiazhou12/article/category/2103997/1           学习opencv基础
5.http://blog.csdn.net/sunshine_in_moon/article/details/45456979   
