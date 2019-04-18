# NN模型设置--网络整体结构设计 - wydbyxr的博客 - CSDN博客
2018年12月04日 10:32:16[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：115
# 网络的设计
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181204102950793.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
# 网络结构设计
  1）金字塔式的形状
  你也许经常在表征能力和减少冗余或者无用信息之间权衡。卷积神经网络通常会降低激活函数的采样，并会增加从输入层到最终层之间的连接通道。
  2）Summation Joining
  Summation 是一种常用的合并分支的方式。在 ResNets 中，使用总和作为连接的机制可以让每一个分支都能计算残差和整体近似。如果输入跳跃连接一直存在，那么 summation 会让每一层学到正确地东西（例如与输入的差别）。在任何分支都可以被丢弃的网络（例如 FractalNet）中，你应该使用这种方式类保持输出的平滑。
# 设计CNN
```
网络有多深？	
网络有多宽？	
每一层是要用什么结构？线性层还是卷积层？	
层与层之间应该如何连接？	
应该使用什么样的 Activation？	
应该使用什么样的优化算法？	
优化算法的初始步长是多少？	
初始步长在训练过程中应该如何下降？	
应该使用什么样的初始化？	
是否需要使用 Momentum 算法？如果是，具体速率是多少？	
卷积层里面是否要加入常数项？	
是否需要使用 Dropout？	
是否需要使用 Batch norm？是否需要自动调整 Batch norm 的参数？	
是否需要使用 Weight decay？	
Weight decay 速度是多少？	
Mini batch 的大小是多少？
```
# 加深网络的方法
  采用简化的residual block 取代了原来 1×1 和 3×3的block; (其实就是加了一个shortcut，也是网络加深必然所要采取的手段)。
# Model size变小会有什么缺陷？
  回答：Model size变小通常会让模型的泛化能力变差，模型更加容易过拟合。所以对于训练更小的模型，模型的结构设计就很重要，不同的模型结构泛化能力是不同的。
