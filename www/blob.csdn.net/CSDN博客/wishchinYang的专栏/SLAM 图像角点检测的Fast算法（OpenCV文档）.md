# SLAM: 图像角点检测的Fast算法（OpenCV文档） - wishchinYang的专栏 - CSDN博客
2015年08月25日 15:13:25[wishchin](https://me.csdn.net/wishchin)阅读数：2824
       官方链接：[http://docs.opencv.org/trunk/doc/py_tutorials/py_feature2d/py_fast/py_fast.html#fast-algorithm-for-corner-detection](http://docs.opencv.org/trunk/doc/py_tutorials/py_feature2d/py_fast/py_fast.html#fast-algorithm-for-corner-detection)
       原文链接：[http://blog.csdn.net/candycat1992/article/details/22285979](http://blog.csdn.net/candycat1992/article/details/22285979)
       本文做了少量修改，并添加了C++代码，若有疑问或者版权问题，请拜读作者或者通知本人。
# 目标
- 理解FAST算法的基本原理
- 使用OpenCV的FAST函数进行角点（corners）检测
# 原理
我们已知很多种特征检测的方法，而且它们其中很多效果都非常不错。但是，当从一个实时运行的程序角度出发，它们还不够快。一个最好的例子就是SLAM（**Simultaneous Localization and Mapping**）移动机器人，它的可计算资源是受限的。
作为上述问题的一个解决方法，FAST（Features from Accelerated Segment Test）算法由Edward Rosten 和 Tom Drummond在他们2006年的论文[“Machine
 learning for high-speed corner detection”](http://dl.acm.org/citation.cfm?id=2094478)（在2010年再次被[修订](http://dl.acm.org/citation.cfm?id=1687114)）中被提出。下面是该算法的一个基本总结。你可以参见原论文去学习更多的细节（下面所有的图片是从原论文中提取的）。
# 使用FAST进行特征检测
- 从图片中选取一个像素点Pi，下面我们将判断它是否是一个特征点。我们首先把它的密度（即像素值）设为I_p。
- 设定一个合适的阙值T。
- 考虑该像素点周围的16个像素。（见下图）
![](https://img-blog.csdn.net/20140327144149718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FuZHljYXQxOTky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 现在，如果在这个大小为16个像素的圆上有n 个连续的像素点，它们的像素值要么都比I_p+T大，要么都比I_p-T小，那么它就是一个角点。（如上图中白色虚线所示）。n 这里被设定为12。
- 我们现在应该提出一个高效的测试，来快速排除一大部分是非角点的点。该测试仅仅检查在位置1、9、5和13四个位置的像素（首先检查1和9，看它们是否亮于或暗于阙值。如果是，再检查5和13）。如果Pi是一个角点，那么上述四个像素点中至少有3个应该必须都大于I_p +T 或者小于I_p-
 T（因为若是一个角点，超过四分之三个圆的部分应该满足判断条件，半圆比包含上述某三个点）。如果都不满足，那么Pi 不可能是一个角点。完整的分段测试可以被用于接受的所有候选点，通过检测圆上的所有点。这种检测有很好的性能，但是有一些缺点：
1. 当n < 12时不能拒绝许多候选点。
2. 检测出来的角点不是最优的，这是因为它的效率是依靠角点外形的排列和分布的。
3. 
4. **相邻的多个特征点会被检测到**。
前三个问题可以使用机器学习的方法解决。最后一个可以使用**non-maximal suppression**。

# 机器学习
- 选择一个图片集合进行学习（最好是来自于目标应用定义域）。
- 在每一张图上运行FAST算法，找到特征点。
- 对于每个特征点，存储它周围的16个像素点到一个vector中。为所有的图片做同样的事，得到它们所有特征点的vectorP。
- 这16个像素中的每一个像素（假设为x），可以有下面三种状态中的一种：
![](https://img-blog.csdn.net/20140327150736890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FuZHljYXQxOTky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 依靠这些状态，特征向量P被划分为3个子集，P_d,P_s,P_b。
- 定义一个新的布尔变量，K_p。如果P是一个角点，那些K_p为真；否则为假。
- 使用ID3算法（决策树分类器）来查询每一个子集。
- 递归计算所有的子集直到它的嫡为0。
- 被创建的决策树就被用于其他图片的fast检测。
# non-maximal suppression
从邻近的位置选取了多个特征点是另一个问题。我们可以使用non-maximal
 suppression来解决。
- 为每一个检测到的特征点计算它的分数函数（score function），V。V是
 P 和它周围16个像素点的绝对偏差的和。
- 考虑两个相邻的特征点，并比较它们的v值。
- v 值较低的点将会被剔除。
# 总结
FAST算法比其他已知的角点检测法要快很多倍。
但是当图片的噪点较多时，它的健壮性并不好。这依靠一个阙值。
# OpenCV中的FAST特征检测
和OpenCV中其他特征检测器的调用相同。如果你想，你可以指定一个阙值，或者决定是否使用non-maximal suppression来判断邻近特征点。
对于相邻特征点，OpenCV定义了三个flags：
- cv2.FAST_FEATURE_DETECTOR_TYPE_5_8
- cv2.FAST_FEATURE_DETECTOR_TYPE_7_12
- cv2.FAST_FEATURE_DETECTOR_TYPE_9_16
下面是一个简单的示例代码（Python）。
```python
import numpy as np  
    import cv2  
    from matplotlib import pyplot as plt  
      
    img = cv2.imread('simple.jpg',0)  
      
    # Initiate FAST object with default values  
    fast = cv2.FastFeatureDetector()  
      
    # find and draw the keypoints  
    kp = fast.detect(img,None)  
    img2 = cv2.drawKeypoints(img, kp, color=(255,0,0))  
      
    # Print all default params  
    print "Threshold: ", fast.getInt('threshold')  
    print "nonmaxSuppression: ", fast.getBool('nonmaxSuppression')  
    print "neighborhood: ", fast.getInt('type')  
    print "Total Keypoints with nonmaxSuppression: ", len(kp)  
      
    cv2.imwrite('fast_true.png',img2)  
      
    # Disable nonmaxSuppression  
    fast.setBool('nonmaxSuppression',0)  
    kp = fast.detect(img,None)  
      
    print "Total Keypoints without nonmaxSuppression: ", len(kp)  
      
    img3 = cv2.drawKeypoints(img, kp, color=(255,0,0))  
      
    cv2.imwrite('fast_false.png',img3)
```
C++代码段：
```cpp
int CEkfSlam::fast_corner_detect_9(
	cv::Mat &im,  double  threshold, std::vector<CvPoint>  &coords)	
{
	//提取快速角点//一次只提取一个角点？//且有时可能会不发现角点，可能一次有多个角点
	cv::Mat  image(im.rows, im.cols, 1);
	if (3== im.channels() ){
		cv::cvtColor (im,image,CV_BGR2GRAY);  
	}
	else{
		image =im.clone();
	}
	//快速角点检测  
	std::vector<cv::KeyPoint>  keypoints;  
	cv::FAST(image,keypoints,threshold,true);  //默认开启极大值抑制！
	coords.resize(keypoints.size() );
	//cv::imshow("kp",image);cv::waitKey(0);
	for (int i=0;i< keypoints.size();++i){
		coords[i].x =keypoints[i].pt.x;
		coords[i].y =keypoints[i].pt.y;
	}
	return 1;
}
```
值得注意的是，下面的写法会出现内存问题：
```cpp
cv::FastFeatureDetector fast(threshold, true);
fast.detect(image, allKeypoints[0]);</del>
```
下面是测试结果。左边是使用了nonmaxSuppression的FAST，右边则没有使用。
![](https://img-blog.csdn.net/20140327152717625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FuZHljYXQxOTky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

