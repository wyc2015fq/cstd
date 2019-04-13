
# 多模态(RGB-D)——深度融合网络 - 小花生的博客 - CSDN博客


2018年10月04日 20:24:07[Peanut_范](https://me.csdn.net/u013841196)阅读数：3186


《Deeply-Fused Nets》
2016，Jingdong Wang et al. 深度融合网络
1.引言：
中心思想是深度融合,就是结合几个基网络(base networks)的中间表达(intermedia representations)作为每个基网络剩下的部分的输入,然后在几个中间表达上用了深度的结合.这个深度融合网络有几个优势:
（1）.它可以学到多尺度(multi-scale)的表达,因为它能够拥有更多基网络的优势.
（2）.在作者的融合网络里是一个深的基网络和一个浅的基网络组成,从深度基网络的早先的中间层到输出的信息流动,从深度基网络输入到后来的中间层都有所提升.
（3）.深度基网络和浅层基网络联合学习,可以相互受益.
2.深度融合网络：
![在这里插入图片描述](https://img-blog.csdn.net/20181004200402522?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1）Shallow Fusion：
一般的融合方法包括两个方法:
1.特征融合(feature fusion),网络一起提取的表达融合,之后接一个分类层.
2.决策融合(decision fusion),模型组合,融合网络计算的分类得分.
2） Deep Fusion
深度融合不仅仅在最后特征表达上面做特征融合，而且在中间特征表达上面做特征融合。
3 Analysis：
![在这里插入图片描述](https://img-blog.csdn.net/2018100420044953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
High capability of combining multi-scale representations.
由K个组的基网络组成的深度融合网络可以由另一个K个组的不同的基网络组成。
Improvement of the information flow.
早先的中间层到输出层有更短的路径。
4.讨论：
Concatenation, Maximization, and Summation.
在中间层用加法融合（Summation），对于每个基网络几乎没有改变：网络结构没有改变。唯一的影响是输出改变了，因为一些信号从其他网络加过来了。
Maximization fusion（最大值融合）是像素的最大值，和加法融合类似，对于每个基网络没有改变。
而concatenation fusion（拼接融合）： 举例来说，在GooLeNet中的inception模块。基网络要被改变，需要更多的参数：在每个基网络中，子序列子网络的输入大小在融合之后增加了，因为融合的结果变大了。或者在初始的基网络中有很多通道是0元素加到输出，来匹配子序列模块的输入。
![在这里插入图片描述](https://img-blog.csdn.net/20181004200551674?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
作者也展示了深度融合网络的单向版本，它与深度监督网络相似，分类层权值共享。
[
](https://img-blog.csdn.net/20181004200551674?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)5.融合方法的实验结果：
![在这里插入图片描述](https://img-blog.csdn.net/2018100420065863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
小结：
深度融合不仅融合最后的表达，而且融合基网络中的中间表达。它的优点有三点：
1.学习了多尺度的表达
2.信息的流动提升，训练一个有较深和较浅的网络组成的网络比一个深的基网络要容易。
3.深网络和浅网络相互学习受益。
实验结果显示作者的方法比ResNet和Highway要好，与现在state-of-the-arts来比有一个有竞争力的表现。
参考：[https://zhuanlan.zhihu.com/p/30336854](https://zhuanlan.zhihu.com/p/30336854)
---
注：博众家之所长，集群英之荟萃。

