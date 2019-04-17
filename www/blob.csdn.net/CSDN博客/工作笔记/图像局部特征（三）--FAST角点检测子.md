# 图像局部特征（三）--FAST角点检测子 - 工作笔记 - CSDN博客





2016年06月25日 20:50:28[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7489
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









# Features From Accelerated Segment Test

# 1. FAST算法原理

      博客中已经介绍了很多图像特征检测算子，我们可以用LoG或者DoG检测图像中的Blobs（[斑点检测](http://www.cnblogs.com/ronny/p/3895883.html)），可以根据图像局部的自相关函数来求得Harris角点（[Harris角点](http://www.cnblogs.com/ronny/p/4009425.html)），后面又提到了两种十分优秀的特征点及它们的描述方法[SIFT特征](http://www.cnblogs.com/ronny/p/4028776.html)与[SURF特征](http://www.cnblogs.com/ronny/p/4045979.html)。SURF特征算是为了提高运算效率对SIFT特征的一种近似，虽然在有些实验环境中已经达到了实时，但是我们实践工程应用中，特征点的提取与匹配只是整个应用算法中的一部分，所以我们对于特征点的提取必须有更高的要求，从这一点来看前面介绍的的那些特征点方法都不可取。

      为了解决这个问题，Edward Rosten和Tom Drummond在2006年发表的“Machine learning for high-speed corner detection[1]”文章中提出了一种FAST特征，并在2010年对这篇论文作了小幅度的修改后重新发表[2]。FAST的全称为Features From Accelerated Segment Test。Rosten等人将FAST角点定义为：若某像素点与其周围领域内足够多的像素点处于不同的区域，则该像素点可能为角点。也就是某些属性与众不同，考虑灰度图像，即若该点的灰度值比其周围领域内足够多的像素点的灰度值大或者小，则该点可能为角点。

# 2. FAST算法步骤


- 从图片中选取一个像素*P* P，下面我们将判断它是否是一个特征点。我们首先把它的亮度值设为*I**p*  Ip。
- 设定一个合适的阈值*t* t。
- 考虑以该像素点为中心的一个半径等于3像素的离散化的Bresenham圆，这个圆的边界上有16个像素（如图1所示）。
	


图1 FAST特征点示意图

- 
现在，如果在这个大小为16个像素的圆上有*n* n个连续的像素点，它们的像素值要么都比*I**p* +*t* Ip+t大，要么都比*I**p* −*t* Ip−t小，那么它就是一个角点。（如图1中的白色虚线所示）。*n* n的值可以设置为12或者9，实验证明选择9可能会有更好的效果。




     上面的算法中，对于图像中的每一个点，我们都要去遍历其邻域圆上的16个点的像素，效率较低。我们下面提出了一种高效的测试（high-speed test）来快速排除一大部分非角点的像素。该方法仅仅检查在位置1，9，5和13四个位置的像素，首先检测位置1和位置9，如果它们都比阈值暗或比阈值亮，再检测位置5和位置13。如果*P* P是一个角点，那么上述四个像素点中至少有3个应该必须都大于*I**p* +*t* Ip+t或者小于*I**p* −*t* Ip−t，因为若是一个角点，超过四分之三圆的部分应该满足判断条件。如果不满足，那么*p* p不可能是一个角点。对于所有点做上面这一部分初步的检测后，符合条件的将成为候选的角点，我们再对候选的角点，做完整的测试，即检测圆上的所有点。



上面的算法效率实际上是很高的，但是有点一些缺点：
- 
当我们设置*n*<12 n<12时就不能使用快速算法来过滤非角点的点；

- 
检测出来的角点不是最优的，这是因为它的效率取决于问题的排序与角点的分布；

- 
对于角点分析的结果被丢弃了；

- 
多个特征点容易挤在一起。


# 3. 使用机器学习做一个角点分类器


- 首先选取你进行角点提取的应用场景下很多张的测试图片。
- 运行FAST角点检测算法来获取测试图片集上的所有角点特征。
- 对于每个角点，我们把它邻域圆上的16个点存储下来保存在一个vector内，处理所有步骤2中得到的角点，并把它们存储在*P* P中。
- 对于图像上的点*p* p,它周围邻域圆上位置为*x*,*x*∈{1…16} x,x∈{1…16}的点表示为*p*→*x* p→x，可以用下面的判断公式将该点*p*→*x* p→x分为3类：
	
*S**p*→*x* =⎧ ⎩ ⎨ ⎪ ⎪ *d*,*s*,*b*, *I**p*→*x* ≤*I**p* –*t**I**p* −*t*≤*I**p*→*x* <*I**p* +*t**I**p* +*t*≤*I**p*→*x*  (*d**a**r**k**e**r*)(*s**i**m**i**l**a**r*)(*b**r**i**g**h**t**e**r*)  Sp→x={d,Ip→x≤Ip–t(darker)s,Ip−t≤Ip→x<Ip+t(similar)b,Ip+t≤Ip→x(brighter)

- 设*P* P为训练图像集中所有像素点的集合，我们任意16个位置中的一个位置*x* x，可以把集合*P* P分为三个部分*P**d* ,*P**s*  Pd,Ps和*P**b*  Pb，其中*P**d*  Pd的定义如下，*P**s*  Ps和*P**b*  Pb的定义与其类似
	
*P**b* ={*p*∈*P*:*S**p*→*x* =*b*} Pb={p∈P:Sp→x=b}

	换句话说，对于任意给定的位置*x* x，它都可以把所有图像中的点分为三类，第一类*P**d*  Pd包括了所有位置*x* x处的像素在阈值*t* t下暗于中心像素，第二类*P**s*  Ps包括了所有位置*x* x处的像素在阈值*t* t下近似于中心像素，*P**b*  Pb包括了所有位置*x* x处的像素在阈值*t* t下亮于中心像素。
- 定义一个新的布尔变量*K**p*  Kp，如果*p* p是一个角点，那些*K**p*  Kp为值，否则为假。
- 使用ID3算法（决策树分类器）来查询每一个子集。
- 递归计算所有的子集直到*K**p*  Kp的熵为0;
- 被创建的决策树就用于于其他图片的FAST检测。

# 4. 非极大值抑制

从邻近的位置选取了多个特征点是另一个问题，我们可以使用Non-Maximal Suppression来解决。
- 为每一个检测到的特征点计算它的响应大小（score function）*V* V。这里*V* V定义为点*p* p和它周围16个像素点的绝对偏差的和。
- 考虑两个相邻的特征点，并比较它们的*V* V值。
- *V* V值较低的点将会被删除。

# 5. OpenCV中进行FAST特征检测

在OpenCV中进行FAST特征提取的函数为FAST。它一共有4个参数，第一个参数是输入的图像，第二个是返回的特征点，第三个是定义的阈值，第四个决定是否使用非极大值抑制。

```
void FAST(InputArray image, vector<KeyPoint>& keypoints, int threshold, bool nonmaxSuppression=true )

C++: void FASTX(InputArray image, vector<KeyPoint>& keypoints, int threshold, bool nonmaxSuppression, int type)
```

另外还有一个接口为FASTX，它提供了第五个参数type用来指定FAST检测中像素邻域圆的参数：TYPE_9_16、TYPE_7_12、TYPE_5_8。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>

using namespace cv; 
using namespace std;

int main(int argc, char** argv) 
{ 
    Mat img = imread("box.png"); 
    std::vector<KeyPoint> keypoints; 
    FAST(img, keypoints, 20); 
    //-- Draw keypoints 
    Mat img_keypoints; 
    drawKeypoints(img, keypoints, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT); 
    //-- Show detected (drawn) keypoints 
    imshow("Keypoints", img_keypoints);

    waitKey(0); 
    return 0; 
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

![image](http://images.cnitblog.com/blog/378920/201411/061539341746935.png)![image](http://images.cnitblog.com/blog/378920/201411/061539410958987.png)

# 6. 总结

FAST算法比其他已知的角点检测算法要快很多倍，但是当图片中的噪点较多时，它的健壮性并不好，而且算法的效果还依赖于一个阈值*t* t。而且FAST不产生多尺度特征而且FAST特征点没有方向信息，这样就会失去旋转不变性。

[1] Edward Rosten and Tom Drummond, “Machine learning for high speed corner detection” in 9th European Conference on Computer Vision, vol. 1, 2006, pp. 430–443.

[2] Edward Rosten, Reid Porter, and Tom Drummond, “Faster and better: a machine learning approach to corner detection” in IEEE Trans. Pattern Analysis and Machine Intelligence, 2010, vol 32, pp. 105-119.

﻿﻿



