# 学习笔记:Keras 错误提示 - 去向前方的博客 - CSDN博客





2018年07月22日 11:19:50[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：198








# 目录：



- [目录：](#目录)
- [Keras使用陷阱](#keras使用陷阱)
- [TF卷积核与TH卷积核](#tf卷积核与th卷积核)
- [向BN层中载入权重](#向bn层中载入权重)
- [Keras的可训练参数在前，不可训练参数在后](#keras的可训练参数在前不可训练参数在后)
- [Merge层的层对象与函数方法](#merge层的层对象与函数方法)




本系列参考官方文档[官方文档](http://keras-cn.readthedocs.io/en/latest/)

这就是keras可以参考前篇：[这就是keras](https://blog.csdn.net/lvsehaiyang1993/article/details/81062537)

学习笔记 Keras:一些基本概念 [一些基本概念](https://blog.csdn.net/lvsehaiyang1993/article/details/81113475)

Keras:常见问题[学习笔记：Keras常见问题](https://blog.csdn.net/lvsehaiyang1993/article/details/81132242)

Windows 下Keras安装和配置指南：[Windows 下Keras安装和配置指南](https://blog.csdn.net/lvsehaiyang1993/article/details/81149002)
# Keras使用陷阱

这里归纳了Keras使用过程中的一些常见陷阱和解决方法，当然未完待续，有新发现会立马补上。

# TF卷积核与TH卷积核

Keras提供了三套后端，Theano和Tensorflow和CNTK，这是一件幸福的事，就像手中拿着馒头，想蘸红糖蘸红糖，想蘸白糖蘸白糖 

如果你从无到有搭建自己的一套网络，则大可放心。但如果你想使用一个已有网络，或把一个用th/tf 训练的网络以另一种后端应用，在载入的时候你就应该特别小心了。 

卷积核与所使用的后端不匹配，不会报任何错误，因为它们的shape是完全一致的，没有方法能够检测出这种错误。 

在使用预训练模型时，一个建议是首先找一些测试样本，看看模型的表现是否与预计的一致。 

如需对卷积核进行转换，可以使用utils.convert_all_kernels_in_model对模型的所有卷积核进行转换
# 向BN层中载入权重

如果你不知道从哪里淘来一个预训练好的BN层，想把它的权重载入到Keras中，要小心参数的载入顺序。 

一个典型的例子是，将caffe的BN层参数载入Keras中，caffe的BN由两部分构成，bn层的参数是mean，std，scale层的参数是gamma，beta 

按照BN的文章顺序，似乎载入Keras BN层的参数应该是[mean, std, gamma, beta] 

然而不是的，Keras的BN层参数顺序应该是[gamma, beta, mean, std]，这是因为gamma和beta是可训练的参数，而mean和std不是
# Keras的可训练参数在前，不可训练参数在后

错误的权重顺序不会引起任何报错，因为它们的shape完全相同 

shuffle和validation_split的顺序 

模型的fit函数有两个参数，shuffle用于将数据打乱，validation_split用于在没有提供验证集的时候，按一定比例从训练集中取出一部分作为验证集 

这里有个陷阱是，程序是先执行validation_split，再执行shuffle的，所以会出现这种情况： 

假如你的训练集是有序的，比方说正样本在前负样本在后，又设置了validation_split，那么你的验证集中很可能将全部是负样本 

同样的，这个东西不会有任何错误报出来，因为Keras不可能知道你的数据有没有经过shuffle，保险起见如果你的数据是没shuffle过的，最好手动shuffle一下
# Merge层的层对象与函数方法

Keras定义了一套用于融合张量的方法，位于keras.layers.Merge，里面有两套工具，以大写字母开头的是Keras Layer类，使用这种工具是需要实例化一个Layer对象，然后再使用。以小写字母开头的是张量函数方法，本质上是对Merge Layer对象的一个包装，但使用更加方便一些。注意辨析。







