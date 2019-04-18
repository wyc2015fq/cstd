# Tensorflow 入门基础 - wsp_1138886114的博客 - CSDN博客





2018年06月14日 09:52:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：145标签：[Tensorflow基础](https://so.csdn.net/so/search/s.do?q=Tensorflow基础&t=blog)
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)









### 一、TensorFlow简介与安装

> 
```
TensorFlow是谷歌基于DistBelief进行研发的第二代人工智能学习系统，采用数据流图（data flow graphs），
    用于数值计算的开源软件库。节点（Nodes）在图中表示数学操作，图中的线（edges）则表示在节点间相互
    张量从流图的一端流动到另一端计算过程。TensorFlow不只局限于神经网络，其数据流式图支持非常自由的
    算法表达， 就可以使用TensorFlow。
TensorFlow可被用于语音识别或图像识别等多项机器深度学习领域，TensorFlow一大亮点是组成的分布式系统。
```

##### 1.1TensorFlow的特点

```
● 高度的灵活性
    TensorFlow 采用数据流图，用于数值计算的开源软件库。只要计算能表示为一个数据流图，
    你就可以使用Tensorflow。
● 真正的可移植性
    Tensorflow 在CPU和GPU上运行，可以运行在台式机、服务器、云服务器、手机移动   设备、
    Docker容器里等等。
● 将科研和产品联系在一起
    过去如果要将科研中的机器学习想法用到产品中，需要大量的代码重写工作。Tensorflow
    将改变这一点。使用Tensorflow可以让应用型研究者将想法迅速运用到产品中，也可以
    让学术性研究者更直接地彼此分享代码，产品团队则用Tensorflow来训练和使用计算模型，
    并直接提供给在线用户，从而提高科研产出率。
● 自动求微分
    基于梯度的机器学习算法会受益于Tensorflow自动求微分的能力。使用Tensorflow，
    只需要定义预测模型的结构，将这个结构和目标函数（objective function）结合在一起，
    并添加数据，Tensorflow将自动为你计算相关的微分导数。
● 多语言支持
    Tensorflow 有一个合理的c++使用界面，也有一个易用的python使用界面来构建和执行
    你的graphs。你可以直接写python/c++程序，也可以用交互式的Ipython界面来用Tensorflow
    尝试这些想法，也可以使用Go，Java，Lua，Javascript，或者是R等语言。
● 性能最优化
    如果你有一个32个CPU内核、4个GPU显卡的工作站，想要将你工作站的计算潜能全发挥出来，
    由于Tensorflow 给予了线程、队列、异步操作等以最佳的支持，Tensorflow让你可以将你手边
    硬件的计算潜能全部发挥出来。你可以自由地将Tensorflow图中的计 算元素分配到不同设备上，
    充分利用这些资源。
```

##### 1.2 TensorFlow的安装

```
使用Anaconda集成环境安装
    执行：conda  install  tensorflow
验证安装是否成功，可以通过导入tensorflow来检验。
    启动 python 执行：import tensorflow as tf
```

##### 1.3TensorFlow的一些主要技术特征：
|编辑模型|Dataflow-like model（数据流模型）|
|----|----|
|语言|Python、C++、Go、Rust、Haskell、Java（还有非官方的JavaScript、R等）|
|部署|Code once、run everywhere（一次编写，各处运行）|
|计算资源|CPU(Linux、Mac、Windows、Android，iOS)    GPU(Linux、Mac、Windows)    TPU(Tensor Processing Unit)|
|实现方式|Local Implementation（单机实现）    Distributed Implementation（分布式实现）|
|平台支持|Google Cloud Platform（谷歌云平台）    Hadoop File System(Hadoop 分布式文件系统)|
|数学表达|Math Graph Expression（数学计算图表达）    Auto Differentiation（自动微分）|


### 二、TensorFlow总体介绍

> 
使用 TensorFlow, 你必须明白 TensorFlow:

```
◆ 使用图 (graph) 来表示计算任务。
◆ 在被称之为 会话 (Session) 的上下文 (context) 中执行图。
◆ 使用 tensor 表示数据。
◆ 通过 变量 (Variable) 维护状态。
◆ 使用 feed 和 fetch 可以为任意的操作(arbitrary operation)赋值或者从其中获取数据。
```

一个 TensorFlow 图描述了计算的过程。为了进行计算, 图必须在会话里被启动. 会话将图的 op 分发到诸如 CPU 或 GPU 之类的设备 上, 同时提供执行 op 的方法. 这些方法执行后, 将产生的 tensor 返回。在 Python 语言中, 返回的 tensor 是 numpy ndarray 对象; 在 C 和 C++ 语言中, 返回的 tensor 是tensorflow::Tensor 实例。

##### 编程模式

```
命令式编程（imperative style programs）
符号式编程（symbolic style programs）。
命令式编程容易理解和调试，命令语句基本没有优化，按原有逻辑执行。符号式编程涉及较多的嵌入和优化，
不容易理解和调试，但运行速度有同比提升。
Torch是典型的命令式风格
caffe、theano、mxnet和Tensorflow都使用了符号式编程。
其中caffe、mxnet采用了两种编程模式混合的方法，
而Tensorflow是完全采用了符号式编程，Theano和Tensorflow的编程模式更相近。
命令式编程是常见的编程模式，编程语言如python/C++都采用命令式编程。命令式编程明确输入变量，
并根据程序逻辑逐步运算，这种模式非常在调试程序时进行单步跟踪，分析中间变量。
```


### 三、 实例详解

> 
```
python 命令式编程                       Tensorflow符号式编程
    a=3;b=5
    c=a*b
    print(c)
    d=a+b
    e=c+d
    print(e)
```

##### 3.1利用constant表示输入

```
import tensorflow as tf

a=tf.constant(5,name="input_a")
b=tf.constant(3,name="input_b")
c=tf.multiply(a,b,name="mul_c")
d=tf.add(a,b,name="add_d")
e=tf.add(c,d,name="add_e")
sess=tf.Session()
 #with tf.Session() as sess:
output=sess.run(e)   ##输出结果为23
print("输出结果:", output)

writer=tf.summary.FileWriter('home/feigu/tmp',sess.graph)
writer.close()
sess.close()
```

##### 3.2 使用variable表示输入

```
import tensorflow as tf

a=tf.Variable(5,name="input_a")
b=tf.Variable(3,name="input_b")
c=tf.multiply(a,b,name="mul_c")
d=tf.add(a,b,name="add_d")
e=tf.add(c,d,name="add_e")
init=tf.global_variables_initializer()
sess=tf.Session()
     #with tf.Session() as sess:
     #init variable 
sess.run(init)
output=sess.run(e)            #输出结果为23
print("输出结果:", output)

writer=tf.summary.FileWriter('home/feigu/tmp',sess.graph)
writer.close()
sess.close()
```

##### 3.3 输入用向量表示

```
import tensorflow as tf

a=tf.Variable([3,5],name="input_a")
     #b=tf.Variable(3,name="input_b")
c=tf.reduce_prod(a,name="mul_c")
d=tf.reduce_sum(a,name="add_d")
e=tf.reduce_sum([c,d],name="add_e")
init=tf.global_variables_initializer()
sess=tf.Session()
     #with tf.Session() as sess:
     #init variable 
sess.run(init)
output=sess.run(e)             #输出结果为23
print("输出结果:", output)

writer=tf.summary.FileWriter('home/feigu/tmp',sess.graph)
writer.close()
sess.close()
```

##### 3.4 用placeholder表示输入

```
import tensorflow as tf

a=tf.placeholder(tf.int8,shape=[None],name="input_a")
     #b=tf.Variable(3,name="input_b")
c=tf.reduce_prod(a,name="mul_c")
d=tf.reduce_sum(a,name="add_d")
e=tf.reduce_sum([c,d],name="add_e")
init=tf.global_variables_initializer()
sess=tf.Session()
     #with tf.Session() as sess:
     #init variable 
     #sess.run(init)
output=sess.run(e,feed_dict={a:[3,5]})   #输出结果为23
print("输出结果:", output)

writer=tf.summary.FileWriter('home/feigu/tmp',sess.graph)
writer.close()
sess.close()
```


### 四、可视化实例

> 
```
import tensorflow as tf

a=tf.constant(5,name="input_a")
b=tf.constant(3,name="input_b")
c=tf.multiply(a,b,name="mul_c")
d=tf.add(a,b,name="add_d")
e=tf.add(c,d,name="add_e")
sess=tf.Session()
    #with tf.Session() as sess:
output=sess.run(e)                #输出结果为23
print("运算结果":output)
writer=tf.summary.FileWriter('home/feigu/tmp',sess.graph)
```

```
然后，转到linux客户端，启动sensorboard 
tensorboard --logdir="home/feigu/tmp"
然后在Web浏览器输入提示的地址(http://IP:6006)，即可看到如下图形：
```




