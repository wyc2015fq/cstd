# 论文笔记 understanding deep learning requires rethinking generalization - AI之路 - CSDN博客





2017年05月04日 21:11:45[AI之路](https://me.csdn.net/u014380165)阅读数：2711








前段时间召开的ICLR2017的最佳论文之一：understanding deep learning requires rethinking generalization。特来拜读，做了点笔记和大家分享，基本按照文章的先后顺序。




**1.1 our contributions**

**randomization tests 随机测试：**

首先作者做了一个随机测试，训练数据中的真实labels用随机labels代替进行训练，实验结果显示训练误差几乎为0，但是测试误差却不低，因为训练labels和测试labels之间没有关系。这个发现总结为：深度神经网络很容易拟合随机标签，换句话说模型是否有效跟你的标签对错没有太大关系（仅限训练误差）。

在随机label情况下，作者认为神经网络拥有effective capacity去有效记住整个数据集。

接下来作者在上一个随机测试的基础上用完全随机的像素点代替原来的真实图像进行训练，发现卷积神经网络依然可以拟合数据且训练误差为0，这说明网络可以拟合随机噪声。进一步，作者在无噪声和有噪声图像之间生成插值图像，加大了图像的随机性，发现随着噪声水平的提升，模型的泛化能力稳定地下降，因为模型虽然可以拟合数据中残留的信息，但同时也在极力拟合噪声。




**the tole of explicit regularization 显示的规则化的作用：**

显示的规则化比如权重衰减（L1，L2正则化），fropout，增加数据量等并没有充分解释神经网络的泛化误差。附原文：

![](https://img-blog.csdn.net/20170504211458706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


作者认为正则化可能提高了模型的泛化能力，但是没有正则化并不一定意味着泛化误差的提高。




**finite sample expressivity 有限样本的表达能力：**

作者通过实验证明即便是只有两层的线性网络也能够很好地表达任何带标签的训练数据。




**the role of implicit regularization 隐式正则化的作用：**

作者认为随机梯度下降（SGD）是一种隐式的正则化，但是具体原因未知。




**related work：**

这部分作者主要说了两件事，一个是学习算法的uniform stability 和训练数据的标签是独立的，另一个是泛化能力的界限和网络的大小是独立的。




**2.1 fitting random labels and pixels**

这里注意两种不同的随机pixels方式。shuffled pixels：同一种随机替换方式应用到所有train和test的图像中。random pixels：每张图像都用不同的随机替换方式。

这是同一个模型在不同设置下的学习曲线：

![](https://img-blog.csdn.net/20170504211650756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


random label一开始收敛速度较慢是因为所有的labels都是不正确的，因此一开始的预测误差和更新尺度都较大，但是最终都是可以收敛的。




这里random labels的top1结果和true labels的差不大多，而且前者的模型参数是直接从后者搬过来的。

![](https://img-blog.csdn.net/20170504211730960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这个图显示了不同随机程度的label都不影响模型最后的收敛，差别只在于收敛速度。

![](https://img-blog.csdn.net/20170504211752364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**3 the role of regularization**

这里作者主要讨论三种正则化方式：1、增加数据，比如图像数据可以通过随机切分图像，给图像的亮度、对比度、色度等增加随机噪声等。2、权重衰减，比如L2正则化。3、dropout。

从这个表可以看出无论是否有正则化，模型的泛化能力变化不大。

![](https://img-blog.csdn.net/20170504211835493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


另外作者还通过实验比较了early stopping，batch normalization等正则化方式，最后总结如下：不管是显示的还是隐式的正则化方式，如果参数调整合适，那么可以提高网络的泛化能力。但是正则化并不是泛化的必要条件，因为在去掉正则化后网络依然表现良好。

**4 finite sample expressivity**

举了参数的例子，说明即便是浅层网络也能有强大的表达能力。

![](https://img-blog.csdn.net/20170504211936822?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**6 conclusion**

主要还是强调模型本身已经足够去表达训练数据，即便没有正则化。




总的看下来，感觉作者挖了许多坑，做了许多实验且看到了意料之外的实验现象，不过具体原因并没有说明，所以看完有点云里雾里，等待后续填补。



