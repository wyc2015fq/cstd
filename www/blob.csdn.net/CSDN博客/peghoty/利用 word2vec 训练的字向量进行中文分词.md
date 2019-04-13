
# 利用 word2vec 训练的字向量进行中文分词 - peghoty - CSDN博客


2013年12月04日 18:28:04[皮果提](https://me.csdn.net/peghoty)阅读数：30334


最近针对之前发表的一篇博文[《Deep Learning 在中文分词和词性标注任务中的应用》](http://blog.csdn.net/itplus/article/details/13616045)中的算法做了一个实现，感觉效果还不错。本文主要是将我在程序实现过程中的一些数学细节整理出来，借此优化一下自己的代码，也希望为对此感兴趣的朋友提供点参考。文中重点介绍训练算法中的模型参数计算，以及 Viterbi 解码算法。

![](https://img-blog.csdn.net/20131215200652687)
![](https://img-blog.csdn.net/20131204180439906)![](https://img-blog.csdn.net/20131204180446109)![](https://img-blog.csdn.net/20131204180452812)![](https://img-blog.csdn.net/20131204180458765)![](https://img-blog.csdn.net/20131204180504796)![](https://img-blog.csdn.net/20131204180511828)![](https://img-blog.csdn.net/20131204180519078)![](https://img-blog.csdn.net/20131204180525015)![](https://img-blog.csdn.net/20131204180530593)![](https://img-blog.csdn.net/20131204180536156)

**相关链接**：
[《Deep Learning 在中文分词和词性标注任务中的应用》](http://blog.csdn.net/itplus/article/details/17122431)
[《Deep Learning for Chinese Word Segmentation and POS tagging》](http://www.aclweb.org/anthology/D/D13/D13-1061.pdf)
[《UFLDL 教程学习笔记（一）神经网络》](http://blog.csdn.net/itplus/article/details/11021593)
[《HMM 自学教程（六）维特比算法》](http://blog.csdn.net/itplus/article/details/15335921)
[《为什么我们喜欢用 sigmoid 这类 S 型非线性变换?》](http://blog.csdn.net/itplus/article/details/11496595)

作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/17122431](http://blog.csdn.net/itplus/article/details/17122431)
欢迎转载/分享, 但请务必声明文章出处.



