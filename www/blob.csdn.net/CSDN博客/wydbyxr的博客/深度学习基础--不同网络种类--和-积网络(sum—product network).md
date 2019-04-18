# 深度学习基础--不同网络种类--和-积网络(sum—product network) - wydbyxr的博客 - CSDN博客
2018年11月09日 11:07:42[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：338
# 和–积网络(sum—product network)
  这种模型是基于和积运算的。
  深度学习最主要的困难是配分函数的学习，如何选择深度结构神经网络的结构使得配分函数更容易计算。就此，Poon等人提出一种新的深度模型结构----和–积网络(sum—product network，SPN)，引入多层隐单元表示配分函数，使得配分函数更容易计算。
## 网络结构
  SPN是有根节点的有向无环图，图中的叶节点为变量，中间节点执行和运算与积运算，连接节点的边带有权值。
  它们在Caltech-101和Olivetti两个数据集上进行实验证明了SPN的性能优于DBN和最近邻方法。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109110701177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018110911071789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
