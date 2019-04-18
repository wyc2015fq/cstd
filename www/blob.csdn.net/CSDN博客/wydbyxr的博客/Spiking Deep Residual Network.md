# Spiking Deep Residual Network - wydbyxr的博客 - CSDN博客
2018年11月29日 09:43:34[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：33
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# Spiking Deep Residual Network
  近些年，SNN由于它的生物可塑性吸引了大众注意力。理论上，SNN具有与ANN一样的计算能力，而且在功耗方面要比ANN更加节能。但是，在目前看来，训练一个很深的SNN依旧存在很大困难。
  本篇文章，提出了一个方法来构造ResNet的脉冲版本。我们把训练好的ResNet网络转化为脉冲神经元组成的SpikingResNet。为了解决转化过程中的问题，文章提出了shotcut归一化机制来缩减连续的激活值，使之近似匹配SNN中的脉冲发放率；还提出一种逐层误差补偿方法来减少由离散导致的误差。
  在MNIST, CIFAR-10, 和CIFAR-100上做了实验，是目前SNN中性能最好的。
  参考资料：
[https://blog.csdn.net/Kerrwy/article/details/80330802](https://blog.csdn.net/Kerrwy/article/details/80330802)
[https://cloud.tencent.com/developer/news/208552](https://cloud.tencent.com/developer/news/208552)
## 转换ResNet到SpikingResNet的机制
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129094217192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 关键细节
  通过假定Relu单元可以近似脉冲神经元的发放率，文章用IF神经元代替Relu。输入方式是图片输入，第一层卷积之后根据是否大于阈值判断输出脉冲。训好的权重先进行归一化，再根据BN缩减（论文中的2式）使用。
  为了保持单元最大脉冲发放率，文章通过block的最大激活值来标准化block的输出。
