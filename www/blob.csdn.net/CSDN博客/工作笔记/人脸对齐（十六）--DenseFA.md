# 人脸对齐（十六）--DenseFA - 工作笔记 - CSDN博客





2018年08月21日 14:19:21[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5380
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### Dense Face Alignment ICCVW2017

[http://cvlab.cse.msu.edu/project-pifa.html](http://cvlab.cse.msu.edu/project-pifa.html)

MatConvNet code

model can run at real time during testing

这里针对人脸对齐问题，我们采用 Dense Face Alignment (DeFA) 密集人脸对齐的策略，providing a very dense 3D alignment for large-pose face images

我们通过两个手段达到这个目标：

1）对3D人脸模型中加入三个约束 landmark fitting constraint, contour fitting constraint and sift pair constraint

2）使用多个人脸对齐数据库进行训练

以前基于 3D 模型拟合的人脸对齐算法只使用 稀疏的特征点作为 supervision。如果要实现高质量的密集人脸对齐(DeFA)，面临的首要问题就是没有对应的训练数据库，所有的人脸对齐数据库中标记的特征点不超过68个特征点，所以我们需要寻找有用的信息来作为额外的 supervision，并将这些信息嵌入到学习框架中。面临的第二个问题就是需要各种的训练数据，但是不同的人脸对齐数据库 labeled differently，标记的特征点个数不一样。

上面两个问题该如何解决了？

additional constraints：

1）contour constraint 预测的人脸形状的轮廓应该和图像中检测到的2D人脸边缘是匹配的。

2）SIFT constraint 对应同一个人的不同人脸图像见到SIFT关键点在 3D人脸模型中应该对应 same vertexes

leveraging multiple datasets： 3D face model fitting approach 对特征点个数不是很敏感，所以可以使用多个不同数据库进行训练

main contributions：

1）我们定义了一个密集人脸对齐问题

2）为了实现这个密集人脸对齐，我们定义了一个新颖的 3D 人脸模型拟合算法加入多个约束和跨数据库训练

3）我们模型的性能优异。可以实时运算

1. 3D Face Representation

3D 人脸表示方法，一个人脸的 3D shape S 我们使用一组 3D vertices 来表示，为了计算这个 S，我们根据 3DMM 表示方法 使用一组3D shape bases 来表示它
![0](https://img-blog.csdn.net/20171116093841502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由三个部分构成： mean shape+ shape bases for identification variances +shape bases for representing expression variances

3D 人脸中的一些 vertices 对应人脸图像中 2D 特征点的位置，基于一种对应关系（weak perspective projection），我们可以根据 3D face shape 来估计 2D 人脸的 dense shape。经过公示推导：The learning of the dense 3D shape is turned into the learning of m and p(projection parameters m shape basis coefficients p)

2. CNN Architecture
![这里写图片描述](https://img-blog.csdn.net/20171116095118224?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.多个约束对应多个损失函数
![这里写图片描述](https://img-blog.csdn.net/20171116095211371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171116095346823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4.Experimental Results
![这里写图片描述](https://img-blog.csdn.net/20171116095422412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

To make the training more manageable, we train our DeFA model in three stages, with the intention to gradually increase the datasets and employed constraints

1) At stage 1, we use 300W-LP to train our DeFA network with parameter constraint (PL).

2) At stage 2, we additionally include samples from the Caltech10K [2], and COFW [6] to continue the training of our network with the additional landmark fitting constraint (LFC).

3) At stage 3, we fine-tune the model with SPC and CFC constraints. For large-pose face alignment, we fine-tune the model with AFLW-LFPA training set.

![这里写图片描述](https://img-blog.csdn.net/20171116095822339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171116095852520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171116095928980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171116095936968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



