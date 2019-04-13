
# DistBelief 框架下的并行随机梯度下降法 - Downpour SGD - peghoty - CSDN博客


2014年06月17日 22:39:45[皮果提](https://me.csdn.net/peghoty)阅读数：15926



本文是读完 Jeffrey Dean, Greg S. Corrado 等人的文章[Large Scale Distributed Deep Networks](http://machinelearning.wustl.edu/mlpapers/paper_files/NIPS2012_0598.pdf)(2012) 后的一则读书笔记，重点介绍在 Google 的软件框架 DistBelief 下设计的一种用来训练大规模深度神经网络的随机梯度下降法 — Downpour SGD，该方法通过分布式地部署多个模型副本和一个“参数服务器”，同时实现了模型并行和数据并行，且对机器失效问题具有很好的容错性。结合 Adagrad 自适应学习率使用，对非凸优化问题有很好的效果。

![](https://img-blog.csdn.net/20140617221835156)

![](https://img-blog.csdn.net/20140623205901078)

![](https://img-blog.csdn.net/20140617221855000)

![](https://img-blog.csdn.net/20140617221911093)

![](https://img-blog.csdn.net/20140617221922734)![](https://img-blog.csdn.net/20140617221934593)

![](https://img-blog.csdn.net/20140617221941390)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/31831661](http://blog.csdn.net/itplus/article/details/31831661)
欢迎转载/分享, 但请务必声明文章出处.

