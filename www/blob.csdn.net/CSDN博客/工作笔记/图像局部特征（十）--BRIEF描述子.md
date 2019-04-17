# 图像局部特征（十）--BRIEF描述子 - 工作笔记 - CSDN博客





2016年06月26日 22:21:23[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6957
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

# 原文：

[http://www.cnblogs.com/ronny/p/4081362.html](http://www.cnblogs.com/ronny/p/4081362.html)

# Binary Robust Independent Elementary Features

[www.cnblogs.com/ronny](http://www.cnblogs.com/ronny)

# 1. BRIEF的基本原理

我们已经知道SIFT特征采用了128维的特征描述子，由于描述子用的浮点数，所以它将会占用512 bytes的空间。类似地，对于SURF特征，常见的是64维的描述子，它也将占用256bytes的空间。如果一幅图像中有1000个特征点（不要惊讶，这是很正常的事），那么SIFT或SURF特征描述子将占用大量的内存空间，对于那些资源紧张的应用，尤其是嵌入式的应用，这样的特征描述子显然是不可行的。而且，越占有越大的空间，意味着越长的匹配时间。



但是实际上SFIT或SURF的特征描述子中，并不是所有维都在匹配中有着实质性的作用。我们可以用PCA、LDA等特征降维的方法来压缩特征描述子的维度。还有一些算法，例如LSH，将SIFT的特征描述子转换为一个二值的码串，然后这个码串用汉明距离进行特征点之间的匹配。这种方法将大大提高特征之间的匹配，因为汉明距离的计算可以用异或操作然后计算二进制位数来实现，在现代计算机结构中很方便。下面来们提取一种二值码串的特征描述子。



BRIEF[1]应运而生，它提供了一种计算二值串的捷径，而并不需要去计算一个类似于SIFT的特征描述子。它需要先平滑图像，然后在特征点周围选择一个Patch，在这个Patch内通过一种选定的方法来挑选出来*n**d*  nd个点对。然后对于每一个点对(*p*,*q*) (p,q)，我们来比较这两个点的亮度值，如果*I*(*p*)>*I*(*q*) I(p)>I(q)则这个点对生成了二值串中一个的值为1，如果*I*(*p*)<*I*(*q*) I(p)<I(q)，则对应在二值串中的值为-1，否则为0。所有*n**d*  nd个点对，都进行比较之间，我们就生成了一个*n**d*  nd长的二进制串。



对于*n**d*  nd的选择，我们可以设置为128，256或512，这三种参数在OpenCV中都有提供，但是OpenCV中默认的参数是256，这种情况下，非匹配点的汉明距离呈现均值为128比特征的高斯分布。一旦维数选定了，我们就可以用汉明距离来匹配这些描述子了。

值得注意的是，对于BRIEF，它仅仅是一种特征描述符，它不提供提取特征点的方法。所以，如果你必须使一种特征点定位的方法，如FAST、SIFT、SURF等。这里，我们将使用CenSurE方法来提取关键点，对BRIEF来说，CenSurE的表现比SURF特征点稍好一些。

CenSurE特征检测子由三个步骤组成。首先利用积分图像计算双层高斯拉普拉斯滤波器响应，其次采用非极大值抑制检测局部极大值，最后滤除分布在边缘或线上的不稳定特征点。Agrawal等人提出的CenSurE特征检测子采用简单的双层滤波器来近似高斯拉普拉斯。

设CenSurE滤波器的内核尺寸为（2n+1）×（2n+1），外核尺寸为（4n+1）×（4n+1）。设In为内核权重系数，On为外核权重系数。为了使这种滤波器的DC响应为0，权重系数需满足下列等式：

On（4n+1）^2=In（2n+1）^2

（1）对尺度进行归一化：

In（2n+1）^2=In+1（2（n+1）+1）^2

（2）CenSurE采用线性尺度，即n=1，2，3，…，如图1所示。图1近似的双层LOG滤波器每个尺度的滤波器在图像的每个像素上计算。将尺度空间的每个点与本尺度的相邻位置以及相邻尺度的对应位置共26个邻域进行逐个比较，通过非极大值抑制得到的局部极值点。弱的响应被滤除，因为这些特征是不稳定的。

最后，用尺度适应的Harris方法计算主曲率的比率以去除位于边缘或线上的不稳定特征点。

总体来说，BRIEF是一个效率很高的提取特征描述子的方法，同时，它有着很好的识别率，但当图像发生很大的平面内的旋转。

# 2. 关于点对的选择

设我们在特征点的邻域块大小为*S*×*S* S×S内选择*n**d*  nd个点对(*p*,*q*) (p,q)，Calonder的实验中测试了5种采样方法：

1）在图像块内平均采样；

2）*p* p和*q* q都符合(0,125 *S* 2 ) (0,125S2)的高斯分布；

3）*p* p符合(0,125 *S* 2 ) (0,125S2)的高斯分布，而*q* q符合(0,1100 *S* 2 ) (0,1100S2)的高斯分布；

4）在空间量化极坐标下的离散位置随机采样

5）把*p* p固定为(0,0) (0,0)，*q* q在周围平均采样

下面是上面5种采样方法的结果示意图。

![image](http://images.cnitblog.com/blog/378920/201411/071515564565603.png)![image](http://images.cnitblog.com/blog/378920/201411/071516017849941.png)

# 2. OpenCV实现BRIEF

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>

using namespace cv;

int main(int argc, char** argv) 
{ 
    Mat img_1 = imread("box.png"); 
    Mat img_2 = imread("box_in_scene.png");

    // -- Step 1: Detect the keypoints using STAR Detector 
    std::vector<KeyPoint> keypoints_1,keypoints_2; 
    StarDetector detector; 
    detector.detect(img_1, keypoints_1); 
    detector.detect(img_2, keypoints_2);

    // -- Stpe 2: Calculate descriptors (feature vectors) 
    BriefDescriptorExtractor brief; 
    Mat descriptors_1, descriptors_2; 
    brief.compute(img_1, keypoints_1, descriptors_1); 
    brief.compute(img_2, keypoints_2, descriptors_2);

    //-- Step 3: Matching descriptor vectors with a brute force matcher 
    BFMatcher matcher(NORM_HAMMING); 
    std::vector<DMatch> mathces; 
    matcher.match(descriptors_1, descriptors_2, mathces); 
    // -- dwaw matches 
    Mat img_mathes; 
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, mathces, img_mathes); 
    // -- show 
    imshow("Mathces", img_mathes);

    waitKey(0); 
    return 0; 
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

![WAZNVG5NFSKT3D@1L5H)1M5](http://images.cnitblog.com/blog/378920/201411/071544089405674.jpg)



[1] Michael Calonder, Vincent Lepetit, Christoph Strecha, and Pascal Fua, “BRIEF: Binary Robust Independent Elementary Features”, 11th European Conference on Computer Vision (ECCV), Heraklion, Crete. LNCS Springer, September 2010.



