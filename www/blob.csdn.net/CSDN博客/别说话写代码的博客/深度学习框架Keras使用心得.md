# 深度学习框架Keras使用心得 - 别说话写代码的博客 - CSDN博客





2018年05月07日 17:52:03[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：295











本文转自：https://blog.csdn.net/xjcvip007/article/details/52801216

最近几个月为了写小论文，题目是关于用深度学习做人脸检索的，所以需要选择一款合适的深度学习框架，caffe我学完以后感觉使用不是很方便，之后有人向我推荐了Keras，其简单的风格吸引了我，之后的四个月我都一直在使用Keras框架，由于我用的时候，tensorflow的相关教程还不是很多，所以后端我使用theano。这次的心得主要分成两篇，第一篇是聊聊开始使用keras容易遇到的坑，第二篇会在我论文发了之后，连同代码和一些更细节的东西一起发出来，我也会在结尾放出一些自己收集的CNN和tensorflow的相关教程，就这样。

先放上Keras官方中文文档的链接：[http://keras-cn.readthedocs.io/en/latest/](http://keras-cn.readthedocs.io/en/latest/)
还有Keras的QQ群号：119427073

本文由浅入深聊一聊我在这个过程中遇到的一些问题：

#### 1、Keras输出的loss，val这些值如何保存到文本中去：

Keras中的fit函数会返回一个History对象，它的History.history属性会把之前的那些值全保存在里面，如果有验证集的话，也包含了验证集的这些指标变化情况，具体写法：

```python
hist=model.fit(train_set_x,train_set_y,batch_size=256,shuffle=True,nb_epoch=nb_epoch,validation_split=0.1)
with open('log_sgd_big_32.txt','w') as f:
    f.write(str(hist.history))
```


我觉得保存之前的loss，val这些值还是比较重要的，在之后的调参过程中有时候还是需要之前loss的结果作为参考的，特别是你自己添加了一些自己的loss的情况下，但是这样的写法会使整个文本的取名比较乱，所以其实可以考虑使用Aetros的插件，[Aetros网址](http://aetros.com/)，这是一个基于Keras的一个管理工具，可以可视化你的网络结构，中间卷积结果的可视化，以及保存你以往跑的所有结果，还是很方便的，就是有些不稳定，有时候会崩。。。

#### 2、关于训练集，验证集和测试集：

其实一开始我也没搞清楚这个问题，拿着测试集当验证集用，其实验证集是从训练集中抽取出来用于调参的，而测试集是和训练集无交集的，用于测试所选参数用于该模型的效果的，这个还是不要弄错了。。。在Keras中，验证集的划分只要在fit函数里设置validation_split的值就好了，这个对应了取训练集中百分之几的数据出来当做验证集。但由于shuffle是在validation _split之后执行的，所以如果一开始训练集没有shuffle的话，有可能使验证集全是负样本。测试集的使用只要在evaluate函数里设置就好了。

```bash
print model.evaluate（test_set_x，test_set_y ,batch_size=256）
```

这里注意evaluate和fit函数的默认batch_size都是32，自己记得修改。

#### 3、关于优化方法使用的问题：

开始总会纠结哪个优化方法好用，但是最好的办法就是试，无数次尝试后不难发现，Sgd的这种学习率非自适应的优化方法，调整学习率和初始化的方法会使它的结果有很大不同，但是由于收敛确实不快，总感觉不是很方便，我觉得之前一直使用Sgd的原因一方面是因为优化方法不多，其次是用Sgd都能有这么好的结果，说明你网络该有多好啊。其他的Adam，Adade，RMSprop结果都差不多，Nadam因为是adam的动量添加的版本，在收敛效果上会更出色。所以如果对结果不满意的话，就把这些方法换着来一遍吧。 
有很多初学者人会好奇怎么使sgd的学习率动态的变化，其实Keras里有个反馈函数叫LearningRateScheduler，具体使用如下：
- 
```python
def step_decay(epoch):
    initial_lrate = 0.01
    drop = 0.5
    epochs_drop = 10.0
    lrate = initial_lrate * math.pow(drop,math.floor((1+epoch)/epochs_drop))
    return lrate
lrate = LearningRateScheduler(step_decay)
sgd = SGD(lr=0.0, momentum=0.9, decay=0.0, nesterov=False)
model.fit(train_set_x, train_set_y, validation_split=0.1, nb_epoch=200, batch_size=256, callbacks=[lrate])
```




上面代码是使学习率指数下降，具体如下图：

![学习率变化](https://img-blog.csdn.net/20161013125150377)
当然也可以直接在sgd声明函数中修改参数来直接修改学习率，学习率变化如下图：
`sgd = SGD(lr=learning_rate, decay=learning_rate/nb_epoch, momentum=0.9, nesterov=True)`- 1

![这里写图片描述](https://img-blog.csdn.net/20161013125438330)
具体可以参考这篇文章[Using Learning Rate Schedules for Deep Learning Models in Python with Keras](http://machinelearningmastery.com/using-learning-rate-schedules-deep-learning-models-python-keras/)

#### 4、关于过拟合问题的讨论：

我现在所知道的解决方法大致只有两种，第一种就是添加dropout层，dropout的原理我就不多说了，主要说一些它的用法，dropout可以放在很多类层的后面，用来抑制过拟合现象，常见的可以直接放在Dense层后面，对于在Convolutional和Maxpooling层中dropout应该放置在Convolutional和Maxpooling之间，还是Maxpooling后面的说法，我的建议是试！这两种放置方法我都见过，但是孰优孰劣我也不好说，但是大部分见到的都是放在Convolutional和Maxpooling之间。关于Dropout参数的选择，这也是只能不断去试，但是我发现一个问题，在Dropout设置0.5以上时，会有验证集精度普遍高于训练集精度的现象发生，但是对验证集精度并没有太大影响，相反结果却不错，我的解释是Dropout相当于Ensemble，dropout过大相当于多个模型的结合，一些差模型会拉低训练集的精度。当然，这也只是我的猜测，大家有好的解释，不妨留言讨论一下。 
当然还有第二种就是使用参数正则化，也就是在一些层的声明中加入L1或L2正则化系数，正则化的原理什么的我就不细说了，具体看代码：

```bash
C1 = Convolution2D(20, 4, 4, border_mode='valid', init='he_uniform', activation='relu',W_regularizer=l2(regularizer_params))
```
- 1

其中W_regularizer=l2(regularizer_params)就是用于设置正则化的系数，这个对于过拟合有着不错的效果，在一定程度上提升了模型的泛化能力。

#### 5、Batchnormalization层的放置问题：

BN层是真的吊，简直神器，除了会使网络搭建的时间和每个epoch的时间延长一点之外，但是关于这个问题我看到了无数的说法，对于卷积和池化层的放法，又说放中间的，也有说池化层后面的，对于dropout层，有说放在它后面的，也有说放在它前面的，对于这个问题我的说法还是试！虽然麻烦。。。但是DL本来不就是一个偏工程性的学科吗。。。还有一点是需要注意的，就是BN层的参数问题，我一开始也没有注意到，仔细看BN层的参数：
`keras.layers.normalization.BatchNormalization(epsilon=1e-06, mode=0, axis=-1, momentum=0.9, weights=None, beta_init='zero', gamma_init='one')`- 1
- 
mode：整数，指定规范化的模式，取0或1

0：按特征规范化，输入的各个特征图将独立被规范化。规范化的轴由参数axis指定。注意，如果输入是形如（samples，channels，rows，cols）的4D图像张量，则应设置规范化的轴为1，即沿着通道轴规范化。输入格式是‘tf’同理。

1：按样本规范化，该模式默认输入为2D


我们大都使用的都是mode=0也就是按特征规范化，对于放置在卷积和池化之间或之后的4D张量，需要设置axis=1，而Dense层之后的BN层则直接使用默认值就好了。

总结
这次暂时先写这么多，这次写的比较浅，还有很多拓展的东西都下次再写了，下面给大家附上一些不错的资料吧~

CNN基础：
【1】[你真的了解CNN网络吗？](https://mp.weixin.qq.com/s?__biz=MzAwMjM3MTc5OA==&mid=403231780&idx=1&sn=4d6b282e45a016df274b72f08ddf8492&scene=1&srcid=0111p2ycZ4HF6hZ8N9eohts2&ascene=1&uin=OTE0NTE3ODQx&devicetype=webwx&version=70000001&pass_ticket=8WwTcRsKg44bcrNwjcBl175U1yC17S0EJItWCw7eG1u5/ripwp/MV3S1QmkuD7Sl)
【2】[CNN近期发展和实用技巧（上）](http://mp.weixin.qq.com/s?__biz=MzI1NTE4NTUwOQ==&mid=2650324619&idx=1&sn=ca1aed9e42d8f020d0971e62148e13be&scene=23&srcid=050393GFTFvObFwISl0iu8to#rd)
【3】[深度 | 从入门到精通：卷积神经网络初学者指南（附论文）](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650717691&idx=2&sn=3f0b66aa9706aae1a30b01309aa0214c&scene=0#wechat_redirect%20%E6%B7%B1%E5%BA%A6%20%7C%20%E4%BB%8E%E5%85%A5%E9%97%A8%E5%88%B0%E7%B2%BE%E9%80%9A%EF%BC%9A%E5%8D%B7%E7%A7%AF%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C%E5%88%9D%E5%AD%A6%E8%80%85%E6%8C%87%E5%8D%97%EF%BC%88%E9%99%84%E8%AE%BA%E6%96%87%EF%BC%89)
【4】[Convolutional Neural Networks (CNNs): An Illustrated Explanation](http://xrds.acm.org/blog/2016/06/convolutional-neural-networks-cnns-illustrated-explanation/)
【5】[Convolutional Neural Networks backpropagation: from intuition to derivation](https://grzegorzgwardys.wordpress.com/2016/04/22/8/)
【6】[贺完结！CS231n官方笔记授权翻译总集篇发布](https://zhuanlan.zhihu.com/p/21930884)

DL和Keras相关：
【1】[深度学习中的激活函数导引](http://mp.weixin.qq.com/s?__biz=MzI1NTE4NTUwOQ==&mid=2650325236&idx=1&sn=7bd8510d59ddc14e5d4036f2acaeaf8d&scene=1&srcid=0801MJXHmgtSeqaaE9Kdh9w2&from=groupmessage&isappinstalled=0#wechat_redirect)
【2】[深度网络的过拟合问题讨论](http://zongwei.leanote.com/post/%E6%B7%B1%E5%BA%A6%E7%BD%91%E7%BB%9C%E7%9A%84%E8%BF%87%E6%8B%9F%E5%90%88%E9%97%AE%E9%A2%98%E8%AE%A8%E8%AE%BA)
【3】[How To Improve Deep Learning Performance](http://machinelearningmastery.com/improve-deep-learning-performance/)
【4】[深度学习最全优化方法总结比较（SGD，Adagrad，Adadelta，Adam，Adamax，Nadam）](https://zhuanlan.zhihu.com/p/22252270)
【5】[Keras/Python深度学习中的网格搜索超参数调优（附源码）](http://geek.csdn.net/news/detail/95494)
【6】[Yoshua Bengio等大神传授：26条深度学习经验](http://www.csdn.net/article/2015-09-16/2825716)
【7】[leriomaggio/deep-learning-keras-euroscipy2016](https://github.com/leriomaggio/deep-learning-keras-euroscipy2016)

Tensorflow相关：
【1】[TensorFlow深度学习，一篇文章就够了](http://blog.jobbole.com/105602/)
【2】[alrojo/tensorflow-tutorial](https://github.com/alrojo/tensorflow-tutorial)
【3】[莫烦 tensorflow 神经网络 教程](http://list.youku.com/albumlist/show?id=27327189&ascending=1&page=1)



