
# 残差神经网络与inception-resnet - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[残差神经网络与inception-resnet](https://www.cnblogs.com/eniac1946/p/8708416.html)
|Posted on|2018-04-03 14:48|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8708416)|[收藏](#)
|一、基本概念
|Residual Connection：
|本质是“短路连接”
|如下图阴影部分，通过增加shortcuts，加速训练，从而可以训练出更深的模型（I.R.v2 > Inception v3）。更深的模型意味着可以学出更多东西，带来精度的提升。
![](https://images2018.cnblogs.com/blog/1181483/201804/1181483-20180403142029219-99559123.png)
|I.R. v2结构，注意到图中inception区块被简化了，比先前的Inception V3种要包含更少的并行塔（parallel towers）。
|Inception模块的特点，是通过这种并联结构减少参数，使得泛化性更好、降低对样本数量的要求。
|实现
|通过将并联部分与“短路连接”的tensors相加（或加权相加），并经过一个‘relu’激活函数。
|x = layers.add([mix0, shortcut])
 x = layers.Activation('relu')(x)
|二、论文
|为了进一步推进这个领域的进步，今天Google团队宣布发布|Inception-ResNet-v2|（一种卷积神经网络——CNN），它在ILSVRC图像分类基准测试中实现了当下最好的成绩。Inception-ResNet-v2是早期Inception V3模型变化而来，从微软的残差网络（ResNet）论文中得到了一些灵感。相关论文信息可以参看我们的论文|Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning|（|Inception-v4, Inception-ResNet以及残差连接在学习上的影响|）：
![Google最新开源Inception-ResNet-v2，借助残差网络进一步提升图像分类水准](https://static.leiphone.com/uploads/new/article/740_740/201609/57c7d60b5f13e.png?imageMogr2/format/jpg/quality/90)
|残差连接（Residual connections ）允许模型中存在shortcuts，可以让研究学者成功地训练更深的神经网络（能够获得更好的表现），这样也能明显地简化Inception块。
|效果
|如下方图表所示，Inception-ResNet-v2架构的精确度比之前的最优模型更高，图表中所示为基于单个图像的ILSVRC 2012图像分类标准得出的排行第一与排行第五的有效精确度。此外，该新模型仅仅要求两倍于Inception v3的容量与计算能力。
![Google最新开源Inception-ResNet-v2，借助残差网络进一步提升图像分类水准](https://static.leiphone.com/uploads/new/article/740_740/201609/57c7d60c205a1.png?imageMogr2/format/jpg/quality/90)
|Inception-ResNet-v2具体代码实现过程参见：
|[https://github.com/tensorflow/models/blob/master/slim/nets/inception_resnet_v2.py](https://github.com/tensorflow/models/blob/master/slim/nets/inception_resnet_v2.py)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
