# 庖丁解牛TLD（三）——算法初始化 - 迭代的是人，递归的是神 - CSDN博客





2011年11月13日 16:37:59[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：14948











上一讲我提到对于算法的初始化工作主要是在tldInit这个函数里实现的。主要分为如下几大步骤，1）初始化Detector。2）初始化Trajectory。3）训练Detector

### 1)初始化Detector

其中bb_scan为扫描grid区域，该函数输入为boundingBox，输出为一系列的RectBox，是根据boundingBox的大小参数对待搜素区域选择一系列的box作为备选的跟踪区域，box的位置和尺度都有变化，和RectBox相应的尺度。但RectBox有6个参数，前4个分别为Rect的左上角坐标（x1，y1）和右下角坐标（x2，y2）。后两个参数求大神解释（PS：后来在fern函数里找到了解释，分别为指向对应尺度特征的指针位置、每一行box的数量——用在搜索邻近box）！对于这个函数内部我还有一个疑惑，就是对ntuples函数功能的使用，哎，怎奈Matlab语法都不熟悉，只能慢慢啃了，感觉作者这里就是把RectBox的左上角的所有可能的坐标值传入该函数，得到左上角坐标位置的全部组合（不知道理解对了没）。

接下来的工作时特征的初始化，是在tldGenerateFeature函数里实现的。这个函数相对独立，作者这里为了产生效果较好的随机特征真是煞费苦心，输入的参数有两个，一个是nTREE = 10，一个是nFEATURE = 13。输出为nTREE组特征，每组特征为nFEATURE个点对，每一个点对有4个参数，分别两点坐标（x1，y1），（x2，y2），取值范围为（0~1）其中第一个点的分辨率为0.1，还不太明白这样设计的原因，待进一步分析代码，有高人指点一下更好。值得注意的是产生的点对不是横坐标相同x1
 = x2，就是纵坐标相同y1 = y2。这里用图片显示一组特征，线段的两个端点

![](http://hi.csdn.net/attachment/201111/11/0_13209933329XKy.gif)


下一步工作为初始化detector。这个功能是用强大的fern函数写的，该函数有多个功能，根据传入参数的标志分别可以实现clear操作、init操作、update操作、evaluate操作、detect操作、get pattern操作。fern函数是用c写的，混合编程没有弄的太明白，还没能调试一下看看，只能看代码猜。在初始化detector的工作里，用到的是init操作。

### 2）初始化Trajectory

这部分没有什么要说明的，都是些零碎的初始化工作，matlab里面对一些必要的变量开辟一些空间和定义一些变量的值。具体分析Trajectory的工作的时候可以具体再分析

### 3）训练Detector

首先得到Target，作者注释说该Target只是用来显示，有待我后续验证。得到Target要调用函数img_patch，img_patch函数是获得一幅图像中目标区域box的像素信息patch。

接下来产生正样本数据集，调用tldGeneratePositiveData。其中第二个参数为RectBox和box的重复区域比例信息，保存在overlap参数中，由函数bb_overlap得到。tldGeneratePositiveData函数首先根据overlap的比例信息选出重复区域比例大于60%并且前num_closet  = 10个的最接近box的RectBox，相当于对RectBox进行筛选。并通过bb_hull函数得到这些RectBox的最大边界。接下来的工作比较重要，要得到Pattern，调用的函数为tldGetPattern。初始化的工作就是对最接近box的RectBox区域得到其patch，然后调用tldPatch2Pattern将像素信息转换为Pattern，具体的说就是归一化RectBox对应的patch的size（放缩至patch_size
 = 15*15），将2维的矩阵变成一维的向量信息，然后将向量信息均值设为0，调整为zero mean and unit variance（ZMUV），这个过程调用函数tldPatch2Pattern实现。接下来处理RectBox最大边界的模糊信息，再次用到img_patch函数，但这次调用的函数有很大的不同，还没太理解作者要做什么啊，怎么感觉还有平移旋转矩阵都出来了，晕啦（求高人指点）。该函数最后返回3个参数，pX为处理后的RectBox最大边界处理后的像素信息，pEx最近邻的RectBox的Pattern，bbP0为最近邻的RectBox。

然后再产生负样本数据tldGenerateNegativeData。得到远离box（重复区域比例小于20%）的num_patches = 100个Pattern保存到nX中，随机选中num_patches = 100个RectBox得到对应的patch保存到nEx中。这里调用了fern（5），即该函数的get pattern操作。

接下来对负样本进行分类，分类到训练集Training Set和验证集Validation Set中去。

接下来使用Training Set进行训练，先调用fern（2），更新，然后调用tldTrainNN最近邻训练数据。

接下来评估验证集Validation Set的阈值。调用tldNN验证。

至此，初始化的工作基本完成，限于本人水平有限，只能先对函数有个大概的认识，深深觉得先要静下心来把训练的算法搞清楚，再回过头来再看一编代码。




