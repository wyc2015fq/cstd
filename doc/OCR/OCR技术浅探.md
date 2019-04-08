# OCR技术浅探



## 1.特征提取



### 研究背景

关于光学字符识别(Optical Character Recognition, 下面都简称OCR)，是指将图像上的文字转化为计算机可编辑的文字内容，众多的研究人员对相关的技术研究已久，也有不少成熟的OCR技术和产品产生，比如汉王OCR、ABBYY FineReader、Tesseract OCR等. 值得一提的是，ABBYY FineReader不仅正确率高(包括对中文的识别)，而且还能保留大部分的排版效果，是一个非常强大的OCR商业软件.

然而，在诸多的OCR成品中，除了Tesseract OCR外，其他的都是闭源的、甚至是商业的软件，我们既无法将它们嵌入到我们自己的程序中，也无法对其进行改进. 开源的唯一选择是Google的Tesseract OCR，但它的识别效果不算很好，而且中文识别正确率偏低，有待进一步改进.

综上所述，不管是为了学术研究还是实际应用，都有必要对OCR技术进行探究和改进. 我们队伍将**完整的OCR系统分为“特征提取”、“文字定位”、“光学识别”、“语言模型”四个方面**，逐步进行解决，最终完成了一个可用的、完整的、用于印刷文字的OCR系统. 该系统可以初步用于电商、微信等平台的图片文字识别，以判断上面信息的真伪.

### 研究假设

在本文中，我们假设图像的文字部分有以下的特征：

> \1. 假设我们要识别的图像字体都是比较规范的印刷字体，如宋体、黑体、楷体、行书等；
>
> \2. 文字与背景应该有比较明显的对比度；
>
> \3. 在设计模型的时候，我们假设了图片文本是横向排版的；
>
> \4. 文字的笔画应该有一定的宽度，不可以太细；
>
> \5. 同一个文字的色彩应该最多是渐变的；
>
> \6. 一般文字是通过比较密集的笔画成字的，并且很多时候都具有一定的连通性.

可以看到，这些特征都是常见的电商宣传海报等的常见特点，因此这些假设都是比较合理的.

### 分析流程

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206112759135-1501784341.png)


**图1：我们的实验流程图**

 

特征提取

作为OCR系统的第一步，特征提取是希望找出图像中候选的文字区域特征，以便我们在第二步 进行文字定位和第三步进行识别。在这部分内容中，我们集中精力模仿肉眼对图像与汉字的处理过程，在图像的处理和汉字的定位方面走了一条创新的道路。这部分工作是整个OCR系统最核心的部分，也是我们工作中最核心的部分。

 

传统的文本分割思路大多数是“边缘检测 + 腐蚀膨胀 + 联通区域检测”，如论文[1]。 然而，在复杂背景的图像下进行边缘检测会导致背景部分的边缘过多(即噪音增加)，同时文字部分的边缘 信息则容易被忽略，从而导致效果变差。       如果在此时进行腐蚀或膨胀，那么将会使得背景区域跟文字区域粘合，效果进一步恶化。（事实上，我们在这条路上已经走得足够远了，我们甚至自己写过边缘检测函数来做这个事情，经过很多测试，最终我们决定放弃这种思路。）

 

因此，在本文中，我们放弃了边缘检测和腐蚀膨胀，通过聚类、分割、去噪、池化等步骤，得到了比较良好的文字部分的特征，整个流程大致如图2，这些特征甚至可以直接输入到文字识 别模型中进行识别，而不用做额外的处理。由于我们每一部分结果都有相应的理论基础作为支撑，因此能够模型的可靠性得到保证。

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101114401-2034339372.png)

图2：特征提取大概流程

 

在这部分的实验中，我们以图3来演示我们的效果。这个图像的特点是尺寸中等，背景较炫，色彩较为丰富，并且文字跟图片混合排版，排版格式不固定，是比较典型的电商类宣传图片。可以看到，处理这张图片的要点就是如何识别图片区域和文字区域，识别并剔除右端的电饭锅，只保留文字区域。 

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101204572-1915419152.png)

图3：小米电饭锅介绍图

 

**图像的预处理**

首先，我们将原始图片以灰度图像的形式读入，得到一个的灰度矩阵，其中m, n 是图像的长、宽。这样读入比直接读入RGB彩色图像维度更低，同时没有明显损失文字信息。转换为灰度图事实上就是将原来的RGB图像的三个通道以下面的公式![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206102105307-1381191640.png) （1）

整合为一个通道：

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101240307-39774005.png)


图3的灰度图如下图

图像本身的尺寸不大，如果直接处理，则会导致文字笔画过小，容易被当成噪音处理掉，因此为了保证文字的笔画有一定的厚度，可以先将图片进行放大。   在我们的实验中，一般将图像放大为原来的两倍就有比较好的效果了。

 

不过，图像放大之后，文字与背景之间的区分度降低了。这是因为图片放大时会使用插值算法来填补空缺部分的像素。这时候需要相应地增大区分度。经过测试，在大多数图片中，使用次数为2的“幂次变换”效果较好。幂次变换为

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101315776-959028992.png)（2）

其中x代表矩阵M中的元素，r为次数，在这里我们选取为2。 然后需要将结果映射到[0,255]区间：

   ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101343276-1703498987.png)（3）

其中 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101413494-615174302.png)是矩阵的最大值和最小值。

 

**灰色聚类**

接着我们就对图像的色彩进行聚类。聚类的有两个事实依据：

 

1. **灰度分辨率**   肉眼的灰度分辨率大概为40，因此对于像素值254和255，在我们肉眼看来都 只是白色；
2. **设计原则**   根据我们一般的审美原则，在考虑海报设计、服装搭配等搭配的时候，一般要 求在服装、海报等颜色搭配不超过三种颜色。

 

更通俗地说，虽然灰度图片色阶范围是[0, 255]，但我们能感觉到的整体的色调一般不多，因此，可以将相近的色阶归为一类，从而减少颜色分布，有效地降低噪音。

 

事实上，聚类是根据图像的特点自适应地进行多值化的过程，避免了传统的简单二值化所带来 的信息损失。由于我们需要自动地确定聚类数目，因此传统的KMeans等聚类方法被我们抛弃 了，而且经过我们测试，诸如MeanShift等可行的聚类方法又存在速度较慢等缺陷。因此，我们 自行设计了聚类方法，使用的是“核概率密度估计”的思路，通过求颜色密度极值的方式来聚类。

 

**核密度估计** 经过预处理的图像，我们可以对每个色阶的出现次数进行统计，得到如图5的频率分布直方图：

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101500619-629084631.png)


图5：对预处理后的图像进行灰色阶统计

 

可以看到，色阶的分布形成了几个比较突出的峰，换言之，存在一定的聚类趋势。 然而，直方 图的统计结果是不连续的，一个平滑的结果更便于我们分析研究，结果也更有说服力。 将统计 结果平滑化的方法，就是核密度估计(kernel density estimation)。

 

核密度估计方法是一种非参数估计方法，由Rosenblatt和Parzen提出，在统计学理论和应用领 域均受到高度的重视[2]。 当然，也可以简单地将它看成一种函数平滑方式。 我们根据大量的数据 来估计某个值出现的概率(密度)时，事实上做的是如下估算：

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101523322-491138565.png)（4）

其中K(x)称为核函数。 当 取为1，且K(x)取

​     ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101546229-238906458.png)（5）

时，就是我们上述的直方图估计。 K(x)这一项的含义很简单，它就是告诉我们在范围h内的![img](http://mmbiz.qpic.cn/mmbiz_png/L16u9H2TiaSVfqHJZvGdW9upv1uTJ857fydicw4rBRoLcMEoNKmTiaKHVVQ8d9MibxjYZZskMPRWw8MAUpP0q6M3tw/0?wx_fmt=png)都算入到x中去，至于怎么算，由![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101621651-551362376.png)给出。可见，h的选择对结果的影响很大，h我们称之为带宽(bandwidth)，它主要影响结果的平滑性。 如果K(x)是离散的，得到的结果还是离散的，但如果K(x)是光滑的，得到的结果也是比较光滑的。一个常用的光滑函数核是高斯核：

   ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101652010-1183109307.png)（6）

所得到的估计也叫高斯核密度估计。 在这里，我们使用scott规则自适应地选取 ，但需要手动指定一个平滑因子，在本文中，我们选取为0。2。对于示例图片，我们得到如图6的红色曲线的结果。

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101723322-196267127.png)


图6：频率分布的高斯核密度估计

 

**极大极小值分割**

 

从图6中我们进一步可以看出，图像确实存在着聚类趋势。 这表现为它有几个明显的极大值和极 小值点，这里的极大值点位于x = 10, 57, 97, 123, 154，极小值点位于25, 71, 121, 142。

 

因此，一个很自然的聚类方法是：有多少个极大值点，就聚为多少类，并且以极小值点作为类 别之间的边界。 也就是说，对于图3，可以将图像分层5层，逐层处理。 分层之后，每一层的形状 如下图，其中白色是1，黑色是0。

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101808260-1131263002.png)

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206101841151-470079484.png)

​                       

 通过聚类将图像分为5个图层

 

可见，由于“对比度”和“渐变性”假设，通过聚类确实可以将文字图层通过核密度估计的聚类方 法分离开来。 而且，通过聚类分层的思路，无需对文字颜色作任何假定，即便是文字颜色跟背 景颜色一致时，也可以获得有效检测。

 

**逐层识别**

当图像有效地进行分层后，我们就可以根据前面的假设，进一步设计相应的模型，通过逐层处 理的方式找出图像中的文字区域。

 

**连通性**

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206102438088-1953095319.png)

可以看到，每一层的图像是由若干连通区域组成的，文字本身是由笔画较 为密集组成的，因此往往文字也能够组成一个连通区域。这里的连通定义为 8邻接，即某个像素周围的8个像素都定义为邻接像素，邻接的像素则被定 义为同一个连通区域。

 

定义了连通区域后，每个图层被分割为若干个连通区域，也就是说，我们 逐步地将原始图像进行分解，如图9。

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206102507697-1629147238.png)

图9 图像分解结构图

 

**抗腐蚀能力**   将图像分解至连通区域这一粒度后，我们就不再细分了，下一步开始识别哪些区域是可能的文字区域。 这里我们要求文字具有一定的抗腐蚀能力。 因此我们先来定义腐蚀。

 

腐蚀是一种图像上的形态学变换，一般针对于二值图像，对于二值图像中的非零像素(即取值为 1的像素)，如果它邻接的像素都为1，则保持不变，否则变为0，这里我们同样采用的是8邻接的 定义。 可以看到，如果连通区域的边界线越长，那么腐蚀运算对它的“伤害”就越大，反之，如果 连通区域的边界线越短，那么腐蚀运算对它的“伤害”就越小。

 

根据以上腐蚀的定义，我们可以给出一个对文字区域的要求：

 

**抗腐蚀要求**        文字所在的连通区域应当具有一定的抗腐蚀能力。

 

这里的“一定”是指在一个连续的范围内，不能太大，也不能太小。       比如，一个面积较大的方形区 域，它的抗腐蚀能力是很强的，因为它边界线很短，但这些区域明显不是文字区域，上一篇文 章中分解后图层5的电饭锅便是属于这一类型；此外，抗腐蚀能力太弱也不可以，比如细长的 线条，腐蚀之后可能就消失了，这些也不作为候选的文字区域，上一篇文章中分解后图层4的 文字边界线就属于这一类型。

 

这里可以定义一个抗腐蚀能力的指标：

连通区域的抗腐蚀能力 = 该区域被腐蚀后的总面积/该区域被腐蚀前的总面积 (7)

 

经过测试，文字区域的抗腐蚀能力大概在[0.1, 0.9]这个区间中。

 

经过抗腐蚀能力筛选分解的5个图层，得到如下图的特征层。

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206102548994-59036227.png)

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206102622635-2106616777.png)

 

只保留抗腐蚀能力在[0。1, 0。9]这个区间中的连通区域

 

池化操作 到现在为止，我们得到了5个特征层，虽然肉眼可以看到，文字主要集中在第5个特征层。但是，对于一般的图片，文字可能分布在多个特征层，因此需要对特征层进行整合。我们这里进行特征整合的方法，类似于卷积神经网络中的“池化”，因此我们也借用了这个名称。 首先，我们将5个特征层进行叠加，得到一幅整体的图像特征(称为叠加特征)。这样的图像特征可以当作最后的特征输出，但并不是最好的方法。我们认为，某个区域内的主要文字特征应该已经集中分布在某个特征层中，而不是分散在所有的特征层。因此，得到叠加特征后，使用类 似“最大值池化”的方式整合特征，步骤如下：

1。直接叠加特征，然后对叠加特征划分连通区域；

2。检测每个连通区域的主要贡献是哪个特征层，该连通区域就只保留这个特征层的来源。

 

经过这样的池化操作后，得到的最终特征结果如图11。

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206102653619-1629021470.png)

图11 池化后的特征

 

**后期处理**

对于我们演示的这幅图像，经过上述操作后，得到的特征图11已经不用再做什么处理了。 然而， 对于一般的图片，还有可能出现一些没处理好的区域，这时候需要在前述结果的基础上进一步 排除。      排除过程主要有两个步骤，一个是低/高密度区排除，另外则是孤立区排除。

 

**密度排除**   一种明显不是文字区域的连通区域是低密度区，一个典型的例子就是由表格线组成的连通区域，这样的区域范围较大，但点很少，也就是密度很低，这种低密度区可以排除。 首先我们来定义连通区域密度和低密度区：

 

**连通区域密度**  从一个连通区域出发，可以找到该连通区域的水平外切矩形，该区域的密度定义为

连通区域密度 =连通区域的面积/外切矩形的面积×原图像总面积/外切矩形的面积 (8)

 

**低密度区**   如果一个连通区域的密度小于16，那么这个连通区域定义为低密度区。

 

直觉上的定义应该是连通区域的面积 / 外切矩形的面积，但这里多了一个因子原图像总面积 / 外切矩形的面积，目的是把面积大小这个影响因素加进去，因为文字一般有明显的边界，容易被分割开来，所以一般来说面积越大的区域越不可能是文本区域。这里的参数16是经验值。 低密度区排除是排除表格等线条较多的非文字区域的有效方法。类似地，范围较大的高密度区也是一类需要排除的区域。      有了低密度区之后，就很容易定义高密度区了：

 

**高密度区定义***  如果一个连通区域以水平外切矩形反转后的区域是一个低密度区，那个这个 连通区域定义为高密度区。

 

这个定义是很自然的，但是却有一定的不合理性。比如“一”字，是一个水平的矩形，于是翻转后 的密度为0，于是这个“一”字就被排除了，这是不合理的。       解决这个问题的一个方案是：

**高密度区定义**    当且仅当下面条件满足时才被定义为高密度区：

  （矩形的面积 −连通区域的面积）/外切矩形的面积× 外切矩形的面积/原图像总面积< 16         （9）

 

这是在原来定义的基础上加上了1，防止了翻转后密度为0的情况。

 

还有另外一种失效的情况，就是假如输入图片是单字图片，那么只有一个连通区域，且原图像总面积 外切矩形的面积接近于1，因此它就被判为低密度区，这样就排除了单字。这种情形确实比较难兼顾。一个可行的解决办法是通过人工指定是单字模式、单行模型还是整体图片模式，Google的Tesseract OCR也提供了这样的选项。

 

**孤立区**

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206102820166-102451950.png)

孤立区排除的出发点是：文字之间、笔画之间应该是比较紧 凑的，如果一个区域明显地孤立于其他区域，那么这个区域 很可能不是文字区域。 也就是说，可以把孤立区给排除掉。 首 先我们定义孤立区的概念：

 

**孤立区**    从一个连通区域出发，可以找到该连通区域的水平外切矩形，将这个矩形中心对称 地向外扩张为原来的9倍(长、宽变为原来的3倍，如左图)，扩展后的区域如果没有包含其他 的连通区域，那么原来的连通区域称为孤立区。

 

在大多数情况，孤立区排除是一种非常简单有效的去噪方法，因为很多噪音点都是孤立区。 但是孤立区排除是会存在一定风险的。 如果一幅图像只有一个文字，构成了唯一一个连通区域， 那么这个连通区域就是孤立的，于是这个文字就被排除了。因此，要对孤立区加上更多的限制，一个可选的额外限制是：被排除的孤立区的占连通区域的面积 / 外切矩形的面积要大于0.75（这个值源于圆与外切正方形的面积之比 π / 4）。







## 2.文字定位和文本切割



### 文字定位

经过前面的特征提取，我们已经较好地提取了图像的文本特征，下面进行文字定位。 主要过程分两步：

1、邻近搜索，目的是圈出单行文字；

2、文本切割，目的是将单行文本切割为单字。

###  **邻近搜索**

我们可以对提取的特征图进行连通区域搜索，得到的每个连通区域视为一个汉字。 这对于大多数汉字来说是适用，但是对于一些比较简单的汉字却不适用，比如“小”、“旦”、“八”、“元” 这些字，由于不具有连通性，所以就被分拆开了，如图13。 因此，我们需要通过邻近搜索算法，来整合可能成字的区域，得到单行的文本区域。

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206104509557-898009186.png)

图13 直接搜索连通区域，会把诸如“元”之类的字分拆开。

 

邻近搜索的目的是进行膨胀，以把可能成字的区域“粘合”起来.。如果不进行搜索就膨胀，那么膨胀是各个方向同时进行的，这样有可能把上下行都粘合起来了。因此，我们只允许区域向单一的一个方向膨胀。我们正是要通过搜索邻近区域来确定膨胀方向(上、下、左、右)：

> **邻近搜索\*** 从一个连通区域出发，可以找到该连通区域的水平外切矩形，将连通区域扩展到整个矩形。 当该区域与最邻近区域的距离小于一定范围时，考虑这个矩形的膨胀，膨胀的方向是最邻近区域的所在方向。

既然涉及到了邻近，那么就需要有距离的概念。下面给出一个比较合理的距离的定义。

 **距离**

 

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206104536010-554953070.png)

 

图14 两个示例区域

 

如上图，通过左上角坐标(x,y)和右下角坐标(z,w)就可以确定一个矩形区域，这里的坐标是以左上角为原点来算的。 这个区域的中心是( (x+z) / 2, (y+w) / 2 )。对于图中的两个区域S和S′，可以计算它们的中心向量差

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206104611572-1888349208.png)

如果直接使用$\sqrt {x_c^2+y_c^2}$作为距离是不合理的，因为这里的邻近应该是按边界来算，而不是中心点。因此，需要减去区域的长度：
$$
({x'}_c,{y'}_c)
$$


 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206104635213-1094449173.png)

距离定义为

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206104655151-1295815489.png)

至于方向，由![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206104715276-1606705471.png)的幅角进行判断即可。

 

然而，按照前面的“邻近搜索*”方法，容易把上下两行文字粘合起来，因此，基于我们的横向排版假设，更好的方法是只允许横向膨胀：

>  
>
> **邻近搜索**   从一个连通区域出发，可以找到该连通区域的水平外切矩形，将连通区域扩展到整个矩形。 当该区域与最邻近区域的距离小于一定范围时，考虑这个矩形的膨胀，膨胀的方向是最邻近区域的所在方向，当且仅当所在方向是水平的，才执行膨胀操作。

**结果**

 

有了距离之后，我们就可以计算每两个连通区域之间的距离，然后找出最邻近的区域。 我们将每个区域向它最邻近的区域所在的方向扩大4分之一，这样邻近的区域就有可能融合为一个新的区域，从而把碎片整合。

 

实验表明，邻近搜索的思路能够有效地整合文字碎片，结果如图15。

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206104740901-184421191.png)

图15 通过邻近搜索后，圈出的文字区域

### 文本切割

经过前面文字定位得到单行的文本区域之后，我们就可以想办法将单行的文本切割为单个的字符了。因为第三步的模型师针对单个的字符建立的，因此这一步也是必须的。

 

### **均匀切割**

基于方块汉字的假设，事实上最简单的切割方法是均匀切割，也就是说不加任何判断，直接按照高度来将单行文本切割为一个个的正方形图片。这种思路可以应对大部分的单行文本，如下图。


![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206105512151-2068646492.png)

当然，均匀切割的弊端也是很明显的。 大多数汉字都是方块汉字，但多数英语和数字都不是，因此如果出现中英文混排的时候，均匀切割就失效了，如上图。

 

### **统计切割** 

 

从图15中可以看出，经过前面的操作，字与字都被很好地分离开了。 因此，另外一种比较简单的思路是对单行的文字图片进行垂直方向的求和，和为0的所在的列就是被切割的的列。

 

用这种统计的思路可以很好地解决中英文混排的单行文字图片分割的问题，但是它也存在一定的弊端。 最明显的就是诸如“小”、“的”等字就被切割开为两部分了。

 

### **前后比较**

 

一个更好的思路是结合前面两部分结果，通过比较前后两部分区域是否组成方形来确定是否切割。 具体步骤是：

 

1。 通过统计求和的思路，得出候选的切割线；

 

2。 如果该候选切割线到左右两条候选切割线的距离之和超过宽长度的1。2倍，那么该候选切割线确定为切割线；

 

3。 如果得到的区域是一个明显的长条矩形，并且没办法按照上面两个步骤切割，那个就均匀切割。

 

这三个步骤比较简单，基于两个假设：

1、数字、英文字符的底与高之比大于60%；

2、汉字的底与高之比低于1.2。经过测试，该算法可以很好地用于前面步骤所提取的图片文本特征的分割。









## 3.光学识别



经过前面的文字定位和文本切割，我们已经能够找出图像中单个文字的区域，接下来可以建立相应的模型对单字进行识别。

 

### **模型选择**

在模型方面，我们选择了深度学习中的卷积神经网络模型，通过多层卷积神经网络，构建了单字的识别模型。

 

卷积神经网络是人工神经网络的一种，已成为当前图像识别领域的主流模型。 它通过局部感知野和权值共享方法，降低了网络模型的复杂度，减少了权值的数量，在网络结构上更类似于生物神经网络，这也预示着它必然具有更优秀的效果。 事实上，我们选择卷积神经网络的主要原因有：

 

1、对原始图像自动提取特征 卷积神经网络模型可以直接将原始图像进行输入，免除了传统模型的人工提取特征这一比较困难的核心部分；

 

2、比传统模型更高的精度 比如在MNIST手写数字识别任务中，可以达到99%以上的精度，这远高于传统模型的精度；

 

3、 比传统模型更好的泛化能力 这意味着图像本身的形变(伸缩、旋转)以及图像上的噪音对识别的结果影响不明显，这正是一个良好的OCR系统所必需的。

 

### **训练数据**

为了训练一个良好的模型，必须有足够多的训练数据。幸运的是，虽然没有现成的数据可以用，但是由于我们只是做印刷字体的识别，因此，我们可以使用计算机自动生成一批训练数据。通过以下步骤，我们构建了一批比较充分的训练数据：

 

1。 更多细节 由于汉字的结构比数字和英文都要复杂，因此，为了体现更多的细节信息，我使用48×48的灰度图像构建样本，作为模型的输入；

 

2。 常见汉字 为了保证模型的实用性，我们从网络爬取了数十万篇微信公众平台上的文章，然后合并起来统计各自的频率，最后选出了频率最高的3000个汉字(在本文中我们只考虑简体字)，并且加上26个字母(大小写)和10个数字，共3062字作为模型的输出；

 

3。 数据充分 我们人工收集了45种不同的字体，从正规的宋体、黑体、楷体到不规范的手写体都有，基本上能够比较全面地覆盖各种印刷字体；

 

4。 人工噪音 每种字体都构建了5种不同字号(46到50)的图片，每种字号2张，并且为了增强模型的泛化能力，将每个样本都加上5%的随机噪音。

 

 

### **模型结构**

在模型结构方面，有一些前人的工作可以参考的。一个类似的例子是MNIST手写数字的识别——它往往作为一个新的图像识别模型的“试金石”——是要将六万多张大小为28×28像素的手写数字图像进行识别，这个案例跟我们实现汉字的识别系统具有一定的相似性，因此在模型的结构方面可以借鉴。一个常见的通过卷积神经网络对MNIST手写数字进行识别的模型结构如图

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206105852713-1914148433.png)

 图18 本文用来识别印刷汉字的网络结构

 

经过充分训练后，如图17的网络结构可以达到99%以上的精确度，说明这种结构确实是可取的。但是很显然，手写数字不过只有10个，而常用汉字具有数千个，在本文的分类任务中，就共有3062个目标。也就是说，汉字具有更为复杂和精细的结构，因此模型的各方面都要进行调整。首先，在模型的输入方面，我们已经将图像的大小从28x28提高为48x48，这能保留更多的细节，其次，在模型结构上要复杂化调整，包括：增加卷积核的数目，增加隐藏节点的数目、调整权重等。最终我们的网络结构如图18。

 

在激活函数方面，我们选取了RuLe函数为激活函数

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206105939401-832971795.png)

 

实验表明，它相比于传统的sigmoid、tanh等激活函数，能够大大地提升模型效果[3][4]；在防止过拟合方面，我们使用了深度学习网络中最常用的Dropout方式[5]，即随机地让部分神经元休眠，这等价于同时训练多个不同网络，从而防止了部分节点可能出现的过拟合现象。

 

需要指出的是，在模型结构方面，我们事实上做了大量的筛选工作。比如隐藏层神经元的数目，我们就耗费了若干天时间，尝试了512、1024、2048、4096、8192等数目，最终得到1024这个比较适合的值。数目太多则导致模型太庞大，而且容易过拟合；太少则容易欠拟合，效果不好。我们的测试发现，从512到1024，效果有明显提升；而再增加节点效果没有明显提升，有时还会有明显下降。

 

### **模型实现**

我们的模型在操作系统为CentOS 7的服务器(24核CPU+96G内存+GTX960显卡)下完成，使用Python 2。7编写代码，并且使用Keras作为深度学习库，用Theano作为GPU加速库(Tensorflow一直提示内存溢出，配置不成功。 )。

 

在训练算法方面，使用了Adam优化方法进行训练，batch size为1024，迭代30次，迭代一次大约需要700秒。

 

如果出现形近字时，应该是高频字更有可能，最典型的例子就是“日”、“曰”了，这两个的特征是很相似的，但是“日”出现的频率远高于“曰”，因此，应当优先考虑“日”。 因此，在训练模型的时候，我们还对模型最终的损失函数进行了调整，使得高频字的权重更大，这样能够提升模型的预测性能。

 

经过多次调试，最终得到了一个比较可靠的模型。 模型的收敛过程如下图。

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110020572-1935530260.png)

训练曲线图：Loss(损失函数)和Acc(精度)

 

### **模型检验**

我们将从以下三个方面对模型进行检验。 实验结果表明，对于单字的识别效果，我们的模型优于Google开源的OCR系统Tesseract。

 

### **训练集检验**

最终训练出来的模型，在训练集的检验报告如表1。

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110043416-1680373814.png)

从表1可以看到，即便在加入了随机噪音的样本中，模型的正确率仍然有99。7%，因此，我们有把握地说，单纯从单字识别这部分来看，我们的结果已经达到了state of the art级别，而且在黑体、宋体等正规字体中(正规字体样本是指所有训练样本中，字体为黑体、宋体、楷体、微软雅黑和Arial unicode MS的训练样本，这几种字体常见于印刷体中。)，正确率更加高！

 

### **测试集检验**

我们另外挑选了5种字体，根据同样的方法生成了一批测试样本(每种字体30620张，共153100张)，用来对模型进行测试，得到模型测试正确率为92。11%。 五种字体的测试结果如表2。

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110121135-337693193.png)

从表中可以看出，即便是对于训练集之外的样本，模型效果也相当不错。接着，我们将随机噪音增大到15%（这对于一张48×48的文字图片来说已经相当糟糕了），得到的测试结果如表3。

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110138651-335935961.png)

平均的正确率为87。59%，也就是说，噪音的影响并不明显，模型能够保持90%左右的正确率。 这说明该模型已经完全达到了实用的程度。









## 4.语言模型和综合评估



### **语言模型**

由于图像质量等原因，性能再好的识别模型，都会有识别错误的可能性，为了减少识别错误率，可以将识别问题跟统计语言模型结合起来，通过动态规划的方法给出最优的识别结果。这是改进OCR识别效果的重要方法之一。

###  **转移概率**

在我们分析实验结果的过程中，有出现这一案例。由于图像不清晰等可能的原因，导致“电视”一词被识别为“电柳”，仅用图像模型是不能很好地解决这个问题的，因为从图像模型来看，识别为“电柳”是最优的选择。但是语言模型却可以很巧妙地解决这个问题。原因很简单，基于大量的文本数据我们可以统计“电视”一词和“电柳”一词的概率，可以发现“电视”一词的概率远远大于“电柳”，因此我们会认为这个词是“电视”而不是“电柳”。

 

从概率的角度来看，就是对于第一个字的区域的识别结果s1，我们前面的卷积神经网络给出了“电”、“宙”两个候选字(仅仅选了前两个，后面的概率太小)，每个候选字的概率W(s1)分别为0.99996、0.00004；第二个字的区域的识别结果s2，我们前面的卷积神经网络给出了“柳”、“视”、“规”(仅仅选了前三个，后面的概率太小)，每个候选字的概率W(s2)分别为0.87838、0.12148、0.00012，因此，它们事实上有六种组合：“电柳”、“电视”、“电规”、“宙柳”、“宙视”、“宙规”。

下面考虑它们的迁移概率。所谓迁移概率，其实就是条件概率P(s1|s2)，即当s1出现时后面接s2的概率。通过10万微信文本，我们统计出，“电”字出现的次数为145001，而“电柳”、“电视“、”电规“出现的次数为0、12426、7；“宙”字出现的次数为1980次，而“宙柳”、“宙视”、“宙规”出现的次数为0、0、18，因此，可以算出

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110506697-1415108315.png)

结果如下图：

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110531979-104512350.png)

 

图20 考虑转移概率

 

从统计的角度来看，最优的s1,s2组合，应该使得式(14)取最大值：

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110553510-1599018874.png)

因此，可以算得s1,s2的最佳组合应该是“电视”而不是“电柳”。这时我们成功地通过统计的方法得到了正确结果，从而提高了正确率。

 

### **动态规划**

![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110615916-215089358.png)

图21 多字图片的规划问题

 

类似地，如图21，如果一个单行文字图片有n个字![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110707072-1446775557.png)需要确定，那么应当使得

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110732666-177765352.png)

取得最大值，这就是统计语言模型的思想，自然语言处理的很多领域，比如中文分词、语音识别、图像识别等，都用到了同样的方法[6]。这里需要解决两个主要的问题：(1)各个![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110754713-834948736.png)的估计；(2)给定各个![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110808776-388250683.png)后如何求解f的最大值。

 

### **转移概率矩阵**

对于第一个问题，只需要从大的语料库中统计si的出现次数#si，以及si,si+1相接地出现的次数#(si,si+1)，然后认为

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110833260-1704873154.png)

即可，本质上没有什么困难。本文的识别对象有3062个，理论上来说，应该生成一个3062×3062的矩阵，这是非常庞大的。当然，这个矩阵是非常稀疏的，我们可以只保存那些有价值的元素。

 

现在要着重考虑当#(si,si+1)=0的情况。在前一节我们就直接当P(si|si+1)=0，但事实上是不合理的。没有出现不能说明不会出现，只能说明概率很小，因此，即便是对于#(si,si+1)=0，也应该赋予一个小概率而不是0。这在统计上称为数据的平滑问题。

 

一个简单的平滑方法是在所有项的频数(包括频数为0的项)后面都加上一个正的小常数α(比如1)，然后重新统计总数并计算频率，这样每个项目都得到了一个正的概率。这种思路有可能降低高频数的项的概率，但由于这里的概率只具有相对意义，因此这个影响是不明显的(一个更合理的思路是当频数小于某个阈值T时才加上常数，其他不加。)。按照这种思路，从数十万微信文章中，我们计算得到了160万的邻接汉字的转移概率矩阵。

### **Viterbi算法**

对于第二个问题，求解最优组合![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110859213-457031314.png)是属于动态规划中求最优路径的问题，其中最有效的方法是Viterbi算法[6]。

 

Viterbi算法是一个简单高效的算法，用Python实现也就十来行的代码。它的核心思想是：如果最终的最优路径经过某个si−1，那么从初始节点到si−1点的路径必然也是一个最优路径——因为每一个节点si只会影响前后两个P(si−1|si)和P(si|si+1)。

 

根据这个思想，可以通过递推的方法，在考虑每个si时只需要求出所有经过各si−1的候选点的最优路径，然后再与当前的si结合比较。这样每步只需要算不超过 次，就可以逐步找出最优路径。Viterbi算法的效率是![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110920307-692371941.png)，l 是候选数目最多的节点si的候选数目，它正比于n，这是非常高效率的。

 

### **提升效果**

实验表明，结合统计语言模型进行动态规划能够很好地解决部分形近字识别错误的情况。在我们的测试中，它能修正一些错误如下：

 ![img](https://images2015.cnblogs.com/blog/732403/201702/732403-20170206110947088-1343180871.png)

通过统计语言模型的动态规划能修正不少识别错误

 

由于用来生成转移矩阵的语料库不够大，因此修正的效果还有很大的提升空间。不管怎么说，由于Viterbi算法的简单高效，这是一个性价比很高的步骤。

 

### **综合评估**

#### **1、数据验证**

尽管在测试环境下模型工作良好，但是实践是检验真理的唯一标准。在本节中，我们通过自己的模型，与京东的测试数据进行比较验证。

 

衡量OCR系统的好坏有两部分内容：**(1)**是否成功地圈出了文字；**(2)**对于圈出来的文字，有没有成功识别。我们采用评分的方法，对每一张图片的识别效果进行评分。评分规则如下：

 

- 如果圈出的文字区域能够跟京东提供的检测样本的box文件中匹配，那么加1分，如果正确识别出文字来，另外加1分，最后每张图片的分数是前面总分除以文字总数。

 

按照这个规则，每张图片的评分最多是2分，最少是0分。如果评分超过1，说明识别效果比较好了。经过京东的测试数据比较，我们的模型平均评分大约是0.84，效果差强人意。

 

#### **2、模型综述**

在本文中，我们的目标是建立一个完整的OCR系统，经过一系列的工作，我们也基本完成了这一目标。

 

在设计算法时，我们紧密地结合基本假设，从模拟人肉眼的识别思路出发，希望能够以最少的步骤来实现目标，这种想法在特征提取和文字定位这两部分得到充分体现。

 

同样地，由于崇尚简洁和模拟人工，在光学字符识别方面，我们选择了卷积神经网络模型，得到了较高的正确率；最后结合语言模型，通过动态规划用较简单的思路提升了效果。

 

经过测试，我们的系统对印刷文字的识别有着不错的效果，可以作为电商、微信等平台的图片文字识别工具。其中明显的特点是，我们的系统可以将整张文字图片输入，并且在分辨率不高的情况下能够获得较好的效果。

 

#### **3、结果反思**

在本文所涉及到的算法中，一个很大的不足之处就是有很多的“经验参数”，比如聚类时h参数的选择、低密度区定义中密度的阈值、卷积神经网络中的卷积核数据、隐藏层节点数目等。由于并没有足够多的标签样本进行研究，因此，这些参数都只能是凭借着经验和少量的样本推算得出。我们期待会有更多的标签数据来得到这些参数的最优值。

 

还有，在识别文字区域方面，还有很多值得改进的地方。虽然我们仅仅是经过几个步骤就去掉了大部分的文字区域，但是这些步骤还是欠直观，亟待简化。我们认为，一个良好的模型应该是基于简单的假设和步骤就能得到不错的效果，因此，值得努力的工作之一就是简化假设，缩减流程。

 

此外，在文本切割方面，事实上不存在一种能够应对任何情况的自动切割算法，因此这一步还有很大的提升空间。据相关文献，可以通过CNN+LSTM模型，直接对单行文本进行识别，但这需要大量的训练样本和高性能的训练机器，估计只有大型企业才能做到这一点。

 

显然，还有很多工作都需要更深入地研究。









## 5.Python示例



### 文件说明：

1. image.py——图像处理函数，主要是特征提取；

2. model_training.py——训练CNN单字识别模型（需要较高性能的服务器，最好有GPU加速，否则真是慢得要死）；

3. ocr.py——识别函数，包括单字分割、前面训练好的模型进行单字识别、动态规划提升效果；

4. main.py——主文件，用来调用1、3两个文件。

5. [模型中包含的字.txt](https://pan.baidu.com/s/1PtaK2j4FzITzteyPuiYRWA)(UTF-8编码);

### 文件1:image.py

```python
# -*- coding:utf-8 -*-

import numpy as np
from scipy import misc,ndimage
from scipy.stats import gaussian_kde as kde
from tqdm import *

def myread(filename): #读取图像，放大两倍，做平方变换
    print u'读取图片中...'
    pic = misc.imread(filename, flatten = True)
    pic = ndimage.zoom(pic, 2)
    pic = pic**2
    pic = ((pic-pic.min())/(pic.max()-pic.min())*255).round()
    print u'读取完成.'
    return pic

def decompose(pic): #核密度聚类，给出极大值、极小值点、背景颜色、聚类图层
    print u'图层聚类分解中...'
    d0 = kde(pic.reshape(-1), bw_method=0.2)(range(256)) #核密度估计
    d = np.diff(d0)
    d1 = np.where((d[:-1]<0)*(d[1:]>0))[0] #极小值
    d1 = [0]+list(d1)+[256]
    d2 = np.where((d[:-1]>0)*(d[1:]<0))[0] #极大值
    if d1[1] < d2[0]:
        d2 = [0]+list(d2)
    if d1[len(d1)-2] > d2[len(d2)-1]:
        d2 = list(d2)+[255]
    dc = sum(map(lambda i: d2[i]*(pic >= d1[i])*(pic < d1[i+1]), range(len(d2))))
    print u'分解完成. 共%s个图层'%len(d2)
    return dc

def erosion_test(dc): #抗腐蚀能力测试
    print u'抗腐蚀能力测试中...'
    layers = []
    #bg = np.argmax(np.bincount(dc.reshape(-1)))
    #d = [i for i in np.unique(dc) if i != bg]
    d = np.unique(dc)
    for k in d:
        f = dc==k
        label_im, nb_labels = ndimage.label(f, structure=np.ones((3,3))) #划分连通区域
        ff = ndimage.binary_erosion(f) #腐蚀操作
        def test_one(i):
            index = label_im==i
            if (1.0*ff[index].sum()/f[index].sum() > 0.9) or (1.0*ff[index].sum()/f[index].sum() < 0.1):
                f[index] = False
        ff = map(test_one, trange(1, nb_labels+1))
        layers.append(f)
    print u'抗腐蚀能力检测完毕.'
    return layers

def pooling(layers): #以模仿池化的形式整合特征
    print u'整合分解的特征中...'
    result = sum(layers)
    label_im, nb_labels = ndimage.label(result, structure=np.ones((3,3)))
    def pool_one(i):
        index = label_im==i
        k = np.argmax([1.0*layers[j][index].sum()/result[index].sum() for j in range(len(layers))])
        result[index] = layers[k][index]
    t = map(pool_one, trange(1, nb_labels+1))
    print u'特征整合成功.'
    return result

def post_do(pic):
    label_im, nb_labels = ndimage.label(pic, structure=np.ones((3,3)))
    print u'图像的后期去噪中...'
    def post_do_one(i):
        index = label_im==i
        index2 = ndimage.find_objects(index)[0]
        ss = 1.0 * len(pic.reshape(-1))/len(pic[index2].reshape(-1))**2
        #先判断是否低/高密度区，然后再判断是否孤立区。
        if (index.sum()*ss < 16) or ((1+len(pic[index2].reshape(-1))-index.sum())*ss < 16):
            pic[index] = False
        else:
            a,b,c,d = index2[0].start, index2[0].stop, index2[1].start, index2[1].stop
            index3 = (slice(max(0, 2*a-b),min(pic.shape[0], 2*b-a)), slice(max(0, 2*c-d),min(pic.shape[1], 2*d-c)))
            if (pic[index3].sum() == index.sum()) and (1.0*index.sum()/(b-a)/(d-c) > 0.75):
                pic[index2] = False    
    t = map(post_do_one, trange(1, nb_labels+1))
    print u'后期去噪完成.'
    return pic

def areas(pic): #圈出候选区域
    print u'正在生成候选区域...'
    pic_ = pic.copy()
    label_im, nb_labels = ndimage.label(pic_, structure=np.ones((3,3)))
    def areas_one(i):
        index = label_im==i
        index2 = ndimage.find_objects(index)[0]
        pic_[index2] = True
    t = map(areas_one, trange(1, nb_labels+1))
    return pic_

#定义距离函数，返回值是距离和方向
#注意distance(o1, o2)与distance(o2, o1)的结果是不一致的
def distance(o1, o2): 
    delta = np.array(o2[0])-np.array(o1[0])
    d = np.abs(delta)-np.array([(o1[1]+o2[1])/2.0, (o1[2]+o2[2])/2.0])
    d = np.sum(((d >= 0)*d)**2)
    theta = np.angle(delta[0]+delta[1]*1j)
    k = 1
    if np.abs(theta) <= np.pi/4:
        k = 4
    elif np.abs(theta) >= np.pi*3/4:
        k = 2
    elif np.pi/4 < theta < np.pi*3/4:
        k = 1
    else:
        k = 3
    return d, k

def integrate(pic, k=0): #k=0是全向膨胀，k=1仅仅水平膨胀
    label_im, nb_labels = ndimage.label(pic, structure=np.ones((3,3)))
    def integrate_one(i):
        index = label_im==i
        index2 = ndimage.find_objects(index)[0]
        a,b,c,d = index2[0].start, index2[0].stop, index2[1].start, index2[1].stop
        cc = ((a+b)/2.0,(c+d)/2.0)
        return (cc, b-a, d-c)
    print u'正在确定区域属性...'
    A = map(integrate_one, trange(1, nb_labels+1))
    print u'区域属性已经确定，正在整合邻近区域...'
    aa,bb = pic.shape
    pic_ = pic.copy()
    def areas_one(i):
        dist = [distance(A[i-1], A[j-1]) for j in range(1, nb_labels+1) if i != j]
        dist = np.array(dist)
        ext = dist[np.argsort(dist[:,0])[0]] #通过排序找最小，得到最邻近区域
        if ext[0] <= (min(A[i-1][1],A[i-1][2])/4)**2:
            ext = int(ext[1])
            index = label_im==i
            index2 = ndimage.find_objects(index)[0]
            a,b,c,d = index2[0].start, index2[0].stop, index2[1].start, index2[1].stop
            if ext == 1: #根据方向来膨胀
                pic_[a:b, c:min(d+(d-c)/4,bb)] = True
            elif ext == 3:
                pic_[a:b, max(c-(d-c)/4,0):d] = True
            elif ext == 4 and k == 0:
                pic_[a:min(b+(b-a)/6,aa), c:d] = True #基于横向排版假设，横向膨胀要大于竖向膨胀
            elif k == 0:
                pic_[max(a-(b-a)/6,0):b, c:d] = True
    t = map(areas_one, trange(1, nb_labels+1))
    print u'整合完成.'
    return pic_

def cut_blank(pic): #切除图片周围的白边，返回范围
    try:
        q = pic.sum(axis=1)
        ii,jj = np.where(q!= 0)[0][[0,-1]]
        xi = (ii, jj+1)
        q = pic.sum(axis=0)
        ii,jj = np.where(q!= 0)[0][[0,-1]]
        yi = (ii, jj+1)
        return [xi, yi]
    except:
        return [(0,1),(0,1)]

def trim(pic, pic_, prange=5): #剪除白边，删除太小的区域
    label_im, nb_labels = ndimage.label(pic_, structure=np.ones((3,3)))
    def trim_one(i):
        index = label_im==i
        index2 = ndimage.find_objects(index)[0]
        box = (pic*index)[index2]
        [(a1,b1), (c1,d1)] = cut_blank(box)
        pic_[index] = False
        if (b1-a1 < prange) or (d1-c1 < prange) or ((b1-a1)*(d1-c1) < prange**2): #删除小区域
            pass
        else: #恢复剪除白边后的区域
            a,b,c,d = index2[0].start, index2[0].stop, index2[1].start, index2[1].stop
            pic_[a+a1:a+b1,c+c1:c+d1] = True
    t = map(trim_one, trange(1, nb_labels+1))
    return pic_

def bound(m):
    frange = (slice(m.shape[0]-1), slice(m.shape[1]-1))
    f0 = np.abs(np.diff(m, axis=0))
    f1 = np.abs(np.diff(m, axis=1))
    f2 = np.abs(m[frange]-m[1:,1:])
    f3 = f0[frange]+f1[frange]+f2[frange] != 0
    return f3

def trim_bound(pic, pic_): #剪除白边，删除太小的区域
    pic_ = pic_.copy()
    label_im, nb_labels = ndimage.label(pic_, structure=np.ones((3,3)))
    def trim_one(i):
        index = label_im==i
        index2 = ndimage.find_objects(index)[0]
        box = pic[index2]
        if 1.0 * bound(box).sum()/box.sum() < 0.15:
            pic_[index] = False
    t = map(trim_one, trange(1, nb_labels+1))
    return pic_
```



### 文件2:model_training.py

```python
# -*- coding:utf-8 -*-

import numpy as np
from PIL import Image, ImageFont, ImageDraw
import pandas as pd
import glob

#包含的汉字列表（太长，仅仅截取了一部分）
hanzi = u'0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz的一是不人有了在你我个大中要这为上生时会以就子到来可能和自们年多发心好用家出关长他成天对也小后下学都点国过地行信方得最说二业分作如看女于面注别经动公开现而美么还事'

#生成文字矩阵
def gen_img(text, size=(48,48), fontname='simhei.ttf', fontsize=48):
    im = Image.new('1', size, 1)
    dr = ImageDraw.Draw(im)
    font = ImageFont.truetype(fontname, fontsize)
    dr.text((0, 0), text, font=font)
    return (((np.array(im.getdata()).reshape(size)==0)+(np.random.random(size)<0.05)) != 0).astype(float)

#生成训练样本
data = pd.DataFrame()
fonts = glob.glob('./*.[tT][tT]*')
for fontname in fonts:
    print fontname
    for i in range(-2,3):
        m = pd.DataFrame(pd.Series(list(hanzi)).apply(lambda s:[gen_img(s, fontname=fontname, fontsize=48+i)]))
        m['label'] = range(3062)
        data = data.append(m, ignore_index=True)
        m = pd.DataFrame(pd.Series(list(hanzi)).apply(lambda s:[gen_img(s, fontname=fontname, fontsize=48+i)]))
        m['label'] = range(3062)
        data = data.append(m, ignore_index=True)

x = np.array(list(data[0])).astype(float)
np.save('x', x) #保存训练数据

dic=dict(zip(range(3062),list(hanzi))) #构建字表

from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.utils import np_utils

batch_size = 1024
nb_classes = 3062
nb_epoch = 30

img_rows, img_cols = 48, 48
# number of convolutional filters to use
nb_filters = 64
# size of pooling area for max pooling
nb_pool = 2
# convolution kernel size
nb_conv = 4

x = np.load('x.npy')
y = np_utils.to_categorical(range(3062)*45*5*2, nb_classes)
weight = ((3062-np.arange(3062))/3062.0+1)**3
weight = dict(zip(range(3063),weight/weight.mean())) #调整权重，高频字优先

model = Sequential()

model.add(Convolution2D(nb_filters, nb_conv, nb_conv,
                        border_mode='valid',
                        input_shape=(1, img_rows, img_cols)))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(nb_pool, nb_pool)))
model.add(Dropout(0.25))
model.add(Convolution2D(nb_filters, nb_conv, nb_conv))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(nb_pool, nb_pool)))
model.add(Dropout(0.25))

model.add(Flatten())
model.add(Dense(1024))
model.add(Activation('relu'))
model.add(Dropout(0.5))

model.add(Dense(nb_classes))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

history = model.fit(x, y,
                    batch_size=batch_size, nb_epoch=nb_epoch,
                    class_weight=weight)

score = model.evaluate(x,y)
print('Test score:', score[0])
print('Test accuracy:', score[1])

model.save_weights('model.model')
```



### 文件3:ocr.py

```python
# -*- coding:utf-8 -*-

import numpy as np
from scipy import misc
from images import cut_blank

#包含的汉字列表（太长了，仅截取了一部分）
hanzi = u'0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz的一是不人有了在你我个大中要这为上生时会以就子到来可能和自们年多发心好用家出关长他成天对也小后下学都点国过地行信方得最说二业分作如看女于面注别经动公开现而美么还事'

dic=dict(zip(range(3062),list(hanzi))) #构建字表

from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.utils import np_utils

batch_size = 128
nb_classes = 3062
img_rows, img_cols = 48, 48
nb_filters = 64
nb_pool = 2
nb_conv = 4

model = Sequential()

model.add(Convolution2D(nb_filters, nb_conv, nb_conv,
                        border_mode='valid',
                        input_shape=(1, img_rows, img_cols)))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(nb_pool, nb_pool)))
model.add(Dropout(0.25))

model.add(Convolution2D(nb_filters, nb_conv, nb_conv))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(nb_pool, nb_pool)))
model.add(Dropout(0.25))

model.add(Flatten())
model.add(Dense(1024))
model.add(Activation('relu'))
model.add(Dropout(0.5))

model.add(Dense(nb_classes))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

model.load_weights('ocr.model')

import pandas as pd
zy = pd.read_csv('zhuanyi.csv', encoding='utf-8', header=None)
zy.set_index(0, inplace=True)
zy = zy[1]

def viterbi(nodes):
    paths = nodes[0]
    for l in range(1,len(nodes)):
        paths_ = paths.copy()
        paths = {}
        for i in nodes[l].keys():
            nows = {}
            for j in paths_.keys():
                try:
                    nows[j+i]= paths_[j]*nodes[l][i]*zy[j[-1]+i]
                except:
                    nows[j+i]= paths_[j]*nodes[l][i]*zy[j[-1]+'XX']
            k = np.argmax(nows.values())
            paths[nows.keys()[k]] = nows.values()[k]
    return paths.keys()[np.argmax(paths.values())]

# mode为direact和search
#前者直接给出识别结果，后者给出3个字及其概率（用来动态规划）
def ocr_one(m, mode='direact'):
    m = m[[slice(*i) for i in cut_blank(m)]]
    if m.shape[0] >= m.shape[1]:
        p = np.zeros((m.shape[0],m.shape[0]))
        p[:,:m.shape[1]] = m
    else:
        p = np.zeros((m.shape[1],m.shape[1]))
        x = (m.shape[1]-m.shape[0])/2
        p[:m.shape[0],:] = m
    m = misc.imresize(p,(46,46), interp='nearest') #这步和接下来几步，归一化图像为48x48
    p = np.zeros((48, 48))
    p[1:47,1:47] = m 
    m = p
    m = 1.0 * m / m.max()
    k = model.predict(np.array([[m]]), verbose=0)[0]
    ks = k.argsort()
    if mode == 'direact':
        if k[ks[-1]] > 0.5:
            return dic[ks[-1]]
        else:
            return ''
    elif mode == 'search':
        return {dic[ks[-1]]:k[ks[-1]],dic[ks[-2]]:k[ks[-2]],dic[ks[-3]]:k[ks[-3]]}

'''
#直接调用Tesseract
import os
def ocr_one(m):
    misc.imsave('tmp.png', m)
    os.system('tesseract tmp.png tmp -l chi_sim -psm 10')
    s = open('tmp.txt').read()
    os.system('rm tmp.txt \n rm tmp.png')
    return s.strip()
'''

def cut_line(pl): #mode为direact或viterbi
    pl = pl[[slice(*i) for i in cut_blank(pl)]]
    pl0 = pl.sum(axis=0)
    pl0 = np.where(pl0==0)[0]
    if len(pl0) > 0:
        pl1=[pl0[0]]
        t=[pl0[0]]
        for i in pl0[1:]:
            if i-pl1[-1] == 1:
                t.append(i)
                pl1[-1]=i
            else:
                pl1[-1] = sum(t)/len(t)
                t = [i]
                pl1.append(i)
        pl1[-1] = sum(t)/len(t)
        pl1 = [0] + pl1 + [pl.shape[1]-1]
        cut_position = [1.0*(pl1[i+1]-pl1[i-1])/pl.shape[0] > 1.2 for i in range(1,len(pl1)-1)]
        cut_position=[pl1[1:-1][i] for i in range(len(pl1)-2) if cut_position[i]] #简单的切割算法
        cut_position = [0] + cut_position + [pl.shape[1]-1]
    else:
        cut_position = [0, pl.shape[1]-1]
    l = len(cut_position)
    for i in range(1, l):
        j = int(round(1.0*(cut_position[i]-cut_position[i-1])/pl.shape[0]))
        ab = (cut_position[i]-cut_position[i-1])/max(j,1)
        cut_position = cut_position + [k*ab+cut_position[i-1] for k in range(1, j)]
    cut_position.sort()
    return pl, cut_position

def ocr_line(pl, mode='viterbi'): #mode为direact或viterbi
    pl, cut_position = cut_line(pl)
    if mode == 'viterbi':
        text = map(lambda i: ocr_one(pl[:,cut_position[i]:cut_position[i+1]+1], mode='search'), range(len(cut_position)-1))
        return viterbi(text)
    elif mode == 'direact':
        text = map(lambda i: ocr_one(pl[:,cut_position[i]:cut_position[i+1]+1]), range(len(cut_position)-1))
        ''.join(text)
```



### 文件4:main.py

```python
# -*- coding:utf-8 -*-

from scipy import ndimage
print u'加载图片工具中...'
from images import *
print u'加载OCR模型中...'
from ocr import *
print u'加载完毕.'

if __name__ == '__main__':
    filename = '../cn.jpg'
    p = myread(filename)
    dc = decompose(p)
    layers = erosion_test(dc)
    result = pooling(layers)
    result = post_do(result)
    result_ = areas(result)
    result_ = integrate(result_, 1)
    result_ = trim(result, result_)
    result_ = integrate(result_, 1)
    result_ = trim(result, result_, 10)
    result_ = trim_bound(result, result_)
    label_im, nb_labels = ndimage.label(result_, structure=np.ones((3,3)))
    for i in range(1, nb_labels+1):
        index = label_im==i
        index2 = ndimage.find_objects(index)[0]
        print ocr_line(result[index2])
```



 



