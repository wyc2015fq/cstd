
# 多任务——Cross-stitch - 小花生的博客 - CSDN博客


2018年08月02日 22:29:15[Peanut_范](https://me.csdn.net/u013841196)阅读数：171


《Cross-stitch Networks for Multi-task Learning》2016
**要解决的问题：**多任务的网络结构没有普遍性
**创新点：**以往的多任务网络结构都是人为设计，从哪层分离，哪层结合，并且需要大量的实验才能验证网络结构的有效性。作者提出了一种使用多任务方式来学习ConvNets中的共享表示的原则性方法。 具体来说，提出了一个新的共享单位：“十字绣”单位。这些单元结合了来自多个网络的激活，并且可以端对端地进行训练。具有十字绣单元的网络可以学习共享和任务特定表示的最佳组合。
**网络结构：**
![](https://img-blog.csdn.net/2018080222250810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018080222250810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们仅在池化层和全连接层之后才应用十字绣单元。十字绣单元可以将共享表示特征map作为下一层输入层的线性组合。
**十字绣单元：**
![](https://img-blog.csdn.net/20180802222647632?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180802222647632?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
S和D的初始值在[0,1]范围内对于稳定学习是重要的，作者针对两种任务给出S=0.9，D=0.1
效果最好，当然根据不同任务的相关性，可以进行不同的调整。
为了确保十字绣操作之后的值与输入值相同的数量级，不同任务D和相同任务S总和为一。
**效率：**跟已有方法比的相对于数据缺乏类别的基准方法的收益巨大。
---

###### 注：博众家之所长，集群英之荟萃。

