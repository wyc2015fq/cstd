# Tensorflow：Tensorboard使用 - 去向前方的博客 - CSDN博客





2018年07月14日 14:59:44[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：108








# 目录



- [目录](#目录)
- [前言](#前言)
- [可视化第一步](#可视化第一步)
- [实现graphs](#实现graphs)
- [后记](#后记)




# 前言

TensorFlow是以流动的张量为名的神经网络开发库，所以Google为了让人们更直观的了解流动的张量的含义，他们做了个TensorBoard让我们直观的看到我们写的框架是怎么个流动法的（纯属YY）。好了玩笑话不说，Google是怎么定义TensorBoard的呢？

```
The computations you'll use TensorFlow for - 
like training a massive deep neural network - 
can be complex and confusing. 
To make it easier to understand, debug, 
and optimize TensorFlow programs, 
we've included a suite of visualization tools called TensorBoard. 
You can use TensorBoard to visualize your TensorFlow graph,
 plot quantitative metrics about the execution of your graph,
  and show additional data like images that pass through it.
```

大意其实就是我刚刚说的，我们使用TensorFlow写的神经网络直接看上去会显得十分的复杂和混乱，为了更加直观的去调试、优化我们的神经网络，他们开发了TensorBoard–一套可视化工具，深度学习的可视化对于理解它究竟学到了什么有很大的帮助。

**Tips：TensorBoard官方支持的浏览器是Chrome和Firefox，所以最好使用这两个浏览器去开TensorBoard吧。**

# 可视化第一步

效果图

![这里写图片描述](https://img-blog.csdn.net/20180714144803295?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们可以对其进行展开，比如我们展开一个first_layer

![WX20170113-152056@2x.png](https://img-blog.csdn.net/2018071414491249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看出来一个layer里面是多么复杂的运算过程，其实我们代码里也就几行代码，TensorFlow帮我们完成了很多事情。

看了效果之后有没有非常想实现它？那么接下来我们就来学习下怎么去用TensorBoard去画出这么一幅图~

# 实现graphs

我们这里用的代码是之前讲TensorFlow入门里最后的拟合曲线的代码，代码如下：

```python
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
def add_layer(layoutname,inputs,in_size,out_size,activatuib_funaction=None):
    Weights=tf.Variable(tf.random_normal([in_size,out_size]))
    biases=tf.Variable(tf.zeros([1,out_size]))+0.1
    Wx_plus_b=tf.matmul(inputs,Weights)+biases

    if activatuib_funaction is None:
        outputs=Wx_plus_b
    else :
        outputs=activatuib_funaction(Wx_plus_b)
    return outputs


x_data=np.linspace(-1,1,300)[:,np.newaxis]
noise=np.random.normal(0,0.05,x_data.shape)
y_data=np.square(x_data)-0.5+noise

xs=tf.placeholder(tf.float32,[None,1])
ys=tf.placeholder(tf.float32,[None,1])


l1=add_layer('first_layer',xs,1,10,activatuib_funaction=tf.nn.relu)
prediction =add_layer('secend_layer',l1,10,1,activatuib_funaction=None)

loss=tf.reduce_mean(tf.reduce_sum(tf.square(ys-prediction),reduction_indices=[1]))

train_step=tf.train.GradientDescentOptimizer(0.1).minimize(loss)

init=tf.global_variables_initializer()

with tf.Session() as sess:
    fig=plt.figure()
    ax=fig.add_subplot(1,1,1)
    ax.scatter(x_data,y_data)
    plt.show(block=False)

    sess.run(init)
    for train in range(1000):

        sess.run(train_step,feed_dict={xs:x_data,ys:y_data})
        if train%50==0:

            try:
                ax.lines.remove(lines[0])
            except Exception:
                  pass
            print train,sess.run(loss,feed_dict={xs:x_data,ys:y_data})
            prediction_value=sess.run(prediction,feed_dict={xs:x_data})
            lines=ax.plot(x_data, prediction_value,'r-',lw=5)
            plt.pause(0.1)
```

我们需要对代码进行写改动，让其生成Graphs。

从最开始的缩略图，我们可以看到我们有input、两个layer、一个loss、一个train，那我们第一步先来构建这几个东西。

我们想要构建这些大的框架，只需要我们在想要显示的地方加上这句话with tf.name_scope(‘显示的名字’):然后缩进想要放在里面的代码。如**input**

我们想要显示的效果如下图

![这里写图片描述](https://img-blog.csdn.net/20180714145110225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们需要用with tf.name_scope(‘input’):把两个输入值缩进进去：

```python
with tf.name_scope('inputs'):
    xs=tf.placeholder(tf.float32,[None,1],name="x_input")
    ys=tf.placeholder(tf.float32,[None,1],name="y_input")
```

x_input、y_input这两个显示值我们只需要在声明的时候在最后加一个name=’显示名’就可以显示出上面的效果了。

然后两个layer也是类似，由于我们定义了一个方法去添加层，可是如果我们不给他名字，生成graphs的时候layer就是layer、layer_1、layer_2这样子，有点不好看，所以我们传入一个layername。

```python
def add_layer(layoutname,inputs,in_size,out_size,activatuib_funaction=None,):
    with tf.name_scope(layoutname):
        with tf.name_scope('weights'):
            Weights=tf.Variable(tf.random_normal([in_size,out_size]),name='W')
        with tf.name_scope('biases'):
            biases=tf.Variable(tf.zeros([1,out_size])+0.1,name='b')
        with tf.name_scope('Wx_plus_b'):
            Wx_plus_b=tf.add(tf.matmul(inputs,Weights),biases)

        if activatuib_funaction is None:
            outputs=Wx_plus_b
        else :
            outputs=activatuib_funaction(Wx_plus_b)
        return outputs
```

layer图层里面还有weights和biases、Wx_plus_b等，所以他们也是小图层，所以我们对他们也要单独定义，另外给weights，biases给个name值。 激励函数这些会自动生成，我们不需要去管。

loss和train也是类似，写完tf.name_scope(”):之后，我们还需要写文件，写文件的代码TensorFlow也帮我们封装好了，我们只需要调用writer=tf.summary.FileWriter(“文件保存路径如：logs/”,sess.graph)，完整代码：

```python
import tensorflow as tf
import numpy as np
def add_layer(layoutname,inputs,in_size,out_size,activatuib_funaction=None,):
    with tf.name_scope(layoutname):
        with tf.name_scope('weights'):
            Weights=tf.Variable(tf.random_normal([in_size,out_size]),name='W')
        with tf.name_scope('biases'):
            biases=tf.Variable(tf.zeros([1,out_size])+0.1,name='b')
        with tf.name_scope('Wx_plus_b'):
            Wx_plus_b=tf.add(tf.matmul(inputs,Weights),biases)

        if activatuib_funaction is None:
            outputs=Wx_plus_b
        else :
            outputs=activatuib_funaction(Wx_plus_b)
        return outputs
x_data=np.linspace(-1,1,300)[:,np.newaxis]
noise=np.random.normal(0,0.09,x_data.shape)
y_data=np.square(x_data)-0.05+noise
with tf.name_scope('inputs'):
    xs=tf.placeholder(tf.float32,[None,1],name="x_in")
    ys=tf.placeholder(tf.float32,[None,1],name="y_in")
l1=add_layer("first_layer",xs,1,10,activatuib_funaction=tf.nn.relu)
prediction =add_layer('second_layout',l1,10,1,activatuib_funaction=None)
with tf.name_scope('loss'):
    loss=tf.reduce_mean(tf.reduce_sum(tf.square(ys-prediction),reduction_indices=[1]))
with tf.name_scope('train'):
    train_step=tf.train.GradientDescentOptimizer(0.1).minimize(loss)

init=tf.global_variables_initializer()

with tf.Session() as sess:
    writer=tf.summary.FileWriter("logs/",sess.graph)
    sess.run(init)
```

由于Graphs展示的是整个神经网络的框架，所以我们可以删去测试数据。

到这里代码已经写好了，运行代码，生成文件，我们会发现项目的logs目录下会多一个文件

![这里写图片描述](https://img-blog.csdn.net/2018071414563583?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

生成文件之后，首先是进入cmd，输入下面的指令来启动TensorBoard

python tensorflow/tensorboard/tensorboard.py –logdir=path/to/log-directory

这里的参数 logdir 指向 SummaryWriter 序列化数据的存储路径。如果logdir目录的子目录中包含另一次运行时的数据，那么 TensorBoard 会展示所有运行的数据。一旦 TensorBoard 开始运行，你可以通过在浏览器中输入 localhost:6006 来查看 TensorBoard。

如果你已经通过pip安装了 TensorBoard，你可以通过执行更为简单地命令来访问 TensorBoard

tensorboard –logdir=/path/to/log-directory

进入 TensorBoard 的界面时，你会在右上角看到导航选项卡，每一个选项卡将展现一组可视化的序列化数据集 。对于你查看的每一个选项卡，如果 TensorBoard 中没有数据与这个选项卡相关的话，则会显示一条提示信息指示你如何序列化相关数据。 

注意命令：tensorboard **–logdir=E:/tensorFlow/Mycode/logs/**

然后我们就可以访问127.0.0.1:6006去查看TensorBoard，如果没有数据的话就刷新一下。到此，我们就成功的使用可视化的方式查看我们的神经网络框架了。
# 后记

TensorFlow配合TensorBoard的Graphs来构建我们的框架确实非常好用，牛逼的Google肯定不会只赋予TensorBoard这一个功能，他能监控我们的训练过程，让我们可视化的看到训练过程中参数的变化，甚至是图像、音频的变化，还能展示训练过程的数据分布图，所以tensorboard、TensorFlow博大精深，慢慢学习吧！ 

PS:感谢周莫烦大神出的[[机器学习系列]](https://www.youtube.com/channel/UCdyjiB5H8Pu7aDTNVXTTpcg)

需要翻墙观看




