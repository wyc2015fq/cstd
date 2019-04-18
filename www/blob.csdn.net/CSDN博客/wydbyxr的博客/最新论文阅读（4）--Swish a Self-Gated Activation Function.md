# 最新论文阅读（4）--Swish: a Self-Gated Activation Function - wydbyxr的博客 - CSDN博客
2018年06月06日 21:28:28[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：139
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
所属专栏：[深度学习--最新论文](https://blog.csdn.net/column/details/23683.html)
# Swish: a Self-Gated Activation Function
```
- 2017年10月  
- Swish
- google
```
　　一种新的激活函数，公式为y=x*sigmoid（x）；在Inception、resnet、mobilenet中会比使用relu提高1%左右的性能；Swish是无上界有下界的函数。 
　　对于新的激活函数，在ImageNet上top-1 accuracy对于Mobile NASNet-A提高了0.9%，对Inception-ResNet-v2取得了0.6%的提升。 
　　Swish 的设计受到 LSTM 和 highway network 中使用 sigmoid 函数进行门控的启发。我们使用同样的值进行门控来简化门控机制，称为自门控（self-gating）。自门控的优势是它仅需要一个简单的标量输入，而正常的门控需要多个标量输入。该特性令使用自门控的激活函数如 Swish 能够轻松替换以单个标量作为输入的激活函数（如 ReLU），无需改变参数的隐藏容量或数量。 
　　该激活函数的公式和图像： 
　　公式为：y=x*sigmoid（x） 
![Swish](https://img-blog.csdn.net/20180606212357234?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180606212458536?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
　　该函数的导数公式和图像： 
![Swish导数公式](https://img-blog.csdn.net/20180606212606780?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180606212638138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
　　分析：Unboundedness（无界性）有利于防止在slow training时gradient逐渐趋于0，造成饱和。同时，being bounded也是有优势的，因为bounded的active function可以有很强的regulairzation，较大的negative inputs会被化解。 
同时，smoothness也是对optimization和generalization也是有着很重要的角色。 
　　该实验是：随机初始化一个6层神经网络，采集Swish layer后的loss landscape，对于一个smooth 的loss landscape更加容易去做优化，因为该Swish layer有着更强的traversable，同时减小了对初始化和learning rate的依赖。 
　　实现： 在 TensorFlow 等大多数深度学习库中只需更改一行代码即可实现 Swish 函数。 
　　性能：对于新的激活函数，在ImageNet上top-1 accuracy对于Mobile NASNet-A提高了0.9%，对Inception-ResNet-v2取得了0.6%的提升。
