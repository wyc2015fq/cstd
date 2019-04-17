# 基于TensorFlow的深度学习实战 - 知乎
# 



TensorFlow是Google的开源深度学习库，在图形分类、音频处理、推荐系统和自然语言处理等场景下都有丰富的应用。毫不夸张得说，TensorFlow的流行让深度学习门槛变得越来越低，只要你有Python和机器学习基础，入门和使用神经网络模型变得非常简单。

## TensorFlow简介

如前所述，TensorFlow是一个深度学习库，使用这一框架，可以用来构建和测试深度神经网络。

深度学习让我们能够以极高的准确性构建复杂的应用程序。图像、视频、文本、音频等领域的问题，都可以通过深度学习解决。TensorFlow可以用于实现前述所有应用。

TensorFlow很快成为GitHub上用户最多的深度学习框架。这个库之所以如此流行，是因为开发人员可以轻松的用其来搭建、测试和部署机器学习应用。
![](https://pic4.zhimg.com/v2-2f34f4f9ddcc844f16ec479edf281f87_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='279' height='511'></svg>)
上面是一个TensorFlow的示意图。我们不用关心这张图具体代表什么，但你需要知道，其中的椭圆和正方形代表节点，节点就是一些数学函数，你可以将节点分组形成各种数学计算，并得到输出。

箭头代表流动到各个节点的数据。所以TensorFlow也被成为数据流库。

## TensorFlow从入门到应用

安装TensorFlow

一般来讲，使用Python工作时最好用virtualenv虚拟环境。Virtualenv可以在一台机器不同的项目间保持Python依赖隔离。使用virtualenv安装TensorFlow不会覆盖已有的Python版本，这样做也能使排查安装问题变得更容易。


首先安装必备软件：


```
# Ubuntu/Linux 64-bit
$ sudo apt-get install python-pip python-dev python-virtualenv 

# Mac OS X 
$ sudo easy_install pip
$ sudo pip install --upgrade virtualenv
```


建立virtualenv环境. 为了将环境建在 ~/tensorflow 目录下, 执行:


```
$ virtualenv --system-site-packages ~/tensorflow
```


接下来激活virtualenv:


```
$ source ~/tensorflow/bin/activate #  with bash 
$ source ~/tensorflow/bin/activate.csh #  with csh
(tensorflow)$
```


Once the virtualenv is activated, you can use pip to install TensorFlow inside it:
激活virtualenv后即可使用pip在virtualenv内安装TensorFlow：


```
# Ubuntu/Linux 64-bit, CPU only:
(tensorflow)$ sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.7.1-cp27-none-linux_x86_64.whl 

# Mac OS X, CPU only:
(tensorflow)$ sudo easy_install --upgrade six
(tensorflow)$ sudo pip install --upgrade https://storage.googleapis.com/tensorflow/mac/tensorflow-0.7.1-cp27-none-any.whl
```


你可访问官方文档来确认所安装的版本。

如果你要在GPU上跑你的代码，你需要访问官方文档来看看是否满足指定的要求。运行Tensorflow GPU需要安装额外的软件。

当你使用完后可执行如下命令关闭虚拟环境：


```
(tensorflow)$ deactivate
```


其它安装TensorFlow的方法请访问官方网站获取信息。

## TensorFlow的 “Hello World”

你可以使用任何文本编辑器编写python代码，然后保存为扩展名“.py”的文件(eg test.py)。用python命令行即可执行test.py。

为了快速的熟悉TensorFlow编程，下面从一段简单的代码开始：


```
import tensorflow as tf

 a = tf.placeholder("float")
 b = tf.placeholder("float")

 y = tf.mul(a, b)

 sess = tf.Session()
 print sess.run(y, feed_dict={a: 3, b: 3})
```


在上面的代码中，导入Python模块*tensorflow*。然后定义符号变量，也称为占位符。在后面程序执行中会操作这些变量。我们把这些变量作为参数，TensorFlow的乘法函数*tf.mul*会调用。数学函数*tf.mul*会操作tensor，这时的动态大小、多维数组。

TensorFlow的算术操作如下：


```
tf.add，tf.sub，tf.mul，tf.div，tf.mod，tf.abs，tf.neg，tf.sign，tf.inv，tf.square，tf.round，tf.sqrt，tf.pow，tf.exp，tf.log，tf.maximum，tf.minimum，tf.cos，tf.sin
```


TensorFlow也为程序员提供一些函数来进行数学操作，列表如下：

操作

描述

tf.diag

给定对角线上的值，返回对角tensor

tf.transpose

转置

tf.matmul

tensor乘法，即矩阵乘法

tf.matrix_determinant

方阵的行列式

tf.matrix_inverse

方阵的逆矩阵

接下来，创建一个会话。事实上，直到这步还没有执行TensorFlow代码。程序通过Session()创建一个会话与Tensorflow库交互；直到调用run()方法才会创建会话，并运行指定的代码。在本例中，run()方法调用变量值和feed_dict参数，表达式运行完成退出会显示结果9 。

本例比较简单，仅仅为了展示TensorFlow完整的过程。然而，我们更感兴趣的是灵活的结构化代码，插入操作来构建计算图。比如，类似于Python编程中的IPython。为了达到这个目的，TensorFlow提供了tf.InteractiveSession()类。

上面的计算图描述的是数学计算。节点（node）代表数学操作，但是它们也可以代表数据项的点，输出结果或者读写持久化的变量。边（edge）描述的是输入和输出的节点之间的关系。

TensorFlow 将图形定义转换成分布式执行的操作, 以充分利用可用的计算资源(如 CPU 或 GPU). 一般你不需要显式指定使用 CPU 还是 GPU, TensorFlow 能自动检测. 如果检测到 GPU, TensorFlow 会尽可能地利用找到的第一个 GPU 来执行操作.

并行计算能让代价大的算法计算加速执行，TensorFlow也在实现上对复杂操作进行了有效的改进。大部分核相关的操作都是设备相关的实现，比如GPU。下面是一些重要的操作：

操作分组

操作

Maths

Add, Sub, Mul, Div, Exp, Log, Greater, Less, Equal

Array

Concat, Slice, Split, Constant, Rank, Shape, Shuffle

Matrix

MatMul, MatrixInverse, MatrixDeterminant

Neuronal Network

SoftMax, Sigmoid, ReLU, Convolution2D, MaxPool

Checkpointing

Save, Restore

Queues and syncronizations

Enqueue, Dequeue, MutexAcquire, MutexRelease

Flow control

Merge, Switch, Enter, Leave, NextIteration

## 逻辑回归问题与模型

前面的Hello world应用并没有训练模型，接下来介绍一个逻辑回归问题与模型。我们使用numpy构建一组线性关系的数据，通过TensorFlow实现的随机梯度算法，在训练足够长的时间后可以自动求解函数中的斜率和截距。
![](https://pic1.zhimg.com/v2-e4be72313164091e5f83f8fb441c1450_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='975' height='757'></svg>)
上面的代码可以在tensorflow_examples项目（[https://github.com/tobegit3hub/tensorflow_examples/blob/master/linear_regression.py](https://link.zhihu.com/?target=https%3A//github.com/tobegit3hub/tensorflow_examples/blob/master/linear_regression.py)）中找到，经过训练，我们看到输出的斜率w约为2，截距b约为10，与我们构建的数据之间的关联关系十分吻合！注意在TensorFlow代码中并没有实现最小二乘法等算法，也没有if-else来控制代码逻辑，完全是由数据驱动并且根据梯度下降算法动态调整Loss值学习出来的。这样我们即使换了其他数据集，甚至换成图像分类等其他领域的问题，无需修改代码也可以由机器自动学习，这也是神经网络和TensorFlow强大的地方。

前面的模型只有w和b两个变量，如果数据处于非线性关系就难以得到很好的结果，因此我们建议使用深层神经网络，这也是TensorFlow设计重点就要解决的深度学习模型。Google在2014年凭借Inception模型赢下了ImageNet全球竞赛，里面代码就是基于TensorFlow实现的，下面是较为复杂的模型定义代码。
![](https://pic3.zhimg.com/v2-e57d2fd5494a292f28245383a50b2462_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1039' height='726'></svg>)
使用TensorFlow已经封装好的全连接网络、卷积神经网络、RNN和LSTM，我们已经可以组合出各种网络模型，实现Inception这样的多层神经网络如拼凑Lego一样简单。但在选择优化算法、生成TFRecords、导出模型文件和支持分布式训练上，有较多的细节，大家在实际练习中要多加注意。

## 深度学习实战

TensorFlow是很好的深度学习框架，希望在深度学习方向有更深入的学习和实践的同学，七月在线特推出**《深度学习集训营》**课程，从TensorFlow搭建DNN解决问题起步，实战BAT工业项目。

**线上线下结合（线下在北京和上海）**，从头到尾全部实战，涵盖特征工程、深度学习在计算机视觉和自然语言处理领域中的应用，更有大规模车辆图片检索等应用于智慧城市、视频监控等场景的杀手级项目。

**加入课程，你将体验到以下优质服务：**

**全是深度学习的典型应用场景**

涵盖特征工程、混合网络、深度学习在**计算机视觉**当中的应用（比如图像分类、图像检索）、深度学习在**自然语言处理**中的应用（比如文本处理、文本分类、图像生成文本）、**聊天机器人**、**大规模车辆图片检索等**应用于智慧城市、视频监控等安防场景的BAT工业项目。

**BAT专家级讲师 + 助教全方位辅导**

我们拥有来自BAT的专家级讲师和数位助教，给你全程全天候1v1般的定制辅导。**平时晚上在线学习，周末线下项目实战**，从而通过在线直播从头到尾掌握深度学习典型应用场景，通过线下项目实战练就DL工业项目的全栈能力。且有问题，**课上课后随时答疑，手把手教会为止。**

**提供GPU云实验平台**

还原BAT真实生产环境，提供工业数据和国内首创的价值数十万的**GPU云实验平台（提前装tensorflow、caffe、mxnet等主流DL框架和相关数据）**。提供完善的实验平台供您动手、真枪实战，拒绝纸上谈兵。

**作业项目考核三管齐下**

开课前提供机器学习、深度学习相关的在线课程做预习。每次课通过GPU + Jupyter Notebook + GitHub在线提交作业，然后**讲师和助教1v1在线批改、讲解作业，且提供可执行的交互式代码，**在不断的阶段性实战和考试中掌握机器学习技能。

**高起步 + 简历优化**

从特征工程、TensorFlow起步，一上来就**实战BAT工业项目**。且根据集训营实战项目，将涉及到的关键知识点和项目经历优化到您的简历中。

**面试求职辅导 + 就业推荐**

精讲深度学习工程师面试时常见考点/模型/算法，且BAT一线技术经理1v1模拟真实面试，从技术、表达等方面全方位提升您的面试能力。根据您的技术特长提供定制化的能力评估、**就业指导以及包括BAT等一线互联网公司的工作机会推荐**。2个月挑战年薪30~50万。




                                                扫下方二维码，免费试听
![](https://pic1.zhimg.com/v2-16d1af8c60418b7058bbea6a2580387c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='1334'></svg>)



