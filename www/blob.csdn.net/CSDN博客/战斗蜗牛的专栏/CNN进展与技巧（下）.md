# CNN进展与技巧（下） - 战斗蜗牛的专栏 - CSDN博客





2016年07月29日 14:24:28[vbskj](https://me.csdn.net/vbskj)阅读数：1442







上期勘误    PPT11,
 随机Pooling中蓝色矩形中概率值相加不为1，修正结果如下：![](http://mmbiz.qpic.cn/mmbiz/xRp3sibCWzgEvu6CmMdX9vvqkW5TLTnA6ia9iaIUic5uG6hiaYCsFaiac3VMGn8YOqYyeUZiajfSXZFWJL729H3Dib4CwA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)    PPT16,
 欧式损失和对比损失函数系数应为1/2N，原文中误为-1/N，修正结果如下：    本文作者刘昕特此感谢上海科技大学汤旭同学和腾讯优图汪铖杰研究员指出上期推送中的错误。作者对此表示深刻歉意，并承诺加强内容审核，同时欢迎深度学习大讲堂的广大读者指正，共同为优质的内容生产而努力。正文
1. Caffe实战技巧    Caffe是目前作为活跃的深度学习开源平台之一。Caffe中的核心概念包括：caffe.proto、Solver、Net、Layer和Blob。其中Solver负责网络参数的更新，每个Solver中包含一个训练网络和一个测试网络，两个网络共享一套参数。每个Net由若干个Layer构成，每个Layer用Blob实际存储参数及梯度信息。每个Blob逻辑上是一个四维矩阵，但是为了提高存储效率，Blob实际上是一个一维数组并通过Caffe框架中的SyncedMemory自动管理CPU和GPU之间的数据同步。    在PyCaffe出现之前，网络结构文件（一般后缀为prototxt）依靠手写，这样费时费力且容易出错。PyCaffe的出现使得我们可以用Python代码定义网络结构。上图中所示为用PyCaffe定义LeNet的定义例子，入口参数来自于caffe.proto中对应层的参数。    进一步的，我们给出了定义CaffeNet和残差网络Residual
 Net的代码片段。    推荐一个网络结构可视化的在线工具netscope，通过这个工具可以方便的检查定义好的网络结构拓扑。    除了卷积层、全连接层之外，Caffe中还有一些特殊的功能层，例如隐式包含的Split层，这对分叉网络的命运影响很大，Split层使得我们可以自由的定义分叉网络而不需要自己管理梯度回传问题。特别提醒注意Eltwise层，这是深度残差网络中Addition操作的推荐实现方式。    Caffe当中目前支持单机多卡的分布式训练，但是基于同步模式，每一轮迭代都要进行梯度的交换。因此在多GPU服务器上就可能存在GPU之间的通讯瓶颈，如果GPU之间不支持P2P方式连接而采用socket连接，会极大影响GPU并行的加速比。Linux下可以采用nvidia-smi
 topo –m命令查看，如果两块显卡之间的桥接方式为SOC也就是socket连接，那么不推荐使用这两块显卡进行分布式训练。注意显卡的桥接方式由主板硬件决定，无法通过软件修改设置！2.
 小数据深度学习实战    本节将以ICCV2015社会年龄估计竞赛为例介绍小数据条件下的深度学习技术。    竞赛全称是ICCV2015
 Apparent Age Estimation Challenge，一共4699张图像，分为train, validation和test三个子集。开发阶段：train训练，在validation上汇报性能。测试阶段：train + validation训练，在test上汇报性能。值得一提的是，这些图像人物的年龄标签并非其真实年龄，而是所谓“社会年龄”，表示从人主观上认定的年龄。比如二十岁的人六十岁的脸，其标签是“60”。【请不要将这个例子在身边对号入座】收集数据时，主办方请了10名志愿者标注年龄并计算了均值和方差。    由于训练数据只有2K，这是一个典型的小数据条件下的深度学习问题。    未能免俗，网络结构称之为AgeNet。AgeNet采用大规模的深度卷积网络学习多层次的年龄特征表示，基于多维的高斯分布编码学习年龄分类器，基于一维实值特征和欧式损失学习年龄回归器。    下面介绍大规模深度网络的结构。上图与Google
 Net非常类似，只不过将两个辅助损失层去掉，加入Batch Normalization加速收敛。是否保留辅助损失层是一个很有争议的问题。辅助层会在网络较深时帮助网络收敛（避免梯度消失），实践中我们发现在加入BN层之后，去掉辅助层反而会带来性能的微小增益（一点儿微小的发现）。AgeNet
 Deep Age Regressor:     1维实值编码 + Euclidean Loss     归一化年龄到[0,1]，采用欧式Loss学习年龄回归器。注意在欧氏损失之前进行了Sigmoid归一(LeCun 1989 CNN开山之作的彩蛋之一)。AgeNet
 Deep Age Classifier:    分布标记编码 + Cross-Entropy Loss     采用标记分布学习(LDL Geng’ TPAMI2013)编码年龄，学习年龄分类器。即既有当前年龄的权值，又有相邻年龄的权值。    Fine-tune是目前deep
 领域解决小数据问题一个不可或缺的技术。因为target domain数据很少，而source domain数据很多。    我们希望pre-train的过程中体现general to specific的语义迁移。为了理解这一点，请看上图。最外圈是随机的图像特征，往里是自然场景图像，再往里是人脸图像，最后体现限定条件的人脸图像，包括特定的pose，特定表情，特定年龄特征，等等。    具体说来，我们在AgeNet的训练过程中，从general的模型慢慢过渡到specific的模型。AgeNet训练中依次训练人脸分类网络、真实年龄估计网络和社会年龄估计网络。我们认为真实年龄包含了跟社会年龄的变化规律。    如果把其中的一个或几个fine-tune拿掉呢？比较有趣的结果是真实年龄网络的fine-tune的效果没有人脸分类网络好，这提示大家可以考虑比较general的网络去做初始化。    最终我们在决赛中取得了亚军。我们方法的优势：
 1）与第一名20个VGG网络相比，我们只使用了8个GoogLeNet，而性能只相差0.6%。2）完全end-to-end学习，我们没有使用单独的统计学习方法进行年龄估计器学习。    一些good
 case和bad case的例子。粗略的观察结论：AgeNet对姿态、光照、种族、遮挡和色彩鲁棒性好，但是对模糊、误配准和老年人性能有所下降。3.
 CNN前沿讨论    方向1：
 在Graph上做CNN，突破2D图像数据卷积的限制。    方向2：网络结构学习，根据样本复杂度自适应调整网络结构。    方向3：
 训练过程中的随机深度约减。![](http://mmbiz.qpic.cn/mmbiz/xRp3sibCWzgEvu6CmMdX9vvqkW5TLTnA6H1xJ1zlGrpADccwicZOxyqUI26FibLOyQ07eytwU24nfALlDkxwpqfIQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)    方向4.
 网络结构简化。![](http://mmbiz.qpic.cn/mmbiz/xRp3sibCWzgEvu6CmMdX9vvqkW5TLTnA6fhvxD8iaAqFW0P6fU3teQ2o0rq7c7Ugnx8Xt3PV9gu2n8G7ag8EQ3bg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


