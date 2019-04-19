# 图像匹配算法之ORB - qq_19833853的博客 - CSDN博客
2017年06月09日 18:10:52[jessie Fei](https://me.csdn.net/qq_19833853)阅读数：250
#include<opencv2\core\core.hpp>  
#include<opencv2\highgui\highgui.hpp>  
#include<opencv2\features2d\features2d.hpp>  
#include<opencv2\calib3d\calib3d.hpp>  
#include<iostream>  
using namespace std;
using namespace cv;
int main(int argc, char* argv[])
{
Mat queryImage, trainImage;
queryImage = imread("1-301.bmp", 0);
trainImage = imread("1-200(1).bmp",0);
Mat src1 = imread("1-301.bmp", CV_LOAD_IMAGE_GRAYSCALE);
Mat src2 = imread("1-200(1).bmp", CV_LOAD_IMAGE_GRAYSCALE);
if (!src1.data || !src2.data)
{
cout << "Error reading images " << std::endl;
return -1;
}
ORB orb;
vector<KeyPoint> queryKeyPoint, trainKeyPoint;
Mat queryDescriptor, trainDescriptor;
orb(queryImage, Mat(), queryKeyPoint, queryDescriptor);
orb(trainImage, Mat(), trainKeyPoint, trainDescriptor);
drawKeypoints(queryImage, queryKeyPoint, queryImage);
drawKeypoints(trainImage, trainKeyPoint, trainImage);
cout << queryKeyPoint.size() << endl;
cout << trainKeyPoint.size() << endl;
imshow("query", queryImage);
imshow("train", trainImage);
//NORM_L1, NORM_L2, NORM_HAMMING, NORM_HAMMING2  
BFMatcher matcher(NORM_L2);
vector<DMatch> matches;
matcher.match(queryDescriptor, trainDescriptor, matches);
vector<DMatch> ransac_matches;
vector<Point2f> queryPoint(matches.size()), trainPoint(matches.size());
for (int i = 0; i<matches.size(); i++)
{
queryPoint[i] = queryKeyPoint[matches[i].queryIdx].pt;
trainPoint[i] = trainKeyPoint[matches[i].trainIdx].pt;
}
vector<unsigned char> inlierMask(matches.size());
Mat H = findHomography(queryPoint, trainPoint, RANSAC, 3, inlierMask);
for (size_t i = 0; i<inlierMask.size(); i++)
{
if (inlierMask[i])
{
ransac_matches.push_back(matches[i]);
}
}
Mat image_match;
//  drawMatches(queryImage,queryKeyPoint,trainImage,trainKeyPoint,matches,image_match);  
drawMatches(queryImage, queryKeyPoint, trainImage, trainKeyPoint, ransac_matches, image_match);
namedWindow("image_match", WINDOW_NORMAL);
imshow("image_match", image_match);
waitKey(0);
vector<Point2f> obj_corners(4);
obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(src1.cols, 0);
obj_corners[2] = cvPoint(src1.cols, src1.rows); obj_corners[3] = cvPoint(0, src1.rows);
vector<Point2f> scene_corners(4);
perspectiveTransform(obj_corners, scene_corners, H);
line(image_match, scene_corners[0] + Point2f(src1.cols, 0), scene_corners[1] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
line(image_match, scene_corners[1] + Point2f(src1.cols, 0), scene_corners[2] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
line(image_match, scene_corners[2] + Point2f(src1.cols, 0), scene_corners[3] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
line(image_match, scene_corners[3] + Point2f(src1.cols, 0), scene_corners[0] + Point2f(src1.cols, 0), Scalar(0, 255, 0), 4);
namedWindow("Good Matches & Object detection", WINDOW_NORMAL);
imshow("Good Matches & Object detection", image_match);
waitKey(0);
return 0;
}
![](https://img-blog.csdn.net/20170609181259076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTk4MzM4NTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170609181321998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTk4MzM4NTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考博客
1.http://blog.csdn.net/quincuntial/article/details/50461692
2.http://blog.csdn.net/yhhyhhyhhyhh/article/details/54407916
