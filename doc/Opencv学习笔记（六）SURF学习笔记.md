# Opencv学习笔记（六）SURF学习笔记

2012年03月25日 16:46:53

crzy_sparrow

阅读数：55727

更多

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									

原创文章，转载请注明出处：http://blog.csdn.net/crzy_sparrow/article/details/7392345

本人挺菜的，肯定有很多错误纰漏之处 ，希望大家不吝指正。



​     看了harris角点检测之后，开始研究SURF角点检测，发现挺复杂的，一时也只了解了大概，把了解的东西总结下，以便下次深入学习。

​     SURF角点检测算法是对SIFT的一种改进，主要体现在速度上，效率更高。它和SIFT的主要区别是图像多尺度空间的构建方法不同。

​    在计算视觉领域，尺度空间被象征性的表述为一个图像金字塔，向下降采样一般用高斯金字塔。其中，输入图像函数反复与高斯函数的核卷积并反复对其进行二次抽样，这种方法主要用于SIFT算法的实现，但每层图像依赖于原图像（当前尺度可能和原图象尺度相差很大，这时候再那原图像卷积确实有点二了），并且图像需要重设尺寸，因此，这种计算方法运算量较大。SURF算法对积分图像进行操作，卷积只和前一幅图像有关，其降采样的方法是申请增加图像核的尺寸，这也是SIFT算法与SURF算法在使用金字塔原理方面的不同。SURF算法允许尺度空间多层图像同时被处理，不需对图像进行二次抽样，从而提高算法性能。

​    其它方面的区别，以我菜鸟级的认识，差不大多。



​    附上两篇SIFT相关的博文，讲得还是比较透彻的，两篇结合起来看基本能明白其原理。

​    两篇讲的东西有点出入，不过对照着看还是能找出问题的，当然啃论文最容易找出问题了。

SIFT简介：http://www.360doc.com/content/11/1230/23/3054335_176200661.shtml

SIFT算法心得：http://www.360doc.com/content/11/1207/17/3054335_170430459.shtml

​    其他博文：

SIFT/SURF算法的深入剖析——谈SIFT的精妙与不足：http://hi.baidu.com/xiaoduo170/blog/item/a22bcc1c2349708286d6b636.html

SIFT/SURF系列：http://www.yongblog.com/archives/tag/surf%E7%AE%97%E6%B3%95



​    SURF论文和源码（论文讲得很详细，代码c++实现）：

http://download.csdn.net/detail/crzy_sparrow/4171374



opencv小试SURF算法：



```cpp
#include "opencv2/opencv.hpp"



 



int main(){



    cv::Mat  image, image1 = cv::imread ("test.jpg");



    //灰度变换



    cv::cvtColor (image1,image,CV_BGR2GRAY);



    std::vector<cv::KeyPoint> keypoints;



    cv::SurfFeatureDetector surf(2500);



    surf.detect (image,keypoints);



    cv::drawKeypoints (image,keypoints,image,cv::Scalar::all (255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);



 



    cv::namedWindow ("surf");



    cv::imshow ("surf",image);



    cv::waitKey (0);



    return 0;



}



 
```



测试结果

：



![img](https://img-my.csdn.net/uploads/201203/25/1332664436_1173.png)

标记圈的半径长短和特征点所在尺度有关，那条半径是特征点的方向。



    SIFT算法的教程、源码及应用软件

1、ubc:DAVID LOWE---SIFT算法的创始人，两篇巨经典

http://www.cs.ubc.ca/~lowe/ 


2、cmu:YanKe---PCASIFT,总结的SIFT方面的文章

http://www.andrew.cmu.edu/user/yke/ 


3、ubc:M.BROWN---SIFT算法用于图像拼接的经典应用autopano-sift，包括一个SIFTLIB库

http://www.cs.ubc.ca/~mbrown/autostitch/autostitch.html 

http://www.cs.ubc.ca/~mbrown/panorama/panorama.html 


4、toronto:Jepson---Matlab SIFT tutorial, 超级超级超级经典~


http://www.cs.toronto.edu/~jepson/csc2503/ 


5、ucla:Vedaldi---加州大学一个博士生编的Matlab SIFT tutorial

http://www.cs.ucla.edu/~vedaldi/


6.http://en.wikipedia.org/wiki/Scale-inva ... _transform


7. 大牛整理的计算机视觉分类


http://www.cs.ubc.ca/~lowe/vision.html


8. http://note.sonots.com/SciSoftware/SIFT.html


9.提到了计算变换矩阵的RANSAC法


http://web.engr.oregonstate.edu/~hess/index.html


10. 仿射不变特征点检测，提到了性能评价的方法


http://www.robots.ox.ac.uk/~vgg/research/affine/


11. 一个日本人，挺牛的


http://note.sonots.com/


12. PCA-SIFT


http://www.cs.cmu.edu/~yke/pcasift/


13 opencv sift


http://web.engr.oregonstate.edu/~hess/index.html


14 matlab sift


http://www.vlfeat.org/~vedaldi/code/sift.html


http://www.vlfeat.org/overview/sift.html


15 Improve Scale Invariant Feature Transform (SIFT) 斯坦福


http://robots.stanford.edu/cs223b04/project9.html


16 Known implementations of SIFT    mit


http://people.csail.mit.edu/albert/ladypack/wiki/index.php/Known_implementations_of_SIFT