# Swish: a Self-Gated Activation Function - wydbyxr的博客 - CSDN博客
2018年11月29日 09:59:06[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：43
# Swish: a Self-Gated Activation Function
  Swish 的设计受到 LSTM 和 highway network 中使用 sigmoid 函数进行门控的启发。我们使用同样的值进行门控来简化门控机制，称为自门控（self-gating）。自门控的优势是它仅需要一个简单的标量输入，而正常的门控需要多个标量输入。该特性令使用自门控的激活函数如 Swish 能够轻松替换以单个标量作为输入的激活函数（如 ReLU），无需改变参数的隐藏容量或数量。
## 激活函数的公式和图像
  公式为：y=x*sigmoid（x）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129095702369.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129095710478.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 激活函数的导数公式和图像
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129095807871.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129095743124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 分析
  1）Unboundedness（无界性）有利于防止在slow training时gradient逐渐趋于0，造成饱和。同时，being bounded也是有优势的，因为bounded的active function可以有很强的regulairzation，较大的negative inputs会被化解。
  2）同时，smoothness也是对optimization和generalization也是有着很重要的角色。
  该实验是：随机初始化一个6层神经网络，采集Swish layer后的loss landscape，对于一个smooth 的loss landscape更加容易去做优化，因为该Swish layer有着更强的traversable，同时减小了对初始化和learning rate的依赖。
## 实现
  在 TensorFlow 等大多数深度学习库中只需更改一行代码即可实现 Swish 函数。
## 性能
  对于新的激活函数，在ImageNet上top-1 accuracy对于Mobile NASNet-A提高了0.9%，对Inception-ResNet-v2取得了0.6%的提升。
