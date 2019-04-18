# 深度学习基础--不同网络种类--teacher-student网络,蒸馏(distillation)网络 - wydbyxr的博客 - CSDN博客
2018年11月11日 09:47:52[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：554
# teacher-student网络,蒸馏(distillation)网络
  属于迁移学习的一种。迁移学习也就是将一个模型的性能迁移到另一个模型上，	对于教师——学生网络，教师网络往往是一个更加复杂的网络，具有非常好的性能和泛化能力，可以用这个网络来作为一个soft target来指导另外一个更加简单的学生网络来学习，使得更加简单、参数运算量更少的学生模型也能够具有和教师网络相近的性能，也算是一种模型压缩的方式。
## 学习过程
  学生网络的训练含有两个目标：一个是hard target，即原始的目标函数，为小模型的类别概率输出与label真值的交叉熵；另一个为soft target，为小模型的类别概率输出与大模型的类别概率输出的交叉熵。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109111028156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 改进版本
  来自《Paying More Attention to Attention: Improving the Performance of Convolutional Neural Networks via Attention Transfer》
  原来的方法有distillation，就是说希望输出的softmax要类似。这篇文章说，我不仅要softmax类似，我要当中的attention map也要类似。attention map他这里是用gradient来定义的。	
  借鉴Distilling的思想，使用复杂网络中能够提供视觉相关位置信息的Attention map来监督小网络的学习，并且结合了低、中、高三个层次的特征。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109111116844.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
### 具体
  教师网络从三个层次的Attention Transfer对学生网络进行监督。
  其中，“三个层次”对应了ResNet中三组Residual Block的输出。这三个层次的Attention Transfer是基于Activation的，即Activation Attention就是feature map在各个通道上的值求和。
