# 【图像处理】计算机视觉 透视变换 Perspective Transformation - YZXnuaa的博客 - CSDN博客
2018年01月30日 14:42:53[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1104

透视变换（Perspective Transformation)是指利用透视中心、[像点](http://baike.baidu.com/view/3438358.htm)、目标点三点共线的条件，按[透视旋转定律](http://baike.baidu.com/view/2077152.htm)使承影面（透视面）绕[迹线](http://baike.baidu.com/view/984087.htm)（透视轴）旋转某一角度，破坏原有的投影光线束，仍能保持承影面上投影几何图形不变的变换。
![](https://img-blog.csdn.net/20140521144153765)
透视变换(Perspective Transformation)是将图片投影到一个新的视平面(Viewing Plane)，也称作投影映射(Projective Mapping)。通用的变换公式为：
![](https://img-blog.csdn.net/20140521142738671)
u,v是原始图片坐标，对应得到变换后的图片坐标x,y,其中![](https://img-blog.csdn.net/20140521142754687)。
变换矩阵![](https://img-blog.csdn.net/20140521142820609)可以拆成4部分，![](https://img-blog.csdn.net/20140521142832296)表示线性变换，比如scaling（缩放），shearing（错切）和ratotion（翻转）。![](https://img-blog.csdn.net/20140521142911312)用于平移，![](https://img-blog.csdn.net/20140521142842390)产生透视变换。所以可以理解成仿射（线性变换+平移）等是透视变换的特殊形式。经过透视变换之后的图片通常不是平行四边形（除非映射视平面和原来平面平行的情况）。
重写之前的变换公式可以得到（默认w为1？）：
![](https://img-blog.csdn.net/20140521143006406)
所以，**已知变换对应的几个点就可以求取变换公式，**反之，特定的变换公式也能生成新的变换后的图片。简单的看一个正方形到四边形的变换：
变换的4组对应点可以表示成：![](https://img-blog.csdn.net/20140521143117828)
根据变换公式得到（默认a33为1？）：
![](https://img-blog.csdn.net/20140521143219625)
定义几个辅助变量：
![](https://img-blog.csdn.net/20140521143302781)
![](https://img-blog.csdn.net/20140521143339968)都为0时变换平面与原来是平行的，可以得到：
![](https://img-blog.csdn.net/20140521143422484)
![](https://img-blog.csdn.net/20140521143339968)不为0时，得到：
![](https://img-blog.csdn.net/20140521143514640)
求解出的变换矩阵就可以将一个正方形变换到四边形。反之，四边形变换到正方形也是一样的。于是，我们通过两次变换：四边形变换到正方形+正方形变换到四边形就可以将任意一个四边形变换到另一个四边形。
![](https://img-blog.csdn.net/20140521143605687)
看一段代码：
**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/26471527#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/26471527#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/363566/fork)
- PerspectiveTransform::PerspectiveTransform(float inA11, float inA21,   
- float inA31, float inA12,   
- float inA22, float inA32,   
- float inA13, float inA23,   
- float inA33) :   
-   a11(inA11), a12(inA12), a13(inA13), a21(inA21), a22(inA22), a23(inA23),  
-   a31(inA31), a32(inA32), a33(inA33) {}  
- 
- PerspectiveTransform PerspectiveTransform::quadrilateralToQuadrilateral(float x0, float y0, float x1, float y1,  
- float x2, float y2, float x3, float y3, float x0p, float y0p, float x1p, float y1p, float x2p, float y2p,  
- float x3p, float y3p) {  
-   PerspectiveTransform qToS = PerspectiveTransform::quadrilateralToSquare(x0, y0, x1, y1, x2, y2, x3, y3);  
-   PerspectiveTransform sToQ =  
-     PerspectiveTransform::squareToQuadrilateral(x0p, y0p, x1p, y1p, x2p, y2p, x3p, y3p);  
- return sToQ.times(qToS);  
- }  
- 
- PerspectiveTransform PerspectiveTransform::squareToQuadrilateral(float x0, float y0, float x1, float y1, float x2,  
- float y2, float x3, float y3) {  
- float dx3 = x0 - x1 + x2 - x3;  
- float dy3 = y0 - y1 + y2 - y3;  
- if (dx3 == 0.0f && dy3 == 0.0f) {  
-     PerspectiveTransform result(PerspectiveTransform(x1 - x0, x2 - x1, x0, y1 - y0, y2 - y1, y0, 0.0f,  
-                                      0.0f, 1.0f));  
- return result;  
-   } else {  
- float dx1 = x1 - x2;  
- float dx2 = x3 - x2;  
- float dy1 = y1 - y2;  
- float dy2 = y3 - y2;  
- float denominator = dx1 * dy2 - dx2 * dy1;  
- float a13 = (dx3 * dy2 - dx2 * dy3) / denominator;  
- float a23 = (dx1 * dy3 - dx3 * dy1) / denominator;  
-     PerspectiveTransform result(PerspectiveTransform(x1 - x0 + a13 * x1, x3 - x0 + a23 * x3, x0, y1 - y0  
-                                      + a13 * y1, y3 - y0 + a23 * y3, y0, a13, a23, 1.0f));  
- return result;  
-   }  
- }  
- 
- PerspectiveTransform PerspectiveTransform::quadrilateralToSquare(float x0, float y0, float x1, float y1, float x2,  
- float y2, float x3, float y3) {  
- // Here, the adjoint serves as the inverse:
- return squareToQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3).buildAdjoint();  
- }  
- 
- PerspectiveTransform PerspectiveTransform::buildAdjoint() {  
- // Adjoint is the transpose of the cofactor matrix:
-   PerspectiveTransform result(PerspectiveTransform(a22 * a33 - a23 * a32, a23 * a31 - a21 * a33, a21 * a32  
-                                    - a22 * a31, a13 * a32 - a12 * a33, a11 * a33 - a13 * a31, a12 * a31 - a11 * a32, a12 * a23 - a13 * a22,  
-                                    a13 * a21 - a11 * a23, a11 * a22 - a12 * a21));  
- return result;  
- }  
- 
- PerspectiveTransform PerspectiveTransform::times(PerspectiveTransform other) {  
-   PerspectiveTransform result(PerspectiveTransform(a11 * other.a11 + a21 * other.a12 + a31 * other.a13,  
-                                    a11 * other.a21 + a21 * other.a22 + a31 * other.a23, a11 * other.a31 + a21 * other.a32 + a31  
-                                    * other.a33, a12 * other.a11 + a22 * other.a12 + a32 * other.a13, a12 * other.a21 + a22  
-                                    * other.a22 + a32 * other.a23, a12 * other.a31 + a22 * other.a32 + a32 * other.a33, a13  
-                                    * other.a11 + a23 * other.a12 + a33 * other.a13, a13 * other.a21 + a23 * other.a22 + a33  
-                                    * other.a23, a13 * other.a31 + a23 * other.a32 + a33 * other.a33));  
- return result;  
- }  
- 
- void PerspectiveTransform::transformPoints(vector<float> &points) {  
- int max = points.size();  
- for (int i = 0; i < max; i += 2) {  
- float x = points[i];  
- float y = points[i + 1];  
- float denominator = a13 * x + a23 * y + a33;  
-     points[i] = (a11 * x + a21 * y + a31) / denominator;  
-     points[i + 1] = (a12 * x + a22 * y + a32) / denominator;  
-   }  
- }  
对一张透视图片变换回正面图的效果：
**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/26471527#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/26471527#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/363566/fork)
- int main(){  
-     Mat img=imread("boy.png");  
- int img_height = img.rows;  
- int img_width = img.cols;  
-     Mat img_trans = Mat::zeros(img_height,img_width,CV_8UC3);  
-     PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(  
-         0,0,  
-         img_width-1,0,  
-         0,img_height-1,  
-         img_width-1,img_height-1,  
-         150,250, // top left
-         771,0, // top right
-         0,1023,// bottom left
-         650,1023  
-         );  
-     vector<float> ponits;  
- for(int i=0;i<img_height;i++){  
- for(int j=0;j<img_width;j++){  
-             ponits.push_back(j);  
-             ponits.push_back(i);  
-         }  
-     }  
-     tansform.transformPoints(ponits);  
- for(int i=0;i<img_height;i++){  
-         uchar*  t= img_trans.ptr<uchar>(i);  
- for (int j=0;j<img_width;j++){  
- int tmp = i*img_width+j;  
- int x = ponits[tmp*2];  
- int y = ponits[tmp*2+1];  
- if(x<0||x>(img_width-1)||y<0||y>(img_height-1))  
- continue;  
-             uchar* p = img.ptr<uchar>(y);  
-             t[j*3] = p[x*3];  
-             t[j*3+1] = p[x*3+1];  
-             t[j*3+2] = p[x*3+2];  
-         }  
-     }  
-     imwrite("trans.png",img_trans);  
- return 0;  
- }  
![](https://img-blog.csdn.net/20140521144153765)
透视变换的原理和矩阵求解请参见前一篇[《透视变换 Perspective Transformation》](http://blog.csdn.net/xiaowei_cqu/article/details/26471527)。在OpenCV中也实现了透视变换的公式求解和变换函数。
求解变换公式的函数：
**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)
- Mat getPerspectiveTransform(const Point2f src[], const Point2f dst[])  
输入原始图像和变换之后的图像的对应4个点，便可以得到变换矩阵。之后用求解得到的矩阵输入perspectiveTransform便可以对一组点进行变换：
**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)
- void perspectiveTransform(InputArray src, OutputArray dst, InputArray m)  
注意这里src和dst的输入并不是图像，而是图像对应的坐标。应用前一篇的例子，做个相反的变换：
**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)
- int main( )  
- {  
-     Mat img=imread("boy.png");  
- int img_height = img.rows;  
- int img_width = img.cols;  
-     vector<Point2f> corners(4);  
-     corners[0] = Point2f(0,0);  
-     corners[1] = Point2f(img_width-1,0);  
-     corners[2] = Point2f(0,img_height-1);  
-     corners[3] = Point2f(img_width-1,img_height-1);  
-     vector<Point2f> corners_trans(4);  
-     corners_trans[0] = Point2f(150,250);  
-     corners_trans[1] = Point2f(771,0);  
-     corners_trans[2] = Point2f(0,img_height-1);  
-     corners_trans[3] = Point2f(650,img_height-1);  
- 
-     Mat transform = getPerspectiveTransform(corners,corners_trans);  
-     cout<<transform<<endl;  
-     vector<Point2f> ponits, points_trans;  
- for(int i=0;i<img_height;i++){  
- for(int j=0;j<img_width;j++){  
-             ponits.push_back(Point2f(j,i));  
-         }  
-     }  
- 
-     perspectiveTransform( ponits, points_trans, transform);  
-     Mat img_trans = Mat::zeros(img_height,img_width,CV_8UC3);  
- int count = 0;  
- for(int i=0;i<img_height;i++){  
-         uchar* p = img.ptr<uchar>(i);  
- for(int j=0;j<img_width;j++){  
- int y = points_trans[count].y;  
- int x = points_trans[count].x;  
-             uchar* t = img_trans.ptr<uchar>(y);  
-             t[x*3]  = p[j*3];  
-             t[x*3+1]  = p[j*3+1];  
-             t[x*3+2]  = p[j*3+2];  
-             count++;  
-         }  
-     }  
-     imwrite("boy_trans.png",img_trans);  
- 
- return 0;  
- }  
得到变换之后的图片：
![](https://img-blog.csdn.net/20140521152755093)
注意这种将原图变换到对应图像上的方式会有一些没有被填充的点，也就是右图中黑色的小点。解决这种问题一是用差值的方式，再一种比较简单就是不用原图的点变换后对应找新图的坐标，而是直接在新图上找反向变换原图的点。说起来有点绕口，具体见前一篇[《透视变换 Perspective
 Transformation》](http://blog.csdn.net/xiaowei_cqu/article/details/26471527)的代码应该就能懂啦。
除了getPerspectiveTransform()函数，OpenCV还提供了findHomography()的函数，不是用点来找，而是直接用透视平面来找变换公式。这个函数在特征匹配的经典例子中有用到，也非常直观：
**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/26478135#)
- int main( int argc, char** argv )  
- {  
-     Mat img_object = imread( argv[1], IMREAD_GRAYSCALE );  
-     Mat img_scene = imread( argv[2], IMREAD_GRAYSCALE );  
- if( !img_object.data || !img_scene.data )  
-     { std::cout<< " --(!) Error reading images " << std::endl; return -1; }  
- 
- //-- Step 1: Detect the keypoints using SURF Detector
- int minHessian = 400;  
-     SurfFeatureDetector detector( minHessian );  
-     std::vector<KeyPoint> keypoints_object, keypoints_scene;  
-     detector.detect( img_object, keypoints_object );  
-     detector.detect( img_scene, keypoints_scene );  
- 
- //-- Step 2: Calculate descriptors (feature vectors)
-     SurfDescriptorExtractor extractor;  
-     Mat descriptors_object, descriptors_scene;  
-     extractor.compute( img_object, keypoints_object, descriptors_object );  
-     extractor.compute( img_scene, keypoints_scene, descriptors_scene );  
- 
- //-- Step 3: Matching descriptor vectors using FLANN matcher
-     FlannBasedMatcher matcher;  
-     std::vector< DMatch > matches;  
-     matcher.match( descriptors_object, descriptors_scene, matches );  
- double max_dist = 0; double min_dist = 100;  
- 
- //-- Quick calculation of max and min distances between keypoints
- for( int i = 0; i < descriptors_object.rows; i++ )  
-     { double dist = matches[i].distance;  
- if( dist < min_dist ) min_dist = dist;  
- if( dist > max_dist ) max_dist = dist;  
-     }  
- 
-     printf("-- Max dist : %f \n", max_dist );  
-     printf("-- Min dist : %f \n", min_dist );  
- 
- //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
-     std::vector< DMatch > good_matches;  
- 
- for( int i = 0; i < descriptors_object.rows; i++ )  
-     { if( matches[i].distance < 3*min_dist )  
-     { good_matches.push_back( matches[i]); }  
-     }  
- 
-     Mat img_matches;  
-     drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,  
-         good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),  
-         vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );  
- 
- //-- Localize the object from img_1 in img_2
-     std::vector<Point2f> obj;  
-     std::vector<Point2f> scene;  
- 
- for( size_t i = 0; i < good_matches.size(); i++ )  
-     {  
- //-- Get the keypoints from the good matches
-         obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );  
-         scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );  
-     }  
- 
-     Mat H = findHomography( obj, scene, RANSAC );  
- 
- //-- Get the corners from the image_1 ( the object to be "detected" )
-     std::vector<Point2f> obj_corners(4);  
-     obj_corners[0] = Point(0,0); obj_corners[1] = Point( img_object.cols, 0 );  
-     obj_corners[2] = Point( img_object.cols, img_object.rows ); obj_corners[3] = Point( 0, img_object.rows );  
-     std::vector<Point2f> scene_corners(4);  
-     perspectiveTransform( obj_corners, scene_corners, H);  
- //-- Draw lines between the corners (the mapped object in the scene - image_2 )
-     Point2f offset( (float)img_object.cols, 0);  
-     line( img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4 );  
-     line( img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar( 0, 255, 0), 4 );  
-     line( img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar( 0, 255, 0), 4 );  
-     line( img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar( 0, 255, 0), 4 );  
- 
- //-- Show detected matches
-     imshow( "Good Matches & Object detection", img_matches );  
-     waitKey(0);  
- return 0;  
- }  
代码运行效果：
![](https://img-blog.csdn.net/20140521153450140)
findHomography()函数直接通过两个平面上相匹配的特征点求出变换公式，之后代码又对原图的四个边缘点进行变换，在右图上画出对应的矩形。这个图也很好地解释了所谓透视变换的“Viewing Plane”。
