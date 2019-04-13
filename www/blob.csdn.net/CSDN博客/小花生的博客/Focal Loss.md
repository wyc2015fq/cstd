
# Focal Loss - 小花生的博客 - CSDN博客


2019年03月18日 20:28:20[Peanut_范](https://me.csdn.net/u013841196)阅读数：44


### Focal Loss
论文：《Focal Loss for Dense Object Detection》
**问题：**目前主流的检测算法分为两个方向：（1）以R-CNN系列为代表的two-stage方向；（2）以YOLO系列为代表的one-stage方向。虽然one-stage方向的速度更快，但是其精度往往比较低。**究其原因，有两个方面：**
1）正样本（Positive Example）和负样本（Negative Example）的不平衡；
2）难样本（Hard Example）和易样本（Easy Example）的不平衡。
**这些不平衡造成模型的效果不准确的原因如下：**
1）Negative example的数量过多，导致Postive example的loss被覆盖，就算Postive example的loss非常大也会被数量庞大的negative example中和掉，这些positive example往往是要检测的前景区域；
2）Hard example往往是前景和背景区域的过渡部分，因为这些样本很难区分，所以叫做Hard Example。剩下的那些Easy example往往很好计算，导致模型非常容易就收敛了。但是损失函数收敛了并不代表模型效果好，因为我们其实更需要把那些hard example训练好。
**本文采用的解决方法是基于交叉熵提出了一个新的损失函数Focal Loss（FL）：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318200919628.png)
FL是一个尺度动态可调的交叉熵损失函数，在FL中有两个参数$\alpha_{t}$和γ，其中$\alpha_{t}$主要作用是解决正负样本的不平衡，γ主要是解决难易样本的不平衡。
[
](https://img-blog.csdnimg.cn/20190318200919628.png)**Focal Loss思路：**
Focal Loss是交叉熵损失的改进版本，一个二分类交叉熵可以表示为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318201350533.png)
其中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318201404857.png)
上面公式可以简写成：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318201414605.png)
**1）α：解决正负样本不平衡**
平衡交叉熵的提出是为了**解决正负样本不平衡**的问题的。它的原理很简单，为正负样本分配不同的权重比值$\alpha\in [0,1]$，当y=1时取α，为-1时取1-α。我们使用和$p_{t}$类似的方法将上面α的情况表示为$\alpha_{t}$，即：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318201717160.png)
那么这个α-balanced交叉熵损失可以表示为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318201739451.png)
ɑ的值往往需要根据验证集进行调整，论文中给出的是0.25。
[
](https://img-blog.csdnimg.cn/20190318201739451.png)**2）γ：解决难易样本不平衡**
FL中γ的引入是为了**解决难易样本不平衡**的问题的。图1是FL中example预测概率和loss值之间的关系。其中蓝色曲线是交叉熵（γ=0 时Focal Loss退化为交叉熵损失）的曲线。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318201806148.png)
图1：CE损失和FL损失曲线图
从图1的曲线中我们可以看出对于一些well-classified examples (easy examples)虽然它们单个example的loss可以收敛到很小，但是由于它们的数量过于庞大，把一些hard example的loss覆盖掉。导致求和之后他们依然会支配整个批次样本的收敛方向。
一个非常简单的策略是继续缩小easy examples的训练比重。作者的思路很简单，给每个乘以$(1-p_{t})^{\gamma}$。因为easy example的score$p_{t}$往往接近1，那么$(1-p_{t})^{\gamma}$值会比较小，因此example得到了抑制，相对的hard example得到了放大，例如图2中$\gamma&gt;0$。的那四条曲线。
FL的求导结果如公式:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318202201782.png)
γ的值也可以根据验证集来调整，论文中给出的值是2。
[
](https://img-blog.csdnimg.cn/20190318202201782.png)**3）FL的最终形式**
结合1）的α和2）的γ，便有了FL的最终形式。作者也通过实验验证了结合两个策略的实验效果最好。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318202226348.png)
**后续：**Focal Loss的最终形式并不是一定要严格的是上式的情况，但是它应满前文的分析，即能缩小easy example的比重。例如在论文附录A中给出的另外一种Focal Loss：$FL^{*}$，曲线见图2。它能取得和FL类似的效果。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318202341879.png)
图2：CE损失和FL*损失曲线图
公式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019031820240183.png)
摘录：
[https://zhuanlan.zhihu.com/p/48958966](https://zhuanlan.zhihu.com/p/48958966)
参考：
[https://zhuanlan.zhihu.com/p/49981234](https://zhuanlan.zhihu.com/p/49981234)
[https://zhuanlan.zhihu.com/p/47851985](https://zhuanlan.zhihu.com/p/47851985)
---

###### 注：博众家之所长，集群英之荟萃。

