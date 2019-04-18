# 最新论文阅读（14）--Spiking Deep Residual Network - wydbyxr的博客 - CSDN博客
2018年06月07日 20:57:32[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：110
# Spiking Deep Residual Network
```
- 
- Spiking Neural Network
- 浙江大学和四川大学
```
　　脉冲神经网络（SNN）在生物理论中备受关注。理论上，SNN具有与ANN一样的计算能力，而且在功耗方面要比ANN更加节能。但是，在目前看来，训练一个很深的SNN依旧存在很大困难。 
　　本文提出了一种脉冲版本的 ResNet。我们把训练好的ResNet网络转化为脉冲神经元组成的SpikingResNet。为了解决转化过程中的问题： 
　　1）使用了一种快捷的正则化机制，适当地对连续值的激活函数进行放缩（标准化）。换句话说，本文提出了shotcut归一化机制来缩减连续的激活值，使之近似匹配SNN中的脉冲发放率，用来匹配脉冲神经网络中的脉冲激发频率。 
　　2）我们还采用了分层的误差补偿方法来减少离散化带来的误差，即提出一种逐层误差补偿方法来减少由离散导致的误差。 
　　在 MNIST、CIFAR 等数据集上实验得到了 state of the art的结果。
### 转换ResNet到SpikingResNet的机制
![这里写图片描述](https://img-blog.csdn.net/20180607205648616?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 关键细节
　　通过假定Relu单元可以近似脉冲神经元的发放率，文章用IF神经元代替Relu。输入方式是图片输入，第一层卷积之后根据是否大于阈值判断输出脉冲。训好的权重先进行归一化，再根据BN缩减（论文中的2式）使用。 
　　为了保持单元最大脉冲发放率，文章通过block的最大激活值来标准化block的输出。
参考资料： 
[https://blog.csdn.net/Kerrwy/article/details/80330802](https://blog.csdn.net/Kerrwy/article/details/80330802)
[https://cloud.tencent.com/developer/news/208552](https://cloud.tencent.com/developer/news/208552)”
