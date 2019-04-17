# tensorflow+入门笔记︱基本张量tensor理解与tensorflow运行结构 - 素质云笔记/Recorder... - CSDN博客





2017年01月22日 11:57:34[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：12558








> 
Gokula Krishnan Santhanam认为，大部分深度学习框架都包含以下五个核心组件：


> - 张量（Tensor）
- 基于张量的各种操作
- 计算图（Computation Graph）
- 自动微分（Automatic Differentiation）工具
- BLAS、cuBLAS、cuDNN等拓展包


.

.

# 一、张量的理解

> 
本节主要参考自文章[《开发丨深度学习框架太抽象？其实不外乎这五大核心组件》](http://mp.weixin.qq.com/s?__biz=MzI5NTIxNTg0OA==&mid=2247485211&idx=2&sn=0520c5b0dc54096eaae271a8864baf61&chksm=ec57b09cdb20398a2b2bd5ed670be6e93bf9ea9d4aaa659fbf3ec9b13f95ba1b30363c89626e&mpshare=1&scene=1&srcid=0121bM8xtsO6akyb3tfzZZaV#rd)


## .1、张量的解读

**张量是所有深度学习框架中最核心的组件，因为后续的所有运算和优化算法都是基于张量进行的。**几何代数中定义的张量是基于向量和矩阵的推广，通俗一点理解的话，我们可以将标量视为零阶张量，矢量视为一阶张量，那么矩阵就是二阶张量。

举例来说，我们可以将任意一张RGB彩色图片表示成一个三阶张量（三个维度分别是图片的高度、宽度和色彩数据）。如下图所示是一张普通的水果图片，按照RGB三原色表示，其可以拆分为三张红色、绿色和蓝色的灰度图片，如果将这种表示方法用张量的形式写出来，就是图中最下方的那张表格。

![这里写图片描述](https://img-blog.csdn.net/20170122111722207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170122111728907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图中只显示了前5行、320列的数据，每个方格代表一个像素点，其中的数据[1.0, 1.0, 1.0]即为颜色。假设用[1.0, 0, 0]表示红色，[0, 1.0, 0]表示绿色，[0, 0, 1.0]表示蓝色，那么如图所示，前面5行的数据则全是白色。**

将这一定义进行扩展，我们也可以用四阶张量表示一个包含多张图片的数据集，其中的四个维度分别是：图片在数据集中的编号，图片高度、宽度，以及色彩数据。

**为什么需要使用张量来进行图片处理？？**

当数据处理完成后，我们还可以方便地将张量再转换回想要的格式。例如Python NumPy包中numpy.imread和numpy.imsave两个方法，分别用来将图片转换成张量对象（即代码中的Tensor对象），和将张量再转换成图片保存起来。

.

## 2、张量的各种操作

有了张量对象之后，下面一步就是一系列针对这一对象的数学运算和处理过程。

![这里写图片描述](https://img-blog.csdn.net/20170122111936424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其实，所谓的“学习”就是不断纠正神经网络的实际输出结果和预期结果之间误差的过程。这里的一系列操作包含的范围很宽，可以是简单的矩阵乘法，也可以是卷积、池化和LSTM等稍复杂的运算。

.

## 3、计算图（Computation Graph）

有了张量和基于张量的各种操作之后，下一步就是将各种操作整合起来，输出我们需要的结果。

但不幸的是，随着操作种类和数量的增多，有可能引发各种意想不到的问题，包括多个操作之间应该并行还是顺次执行，如何协同各种不同的底层设备，以及如何避免各种类型的冗余操作等等。这些问题有可能拉低整个深度学习网络的运行效率或者引入不必要的Bug，而计算图正是为解决这一问题产生的。

据AI科技评论了解，**计算图首次被引入人工智能领域是在2009年的论文《Learning Deep Architectures for AI》。**当时的图片如下所示，作者用不同的占位符（*，+，sin）构成操作结点，以字母x、a、b构成变量结点，再以有向线段将这些结点连接起来，组成一个表征运算逻辑关系的清晰明了的“图”型数据结构，这就是最初的计算图。

![这里写图片描述](https://img-blog.csdn.net/20170122112102487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

后来随着技术的不断演进，加上脚本语言和低级语言各自不同的特点（概括地说，脚本语言建模方便但执行缓慢，低级语言则正好相反），因此业界逐渐形成了这样的一种开发框架：前端用Python等脚本语言建模，后端用C++等低级语言执行（这里低级是就应用层而言），以此综合了两者的优点。可以看到，这种**开发框架大大降低了传统框架做跨设备计算时的代码耦合度，也避免了每次后端变动都需要修改前端的维护开销。**而这里，在前端和后端之间起到关键耦合作用的就是计算图。

需要注意的是，通常情况下开发者不会将用于中间表示得到的计算图直接用于模型构造，因为这样的计算图通常包含了大量的冗余求解目标，也没有提取共享变量，因而通常都会经过依赖性剪枝、符号融合、内存共享等方法对计算图进行优化。

目前，各个框架对于计算图的实现机制和侧重点各不相同。例如Theano和MXNet都是以隐式处理的方式在编译中由表达式向计算图过渡。而Caffe则比较直接，可以创建一个Graph对象，然后以类似Graph.Operator(xxx)的方式显示调用。

**因为计算图的引入，开发者得以从宏观上俯瞰整个神经网络的内部结构，就好像编译器可以从整个代码的角度决定如何分配寄存器那样，计算图也可以从宏观上决定代码运行时的GPU内存分配，以及分布式环境中不同底层设备间的相互协作方式。**

.

## 4、自动微分（Automatic Differentiation）工具

计算图带来的另一个好处是让模型训练阶段的梯度计算变得模块化且更为便捷，也就是自动微分法。

将待处理数据转换为张量，针对张量施加各种需要的操作，通过自动微分对模型展开训练，然后得到输出结果开始测试。那么如何微分中提高效率呢？

**第一种方法：模拟传统的编译器**

每一种张量操作的实现代码都会预先加入C语言的转换部分，然后由编译器在编译阶段将这些由C语言实现的张量操作综合在一起。目前pyCUDA和Cython等编译器都已经实现了这一功能。

**第二种方法：利用脚本语言实现前端建模**

用低级语言如C++实现后端运行，这意味着高级语言和低级语言之间的交互都发生在框架内部，因此每次的后端变动都不需要修改前端，也不需要完整编译（只需要通过修改编译参数进行部分编译），因此整体速度也就更快。

**第三种方法：现成的扩展包**

例如最初用Fortran实现的BLAS（基础线性代数子程序），就是一个非常优秀的基本矩阵（张量）运算库，此外还有英特尔的MKL（Math Kernel Library）等，开发者可以根据个人喜好灵活选择。

一般的BLAS库只是针对普通的CPU场景进行了优化，但目前大部分的深度学习模型都已经开始采用并行GPU的运算模式，因此利用诸如NVIDIA推出的针对GPU优化的cuBLAS和cuDNN等更据针对性的库可能是更好的选择。
.

.

# 二、tensorflow运行结构

> 
本节内容为小象学院深度学习二期，课程笔记，由寒小阳老师授课，感谢寒小阳老师，讲得深入浅出，适合我这样的菜鸟~


![这里写图片描述](https://img-blog.csdn.net/20170122115430956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

## 1、tensorflow框架整体结构

用张量tensor表示数据；计算图graph表示任务；在会话session中执行context；

通过变量维护状态；通过feed和fetch可以任意的操作（arbitrary operation）、赋值、获取数据

.

## 2、Numpy和tensorflow中的张量对比

![这里写图片描述](https://img-blog.csdn.net/20170122113707390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意：如果tensorflow要输出张量不跟numpy中的array一样，要借助eval()

```
print(tensor.eval())
```

.

## 3、tensorflow中的计算图

![这里写图片描述](https://img-blog.csdn.net/20170122114720841?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
#创建节点
import tensorflow as tf
matrix1=tr.constant([[3.,3.]])    #常量节点，1*2
matrix2=tr.constant([[2.],[2.]])  #常量节点，2*1
product=tf.matmul(matrix1,matrix2) #矩阵乘法节点，两常量相乘

#执行
sess=tf.Session()         #创建session
result=sess.run(product)  #run进行执行
print result              #返回一个numpy中的ndarray对象
sess.close()              #关闭对话，不然占用资源
```

.

## 4、用变量来保存参数w

```
W2=tf.Variable(tf.zeros((2,2)).name="weights")
sess.run(tf.initialize_all_variables())
print(sess.run(W2))
```

注意需要tf.initialize_all_variables对变量进行初始化才能赋值的。

.

## 5、如何指定、调用GPU/CPU

用with…device语句来指派。

![这里写图片描述](https://img-blog.csdn.net/20170122115329708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

## 6、计算模型computation graph 与层layer模型

计算模型

首先构造好整个计算链路，然后进行计算。同时可以对链路进行优化+分布式。
![这里写图片描述](https://img-blog.csdn.net/20170122113311010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

总得来看，链路结构较为复杂，但是比较好计算高效率运算。
layer模型

每个层固定实现前向与后向，同时必须手动指定目标GPU

.

## 7、报错修复

经常出现：`Variable xxx already exists, disallowed. Did you mean to set reuse=True or reuse`的报错，解决：

```
tf.reset_default_graph()
```

## 延伸一：tensorflow镜像在utundu安装

只要你的服务器上Ubuntu，CUDA+cuDNN安装好了之后，就可以直接一步到位，快到飞起。

一步到位，配好
[https://mirrors.tuna.tsinghua.edu.cn/help/tensorflow/](https://mirrors.tuna.tsinghua.edu.cn/help/tensorflow/)
![这里写图片描述](https://img-blog.csdn.net/20170304153742323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
## 延伸二：TensorFlow 的黑科技摘录

> 
摘录自知乎专栏：[TensorFlow-dev-summit：那些TensorFlow上好玩的和黑科技](https://zhuanlan.zhihu.com/p/25592272?group_id=822216196656091136)


1、TensorFlow 1.0在分布式训练inception-v3模型上，64张GPU可以达到58X的加速比，更灵活的高层抽象接口，以及更稳定的API。

2、XLA: An Experimental TensorFlow Compiler，快速地将TensorFlow转成比较底层的实现（依赖device），这里后面有个talk，详细讲述了XLA。

3、有趣的应用案例，皮肤癌图像分类，pretrained的inception-v3对数据来做finetuning，最后在手机上做inference

4、Keras与TensorFlow的集成。

Keras的用户可以更快的在TensorFlow的框架下做出相应地模型，能更方便地进行分布式训练，使用Google的Cloud ML， 进行超参，还有更更重要的：TF-Serving

5、分布式TensorFlow

1.0版本上有了很大的提升可以做到64块GPU上达到58倍的加速，这里先基本介绍下数据并行和模型并行

.
## 延伸三：[TensorFlow精选Github开源项目](https://mp.weixin.qq.com/s?__biz=MzIzNDQyNjI5Mg==&mid=2247484468&idx=1&sn=ca9ab40942c302ab4664373c04948cfd&chksm=e8f7deefdf8057f9836160f1c9d7f412176ab6854ee717d24c567a117073d02bab169bbfacf2&mpshare=1&scene=1&srcid=04019a1SYOdsHIaQnrZo9hLE#rd)
- TensorFlow源码

[https://github.com/tensorflow/tensorflow](https://github.com/tensorflow/tensorflow)
- 基于TensorFlow的框架

[https://github.com/fchollet/keras](https://github.com/fchollet/keras)
[https://github.com/tflearn/tflearn](https://github.com/tflearn/tflearn)
[https://github.com/beniz/deepdetect](https://github.com/beniz/deepdetect)
[https://github.com/tensorflow/fold](https://github.com/tensorflow/fold)
[https://github.com/leriomaggio/deep-learning-keras-tensorflow](https://github.com/leriomaggio/deep-learning-keras-tensorflow)- 精选入门教程

[https://github.com/tensorflow/models](https://github.com/tensorflow/models)
[https://github.com/aymericdamien/TensorFlow-Examples](https://github.com/aymericdamien/TensorFlow-Examples)
[https://github.com/donnemartin/data-science-ipython-notebooks](https://github.com/donnemartin/data-science-ipython-notebooks)
[https://github.com/jtoy/awesome-tensorflow](https://github.com/jtoy/awesome-tensorflow)
[https://github.com/jikexueyuanwiki/tensorflow-zh](https://github.com/jikexueyuanwiki/tensorflow-zh)
[https://github.com/nlintz/TensorFlow-Tutorials](https://github.com/nlintz/TensorFlow-Tutorials)
[https://github.com/pkmital/tensorflow_tutorials](https://github.com/pkmital/tensorflow_tutorials)
[https://github.com/deepmind/learning-to-learn](https://github.com/deepmind/learning-to-learn)
[https://github.com/BinRoot/TensorFlow-Book](https://github.com/BinRoot/TensorFlow-Book)
[https://github.com/jostmey/NakedTensor](https://github.com/jostmey/NakedTensor)
[https://github.com/alrojo/tensorflow-tutorial](https://github.com/alrojo/tensorflow-tutorial)
[https://github.com/CreatCodeBuild/TensorFlow-and-DeepLearning-Tutorial](https://github.com/CreatCodeBuild/TensorFlow-and-DeepLearning-Tutorial)
[https://github.com/sjchoi86/Tensorflow-101](https://github.com/sjchoi86/Tensorflow-101)
[https://github.com/chiphuyen/tf-stanford-tutorials](https://github.com/chiphuyen/tf-stanford-tutorials)
[https://github.com/google/prettytensor](https://github.com/google/prettytensor)
[https://github.com/ahangchen/GDLnotes](https://github.com/ahangchen/GDLnotes)
[https://github.com/Hvass-Labs/TensorFlow-Tutorials](https://github.com/Hvass-Labs/TensorFlow-Tutorials)
[https://github.com/NickShahML/tensorflow_with_latest_papers](https://github.com/NickShahML/tensorflow_with_latest_papers)
[https://github.com/nfmcclure/tensorflow_cookbook](https://github.com/nfmcclure/tensorflow_cookbook)
[https://github.com/ppwwyyxx/tensorpack](https://github.com/ppwwyyxx/tensorpack)
[https://github.com/rasbt/deep-learning-book](https://github.com/rasbt/deep-learning-book)
[https://github.com/pkmital/CADL](https://github.com/pkmital/CADL)
[https://github.com/tensorflow/skflow](https://github.com/tensorflow/skflow)- 无人驾驶

[https://github.com/kevinhughes27/TensorKart](https://github.com/kevinhughes27/TensorKart)
[https://github.com/SullyChen/Autopilot-TensorFlow](https://github.com/SullyChen/Autopilot-TensorFlow)
- 深度强化学习

[https://github.com/dennybritz/reinforcement-learning](https://github.com/dennybritz/reinforcement-learning)
[https://github.com/zsdonghao/tensorlayer](https://github.com/zsdonghao/tensorlayer)
[https://github.com/matthiasplappert/keras-rl](https://github.com/matthiasplappert/keras-rl)
[https://github.com/nivwusquorum/tensorflow-deepq](https://github.com/nivwusquorum/tensorflow-deepq)
[https://github.com/devsisters/DQN-tensorflow](https://github.com/devsisters/DQN-tensorflow)
[https://github.com/coreylynch/async-rl](https://github.com/coreylynch/async-rl)
[https://github.com/carpedm20/deep-rl-tensorflow](https://github.com/carpedm20/deep-rl-tensorflow)
[https://github.com/yandexdataschool/Practical_RL](https://github.com/yandexdataschool/Practical_RL)- 自然语言处理

文本分类
[https://github.com/dennybritz/cnn-text-classification-tf](https://github.com/dennybritz/cnn-text-classification-tf)

序列建模
[https://github.com/google/seq2seq](https://github.com/google/seq2seq)

中文分词
[https://github.com/koth/kcws](https://github.com/koth/kcws)

基于文本的图像合成
[https://github.com/paarthneekhara/text-to-image](https://github.com/paarthneekhara/text-to-image)

RNN语言建模
[https://github.com/sherjilozair/char-rnn-tensorflow](https://github.com/sherjilozair/char-rnn-tensorflow)
[https://github.com/silicon-valley-data-science/RNN-Tutorial](https://github.com/silicon-valley-data-science/RNN-Tutorial)

神经图灵机
[https://github.com/carpedm20/NTM-tensorflow](https://github.com/carpedm20/NTM-tensorflow)- 语音领域

语音合成
[https://github.com/ibab/tensorflow-wavenet](https://github.com/ibab/tensorflow-wavenet)
[](https://github.com/tomlepaine/fast-wavenet)https://github.com/tomlepaine/fast-wavenet

语音识别
[](https://github.com/buriburisuri/speech-to-text-wavenet)https://github.com/buriburisuri/speech-to-text-wavenet
[](https://github.com/pannous/tensorflow-speech-recognition)https://github.com/pannous/tensorflow-speech-recognition- 
计算机视觉

风格转换
[](https://github.com/anishathalye/neural-style)https://github.com/anishathalye/neural-style
[](https://github.com/cysmith/neural-style-tf)https://github.com/cysmith/neural-style-tf

运用GAN图像生成
[](https://github.com/carpedm20/DCGAN-tensorflow)https://github.com/carpedm20/DCGAN-tensorflow

图像到图像的翻译
[](https://github.com/affinelayer/pix2pix-tensorflow)https://github.com/affinelayer/pix2pix-tensorflow

图像超分辨
[](https://github.com/Tetrachrome/subpixel)https://github.com/Tetrachrome/subpixel

人脸识别
[](https://github.com/davidsandberg/facenet)https://github.com/davidsandberg/facenet

目标检测
[](https://github.com/TensorBox/TensorBox)https://github.com/TensorBox/TensorBox

运动识别
[](https://github.com/guillaume-chevalier/LSTM-Human-Activity-Recognition)https://github.com/guillaume-chevalier/LSTM-Human-Activity-Recognition

图像复原
[](https://github.com/bamos/dcgan-completion.tensorflow)https://github.com/bamos/dcgan-completion.tensorflow

生成模型
[](https://github.com/wiseodd/generative-models)https://github.com/wiseodd/generative-models- 
TensorFlow实时debug工具

[](https://github.com/ericjang/tdb)https://github.com/ericjang/tdb- 
TensorFlow在树莓派上的应用

[](https://github.com/samjabrahams/tensorflow-on-raspberry-pi)https://github.com/samjabrahams/tensorflow-on-raspberry-pi- 
TensorFlow基于R的应用

[](https://github.com/rstudio/tensorflow)https://github.com/rstudio/tensorflow- 
实时Spark与TensorFlow的输入pipeline

[](https://github.com/fluxcapacitor/pipeline)https://github.com/fluxcapacitor/pipeline
[](https://github.com/yahoo/TensorFlowOnSpark)https://github.com/yahoo/TensorFlowOnSpark- 
caffe与TensorFlow结合

[](https://github.com/ethereon/caffe-tensorflow)https://github.com/ethereon/caffe-tensorflow- 
概率建模

[](https://github.com/blei-lab/edward)https://github.com/blei-lab/edward











