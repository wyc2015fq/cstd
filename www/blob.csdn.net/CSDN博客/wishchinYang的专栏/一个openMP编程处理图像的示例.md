# 一个openMP编程处理图像的示例 - wishchinYang的专栏 - CSDN博客
2013年10月20日 14:22:39[wishchin](https://me.csdn.net/wishchin)阅读数：1428
一个openMP编程处理图像的示例：     
     从硬盘读入两幅图像，对这两幅图像分别提取特征点，特征点匹配，最后将图像与匹配特征点画出来。理解该例子需要一些图像处理的基本知识，我不在此详细介绍。另外，编译该例需要opencv，我用的版本是2.3.1，关于opencv的安装与配置也不在此介绍。我们首先来看传统串行编程的方式。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 #include "opencv2/highgui/highgui.hpp"
 2 #include "opencv2/features2d/features2d.hpp"
 3 #include <iostream>
 4 #include <omp.h>
 5 int main( ){
 6     cv::SurfFeatureDetector detector( 400 );    
 7     cv::SurfDescriptorExtractor extractor;
 8     cv::BruteForceMatcher<cv::L2<float> > matcher;
 9     std::vector< cv::DMatch > matches;
10     cv::Mat im0,im1;
11     std::vector<cv::KeyPoint> keypoints0,keypoints1;
12     cv::Mat descriptors0, descriptors1;
13     double t1 = omp_get_wtime( );
14     //先处理第一幅图像
15     im0 = cv::imread("rgb0.jpg", CV_LOAD_IMAGE_GRAYSCALE );
16     detector.detect( im0, keypoints0);
17     extractor.compute( im0,keypoints0,descriptors0);
18     std::cout<<"find "<<keypoints0.size()<<"keypoints in im0"<<std::endl;
19     //再处理第二幅图像
20     im1 = cv::imread("rgb1.jpg", CV_LOAD_IMAGE_GRAYSCALE );
21     detector.detect( im1, keypoints1);
22     extractor.compute( im1,keypoints1,descriptors1);
23     std::cout<<"find "<<keypoints1.size()<<"keypoints in im1"<<std::endl;
24     double t2 = omp_get_wtime( );
25     std::cout<<"time: "<<t2-t1<<std::endl;
26     matcher.match( descriptors0, descriptors1, matches );
27     cv::Mat img_matches;
28     cv::drawMatches( im0, keypoints0, im1, keypoints1, matches, img_matches ); 
29     cv::namedWindow("Matches",CV_WINDOW_AUTOSIZE);
30     cv::imshow( "Matches", img_matches );
31     cv::waitKey(0);
32     return 1;
33 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
很明显，读入图像，提取特征点与特征描述子这部分可以改为并行执行，修改如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 #include "opencv2/highgui/highgui.hpp"
 2 #include "opencv2/features2d/features2d.hpp"
 3 #include <iostream>
 4 #include <vector>
 5 #include <omp.h>
 6 int main( ){
 7     int imNum = 2;
 8     std::vector<cv::Mat> imVec(imNum);
 9     std::vector<std::vector<cv::KeyPoint>>keypointVec(imNum);
10     std::vector<cv::Mat> descriptorsVec(imNum);
11     cv::SurfFeatureDetector detector( 400 );    cv::SurfDescriptorExtractor extractor;
12     cv::BruteForceMatcher<cv::L2<float> > matcher;
13     std::vector< cv::DMatch > matches;
14     char filename[100];
15     double t1 = omp_get_wtime( );
16 #pragma omp parallel for
17     for (int i=0;i<imNum;i++){
18         sprintf(filename,"rgb%d.jpg",i);
19         imVec[i] = cv::imread( filename, CV_LOAD_IMAGE_GRAYSCALE );
20         detector.detect( imVec[i], keypointVec[i] );
21         extractor.compute( imVec[i],keypointVec[i],descriptorsVec[i]);
22         std::cout<<"find "<<keypointVec[i].size()<<"keypoints in im"<<i<<std::endl;
23     }
24     double t2 = omp_get_wtime( );
25     std::cout<<"time: "<<t2-t1<<std::endl;
26     matcher.match( descriptorsVec[0], descriptorsVec[1], matches );
27     cv::Mat img_matches;
28     cv::drawMatches( imVec[0], keypointVec[0], imVec[1], keypointVec[1], matches, img_matches ); 
29     cv::namedWindow("Matches",CV_WINDOW_AUTOSIZE);
30     cv::imshow( "Matches", img_matches );
31     cv::waitKey(0);
32     return 1;
33 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
两种执行方式做比较，时间为：2.343秒v.s. 1.2441秒
在上面代码中，为了改成适合#pragma omp parallel for执行的方式，我们用了STL的vector来分别存放两幅图像、特征点与特征描述子，但在某些情况下，变量可能不适合放在vector里，此时应该怎么办呢？这就要用到openMP的另一个工具，section，代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 #include "opencv2/highgui/highgui.hpp"
 2 #include "opencv2/features2d/features2d.hpp"
 3 #include <iostream>
 4 #include <omp.h>
 5 int main( ){
 6     cv::SurfFeatureDetector detector( 400 );    cv::SurfDescriptorExtractor extractor;
 7     cv::BruteForceMatcher<cv::L2<float> > matcher;
 8     std::vector< cv::DMatch > matches;
 9     cv::Mat im0,im1;
10     std::vector<cv::KeyPoint> keypoints0,keypoints1;
11     cv::Mat descriptors0, descriptors1;
12     double t1 = omp_get_wtime( );
13 #pragma omp parallel sections
14     {
15 #pragma omp section
16         {
17             std::cout<<"processing im0"<<std::endl;
18             im0 = cv::imread("rgb0.jpg", CV_LOAD_IMAGE_GRAYSCALE );
19             detector.detect( im0, keypoints0);
20             extractor.compute( im0,keypoints0,descriptors0);
21             std::cout<<"find "<<keypoints0.size()<<"keypoints in im0"<<std::endl;
22         }
23 #pragma omp section
24         {
25             std::cout<<"processing im1"<<std::endl;
26             im1 = cv::imread("rgb1.jpg", CV_LOAD_IMAGE_GRAYSCALE );
27             detector.detect( im1, keypoints1);
28             extractor.compute( im1,keypoints1,descriptors1);
29             std::cout<<"find "<<keypoints1.size()<<"keypoints in im1"<<std::endl;
30         }
31     }
32     double t2 = omp_get_wtime( );
33     std::cout<<"time: "<<t2-t1<<std::endl;
34     matcher.match( descriptors0, descriptors1, matches );
35     cv::Mat img_matches;
36     cv::drawMatches( im0, keypoints0, im1, keypoints1, matches, img_matches ); 
37     cv::namedWindow("Matches",CV_WINDOW_AUTOSIZE);
38     cv::imshow( "Matches", img_matches );
39     cv::waitKey(0);
40     return 1;
41 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上面代码中，我们首先用#pragma omp parallel sections将要并行执行的内容括起来，在它里面，用了两个#pragma omp section，每个里面执行了图像读取、特征点与特征描述子提取。将其简化为伪代码形式即为：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 #pragma omp parallel sections
 2 {
 3     #pragma omp section
 4     {
 5         function1();
 6     }
 7 　　#pragma omp section
 8     {
 9         function2();
10     }
11 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
意思是：parallel sections里面的内容要并行执行，具体分工上，每个线程执行其中的一个section，如果section数大于线程数，那么就等某线程执行完它的section后，再继续执行剩下的section。在时间上，这种方式与人为用vector构造for循环的方式差不多，但无疑该种方式更方便，而且在单核机器上或没有开启openMP的编译器上，该种方式不需任何改动即可正确编译，并按照单核串行方式执行。
以上分享了这两天关于openMP的一点学习体会，其中难免有错误，欢迎指正。另外的一点疑问是，看到各种openMP教程里经常用到private,shared等来修饰变量，这些修饰符的意义和作用我大致明白，但在我上面所有例子中，不加这些修饰符似乎并不影响运行结果，不知道这里面有哪些讲究。
在写上文的过程中，参考了包括以下两个网址在内的多个地方的资源，不再一 一列出，在此一并表示感谢。
