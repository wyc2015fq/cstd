
# Eager Mode,写在TensorFlow 2.0 到来之前 - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年12月07日 15:37:17[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：217


本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
[书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
[书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)
本文主要讲解了在编写基于TensorFlow的应用过程中如何使用Eager Mode。内容主要包括
Eager Mode简介
Eager Mode下的自动求导
在Eager Mode下创建一个模型并进行训练。
另外，本文还提供了基于TensorFlow底层API方式编写的实现同样功能的模型，读者可以对比两种不同编写方式的异同点。
**Eager Mode 简介**
在TensorFlow 团队今年8月份发布的关于TensorFlow 2.0即将到来的公告中我们可以看到，Eager executio将作为TensorFlow 2.0 的核心特征[1]，这预示着未来将有大量基于Eager Mode的应用出现。在今年年底TensorFlow 2.0 预览版发布之前，我们首先来回顾一下目前Eager Mode的功能。本文中使用的代码是基于1.12 版本的TensorFlow, 相信在对Eager Mode的使用方式方面与2.0 版本不会有重大区别，所有代码均可在SIGAI在线编程中的sharedata/intro_to_tf路径下免费获取，欢迎大家使用。
Eager Mode 是由大名鼎鼎的Google Brain团队首先开始研发，最后合并到TensorFlow框架下的一种命令式编程的方式，其首次公开发布于2017年10月31日[2]。在Eager Mode 推出之前，基于TensorFlow的程序使用的静态计算图，计算过程首先被编译成一个有向无环图。这种模式下模型的计算性能得到了保证，但是却大大牺牲了灵活性，如果对计算图进行修改，则在每个session执行计算之前将会要重新编译计算图，会带了额外的开销。此外，由于使用了静态图的模式，如果使用Python作为语言的前端，需要使用其自带的tfdbg进行调试，过程相对而言比较繁琐。Eager Mode的为TensorFlow带来了如下新的特性：
可以自然控制代码的结构并使用Python内置的数据结构，使得在小型模型或者小数据集上快速迭代成为可能
优化了调试过程，无需使用Session即可查看模型运行结果或者测试修改，支持使用标准的Python调试工具
简化了程序逻辑设计，可以使用Python的控制流而不是图控制流
虽然相较于静态图的模式有很大不同，但是在Eager Mode下，同样可以使用大部分之前TensorFlow的操作以及使用GPU进行加速计算。Eager Mode拥有众多不同的特性，但是起启用方式却非常简单，只需要在执行TensorFlow相关操作之前使用如下语句即可：![](https://img-blog.csdnimg.cn/20181207153555114.png)
需要注意的是这种启用方式是不可逆的，即开启了Eager Mode之后就不能再执行基于静态计算图的操作了。下面举一个简单的例子，假设我们想对128张32x32的图片进行进行卷积以及pooling 计算，在Eager Mode下只需要执行如下语句即可：
![](https://img-blog.csdnimg.cn/20181207153609902.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)在Jupyter Notebook 中，我们可以快速查看执行结果：
![image.png](http://www.sigai.cn/upload/image/20181207/1544167937143279.png)
图 1 Eager 模式下快速计算TensorFlow操作示例
从上述小例子中我们看到Eager Mode下我们可以像使用Numpy数组一样快速获得模型部分结构的执行结果，而无需针对每次计算过程都执行创建session，构建feed_dict等比较繁琐的操作，这为我们验证模型的前向计算过程提供了非常大的便利。另外Eager Mode下的一个比较便利之处在于，Numpy数组以及Python的原生对象会被自动转换为tf.Tensor对象，而Numpy操作也可以直接作用于tf.Tensor对象。
**Eager Mode下的自动求导**
相信PyTorch中的Autograd机制为很多科研工作者在快速实现算法原型的过程中带来了很多帮助，而TensorFlow框架下迟迟无法使用类似的功能。在Eager Mode下自动求导功能得到支持，求导过程中所使用的GradientTape是通过堆栈来实现的。在前向过程中，所有在某个tf.GradientTape 的Context下所做的所有操作都会被记录下来，这个过程是一个不断向一个tape堆栈中push新的tape的过程。在求导过程中，这个过程实际上就是栈顶tape不断被弹出的过程。
![image.png](http://www.sigai.cn/upload/image/20181207/1544168041153910.png)
图2 Eager Mode下自动求导示例
默认情况下，tape的使用是一次性的，如果需要再次使用，需要在首次使用时加上 persisitent=True参数。
![image.png](http://www.sigai.cn/upload/image/20181207/1544167979373019.png)
图3 多次使用tape示例
在前向过程中，同样可以使用Python控制流来定义所执行的操作。图4所示的示例中，f(x,y)函数内部使用了Python内部的for循环以及逻辑判断操作，这些Python的原生操作都是tf.GradientTape可以支持的。
![image.png](http://www.sigai.cn/upload/image/20181207/1544167997330224.png)
图4 使用Python 控制流定义操作
**创建模型并训练**
作为TensorFlow 2.0中的核心特征，Eager Mode 可以完整支持TensorBoard以及保存模型，目前summary相关操作仍需使用tf.contrib.summary实现，预计在TensorFlow 2.0 发布之时将可以直接使用tf.summary操作进行。而模型的保存部分则可以使用与常规模式下相同的tf.train.Saver完成。接下来以MNIST数据集为例来进行模型的创建以及训练。首先我们定义了一个包含了CNN和RNN的模型，其模型定义如下：
由于Eager Mode 下求导过程中需要指定对哪些变量进行求导，所以在搭建Eager Mode求导可用的网络时，建议使用tf.keras.Model作为基类，并重载call函数，这样可以简化前向计算以及求导的过程中所需的操作。此外，TensorFlow 开发团队官方已经声明将会持续将Keras与TensorFlow平台紧密结合起来[3]，我们推荐读者使用基于Keres的高级API构建模型。
接下来的是在数据集上迭代并进行梯度更新的过程:
从以上代码可以看出，在计算出loss之后，首先计算出梯度，然后使用optimizer.apply_gradients函数即可完成某个batch的梯度更新。另外一点值得注意的是，我们在本次试验中使用了上篇文章所讲到的TFRecords文件，在Eager Mode下使用tf.dataset API 加载的文件时，我们可以使用Python内置的emunerate直接遍历整个数据集。
在SIGAI 在线编程下的sharedata/intro_to_tf/mnist_eager.py文件中，我们可以看到完整的代码，同时还有一份与之对应的mnist_low_level.py文件，从两个文件的对比我们可以看出使用高级API可以让我们将精力集中在网络的搭建过程而不是繁杂的variable_scope维护以及常用操作的组合过程。但是我们仍让需要手动完成summary文件的保存过程，checkpoint文件的保存或者恢复过程。值得庆幸的是，通过tf.train.Checkpoint所保存的模型可以在图模式和Eager Mode下交替使用。
如果读者运行了两种不同方式搭建的网络，我们可以发现目前Eager Mode虽然带来了极大的便利，但由于本文使用的网络比较简单，Eager Mode下的执行速度相较于静态图模式要慢不少。而官方文档中也提到在运行计算密集型的网络时，Eager Mode下的执行速度才会跟图模式相差无几。
本文主要讲解了TensorFlow中的Eager Mode相关内容，主要包括了Eager Mode的简介以及其发展过程，然后介绍了Eager Mode 中比较实用的自动求导(autograd)功能，接着本文以MNIST数据集分类为例展示了一个完整的数据加载，模型训练，训练指标保存，checkpoint保存以及加载的过程。
参考文献:
[1] https://groups.google.com/a/tensorflow.org/forum/\#!topic/discuss/bgug1G6a89A
[2] http://ai.googleblog.com/2017/10/eager-execution-imperative-define-by.html
[3] https://medium.com/tensorflow/standardizing-on-keras-guidance-on-high-level-apis-in-tensorflow-2-0-bad2b04c819a


