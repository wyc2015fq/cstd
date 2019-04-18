# VC数字图像处理编程讲座之十 - 深之JohnChen的专栏 - CSDN博客

2006年02月20日 10:01:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5955标签：[图像处理																[编程																[算法																[扩展																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)


VC数字图像处理编程讲座之十
                                                                                                                                 cndg

**图像几何特性分析**

一、物体边界计算和记数

　　（一）邻接和连通

　　邻接和连通是图像的基本几何特性之一，主要研究象素或由象素构成的目标物之间的关系.对数字图像来讲首先要搞清楚图像阵列的结构形式.下面均以方格阵为基础来讨论分析.

　　1.邻点

　　对图像中一点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image002.gif) ，称它的上下左右四点为4—邻点定义的邻点.若再加上两个对角线的四个端点（左上、右上、左下、右下）则称这八个点为8—邻点定义的临点。见图 8-6-1。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image003.gif)
图 8-6-1　临点的概念

　　2.邻接

　　设A和B为图像的两个子集，若A中至少有一点，其临点在B内，称A和B邻接。显然有和8—邻点邻接两个概念。两个子集是8—邻点邻接不一定是4—邻点邻接。如图 8-6-2所示。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image004.gif)
图 8-6-2　8—邻点邻接而4—邻点不邻接示例

　　3.路径（道路）

　　一条从点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image006.gif) 到点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image008.gif) 的路径是满足下列关系的点的序列。 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image010.gif) 其中彼此靠近的两点邻接。 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image012.gif) 为路径的长度。同样存在4-邻点路径和8-邻点路径。如图 8-6-3

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image013.gif)
图 8-6-3　路径示例

　　4.连通

　　若点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image015.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image017.gif) 为图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image019.gif) 中的点，且存在一条从 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image020.gif) 到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image021.gif) 的完全由 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image022.gif) 中点构成的路径，则称 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image023.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image024.gif) 是连通的。对于 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image025.gif) 中的任一点，所有连通到它的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image027.gif) 中点的集合称为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image028.gif) 中的连通分量。若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image029.gif) 只有一个连通分量，称 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image030.gif) 是连通的。显然也有4-邻点连通和8-邻点连通的区别。如图 8-6-4所示。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image031.gif)
图 8-6-4　连通示例

　　5.背景和孔

　　设 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image033.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image034.gif) 的补集，凡是连通到图像边缘的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image036.gif) 中所有点都属于 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image037.gif) 的同一连通分量，称这个分量为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image038.gif) 的背景B。而 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image039.gif) 中所有其他分量称为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image040.gif) 的孔。若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image041.gif) 中没有孔，称此 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image042.gif) 为单连通。要注意，在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image043.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image044.gif) 中采用不同的邻点概念来定义其连通性可以避免一些不合理的现象。如 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image046.gif) 这样一个图案，对角两个1是连通的，而另一个对角两个0就不能连通，反之亦然。但若对1和0即 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image047.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image048.gif) 都采用8-邻点连通，那么它们两者都连通，这就产生了矛盾。因此，必须对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image049.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image050.gif) 用不同的邻点概念来分析研究其连通特性。

　　6.包围

　　设 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image051.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image053.gif) 是图像中两个不相交的子集，若从 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image054.gif) 中的任一点到达图像边缘的任一条路径必须与 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image055.gif) 相遇，则称 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image056.gif) 包围 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image057.gif) ，或称 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image058.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image059.gif) 内。

　　（二）边界和边界的计算

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image060.gif) 的边界 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image062.gif) 是指在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image063.gif) 中有邻点的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image064.gif) 中点的集合。 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image065.gif) 中去掉 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image066.gif) 的其余部分点的集合称其为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image067.gif) 的内部，即为差集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image069.gif) 。边界研究同样有4-邻点和8-邻点两种定义方法。

　　1.边界的并行计算

　　（１）设有一幅图像见8-6-5（a），其中1的集合代表物体 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image070.gif) 。对图像进香逻辑非运算获得图8-6-5（b）。

　　（２）将图 8-6-5（b）在上下左右四个方向上移动一个点的位置，获得图 8-6-5（c）中四个图像。（对8-邻点定义，还要在四个角方向上移动四次，得出8个图像）。

　　（３）将图 8-6-5（c）中四个图像进行逻辑或运算得到图8-6-5（d）。

　　（４）将图 8-6-5（d）和图 8-6-5（a）进行逻辑与运算即可以获得物体边界 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image071.gif) 。见图 8-6-5（e）。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image072.gif)
图 8-6-5　边界并行计算示例

　　2.边界的跟踪算法

　　这里要介绍的边界跟踪算法比图像分割中用扫描方式进行界线跟踪方法要简单得多。因为这儿主要是对二值图像，获取边界的目的是在需要时用简单的链码形式表示，以便于实际应用。

　　设 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image074.gif) 为物体 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image075.gif) 的一个连通分量， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image077.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image078.gif) 的一个连通分量， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image079.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image080.gif) 的边界即为邻接到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image081.gif) 的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image082.gif) 中点的集合。现在约定对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image083.gif) 用8-邻点连通， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image084.gif) 用4-邻点连通。给定一对初始点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image086.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image088.gif) 。 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image089.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image090.gif) 中， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image091.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image092.gif) 中。边界跟踪算法（简称为BF算法）步骤如下：

　　（１）将 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image093.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image094.gif) 的值分别变为3和2。

　　（２）将 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image095.gif) 点作为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image096.gif) 点的第一个邻点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image098.gif) ，按顺时针方向搜索所有邻点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image100.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image102.gif) ，………， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image104.gif) 。一但遇到邻点值为1，3和4即停止。并记此点为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image106.gif) 。

　　①在搜索过程中，若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image108.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image110.gif) ，对于 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image112.gif) 时， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image114.gif) 时变3为4，变2为0并结束。

　　②否则，（设 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image115.gif) 中点值均为1）取 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image116.gif) 作为新的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image117.gif) ，且变 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image118.gif) 值为4。取 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image120.gif) 作为新的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image121.gif) ，重复第（2）步。

　　（3）当BF算法结束时，图中所有值为4的点的集合即为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image122.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image123.gif) 的边界。

　　应用BF算法跟踪检测图像中给定子集的全部界线点的步骤如下：

　　（1）应用电视光栅方式扫描图像，若发现有1或2出现在紧靠0的右边，或者是发现1或2出现在一行的最左端时停止，则此点一定是尚未跟踪过的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image124.gif) 的边界点（譬如说 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image125.gif) 边界）假定为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image127.gif) 点。

　　（2）应用BF算法跟踪这个边界 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image128.gif) ，并且按下列约定给它们标志。

　　①若它们有 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image129.gif) 的点作为左邻点，则变这些点为3（原来为1或2）

　　②若它们没有 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image130.gif) 的点作为左邻点，则变这些点位（原来为1）

　　③若它们原来标志是3的不变。

　　（３）当BF算法完成后，回到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image131.gif) 点继续向下扫，并再用BF算法。直到扫描完成后，所有的边界均被标志以2或3。

　　有以下几点需要说明：

　　首先，若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image132.gif) 是单连通，只用一个标志3就可以了。但若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image133.gif) 是复连通（其中有孔）只用一个标志，会使其中的孔丢失。如图 8-6-6所示（未标值处为0）

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image134.gif)图 8-6-6　S为复连通的边界跟踪算法

　　若只使用一个标志3将得到图 8-6-6（b）的结果，作为孔 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image136.gif) 的边界点（图 8-6-6（b）中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image138.gif) 点）将绝不可能被发现。因为按跟踪检测步骤①规定，紧靠 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image140.gif) 的左边必须有0才能被检测到。但是若用两种标志时，在跟踪背景边界后就会获得图 8-6-6（c）的结果。继续扫描到带*号的2点时，以为它的左边是0，即可以停止扫描而进行BF算法，从而找到这个孔 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image141.gif) 的边界点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image142.gif) （孔H仅有这一点在H边界上，而不在背景边界上）。

　　其次，将跟踪检测算法略加改进后，可以使用于未被分割的灰度图像。只要知道图像 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image144.gif) 中哪些点是属于感兴趣的目标 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image145.gif) ，哪些不属于即可。

　　再次，如果应用BF算法来跟踪检测弧和曲线，要根据情况做一些改进，过分复杂的改进，要考虑到实际应用的可能性和有效性。

　　（三）目标物体的标定和计数

　　在实际应用中，往往需要将图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image146.gif) 的每一个分量作为单独目标物来处理，因此除了给它们以不同的标志外，还要能够给予正确的计数，达到这目标的基本方法就是搜索扩展法和扫描方法。

　　1.搜索扩展法

　　设图像经分割后 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image147.gif) 的每一个分量中点全都是1。进行系统的扫描搜索，若碰到1，将1改变为尚未使用过的标志符号如 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image149.gif) 。进而将 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image150.gif) 的邻点中为1的点全部改变成标志 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image151.gif) ，如此扩展到整个分量。这样这个分量全部以 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image152.gif) 标志。依此类推使全图搜索扩展完毕，此时 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image153.gif) 中不同分量将给予不同的标志。显然这样做是很费时间的。

　　2.扫描法

　　应用扫描方式给图像中目标物体标志有很多方法，现介绍一种简单方法。

　　设图像经分割后 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image154.gif) 的每一个分量中的点全为1。首先扫描图像，若在某一行遇有一段或不相同的若干段1，则先给予不同的标志 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image156.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image158.gif) ，……。接着扫描一行，在遇到1时，若此1和上行相邻段相同标志如 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image159.gif) 。若此1和上行几段相邻，则将它们都改用统一标志。若与上一行各段均不相邻，给予未使用的标志如 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image161.gif) ，如此类推扫完全图。

　　3.计数

　　这里仅介绍一种最简单的计数方法——跟踪法。就是在开始出1的段的那一行，记下出现不相邻的1的段数。以下逐行计算，凡是遇到有1的段，若和前面一行1的段不相邻，则总数加1，若和前面一行 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image163.gif) 段相邻，则总数减去 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image165.gif) 。扫描完成后所的的总数就是 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image166.gif) 的分量个数。当然这个分量的个数包括单连通和复连通（即有孔的连通分量）。

二、大小和距离的计算

　　（一）面积计算

　　定义数字图像中某一个子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image167.gif) 的面积就是 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image168.gif) 中的点数（象素数）。可见图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image169.gif) 的面积计算很简单.例如图像中有 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image170.gif) 个 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image171.gif) 分量，![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image173.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image175.gif) ，……， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image177.gif) ，……， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image179.gif) 。

　　设置 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image180.gif) 个计数器分别和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image181.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image183.gif) ，……， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image185.gif) 对应，扫描遇到1时，判定属于哪个分量，即在哪个计数器上加1。扫描结束， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image186.gif) 个计数器内容即为对应分量面积。图像面积计算方法也很多，可以根据图像特点、应用要求，以及你所拥有的计算机能力来自行制定。例如对于已划分好连通分量的图像，可以采用一次扫描来完成起面积计算。其方法步骤如下：

　　（1）在扫描到有1的段数第一行时，将各段中1的个数存入对应的计数器，并且建立对应的指针（如地址）。

　　（2）在以下各行扫描过程中，遇到含1的段 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image187.gif) ，进行下列判决和操作。

　　①若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image188.gif) 段和前一行中各个1的段都不连通，建立一个新的计数器，并把 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image189.gif) 的长度（即含1个数）计入此计数器。

　　②若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image190.gif) 段和前一行中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image192.gif) 段相连通，则将 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image193.gif) 的长度计入 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image195.gif) 的计数器，同时将指针由 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image196.gif) 移到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image198.gif) 。

　　③若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image199.gif) 段和前一行若干段 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image201.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image203.gif) ，……， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image205.gif) 相连通。则将 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image207.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image209.gif) ，……， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image211.gif) 的计数器中数和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image212.gif) 的长度一起加到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image213.gif) 计数器中，并且把指针由 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image214.gif) 移到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image215.gif) 。

　　（3）扫描完全图计数器中存数即对应的分量面积

　　图像面积计算的精度直接依赖于数字化过程中取样间隔的大小，显然，取样间隔越小，计算出来的数字面积就越接近于真实面积。另外面积计算与噪声大小也有关系。为了减少噪声影响，常通过设置面积门限来解决，在面积门限以下的少数几个1，可以不计入连通分量面积。

　　（二）周长的计算

　　数字图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image216.gif) 的周长定义有不同的概念，因此计算的方法也就不同，通常有以下三种：

　　（１）周长定义一：把 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image217.gif) 的点和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image219.gif) 的点分割开来的“裂缝”长度之总和，也就是 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image220.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image221.gif) 点对数。（ ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image222.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image223.gif) 相邻，且 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image224.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image225.gif) 中， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image226.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image227.gif) 中）

　　（２）周长的定义二：BF边界算法的步数。

　　（３）周长定义三： ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image228.gif) 的边界 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image230.gif) 的面积，即 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image232.gif) 的点数。

　　数字图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image233.gif) 周长计算精度，受取样间隔大小和噪声影响更严重，常常随取样间隔减少呈指数增加。实际应用中要特别注意。

　　例8-5 计算下列子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image234.gif) 的周长（图 8-6-7）

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image235.gif)
图 8-6-7　周长计算示例

　　根据不同的周长定义，其计算结果见表8-6-1。

||a|b|c|
|----|----|----|----|
|定义一周长|8|10|16|
|定义二周长|4|6|6|
|定义三周长|4|4|4|

表 8-6-1　周长示例计算结果

　　（三）距离的定义和计算方法

　　1.距离的定义

　　设 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image237.gif) 为对每一个点对均为非负数的函数。若对所有点对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image238.gif) 、 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image239.gif) 、 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image241.gif) 满足下列三个条件的函数形式均可作为距离定义。

　　①当且仅当 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image243.gif) 时 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image245.gif)

　　② ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image247.gif)

　　③ ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image249.gif)

　　常用的有三种距离定义：

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image250.gif)

图 8-6-8　三种距离定义

　　（１）欧几里德距离，以 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image252.gif) 表示。见图 8-6-8（a）

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image254.gif)

　　对一幅 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image256.gif) 数字图像，其对角距离 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image258.gif) 。

　　（２）市区街道距离，以 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image260.gif) 表示，见图 8-6-8（b）.

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image262.gif)

　　对一幅 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image263.gif) 数字图像，其对角距离 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image265.gif) .因为对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image267.gif) 点距离为1点，就是4-邻点定义域，所以用 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image268.gif) 表示。

　　（３）棋盘距离，以 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image270.gif) 表示，见图 8-6-8（c）

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image272.gif)

　　对一幅 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image273.gif) 数字图像，其对角距离 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image275.gif) ，同样对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image276.gif) 点距离为1点就是8-邻点定义域，所以用 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image278.gif) 表示。

　　2.距离计算

　　距离算法有以下几种：

　　（1）距离的并行算法简称PD算法

　　（2）距离的序贯算法

　　（四）对称中轴（骨架）和收缩扩展以及减弱（细化）运算

　　1.对称中轴

　　从一般概念上讲，“对称中轴”是指物体 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image279.gif) 上到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image281.gif) 的距离为局部最大的点的集合。也可以简称为“中轴”或“对称轴”。它好象是物体的中心骨架，所以常常直接称其为“骨架”。如果给定物体的对称中轴和它上面各点到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image282.gif) 的距离。那么就可以重建这个物体。因此也可以用它来描述物体。但常没有用边界描述物体节省比特数多，而且对噪声敏感，还会受到连通性的影响。因此只在一些特殊场合下应用。

　　令 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image284.gif) 为物体 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image285.gif) 的对称中轴，则 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image287.gif) 是 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image288.gif) 中到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image289.gif) 的距离为局部最大的点集。即当且仅当满足下式：

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image291.gif)

　　式中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image292.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image293.gif) 中的点， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image295.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image296.gif) 的邻点。

　　从这个式子可见， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image297.gif) 的对称中轴 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image298.gif) 上任一点到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image299.gif) 的距离大于这个点所有邻点到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image300.gif) 的距离。

　　2.扩展和收缩

　　对于任意图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image301.gif) ，其距离为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image303.gif) 范围内的点集，可以通过将 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image304.gif) 向 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image305.gif) 扩展而获得。 其办法是：若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image306.gif) 中点的邻点有0也有1，则将这些邻点中的0全部改成1，并且重复 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image307.gif) 次就可获得。其结果用 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image309.gif) 表示。同样，不在到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image310.gif) 的距离为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image311.gif) 之内的点集，可由 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image312.gif) 收缩而获得。其办法是：若 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image313.gif) 上的点的邻点中有0也有1，则将这些邻点中的1全部改成0，并且重复 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image314.gif) 次就可以获得。其结果记为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image316.gif) 。在收缩扩展运算过程中要注意以下几点：

　　（１）收缩扩展过程中“邻点”和“距离”定义要保持一致，即对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image317.gif) 用4-邻点定义，而对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image319.gif) 用8-邻点定义。

　　（２）收缩 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image320.gif) 相当于扩展 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image321.gif) 。扩展 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image322.gif) 相当于收缩 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image323.gif) 。而且收缩和扩展可以组合运算，如 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image325.gif) 表示对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image326.gif) 先扩展 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image328.gif) 次再收缩 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image330.gif) 次的结果。但 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image331.gif) ，![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image333.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image335.gif) 三者并不一定相同。如图 8-6-13所示。此处采用 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image336.gif) 和8-邻点定义。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image337.gif)
图 8-6-9　收缩和扩展运算

　　一般情况下式成立

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image339.gif)

　　对某一个常数 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image341.gif) ，下式成立

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image343.gif)

　　（３）收缩和扩展运算在并行计算机中能够有效地进行操作运行。收缩和扩展运算在许多图像技术中都可以运用。例如通过收缩扩展可以获得图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image344.gif) 的对称中轴（骨架）。这种方法好象在一块干草地四周同时放火，火不断地向中心方向燃烧，最后熄火线即为对称中轴，因此也有人称其为“火烧草地”法。也可以用来检测图像中某些部分是孤立的，还是成团聚集的，因为这些部分经过扩展几次，再收缩几次，孤立点将保持不变，而成团聚集的部分将汇成块。还可以用来填充边界的不连续和不完整区域。在某些情况下，先收缩在扩展还可以去除噪声。。应该说明的是收缩对分析图像的拓扑性质很有用处，如将连通分量收缩一点，对于计数运算就很方便了。所谓图像的拓扑性质概念就是将图像划在橡皮上，当橡皮变形时而图像中的某些不变的几何性质。如相邻、连通、交叉、包围等等。都是图像的拓扑性质。而角度、面积、周长等就不是图像的拓扑性质了。

　　3.减薄（细化）

　　收缩可以使物体缩小，但不能保持其形状特性，因此在下面形状分析中采用细化算法，可以使物体仍保持其曲线形状。细化算法和收缩算法基本相同。只是在运算过程中只消去那些至少有两个1邻接的象素。因此物体的端点不会被消除，从而保留了曲线形状。细化运算可以分为两步，即先左右后上下。当然先上下后左右也是可以的。

三、物体形状分析

　　用图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image345.gif) 的形状来描述物体，可以更接近人们的视觉习惯。但形状本身的概念是很复杂的。它不是指子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image346.gif) 的某一个单一性质，而是一族性质的综合，不但是多维的而且与人们的视觉心理有关。因此目前还不能作出确切的分析研究，只能对其某些方面加以分析，作为一些场合下的初级应用。

　　（一）曲线分析

　　1.一些基本概念

　　曲线和弧，若一个图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image347.gif) ，它的大多数点只有两个邻点在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image348.gif) 内，则称 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image349.gif) 是弧状或曲线状。这些点称为曲线上普通点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image351.gif) ，如图 8-6-10中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image352.gif) 。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image353.gif)图 8-6-10　一些有关曲线概念示意图

　　曲线的端点： ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image354.gif) 上只有一个邻点在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image355.gif) 内的点，称其为端点，如图 8-6-10中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image357.gif) 。

　　曲线的支点：具有二个以上的邻点在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image358.gif) 内的点，称其为支点。如图 8-6-10中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image360.gif) 。

　　简单曲线（弧）：有端点无支点的曲线（弧）称为简单弧。如图 8-6-10（b）。

　　简单封闭曲线：无端点也无支点的弧称为简单封闭曲线。如图 8-6-10（c）。

　　所有上述概念都存在4-邻点和8-邻点的两种不同定义方法。在采用链码表示曲线时，4-邻点定义用4向链码，8-邻点定义用8向链码。

　　2.曲线长度（弧长）斜率和曲率

　　曲线的长度，斜率和曲率可以采用几何学中连续曲线的长度，斜率和曲率定义概念结合数字曲线的特点加以定义。例如求图 8-6-11中曲线 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image361.gif) 的长度。显然 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image362.gif) 是 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image364.gif) 点的集合，即 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image366.gif) 。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image367.gif)图 8-6-11　曲线S及八向链码

　　设有一个数字弧 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image369.gif) 见图 8-6-11所视。定义 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image371.gif) 和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image373.gif) 间的弧长为：由 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image374.gif) 沿曲线 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image375.gif) 移动到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image376.gif) 的步数。此处应注意垂直和水平方向每步算1，对角方向每步计为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image378.gif) 。例如由 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image380.gif) 到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image382.gif) 的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image383.gif) 的长度为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image385.gif) 。

　　对于数字曲线的斜率和曲率不能简单利用连续曲线的有关概念，因为数字曲线只有8个固定斜率，往往与实际相差较大。因此要重新定义。下面介绍一种简单的平滑算法：定义 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image386.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image387.gif) 点的左斜率k和右K斜率分别为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image388.gif) 到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image390.gif) 方向和 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image391.gif) 到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image393.gif) 方向的斜率。其中K为大于或等于1的整数。 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image394.gif) 的左右K斜率的差，则定义为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image395.gif) 曲线在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image396.gif) 的曲率。例如图 8-6-11中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image397.gif) 曲线在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image398.gif) 点的曲率计算如下：

　　当 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image400.gif) 时， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image401.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image402.gif) 点的左1斜率为45度，而右1斜率为0度，其曲率为45度。

　　当 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image404.gif) 时， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image405.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image406.gif) 点的左2斜率为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image408.gif) ，而右2斜率为0度，其曲率为30度。

　　当 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image410.gif) 时， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image411.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image412.gif) 点的左3斜率和右3斜率都为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image414.gif) ，其曲率为0度。 

　　可见当 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image415.gif) 取到3时，其斜率和曲率都等于（或接近） ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image416.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image417.gif) 的真正斜率和曲率。也就是讲 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image418.gif) 的取值是很重要的。直接影响计算效果。

　　数字曲线用链码表示，实际就是将曲线看成是各点斜率的函数。而“差分链码”也就是用曲线各点曲率来描述 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image419.gif) 。如果用斜率或曲率直方图来表示曲线，可以获得这个数字曲线许多有用的信息。例如 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image420.gif) 斜率直方图中尖峰出现，表明曲线有强烈的方向偏移。而低曲率的直方图对应比较直而平滑的曲线。高曲率直方图对应频繁摆动的曲线等。

　　3.曲线的分段 

　　在一幅由曲线组成的图案中，可以将曲线在其支点处断开，分成许多简单的弧或封闭曲线，好象将汉字图案分割成许多有方向的笔画似的。曲线分段方法有很多，这里仅就一些最基本的方法从概念上说明一下：

　　（１）阈值法：对曲线的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image421.gif) -斜率或曲率给出一定的阈值范围，抑制在这个范围外的曲线上点。比较有意义的是检测曲线曲率极大值（即曲线的角的位置）。这是最初级分段方法。

　　（２）拐点分段：曲线拐点就是曲线曲率的过0点处，由此处可将曲线分为凹凸两部分，作为物体的形状分析。

　　（３）作弦测弧，如给定曲线一点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image422.gif) ，从 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image423.gif) 到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image424.gif) 划一个弦，可以通过测量 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image425.gif) 到弦的距离，若此距离小，表明曲线在此点附近比较直或者是拐点附近，也可以测量弦/弧比，比值大说明曲线在此点附近比较直。反之，曲线在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image426.gif) 处曲率大。

　　（４）匹配法：如给定一个模板曲线T，其斜率序列的链码表示为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image428.gif) 。曲线 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image429.gif) 的斜率序列链码形式为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image431.gif) 。从 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image433.gif) 点开始计算下列公式：

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image435.gif)

　　式子中 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image436.gif) 小于或远小于 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image438.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image439.gif) 为左右斜率定义中的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image440.gif) 值。 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image442.gif) 为绝对角度差的某种测度。可见，此式值越小，其匹配程度越好。这种方法可应用于曲线的搜索枝中。

　　在空间域中还可以应用分段逼近的分段方法等，有些场合下，还可以先将曲线进行坐标变换或正交变换等，再在这些域中进行分段处理。

　　（二）投影和截面

　　1.投影：设 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image443.gif) 为一幅 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image445.gif) 大小的数字图像，定义各列和的向量 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image446.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image447.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image449.gif) 轴上的投影，同样定义 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image451.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image452.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image454.gif) 轴上的投影，也可以定义 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image456.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image457.gif) 在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image459.gif) 方向上的投影。以二值阿拉伯数字“8”为例，见图 8-6-16。

![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image460.gif)
图 8-6-12　投影

　　投影在图像中物体的检测和定位方面是有价值的。一个紧凑物体，其投影在它的位置范围往往是变化缓慢的。反之，即作为检测物体的位置。投影可以为物体的辨认如图案和文字的识别，提供许多有用的性质，国内某些汉字识别方法研究就是利用不同汉字有不同的投影的特性。关于投影的深入研究及CT技术中的应用将在本书后面介绍。

　　2.截面：数字图像 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image461.gif) 沿直线或曲线 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image463.gif) 各点灰度级序列，被称为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image464.gif) 沿 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image465.gif) 的截面。因此图像 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image466.gif) 在斜率为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image468.gif) 直线上的投影，就是图像 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image469.gif) 沿斜率为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image471.gif) 直线族的截面之和的序列。如图 8-6-17所示。

　　不难看出图像 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image472.gif) 在斜率为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image473.gif) 的直线上的投影为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image475.gif) 。其截面为{1；1，3；0，1，2；1，2；2；1，3，3；2，2，2}。

　　利用物体在合适方向上的截面特性来探测、定位和辨认物体是很有意义的方法。还可以用来判断物体的大致形状，以及分割图像子集为块进而进一步计测等等。

　　（三）凹凸性

　　设 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image476.gif) 是图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image477.gif) 中的点，若通过 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image478.gif) 的每条直线只与 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image479.gif) 相交一次，则称 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image480.gif) 为发自 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image481.gif) 的星形的，也就是站在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image482.gif) 点能看到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image483.gif) 的所有点。

　　定义 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image484.gif) 满足下列条件之一者，称此 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image485.gif) 为凸状的。

　　（１）从 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image486.gif) 中每点看， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image487.gif) 都是星形的。

　　（２）对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image488.gif) 中任意两点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image489.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image490.gif) ，从 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image491.gif) 到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image492.gif) 的直线段完全在 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image493.gif) 中。

　　（３）对 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image494.gif) 中任意两点 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image495.gif) ， ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image496.gif) ，从 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image497.gif) 到 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image498.gif) 的直线中点位于 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image499.gif) 中。

　　（４）任一条直线与 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image500.gif) 只能相交一次。

　　可见上述四个条件是等效的。一个凸状物体是没有凹处，也不会有孔，而且是连通的。但要注意，在数字图像中是凸性的物体，在数字化以前的模拟图像中可能有细小的凹处，这些细小凹处常在取样时被漏掉。

　　为了应用图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image501.gif) 的凹凸性分析它的形状特征，常常引出一个“凸壳”概念：对于任何一个子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image502.gif) ，有一个最小的包含 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image503.gif) 的凸集，即所有包含 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image504.gif) 的凸集的交集。称其为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image505.gif) 的凸壳。凸壳计算可以应用并行算法，也可以应用序贯算法。

　　（四）分散度和伸长度

　　（１）分散度：设图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image506.gif) ，面积为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image507.gif) ，周长为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image508.gif) ，定义 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image510.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image511.gif) 的“分散度”。可见这个测度是符合人的常识的，相同面积的几何形状物体，其周长越小，越紧凑。对圆形 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image512.gif) 来讲 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image514.gif) ，其他任何形状的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image515.gif) ，其 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image517.gif) 。因此圆形 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image518.gif) 最紧凑。数字图像同样可以应用上述分散度测度。但数字圆形 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image519.gif) 其分散度 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image520.gif) 并不是最小。这与 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image521.gif) 的测量方法有关。

　　（２）伸长度：设图像子集 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image522.gif) 的面积为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image523.gif) ，宽度为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image525.gif) ，即使 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image526.gif) 完全消失所需的最少收缩步数。定义 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image528.gif) 为 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image529.gif) 的伸长度。这也是符合人们的习惯，面积一定的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image530.gif) ，其宽度 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image531.gif) 越小，肯定越瘦长。反之，越胖短。

　　要注意的是 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image532.gif) 的分散度和伸长度有时并不一致，如一个锯齿状边缘的圆 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image533.gif) ，分散测度 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image534.gif) 可能很高，但伸长度测度 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image535.gif) 不是很小的，因为这样 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image536.gif) 收缩起来很快就会消失。即 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image537.gif) 是小的。从概念上将也就是很分散的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image538.gif) 并不一定是伸长的。另外，这个伸长度测度对噪声很敏感，特别是有孔存在的 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image539.gif) ，收缩起来很快，即 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image540.gif) 可能很小。从而使伸长度加大。改善的办法是反复扩展和压缩几次，去掉孤立噪声和小孔。因此这两个测度还是值得研究。

　　利用前面介绍的扩展和收缩技术，结合伸长度概念可以用来检测图像中不伸长部分（如湖泊，海洋）及伸长部分（如河流）等。

　　（五）复杂性

　　复杂性是物体形状分析的一个重要性质，往往是许多性质的结合，因为人对复杂性的判断是依赖物体的许多性质。而且与观察环境，观察者的知识习惯等心理因素有关。建筑师认为并不复杂的工程图，电子学工程师看上去很复杂。相反，电子学工程师认为简单的线路，建筑师可能认为复杂得很。因此这是一个很难定义和测度的型号子。从人的经验上，有以下几个方面值得考虑：

　　（１） ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image541.gif) 的边界上曲率极大值越多（角越多），其复杂性高。

　　（２） ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image542.gif) 边界上的曲率变化越大（角大小变化多），其复杂性越高。

　　（３）要确定或描述物体 ![](http://www.fjtu.com.cn/fjnu/courseware/0334/course/_source/web/lesson/char8/j6_image/image543.gif) 的信息量越多，其形状越复杂。

　　另外，分散度和对称性也是物体复杂性的分析的重要因素。对称物体比不对称物体所需要的描述信息两少一倍。


