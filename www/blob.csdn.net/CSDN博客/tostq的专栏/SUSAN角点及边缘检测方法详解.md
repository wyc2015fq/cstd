# SUSAN角点及边缘检测方法详解 - tostq的专栏 - CSDN博客





2015年10月21日 15:50:57[tostq](https://me.csdn.net/tostq)阅读数：8141








SUSAN检测方法是一种基于窗口模板的检测方法，主要是通过在图像每个像素点位置处建立一个窗口，这个窗口是圆形的，这里为了得到各方向同性的响应，窗口内可以是常数权值或高斯权值，一般情况下，窗口半径为3.4个像素（即窗口内总有37个像素）。

这样的窗口模板被放置在每个像素的位置，确定点之间强度相似程度可以由下面的图来描述，这里的x轴是指像素点之间强度差别，y轴指的相似程度，为1就指完全相似。

![](https://img-blog.csdn.net/20151021155133505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151021155158053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


考虑为了运算简单，一般情况下使用a线（上图），最后统计所以同中心点相似的点数（即相似程度为1的点）相似点所在区域被称为USAN（Univalue Segment Assimilating Nucleus），而特征边缘或角点就是在这个点数值局部最小地方。图中的分界点（区分是否相似的像素差别）实际上反映了图像特征的最小对比程度，及被排除噪声的最大数目。

SUSAN边缘及角点检测没有用到图像像素的梯度（导数），因为这个原因，所以即使在噪声存在情况下，也能有好的表现。因为只要噪声足够的少，没有包含到所有相似像素的点，那么噪声就可以被排除。在计算时，单个值的集合统一消除了噪声的影响，所以SUSAN对于噪声具有较好的鲁棒性。

SUSAN是通过比较像素点邻域同其中心相似程度，所以也具有了光强变化不变性（因为像素点差别不会变化），及旋转不变性（旋转不会改变局部像素间的相似程度），及一定程度的尺度不变性（角点的角度在尺度放大并不会变化，这里的一定程度是指在尺度放大时，局部的曲度会慢慢平滑）。另外SUSAN使用的参数也非常的少，所以对于计算量及储存量要求低。

SUSAN方法被提出来，主要是为针对于边缘检测和角点检测的，然后其对于噪声较高的鲁棒性，也会用于在噪声消除中，被用于去选择最佳局部平滑邻域（相似程度的点最多的地方）。本文重点描述SUSAN方法用于边缘检测和角点检测的思路，并简要介绍下SUSAN方法是如何应用于噪声消除领域的。



**一、SUSAN边缘检测**

**（1）边缘响应的计算**

首先考虑到图像以每点像素为中心的圆形模板（半径为3.4个像素，模板内共有37个像素），对于其内的每个邻域点都作如下相似度衡量的计算（标准是上图的a线），这里的r是领域像素距离中心的长度，而r0是中心位置，t指相似度分界值（其决定了特征同背景的最小区别程度，及最大可被排除的噪声数）。

![](https://img-blog.csdn.net/20151021155335862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


当然我们也可以用平滑的线来从代替这种直接的分割方式（如下图b线），这样可以获得更稳定而敏感的结果，虽然计算复杂但是可以通过查找表来获得较快的速度。公式如下：

![](https://img-blog.csdn.net/20151021155350401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


并计算总共的相似程度：

![](https://img-blog.csdn.net/20151021155406697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


接下来，将n同一个固定的阈值g比较（一般设置为最大同中心相似点数nmax的0.75倍左右），初始的边缘响应可以用下面等式计算：

![](https://img-blog.csdn.net/20151021155424608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里g是为了排除噪声的影响，而当n小于这个阈值时，才能被考虑其的边缘是否是边缘，n越小，其边缘响应就越大。如果一个阶跃边缘被考虑，如果这个点在边缘上，那么其USAN的值应当是小于或等于0.5倍的最大值，而对于曲线边缘来说，这个值应该会更少（因为曲度会更小，所以在窗口内，曲线内的区域会更小），所以g的加入不会影响原来的边缘检测的结果的（即所得的边缘响应R是不会为0的）。

**（2）边缘方向的计算**

为什么要计算边缘方向呢？首先，非极大值抑制需要找到边缘方向（这个之后再解释），另外确定子像素级精度也需要找到边缘方向（这个好理解），最后，一些应用可以会用到边缘方向（也包括位置及长度），一个像素若其边缘长度不会0，那么便有边缘方向，边缘一般有两种情况：

![](https://img-blog.csdn.net/20151021155506748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


一种是标准的边缘点（如同中的a和b，其刚好位于边缘的一边或另一边），而c是另一种情况，其刚好位于左右两边的过渡带间，而过渡带的强度恰好为左右像素强度的一半。这里还引入了一个概念，USAN的引力中心（USAN区域即图像模板内的白色区域，就是同中心相似的区域），其可以由下式计算：

![](https://img-blog.csdn.net/20151021155559626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中a和b情况可以被视一种像素间边缘，引力中心同模板中心的向量恰好垂直于局部边缘方向，我们可以通过这种方式来找到其边缘方向。

而上图c的情况就完全不一样了，其引力中心同模板中心恰好在一点上（这被视为一种像素内边缘），此时边缘的方向就要找到USAN区域内最长的对称轴方向，这个可以通过如下公式来获得：

![](https://img-blog.csdn.net/20151021155621658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20151021155635771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


前两项的比值可以用于确定边缘的方向（度数），而后一项的符号用于确定一个倾斜边缘其梯度的符号（往那边倾斜）。

接下来的问题就是如何自动决定每点是属于以上哪种的边缘，首先如果USAN区域（像素数）比模板直径（像素数）要小，那么这应该是像素内边缘（如c）。如果要大的话，然后又找到USAN的引力中心，那么这应该是像素间边缘情况（如a和b）。如果引力中心同模板中心的距离小于1个像素的话，此时用于像素内边缘情况（如c）来计算更为精确。

**（3）非极大值抑制**

**（4）子像素精度**

对于每一个边缘点，首先找到边缘方向，然后在边缘垂直方向上减少边缘宽度，之后剩下的边缘点做3点二次曲线拟合，而曲线的转折点（一般情况下同最初边缘点的距离应当少于半个像素）被视为边缘的精确位置。

**（5）检测位置并不依赖于窗口大小**

即窗口模板增大，不会改变检测的边缘的位置，即SUSAN具有尺度不变性，另外其对于视点改变后的图像检测重复率也非常高，因为视点变化不会影响其角点是否存在。



**二、SUSAN角点检测方法**

角点检测方法同其边缘检测方法非常类似，其使用相同的方法来计算每个像素点的同圆形窗口内的相似程度n，然后n也是同一个权值g来比较，不过这里的g只是控制噪声存在数目，而需要考虑边缘，所以g的值为nmax的一半就可以了。

在特征检测方法里，我们一般需要至少一个阈值来区分特征同非特征，而这个阈值的选择将极大影响了最终特征提取的结果。一般情况下，这里有两种类型的阈值：一个衡量质量，一个衡量数量。SUSAN方法里的t和g恰好分别符合这两种情况，g控制角点检测的质量，如果g越小，那么我们得到的角点就更尖锐，而t则控制角点数目，如果t越小，那么我们检测的角点数目就越小，然后t却不会影响最终角点质量。（这段是题外话了>_<）

在计算角响应度后，而在非极大值抑制之前，我们可以排除一些在边缘或噪声位置的误检测角点，比如一条倾斜率比较小的直线，因为边缘线至少要一个像素宽度，所以会在直线上形成不连续断裂，这些断裂的位置可能会形成被误检的角点（如上图的c）。

**（1）排除误检的角点**

首先找到USAN的引力中心，然后计算引力中心同模板中心的距离，当USAN恰好能指示一个正确角点时，其引力中心同模板中心将不会靠得太近，而如果是条细线的话，引力中心同模板中心将会很近（如上图a和b所示）。另外的操作就是要加强USAN的邻近连续性，在真实图像里，非常容易出现小的噪声点，而这些噪声点可能分布于USAN内。所以对于模板内所有的像素，如果其是位于引力中心同模板中心所连的直线上，那么都应该视为USAN的一部分。

**（2）非极大值抑制**

SUSAN角点检测方法同基于导数检测方法一个非常大的优势在于，其不会在靠近中心相邻的区域不会与中心区域的角响应很难区分，所以局部的非极大值抑制中需要简单地选择局部的最大值就可以了。



**三、SUSAN噪声滤波方法**

SUSAN噪声滤波方法主要是通过仅平滑那些同中心像素相似的区域（即USAN），而由此保留图像的结构信息。而USAN的平滑主要就是找到其中所以像素的平均值，而不会对其相邻的不相关区域进行操作。

不过在区分相似度的公式上，却没有用到原来的公式，而是如下：

![](https://img-blog.csdn.net/20151021155752473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


滤波器的最终公式为：

![](https://img-blog.csdn.net/20151021155808228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**参考文献**

**[SUSAN—ANew Approach to Low Level Image Processin](http://download.csdn.net/detail/tostq/9200095)**






