# 运动目标跟踪（十一）--CN及CSK跟踪原理 - 工作笔记 - CSDN博客





2016年08月17日 11:48:24[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：14400
个人分类：[运动跟踪](https://blog.csdn.net/App_12062011/article/category/6269524)

所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









**原文：**

**http://blog.csdn.net/hlinghling/article/details/44308199**

**基于自适应颜色属性的目标追踪（就是CN）**



Adaptive Color Attributes for Real-Time Visual Tracking

**基于自适应颜色属性的实时视觉追踪**

 3月讲的第一篇论文，个人理解，存在很多问题，欢迎交流！




这是CVPR2014年的文章。名字翻译为基于自适应选择颜色属性的实时视觉跟踪。首先理解什么是Adaptive color attributes，文章中colorattributes把颜色分为11类，就是将RGB三种颜色细化为黑、蓝、棕、灰、绿、橙、粉、紫、红、白和黄共11种。那么如何做到adaptive（自适应）？就是实时的选择比较显著的颜色，这个选择的过程是一种类似PCA（主成分分析）中降维的思想，将11维特征降为2 维（代码里使用的就是2)，文章重点之一就是这个。



**主成分分析：主要用于特征的降维。**

PCA是指它可以从多元事物中解析出主要影响因素，揭示事物的本质，简化复杂的问题，是寻找最小均方意义下、最能代表原始数据的投影方法。主成分分析的主要思想是寻找到数据的主轴方向，由主轴构成一个新的坐标系，这里的维数可以比原维数低，然后数据由原坐标系向新的坐标系投影，这个投影的过程就是降维的过程。



# 摘要



视觉跟踪在计算机视觉中是一个很有挑战性的问题，现在最好的（state-of-art) 视觉跟踪器或者依赖于光照信息或是使用简单的颜色表示来描述图片。与视觉跟踪不同的是，在目标识别和检测问题中，结合光照信息和复杂的颜色特征可以提供非常好的表现。由于跟踪问题的复杂性，所需要的颜色特征应该被有效的计算并且拥有一定的光学不变形同时保持较高的辨别力。

这篇文章研究基于检测的跟踪（tracking-by-detection）结构下颜色属性的贡献值。我们的结果表明颜色属性对于视觉跟踪问题具有优越的表现。我们进而提出一种自适应低维颜色属性的转化。用41个有挑战性的基准颜色序列进行基于量化和属性评估方法的评价。该方法比基于光照强度的跟踪器提升了24%的距离精度。此外，可以得到我们的方法胜过最先进的追踪方法，并且速度可达到100fps以上。

# 1.     Introduction

目标跟踪就是在图片序列中找到目标的位置，（目标是提前标明的，即在第一帧给出），在计算机视觉中是最优挑战性的问题，在许多应用中扮演了至关重要的作用，比如，人机交互，视频监控，机器人。跟踪问题之所以复杂是因为，跟踪过程中可能发生光照改变，遮挡，背景干扰，跟踪目标发生形变。本文调查了如何使用颜色信息来减小上述问题的影响。

现在最先进的跟踪器或是使用光照强度（RGB值）或是使用纹理信息。尽管现在在视觉跟踪方面已经取得了很大的进展，但是对于颜色信息的使用还是仅限于简单的颜色空间转换。和视觉跟踪不同的是，在目标检测方面，复杂的，巧妙设计的颜色特征显示了非常好的效果，而利用颜色信息做视觉跟踪是一件很难的事情。颜色测量结果在整个图片序列中变化很大，原因包括光照改变，阴影，相机和目标几何位置的变化。对于彩色图像在上述情况下鲁棒性的评价已经用在 图像分类，行为识别上，因此我们使用现在的评价方法来评价对目标跟踪这个问题的颜色转换的方法。

现在处理视觉跟踪问题的方法有两种，叫生成方法和判别方法（看过机器学习的人应该很熟）。生成方法不断去搜索和目标最相似的区域，这类方法或者基于模板匹配或是基于子空间模型（子空间这个概念有点唬人，我觉得就是将原始的目标，分为好多层，就像ps里的图层一样）。判别方法旨在将目标从背景中区分出来，就是将跟踪问题变为二分类问题。生成方法只使用了目标的信息，而判别方法既使用目标信息也使用背景的信息，找到一个将目标区分出来的分类界限。这种方法在很多基于tracking by detection框架的算法中使用，即使用目标和目标附近的环境训练出一个online的分类器。在前几年有个对现在比较优秀跟踪器的评价，（可以看cvpr2013年的一个benchmark)，里面有个csk的跟踪器，排在前十名，而且拥有非常高的速度，这种跟踪器发现了一个密度采样的策略，就是对一帧中多个子窗口的处理，可以归为对一个循环矩阵的处理，由于csk良好的表现和快速，我们的方法是基于csk跟踪器。

贡献：本文使用颜色属性扩展了csk跟踪器，并取得了良好的表现。csk的模型更新机制在处理多通道信号时，是次优的，为此我们调整了原来csk的更新机制，在实验中证明了新机制在多通道跟踪中的有效性。高维的颜色属性导致计算复杂度上的增加，可能限制跟踪器在实时应用和机器人中的使用，为此我们提出了自适应的维度下降方法，将原始的11维下降到2维，这样跟踪器的帧率就可以达到100fps以上并且不影响精确度。最后我们做了大量的评价证明了。





# 2.     The CSK Tracker

            CSK是在一个单独的图像碎片中从目标中得到核心的最小方形分类器。首先，这篇文章使用的决策函数是一个结构风险最小化的函数：

![](https://img-blog.csdn.net/20150316165957364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


文章中：

![](https://img-blog.csdn.net/20150316170301319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150316170152022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20150316170206733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150316170350927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


具体的细节后面再说。

            Vapnik等人在多年研究统计学习理论基础上对线性分类器提出了另一种设计最佳准则。其原理也从线性可分说起，然后扩展到线性不可分的情况。甚至扩展到使用非线性函数中去，这种分类器被称为支持向量机（Support Vector Machine，简称SVM）。支持向量机的提出有很深的理论背景。

       支持向量机方法是在近年来提出的一种新方法。

            SVM的主要思想可以概括为两点：⑴它是针对线性可分情况进行分析，对于线性不可分的情况，通过使用非线性映射算法将低维输入空间线性不可分的样本转化为高维特征空间使其线性可分，从而 使得高维特征空间采用线性算法对样本的非线性特征进行线性分析成为可能；





这里主要说代码的思路。



            首先：读入视频文件，得到groundtruth信息，也就得到了object的位置和大小的信息；然后得到一个在目标框图内目标的分布函数（高斯的分布，这一点我不是很明白，和公式里面不一样）；



            接下来：  读入第一张图片，转化为灰度图，对框内的数据进行窗口滤波的处理，得到一个边缘效应比较小的数据。并且这个数据是被归一化到-0.5~0.5的；



            然后：     通过以上数据求得核函数K；然后利用K再求出f（x）需要用到的 alpha（就是上面公式里面的c）；（值得注意的是这里对于这两个重要的参数的求解都是从FourierDomain求得的，这里是本文的一个创新点，也是速度如此快的原因）



            接下来：   对于后面的每一帧图像， 先转化为灰度图像，然后用hann窗预处理好输入的数据；接下来结合上一帧图像的信息再次计算K；然后由现在的alpha和K来计算出响应值，选出响应值最大的位置。（值得注意的是这里计算的出来的响应值是待处理的Frame里面的每一个可能的目标区域）



            最后：  根据响应值最大的位置来计算现在的K，然后更新alpha。然后处理下一帧图像。（同时也要看到，计算响应值和更新alpha所用到的K的计算的方式是不一样的。代码里面，计算响应值的K是目标和待检测的目标img进行卷积的，而更新的时候是目标和自己卷积的）





l **高斯函数**

[高斯](http://baike.baidu.com/view/2129.htm)函数的形式为：




[](http://baike.baidu.com/picture/856844/856844/0/32bb9c8be919e5c0fd1f103f.html?fr=lemma&ct=single)

其中 a、b 与 c 为实数常数 ，且a > 0.

c2 = 2 的[高斯](http://baike.baidu.com/view/2129.htm)函数是[傅立叶变换](http://baike.baidu.com/view/191871.htm)的特征函数。这就意味着[高斯](http://baike.baidu.com/view/2129.htm)函数的[傅立叶变换](http://baike.baidu.com/view/191871.htm)不仅仅是另一个高斯函数，而且是进行傅立叶变换的函数的标量倍。

在计算机视觉中，有时也简称为[高斯函数](http://baike.baidu.com/view/856844.htm)。[高斯函数](http://baike.baidu.com/view/856844.htm)具有五个重要的性质，这些性质使得它在早期图像处理中特别有用．这些性质表明，高斯平滑滤波器无论在空间域还是在[频率域](http://baike.baidu.com/view/737506.htm)都是十分有效的低通[滤波器](http://baike.baidu.com/view/141368.htm)，且在实际图像处理中得到了工程人员的有效使用．[高斯函数](http://baike.baidu.com/view/856844.htm)具有五个十分重要的性质，它们是：

（1）[**二维**](http://baike.baidu.com/view/719535.htm)[**高斯函数**](http://baike.baidu.com/view/856844.htm)**具有旋转对称性，即滤波器在各个方向上的平滑程度是相同的．一般来说，一幅图像的边缘方向是事先不知道的，因此，在滤波前是无法确定一个方向上比另一方向上需要更多的平滑．旋转对称性意味着高斯平滑滤波器在后续**[**边缘检测**](http://baike.baidu.com/view/178264.htm)**中不会偏向任一方向．**

（2）高斯函数是[单值函数](http://baike.baidu.com/view/4392676.htm)．这表明，高斯滤波器用像素邻域的加权均值来代替该点的像素值，而每一邻域像素点[权值](http://baike.baidu.com/view/2109980.htm)是随该点与中心点的距离单调增减的．这一性质是很重要的，因为边缘是一种图像局部特征，如果平滑运算对离算子中心很远的像素点仍然有很大作用，则平滑运算会使图像失真．

（3）高斯函数的[傅立叶变换](http://baike.baidu.com/view/191871.htm)频谱是单瓣的．正如下面所示，这一性质是高斯函数付立叶变换等于高斯函数本身这一事实的直接推论．图像常被不希望的高频信号所污染(噪声和细纹理)．而所希望的图像特征（如边缘），既含有低频分量，又含有高频分量．高斯函数付立叶变换的单瓣意味着平滑图像不会被不需要的高频信号所污染，同时保留了大部分所需信号．

（4）高斯滤波器宽度(决定着平滑程度)是由参数σ表征的，而且σ和平滑程度的关系是非常简单的．σ越大，高斯滤波器的[频带](http://baike.baidu.com/view/488112.htm)就越宽，平滑程度就越好．通过调节平滑程度参数σ，可在图像特征过分模糊(过平滑)与平滑图像中由于噪声和细纹理所引起的过多的不希望突变量(欠平滑)之间取得折衷．

（5）由于[高斯函数](http://baike.baidu.com/view/856844.htm)的可分离性，大[高斯滤波](http://baike.baidu.com/view/2103722.htm)器可以得以有效地实现．[二维](http://baike.baidu.com/view/719535.htm)高斯函数[卷积](http://baike.baidu.com/view/523298.htm)可以分两步来进行，首先将图像与[一维](http://baike.baidu.com/view/724359.htm)高斯函数进行卷积，然后将卷积结果与方向垂直的相同一维高斯函数卷积．因此，二维高斯滤波的计算量随滤波模板宽度成[线性](http://baike.baidu.com/view/300474.htm)增长而不是成平方增长．

l **希尔伯特****空间**

完备的内积空间，在一个[复数](http://zh.wikipedia.org/wiki/%E5%A4%8D%E6%95%B0)向量空间上的给定的[内积](http://zh.wikipedia.org/wiki/%E5%86%85%E7%A7%AF)可以按照如下的方式导出一个[范数](http://zh.wikipedia.org/wiki/%E8%8C%83%E6%95%B0)（norm）：

此空间称为是一个希尔伯特空间，如果其对于这个范数来说是[完备](http://zh.wikipedia.org/wiki/%E5%AE%8C%E5%A4%87)的。这里的完备性是指，任何一个[柯西列](http://zh.wikipedia.org/wiki/%E6%9F%AF%E8%A5%BF%E5%88%97)都收敛到此空间中的某个元素，即它们与某个元素的范数差的[极限](http://zh.wikipedia.org/wiki/%E6%9E%81%E9%99%90)为。任何一个希尔伯特空间都是[巴拿赫空间](http://zh.wikipedia.org/wiki/%E5%B7%B4%E6%8B%BF%E8%B5%AB%E7%A9%BA%E9%97%B4)，但是反之未必。

l  **离散傅里叶变换和快速傅里叶变换**

l  **高斯RBF内核（高斯径向基核函数）**

所谓径向基函数 (Radial Basis Function简称 RBF),就是某种沿径向对称的标量函数。通常定义为空间中任一点x到某一中心xc之间[欧氏距离](http://baike.baidu.com/view/2898228.htm)的[单调函数](http://baike.baidu.com/view/377489.htm),可记作
 k(||x-xc||),其作用往往是局部的 ,即当x远离xc时函数取值很小。最常用的径向基函数是高斯核函数
 ,形式为k(||x-xc||)=exp{- ||x-xc||^2/(2*σ^2) }其中xc为核函数中心,σ为函数的宽度参数
 , 控制了函数的径向作用范围。

**l  核函数（Kernels）**

            将核函数形式化定义，如果原始特征内积是[](http://images.cnblogs.com/cnblogs_com/jerrylead/201103/201103182034291172.png)，映射后为[](http://images.cnblogs.com/cnblogs_com/jerrylead/201103/201103182034309711.png)，那么定义核函数（Kernel）为[](http://images.cnblogs.com/cnblogs_com/jerrylead/201103/201103182034302186.png)

由于计算的是内积，我们可以想到IR中的余弦相似度，如果x和z向量夹角越小，那么核函数值越大，反之，越小。因此，核函数值是[](http://images.cnblogs.com/cnblogs_com/jerrylead/201103/201103182034377838.png)和[](http://images.cnblogs.com/cnblogs_com/jerrylead/201103/20110318203438804.png)的相似度。

**SVM的核函数如何选取？**

1. Linear核：主要用于线性可分的情形。参数少，速度快，对于一般数据，分类效果已经很理想了。

2. RBF核：主要用于线性不可分的情形。参数多，分类结果非常依赖于参数。有很多人是通过训练数据的交叉验证来寻找合适的参数，不过这个过程比较耗时。我个人的体会是：使用libsvm，默认参数，RBF核比Linear核效果稍差。通过进行大量参数的尝试，一般能找到比linear核更好的效果。



# 3.     颜色视觉追踪

为了能合并颜色信息，我们通过定义一个合适的内核K来扩展CSK追踪不起到多维颜色特征。

1.     a Hann Window

汉宁窗又称升余弦窗，汉宁窗可以看作是3个矩形时间窗的频谱之和，或者说是 3个
 sinc（t）型函数之和，而括号中的两项相对于第一个谱窗向左、右各移动了 π/T，从而使旁瓣互相抵消，消去高频干扰和漏能。可以看出，汉宁窗主瓣加宽并降低，旁瓣则显著减小，从减小泄漏观点出发，汉宁窗优于矩形窗．但汉宁窗主瓣加宽，相当于分析带宽加宽，频率分辨力下降。




2.     低维度自适应颜色属性

提出一种自适应维度减少的方法，它能够存储有用信息的同时大大减少颜色维度的数量，从而使得速度有了显著的提升。



**Adaptivecolor attributes:**

  文章中colorattributes把颜色分为11类，就是将RGB三种颜色细化为11种基本颜色（black，blue，brown，grey，green，orange，pink，purple，red，white，yellow）。



 如何做到adaptive（自适应）？就是实时的选择比较显著的颜色，这个选择的过程是一种类似PCA（主成分分析）中降维的思想，将11维特征降为2 维（代码里使用的就是2) 。

**对于CN（color name）映射有两种规范化技术：**

1.将11个颜色容器中各减去1/11,CN将变为10维子空间

2.将CN投影到10维子空间的标准正交基上。（效果更好）



l  **基于颜色特征拓展CSK分类器：**

**目标模型由两部分组成：**

学习目标外观（thelearned target appearance）  &&  变换的分类器参数A

1）CSK tracker 线性插值：

           （并非前面所有的帧都同时用于更新当前的数据）

2）文献4 MOSSE tracker：在更新方案中计算当前帧的同时考虑前面所有帧（本文所用的方法）

           （但仅用于一维，我们需要将方法用于多维颜色特征）



**更新分类器：**

l  **最小代价函数：**

**![](https://img-blog.csdn.net/20150316164411986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

![](https://img-blog.csdn.net/20150316164345926?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







l  **低维自适应颜色属性：**



**方法：通过最小代价函数，为当前帧P找到一个适合的降维映射。**

**![](https://img-blog.csdn.net/20150316164539090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGxpbmdobGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**










# 4.     实验

三个评估参数：

CLE中心位置误差：目标的估计中心位置与真实地面的平均欧氏距离；

DP距离精度：中心误差小于一个给定阈值的帧的相对数量；

OP重复精度：边界框重叠的地方超过一个阈值的百分比。

实验图见论文

**1.颜色特征的评估**



2.**更新方案**



3.**低维自适应颜色属性**



4.与最先进方法比较




结论：

CN和CN2得到较好的结果，CN2速度要快，本文提出的的方法较好！

总结：

本文提出使用颜色属性来进行视觉追踪，并且将CSK追踪器的学习方案扩展到多通道颜色特征，提出基于颜色属性的低维自适应扩展。最终可以得到结论：文章中提出的方法在100FPS的速度的同时保持最高的精确度。



# 解释：



Struck：Structuredoutput tracking with kernels基于内核的结构化输出跟踪

EDFT：Enhanced distribution fieldtracking using channel representations基于通道表示的增强分布场追踪

CSK：Exploiting the circulantstructure of tracking-by-detection with kernels

LSHT：Visual tracking via localitysensitive histograms基于局部敏感直方图的目标跟踪算法

 有对应源码和论文

本文的参考文献：

1.CSK :J.Henriques, R.Caseiro,
 P. Martins, and J. Batista. Exploitingthecirculant structure oftracking-by-detection with kernels. InECCV, 2012.

2. J. van deWeijer, C.Schmid, J. J.Verbeek,
 and D.Larlus. Learning color namesfor real-world applications.TIP,18(7):1512–1524,2009. 




