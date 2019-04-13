
# 轻量化模型结构——SqueezeNet - 小花生的博客 - CSDN博客


2018年09月12日 22:53:03[Peanut_范](https://me.csdn.net/u013841196)阅读数：490



###### 《SqueezeNet: AlexNet-level accuracy with 50x fewer parameters and<0.5MB model size》
UC Berkely 和 Stanford University
**1.相同准确率下，更少参数量的模型有几点好处：**
1）更加高效的分布式训练
2）向客户端提供新模型的开销更小
3）FPGA和嵌入式设备部署更便利
**2.网络结构的设计策略**
1）使用1x1的卷积核代替3x3的卷积核，可以减少9倍参数
2）减少输入3x3卷积的特征图的数量，因为参数由NCHW决定。
3）延后下采样，使得卷积层有较大的activtion maps。较大的特征图会带来较高的准确率。
**3.SqueezeNet的核心Fire Module**
1）Fire module由两层构成，分别是squeeze层和expand层，如下图所示，squeeze层是由s1个1x1卷积核组成的，expand层由e1个1x1的卷积核和e3个3x3的卷积核组成的，得到的feature map进行concat。对应策略1
2）一般令s1 < e1 + e3, 这样可以限制输入到3x3卷积核的channels。对于策略2
3）stride为2的max-pooling只位于conv1，fire4，fire8和conv10.对于策略3
**其它细节：**
在3x3的输入特征图上加了1像素的zero-padding，使其与1x1的输出大小一致
squeeze和expand层后跟ReLU激活函数
fire9后加0.5的Dropout
没有FC层（可以较少大量参数）
![这里写图片描述](https://img-blog.csdn.net/20180912224528106?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**总体的网络结构：**
![这里写图片描述](https://img-blog.csdn.net/20180912224558887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
结构清晰直观，后两种结构吸收了ResNet的基本思想进行设计，发现一定程度上可以提升性能。
![这里写图片描述](https://img-blog.csdn.net/20180912224633342?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**各层的详细说明：**
![这里写图片描述](https://img-blog.csdn.net/201809122247053?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/201809122247053?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Fire model模块由三个可调参数：s1，e1，e3，分别代表卷积核的个数，同时也表示对应输出feature map的维数，由上图可知，作者提出的SqueezeNet结构中，e1=e3=4s1.
网络结构设计思想，同样与VGG的类似，堆叠的使用卷积操作，只不过这里堆叠的使用本文提出的Fire module。
**Squeezenet的参数数量以及性能：**
![这里写图片描述](https://img-blog.csdn.net/20180912224734855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912224734855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从这里可以看到，论文题目中提到的小于0.5M，是采用别的模型压缩技术获得的。在准确率差不多的情况下，SqueezeNet模型参数显著降低了50X。
[
](https://img-blog.csdn.net/20180912224734855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**SqueezeNet小结：**
1）Fire module与GoogLeNet思想类似，采用1x1卷积对feature map的维数进行[压缩],从而达到减少权值参数的目的；
2）采用与VGG类似的思想–堆叠的使用卷积，这里堆叠使用的是Fire module。
参考：[https://blog.csdn.net/u011995719/article/details/79100582](https://blog.csdn.net/u011995719/article/details/79100582)
---

###### 注：博众家之所长，集群英之荟萃。

[
  ](https://img-blog.csdn.net/20180912224734855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)