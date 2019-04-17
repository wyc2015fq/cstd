# 图像局部特征（九）--斑点检测LOG算子 - 工作笔记 - CSDN博客





2016年06月26日 21:51:04[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8844
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

# 1. 什么是斑点

斑点通常是指与周围有着颜色和灰度差别的区域。在实际地图中，往往存在着大量这样的斑点，如一颗树是一个斑点，一块草地是一个斑点，一栋房子也可以是一个斑点。由于斑点代表的是一个区域，相比单纯的角点，它的稳定性要好，抗噪声能力要强，所以它在图像配准上扮演了很重要的角色。

同时有时图像中的斑点也是我们关心的区域，比如在医学与生物领域，我们需要从一些X光照片或细胞显微照片中提取一些具有特殊意义的斑点的位置或数量。

比如下图中天空的飞机、向日葵的花盘、X线断层图像中的两个斑点。

![image](http://images.cnitblog.com/blog/378920/201409/281359567014593.png)![image](http://images.cnitblog.com/blog/378920/201409/281400135765248.png)![image](http://images.cnitblog.com/blog/378920/201409/281400162177721.png)

在视觉领域，斑点检测的主要思路都是检测出图像中比它周围像素灰度值大或比周围灰度值小的区域。一般有两种方法来实现这一目标：
- 基于求导的微分方法，这类的方法称为微分检测器；
- 基于局部极值的分水岭算法。

这里我们重点介绍第一种方法，主要检测LOG斑点。而OpenCV中SimpleBlobDetector斑点检测算子就实现了第二种方法，我们这里也会介绍它的接口使用方法。

## 2. LOG斑点检测

## 2.1 基本原理

利用高斯拉普通拉斯（Laplace of Gaussian,LOG）算子检测图像斑点是一种十分常用的方法，对于二维高斯函数：



*G*(*x*,*y*;*σ*)=12*π**σ* 2  *e**x**p*(−*x* 2 +*y* 2 2*σ* 2  ) G(x,y;σ)=12πσ2exp(−x2+y22σ2)







它的拉普拉斯变换为:



∇ 2 *g*=∂ 2 *g*∂*x* 2  +∂ 2 *g*∂*y* 2   ∇2g=∂2g∂x2+∂2g∂y2







规范化的高斯拉普变换为：



∇ 2 *n**o**r**m* =*σ* 2 ∇ 2 *g*=*σ* 2 (∂ 2 *g*∂*x* 2  +∂ 2 *g*∂*y* 2  )=−12*π**σ* 2  [1−*x* 2 +*y* 2 *σ* 2  ]⋅*e**x**p*(−*x* 2 +*y* 2 2*σ* 2  ) ∇norm2=σ2∇2g=σ2(∂2g∂x2+∂2g∂y2)=−12πσ2[1−x2+y2σ2]⋅exp(−x2+y22σ2)







规范化算法子在二维图像上显示是一个圆对称函数，如下图所示。我们可以用这个算子来检测图像中的斑点，并且可以通过改变*σ* σ的值，可以检测不同尺寸的二维斑点。

![image](http://images.cnitblog.com/blog/378920/201409/281447242644638.png)![image](http://images.cnitblog.com/blog/378920/201409/281447267799839.png)

## 2.2 LOG原理解释

其实从更直观的角度去解释为什么LOG算子可以检测图像中的斑点是：

> 
图像与某一个二维函数进行卷积运算实际就是求取图像与这一函数的相似性。同理，图像与高斯拉普拉斯函数的卷积实际就是求取图像与高斯拉普拉斯函数的相似性。当图像中的斑点尺寸与高斯拉普拉斯函数的形状趋近一致时，图像的拉普拉斯响应达到最大。


从概率的角度解释为：假设原图像是一个与位置有关的随机变量X的密度函数，而LOG为随机变量Y的密度函数，则随机变量X+Y的密度分布函数即为两个函数的卷积形式(这一部分的理论，可以参见本博客概率与统计相关文章)。如果想让X+Y能取到最大值，则X与Y能保持步调一致最好，即X上升时，Y也上升，X最大时，Y也最大。

那么LOG算子是怎么被构想出来的呢？

事实上我们知道Laplace可以用来检测图像中的局部极值点，但是对噪声敏感，所以在我们对图像进行Laplace卷积之前，我们用一个高斯低通滤波对图像进行卷积，目标是去除图像中的噪声点。这一过程 可以描述为:

先对图像*f*(*x*,*y*) f(x,y)用方差为*σ* σ的高斯核进行高斯滤波，去除图像中的噪点。



*L*(*x*,*y*;*σ*)=*f*(*x*,*y*)∗*G*(*x*,*y*;*σ*) L(x,y;σ)=f(x,y)∗G(x,y;σ)







然后对图像的拉普拉斯图像则为：



∇ 2 =∂ 2 *L*∂*x* 2  +∂ 2 *L*∂*y* 2   ∇2=∂2L∂x2+∂2L∂y2







而实际上有下面等式：



∇ 2 [*G*(*x*,*y*)∗*f*(*x*,*y*)]=∇ 2 [*G*(*x*,*y*)]∗*f*(*x*,*y*) ∇2[G(x,y)∗f(x,y)]=∇2[G(x,y)]∗f(x,y)







所以，我们可以先求高斯核的拉普拉斯算子，再对图像进行卷积。也就是一开始描述的步骤。

## 2.3 LOG算子的实现

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
Mat Feat::getHOGKernel(Size& ksize, double sigma)
{
    Mat kernel(ksize, CV_64F);
    Point centPoint = Point((ksize.width -1)/2, ((ksize.height -1)/2));
    // first calculate Gaussian
    for (int i=0; i < kernel.rows; i++)
    {
        double* pData = kernel.ptr<double>(i);
        for (int j = 0; j < kernel.cols; j++)
        {
            double param = -((i - centPoint.y) * (i - centPoint.y) + (j - centPoint.x) * (j - centPoint.x)) / (2*sigma*sigma);
            pData[j] = exp(param);
        }
    }
    double maxValue;
    minMaxLoc(kernel, NULL, &maxValue);
    for (int i=0; i < kernel.rows; i++)
    {
        double* pData = kernel.ptr<double>(i);
        for (int j = 0; j < kernel.cols; j++)
        {
            if (pData[j] < EPS* maxValue)
            {
                pData[j] = 0;
            }
        }
    }

    double sumKernel = sum(kernel)[0];
    if (sumKernel != 0)
    {
        kernel = kernel / sumKernel;
    }
    // now calculate Laplacian
    for (int i=0; i < kernel.rows; i++)
    {
        double* pData = kernel.ptr<double>(i);
        for (int j = 0; j < kernel.cols; j++)
        {
            double addition = ((i - centPoint.y) * (i - centPoint.y) + (j - centPoint.x) * (j - centPoint.x) - 2*sigma*sigma)/(sigma*sigma*sigma*sigma);
            pData[j] *= addition;
        }
    }
    // make the filter sum to zero
    sumKernel = sum(kernel)[0];
    kernel -= (sumKernel/(ksize.width  * ksize.height));    

    return kernel;
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

## 2.4 多尺度检测

我们注意到当*σ* σ尺度一定时，只能检测对应半径的斑点，那么检测的是多大半径的斑点呢，我们可以通过对规范化的二维拉普拉斯高斯算子求导：

规范化的高斯拉普拉斯函数为：



∇ 2 *n**o**r**m* =−12*π**σ* 2  [1−*x* 2 +*y* 2 *σ* 2  ]⋅*e**x**p*(−*x* 2 +*y* 2 2*σ* 2  ) ∇norm2=−12πσ2[1−x2+y2σ2]⋅exp(−x2+y22σ2)







求∇ 2 *n**o**r**m*  ∇norm2的极点值等价于求取下式：



∂(∇ 2 *n**o**r**m* )∂*σ* =0 ∂(∇norm2)∂σ=0







得到：



(*x* 2 +*y* 2 −2*σ* 2 )⋅*e**x**p*(−(*x* 2 +*y* 2 )2*σ* 2  ) (x2+y2−2σ2)⋅exp(−(x2+y2)2σ2)









*r* 2 −2*σ* 2 =0 r2−2σ2=0







对于图像中的斑点，在尺度*σ*=*r*/2  √  σ=r/2时，高斯拉普拉斯响应值达到最大。同理，如果图像中的圆形斑点黑白反向，那么，它的高斯拉普拉斯响应值在*σ*=*r*/2  √  σ=r/2时达到最小。将高斯拉普拉斯响应达到峰值时的尺度*σ* σ值，称为特征尺度。

那么在多尺度的情况下，同时在空间和尺度上达到最大值（或最小值）的点就是我们所期望的斑点。对于二维图像*I*(*x*,*y*) I(x,y)，计算图像在不同尺度下的离散拉普拉斯响应值，然后检查位置空间中的每个点；如果该点的拉普拉斯响应值都大小于或小于其他26个立方空间领域(9+8+9)的值，那么该点就是被检测到的图像斑点。

![image](http://images.cnitblog.com/blog/378920/201409/281656048895166.png)

# 3 OpenCV进行斑点检测

opencv中检测Blobs的类为SimpleBlobDetector，这个类在opencv中的定义如下：

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
class SimpleBlobDetector : public FeatureDetector
{
public:
struct Params
{
    Params();
    float thresholdStep;
    float minThreshold;
    float maxThreshold;
    size_t minRepeatability;
    float minDistBetweenBlobs;

    bool filterByColor;
    uchar blobColor;

    bool filterByArea;
    float minArea, maxArea;

    bool filterByCircularity;
    float minCircularity, maxCircularity;

    bool filterByInertia;
    float minInertiaRatio, maxInertiaRatio;

    bool filterByConvexity;
    float minConvexity, maxConvexity;
};

SimpleBlobDetector(const SimpleBlobDetector::Params &parameters = SimpleBlobDetector::Params());

protected:
    ...
};
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

算法的大致步骤如下：
- 对[minThreshold,maxThreshold)区间，以thresholdStep为间隔，做多次二值化。
- 对每张二值图片，使用[findContours()](http://docs.opencv.org/master/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html#void%20findContours(InputOutputArray%20image,%20OutputArrayOfArrays%20contours,%20OutputArray%20hierarchy,%20int%20mode,%20int%20method,%20Point%20offset))提取连通域并计算每一个连通域的中心。
- 根据2得到的中心，全部放在一起。一些很接近的点［由theminDistBetweenBlobs控制多少才算接近］被归为一个group,对应一个bolb特征..
- 从3得到的那些点,估计最后的blob特征和相应半径，并以key points返回。

同时该支持提取特征的方法，一共有5个选项，这里就不多加描述了，默认是提取黑色圆形的Blob特征。下面是一个示例

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
int main(int argc, char** argv) 
{ 
    Mat image = imread(argv[1]); 
    vector<KeyPoint> keyPoints; 
    SimpleBlobDetector::Params params;

    SimpleBlobDetector blobDetect(params); 
    blobDetect.create("SimpleBlob"); 
    blobDetect.detect(image, keyPoints); 
    cout << keyPoints.size() << endl; 
    drawKeypoints(image, keyPoints, image, Scalar(255,0,0));

    namedWindow("blobs"); 
    imshow("blobs", image); 
    waitKey(); 
    return 0; 
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

![image](http://images.cnitblog.com/blog/378920/201409/281630535141199.png)![image](http://images.cnitblog.com/blog/378920/201409/281631146706329.png)

总体来说，OpenCV的斑点检测效果还算不错，但是在有些图像的效果上明显不如LOG算子检测的检测效果。

# 4. 扩展阅读

一个与LOG滤波核近似的是高斯差分DOG滤波核，它的定义为：



*D*(*x*,*y*,*σ*)=(*G*(*x*,*y*,*k**σ*)–*G*(*x*,*y*,*σ*))∗*I*(*x*,*y*)=*L*(*x*,*y*,*k**σ*)−*L*(*x*,*y*,*σ*) D(x,y,σ)=(G(x,y,kσ)–G(x,y,σ))∗I(x,y)=L(x,y,kσ)−L(x,y,σ)







其中*k* k为两个相邻尺度间的比例因子。

DOG可以看作为LOG的一个近似，但是它比LOG的效率更高。

![image](http://images.cnitblog.com/blog/378920/201409/281631178571903.png)

前面介绍的微分算子在近圆的斑点检测方面效果很好，但是这些检测算子被限定于只能检测圆形斑点，而且不能估计斑点的方向，因为LOG算子等都是中心对称的。如果我们定义一种二维高斯核的变形，记它在X方向与Y方向上具有不同的方差，则这种算子可以用来检测带有方向的斑点。



*G*(*x*,*y*)=A⋅*e**x**p*(−[(*a**x* 2 +2*b**x**y*+*c**y* 2 )]) G(x,y)=A⋅exp(−[(ax2+2bxy+cy2)])









*a*=*c**o**s* 2 *θ*2*σ* 2 *x*  +*s**i**n* 2 *θ*2*σ* 2 *y*  ,*b*=−*s**i**n*2*θ*2*σ* 2 *x*  +*s**i**n*2*θ*4*σ* 2 *y*  ,*c*=*s**i**n* 2 *θ*2*σ* 2 *x*  +*c**o**s* 2 *θ*2*σ* 2 *y*   a=cos2θ2σx2+sin2θ2σy2,b=−sin2θ2σx2+sin2θ4σy2,c=sin2θ2σx2+cos2θ2σy2







其中A A是规一性因子。

# 5. 参考资料

1. 《现代数字图像 -- 处理技术提高与应用案例详解》

2. 《图像局部不变性特征与描述》

3.  Lindeberg, T. Feature Detection with Automatic Scale Selection

4. Hui Kong. A Generalized Laplacian Of Gaussian Filter for Blob Detection and Its Applications.

5. [OpenCV2马拉松第20圈——blob特征检测原理与实现](http://blog.csdn.net/abcd1992719g/article/details/27071273)

﻿﻿



