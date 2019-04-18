# 深度学习基础--不同网络种类--限制玻尔兹曼机(RBM) - wydbyxr的博客 - CSDN博客
2018年11月09日 10:26:28[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：54
个人分类：[深度学习基础](https://blog.csdn.net/wydbyxr/article/category/6829999)
# 限制玻尔兹曼机(RBM)
  是一个关于输入(可见) 神经元v 和输出(隐藏) 神经元h 之间的概率生成模型。
  参考资料：
[https://blog.csdn.net/Young_Gy/article/details/70238133](https://blog.csdn.net/Young_Gy/article/details/70238133)
## RBM及其改进
  1）RBM的可见层和隐藏层的神经元之间通过一个权值矩阵w 和两个偏置向量c 和b 连接。在可见层神经元之间或者隐藏层神经元之间都没有连接。
  给定一组v 和h, 可定义该模型的能量函数为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109102542886.png)
  2）条件限制玻尔兹曼机(Conditional restricted Boltzman machines, CRBM)是限制玻尔兹曼机在时间维度上的一个扩展,
  3）空间- 时间深度信念网络(Space-time deep belief network,ST-DBN), ST-DBN 使用卷积RBM 神经网络将空间抽取层和时间抽取层组合在一起在视频数据上提取不变特征, 并在KTH 数据库上获得了91.13% 的识别率。
