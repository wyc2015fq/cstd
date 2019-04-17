# 三维计算机视觉（七）--Spin image - 工作笔记 - CSDN博客





2017年10月07日 12:20:55[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7462
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









Spin image是基于点云空间分布的最经典的特征描述方法。

Spin image的思想是将一定区域的点云分布转换成二维的spin image​，然后对场景和模型的spin image​s进行相似性度量。原理图如下：


*![](https://img-blog.csdn.net/20171007122444290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*


大写P------三维网格某顶点p的切面

**n**-------p点单位法向量

​x-------p附近的三维网格上的另一个顶点

α------x点在P上投影与p的距离

β------x点与P点的垂直距离

其中p和**n**定义为一个定向点(Oriented point)。

一、生成一个spin image的步骤：

1.定义一个​Oriented point

2.以Oriented point为轴生成一个圆柱坐标系

​3.定义Spin image的参数，Spin image是一个具有一定大小（行数列数）、分辨率（二维网格大小）的二维图像（或者说网格）。Spin image的三个关键参数将在后面讨论。

4.将圆柱体内的三维坐标​投影到二维Spin image，这一过程可以理解为一个Spin image绕着法向量**n**旋转360度，Spin image扫到的三维空间的点会落到Spin image的网格中。就是如下的公式：

![](https://img-blog.csdn.net/20171007134743857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


*﻿从三维空间投影到spinimage坐标*


5.根据spin image中的每个网格中落入的点不同，计算每个网格的强度I，显示spin image时以每个网格（也就是像素）I不同为依据。最直接的方法是直接计算每个网格中落入的点，然而为了降低对位置的敏感度降低噪音影响增加稳定性，Johnson论文中用双线性插值的方法将一个点分布到4个像素中。原理如下图：


*![](https://img-blog.csdn.net/20171007140624298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**﻿双线性插值*


上图中，默认的网格（像素）边长是1（真实边长的选择会在稍后讨论），当一个点落入网格（i,j）中时会被双线性插值分散到（i,j）、（i,j+1）、（i+1,j）、（i+1,j+1）四个网格中。

这样就获得了spin image，如下图所示。​


*![](https://img-blog.csdn.net/20171007140714872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)﻿**spin image*





​如上图所示，第一副图很巧选择的是一个那片区域中没有比选择的点更凸出的点，spin image就像一副带粘性的砧板，绕着所选择顶点的法向量轴自转360°，所到之处的质点全部被粘到板上，累积成一副图像，在经过处理就成了spin image。

二、spin image的三个关键参数

1.​分辨率，即二维网格的也就是像素的实际尺寸，使用和三维网格相近的尺寸比较合适，因此通常是取三维网格所有边的平均值来作为spin image的每个网格尺寸，通常会把网格的长和宽定义成相等，即边长，边长的计算公式：


![](https://img-blog.csdn.net/20171007140922927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[](http://photo.blog.sina.com.cn/showpic.html#blogid=&url=http://album.sina.com.cn/pic/002QueF9zy72Tb7jZpz5d)


​e为三维网格模型中的一条边，N为三维网格模型中的边的总数。

2.大小，即spin image的行数和列数​，两者一般也相等。可以参考的大小10x10或20x20等。

3.​support angle,即法向量夹角的大小限制。空间中顶点的法向量与创建圆柱坐标系所选点法向量之间的夹角。效果如下图：


![](https://img-blog.csdn.net/20171007141024533?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[](http://photo.blog.sina.com.cn/showpic.html#blogid=&url=http://album.sina.com.cn/pic/002QueF9zy72TdmA0s830)


可以看出，对角度限制以后，那些相当于切面的“凹点（大于90°）”被剔除，保留了主要信息，降低了后续的计算量。一般角度限制范围为60°--90°之间。

三.spin image特征匹配​中的相似性度量和匹配时点的选择

​1.相似性度量

使用以下公式来计算两个spin images之间的相似性：


![](https://img-blog.csdn.net/20171007143802983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中R的计算公式：


![](https://img-blog.csdn.net/20171007143815083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[](http://photo.blog.sina.com.cn/showpic.html#blogid=&url=http://album.sina.com.cn/pic/002QueF9zy72TeUjrywb0)


​N为每个spin image像素数，atanh为反双曲正切函数，其输入的范围[-1,1]，原文中解释为这是一种典型的统计学手段。R的取值范围是[-1,1]，两个spin images越相似R越接近于1，完全一样时R的值为1。

可以看出​C由两部分组成第一部分是经​反双曲正切函数得出的值的平方，第二部分是一个权重λ乘以一个较小的数，当两个spin images相似时第二部分所占比重应较小，当不接近时第二部分所占比重应较大，λ起的作用是用来限制spin
 images低重合时匹配的情况。文中对λ的选择方式是将所有的spin images中的非空像素数按大小顺序列出来然后取中位数。这个中位数差不多是像素重叠的期望值。然后考虑到低重叠的情况，取这个中位数的一半来作为λ。

​三维模型与场景匹配按下图的流程进行


![](https://img-blog.csdn.net/20171007151150865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[](http://photo.blog.sina.com.cn/showpic.html#blogid=&url=http://album.sina.com.cn/pic/002QueF9zy72U8Xa5TMf6)


​如上图所示，计算出相关系数之后，会面临一个问题就是对于模型上的一个点，在场景中可能会有不止一个点与目标点的特征相近匹配，原因是模型的对称部分或者目标点附近的点的干扰。所以在确定Plausible correspondences的时候要进行过滤以及多元组匹配。文中给出了两种过滤的方法。

确定​Plausible correspondences之后就可以计算出Plausible Transformation。之后再用ICP算法进行精确匹配就可以得到精确的匹配。

​2.模型和场景匹配时点的选择

两个三维模型之间至少得有三个点匹配才能确定匹配。

对三维场景点的选择：

通过对形状编码确保均匀抽样，通过选择有精确法向量的spin image来提高匹配的几率。无杂点的完美模型一般随机选择10%的点就够了​，实际运用中一般选择1/20-1/2。

对三维模型点的选择​使用了相似度量直方图的方法​

​总结：Spin Image对遮挡和背景干扰具有很强的稳健性，在点云配准和三维目标识别中得到广泛应用，其不足之处在于不具有尺度不变性，需要较大的存储空间和要求点云均匀分布。


Reference:

1.Andrew Edie Johnson​.Spin-Images: A Representation for 3-D Surface Matching.1997

2.郭裕兰.距离图像局部特征提取方法综述.2011

3.刘瑶.基于自旋图的三维自动目标识别.2012



