# 运动目标跟踪（十九）--TLD - 工作笔记 - CSDN博客





2016年08月22日 15:20:26[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：12815
个人分类：[运动跟踪](https://blog.csdn.net/App_12062011/article/category/6269524)

所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









简单整理了下，该有的都有了，好乱。。。

TLD是一种算法的简称，原作者把它叫做Tracking-Learning-Detection。

作者网站链接：[http://personal.ee.surrey.ac.uk/Personal/Z.Kalal/](http://personal.ee.surrey.ac.uk/Personal/Z.Kalal/)

论文：

（1）Tracking-Learning-Detection

（2）Forward-Backward
 Error Automatic Detection of Tracking Failures

（3）Online
 learning of robust object detectors during unstable tracking


源代码资源：


1. 原作者 Zdenek Kalal

作者主页： http://info.ee.surrey.ac.uk/Personal/Z.Kalal/

源代码页: https://github.com/zk00006/OpenTLD

编程语言：Matlab + C


2. Alan Torres版

源代码页：https://github.com/alantrrs/OpenTLD

实现语言：C++


博客资源（中文）：


1. TLD（Tracking-Learning-Detection）学习与源码理解 (zouxy09)


http://blog.csdn.net/zouxy09/article/details/7893011


2. 《再谈PN学习》：


http://blog.csdn.net/carson2005/article/details/7647519


4. 《TLD视觉跟踪技术解析》


http://www.asmag.com.cn/number/n-50168.shtml

http://blog.sina.com.cn/s/blog_627250020102ux9p.html

相对来说，TLD算是比较入门的跟踪算法了，没有特别复杂的理论原理。实际上，一句话总结，就是中值光流+级联检测（依次从方差，随机森林，最近邻）的融合部分，之后，根据结果，更新级联检测。但其考虑到了多尺度。

直接转大牛的分析，对着代码过了一边，没什么问题。

**原理分析：**

TLD(Tracking-Learning-Detection)是英国萨里大学的一个捷克籍博士生在其攻读博士学位期间提出的一种新的单目标长时间（long term tracking）跟踪算法。该算法与传统跟踪算法的显著区别在于将传统的跟踪算法和传统的检测算法相结合来解决被跟踪目标在被跟踪过程中发生的形变、部分遮挡等问题。同时，通过一种改进的在线学习机制不断更新跟踪模块的“显著特征点”和检测模块的目标模型及相关参数，从而使得跟踪效果更加稳定、鲁棒、可靠。

对于长时间跟踪而言，一个关键的问题是：当目标重新出现在相机视野中时，系统应该能重新检测到它，并开始重新跟踪。但是，长时间跟踪过程中，被跟踪目标将不可避免的发生形状变化、光照条件变化、尺度变化、遮挡等情况。传统的跟踪算法，前端需要跟检测模块相互配合，当检测到被跟踪目标之后，就开始进入跟踪模块，而此后，检测模块就不会介入到跟踪过程中。但这种方法有一个致命的缺陷：即，当被跟踪目标存在形状变化或遮挡时，跟踪就很容易失败；因此，对于长时间跟踪，或者被跟踪目标存在形状变化情况下的跟踪，很多人采用检测的方法来代替跟踪。该方法虽然在某些情况下可以改进跟踪效果，但它需要一个离线的学习过程。即：在检测之前，需要挑选大量的被跟踪目标的样本来进行学习和训练。这也就意味着，训练样本要涵盖被跟踪目标可能发生的各种形变和各种尺度、姿态变化和光照变化的情况。换言之，利用检测的方法来达到长时间跟踪的目的，对于训练样本的选择至关重要，否则，跟踪的鲁棒性就难以保证。

考虑到单纯的跟踪或者单纯的检测算法都无法在长时间跟踪过程中达到理想的效果，所以，TLD方法就考虑将两者予以结合，并加入一种改进的在线学习机制，从而使得整体的目标跟踪更加稳定、有效。

简单来说，TLD算法由三部分组成：跟踪模块、检测模块、学习模块；如下图所示

![](https://img-my.csdn.net/uploads/201206/09/1339204756_3989.png)


其运行机制为：检测模块和跟踪模块互不干涉的并行进行处理。首先，跟踪模块假设相邻视频帧之间物体的运动是有限的，且被跟踪目标是可见的，以此来估计目标的运动。如果目标在相机视野中消失，将造成跟踪失败。检测模块假设每一个视帧都是彼此独立的，并且根据以往检测和学习到的目标模型，对每一帧图片进行全图搜索以定位目标可能出现的区域。同其它目标检测方法一样，TLD中的检测模块也有可能出现错误，且错误无非是错误的负样例和错误的正样例这两种情况。而学习模块则根据跟踪模块的结果对检测模块的这两种错误进行评估，并根据评估结果生成训练样本对检测模块的目标模型进行更新，同时对跟踪模块的“关键特征点”进行更新，以此来避免以后出现类似的错误。TLD模块的详细；流程框图如下所示：

![](https://img-my.csdn.net/uploads/201206/09/1339204785_9900.png)





在详细介绍TLD的流程之前，有一些基本知识和基本概念需要予以澄清：

基本知识：



检测特征：

检测部分用到了一种作者称为Fern的结构，它是在Random
 Forests的基础上改进得到的，不妨称之为Random Fern。

**2bitBP(2bit Binary Pattern)的特征**

这种特征是一种类似于harr-like的特征，这种特征包括了特征类型以及相应的特征取值。

假定现在我们要判断一个Patch块是不是我们要检测的目标。所谓特征类型，就是指在这个Patch在(x,y)坐标，取的一个长width，高height的框子，这个组合(x,
 y, width, height)就是相应的特征类型。

下面解释什么是特征的取值。在已经选定了特征类型的情况下，如果我们把框子左右分成相等的两部分，分别计算左右两部分的灰度和，那么就有两种情况：(1)左边灰度大，(2)右边灰度大，直观的看，就是左右两边哪边颜色更深。同样的，把框子分成上下相等的两部分，也会有两种情况，直观地看，就是上下两边哪边颜色更深。于是在分了上下左右后，总共会有4种情况，可以用2bit来描述这4种情况，即可得到相应的特征取值。这个过程可以参见图1。

实际上每种类型的特征都从某个角度来看待我们要跟踪的对象。比如图1中的红框，这个框子中，车灯的地方灰度应该要深一些，那么红框这个类型的特征，实际上就意味着，它认为，如果该Patch是一个车子，在相应的地方，相应的长和高，这个地方颜色也应该深一些。

![](https://img-blog.csdn.net/20160822170514678)



图1. 2bitBP特征说明



    其实，这跟FAST特征的本质是一样的。只是FAST取的是点附近圆范围内的。这里只取相邻一次。

**Random Fern**

前面已经提及，每种类型的特征都代表了一种看待跟踪对象的观点，那么是否可以用若干种类型的特征来进行一个组合，使之更好地描述跟踪的对象呢？答案是肯定的，还是举图1的例子，左边有一个车灯，右边也有一个车灯，如果我们把这两个框子都取到了，可以预见检测的效果会比只有一个框子来得好。Random
 Fern的思想就是用多个特征组合来表达对象。

下面，我们先讲一个Fern是怎么生成和决策的，再讲多个Fern的情况下，如何进行统一决策。

不妨假设我们选定了nFeat种类型的特征来表达对象。每个棵Fern实际上是一棵4叉树，如图2所示，选了多少种类型的特征，这棵4叉树就有多少层。对于一个Patch，每一层就用相应的类型的特征去判断，计算出相应类型特征的特征取值，由于采用的是2bitBP特征，会有4种可能取值，在下一层又进行相同的操作，这样每个Patch最终会走到最末层的一个叶子节点上。

**对于训练过程**，要记录落到每个叶子结点上的正样本个数(用nP记)，同时也要记录落到每个叶子结点上的负样本的个数(用nN记)。则可算出正样本落到每个叶子结点上的后验概率nP/(nP+nN)。

**对于检测过程**，要检测的Patch最终会落到某个叶子结点上，由于训练过程已经记录了正样本落到每个叶子结点上的后验概率，最终可输出该Patch为正样本的概率。



![](https://img-blog.csdn.net/20160822170519803)




图2. Fern的结构

    这里Fern二值表示，更像是ORB特征的简化版。

前面介绍了一个Fern的生成，以及用Fern检测一个Patch，并给出它为正样本的概率。这样多个Fern进行判断时，就会给出多个后验概率。这就好比我们让多个人来决策，看这个东西是不是正样本，每个人对应于一个Fern。最终我们计算这一系列的Fern输出的后验的均值，看是否大于阈值，从而最终确定它是否是正样本。

  作者巧妙地将随机森林的思想，与ORB思想，结合起来，形成自己的分类器 。

PN学习

参考：http://blog.csdn.net/carson2005/article/details/7483027

PN学习（PN learning）是一种利用带标签的样本（一般用于分类器训练，以下均称之为测试样本）和不带标签的样本（一般用于分类器测试，以下均称之为测试样本）之间存在的结构性特征（见下面的解释）来逐步（学习）训练两类分类器并改善分类器分类性能的方法。 

正约束(Positive constraint)和负约束(negative constraint)用来限制测试样本的标签赋值过程，而PN学习正是受正负约束所操控的。PN学习对分类器在测试样本上的分类结果进行评估，找到那些分类结果与约束条件相矛盾的样本，重新调整训练集，并进行重复迭代训练，直到某个条件满足，才停止分类器训练过程。在目标跟踪过程中，由于被跟踪目标的形状、姿态等容易发生变化，造成目标跟丢的情况时有发生，所以，在这种情况下，对被跟踪目标的在线学习和检测是个很好的策略。而PN学习正好可以在此处大显身手。

很多学习算法都假设测试样本是彼此独立的，然而，在计算机视觉的应用中，有些测试样本的标签却存在彼此依赖的关系。标签之间存在的这种依赖关系，我们称之为结构性的。例如，在目标检测过程中，我们的任务是对图片中目标可能存在的所有区域赋予标签，即：该区域属于前景或者背景，而这里的标签仅能是前景或背景两者之一。再比如，在利用视频序列进行目标跟踪过程中，紧邻被跟踪目标运动轨迹线的区域，可以认为是前景标签，而远离轨迹线的区域，可以认为是背景标签。而前面提到的正约束则表示所有可能的标签为正的模式，例如，此处的紧邻轨迹线的区域；负约束表示所有可能的标签为负的模式。

通过以上的分析，不难发现，PN学习可以定义为一下的过程：

（1）准备一个数量较少的训练样本集合和一个数量很大的测试样本集合。

（2）利用训练样本训练一个初始分类器。同时，利用训练样本对(先验)约束条件进行相应的调整。

（3）利用分类器对测试样本赋予标签，并找出分类器赋予的标签同约束条件相矛盾的那些样本；

（4）将上述相矛盾的样本重新赋予标签，将其加入训练样本，重新训练分类器；

反复迭代上述过程，直到满足某个约束条件。

![](https://img-blog.csdn.net/20160823131001789)


![](https://img-blog.csdn.net/20160823131022439)


![](https://img-blog.csdn.net/20160823131028392)







在任意时刻，被跟踪目标都可以用其状态属性来表示。该状态属性可以是一个表示目标所在位置、尺度大小的跟踪框，也可以是一个标识被跟踪目标是否可见的标记。两个跟踪框的空间域相似度是用重叠度（overlap）来度量，其计算方法是两个跟踪框的交集与两者并集的商。目标的形状采用图像片（image patch，个人认为，可以理解为滑动窗口）p来表示，每一个图像片都是从跟踪框内部采样得到的，并被归一化到15*15的大小。两个图相片![](https://img-my.csdn.net/uploads/201206/09/1339205030_3400.jpg)

![](https://img-my.csdn.net/uploads/201206/09/1339205061_3571.jpg)


![](https://img-my.csdn.net/uploads/201206/09/1339205142_8279.jpg)


![](https://img-my.csdn.net/uploads/201206/09/1339205214_5603.jpg)


![](https://img-my.csdn.net/uploads/201206/09/1339205240_7231.jpg)












||
|----|










以下是源码分析：

原文：

http://blog.csdn.net/zouxy09/article/details/7893026


从main()函数切入，分析整个TLD运行过程如下：

（这里只是分析工作过程，全部注释的代码见博客的更新）

1、分析程序运行的命令行参数；

./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt –r



2、读入初始化参数（程序中变量）的文件parameters.yml；



3、通过文件或者用户鼠标框选的方式指定要跟踪的目标的Bounding Box；



4、用上面得到的包含要跟踪目标的Bounding  Box和第一帧图像去初始化TLD系统，

   tld.init(last_gray, box, bb_file); 初始化包含的工作如下：



4.1、buildGrid(frame1, box);

检测器采用扫描窗口的策略：扫描窗口步长为宽高的 10%，尺度缩放系数为1.2；此函数构建全部的扫描窗口grid，并计算每一个扫描窗口与输入的目标box的重叠度；重叠度定义为两个box的交集与它们的并集的比；



4.2、为各种变量或者容器分配内存空间；



4.3、getOverlappingBoxes(box, num_closest_init);

此函数根据传入的box（目标边界框），在整帧图像中的全部扫描窗口中（由上面4.1得到）寻找与该box距离最小（即最相似，重叠度最大）的num_closest_init（10）个窗口，然后把这些窗口归入good_boxes容器。同时，把重叠度小于0.2的，归入bad_boxes容器；相当于对全部的扫描窗口进行筛选。并通过BBhull函数得到这些扫描窗口的最大边界。



4.5、classifier.prepare(scales);

准备分类器，scales容器里是所有扫描窗口的尺度，由上面的buildGrid()函数初始化；

TLD的分类器有三部分：方差分类器模块、集合分类器模块和最近邻分类器模块；这三个分类器是级联的，每一个扫描窗口依次全部通过上面三个分类器，才被认为含有前景目标。这里prepare这个函数主要是初始化集合分类器模块；

集合分类器（随机森林）基于n个基本分类器（共10棵树），每个分类器（树）都是基于一个pixel comparisons（共13个像素比较集）的，也就是说每棵树有13个判断节点（组成一个pixel comparisons），输入的图像片与每一个判断节点（相应像素点）进行比较，产生0或者1，然后将这13个0或者1连成一个13位的二进制码x（有2^13种可能），每一个x对应一个后验概率P(y|x)=
 #p/(#p+#n) （也有2^13种可能），#p和#n分别是正和负图像片的数目。那么整一个集合分类器（共10个基本分类器）就有10个后验概率了，将10个后验概率进行平均，如果大于阈值（一开始设经验值0.65，后面再训练优化）的话，就认为该图像片含有前景目标；

后验概率P(y|x)= #p/(#p+#n)的产生方法：初始化时，每个后验概率都得初始化为0；运行时候以下面方式更新：将已知类别标签的样本（训练样本）通过n个分类器进行分类，如果分类结果错误，那么相应的#p和#n就会更新，这样P(y|x)也相应更新了。

pixel comparisons的产生方法：先用一个归一化的patch去离散化像素空间，产生所有可能的垂直和水平的pixel comparisons，然后我们把这些pixel comparisons随机分配给n个分类器，每个分类器得到完全不同的pixel comparisons（特征集合），这样，所有分类器的特征组统一起来就可以覆盖整个patch了。

特征是相对于一种尺度的矩形框而言的，TLD中第s种尺度的第i个特征features[s][i] = Feature(x1, y1, x2, y2);是两个随机分配的像素点坐标（就是由这两个像素点比较得到0或者1的）。每一种尺度的扫描窗口都含有totalFeatures = nstructs * structSize个特征；nstructs为树木（由一个特征组构建，每组特征代表图像块的不同视图表示）的个数；structSize为每棵树的特征个数，也即每棵树的判断节点个数；树上每一个特征都作为一个决策节点；

prepare函数的工作就是先给每一个扫描窗口初始化了对应的pixel comparisons（两个随机分配的像素点坐标）；然后初始化后验概率为0；



4.6、generatePositiveData(frame1, num_warps_init);

此函数通过对第一帧图像的目标框box（用户指定的要跟踪的目标）进行仿射变换来合成训练初始分类器的正样本集。具体方法如下：先在距离初始的目标框最近的扫描窗口内选择10个bounding box（已经由上面的getOverlappingBoxes函数得到，存于good_boxes里面了，还记得不？），然后在每个bounding box的内部，进行±1%范围的偏移，±1%范围的尺度变化，±10%范围的平面内旋转，并且在每个像素上增加方差为5的高斯噪声（确切的大小是在指定的范围内随机选择的），那么每个box都进行20次这种几何变换，那么10个box将产生200个仿射变换的bounding
 box，作为正样本。具体实现如下：

getPattern(frame(best_box), pEx, mean, stdev);此函数将frame图像best_box区域的图像片归一化为均值为0的15*15大小的patch，存于pEx（用于最近邻分类器的正样本）正样本中（最近邻的box的Pattern），该正样本只有一个。

generator(frame, pt, warped, bbhull.size(), rng);此函数属于PatchGenerator类的构造函数，用来对图像区域进行仿射变换，先RNG一个随机因子，再调用（）运算符产生一个变换后的正样本。

classifier.getFeatures(patch, grid[idx].sidx, fern);函数得到输入的patch的特征fern（13位的二进制代码）；

pX.push_back(make_pair(fern, 1));   //positive ferns <features, labels=1>然后标记为正样本，存入pX（用于集合分类器的正样本）正样本库；

以上的操作会循环 num_warps * good_boxes.size()即20 * 10 次，这样，pEx就有了一个正样本，而pX有了200个正样本了；



4.7、meanStdDev(frame1(best_box), mean, stdev);

统计best_box的均值和标准差，var = pow(stdev.val[0],2) * 0.5;作为方差分类器的阈值。



4.8、generateNegativeData(frame1);

     由于TLD仅跟踪一个目标，所以我们确定了目标框了，故除目标框外的其他图像都是负样本，无需仿射变换；具体实现如下：

     由于之前重叠度小于0.2的，都归入 bad_boxes了，所以数量挺多，把方差大于var*0.5f的bad_boxes都加入负样本，同上面一样，需要classifier.getFeatures(patch, grid[idx].sidx, fern);和nX.push_back(make_pair(fern, 0));得到对应的fern特征和标签的nX负样本（用于集合分类器的负样本）；

    然后随机在上面的bad_boxes中取bad_patches（100个）个box，然后用 getPattern函数将frame图像bad_box区域的图像片归一化到15*15大小的patch，存在nEx（用于最近邻分类器的负样本）负样本中。

这样nEx和nX都有负样本了；（box的方差通过积分图像计算）



4.9、然后将nEx的一半作为训练集nEx，另一半作为测试集nExT；同样，nX也拆分为训练集nX和测试集nXT；



4.10、将负样本nX和正样本pX合并到ferns_data[]中，用于集合分类器的训练；



4.11、将上面得到的一个正样本pEx和nEx合并到nn_data[]中，用于最近邻分类器的训练；



4.12、用上面的样本训练集训练 集合分类器（森林） 和 最近邻分类器：

  classifier.trainF(ferns_data, 2); //bootstrap = 2

对每一个样本ferns_data[i] ，如果样本是正样本标签，先用measure_forest函数返回该样本所有树的所有特征值对应的后验概率累加值，该累加值如果小于正样本阈值（0.6* nstructs，这就表示平均值需要大于0.6（0.6* nstructs / nstructs）,0.6是程序初始化时定的集合分类器的阈值，为经验值，后面会用测试集来评估修改，找到最优），也就是输入的是正样本，却被分类成负样本了，出现了分类错误，所以就把该样本添加到正样本库，同时用update函数更新后验概率。对于负样本，同样，如果出现负样本分类错误，就添加到负样本库。

  classifier.trainNN(nn_data);

     对每一个样本nn_data，如果标签是正样本，通过NNConf(nn_examples[i], isin, conf, dummy);计算输入图像片与在线模型之间的相关相似度conf，如果相关相似度小于0.65 ，则认为其不含有前景目标，也就是分类错误了；这时候就把它加到正样本库。然后就通过pEx.push_back(nn_examples[i]);将该样本添加到pEx正样本库中；同样，如果出现负样本分类错误，就添加到负样本库。



4.13、用测试集在上面得到的 集合分类器（森林） 和 最近邻分类器中分类，评价并修改得到最好的分类器阈值。

  classifier.evaluateTh(nXT, nExT);

   对集合分类器，对每一个测试集nXT，所有基本分类器的后验概率的平均值如果大于thr_fern（0.6），则认为含有前景目标，然后取最大的平均值（大于thr_fern）作为该集合分类器的新的阈值。

   对最近邻分类器，对每一个测试集nExT，最大相关相似度如果大于nn_fern（0.65），则认为含有前景目标，然后取最大的最大相关相似度（大于nn_fern）作为该最近邻分类器的新的阈值。



5、进入一个循环：读入新的一帧，然后转换为灰度图像，然后再处理每一帧processFrame；



6、processFrame(last_gray, current_gray, pts1, pts2, pbox, status, tl, bb_file);逐帧读入图片序列，进行算法处理。processFrame共包含四个模块（依次处理）：跟踪模块、检测模块、综合模块和学习模块；



6.1、跟踪模块：track(img1, img2, points1,points2);

track函数完成前一帧img1的特征点points1到当前帧img2的特征点points2的跟踪预测；



6.1.1、具体实现过程如下：

（1）先在lastbox中均匀采样10*10=100个特征点（网格均匀撒点），存于points1：

bbPoints(points1, lastbox);

（2）利用金字塔LK光流法跟踪这些特征点，并预测当前帧的特征点（见下面的解释）、计算FB error和匹配相似度sim，然后筛选出
 FB_error[i] <= median(FB_error) 和 sim_error[i] > median(sim_error) 的特征点（舍弃跟踪结果不好的特征点），剩下的是不到50%的特征点：

tracker.trackf2f(img1,
 img2, points, points2);

（3）利用剩下的这不到一半的跟踪点输入来预测bounding box在当前帧的位置和大小 tbb：

bbPredict(points, points2, lastbox, tbb);

（4）跟踪失败检测：如果FB error的中值大于10个像素（经验值），或者预测到的当前box的位置移出图像，则认为跟踪错误，此时不返回bounding box：

if (tracker.getFB()>10 || tbb.x>img2.cols ||  tbb.y>img2.rows || tbb.br().x < 1 || tbb.br().y <1)

（5）归一化img2(bb)对应的patch的size（放缩至patch_size = 15*15），存入pattern：

getPattern(img2(bb),pattern,mean,stdev);

（6）计算图像片pattern到在线模型M的保守相似度：

classifier.NNConf(pattern,isin,dummy,tconf);

（7）如果保守相似度大于阈值，则评估本次跟踪有效，否则跟踪无效：

if (tconf>classifier.thr_nn_valid) tvalid =true;



6.1.2、TLD跟踪模块的实现原理和trackf2f函数的实现：

   TLD跟踪模块的实现是利用了Media Flow 中值光流跟踪和跟踪错误检测算法的结合。中值流跟踪方法是基于Forward-Backward Error和NNC的。原理很简单：从t时刻的图像的A点，跟踪到t+1时刻的图像B点；然后倒回来，从t+1时刻的图像的B点往回跟踪，假如跟踪到t时刻的图像的C点，这样就产生了前向和后向两个轨迹，比较t时刻中 A点和C点的距离，如果距离小于一个阈值，那么就认为前向跟踪是正确的；这个距离就是FB_error；

bool LKTracker::trackf2f(const Mat& img1, const Mat& img2, vector<Point2f> &points1, vector<cv::Point2f> &points2)

函数实现过程如下：

（1）先利用金字塔LK光流法跟踪预测前向轨迹：

  calcOpticalFlowPyrLK( img1,img2, points1, points2, status, similarity, window_size, level, term_criteria, lambda, 0);

（2）再往回跟踪，产生后向轨迹：

  calcOpticalFlowPyrLK( img2,img1, points2, pointsFB, FB_status,FB_error, window_size, level, term_criteria, lambda, 0);

（3）然后计算 FB-error：前向与 后向 轨迹的误差：

  for( int i= 0; i<points1.size(); ++i )

        FB_error[i] = norm(pointsFB[i]-points1[i]);     

（4）再从前一帧和当前帧图像中（以每个特征点为中心）使用亚象素精度提取10x10象素矩形（使用函数getRectSubPix得到），匹配前一帧和当前帧中提取的10x10象素矩形，得到匹配后的映射图像（调用matchTemplate），得到每一个点的NCC相关系数（也就是相似度大小）。

normCrossCorrelation(img1, img2, points1, points2);

（5）然后筛选出 FB_error[i] <= median(FB_error) 和 sim_error[i] > median(sim_error) 的特征点（舍弃跟踪结果不好的特征点），剩下的是不到50%的特征点；

filterPts(points1, points2);



6.2、检测模块：detect(img2);

TLD的检测分类器有三部分：方差分类器模块、集合分类器模块和最近邻分类器模块；这三个分类器是级联的。当前帧img2的每一个扫描窗口依次通过上面三个分类器，全部通过才被认为含有前景目标。具体实现过程如下：

先计算img2的积分图，为了更快的计算方差：

integral(frame,iisum,iisqsum);

然后用高斯模糊，去噪：

  GaussianBlur(frame,img,Size(9,9),1.5); 

下一步就进入了方差检测模块：



6.2.1、方差分类器模块：getVar(grid[i],iisum,iisqsum) >= var

利用积分图计算每个待检测窗口的方差，方差大于var阈值（目标patch方差的50%）的，则认为其含有前景目标，通过该模块的进入集合分类器模块：



6.2.2、集合分类器模块：

集合分类器（随机森林）共有10颗树（基本分类器），每棵树13个判断节点，每个判断节点经比较得到一个二进制位0或者1，这样每棵树就对应得到一个13位的二进制码x（叶子），这个二进制码x对应于一个后验概率P(y|x)。那么整一个集合分类器（共10个基本分类器）就有10个后验概率了，将10个后验概率进行平均，如果大于阈值（一开始设经验值0.65，后面再训练优化）的话，就认为该图像片含有前景目标；具体过程如下：

（1）先得到该patch的特征值（13位的二进制代码）：

classifier.getFeatures(patch,grid[i].sidx,ferns);

（2）再计算该特征值对应的后验概率累加值：

conf = classifier.measure_forest(ferns);           

（3）若集合分类器的后验概率的平均值大于阈值fern_th（由训练得到），就认为含有前景目标：

if (conf > numtrees * fern_th)  dt.bb.push_back(i); 

（4）将通过以上两个检测模块的扫描窗口记录在detect structure中；

（5）如果顺利通过以上两个检测模块的扫描窗口数大于100个，则只取后验概率大的前100个；

nth_element(dt.bb.begin(), dt.bb.begin()+100, dt.bb.end(),

CComparator(tmp.conf));

进入最近邻分类器：



6.2.3、最近邻分类器模块

（1）先归一化patch的size（放缩至patch_size = 15*15），存入dt.patch[i];

getPattern(patch,dt.patch[i],mean,stdev); 

（2）计算图像片pattern到在线模型M的相关相似度和保守相似度：

classifier.NNConf(dt.patch[i],dt.isin[i],dt.conf1[i],dt.conf2[i]);

（3）相关相似度大于阈值，则认为含有前景目标：

if (dt.conf1[i]>nn_th)  dbb.push_back(grid[idx]);

到目前为止，检测器检测完成，全部通过三个检测模块的扫描窗口存在dbb中；



6.3、综合模块：

TLD只跟踪单目标，所以综合模块综合跟踪器跟踪到的单个目标和检测器可能检测到的多个目标，然后只输出保守相似度最大的一个目标。具体实现过程如下：

（1）先通过 重叠度 对检测器检测到的目标bounding box进行聚类，每个类的重叠度小于0.5：

clusterConf(dbb, dconf, cbb, cconf);

（2）再找到与跟踪器跟踪到的box距离比较远的类（检测器检测到的box），而且它的相关相似度比跟踪器的要大：记录满足上述条件，也就是可信度比较高的目标box的个数：

if (bbOverlap(tbb, cbb[i])<0.5 && cconf[i]>tconf) confident_detections++;

（3）判断如果只有一个满足上述条件的box，那么就用这个目标box来重新初始化跟踪器（也就是用检测器的结果去纠正跟踪器）：

if (confident_detections==1)  bbnext=cbb[didx];

（4）如果满足上述条件的box不只一个，那么就找到检测器检测到的box与跟踪器预测到的box距离很近（重叠度大于0.7）的所以box，对其坐标和大小进行累加：

if(bbOverlap(tbb,dbb[i])>0.7)  cx += dbb[i].x;……

（5）对与跟踪器预测到的box距离很近的box 和 跟踪器本身预测到的box 进行坐标与大小的平均作为最终的目标bounding box，但是跟踪器的权值较大：

bbnext.x = cvRound((float)(10*tbb.x+cx)/(float)(10+close_detections));……

（6）另外，如果跟踪器没有跟踪到目标，但是检测器检测到了一些可能的目标box，那么同样对其进行聚类，但只是简单的将聚类的cbb[0]作为新的跟踪目标box（不比较相似度了？？还是里面已经排好序了？？），重新初始化跟踪器：

bbnext=cbb[0];

至此，综合模块结束。



6.4、学习模块：learn(img2);

    学习模块也分为如下四部分：












6.4.1

、检查一致性：

（1）归一化img(bb)对应的patch的size（放缩至patch_size = 15*15），存入pattern：

  getPattern(img(bb), pattern, mean, stdev);

（2）计算输入图像片（跟踪器的目标box）与在线模型之间的相关相似度conf：

  classifier.NNConf(pattern,isin,conf,dummy);

（3）如果相似度太小了或者如果方差太小了或者如果被被识别为负样本，那么就不训练了；

if (conf<0.5)……或if (pow(stdev.val[0], 2)< var)……或if(isin[2]==1)……



6.4.2、生成样本：

先是集合分类器的样本：fern_examples：

（1）先计算所有的扫描窗口与目前的目标box的重叠度：

grid[i].overlap = bbOverlap(lastbox, grid[i]);

（2）再根据传入的lastbox，在整帧图像中的全部窗口中寻找与该lastbox距离最小（即最相似，重叠度最大）的num_closest_update个窗口，然后把这些窗口归入good_boxes容器（只是把网格数组的索引存入）同时，把重叠度小于0.2的，归入 bad_boxes 容器：

  getOverlappingBoxes(lastbox, num_closest_update);

（3）然后用仿射模型产生正样本（类似于第一帧的方法，但只产生10*10=100个）：

generatePositiveData(img, num_warps_update); 

（4）加入负样本，相似度大于1？？相似度不是出于0和1之间吗？

idx=bad_boxes[i];

if (tmp.conf[idx]>=1) fern_examples.push_back(make_pair(tmp.patt[idx],0));

然后是最近邻分类器的样本：nn_examples：

if (bbOverlap(lastbox,grid[idx]) < bad_overlap)

        nn_examples.push_back(dt.patch[i]);



6.4.3、分类器训练：

classifier.trainF(fern_examples,2);

classifier.trainNN(nn_examples);



6.4.4、把正样本库（在线模型）包含的所有正样本显示在窗口上

classifier.show();

至此，tld.processFrame函数结束。



7、如果跟踪成功，则把相应的点和box画出来：

    if (status){

      drawPoints(frame,pts1);

      drawPoints(frame,pts2,Scalar(0,255,0));  //当前的特征点用蓝色点表示

      drawBox(frame,pbox);

      detections++;

}



8、然后显示窗口和交换图像帧，进入下一帧的处理：

    imshow("TLD", frame);

swap(last_gray, current_gray);

至此，main()函数结束（只分析了框架）。







参考：

http://blog.csdn.net/carson2005/article/details/7647500

http://blog.sina.com.cn/s/blog_8ff949cf0100yfof.html






