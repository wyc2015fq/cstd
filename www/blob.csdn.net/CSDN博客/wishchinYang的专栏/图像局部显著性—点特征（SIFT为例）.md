# 图像局部显著性—点特征（SIFT为例） - wishchinYang的专栏 - CSDN博客
2015年04月20日 11:32:33[wishchin](https://me.csdn.net/wishchin)阅读数：11253
        基于古老的Marr视觉理论，视觉识别和场景重建的基础即第一阶段为局部显著性探测。探测到的主要特征为直觉上可刺激底层视觉的局部显著性——特征点、特征线、特征块。
        SalientDetection 已经好就没有复习过了，DNN在识别领域的超常表现在各个公司得到快速应用，在ML上耗了太多时间，求职时被CV的知识点虐死...
**点探测总结（SIft、PCA-SIft、Surf、GLOH、Brief、Brisk、ORB、Freak）**
       特征点寻找的准则之一是算法的通用准则—泛化性能，即在一个场景中中适用，在另一个场景中也具有相似的效果。而在CV领域，专门对于图像处理问题，应对图像的缩放、平移、旋转、明暗问题，进而提出了特征描述算法的相应要求：缩放不变性、平移不变性、旋转不变性和光照不变性。
       广受好评SIFT特征在综合评价上几乎达到算法极限，其中图像金字塔、计算图像梯度寻找主方向、梯度归一化，分别应对缩放不变、旋转不变、和光照不变，而同时局部特征用于模式识别不需要考虑相对平移的影响。参考 原文链接（SIFT）：[http://www.cnblogs .com/cfantaisie/archive/2011/06/14/2080917.html](http://www.cnblogs.com/cfantaisie/archive/2011/06/14/2080917.html) ，文章有修改！为个人意见。
       参考以前的一篇：[Sift算法总结：用于图像搜索](http://blog.csdn.net/wishchin/article/details/18319477)
       推荐搜索系列：[http://blog.csdn.net/cserchen/article/category/785155](http://blog.csdn.net/cserchen/article/category/785155)**。论文下载见文章末尾的参考资料链接**
** 几个主要的特征点算法年代发展表：**
        1.  1999年的SIFT（ICCV 1999，并改进发表于IJCV 2004，本文描述），尺度非相关特征提取；
        2.  2005年的[GLOH](http://blog.csdn.net/wishchin/article/details/60963562) 特征（2005年的PAMI期刊）；GLOH采用圆形划分取代SIFT的正方形区间划分。
       3.  2006年的[SURF](http://blog.csdn.net/wishchin/article/details/62417331)（2006年的ECCV）；SURF算法是对SIFT算法加强版，同时加速的具有鲁棒性的特征。第二、标准的SURF算子比SIFT算子快好几倍，并且在多幅图片下具有更好的鲁棒性。SURF最大的特征在于采用了harr特征以及积分图像integral image的概念，这大大加快了程序的运行速度。
         4.  2010年的[Brief](http://blog.csdn.net/wishchin/article/details/50161641)特征（ECCV2010 ）； Brief:Binary Robust Independent Elementary Features.
       5.  2011年的brisk算法（ICCV2011）；BRISK算法主要利用FAST9-16进行特征点检测（为什么是主要？因为用到一次FAST5-8），可参见博客：[FAST特征点检测算法](http://blog.csdn.net/hujingshuang/article/details/46898007)。要解决尺度不变性，就必须在尺度空间进行特征点检测，于是BRISK算法中构造了图像金字塔进行多尺度表达。
        补充：2006年提出的[fast](http://blog.csdn.net/hujingshuang/article/details/46898007)作为一种角点检测的方法，不涉及特征子描述。
        6.  2011年的[ORB](http://blog.csdn.net/wishchin/article/details/50161641)特征（ICCV2011）；ORB是是ORiented Brief 的简称，对Brief的特定性质进行了改进。
        7.  2012年的[freak](http://blog.csdn.net/hujingshuang/article/details/47060677)算法（CVPR2012）；[Fast Retina Keypoint 论文](http://infoscience.epfl.ch/record/175537/files/2069.pdf)。与brisk算法有相似之处，是在BRISK算法上的改进。
**SIFT特征**
       Sift是David Lowe于1999年提出的局部特征描述子，并于2004年进行了更深入的发展和完善。Sift特征匹配算法可以处理两幅图像之间发生平移、旋转、仿射变换情况下的匹配问题，具有很强的匹配能力。在Mikolajczyk对包括Sift算子在内的十种局部描述子所做的不变性对比实验中，Sift及其扩展算法已被证实在同类描述子中具有最强的健壮性。
**一、Sift特征点生成的主要步骤 **
**         1)、尺度空间的生成；**
**         2)、检测尺度空间极值点；**
**         3)、精确定位极值点；**
**         4)、为每个关键点指定方向参数；**
**         5)、关键点描述子的生成。**
** 1.尺度空间概念：在一个合适的范围内，使用高斯卷积核模拟图像的多尺度化**
![](http://pic002.cnblogs.com/images/2011/266172/2011061421002312.png)
![](http://pic002.cnblogs.com/images/2011/266172/2011061422084343.png)
　　　　　　　　　　　　　 L(x,y,σ), σ= 1.6 a good tradeoff
![](http://pic002.cnblogs.com/images/2011/266172/2011061422101523.png)
　　　　　　　　　　　　   D(x,y,σ), σ= 1.6 a good tradeoff
![](http://pic002.cnblogs.com/images/2011/266172/2011061420034681.jpg)
![](http://pic002.cnblogs.com/images/2011/266172/2011061423070164.png)
关于尺度空间的理解说明：图中的2是必须的，尺度空间是连续的。
        注释：高斯核通过调整核的参数，具有缩放到多尺度空间保持信息稳定的性质，但本质上需要图像像素性质遵循高斯分布，而在大多数情况下，这种情况是满足的。
        在 Lowe的论文中 ，  将第0层的初始尺度定为1.6，图片的初始尺度定为0.5. 在检测极值点前对原始图像的高斯平滑以致图像丢失高频信息，所以 Lowe 建议在建立尺度空间前首先对原始图像长宽扩展一倍，以保留原始图像信息，增加特征点数量。尺度越大图像越模糊。
![](http://pic002.cnblogs.com/images/2011/266172/2011061421011354.png)
**next octave 是由 first octave 降采样得到（如2**）
![](http://pic002.cnblogs.com/images/2011/266172/2011061501540067.png)，![](http://pic002.cnblogs.com/images/2011/266172/2011061501563284.png)
　　　　　　　　　　  尺度空间的所有取值，s为每组层数，一般为3~5
**2、检测空间极值点**
![](http://pic002.cnblogs.com/images/2011/266172/2011061421014812.png)
同一组中的**相邻尺度（由于k的取值关系，肯定是上下层）**之间进行寻找
![](http://pic002.cnblogs.com/images/2011/266172/2011061502345978.png)
在极值比较的过程中，每一组图像的首末两层是无法进行极值比较的，**为了满足尺度变化的连续性，**我们在每一组图像的顶层继续用高斯模糊生成了 3 幅图像，高斯金字塔有每组S+3层图像。DOG金字塔每组有S+2层图像.
**3、精确定位极值点**
![](http://pic002.cnblogs.com/images/2011/266172/2011061421040745.png)
![](http://pic002.cnblogs.com/images/2011/266172/2011061502413390.png)
  　 　　  If ratio > (r+1)2/(r), throw it out (SIFT uses r=10)
![](http://pic002.cnblogs.com/images/2011/266172/2011061502441120.png)表示DOG金字塔中某一尺度的图像x方向求导两次
**通过拟和三维二次函数以精确确定关键点的位置和尺度（达到亚像素精度）？**
** 4、主方向的确定**
![](http://pic002.cnblogs.com/images/2011/266172/2011061421062893.png)
**直方图中的峰值就是主方向，其他的达到最大值80%的方向可作为辅助方向**
![](http://pic002.cnblogs.com/images/2011/266172/2011061421442969.jpg)
**     Identify peak and assign orientation and sum of magnitude to key point The user may choose a threshold to exclude key points based on their assigned sum of magnitudes.**
       利用关键点邻域像素的**梯度方向分布特性**为每个关键点指定方向参数，使算子具备旋转不变性。以关键点为中心的邻域窗口内采样，并用直方图统计邻域像素的梯度方向。梯度直方图的范围是0～360度，其中每10度一个柱，总共36个柱。随着距中心点越远的领域其对直方图的贡献也响应减小.Lowe论文中还提到要使用高斯函数对直方图进行平滑，减少突变的影响。
![](http://pic002.cnblogs.com/images/2011/266172/2011061423362721.png)
**关键点描述子的生成步骤**
![](http://pic002.cnblogs.com/images/2011/266172/2011061420184544.png)
通过对关键点周围图像区域分块，计算块内梯度直方图，生成具有独特性的向量，这个向量是该区域图像信息的一种抽象，具有唯一性**。5、特征描述子生成**
![](http://pic002.cnblogs.com/images/2011/266172/2011061421064737.png)
      每一个小格都代表了特征点邻域所在的尺度空间的一个像素 ，箭头方向代表了像素梯度方向，箭头长度代表该像素的幅值。然后在4×4的窗口内计算8个方向的梯度方向直方图。**绘制每个梯度方向的累加可形成一个种子点。**
**![](http://pic002.cnblogs.com/images/2011/266172/2011061502585349.png)**
       每个直方图有8方向的梯度方向，每一个描述符包含一个位于关键点附近的四个直方图数组.**这就导致了SIFT的特征向量有128维.**（先是一个4×4的来计算出一个直方图，每个直方图有8个方向。所以是4×4×8=128维）将这个**向量归一化之后，就进一步去除了光照的影响。**
![](http://pic002.cnblogs.com/images/2011/266172/2011061421445712.png)![](http://pic002.cnblogs.com/images/2011/266172/2011061422154937.jpg)
****                                                                                                                                                     旋转为主方向****
** 6.计算过程综述：**
![](http://pic002.cnblogs.com/images/2011/266172/2011061503125847.png)
![](http://pic002.cnblogs.com/images/2011/266172/2011061503133120.png)
![](http://pic002.cnblogs.com/images/2011/266172/2011061503140831.png)
![](http://pic002.cnblogs.com/images/2011/266172/2011061503151478.png)
![](http://pic002.cnblogs.com/images/2011/266172/2011061503171726.png)
### **二、Sift特征总结（概念补充）**
**什么是局部特征？**
　　•局部特征从总体上说是图像或在视觉领域中一些有别于其周围的局部地方，约束是Hash特性；
　　•局部特征通常是描述一块区域，使其能具有高可区分度，意味着局部显著性；
　　•局部特征的好坏直接会决定着后面使用局部特征进行分类、识别是否会得到一个好的结果。
**局部特征需具备的特性？**
　　•重复性，这是算法的一般特性，默认遵循hash性质。
　　•可区分性，可分离性，用于模式分类的一般基准。
　　•准确性，描述准确性，即hash映射的直观性能，即一定保证近邻哈希。
　　•数量以及效率，hash压缩函数分布的均匀性，此性能影响到后续的学习效果。
　　•不变性：hash映射的近邻特性，这标识了近邻hash函数本质要求，即近邻的点在映射到另一个空间也应该是近邻。
**局部特征提取算法-SIFT**
　　•SIFT算法由D.G.Lowe 1999年提出，2004年完善总结。后来Y.Ke将其描述子部分用PCA代替直方图的方式，对其进行改进。
 　　•SIFT算法是一种提取局部特征的算法，在尺度空间寻找极值点，提取位置，尺度，旋转不变量
　　•**SIFT特征**是图像的局部特征，其对旋转、尺度缩放、亮度变化保持不变性，对视角变化、仿射变换、噪声也保持一定程度的稳定性。
　　•独特性好，信息量丰富，适用于在海量特征数据库中进行快速、准确的匹配。
　　•多量性，即使少数的几个物体也可以产生大量**SIFT特征**向量，这涉及到fast角点检测的要求。
　　•可扩展性，可以很方便的与其他形式的特征向量进行联合。
**尺度空间理论**
　　•尺度空间理论目的是模拟图像数据的多尺度特征
　　•其基本思想是在视觉信息图像信息处理模型中引入一个被视为尺度的参数, 通过连续变化尺度参数获得不同尺度下的视觉处理信息, 然后综合这些信息以深入地挖掘图像的本质特征。
**描述子生成的细节**
　　•以极值点为中心点，并且以此点所处于的高斯尺度sigma值作为半径因子。对于远离中心点的梯度值降低对其所处区域的直方图的贡献，防止一些突变的影响。
　　•每个极值点对其进行三线性插值，这样可以把此极值点的贡献均衡的分到直方图中相邻的柱子上
**归一化处理**
　　•在求出4*4*8的128维特征向量后，此时**SIFT特征**向量已经去除了尺度变化、旋转等几何变形因素的影响。而图像的对比度变化相当于每个像素点乘上一个因子，光照变化是每个像素点加上一个值，但这些对图像归一化的梯度没有影响。因此将特征向量的长度归一化，则可以进一步去除光照变化的影响。
　　•对于一些非线性的光照变化，SIFT并不具备不变性，但由于这类变化影响的主要是梯度的幅值变化，对梯度的方向影响较小，因此作者通过限制梯度幅值的值来减少这类变化造成的影响。
**PCA-SIFT算法**
　　• PCA-SIFT与标准SIFT**有相同的 亚像素位置 ，尺度 和 主方向。但在第4步 计算描述子的 设计，采用的 主成分分析 的技术。**
　　•下面介绍一下其特征描述子计算的部分：
　　　　•不用原先的4*4*8的特征描述，而用特征点周围的41×41的像斑计算它的主元，并用**PCA-SIFT将原来的2×39×39（或者XY两个方向）即3042维的向量降成20或X维**，以达到更精确的表示方式。
　　　　•它的主要步骤为，对每一个关键点：在关键点周围提取一个41×41的像斑于给定的尺度，旋转到它的主方向 ；计算39×39水平和垂直的梯度，形成一个大小为3042的矢量；用预先计算好的投影矩阵n×3042与此矢量相乘；这样生成一个**大小为n**的PCA-SIFT描述子。
       注意：主成分分析的劣势在于其真正地实现局部空间压缩，所以其泛化性能也一定会产生损失。
**三、工程师角度理解SIFT（目的驱动）**
SIft的主要价值在于：
        缩放不变性：这一点 以来尺度空间理论来解决，使用 高斯滤波 理论上可以解决尺度上的差异。在特征点检测过程中，受到尺度的影响不大；在特征点的描述上，仍然要考虑到尺度对特征向量的影响。
        旋转不变性：通过对关键点指定主方向来实现。在检测到的特征点邻域内，寻找局部的梯度方向，作为主方向即世界坐标轴。图像局部旋转后以世界坐标轴标定，可以重新对齐，即实现旋转不变。
        特征点位置确定：位置确定是Sift特征算法可以改进的地方，并且应对与不同的场景要使用不同的方法，即不是sift特征的特异之处。
        PCA—sift：在超量图像检测的场景下，PCA化并不一定能取得好的效果，也有可能适得其反。
**四、点-区块检测对比总结：**
        参考链接：[特征提取方法 SIFT,PCA-SIFT,GLOH,SURF](http://blog.csdn.net/abcjennifer/article/details/7681718)对比总结
        总结论文：A comparison of SIFT, PCA-SIFT and SURF 对三种方法给出了性能上的比较，源图片来源于Graffiti datase，对原图像进行尺度、旋转、模糊、亮度变化、仿射变换等变化后，再与原图像进行匹配，统计匹配的效果。效果以可重复出现性为评价指标。
|**method**|**Time**|**Scale**|**Rotation**|**Blur**|**Illumination**|**Affine**|
|----|----|----|----|----|----|----|
|**Sift**|common|best|best|common|common|good|
|**PCA-sift**|good|good|good|best|good|best|
|**Surf**|best|common|common|good|best|good|
        由此可见，SIFT在尺度和旋转变换的情况下效果最好，SURF在亮度变化下匹配效果最好，在模糊方面优于SIFT，而尺度和旋转的变化不及SIFT，旋转不变上比SIFT差很多。速度上看，SURF是SIFT速度的3倍。
         采用NN作为匹配策略的特征描述子性能测评结果：
![](https://img-my.csdn.net/uploads/201206/21/1340246687_9646.jpg)
**五、总结**
         暂无...............................
参考资料：
Sift Wiki百科：[https://en.wikipedia.org/wiki/Scale-invariant_feature_transform](https://en.wikipedia.org/wiki/Scale-invariant_feature_transform)
     Sift 原始论文：Lowe, David G. (1999).["Object recognition from local scale-invariant features"](http://www.cs.ubc.ca/~lowe/papers/iccv99.pdf) (PDF). *Proceedings of the International Conference on Computer Vision*.**2**. pp. 1150–1157.[doi](https://en.wikipedia.org/wiki/Digital_object_identifier):[10.1109/ICCV.1999.790410](https://dx.doi.org/10.1109%2FICCV.1999.790410).
      Sift 改进和发展：Lowe, David G. (2004).["Distinctive Image Features from Scale-Invariant Keypoints"](http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf). *International Journal of Computer Vision*.**60** (2): 91–110.[doi](https://en.wikipedia.org/wiki/Digital_object_identifier):[10.1023/B:VISI.0000029664.99615.94](https://dx.doi.org/10.1023%2FB%3AVISI.0000029664.99615.94).
 GLOH Wiki百科: [https://en.wikipedia.org/wiki/GLOH](https://en.wikipedia.org/wiki/GLOH)
     GLOH原始论文: [Krystian Mikolajczyk and Cordelia Schmid "A performance evaluation of local descriptors", IEEE Transactions on Pattern Analysis and Machine Intelligence, 10, 27, pp 1615--1630, 2005.](http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1498756&isnumber=32189)
 Surf 原始论文: Herbert Bay, Andreas Ess, Tinne Tuytelaars, and Luc Van Gool, "[Speeded Up Robust Features](http://www.vision.ee.ethz.ch/en/publications/papers/articles/eth_biwi_00517.pdf)", ETH Zurich, Katholieke Universiteit Leuven( SURF was first presented by[Herbert Bay](https://en.wikipedia.org/wiki/Herbert_Bay), et al., at the 2006[European Conference on Computer Vision](https://en.wikipedia.org/wiki/European_Conference_on_Computer_Vision).).
BRISK 原始论文: 《[BRISK:Binary Robust Invariant Scalable Keypoints](http://www.robots.ox.ac.uk/~vgg/rg/papers/brisk.pdf)》ICCV2011文章
Freak  原始论文:《[FREAK: Fast Retina Keypoint](http://infoscience.epfl.ch/record/175537/files/2069.pdf)》CVPR2012
Surf 原始论文:Herbert Bay, Andreas Ess, Tinne Tuytelaars, Luc Van Gool, "[SURF: Speeded Up Robust Features](http://www.vision.ee.ethz.ch/~surf/eccv06.pdf)", Computer Vision and Image Understanding (CVIU), Vol. 110, No. 3, pp. 346--359, 2008.
