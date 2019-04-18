# 深度学习基础--不同网络种类--Hopfield神经网络模型 - wydbyxr的博客 - CSDN博客
2018年11月09日 11:04:03[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：265
# Hopfield神经网络模型
  Hopfield网络是一个著名的神经网络模型，通过对网络进行训练，可以让它记住相应的模式，并在适当的条件下联想回忆提取出相关的模式。也就是说，Hopfield模型通过训练（改变相互连接的权重），可以将要记忆的模式映射为能量最小的状态，之后通过Ising模型的邻域相互作用规则自发演化到这种最小能量状态。
## Ising模型与Hopfield神经网络模型
  Ising模型的一个显著的性质就是，随着系统的演化，它的能量会自发地降低。我们前面已经提到这种让整体降低能量的方法实际上与拷贝邻居状态的微观原则一致。于是，我们可以设计一种微观的演化机制，而使得宏观的某种待优化的函数（例如能量）能够自然地被优化。这就是Hopfield网络模型的起源。
## Hopfield的结构
  是一个加权的网络，如下图，每个节点都是一个神经元，加权的连边表示神经元之间的突触连接。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109110206635.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  在图中，第0层仅仅是作为网络的输入，它不是实际神经元，所以无计算功能；
  而第1层是实际神经元，故而执行对输人信息和权系数乘积求累加和，并由非线性函数f处理后产生输出信息。
  f是一个简单的阈值函效，如果神经元的输出信息大于阈值θ，那么，神经元的输出就取值为1；小于阈值θ，则神经元的输出就取值为θ。
### 二值神经元
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109110303845.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## Hopfield网络的运行
  分为两个阶段，它们的输入数粗示意图如下所示：		
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109110330864.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
