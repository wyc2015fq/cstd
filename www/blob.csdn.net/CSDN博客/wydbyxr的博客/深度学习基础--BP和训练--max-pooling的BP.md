# 深度学习基础--BP和训练--max-pooling的BP - wydbyxr的博客 - CSDN博客
2018年11月13日 09:55:35[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：119
# max-pooling的BP
  对于max-pooling，其正向计算如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113095516414.png)
  反向计算如下图，其中，需要说明反向计算过程是将残差传递至上一层最大值的位置，其它位置置0。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113095523678.png)
