# 热核特征（heat kernel signature,HKS）简单解释（附可运行代码） - 快来学习鸭～～～ - CSDN博客





2016年12月09日 13:56:52[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：6949标签：[热核特征																[HKS																[SIHKS																[WKS																[SGWS](https://so.csdn.net/so/search/s.do?q=SGWS&t=blog)
个人分类：[三维](https://blog.csdn.net/sxf1061926959/article/category/6565809)





**Author:** DivinerShi

[本文地址](http://blog.csdn.net/sxf1061926959/article/details/53538105)

最近在看相关东西，直接看文献的话，废话太多，中文的相关解释又很少，所以直接把wiki的翻译过来了，并做一点扩展。[wiki英文原版](https://en.wikipedia.org/wiki/Heat_kernel_signature)
**热核特征（Heat Kernel signature，HKS）**是用于形变三维形状分析的特征描述子，属于谱形状分析方法。对于三维形状上的每个点，HKS定义了它的特征向量（feature vector）用于表示点的局部和全局属性。其广泛应用于是三维分割、分类、结构探索、形状匹配和形状检索。 

    HKS由Jian Sun,Maks Ovsjanikov 和Leonidas Guibas在2009年提出。它是基于热核（heat kernel）提出来的。HKS是基于拉普拉斯贝尔特米算子的形状描述子。
**简单理解的话，热核特征是去计算三维模型表面的每个点，随时间变化后热量的剩余情况，因为每个点周围的情况是不一样的，这样的话，每个点假设都有一个相同的初始热量，随时间推移，因为点周边的情况不一样，那么热量扩散的速度也不一样，所以随着时间的变化，每个点的热量变化将会形成一条下降的曲线，再把这条曲线离散化，我们就可以得到一个点的热核特征。再按该方法去计算每个点的热核特征，我们就可以得到整个三维模型的热核特征，可以用一个大矩阵表示。**

# OverView

HKS具有等距变换不变性（简单的说就是在三维模型发生非拉伸形变，如铰链，关节等变换的时候具有不变性质的属性），所以广泛使用在非刚性三维形状处理任务上。对每个确定的关键点使用特征向量进行表示。

HKS是基于表面热扩散的概念产生的，给定一个表面初始热量分布![这里写图片描述](https://img-blog.csdn.net/20170112133517094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，热核![这里写图片描述](https://img-blog.csdn.net/20170112133631658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示t时间内热量从x点转移到y点所需要的热量。热核对等距变化是不变的，而且对小的扰动稳定。另外，热核能完全等距表征一个形状，并且随着时间t的增加，就越能表示形状的全局属性。因为![这里写图片描述](https://img-blog.csdn.net/20170112133731346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)为一个时间域内的一对点x,y定义,得计算两两之间的数据，所以使用热核直接作为特征将导致较高的复杂度。而HKS只考虑![这里写图片描述](https://img-blog.csdn.net/20170112133828738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，将其自身限制在时间域内。但是HKS在特定条件下保留了热核的大部分属性。
# Technical details

黎曼流形M上的热扩散方法如下： 
![这里写图片描述](https://img-blog.csdn.net/20170112133941771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，![这里写图片描述](https://img-blog.csdn.net/20170112134047476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是一个拉普拉斯算子，![这里写图片描述](https://img-blog.csdn.net/20170112134142832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是点x在时间t的热分布。然后这个方程的解可以表示为： 
![这里写图片描述](https://img-blog.csdn.net/20170112134342974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对热核进行特征分解得到： 
![这里写图片描述](https://img-blog.csdn.net/20170112134822300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，![这里写图片描述](https://img-blog.csdn.net/20170112140752841?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)和![这里写图片描述](https://img-blog.csdn.net/20170112140813545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是![这里写图片描述](https://img-blog.csdn.net/20170112140850360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的第i个特征值和特征向量。 

    热核能完全等距的表征一个形状表面：对于任何两个黎曼流形M和N上的满映射![这里写图片描述](https://img-blog.csdn.net/20170112140628714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，如果![这里写图片描述](https://img-blog.csdn.net/20170112134955755?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，那么T是一个等距变换，反之亦然。对于一个简洁的特征描述子，HKS将热核限制到时间域内， 
![这里写图片描述](https://img-blog.csdn.net/20170112135108916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
HKS和热核一样，在M和N的![这里写图片描述](https://img-blog.csdn.net/20161209140721868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的特征值不重复的情况下表示模型的表面。![这里写图片描述](https://img-blog.csdn.net/20170112135223461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)能简单理解为由![这里写图片描述](https://img-blog.csdn.net/20170112141010704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)确定截至频率的低通滤波器。
# Practical considerations

一般来说![这里写图片描述](https://img-blog.csdn.net/20170112135310321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是一个无参的连续函数，而HKS在实际情况下表示为一个在时间![这里写图片描述](https://img-blog.csdn.net/20170112135423401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)上采样的离散序列![这里写图片描述](https://img-blog.csdn.net/20170112135506745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。在大多数应用中，一个对象的潜在流形结构是未知的。而HKS的话只要有流形的网格表示（mesh），通过使用![这里写图片描述](https://img-blog.csdn.net/20161209141042838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的一个离散的近似和使用热方程的离散类似物，就能计算HKS。在离散的情况下，拉普拉斯贝尔特米算子是一个稀疏矩阵，能被写做： 
![这里写图片描述](https://img-blog.csdn.net/20170112135559778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    其中A是一个A(i,i)表示的正对角矩阵，它对应网格中共享顶点i的三角形面积，W是一个对称的半正定权重矩阵。L能被分解成![这里写图片描述](https://img-blog.csdn.net/20170112135657748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，其中![这里写图片描述](https://img-blog.csdn.net/20161209141201621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是L升序排列的特征值的对角矩阵，![这里写图片描述](https://img-blog.csdn.net/20161209141229262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是其对应的特征向量。离散热核矩阵如下： 
![这里写图片描述](https://img-blog.csdn.net/20170112135816653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170112135925284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示时间t后顶点i和j之间的热量扩散。然后HKS就由矩阵的对角元素给出，其在离散的时间间隔上采样。和连续的情况相似，离散的HKS对噪声鲁棒。
# Limitations

## 1、不重复的特征值

使用HKS来表示模型表面的等距的主要属性需要保证模型表面的特征值不重复。也有一些特殊的例子不需要保证这个条件（尤其是有对称性质的），如球体。

## 2、时间参数选择

HKS中时间参数是和全局信息尺度非常相关的。但是没有很直接的方式去离散化的选择时间。现存的用对数的方式选择时间只是一个没有保证的启发式方法。

## 3、时间复杂度

离散热核需要大小为nxn的矩阵的特征分解，其中n是流形的网格表示（mesh）的顶点数。随着n的增加，计算特征分解的代价很高。但是需要指出的是，因为特征值的逆指数依赖，通常只需要很小的特征向量（少于100）就足够去获得一个HKS的很好的近似。

## 4、非等距变换

热核的性能保证只适用于等距变换。但是，真实形状常常不是等距变换的。一个简单的例子是当人握起拳头的时候，两个手指的测地距离发生了改变，就是两个手指挨在一起了。

# Relation with other methods

## 1、曲率

点x的连续的HKS，黎曼流形上的![这里写图片描述](https://img-blog.csdn.net/20161209141417212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是和标量曲率（scalar curvature）s(x)相关的，通过公式： 
![这里写图片描述](https://img-blog.csdn.net/20170112140057973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因此，HKS能解释为点x在尺度t时的曲率。

## 2、波核特征（Wave kernel signature,wks）

WKS和HKS相似，只是把热方程改成了薛定谔波动方程（ Schrödinger wave equation）： 
![这里写图片描述](https://img-blog.csdn.net/20170112140206826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    其中![这里写图片描述](https://img-blog.csdn.net/20170112141103346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是复波函数。在点x出测量粒子的平均概率由下式给出： 
![这里写图片描述](https://img-blog.csdn.net/20170112140311842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    其中f是初始能量分布。通过固定一系列这些能量分布![这里写图片描述](https://img-blog.csdn.net/20170112141223472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，WKS能被获得作为一个离散序列![这里写图片描述](https://img-blog.csdn.net/20170112140358461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。不像HKS，WKS能被理解为一系列的带通滤波器，从而实现更好的特征定位。但是，WKS不能很好的表示大尺度的特征（因为大尺度的特征被过滤了），在形状匹配应用中性能不佳。
## 3、全局特征点（GPS）

和HKS相似，GPS是基于拉普拉斯贝尔特米算子。点x处的GPS是在点x用拉普拉斯贝尔特米算子的尺度特征函数计算出的向量。GPS是一个全局特征，而HKS的尺度是可以根据热扩散的时间参数不同而不同。因此，HKS能被用在局部形状匹配应用中，而GPS不行。

## 4、谱图小波特征（SGWS）

SGWS提供了谱描述子的一般形式，其中可以通过制定滤波函数来获得HKS。SGWS是一个多解的局部描述子。不仅等距不变而且更加简洁，易于计算，结合了带通和低通滤波器的优点。

# Extensions

## 1、尺度不变

尽管HKS在多尺度表示形状，但是并不是天生的尺度不变。举例来说，对于一个形状和它的不同尺度版本的形状，在没有预先标准化的前提下，它们的HKS是不一样的。一个简单的方法是去报尺度不变性是去预先缩放形状到有一个相同的表面积。使用上面的符号，就是说：

![这里写图片描述](https://img-blog.csdn.net/20170112140501071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

或者说，HKS的尺度不变版本也可以通过构造一个尺度空间来表示。在尺度空间中，一个缩放的形状的HKS对应到转化为一个乘法因子。该HKS的傅立叶变化将时间平移变换到复平面中，并且通过考虑变换的模量可以消除对平移的依赖性。（这块我也不懂，直译的）。
## 2、体积HKS

HKS被定义为3D形状的边界表面，表示为一个2D的黎曼流形。代替只考虑边界，3D形状的整个体积能被考虑定义HKS的体积版本。体积HKS定义和一般的HKS一样，通过考虑整个体积上（作为三个子流形）的热方程，并且在3D形状的2D流形边界上定义了一个Neumann边界条件。体积HKS表示了体积的等距变换，其表示的对真实的3D对象的变换比边界变换更真实。（这块也不是很懂，直译）。

## 3、性状搜素

尺度不变HKS特征可以被用在词袋模型中，用于形状检索。这个特征，通过考虑他们的一些空间关系，被用于构建几何单词，通过这种方法，一个特征能表示为一个word，一个形状表示为一个sentence。形状是通过二进制代码形成一个索引集合，给定一个查询形状，在索引中相似的形状可能是等距变换的，然后通过海明距离，找出距离最短的形状。

# References
- Sun, J. and Ovsjanikov, M. and Guibas, L. (2009). “A Concise and Provably Informative Multi-Scale Signature-Based on Heat Diffusion”. Computer Graphics Forum. 28. pp. 1383–1392. 

2.Alexander M. Bronstein (2011). “Spectral descriptors for deformable shapes”. arXiv:1110.5015 

3.Grigor’yan, A. (2006). “Heat kernels on weighted manifolds and applications”. Contemporary Mathematics. 398: 93–191. doi:10.1090/conm/398/07486. 

4.Aubry, M. and Schlickewei, U. and Cremers, D. (2011). “The Wave Kernel Signature—A Quantum Mechanical Approach to Shape Analysis”. IEEE International Conference on Computer Vision (ICCV) - Workshop on Dynamic Shape Capture and Analysis (4DMOD). 

5.Rustamov, R.M. (2007). “Laplace–Beltrami eigenfunctions for deformation invariant shape representation”. Proceedings of the fifth Eurographics symposium on Geometry processing. Eurographics Association. pp. 225–233. 

6.C. Li; A. Ben Hamza (2013). “A multiresolution descriptor for deformable 3D shape retrieval”. The Visual Computer: 513–524. 

7.Bronstein, M.M.; Kokkinos, I. (2010). “Scale-invariant heat kernel signatures for non-rigid shape recognition”. Computer Vision and Pattern Recognition (CVPR), 2010. IEEE. pp. 1704–1711. 

8.Raviv, D. and Bronstein, M.M. and Bronstein, A.M. and Kimmel, R. (2010). “Volumetric heat kernel signatures”. Proceedings of the ACM workshop on 3D object retrieval. ACM. pp. 30–44. 

9.Bronstein, A.M. and Bronstein, M.M. and Guibas, L.J. and Ovsjanikov, M. (2011). “Shape google: Geometric words and expressions for invariant shape retrieval”. ACM Transactions on Graphics. ACM. 30 (1). 

网上找了些都不能用，师兄给了个可运行的matlab代码，共享下。 
[热核特征的matlab可运行代码（github地址）](https://github.com/Shicoder/HKS.git)](https://so.csdn.net/so/search/s.do?q=WKS&t=blog)](https://so.csdn.net/so/search/s.do?q=SIHKS&t=blog)](https://so.csdn.net/so/search/s.do?q=HKS&t=blog)](https://so.csdn.net/so/search/s.do?q=热核特征&t=blog)




