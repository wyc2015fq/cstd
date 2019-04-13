
# 受限玻尔兹曼机（RBM）学习笔记（五）梯度计算公式 - peghoty - CSDN博客


2014年02月14日 18:14:13[皮果提](https://me.csdn.net/peghoty)阅读数：50445



去年 6 月份写的博文《Yusuke Sugomori 的 C 语言 Deep Learning 程序解读》是囫囵吞枣地读完一个关于 DBN 算法的开源代码后的笔记，当时对其中涉及的算法原理基本不懂。近日再次学习 RBM，觉得有必要将其整理成笔记，算是对那个代码的一个补充。


**目录链接**
（一）预备知识
[（二）网络结构](http://blog.csdn.net/itplus/article/details/19168967)
[（三）能量函数和概率分布](http://blog.csdn.net/itplus/article/details/19168989)
（四）对数似然函数
[（五）梯度计算公式](http://blog.csdn.net/itplus/article/details/19207371)
[（六）对比散度算法](http://blog.csdn.net/itplus/article/details/19408143)
[（七）RBM 训练算法](http://blog.csdn.net/itplus/article/details/19408701)
（八）RBM 的评估

![](https://img-blog.csdn.net/20140214180006531)![](https://img-blog.csdn.net/20140214180015046)![](https://img-blog.csdn.net/20140214180023156)![](https://img-blog.csdn.net/20140214182009515)
![](https://img-blog.csdn.net/20140218225224828)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/19207371](http://blog.csdn.net/itplus/article/details/19207371)
欢迎转载/分享, 但请务必声明文章出处.


