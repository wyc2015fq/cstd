# 图像局部特征（二）--Harris角点检测子 - 工作笔记 - CSDN博客





2016年06月25日 16:26:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：11593
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

一、角点定义

有定义角点的几段话：

1、角点检测(Corner Detection)是计算机视觉系统中用来获得图像特征的一种方法，广泛应用于运动检测、图像匹配、视频跟踪、三维建模和目标识别等领域中。也称为特征点检测。

角点通常被定义为两条边的交点，更严格的说，角点的局部邻域应该具有两个不同区域的不同方向的边界。而实际应用中，大多数所谓的角点检测方法检测的是拥有特定特征的图像点，而不仅仅是“角点”。这些特征点在图像中有具体的坐标，并具有某些数学特征，如局部最大或最小灰度、某些梯度特征等。

现有的角点检测算法并不是都十分的鲁棒。很多方法都要求有大量的训练集和冗余数据来防止或减少错误特征的出现。角点检测方法的一个很重要的评价标准是其对多幅图像中相同或相似特征的检测能力，并且能够应对光照变化、图像旋转等图像变化。

2、在我们解决问题时，往往希望找到特征点，“特征”顾名思义，指能描述物体本质的东西，还有一种解释就是这个特征微小的变化都会对物体的某一属性产生重大的影响。而角点就是这样的特征。

观察日常生活中的“角落”就会发现，“角落”可以视为所有平面的交汇处，或者说是所有表面的发起处。假设我们要改变一个墙角的位置，那么由它而出发的平面势必都要有很大的变化。所以，这就引出了图像角点的定义

“如果某一点在任意方向的一个微小变动都会引起灰度很大的变化，那么我们就把它称之为角点”

3.特征检测与匹配是Computer Vision 应用总重要的一部分，这需要寻找图像之间的特征建立对应关系。点，也就是图像中的特殊位置，是很常用的一类特征，点的局部特征也可以叫做“关键特征点”(**keypoint feature**)，或“兴趣点”(**interest point**)，或“角点”(**conrner**)。

关于角点的具体描述可以有几种：
- 一阶导数(即灰度的梯度)的局部最大所对应的像素点；
- 两条及两条以上边缘的交点；
- 图像中梯度值和梯度方向的变化速率都很高的点；
- 角点处的一阶导数最大，二阶导数为零，指示物体边缘变化不连续的方向。

原文：

[http://www.cnblogs.com/ronny/p/4009425.html](http://www.cnblogs.com/ronny/p/4009425.html)

[Harris角点](http://www.cnblogs.com/ronny/p/4009425.html)

# 1. 不同类型的角点

在现实世界中，角点对应于物体的拐角，道路的十字路口、丁字路口等。从图像分析的角度来定义角点可以有以下两种定义：
- 角点可以是两个边缘的角点；
- 角点是邻域内具有两个主方向的特征点；

前者往往需要对图像边缘进行编码，这在很大程度上依赖于图像的分割与边缘提取，具有相当大的难度和计算量，且一旦待检测目标局部发生变化，很可能导致操作的失败。早期主要有Rosenfeld和Freeman等人的方法，后期有CSS等方法。

基于图像灰度的方法通过计算点的曲率及梯度来检测角点，避免了第一类方法存在的缺陷，此类方法主要有Moravec算子、Forstner算子、Harris算子、SUSAN算子等。

![image](http://images.cnitblog.com/blog/378920/201410/071544042804359.png)

这篇文章主要介绍的Harris角点检测的算法原理，比较著名的角点检测方法还有jianbo Shi和Carlo Tomasi提出的Shi-Tomasi算法，这个算法开始主要是为了解决跟踪问题，用来衡量两幅图像的相似度，我们也可以把它看为Harris算法的改进。OpenCV中已经对它进行了实现，接口函数名为[GoodFeaturesToTrack()](http://www.opencv.org.cn/opencvdoc/2.3.2/html/modules/imgproc/doc/feature_detection.html#goodfeaturestotrack)。另外还有一个著名的角点检测算子即SUSAN算子，SUSAN是Smallest Univalue Segment Assimilating Nucleus（最小核值相似区）的缩写。SUSAN使用一个圆形模板和一个圆的中心点，通过圆中心点像素与模板圆内其他像素值的比较，统计出与圆中心像素近似的像元数量，当这样的像元数量小于某一个阈值时，就被认为是要检测的角点。我觉得可以把SUSAN算子看为Harris算法的一个简化。这个算法原理非常简单，算法效率也高，所以在OpenCV中，它的接口函数名称为：[FAST()](http://docs.opencv.org/modules/features2d/doc/feature_detection_and_description.html#fast) 。

# 2. Harris角点

## 2.1 基本原理

人眼对角点的识别通常是在一个局部的小区域或小窗口完成的。如果在各个方向上移动这个特征的小窗口，窗口内区域的灰度发生了较大的变化，那么就认为在窗口内遇到了角点。如果这个特定的窗口在图像各个方向上移动时，窗口内图像的灰度没有发生变化，那么窗口内就不存在角点；如果窗口在某一个方向移动时，窗口内图像的灰度发生了较大的变化，而在另一些方向上没有发生变化，那么，窗口内的图像可能就是一条直线的线段。

![image](http://images.cnitblog.com/blog/378920/201410/081646497655888.png)

对于图像*I*(*x*,*y*) I(x,y)，当在点(*x*,*y*) (x,y)处平移(Δ*x*,Δ*y*) (Δx,Δy)后的自相似性，可以通过自相关函数给出：



*c*(*x*,*y*;Δ*x*,Δ*y*)=∑ (*u*,*v*)∈*W*(*x*,*y*) *w*(*u*,*v*)(*I*(*u*,*v*)–*I*(*u*+Δ*x*,*v*+Δ*y*)) 2  c(x,y;Δx,Δy)=∑(u,v)∈W(x,y)w(u,v)(I(u,v)–I(u+Δx,v+Δy))2





其中，*W*(*x*,*y*) W(x,y)是以点(*x*,*y*) (x,y)为中心的窗口，*w*(*u*,*v*) w(u,v)为加权函数，它既可是常数，也可以是高斯加权函数。

![image](http://images.cnitblog.com/blog/378920/201410/081646516874975.png)

根据泰勒展开，对图像*I*(*x*,*y*) I(x,y)在平移(Δ*x*,Δ*y*) (Δx,Δy)后进行一阶近似：



*I*(*u*+Δ*x*,*v*+Δ*y*)=*I*(*u*,*v*)+*I**x* (*u*,*v*)Δ*x*+*I**y* (*u*,*v*)Δ*y*+*O*(Δ*x* 2 ,Δ*y* 2 )≈*I*(*u*,*v*)+*I**x* (*u*,*v*)Δ*x*+*I**y* (*u*,*v*)Δ*y* I(u+Δx,v+Δy)=I(u,v)+Ix(u,v)Δx+Iy(u,v)Δy+O(Δx2,Δy2)≈I(u,v)+Ix(u,v)Δx+Iy(u,v)Δy





其中，*I**x* ,*I**y*  Ix,Iy是图像*I*(*x*,*y*) I(x,y)的偏导数，这样的话，自相关函数则可以简化为：



*c*(*x*,*y*;Δ*x*,Δ*y*)≈∑ *w* (*I**x* (*u*,*v*)Δ*x*+*I**y* (*u*,*v*)Δ*y*) 2 =[Δ*x*,Δ*y*]*M*(*x*,*y*)[Δ*x*Δ*y* ] c(x,y;Δx,Δy)≈∑w(Ix(u,v)Δx+Iy(u,v)Δy)2=[Δx,Δy]M(x,y)[ΔxΔy]





其中



*M*(*x*,*y*)=∑ *w* [*I**x* (*x*,*y*) 2 *I**x* (*x*,*y*)*I**y* (*x*,*y*) *I**x* (*x*,*y*)*I**y* (*x*,*y*)*I**y* (*x*,*y*) 2  ]=[∑ *w**I**x* (*x*,*y*) 2 ∑ *w**I**x* (*x*,*y*)*I**y* (*x*,*y*) ∑ *w**I**x* (*x*,*y*)*I**y* (*x*,*y*)∑ *w**I**y* (*x*,*y*) 2  ]=[*A**C**C**B* ] M(x,y)=∑w[Ix(x,y)2Ix(x,y)Iy(x,y)Ix(x,y)Iy(x,y)Iy(x,y)2]=[∑wIx(x,y)2∑wIx(x,y)Iy(x,y)∑wIx(x,y)Iy(x,y)∑wIy(x,y)2]=[ACCB]





也就是说图像*I*(*x*,*y*) I(x,y)在点(*x*,*y*) (x,y)处平移(Δ*x*,Δ*y*) (Δx,Δy)后的自相关函数可以近似为二项函数：



*c*(*x*,*y*;Δ*x*,Δ*y*)≈*A*Δ*x* 2 +2*C*Δ*x*Δ*y*+*B*Δ*y* 2  c(x,y;Δx,Δy)≈AΔx2+2CΔxΔy+BΔy2





其中



*A*=∑ *w**I* 2 *x* ,*B*=∑ *w**I* 2 *y* ,*C*=∑ *w**I**x**I**y*  A=∑wIx2,B=∑wIy2,C=∑wIxIy





二次项函数本质上就是一个椭圆函数。椭圆的扁率和尺寸是由*M*(*x*,*y*) M(x,y)的特征值*λ* 1 、*λ* 2  λ1、λ2决定的，椭贺的方向是由*M*(*x*,*y*) M(x,y)的特征矢量决定的，如下图所示，椭圆方程为：



[Δ*x*,Δ*y*]*M*(*x*,*y*)[Δ*x*Δ*y* ]=1 [Δx,Δy]M(x,y)[ΔxΔy]=1





![image](http://images.cnitblog.com/blog/378920/201410/081646524376603.png)

椭圆函数特征值与图像中的角点、直线（边缘）和平面之间的关系如下图所示。共可分为三种情况：
- 图像中的直线。一个特征值大，另一个特征值小，*λ* 1 ≫*λ* 2  λ1≫λ2或*λ* 2 ≫*λ* 1  λ2≫λ1。自相关函数值在某一方向上大，在其他方向上小。
- 图像中的平面。两个特征值都小，且近似相等；自相关函数数值在各个方向上都小。
- 图像中的角点。两个特征值都大，且近似相等，自相关函数在所有方向都增大。
	
![image](http://images.cnitblog.com/blog/378920/201410/081646535629047.png)


根据二次项函数特征值的计算公式，我们可以求*M*(*x*,*y*) M(x,y)矩阵的特征值。但是Harris给出的角点差别方法并不需要计算具体的特征值，而是计算一个角点响应值*R* R来判断角点。*R* R的计算公式为：



*R*=*d**e**t**M*−*α*(*t**r**a**c**e**M*) 2  R=detM−α(traceM)2





式中，*d**e**t**M* detM为矩阵*M*=[*A**B**B**C* ] M=[ABBC]的行列式；*t**r**a**c**e**M* traceM为矩阵*M* M的直迹；*α* α为经常常数，取值范围为0.04~0.06。事实上，特征是隐含在*d**e**t**M* detM和*t**r**a**c**e**M* traceM中，因为，



*d**e**t**M*=*λ* 1 *λ* 2 =*A**C*−*B* 2  detM=λ1λ2=AC−B2







*t**r**a**c**e**M*=*λ* 2 +*λ* 2 =*A*+*C* traceM=λ2+λ2=A+C





## 2.2 Harris角点算法实现

根据上述讨论，可以将Harris图像角点检测算法归纳如下，共分以下五步：

1. 计算图像*I*(*x*,*y*) I(x,y)在*X* X和*Y* Y两个方向的梯度*I**x* 、*I**y*  Ix、Iy。



*I**x* =∂*I*∂*x* =*I*⊗(−1 0 1)，*I**y* =∂*I*∂*x* =*I*⊗(−1 0 1) *T*  Ix=∂I∂x=I⊗(−1 0 1)，Iy=∂I∂x=I⊗(−1 0 1)T





2. 计算图像两个方向梯度的乘积。



*I* 2 *x* =*I**x* ⋅*I**y* ，*I* 2 *y* =*I**y* ⋅*I**y* ，*I**x**y* =*I**x* ⋅*I**y*  Ix2=Ix⋅Iy，Iy2=Iy⋅Iy，Ixy=Ix⋅Iy





3. 使用高斯函数对*I* 2 *x* 、*I* 2 *y* 和*I**x**y*  Ix2、Iy2和Ixy进行高斯加权（取*σ*=1 σ=1），生成矩阵*M* M的元素*A*、*B* A、B和*C* C。



*A*=*g*(*I* 2 *x* )=*I* 2 *x* ⊗*w*，*C*=*g*(*I* 2 *y* )=*I* 2 *y* ⊗*w*，*B*=*g*(*I**x*,*y* )=*I**x**y* ⊗*w* A=g(Ix2)=Ix2⊗w，C=g(Iy2)=Iy2⊗w，B=g(Ix,y)=Ixy⊗w





4. 计算每个像素的Harris响应值*R* R，并对小于某一阈值*t* t的*R* R置为零。



*R*={*R*:*d**e**t**M*−*α*(*t**r**a**c**e**M*) 2 <*t*} R={R:detM−α(traceM)2<t}





5. 在3×3 3×3或5×5 5×5的邻域内进行非最大值抑制，局部最大值点即为图像中的角点。

> 
Harris角点检测的C++实现代码：[https://github.com/RonnyYoung/ImageFeatures/blob/master/source/harris.cpp](https://github.com/RonnyYoung/ImageFeatures/blob/master/source/harris.cpp)


## 2.3 Harris角点的性质

### 1. 参数*α* α对角点检测的影响

假设已经得到了矩阵*M* M的特征值*λ* 1 ≥*λ* 2 ≥0 λ1≥λ2≥0，令*λ* 2 =*k**λ* 1 ,0≤*k*≤1 λ2=kλ1,0≤k≤1。由特征值与矩阵*M* M的直迹和行列式的关系可得：



*d**e**t**M*=∏ *i**λ**i**t**r**a**c**e**M*=∑ *i**λ**i*  detM=∏iλi      traceM=∑iλi





从而可以得到角点的响应



*R*=*λ* 2 *λ* 2 =*α*(*λ* 2 +*λ* 2 ) 2 =*λ* 2 (*k*−*α*(1+*k*) 2 ) R=λ2λ2=α(λ2+λ2)2=λ2(k−α(1+k)2)





假设*R*≥0 R≥0，则有：



0≤*α*≤*k*(1+*k*) 2  ≤0.25 0≤α≤k(1+k)2≤0.25





对于较小的*k* k值，*R*≈*λ* 2 (*k*−*α*),*α*<*k* R≈λ2(k−α),α<k。

由此，可以得出这样的结论：增大*α* α的值，将减小角点响应值*R* R，降低角点检测的灵性，减少被检测角点的数量；减小*α* α值，将增大角点响应值*R* R，增加角点检测的灵敏性，增加被检测角点的数量。

### 2. Harris角点检测算子对亮度和对比度的变化不敏感

这是因为在进行Harris角点检测时，使用了微分算子对图像进行微分运算，而微分运算对图像密度的拉升或收缩和对亮度的抬高或下降不敏感。换言之，对亮度和对比度的仿射变换并不改变Harris响应的极值点出现的位置，但是，由于阈值的选择，可能会影响角点检测的数量。

![image](http://images.cnitblog.com/blog/378920/201410/081650213746803.png)![image](http://images.cnitblog.com/blog/378920/201410/081650220774418.png)

### 3. Harris角点检测算子具有旋转不变性

Harris角点检测算子使用的是角点附近的区域灰度二阶矩矩阵。而二阶矩矩阵可以表示成一个椭圆，椭圆的长短轴正是二阶矩矩阵特征值平方根的倒数。当特征椭圆转动时，特征值并不发生变化，所以判断角点响应值*R* R也不发生变化，由此说明Harris角点检测算子具有旋转不变性。

### 4. Harris角点检测算子不具有尺度不变性

如下图所示，当右图被缩小时，在检测窗口尺寸不变的前提下，在窗口内所包含图像的内容是完全不同的。左侧的图像可能被检测为边缘或曲线，而右侧的图像则可能被检测为一个角点。

![image](http://images.cnitblog.com/blog/378920/201410/071802526246781.png)

## 2.4 Harris的OpenCV接口

OpenCV的Hairrs角点检测的函数为cornerHairrs()，但是它的输出是一幅浮点值图像，浮点值越高，表明越可能是特征角点，我们需要对图像进行阈值化。
`C++: void cornerHarris(InputArray src, OutputArray dst, int blockSize, int apertureSize, double k, int borderType = BORDER_DEFAULT);`- **src** – 输入的单通道8-bit或浮点图像。
- **dst** – 存储着Harris角点响应的图像矩阵，大小与输入图像大小相同，是一个浮点型矩阵。
- **blockSize** – 邻域大小。
- **apertureSize** – 扩展的微分算子大。
- **k** – 响应公式中的，参数*α* α。
- **boderType** – 边界处理的类型。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
int main()
{
    Mat image = imread("../buliding.png");
    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);

    Mat cornerStrength;
    cornerHarris(gray, cornerStrength, 3, 3, 0.01);
    threshold(cornerStrength, cornerStrength, 0.001, 255, THRESH_BINARY);
    return 0;
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)



![image](http://images.cnitblog.com/blog/378920/201410/091745208908342.png)![image](http://images.cnitblog.com/blog/378920/201410/091748402335176.png)![image](http://images.cnitblog.com/blog/378920/201410/091745223902601.png)

从上面上间一幅图像我们可以看到，有很多角点都是粘连在一起的，我们下面通过加入非极大值抑制来进一步去除一些粘在一起的角点。

非极大值抑制原理是，在一个窗口内，如果有多个角点则用值最大的那个角点，其他的角点都删除，窗口大小这里我们用3*3，程序中通过图像的膨胀运算来达到检测极大值的目的，因为默认参数的膨胀运算就是用窗口内的最大值替代当前的灰度值。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
int main()
{
    Mat image = imread("buliding.png");
    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);

    Mat cornerStrength;
    cornerHarris(gray, cornerStrength, 3, 3, 0.01);

    double maxStrength;
    double minStrength;
    // 找到图像中的最大、最小值
    minMaxLoc(cornerStrength, &minStrength, &maxStrength);

    Mat dilated;
    Mat locaMax;
    // 膨胀图像，最找出图像中全部的局部最大值点
    dilate(cornerStrength, dilated, Mat());
    // compare是一个逻辑比较函数，返回两幅图像中对应点相同的二值图像
    compare(cornerStrength, dilated, locaMax, CMP_EQ);

    Mat cornerMap;
    double qualityLevel = 0.01;
    double th = qualityLevel*maxStrength; // 阈值计算
    threshold(cornerStrength, cornerMap, th, 255, THRESH_BINARY);
    cornerMap.convertTo(cornerMap, CV_8U);
    // 逐点的位运算
    bitwise_and(cornerMap, locaMax, cornerMap);

    drawCornerOnImage(image, cornerMap);
    namedWindow("result");
    imshow("result", image);
    waitKey();

    return 0;
}
void drawCornerOnImage(Mat& image, const Mat&binary)
{
    Mat_<uchar>::const_iterator it = binary.begin<uchar>();
    Mat_<uchar>::const_iterator itd = binary.end<uchar>();
    for (int i = 0; it != itd; it++, i++)
    {
        if (*it)
            circle(image, Point(i%image.cols, i / image.cols), 3, Scalar(0, 255, 0), 1);
    }
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

现在我们得到的效果就比默认的函数得到的结果有相当的改善，如上面最右边效果图。

# 3. 多尺度Harris角点

## 3.1 多尺度Harris角点的原理

虽然Harris角点检测算子具有部分图像灰度变化的不变性和旋转不变性，但它不具有尺度不变性。但是尺度不变性对图像特征来说至关重要。人们在使用肉眼识别物体时，不管物体远近，尺寸的变化都能认识物体，这是因为人的眼睛在辨识物体时具有较强的尺度不变性。在[图像特征提取：尺度空间理论](http://www.cnblogs.com/ronny/p/3886013.html)这篇文章里就已经讲到了高斯尺度空间的概念。下面将Harris角点检测算子与高斯尺度空间表示相结合，使用Harris角点检测算子具有尺度的不变性。

仿照Harris角点检测中二阶矩的表示方法，使用*M*=*μ*(*x*,*σ**I* ,*σ**D* ) M=μ(x,σI,σD)为尺度自适应的二阶矩：



*M*=*μ*(*x*,*σ**I* ,*σ**D* )=*σ* 2 *D**g*(*σ**I* )⊗[*L* 2 *x* (*x*,*σ**D* )*L**x**L**y* (*x*,*σ**D* ) *L**x**L**y* (*x*,*σ**D* )*L* 2 *y* (*x*,*σ**D* ) ] M=μ(x,σI,σD)=σD2g(σI)⊗[Lx2(x,σD)LxLy(x,σD)LxLy(x,σD)Ly2(x,σD)]





其中，*g*(*σ**I* ) g(σI)表示尺度为*s**i**g**m**a**I*  sigmaI的高斯卷积核，*x* x表示图像的位置。与高斯测度空间类似，使用*L*(*x*) L(x)表示经过高斯平滑后的图像，符号⊗ ⊗表示卷积，*L**x* (*x*,*σ**D* ) Lx(x,σD)和*L**y* (*x*,*σ**D* ) Ly(x,σD)表示对图像使用高斯*g*(*σ**D* ) g(σD)函数进行平滑后，在*x* x或*y* y方向取其微分的结果，即*L**x* =∂ *x**L* Lx=∂xL和*L**y* =∂ *y**L* Ly=∂yL。通常将*σ**I*  σI称为积分尺度，它是决定Harris角点当前尺度的变量，*σ**D*  σD为微分尺度或局部尺度，它是决定角点附近微分值变化的变量。显然，积分尺度*σ**I*  σI应该大于微分尺度*σ**D*  σD。

## 3.2 多尺度Harris角点实现

首先，检测算法从预先定义的一组尺度中进行积分尺度搜索，这一组尺度定义为



*σ* 1 …*σ**n* =*σ* 0 …*k**n**σ* 0  σ1…σn=σ0…knσ0





一般情况下使用k=1.4。为了减少搜索的复杂性，对于微分尺度*σ**D*  σD的选择，我们采用在积分尺度的基础上，乘以一个比例常数，即*σ**D* =*s**σ**I*  σD=sσI，一般取s=0.7。这样，通常使用积分和微分的尺度，便可以生成*μ*(*x*,*σ**I* ,*σ**D* ) μ(x,σI,σD)，再利用Harris角点判断准则，对角点进行搜索，具体可以分两步进行。

1. 与Harris角点搜索类似，对于给定的尺度空间值*σ**D*  σD，进行如下角点响应值计算和判断：



*c**o**r**n**e**r**n**e**s**s*=*d**e**t*(*μ*(*x*,*σ**n* )−*α**t**r**a**c**e* 2 (*μ*(*x*,*σ**n* )))>*t**h**r**e**s**h**o**l**d**H*  cornerness=det(μ(x,σn)−αtrace2(μ(x,σn)))>thresholdH





2. 对于满足1中条件的点，在点的8邻域内进行角点响应最大值搜索（即非最大值抑制）出在8邻域内角点响应最大值的点。对于每个尺度*σ**n* (1,2,…,*n*) σn(1,2,…,n)都进行如上搜索。

由于位置空间的候选点并不一定在尺度空间上也能成为候选点，所以，我们还要在尺度空间上进行搜索，找到该点的所谓特征尺度值。搜索特征尺度值也分两步。

1. 对于位置空间搜索到的每个候选点，进行拉普拉斯响应计算，并满足其绝对值大于给定的阈值条件：



*F*(*x*,*σ**n* )=*σ* 2 *n* |*L**x**x* (*x*,*σ**n* )+*L**y**y* (*x*,*σ**n* )|≥*t**h**r**e**s**h**o**l**d**L*  F(x,σn)=σn2|Lxx(x,σn)+Lyy(x,σn)|≥thresholdL





2. 与邻近的两个尺度空间的拉普拉斯响应值进行比较，使其满足：



*F*(*x*,*σ**n* )>*F*(*x*,*σ**l* ),   *l*∈{*n*−1.*n*+1} F(x,σn)>F(x,σl),   l∈{n−1.n+1}





满足上述条件的尺度值就是该点的特征尺度值。这样，我们就找到了在位置空间和尺度空间都满足条件的Harris角点。

> 
多尺度Harris角点检测C++实现：[https://github.com/RonnyYoung/ImageFeatures/blob/master/source/harrisLaplace.cpp](https://github.com/RonnyYoung/ImageFeatures/blob/master/source/harrisLaplace.cpp)


# 4. 更多的讨论

在上面描述的Harris角点具有光照不变性、旋转不变性、尺度不变性，但是严格意义上来说并不具备仿射不变性。Harris-Affine是一种新颖的检测仿射不变特征点的方法，可以处理明显的仿射变换，包括大尺度变化和明显的视角变化。Harris-Affine主要是依据了以下三个思路：
- 特征点周围的二阶矩的计算对区域进行的归一化，具有仿射不变性；
- 通过在尺度空间上归一化微分的局部极大值求解来精化对应尺度；
- 自适应仿射Harris检测器能够精确定位牲点；

这篇文章不对Harris-Affine作进一步的描述，有时间会对这一算法做专门的分析，有兴趣的可以参考原论文：[Scale & Affine Invariant Interest Point Detectors](https://github.com/RonnyYoung/ImageFeatures/blob/master/papers/corners/Scale%26Affine_Invariant_Interest_Point_Detectors..pdf).

# 5. 参考资料

[1] 《图像局部不变特征与描述》王永明，王贵锦。

[2] [Harris角点及Shi-Tomasi角点检测](http://blog.csdn.net/xw20084898/article/details/21180729)

[3] [图像特征提取PPT](http://wenku.baidu.com/link?url=OoAlTQjV3_WSi9ycEopQNL0FyWqHG2Sh_whAhnwqFB-R6Hi7Z-Sjsv_eM-ei4iugs9qu8LUF6uw1dirWTQvFTmer06mr9-2yvWPAWmM2-Fm)

[4] [Harris角点检测算法 1](http://www.cnblogs.com/ztfei/archive/2012/05/07/2487123.html)

[5] [OpenCV Harris角点检测](http://docs.opencv.org/modules/imgproc/doc/feature_detection.html?highlight=harris#cv.CornerHarris)

[6] [Opencv学习笔记（五）Harris角点检测](http://blog.csdn.net/crzy_sparrow/article/details/7391511)



3、Shi-Tomasi 算法

Shi-Tomasi 算法是Harris 算法的改进。Harris 算法最原始的定义是将矩阵 M 的行列式值与 M 的迹相减，再将差值同预先给定的阈值进行比较。后来Shi 和Tomasi 提出改进的方法，若两个特征值中较小的一个大于最小阈值，则会得到强角点。

如上面第二幅图中，对自相关矩阵 M 进行特征值分析，产生两个特征值![](https://img-my.csdn.net/uploads/201207/31/1343711271_9118.png)和两个特征方向向量。因为较大的不确定度取决于较小的特征值，也就是![](https://img-my.csdn.net/uploads/201207/31/1343711386_9399.png)，所以通过寻找最小特征值的最大值来寻找好的特征点也就解释的通了。

Shi 和Tomasi 的方法比较充分，并且在很多情况下可以得到比使用Harris 算法更好的结果。

 【相关代码】

由于这种Shi-Tomasi算子与1994年在文章 *Good Features to Track* [1]中提出，OpenCV 实现的算法的函数名定义为 goodFeaturesToTrack：

**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)
- **void** goodFeaturesToTrack( InputArray image, OutputArray corners,  
- **int** maxCorners, **double** qualityLevel, **double** minDistance,  
-                                      InputArray mask=noArray(), **int** blockSize=3,  
- **bool** useHarrisDetector=**false**, **double** k=0.04 );  

自定义使用函数（以方便createTrackbar的响应）如下：

**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)
- **void** cornerShiTomasi_demo( **int**, **void*** )  
- {  
- **if**( maxCorners < 1 ) { maxCorners = 1; }  
- /// Parameters for Shi-Tomasi algorithm
-   vector<Point2f> corners;  
- **double** qualityLevel = 0.01;  
- **double** minDistance = 10;  
- **int** blockSize = 3;  
- **bool** useHarrisDetector = **false**;  
- **double** k = 0.04;  
- /// Copy the source image
-   Mat cormat;  
- /// Apply corner detection :Determines strong corners on an image.
-   goodFeaturesToTrack( src_gray,   
-                corners,  
-                maxCorners,  
-                qualityLevel,  
-                minDistance,  
-                Mat(),  
-                blockSize,  
-                useHarrisDetector,  
-                k );  
- /// Draw corners detected
- **for**( **int** i = 0; i < corners.size(); i++ ){   
-       circle( dst_norm_scaled,  corners[i], 5,  Scalar(255), 2, 8, 0 );   
-       circle( src, corners[i], 4, Scalar(0,255,0), 2, 8, 0 );   
-   }  
- 
- /// Show what you got
-   imshow( corners_window, dst_norm_scaled );  
-   imshow( source_window, src );    
- }  




实践

在主函数中定义两个进度条方便调整阈值：

**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)
- namedWindow( source_window, CV_WINDOW_AUTOSIZE );  
- createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );  
- createTrackbar( "Max  corners:", source_window, &maxCorners, maxTrackbar, cornerShiTomasi_demo );    
- 
- namedWindow( corners_window, CV_WINDOW_AUTOSIZE );  
- namedWindow( source_window, CV_WINDOW_AUTOSIZE );  
- imshow( source_window, src );    
- 
- cornerHarris_demo( 0, 0 );  
- cornerShiTomasi_demo( 0, 0 );  



这里还需要说的是OpenCV 2.4.2中给的角点检测跟踪的示例代码有些问题，是应为SURF等不再定义在 feature2d模块中，而是legacy和nonfree，所以需要加入引用：

**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7805206#)
- #include "opencv2/legacy/legacy.hpp"
- #include "opencv2/nonfree/nonfree.hpp"



角点检测结果：

![](https://img-my.csdn.net/uploads/201207/31/1343712320_9626.png)

蓝色实心点为Harris检测结果，绿色空心圈为goodFeaturetoTrack检测结果。

M特征值分解后每个像素点相减的图（也就是Harris阈值判断的图）如下：

![](https://img-my.csdn.net/uploads/201207/31/1343712544_1407.png)

黑色实心点为Harris阈值检测结果，白色空心圈为阈值为27时Shi-Tomasi检测结果。

## 4、FAST角点检测算法

Smith 和 Brady在1997年提出了一种完全不同的角点提取方法，即“SUSAN (Smallest UnivalueSegment AssimilatingNucleus)”提取算子。SUSAN 提取算子的基本原理是，与每一图像点相关的局部区域具有相同的亮度。如果某一窗口区域内的每一像元亮度值与该窗口中心的像元亮度值相同或相似，这一窗口区域将被称之为“USAN”。计算图像每一像元的“USAN”，为我们提供了是否有边缘的方法。位于边缘上的像元的“USAN”较小，位于角点上的像元的“USAN”更小。因此，我们仅需寻找最小的“USAN”，就可确定角点。该方法由于不需要计算图像灰度差，因此，具有很强的抗噪声的能力。



Edward Rosten and TomDrummond 在2006年提出了一种简单快速的角点探测算法，该算法检测的角点定义为在像素点的周围邻域内有足够多的像素点与该点处于不同的区域。应用到灰度图像中，即有足够多的像素点的灰度值大于该点的灰度值或者小于该点的灰度值。

![](https://img-blog.csdn.net/20140114205555062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWhhZGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

考虑下图中p点附近半径为3的圆环上的16个点，一个思路是若其中有连续的12个点的灰度值与p点的灰度值差别超过某一阈值，则可以认为p点为角点。



这一思路可以使用机器学习的方法进行加速。对同一类图像，例如同一场景的图像，可以在16个方向上进行训练，得到一棵决策树，从而在判定某一像素点是否为角点时，不再需要对所有方向进行检测，而只需要按照决策树指定的方向进行2-3次判定即可确定该点是否为角点。

﻿﻿



