# 深度学习基础--不同网络种类--ladder network - wydbyxr的博客 - CSDN博客
2018年11月11日 09:55:10[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：191
# ladder network
  参考资料：
[https://blog.csdn.net/hjimce/article/details/50877704](https://blog.csdn.net/hjimce/article/details/50877704)
[https://blog.csdn.net/swfa1/article/details/50866623](https://blog.csdn.net/swfa1/article/details/50866623)
## 优点
  1）与监督学习兼容，能够增加到现有的向前神经网络中，比如MLP或者CNN，论文证明了通过对监督学习的方法作为起点，增加了非监督的仿真算法，提高了目前最高的算法的准确率。
  2）从局部学习中的学习结果的可扩展性，除了有监督的的在顶层的目标之外，次模型在每一层都有一个学习的目标，对于非常深的神经网络是有效的。【防治梯度消失，与highway network一致】，本文用了两个深度神经网络的结构进行了验证。
  3）计算有效性：模型的编码部分与一般的监督学习有关。增加一个编码，近似使得训练时间变为三倍。然而如果可以更加充分的提取信息的话，这个过程是不必要的，同样的结果可以更快的得到。
