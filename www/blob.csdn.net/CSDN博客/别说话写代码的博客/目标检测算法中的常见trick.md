# 目标检测算法中的常见trick - 别说话写代码的博客 - CSDN博客





2018年12月09日 11:53:21[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：168








转自：[https://zhuanlan.zhihu.com/p/39262769](https://zhuanlan.zhihu.com/p/39262769)

## 目标检测算法中的常见trick

最近忙着打比赛，感觉看论文也很难静下心来了。基本上看得相当匆忙，主要还是以应用为主。上周压力比较大，没有来得及更新，感觉再不更就说不过去了。

因为比赛比较追求performance，所以想着把以前看到的一些trick或者还不够了解的trick稍作整理，以后发现新的就更新到这里。像BN和dropout的使用或者align、fpn这些常见的or偏向算法层面的就不提了。主要还是探讨一些DL算法以外的东西吧。

另外因为个人还是见识不够，所以希望大家能在评论里多多留言补充讨论。

目录如下：
- Data Augmentation
- OHEM
- Soft NMS
- Multi Scale Training/Testing

## Data Augmentation

中文来说叫数据增广，数据增广可以增加数据集的数量和多样性，从而提升算法的最终效果。卷积具有平移不变性，因此数据增广一般使用的是更为复杂的策略，例如翻转、旋转或者裁切、放缩。下面说的Multi Scale Training其实也可以看做数据增广的一种。

考虑到实现的方便性，目前比较常见的实现一般是随机对图像进行翻转，网络允许的话，也会加入一些随机放缩到一组特定尺寸中的某个。

最好是增强后的数据不改变原数据的分布，还有就是尽可能多增强一些难检测难分类样本，如果增强的足够多是可以克服过拟合，增加模型的泛华能力

裁切倒是在SSD里看过，但是据说这部分源码实现比较混乱，有兴趣的不妨研究下。

## OHEM

即online hard example mining，从名字也可以看出它和Hard Negative Mining有一定的关系。这里给出R-FCN论文中对它的描述：

![](https://img-blog.csdnimg.cn/20181219210700511.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

概况地说，OHEM不是随机选择正负样本，而是会对所有RoIs的损失进行评估，选择那些loss大的样本来优化网络。

## Soft NMS

NMS想必大家都很熟悉了，对于下图这种情况，NMS可以有效筛除多余的检测框，让结果更加精准

![](https://img-blog.csdnimg.cn/20181210115110523.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

但是另一些情况，NMS会把重叠、贴近的目标筛掉，比如下图中的人和马因为贴的太近，很容易被NMS排除掉一个框……



![](https://img-blog.csdnimg.cn/20181210115127305.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

在这种情况下，Soft NMS就提出了，它们两种算法的区别在于：



![](https://img-blog.csdnimg.cn/20181210115145862.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

可以看到，NMS算法本身是先对得分进行排序，然后从输出结果挑出其中得分最高的放入最终输出的检测结果的部分，接着遍历剩下的框，如果IoU重叠超过某个阈值，就直接排除掉这个框；遍历一次之后，再从剩下的框里按得分排序，挑出得分最高的加入最终输出的结果……如此不断循环直到最后输出结果里所有的框都被挑出来或者排除掉……

Soft NMS本质上只改变了一步，就是IoU如果超过某个阈值，不再是直接排除掉这个框，而是降低它的得分。得分如果低到一定程度就会被排除，但是如果降低后仍然比较高，就有保留下来的机会。

降低分数的方法有线性和高斯：

![](https://img-blog.csdnimg.cn/20181210114957821.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)



个人 感觉softnms对遮挡严重情况处理较好，否则效果不一定好不好 

## 图识别ining/Testing

很多网络并不关注输入图像的size，在这种情况下，可以在训练的时候将一张图像随机resize到多个尺寸中的一个进行训练，而测试的时候可以将图像resize各个尺寸，得到不同尺度的框做NMS，思路和图像金字塔比较像。

前者可以有效增加数据的多样性，提高网络的性能。而即使没有使用Multi Scale Training也一样可以使用Multi Scale Testing，一般来说对提升performance也有帮助。



