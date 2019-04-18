# Efficient Graph-Based Image Segmentation原文的详细解析 - aiaiai010101的博客 - CSDN博客

2017年03月20日 16:55:37[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：756



﻿﻿

**图像分割—基于图的图像分割（Graph-Based Image Segmentation）**

*Reference: *

Efficient Graph-Based Image Segmentation,IJCV 2004,MIT [Code](http://cs.brown.edu/~pff/segment/)

Graph-Based Segmentation 是经典的图像分割[算法](http://lib.csdn.net/base/31)，作者Felzenszwalb也是提出DPM算法的大牛。该算法是基于图的**贪心聚类**算法，实现简单，速度比较快，精度也还行。不过，目前直接用它做分割的应该比较少，毕竟是99年的跨世纪元老，但是很多算法用它作垫脚石，比如Object
 Propose的开山之作《Segmentation as Selective Search for Object Recognition》就用它来产生过分割（oversegmentation）。还有的语义分割(senmatic segmentation )算法用它来产生超像素（superpixels）具体忘记了……

# 图的基本概念 

因为该算法是将照片用加权图抽象化表示，所以补充图的一些基本概念。 

**图**是由**顶点**集![](http://images.cnitblog.com/blog/460184/201407/212109540575764.png)（vertices）和**边**集![](http://images.cnitblog.com/blog/460184/201407/212109544003193.png)（edges）组成，表示为![](http://images.cnitblog.com/blog/460184/201407/212109548693350.png)，顶点![](http://images.cnitblog.com/blog/460184/201407/212109551976550.png)，在本文中即为单个的像素点，连接一对顶点的边![](http://images.cnitblog.com/blog/460184/201407/212109555106520.png)具有**权重![](http://images.cnitblog.com/blog/460184/201407/212109557756206.png)**，本文中的意义为顶点之间的**不**相似度，所用的是**无向图**。

![](http://images.cnitblog.com/blog/460184/201407/212109561971378.png)

**树：**特殊的图，图中任意两个顶点，都有路径相连接，但是没有**回路。**如上图中**加粗**的边所连接而成的图。如果看成一团乱连的珠子，**只保留**树中的珠子和连线，那么随便选个珠子，都能把这棵树中所有的珠子都提起来。如果，i和h这条边也保留下来，那么h,I,c,f,g就构成了一个回路。

**最小生成树**（MST, [minimum spanning tree](http://en.wikipedia.org/wiki/Minimum_spanning_tree)）：特殊的树，给定需要连接的顶点，选择**边权之和最小的树。**上图即是一棵MST

![](http://images.cnitblog.com/blog/460184/201407/212109565254577.png)

本文中，初始化时每一个像素点都是一个顶点，然后逐渐合并得到一个区域，确切地说是连接这个区域中的像素点的一个MST。如图，棕色圆圈为顶点，线段为边，合并棕色顶点所生成的MST，对应的就是一个分割区域。分割后的结果其实就是森林。

# 相似性 

既然是聚类算法，那应该依据何种规则判定何时该合二为一，何时该继续划清界限呢？ 

对于**孤立的两个像素点**，所不同的是颜色，自然就用颜色的距离来衡量两点的相似性，本文中是使用RGB的距离，即![](http://images.cnitblog.com/blog/460184/201407/212109568072791.png)

当然也可以用perceptually uniform的Luv或者Lab色彩空间，对于灰度图像就只能使用亮度值了，此外，还可以先使用纹理特征滤波，再计算距离，比如，先做Census Transform再计算[Hamming distance](http://en.wikipedia.org/wiki/Hamming_distance)距离。

## 全局阈值à自适应阈值

上面提到应该用亮度值之差来衡量两个像素点之间的差异性。对于两个区域（子图）或者一个区域和一个像素点的相似性，最简单的方法即只考虑连接二者的边的不相似度。 

![](http://images.cnitblog.com/blog/460184/201407/212109572752948.png)

如图，已经形成了棕色和绿色两个区域，现在通过紫色边来判断这两个区域是否合并。那么我们就可以设定一个**阈值**，当两个像素之间的差异（即不相似度）小于该值时，合二为一。迭代合并，最终就会合并成一个个区域，这就是区域生长的基本思想：星星之火，可以燎原。

![](http://images.cnitblog.com/blog/460184/201407/212109576199376.png)![](http://images.cnitblog.com/blog/460184/201407/212109580889533.png)

显然，上面这张图应该聚成右图所思的3类，高频区h,斜坡区s,平坦区p。如果我们设置一个**全局阈值**，那么如果h区要合并成一块的话，那么该阈值要选很大，但是那样就会把p和s区域也包含进来，分割结果**太粗**。如果以p为参考，那么阈值应该选特别小的值，那样的话，p区是会合并成一块，但是，h区就会合并成特别特别多的小块，如同一面支离破碎的镜子，分割结果**太细**。

显然，全局阈值并不合适，那么自然就得用**自适应阈值**。对于p区该阈值要特别小，s区稍大，h区巨大。 

对于两个区域（原文中叫Component,实质上是一个MST,单独的一个像素点也可以看成一个区域）,本文使用了非常直观，但抗干扰性并不强的方法。先来两个定义，原文依据这两个附加信息来得到自适应阈值。

一个区域的**类内差异![](http://images.cnitblog.com/blog/460184/201407/212109583544518.png)**：

![](http://images.cnitblog.com/blog/460184/201407/212109586827718.png)

可以近似理解为一个区域内部最大的亮度差异值，定义是MST中不相似度最大的一条边。 

两个区域的**类间差异![](http://images.cnitblog.com/blog/460184/201407/212109589799160.png)**：

![](http://images.cnitblog.com/blog/460184/201407/212109592607374.png)

即连接两个区域所有边中，不相似度最小的边的不相似度，也就是两个区域最相似的地方的不相似度。 

那么直观的判断是否合并的标准： 

![](http://images.cnitblog.com/blog/460184/201407/212109595103831.png)

等价条件 

![](http://images.cnitblog.com/blog/460184/201407/212109598228503.png)

    解释： ![](http://images.cnitblog.com/blog/460184/201407/212110001824460.png)，![](http://images.cnitblog.com/blog/460184/201407/212110004637375.png)分别是区域![](http://images.cnitblog.com/blog/460184/201407/212110007292360.png)和![](http://images.cnitblog.com/blog/460184/201407/212110010253802.png)所能忍受的最大差异，当二者都能忍受当前差异![](http://images.cnitblog.com/blog/460184/201407/212110013072016.png)时，你情我愿，一拍即合，只要有一方不愿意，就不能强求。

    特殊情况，当二者都是孤立的像素值时，![](http://images.cnitblog.com/blog/460184/201407/212110015721702.png)，所有像素都是"零容忍"只有像素值完全一样才能合并，自然会导致过分割。所以刚开始的时候，应该给每个像素点设定一个可以容忍的范围，当生长到一定程度时，就应该去掉该初始容忍值的作用。原文条件如下

![](http://images.cnitblog.com/blog/460184/201407/212110018381389.png)    增加项![](http://images.cnitblog.com/blog/460184/201407/212110021357129.png):

![](http://images.cnitblog.com/blog/460184/201407/212110023853586.png)

其中![](http://images.cnitblog.com/blog/460184/201407/212110027602772.png)为区域![](http://images.cnitblog.com/blog/460184/201407/212110030727444.png)所包含的像素点的个数，如此，随着区域逐渐扩大，这一项的作用就越来越小，最后几乎可以忽略不计。那么![](http://images.cnitblog.com/blog/460184/201407/212110033387130.png)就是一个可以控制所形成的的区域的大小，如果，![](http://images.cnitblog.com/blog/460184/201407/212110035883587.png)那么，几乎每个像素都成为了一个独立的区域，如果![](http://images.cnitblog.com/blog/460184/201407/212110040256286.png)，显然整张图片都会聚成一块。所以，![](http://images.cnitblog.com/blog/460184/201407/212110042752743.png)越大，分割后的图片也就越大。

当然，可以采用中位数来应对超调，不过这就变成了一个NP难问题，证明见原文 

## 形状相似

**前面提到的用颜色信息来聚类，修改相似性衡量标准，可以聚类成我们想要的特定形状。比如我们希望得到很多长条形的区域，那么可以用聚类后的所形成的区域的面积/周长 + 亮度值的差 衡量两个子图或者两个像素之间的相似度。因为长条形的面积/周长会比较小。**

# 算法步骤 

**Step 1**: 计算每一个像素点与其8邻域或4邻域的不相似度。

![](http://images.cnitblog.com/blog/460184/201407/212110045724186.png)

如左边所示，实线为只计算4领域，加上虚线就是计算8邻域，由于是无向图，按照从左到右，从上到下的顺序计算的话，只需要计算右图中灰色的线即可。 

**Step 2:** 将**边**按照不相似度**non-decreasing排列（**从小到大**）**排序得到![](http://images.cnitblog.com/blog/460184/201407/212110049161615.png)。

**Step 3:** 选择![](http://images.cnitblog.com/blog/460184/201407/212110052919799.png)

**Step 4:** 对当前选择的边![](http://images.cnitblog.com/blog/460184/201407/212110056352527.png)进行合并判断。设其所连接的顶点为![](http://images.cnitblog.com/blog/460184/201407/212110059477199.png)。如果满足合并条件：

（1）![](http://images.cnitblog.com/blog/460184/201407/212110063226384.png)不属于同一个区域![](http://images.cnitblog.com/blog/460184/201407/212110066044598.png)；

（2）不相似度不大于二者内部的不相似度。![](http://images.cnitblog.com/blog/460184/201407/212110069006041.png)则执行**Step 4**。否则执行**Step 5**

**Step 5:** 更新阈值以及类标号。

更新类标号：将![](http://images.cnitblog.com/blog/460184/201407/212110071977483.png)的类标号统一为![](http://images.cnitblog.com/blog/460184/201407/212110075107454.png)的标号。

更新该类的不相似度阈值为：![](http://images.cnitblog.com/blog/460184/201407/212110077603911.png)。

**注意：**由于不相似度小的边先合并，所以，![](http://images.cnitblog.com/blog/460184/201407/212110080575354.png)即为当前合并后的区域的最大的边，即![](http://images.cnitblog.com/blog/460184/201407/212110083225040.png)。

**Step 6:** 如果![](http://images.cnitblog.com/blog/460184/201407/212110085884726.png)，则按照排好的顺序，选择下一条边执行**Step 4**，否则结束。

# 结果 

![](http://images.cnitblog.com/blog/460184/201407/212110092442126.png)

![](http://images.cnitblog.com/blog/460184/201407/212110101826738.gif)![](http://images.cnitblog.com/blog/460184/201407/212110105417396.gif)

Segmentation parameters: sigma = 0.5, k= 500, min = 50.

**Sigma****：**先对原图像进行高斯滤波去噪，sigma即为高斯核的![](http://images.cnitblog.com/blog/460184/201407/212110109003352.png)

**k:**控制合并后的区域的大小，见前文

**min: **后处理参数，分割后会有很多小区域，当区域![](http://images.cnitblog.com/blog/460184/201407/212110115412224.png)像素点的个数![](http://images.cnitblog.com/blog/460184/201407/212110120259908.png)小于min时，选择与其差异最小的区域![](http://images.cnitblog.com/blog/460184/201407/212110123855864.png)合并即![](http://images.cnitblog.com/blog/460184/201407/212110126971537.png)。

# 性质讨论 

结果虽然不是很好，但有很好的全局性质，结论很有意思，有兴趣的可以看看。 

首先要说明的是，对于任何图像，始终**存在**一种分割方法，使得分割的结果既不过细，也不过粗。但是并**不唯一**。

## 引理 

如果step 4 时，![](http://images.cnitblog.com/blog/460184/201407/212110129942980.png)，但并没有合并，即![](http://images.cnitblog.com/blog/460184/201407/212110132914422.png)，那么肯定有一个区域已经分割好了，比如![](http://images.cnitblog.com/blog/460184/201407/212110135578406.png)，那么区域![](http://images.cnitblog.com/blog/460184/201407/212110138228093.png)的范围就不会再有增加，它将会成为最终的分割区域中的一个区域。

### Proof: 

假设，![](http://images.cnitblog.com/blog/460184/201407/212110141358063.png)，由于边是按照non-decreasing排序，所以剩下的连接![](http://images.cnitblog.com/blog/460184/201407/212110144161979.png)的边的不相似度肯定都不低于![](http://images.cnitblog.com/blog/460184/201407/212110147756935.png)，最小的边都不行，其余的边自然是靠边站了。

不过，原文说只能只有一个已经分割好了，但是我觉得还有一种情况，![](http://images.cnitblog.com/blog/460184/201407/212110150575149.png) 并且![](http://images.cnitblog.com/blog/460184/201407/212110152913078.png)，那么这两个区都应该分好了才对呀。

## Not Too fine 

分割太细，也就是本来不应该分开的区域被拦腰截断，但是本算法是能保证有情人终成眷属的，绝对不会干棒打鸳鸯拆散一对是一对的事。 

### Proof: 

![](http://images.cnitblog.com/blog/460184/201407/212110156196277.png)

反证法：如上图。本不应该分割，则应该满足条件![](http://images.cnitblog.com/blog/460184/201407/212110159479477.png)。如果分开了，那么必定存在一条边![](http://images.cnitblog.com/blog/460184/201407/212110163075433.png)导致二者没有合并，那么由前面的引理，必定存在一个区域成为最终分割结果的一部分，假设为A部分，再回溯到判断这条边的时候，必定有，![](http://images.cnitblog.com/blog/460184/201407/212110165888348.png)，从而![](http://images.cnitblog.com/blog/460184/201407/212110169474305.png)，由于是按non-decreasing
 顺序，所以A部分和B部分最小的边就是![](http://images.cnitblog.com/blog/460184/201407/212110172133991.png)，那么![](http://images.cnitblog.com/blog/460184/201407/212110174797975.png)与假设条件矛盾。

## Not Too coarse 

分割太粗，也就是本应该分开的区域没有分开。但本算法能保证当断则断，不会藕断丝连。 

反证法：如上图。本应该分割，则应满足条件![](http://images.cnitblog.com/blog/460184/201407/212110177759418.png)。假设还是![](http://images.cnitblog.com/blog/460184/201407/212110181043618.png) ，![](http://images.cnitblog.com/blog/460184/201407/212110184005061.png)为连接A,B最小的边。如果合并了，由于![](http://images.cnitblog.com/blog/460184/201407/212110186664747.png)，而且是non-decreasing
 顺序，所以在判定边![](http://images.cnitblog.com/blog/460184/201407/212110190413933.png)之前A区域已经形成。如果分割过粗，则判定这条边时最小的边满足![](http://images.cnitblog.com/blog/460184/201407/212110192919389.png)，则必定![](http://images.cnitblog.com/blog/460184/201407/212110196971332.png)使得二者合并了。和条件矛盾。

## 等权边处理先后次序的影响 

如果两条边![](http://images.cnitblog.com/blog/460184/201407/212110199631018.png)，![](http://images.cnitblog.com/blog/460184/201407/212110213076572.png)的权值相同，那么排序时候，谁排前头，谁落后面有影响吗？结论是木有。

### Proof: 

**Case1**：![](http://images.cnitblog.com/blog/460184/201407/212110215414502.png)，![](http://images.cnitblog.com/blog/460184/201407/212110218078486.png)连接的区域相同，即![](http://images.cnitblog.com/blog/460184/201407/212110220416416.png)，![](http://images.cnitblog.com/blog/460184/201407/212110223071401.png)连接的都是区域![](http://images.cnitblog.com/blog/460184/201407/212110236665886.png)，那么它俩谁在前面都没关系。

**Case2**：![](http://images.cnitblog.com/blog/460184/201407/212110239474100.png)，![](http://images.cnitblog.com/blog/460184/201407/212110243856798.png)连接的区域完全不同，比如![](http://images.cnitblog.com/blog/460184/201407/212110246972471.png)连接区域，![](http://images.cnitblog.com/blog/460184/201407/212110249632157.png)，![](http://images.cnitblog.com/blog/460184/201407/212110252752127.png)连接区域![](http://images.cnitblog.com/blog/460184/201407/212110255411814.png),那么谁先谁后，都不影响![](http://images.cnitblog.com/blog/460184/201407/212110258695013.png)是否合并，也不影响![](http://images.cnitblog.com/blog/460184/201407/212110262444198.png)是否合并。

**Case3**：![](http://images.cnitblog.com/blog/460184/201407/212110265252412.png)连接![](http://images.cnitblog.com/blog/460184/201407/212110267912098.png),![](http://images.cnitblog.com/blog/460184/201407/212110270417555.png)连接![](http://images.cnitblog.com/blog/460184/201407/212110274003511.png)

Case3-1：![](http://images.cnitblog.com/blog/460184/201407/212110276355739.png)在先，![](http://images.cnitblog.com/blog/460184/201407/212110279471411.png)在后，并且，![](http://images.cnitblog.com/blog/460184/201407/212110281976868.png)使得![](http://images.cnitblog.com/blog/460184/201407/212110284948311.png)合并，交换二者处理顺序，先处理![](http://images.cnitblog.com/blog/460184/201407/212110287133011.png)，后处理![](http://images.cnitblog.com/blog/460184/201407/212110289796995.png)。如果![](http://images.cnitblog.com/blog/460184/201407/212110292605209.png)不合并![](http://images.cnitblog.com/blog/460184/201407/212110294943139.png)，那不影响![](http://images.cnitblog.com/blog/460184/201407/212110297448595.png)合并![](http://images.cnitblog.com/blog/460184/201407/212110300411039.png)；如果![](http://images.cnitblog.com/blog/460184/201407/212110303075023.png)合并![](http://images.cnitblog.com/blog/460184/201407/212110305724709.png)，那么合并后的![](http://images.cnitblog.com/blog/460184/201407/212110308076937.png)，照样合并。

Case3-2：![](http://images.cnitblog.com/blog/460184/201407/212110312133581.png)在先，![](http://images.cnitblog.com/blog/460184/201407/212110315253551.png)在后，并且，![](http://images.cnitblog.com/blog/460184/201407/212110318858507.png)不合并![](http://images.cnitblog.com/blog/460184/201407/212110321354964.png)，交换二者处理顺序，先处理![](http://images.cnitblog.com/blog/460184/201407/212110324004650.png)，后处理![](http://images.cnitblog.com/blog/460184/201407/212110326356878.png)。如果是![](http://images.cnitblog.com/blog/460184/201407/212110328853335.png)。那![](http://images.cnitblog.com/blog/460184/201407/212110333077506.png)是否合并![](http://images.cnitblog.com/blog/460184/201407/212110335252207.png)，都不会使得![](http://images.cnitblog.com/blog/460184/201407/212110337911893.png)合并；如果![](http://images.cnitblog.com/blog/460184/201407/212110341828605.png),那同样也有![](http://images.cnitblog.com/blog/460184/201407/212110345102806.png)，同样也没影响。

# 补充： 

# 彩色图片 

对于彩色图片，上文是将R,G,B作为距离，整张图片只进行一次分割，原文说对每一个通道都进行一次分割，最后对结果取交集，也就是说图片中的两个点要划分到同一个区域，则在R,G,B三个通道的划分结果中，它俩得始终在同一个区域。原文说这样效果更好……不过他的程序是采用一次分割。

# Nearest Neighbor Graphs 

前文是只用了空间位置![](http://images.cnitblog.com/blog/460184/201407/212110347915721.png)来构件图的连接关系，缺点是明显的，空间不相邻，色彩完全一样也白搭，于是中间稍微有断开都会分成多个部分。于是另一种更为**平等**的策略是二者一块考虑，先映射到特征空间![](http://images.cnitblog.com/blog/460184/201407/212110350412178.png)，再构建图。此时有连接关系的就不一定是4/8邻域了，由于有![](http://images.cnitblog.com/blog/460184/201407/212110353853905.png)对边，因此如果考虑所有边的连接关系的话，太恐怖了！原文是对每个像素点找10个欧氏距离最近的点即10最近邻，构建图，当然，另外一种方法不是固定邻居数目，而是限定距离范围。

![](http://images.cnitblog.com/blog/460184/201407/212110358078076.png)

那么类内距离![](http://images.cnitblog.com/blog/460184/201407/212110360574533.png)的解释就和直观了，类内最短的距离，那么会以这条边为半径，在特征空间构成一个超球体，不过会和别人有相交。

![](http://images.cnitblog.com/blog/460184/201407/212110362758233.png)同样还是两个类直接的最短距离。

找10-NN也是相当累的是，原文采用近似算法ANN《Approximate nearest neighbor searching》来找10近邻，快。 

剩下的和上面一样，但是有一点我没明白，就是![](http://images.cnitblog.com/blog/460184/201407/212110365254690.png)的更新，比如上图，肯定是用绿色这条线更新，那么![](http://images.cnitblog.com/blog/460184/201407/212110367751148.png)的意义就不再是包含集合所有点的最短半径了，求解？

![](http://images.cnitblog.com/blog/460184/201407/212110378076731.png)![](http://images.cnitblog.com/blog/460184/201407/212110389477517.png)

**补充：如果是Grid Graphs的方法，那么左图的鲜花就不会被划分为同一种类别的区域。**

本文转载自： http://blog.csdn.net/huixingshao/article/details/42642973

**个人阅读理解：**

**这篇文章对原文解析得很详细，里边涉及到了很多图像方面的基础算法。**

**特别是详述了两种方法------------------基于Grid Graphs的方法和基于Nearest Neighbor Graphs的方法的不同。**

**前者分割时只有相邻且颜色相同区域才可能被合并成一个区域，后者则考虑到了空间上同种类别区域不相连的可能，映射到一个高维空间再分割(五个维度---(x,y,r,g,b))，这样即使不相连的区域，颜色相同，也可能被合并为同一个区域。**

