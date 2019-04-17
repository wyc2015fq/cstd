# 深度学习：前馈神经网络与BP反向传播 - Machine Learning with Peppa - CSDN博客





2018年06月15日 20:42:15[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：222








# 1. 定义
前馈神经网络（feedforward neural network），简称前馈网络，是人工神经网络的一种。在此种神经网络中，各神经元从输入层开始，接收前一级输入，并输出到下一级，直至输出层。整个网络中无反馈，可用一个有向无环图表示。

前馈神经网络采用一种单向多层结构。其中每一层包含若干个神经元，同一层的神经元之间没有互相连接，层间信息的传送只沿一个方向进行。其中第一层称为输入层。最后一层为输出层．中间为隐含层，简称隐层。隐层可以是一层。也可以是多层。

对于前馈神经网络结构设计，通常采用的方法有3类：直接定型法、修剪法和生长法。直接定型法设计一个实际网络对修剪法设定初始网络有很好的指导意义；修剪法由于要求从一个足够大的初始网络开始，注定了修剪过程将是漫长而复杂的，更为不幸的是，BP训练只是最速下降优化过程，它不能保证对于超大初始网络一定能收敛到全局最小或是足够好的局部椎小。因此，修剪法并不总是有效的，生长法似乎更符合人的认识事物、积累知识的过程，具有自组织的特点，则生长法可能更有前途，更有发展潜力。

**常见前馈神经网络**
- 感知器网络
感知器（又叫感知机）是最简单的前馈网络，它主要用于模式分类，也可用在基于模式分类的学习控制和多模态控制中。感知器网络可分为单层感知器网络和多层感知器网络。
- BP网络
BP网络是指连接权调整采用了反向传播（Back Propagation）学习算法的前馈网络。与感知器不同之处在于，BP网络的神经元变换函数采用了S形函数（Sigmoid函数），因此输出量是0~1之间的连续量，可实现从输入到输出的任意的非线性映射。
- RBF网络
RBF网络是指隐含层神经元由RBF神经元组成的前馈网络。RBF神经元是指神经元的变换函数为RBF（Radial Basis Function，径向基函数）的神经元。典型的RBF网络由三层组成：一个输入层，一个或多个由RBF神经元组成的RBF层（隐含层），一个由线性神经元组成的输出层。



# 2. 向前传播

下图直观解释了层神经网络模型向前传播的一个例子，圆圈表示神经网络的输入，“+1”的圆圈被称为偏置节点。神经网络最左边的一层叫做输入层，最右的一层叫做输出层。中间所有节点组成的一层叫做隐藏层。 


![这里写图片描述](https://img-blog.csdn.net/20160916155002721)

每个神经元的表达式如下： 


![这里写图片描述](https://img-blog.csdn.net/20160916155442973)





### 单层前馈神经网络


单层前馈神经网络是最简单的一种[人工神经网络](https://baike.baidu.com/item/%E4%BA%BA%E5%B7%A5%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C/382460)，其只包含一个输出层，输出层上节点的值（输出值）通过输入值乘以权重值直接得到。取出其中一个元进行讨论，其输入到输出的变换关系为




![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D124/sign=131a733c3f6d55fbc1c6722459234f40/e850352ac65c103866ced120b0119313b17e899b.jpg)



![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D128/sign=1e4bbe3cf21f3a295ec8d1cca124bce3/c8177f3e6709c93de2d011159d3df8dcd1005435.jpg)


上式中，![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D125/sign=828b2087b119ebc4c478729bb727cf79/0bd162d9f2d3572cea65833e8813632763d0c349.jpg)是输入[特征向量](https://baike.baidu.com/item/%E7%89%B9%E5%BE%81%E5%90%91%E9%87%8F/8663983)，![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D21/sign=52c428da79f40ad111e4c0e2552c86c3/37d12f2eb9389b50177dffb08735e5dde6116e55.jpg)是![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D12/sign=d1ec80b6014f78f0840b9ef178315481/359b033b5bb5c9ea3fcf1d01d739b6003af3b317.jpg)到![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D16/sign=ac167dfe13dfa9ecf92e521160d081c8/8435e5dde71190ef28038d3dcc1b9d16fcfa6053.jpg)的连接权，输出量![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D112/sign=7ba3990c9e82d158bf825db0b20b19d5/d31b0ef41bd5ad6e776e7b0883cb39dbb7fd3cb8.jpg)是按照不同特征的分类结果。




### 多层前馈神经网络


[多层前馈神经网络](https://baike.baidu.com/item/%E5%A4%9A%E5%B1%82%E5%89%8D%E9%A6%88%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C/10435655)有一个输入层，中间有一个或多个隐含层，有一个[输出层](https://baike.baidu.com/item/%E8%BE%93%E5%87%BA%E5%B1%82/7202179)。[多层感知器](https://baike.baidu.com/item/%E5%A4%9A%E5%B1%82%E6%84%9F%E7%9F%A5%E5%99%A8/10885549)网络中的输入与输出变换关系为


![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D321/sign=87254d8b272dd42a5b0907a9323a5b2f/3801213fb80e7bec04e81b032d2eb9389b506b25.jpg)



![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D166/sign=11899a9741a98226bcc12f21bc83b97a/f3d3572c11dfa9ec705e4ef560d0f703908fc1e4.jpg)



![](https://gss3.bdstatic.com/7Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D293/sign=89fd2ada2a381f309a198aa09a034c67/0b46f21fbe096b63e1f646b20e338744eaf8ac60.jpg)
这时每一层相当于一个单层前馈神经网络，如对第![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D13/sign=0a22553e56e736d15c13880b9a5029ac/aa18972bd40735fa54e1555b9c510fb30f24083a.jpg)层，它形成一个![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D30/sign=a494f01653da81cb4ae685cd506671fa/54fbb2fb43166d2246b00d76442309f79152d208.jpg)的[超平面](https://baike.baidu.com/item/%E8%B6%85%E5%B9%B3%E9%9D%A2/5360532)。它对于该层的输入模式进行线性分类，但是由于多层的组合，最终可以实现对输入模式的较复杂的分类。

# 3. 反向传播算法

反向传播算法的含义是：第l 层的一个神经元的残差是所有与该神经元相连的第l+ 1 层的神经元的残差的权重和，然后在乘上该神经元激活函数的梯度。

借网上一张图，反向传播算法可表示为以下几个步骤：

![图片名称](https://img-blog.csdn.net/20160916185141894)





