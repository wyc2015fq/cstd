
# 头部姿态估计——QuatNet - 小花生的博客 - CSDN博客


2018年10月06日 11:00:35[Peanut_范](https://me.csdn.net/u013841196)阅读数：759


《QuatNet: Quaternion-based Head Pose Estimation with Multi-regression Loss》
2018，Heng-Wei Hsu et al. QuatNet
1.引言：
作者提出了multi-regression loss function来使用CNNs进行头部姿态估计，输入采用RGB，并没有深度（Depth）信息。损失函数为L2 regression loss结合ordinal regression loss。
1）ordinal regression loss是被用于去处理non-stationary property，学习鲁棒的特征。
![在这里插入图片描述](https://img-blog.csdn.net/20181006103803272?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
The non-stationary property：不同的头部姿态角度面部的特征将会发生变化。
Ordinal regression learns 去预测labels的排序，而非label值本身，当labels的顺序扮演着更加重要的角色相比于它们实际的数值，这是非常重要和有效的。
2）L2 regression loss利用特征去提供更加精确的角度预测
3）Label：本文使用Euler angle和quaternions作为网络回归的结果，发现基于四元数的效果更好，因此论文题目为：QuatNet。
2. QuatNet网络结构：
![在这里插入图片描述](https://img-blog.csdn.net/20181006103839732?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
QuatNet基础网络采用GoogLeNet model，最后一层池化层被替换为本文提出的regression nets和ranking nets。
[
](https://img-blog.csdn.net/20181006103839732?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)网络结构的细节：
![在这里插入图片描述](https://img-blog.csdn.net/201810061039037?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Regression Net:
这4个回归网络采用相同的网络结构，一个256维的全连接层紧接着一个维度为1全连接层。
对应着四元数的4个值，如果为欧拉角就为3个值。
Ranking Net:
N多个二分类子网络，![在这里插入图片描述](https://img-blog.csdn.net/20181006103936189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注：ranking nets只在训练阶段进行约束，测试时去掉该网络。
[
](https://img-blog.csdn.net/20181006103936189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)QuatNet的损失函数为：
![在这里插入图片描述](https://img-blog.csdn.net/20181006104020932?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[，其中](https://img-blog.csdn.net/20181006104020932?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
$$
\lambda ^{Q}
$$
[=0.1。](https://img-blog.csdn.net/20181006104020932?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.EulerNet网络细节：
![在这里插入图片描述](https://img-blog.csdn.net/20181006104339685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ranking的角度限制：
![在这里插入图片描述](https://img-blog.csdn.net/20181006104631475?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.不同模型的结果比较：
![在这里插入图片描述](https://img-blog.csdn.net/2018100610475129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从结果可以看出，加入ranking loss效果有提升。
[
](https://img-blog.csdn.net/2018100610475129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)Methods所对应的方法为：
![在这里插入图片描述](https://img-blog.csdn.net/20181006104824329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
regression net, ranking net and cross-entropy net结构的细节：
![在这里插入图片描述](https://img-blog.csdn.net/20181006104908290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.评价标准：
MAE（平均绝对误差）和每个角度的累计误差分布曲线（cumulative error distribution curve）
The cumulative error distribution curve reflects the proportion of test images whose errors are
below a certain threshold.
反应了测试数据在一定阈值下的错误数据比例。eg：
$$
\pm 15^{o}
$$
1）MAE：
![在这里插入图片描述](https://img-blog.csdn.net/20181006105246562?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）cumulative error distribution curve：
![在这里插入图片描述](https://img-blog.csdn.net/20181006105445820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181006105713432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6.小结：
使用L2 regression loss结合ordinal regression loss可以提升姿态估计的准确率。
---
注：博众家之所长，集群英之荟萃。

[
  ](https://img-blog.csdn.net/20181006105445820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)