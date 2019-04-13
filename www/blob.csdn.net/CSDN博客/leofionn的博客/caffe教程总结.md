
# caffe教程总结 - leofionn的博客 - CSDN博客


2018年07月23日 22:57:59[leofionn](https://me.csdn.net/qq_36142114)阅读数：107


直接放链接了：
1.这个是关于人脸识别的：http://www.jianshu.com/p/a76c18a3c6d5
2.这个是讲解了一些基础的fine tuning,caffe的基础入门教程：http://www.voidcn.com/article/p-nigpneqn-bac.html
3.github上的一个教程，因为github不仅仅可以上传代码，还可以上传教程，类似于csdn的博客，放链接：
https://github.com/humphd/have-fun-with-machine-learning/blob/master/README_zh-tw.md
可以尝试减小学习率base_lr（默认为0.01），同样对于fine-tuning，也应设置个较低的base_lr（0.001）
对于fine-tuning，还应适当降低stepsize的值（20000），其实就是避免初始模型变得太快而导致发散

输入数据格式改为imagedata，相应的data_param也改为image_data_param，mean_file也不用了，此外由于图像尺寸较大（340*250），需要适当缩小batch_size的值，否则使用GPU时可能超出其缓存大小而报错
最后一层的全连接层的输出改为2（因为这个例子是2元分类），对于fine-tuning，这一层的lr_mult需要适当增加（10），因为这一层在fine-tuning中需要变的最多
4.讲fine-tuning 的一个博客园的博客：http://www.cnblogs.com/louyihang-loves-baiyan/p/5038758.html
具体再看一下这个链接，写的还不错吧，当做第一个fine tuning例子用来跑DenseNet的fine tuning，试一下是否可行
fine-tuning的原因：在这种情况下重新训练一个新的网络是比较复杂的，而且参数不好调整，数据量也不够，因此fine-tuning微调就是一个比较理想的选择。
fine-tuning与training from scratch（重新训练）的区别：fine-tuning是基于之前已经训练好的caffemodel进行继续训练，training from scratch表示重新训练

那么在网络的微调中，我们的整个流程分为以下几步：
依然是准备好我们的训练数据和测试数据
计算数据集的均值文件，因为集中特定领域的图像均值文件会跟ImageNet上比较General的数据的均值不太一样
修改网络最后一层的输出类别，并且需要加快最后一层的参数学习速率
调整Solver的配置参数，通常学习速率和步长，迭代次数都要适当减少
启动训练，并且需要加载pretrained模型的参数

