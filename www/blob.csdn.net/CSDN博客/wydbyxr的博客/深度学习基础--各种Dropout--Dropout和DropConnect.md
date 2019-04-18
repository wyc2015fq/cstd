# 深度学习基础--各种Dropout--Dropout和DropConnect - wydbyxr的博客 - CSDN博客
2018年11月12日 11:31:26[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：159
# dropout是一种正则化的方法
# Dropout和DropConnect
  其实在实验中我们经常使用的是dropout （(Hinton et al., 2012).）方法，dropconnect的方法只是对其进行了简单的改进
  在全连接层引入"Dropout"或"DropConnect"的方法，即在训练过程中以一定概率P将隐含层节点的输出值(对于"DropConnect"为输入权值)清0，而用反向传播算法更新权值时，不再更新与该节点相连的权值。		
  dropconnect的效果和dropoup比的话，会有一些提升，但是两者还是比较相似的，差别不大。这2种方法都会降低训练速度。
## 两者的比较
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112113206788.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
### 1）dropout
  是在全连接时，进行的随机放弃连接，文中介绍其是在进行激活函数运算之后乘以一个二进制的掩码矩阵（矩阵的中的0,1是随机的），这样就是dropout的计算，这样可以起到减少overfitting的作用，具体是减少前后神经元的一种适应性的依赖。
  Dropout作用：通过阻止特征检测器的共同作用来提高神经网络的性能可以看出，网络在提取训练集特征时，舍弃掉了一部分特征来提高网络的泛化能力。
### 2）DropConnect
  Dropout是将输出随机置0，而DropConnect是将权重随机置0。 文章说之所以这么干是因为原来的Dropout进行的不够充分，随机采样不够合理。
  训练的时候，训练过程与Dropout基本相同。测试的时候，我们同样需要一种近似的方法。但其实发现效果并不比Dropout优秀太多，反而计算量要大很多，因此到目前DropConnect并没有得到广泛的应用。
