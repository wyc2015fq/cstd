# 一文说清楚pytorch和tensorFlow的区别究竟在哪里 - ibelieve8013的博客 - CSDN博客





2018年11月19日 23:05:52[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：2810








最近用了一点pytorch，想着稍稍理一下，这样一个和TensorFlow抗衡的一个框架，究竟是何方神圣？

首先我们要搞清楚pytorch和TensorFlow的一点区别，那就是pytorch是一个动态的框架，而TensorFlow是一个静态的框架。何为静态的框架呢？我们知道，TensorFlow的尿性是，我们需要先构建一个TensorFlow的计算图，构建好了之后，这样一个计算图是不能够变的了，然后我们再传入不同的数据进去，进行计算。这就带来一个问题，就是固定了计算的流程，势必带来了不灵活性，如果我们要改变计算的逻辑，或者随着时间变化的计算逻辑，这样的动态计算TensorFlow是实现不了的，或者是很麻烦。

但是pytorch就是一个动态的框架，这就和python的逻辑是一样的，要对变量做任何操作都是灵活的。

举个简单的例子，当我们要实现一个这样的计算图时：

![](https://img-blog.csdnimg.cn/2018111922402452.png)

用TensorFlow是这样的：

![](https://img-blog.csdnimg.cn/20181119224220631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

而用pytorch是这样的：

![](https://img-blog.csdnimg.cn/20181119224247531.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

当然，里面都包含了建立前向计算图，传入变量数据，求梯度等操作，但是显而易见，pytorch的代码更为凝练。TensorFlow我也用得比较少，我本文重点说说pytorch的一些学习心得，一是总结，而是若有缘人能看见此文，也能当个参考。

其实一个好的框架应该要具备三点：对大的计算图能方便的实现；能自动求变量的导数；能简单的运行在GPU上；pytorch都做到了，但是现在很多公司用的都是TensorFlow，而pytorch由于比较灵活，在学术科研上用得比较多一点。鄙人认为可能，Google可能下手早一些，而Facebook作后来者，虽然灵活，但是很多公司已经入了TensorFlow的坑了，要全部迁移出来还是很费功夫；而且，TensorFlow在GPU的分布式计算上更为出色，在数据量巨大时效率比pytorch要高一些，我觉得这个也是一个重要的原因吧。

好的，不闲扯了。pytorch的一些心得，我总结一下：

首先，pytorch包括了三个层次：tensor,variable,Module。tensor，即张量的意思，由于是矩阵的运算，十分适合在GPU上跑。但是这样一个tensor为什么还不够呢？要搞出来一个variable，其实variable只是tensor的一个封装，这样一个封装，最重要的目的，就是能够保存住该variable在整个计算图中的位置，详细的说：能够知道计算图中各个变量之间的相互依赖关系。什么，你问这有什么用？当然是为了反向求梯度了；而Module，是一个更高的层次，如果使用这个Module，那可厉害了，这是一个神经网络的层次，可以直接调用全连接层，卷积层，等等神经网络。

感觉写不完了，有人看再更吧，如果又兴趣的话，可以去看我的这一课[https://blog.csdn.net/ibelieve8013/article/details/84206410](https://blog.csdn.net/ibelieve8013/article/details/84206410)的导图，只要你认真看，真的很清晰的。



