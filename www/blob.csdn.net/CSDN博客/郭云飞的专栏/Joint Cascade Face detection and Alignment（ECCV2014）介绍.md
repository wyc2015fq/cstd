
# Joint Cascade Face detection and Alignment（ECCV2014）介绍 - 郭云飞的专栏 - CSDN博客


2017年10月11日 18:42:18[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：162


**一、总体**
在*Joint Cascade Face detection and Alignment（ECCV2014）*（以下简称论文）之前，比较广泛的人脸检测（object detection）方法是Viola-Jones检测器（典型的为Haar + Ababoost）。V-J分类器基于以下两个原则进行检测的：
逐步提升的级联结构
简单的特征
这种方法在日常生活场景中效果不甚理想。在一些复杂的情况下（光照、表情、遮挡），简单的特征会使检测能力大大下降。
一些人尝试离散的训练出各种角度的识别器来增强鲁棒性，但是用这种离散的方式来估计人脸角度本身就带来了很大的困难，训练出的模型往往速度过慢或准确率不高。
最近又提出了一种不使用V-J框架的人脸检测算法：**DPM（Deformable Parts Model, 可变形的组件模型）**，它可以做到人脸检测和关键点定位的一气呵成，但是其计算量太大导致时间消耗过高。
OK，回到本论文
论文采用了一种新的算法来检测人脸，其仍遵循V-J的经典框架，采用**像素差**作为特征，有着杰出的速度（28.6 ms for a VGA image ）与实现效果（achieves the best accuracy on the challenging datasets）。
论文的突破点在于采用了人脸检测点作为人脸检测的一种指标，过去的工作中人们大多数将关键点检测（face alignment）与人脸检测（face detection）分成两部分。论文通过实验指出，关键点检测可以作为人脸检测的一个重要评价标准，其实我们想一下也能知道，假如关键点检测到鼻子在嘴巴下面，那么这张脸想必不会是人脸。关键在于如何有效的利用这种特征。
论文与另一篇论文密不可分，*《Face Alignment at 3000 FPS via Regressing Local Binary Features》*（以下简称3000FPS），只有理解好这篇论文，才能理解其利用回归树做的关键点定位算法，能够理解其关键点定位算法，才能理解本论文提出的分类回归树算法。
阅读步骤：分类树与回归树、随机森林-->3000FPS-->本论文
**二、论文通过试验证明了关键点如何帮助人脸检测：**
试验步骤：
使用OpenCV中的V-J检测器，在一个比较低的阈值下检测，以保证一个高的召回率。
将得到许多图像窗口，其中有许多是false positive(错误的正样本)，将这些窗口分为真假正样本两部分，用于训练一个线性SVM分类器。
在测试的时候，将V-J检测器输出的窗口，送人SVM分类器中进行筛选。
上边第2步中提取特征并训练SVM：
所有的V-J输出的窗口都被归一化到96*96pixel大小，论文实验了几种不同的特征：
将窗口分为6*6各不重叠的部分，在每一块区域上进行SIFT描述子的提取。
选用27个面部的平均形状点，并以这些点为中心，提取SIFT描述子。
按照参考文献【21】中算法得到面部的27标点，并且以此为中心提取SIFT描述子。
上述三种情况下，所有的SIFT描述子都被连接成一个向量，送入SVM训练。试验结果：
下图画出了分类情况分布图，（1）是原始的V-J检测器，（2）（3）（4）分别是结合了上述三种特征下训练的SVM分类器的分类结果。
![](https://img-blog.csdn.net/20171011190008524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
由上图可见，做一下*shape alignment*可以得到一个更好的分类效果。但是问题来了：如果把所有的windows都做一下*alignment*，即使是3000 faces per second的速度一张图可能也要处理上1秒，这无法满足一般一秒30帧的实时需求。论文作者也说，用opencv分类器，参数设成99%的recall率将会带来很严重的效率灾难——一张图能找出来3000个框，处理一张图都要好几秒。这么渣的效率可咋办呢？以上内容已经证明了alignment确实对detection的preciseness有帮助，这就够啦。
**三、关键点用于人脸检测的初步结合尝试：**
论文提到两个算法，分别是：
Cascade Detection
Cascade Alignment
这两个算法都是现成的，V-J框架就是一个cascade detection，我们不再多说。Cascade Alignment相比来说是个较新的方法，主要是用回归的方法，将关键点不断回归到正确的位置。实现的方法是在做检测的时候引入关键点参数，然后在一层检测过后更新关键点参数，具体做法就是在训练检测过程的弱分类器时加入关键点参数，算法流程如下：
![](https://img-blog.csdn.net/20171011191345433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上边的算法很容易理解，其实就是在每一层内，先做检测，检测完在做关键点校准，缺点也很明显，一来是效率不高，二来是两套模型训练和测试起来很麻烦，别急，下面是改进。
![](https://img-blog.csdn.net/20171011191555853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里的就是一棵*CRDT*，是其阈值。
整个算法的训练过程如下：
![](https://img-blog.csdn.net/20171012111417038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





