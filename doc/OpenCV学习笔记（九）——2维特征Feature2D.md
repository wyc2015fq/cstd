# OpenCV学习笔记（九）——2维特征Feature2D

2011年10月24日 20:51:10

yang_xian521

阅读数：44536

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

基于特征点的图像匹配是图像处理中经常会遇到的问题，手动选取特征点太麻烦了。比较经典常用的特征点自动提取的办法有Harris特征、SIFT特征、SURF特征。

先介绍利用SURF特征的特征描述办法，其操作封装在类SurfFeatureDetector中，利用类内的detect函数可以检测出SURF特征的关键点，保存在vector容器中。第二部利用SurfDescriptorExtractor类进行特征向量的相关计算。将之前的vector变量变成向量矩阵形式保存在Mat中。最后强行匹配两幅图像的特征向量，利用了类BruteForceMatcher中的函数match。代码如下：



```cpp
/**
 * @file SURF_descriptor
 * @brief SURF detector + descritpor + BruteForce Matcher + drawing matches with OpenCV functions
 * @author A. Huaman
 */
 
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
 
using namespace cv;
 
void readme();
 
/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{
  if( argc != 3 )
  { return -1; }
 
  Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
  
  if( !img_1.data || !img_2.data )
  { return -1; }
 
  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;
 
  SurfFeatureDetector detector( minHessian );
 
  std::vector<KeyPoint> keypoints_1, keypoints_2;
 
  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );
 
  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;
 
  Mat descriptors_1, descriptors_2;
 
  extractor.compute( img_1, keypoints_1, descriptors_1 );
  extractor.compute( img_2, keypoints_2, descriptors_2 );
 
  //-- Step 3: Matching descriptor vectors with a brute force matcher
  BruteForceMatcher< L2<float> > matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );
 
  //-- Draw matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2, matches, img_matches ); 
 
  //-- Show detected matches
  imshow("Matches", img_matches );
 
  waitKey(0);
 
  return 0;
}
 
/**
 * @function readme
 */
void readme()
{ std::cout << " Usage: ./SURF_descriptor <img1> <img2>" << std::endl; }
```

当然，进行强匹配的效果不够理想，这里再介绍一种FLANN特征匹配算法。前两步与上述代码相同，第三步利用FlannBasedMatcher类进行特征匹配，并只保留好的特征匹配点，代码如下：



```cpp
  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );
 
  double max_dist = 0; double min_dist = 100;
 
  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_1.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }
 
  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );
  
  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  //-- PS.- radiusMatch can also be used here.
  std::vector< DMatch > good_matches;
 
  for( int i = 0; i < descriptors_1.rows; i++ )
  { if( matches[i].distance < 2*min_dist )
    { good_matches.push_back( matches[i]); }
  }  
 
  //-- Draw only "good" matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2, 
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), 
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS ); 
 
  //-- Show detected matches
  imshow( "Good Matches", img_matches );
```

在FLANN特征匹配的基础上，还可以进一步利用Homography映射找出已知物体。具体来说就是利用findHomography函数利用匹配的关键点找出相应的变换，再利用perspectiveTransform函数映射点群。具体代码如下：



```cpp
  //-- Localize the object from img_1 in img_2 
  std::vector<Point2f> obj;
  std::vector<Point2f> scene;
 
  for( int i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    obj.push_back( keypoints_1[ good_matches[i].queryIdx ].pt );
    scene.push_back( keypoints_2[ good_matches[i].trainIdx ].pt ); 
  }
 
  Mat H = findHomography( obj, scene, CV_RANSAC );
 
  //-- Get the corners from the image_1 ( the object to be "detected" )
  Point2f obj_corners[4] = { cvPoint(0,0), cvPoint( img_1.cols, 0 ), cvPoint( img_1.cols, img_1.rows ), cvPoint( 0, img_1.rows ) };
  Point scene_corners[4];
 
  //-- Map these corners in the scene ( image_2)
  for( int i = 0; i < 4; i++ )
  {
    double x = obj_corners[i].x; 
    double y = obj_corners[i].y;
 
    double Z = 1./( H.at<double>(2,0)*x + H.at<double>(2,1)*y + H.at<double>(2,2) );
    double X = ( H.at<double>(0,0)*x + H.at<double>(0,1)*y + H.at<double>(0,2) )*Z;
    double Y = ( H.at<double>(1,0)*x + H.at<double>(1,1)*y + H.at<double>(1,2) )*Z;
    scene_corners[i] = cvPoint( cvRound(X) + img_1.cols, cvRound(Y) );
  }  
   
  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
  line( img_matches, scene_corners[0], scene_corners[1], Scalar(0, 255, 0), 2 );
  line( img_matches, scene_corners[1], scene_corners[2], Scalar( 0, 255, 0), 2 );
  line( img_matches, scene_corners[2], scene_corners[3], Scalar( 0, 255, 0), 2 );
  line( img_matches, scene_corners[3], scene_corners[0], Scalar( 0, 255, 0), 2 );
 
  //-- Show detected matches
  imshow( "Good Matches & Object detection", img_matches );
```

 然后再看一下Harris特征检测，在计算机视觉中，通常需要找出两帧图像的匹配点，如果能找到两幅图像如何相关，就能提取出两幅图像的信息。我们说的特征的最大特点就是它具有唯一可识别这一特点，图像特征的类型通常指边界、角点（兴趣点）、斑点（兴趣区域）。角点就是图像的一个局部特征，应用广泛。harris角点检测是一种直接基于灰度图像的角点提取算法，稳定性高，尤其对L型角点检测精度高，但由于采用了高斯滤波，运算速度相对较慢，角点信息有丢失和位置偏移的现象，而且角点提取有聚簇现象。具体实现就是使用函数cornerHarris实现。 





除了利用Harris进行角点检测，还可以利用Shi-Tomasi方法进行角点检测。使用函数goodFeaturesToTrack对角点进行检测，效果也不错。也可以自己制作角点检测的函数，需要用到cornerMinEigenVal函数和minMaxLoc函数，最后的特征点选取，判断条件要根据自己的情况编辑。如果对特征点，角点的精度要求更高，可以用cornerSubPix函数将角点定位到子像素。