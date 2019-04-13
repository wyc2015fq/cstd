
# opencv︱HOG描述符介绍+opencv中HOG函数介绍（一） - 素质云笔记-Recorder... - CSDN博客

2017年04月08日 11:58:16[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2765


> 1、HOG与SIFT的区别

> HOG和SIFT都是描述子，以及由于在具体操作上有很多相似的步骤，所以致使很多人误认为HOG是SIFT的一种，其实两者在使用目的和具体处理细节上是有很大的区别的。HOG与SIFT的主要区别如下：

> (1)SIFT是基于关键点特征向量的描述。 (2)HOG是将图像均匀的分成相邻的小块，然后在所有的小块内统计梯度直方图。

> (3)SIFT需要对图像尺度空间下对像素求极值点，而HOG中不需要。

> (4)SIFT一般有两大步骤，第一个步骤对图像提取特征点，而HOG不会对图像提取特征点。

> 2、HOG的优缺点 优点：

> (1)HOG表示的是边缘(梯度)的结构特征，因此可以描述局部的形状信息； (2)位置和方向空间的量化一定程度上可以抑制平移和旋转带来的影响；

> (3)采取在局部区域归一化直方图，可以部分抵消光照变化带来的影响；

> (4)由于一定程度忽略了光照颜色对图像造成的影响，使得图像所需要的表征数据的维度降低了；

> (5)而且由于这种分块分单元的处理方法，也使得图像局部像素点之间的关系可以很好得到表征。 缺点：

> (1)描述子生成过程冗长，导致速度慢，实时性差； (2)很难处理遮挡问题； (3)由于梯度的性质，该描述子对噪点相当敏感

> 本章转载于博客：HOG：
> [从理论到OpenCV实践](http://blog.csdn.net/zhazhiqiang/article/details/21047207)

> .

> 一、HOG描述符介绍

> 1、定义与本质

> HOG特征描述子的定义：

> locally normalised histogram of gradient orientation in dense overlapping grids，即局部归一化的梯度方向直方图，是一种对图像局部重叠区域的密集型描述符, 它通过计算局部区域的梯度方向直方图来构成特征。

> Histogram of Oriented Gradient descriptors provide a dense overlapping description of image regions，即统计图像局部区域的梯度方向信息来作为该局部图像区域的表征。

> .

> 2、算法流程
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112239077971665.jpg)
> [ ](http://images.cnitblog.com/i/611502/201403/112239077971665.jpg)
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112239159668299.jpg)
> 解释一下：

> 灰度化：    由于颜色信息作用不大，通常转化为灰度图。

> 标准化gamma空间：

> 为了减少光照因素的影响，首先需要将整个图像进行规范化(归一化)，这种处理能够有效地降低图像局部的阴影和光照变化。

> 计算图像每个像素的梯度(包括大小和方向)：

> 计算图像横坐标和纵坐标方向的梯度，并据此计算每个像素位置的梯度方向值；求导操作不仅能够捕获轮廓，人影和一些纹理信息，还能进一步弱化光照的影响。

> 将图像分割为小的Cell单元格：

> 由于Cell单元格是HOG特征最小的结构单位，而且其块Block和检测窗口Win的滑动步长就是一个Cell的宽度或高度，所以，先把整个图像分割为一个个的Cell单元格(8*8像素)。

> 【重点】为每个单元格构建梯度方向直方图：

> 这步的目的是：统计局部图像梯度信息并进行量化（或称为编码），得到局部图像区域的特征描述向量。同时能够保持对图像中人体对象的姿势和外观的弱敏感性。

> 【重点】把单元格组合成大的块（block），块内归一化梯度直方图：

> 由于局部光照的变化以及前景-背景对比度的变化，使得梯度强度的变化范围非常大。这就需要对梯度强度做归一化。归一化能够进一步地对光照、阴影和边缘进行压缩。

> 生成HOG特征描述向量：    将所有“block”的HOG描述符组合在一起，形成最终的feature vector，该feature

> vector就描述了detect window的图像内容。

> .

> 3、图像归一化方法

> 四种归一化方法：

> 作者采用了四中不同的方法对区间进行归一化，并对结果进行了比较。引入v表示一个还没有被归一 化的向量，它包含了给定区间（block）的所有直方图信息。| | vk | |表示v的k阶范数，这里的k去1、2。用e表示一个很小的常数。这时，归一化因子可以表示如下：

> L2-norm：
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112241313937228.jpg)
> L1-norm：
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112241385917493.jpg)
> L1-sqrt：
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112241447027661.jpg)
> L2-Hys：它可以通过先进行L2-norm，对结果进行截短（clipping）（即值被限制为v -

> 0.2v之间），然后再重新归一化得到。

> 作者发现：采用L2- Hys，L2-norm 和 L1-sqrt方式所取得的效果是一样的，L1-norm稍微表现出一点点不可靠性。但是对于没有被归一化的数据来说，这四种方法都表现出来显着的改进。
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112241585225308.jpg)
> 区间(块)有两个主要的几何形状——矩形区间（R-HOG）和环形区间（C-HOG）。

> A、R-HOG区间（blocks）：大体上是一些方形的格子，它可以有三个参数来表征：每个区间中细胞单元的数目、每个细胞单元中像素点的数目、每个细胞的直方图通道数目。例如：行人检测的最佳参数设置是：3×3细胞/区间、6×6像素/细胞、9个直方图通道。则一块的特征数为：3*3*9；作者还发现，对于R-HOG，在对直方图做处理之前，给每个区间（block）加一个高斯空域窗口（Gaussian spatial window）是非常必要的，因为这样可以降低边缘的周围像素点（pixels around the edge）的权重。R-HOG是各区间被组合起来用于对空域信息进行编码（are used in conjunction to encode spatial form information）。
B、C-HOG区间（blocks）：有两种不同的形式，它们的区别在于：一个的中心细胞是完整的，一个的中心细胞是被分割的。如右图所示：
> 作者发现C-HOG的这两种形式都能取得相同的效果。C-HOG区间（blocks）可以用四个参数来表征：角度盒子的个数（number of angular bins）、半径盒子个数（number of radial bins）、中心盒子的半径（radius of the center bin）、半径的伸展因子（expansion factor for the radius）。通过实验，对于行人检测，最佳的参数设置为：4个角度盒子、2个半径盒子、中心盒子半径为4个像素、伸展因子为2。

> 前面提到过，对于R-HOG，中间加一个高斯空域窗口是非常有必要的，但对于C-HOG，这显得没有必要。C-HOG看起来很像基于形状上下文（Shape Contexts）的方法，但不同之处是：C-HOG的区间中包含的细胞单元有多个方向通道（orientation channels），而基于形状上下文的方法仅仅只用到了一个单一的边缘存在数（edge presence count）。

> .

> 4、 分块之间的相关性问题的解决

> 方案一：块重叠，重复统计计算

> 在重叠方式中，块与块之间的边缘点被重复根据权重投影到各自相邻块（block）中，从而一定模糊了块与块之间的边界，处于块边缘部分的像素点也能够给相邻块中的方向梯度直方图提供一定贡献，从而达到关联块与块之间的关系的作用。Datal对于块和块之间相互重叠程度对人体目标检测识别率影响也做了实验分析。
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112242275886057.png)
> 方案二：线性插值权重分配

> 有些文献采用的不是块与块重叠的方法，而是采用线性插值的方法来削弱混叠效应。这种方法的主要思想是每个Block都对临近的Block都有影响，这种影响，我们可以以一种加权方式附加上去。
![这里写图片描述](http://images.cnitblog.com/i/611502/201403/112242431357374.png)
> 基于线性插值的基本思想，对于上图四个方向（横纵两个45度斜角方向）个进行一次线性插值就可以达到权重分配目的。下面介绍一维线性插值。假设x1和x2是x块相邻两块的中心，且x1

> 二、OpenCV的HOG描述符实现

> 1、HOGDescriptor

> 作用：创造一个HOG描述子和检测器

> HOGDescriptor(Size win_size=Size(
> 64
> ,
> 128
> ), 
　　　　　　　　　　　　　　　　　　　　　　Size block_size=Size(
> 16
> ,
> 16
> ), 
　　　　　　　　　　　　　　　　　　　　　　Size block_stride=Size(
> 8
> ,
> 8
> ),
　　　　　　　　　　　　　　　　　　　　　　Size cell_size=Size(
> 8
> ,
> 8
> ),
> int
> nbins=
> 9
> ,
> double
> win_sigma=DEFAULT_WIN_SIGMA,
> double
> threshold_L2hys=
> 0.2
> ,
> bool
> gamma_correction=
> true
> ,
> int
> nlevels=DEFAULT_NLEVELS
　　　　　　　　　　　　　　　　　　　　　　)
> 参数注释：

> <1>win_size：检测窗口大小。

> <2>block_size：块大小，目前只支持Size(16, 16)。

> <3>block_stride：块的滑动步长，大小只支持是单元格cell_size大小的倍数。

> <4>cell_size：单元格的大小，目前只支持Size(8, 8)。

> <5>nbins：直方图bin的数量(投票箱的个数)，目前每个单元格Cell只支持9个。

> <6>win_sigma：高斯滤波窗口的参数。

> <7>threshold_L2hys：块内直方图归一化类型L2-Hys的归一化收缩率

> <8>gamma_correction：是否gamma校正

> <9>nlevels：检测窗口的最大数量

> 2、getDescriptorSize函数

> 作用：获取一个检测窗口的HOG特征向量的维数

> 3、getBlockHistogramSize函数

> 作用：获取块的直方图大小

> 4、setSVMDetector 函数

> 作用：设置线性SVM分类器的系数

> 5、getDefaultPeopleDetector 函数

> (1)作用：获取行人分类器(默认检测窗口大小)的系数(获得3780维检测算子)

> 6、getPeopleDetector48x96 函数

> 作用：获取行人分类器(48*96检测窗口大小)的系数

> 7、getPeopleDetector64x128 函数

> 作用：获取行人分类器(64*128检测窗口大小)的系数

> 8、detect 函数

> (1)作用：用没有多尺度的窗口进行物体检测

> (2)函数原型：

> C++:
> void
> gpu::HOGDescriptor::detect(
> const
> GpuMat& img,
> vector
> <Point>
> & found_locations,
> double
> hit_threshold=
> 0
> , 
　　　　　　　　　　　　　　　　　　　　　Size win_stride=Size(), 
　　　　　　　　　　　　　　　　　　　　　Size padding=Size()
　　　　　　　　　　　　　　　　　　　　　)
> 参数注释：

> <1>img：源图像。只支持CV_8UC1和CV_8UC4数据类型。

> <2>found_locations：检测出的物体的边缘。

> <3>hit_threshold：特征向量和SVM划分超平面的阀值距离。通常它为0，并应由检测器系数决定。但是，当系数被省略时，可以手动指定它。

> <4>win_stride：窗口步长，必须是块步长的整数倍。

> <5>padding：模拟参数，使得CUP能兼容。目前必须是(0,0)。

> 9、detectMultiScale 函数(需有setSVMDetector)

> (1)作用：用多尺度的窗口进行物体检测

> (2)函数原型：

> C++:
> void
> gpu::HOGDescriptor::detectMultiScale(
> const
> GpuMat& img,
> vector
> <Rect>
> & found_locations,
> double
> hit_threshold=
> 0
> , 
　　　　　　　　　　　　　　　　　　　　　　　　　　Size win_stride=Size(), 
　　　　　　　　　　　　　　　　　　　　　　　　　　Size padding=Size(),
> double
> scale0=
> 1.05
> ,
> int
> group_threshold=
> 2
> )
> (3)参数注释

> <1>img：源图像。只支持CV_8UC1和CV_8UC4数据类型。

> <2>found_locations：检测出的物体的边缘。

> <3>hit_threshold：特征向量和SVM划分超平面的阀值距离。通常它为0，并应由检测器系数决定。但是，当系数被省略时，可以手动指定它。

> <4>win_stride：窗口步长，必须是块步长的整数倍。

> <5>padding：模拟参数，使得CUP能兼容。目前必须是(0,0)。

> <6>scale0：检测窗口增长参数。

> <7>group_threshold：调节相似性系数的阈值。检测到时，某些对象可以由许多矩形覆盖。 0表示不进行分组。

> 10、getDescriptors 函数

> (1)作用：返回整个图片的块描述符 (主要用于分类学习)。

> (2)函数原型：

> C++:
> void
> gpu::HOGDescriptor::getDescriptors(
> const
> GpuMat& img, 
　　　　　　　　　　　　　　　　　　　　　　　　　　Size win_stride, 
　　　　　　　　　　　　　　　　　　　　　　　　　　GpuMat& descriptors,
> int
> descr_format=DESCR_FORMAT_COL_BY_COL
　　　　　　　　　　　　　　　　　　　　　　　　　　)
> (3)参数注释

> <1>img：源图像。只支持CV_8UC1和CV_8UC4数据类型。

> <2>win_stride：窗口步长，必须是块步长的整数倍。

> <3>descriptors：描述符的2D数组。

> <4>descr_format：描述符存储格式：

> DESCR_FORMAT_ROW_BY_ROW - 行存储。

> DESCR_FORMAT_COL_BY_COL - 列存储。

> 11、computeGradient 函数

> (1)作用：计算img经扩张后的图像中每个像素的梯度和角度

> (2)函数原型：

> void
> HOGDescriptor::computeGradient(
> const
> Mat& img, 
Mat& grad, 
Mat& qangle,
Size paddingTL, 
Size paddingBR
)
> const
> (3)参数注释

> <1>img：源图像。只支持CV_8UC1和CV_8UC4数据类型。

> <2>grad：输出梯度（两通道），记录每个像素所属bin对应的权重的矩阵，为幅值乘以权值。这个权值是关键，也很复杂：包括高斯权重，三次插值的权重，在本函数中先值考虑幅值和相邻bin间的插值权重。

> <3>qangle：输入弧度（两通道），记录每个像素角度所属的bin序号的矩阵,均为2通道,为了线性插值。

> <4>paddingTL：Top和Left扩充像素数。

> <5>paddingBR：Bottom和Right扩充像素数。

> 12、compute 函数

> (1)作用：计算HOG特征向量

> (2)函数原型：

> void
> HOGDescriptor::compute(
> const
> Mat& img,
> vector
> <
> float
> >
> & descriptors,
　　　　　　　　　　　　　　　　Size winStride, 
　　　　　　　　　　　　　　　　Size padding,
> const
> vector
> <Point>
> & locations
　　　　　　　　　　　　　　　　)
> const
> (3)参数注释

> <1>img：源图像。只支持CV_8UC1和CV_8UC4数据类型。

> <2>descriptors：返回的HOG特征向量，descriptors.size是HOG特征的维数。

> <3>winStride：窗口移动步长。

> <4>padding：扩充像素数。

> <5>locations：对于正样本可以直接取(0,0),负样本为随机产生合理坐标范围内的点坐标。


