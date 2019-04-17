# TensorFlow多GPU并行的实现 - 战斗蜗牛的专栏 - CSDN博客





2018年02月07日 14:57:30[vbskj](https://me.csdn.net/vbskj)阅读数：2237









    深度学习算法由于其数据量大、算法复杂度高等特点，常常需要采用某种形式的并行机制，常用的并行方法有数据并行（data parallel）和模型并行（model parallel）两种。尽管现有的深度学习框架大多都支持多GPU，但caffe、theano、tensorflow采用的都是数据并行，而亚马逊推出的DSSTNE（Deep Scalable Sparse Tensor Network Engine）也支持模型并行，感兴趣的读者可以阅读其Github源代码。这里我主要介绍的是tensorflow多GPU编程，尽管tensorflow官网已经给出了很详细的说明文档，但在我自己实现过程中也遇到了一些问题，在这里记录下来。


        数据并行的原理很简单，如下图，其中CPU主要负责梯度平均和参数更新，而GPU1和GPU2主要负责训练模型副本（model replica），这里称作“模型副本”是因为它们都是基于训练样例的子集训练得到的，模型之间具有一定的独立性。具体的训练步骤如下，

![](https://img-blog.csdn.net/20161116155243864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 

 （1）在GPU1、GPU2上分别定义模型参数变量，网络结构；


（2）对于单独的GPU，分别从数据管道读取不同的数据块，然后进行前向传播（forward propagation）计算出loss，再计算关于当前Variables的gradients；


（3）把所有GPU输出的梯度数据转移到CPU上，先进行梯度取平均操作，然后进行模型参数的更新；


（4）重复步骤(1)-(3)，直到模型参数收敛为止；



值得说明的是，在第（1）步中定义模型参数时，要考虑到不同model replica之间要能够share Variables，因此要采用tf.get_variable()函数而不是直接tf.Variables()（关于tf.get_variable()和tf.Variable的区别，请读者参见其帮助手册）。另外，因为tensorflow和theano类似，都是先定义好tensor Graph，再基于已经定义好的Graph进行模型迭代式训练的。因此在每次迭代过程中，只会对当前的模型参数进行更新，而不会调用tf.get_variable()函数重新定义模型变量，因此变量共享只是存在于模型定义阶段的一个概念。


       在实现过程中，感觉跟上一篇博客中单cpu版本的训练程序很相似，且理解起来比较简单，因此这里就不再帖程序了。当然，这个过程中我也遇到了一些问题，通过单步调试和查阅一些资料，找到了问题的原因，这里一并记录下来，方便以后遇到相同的问题时能从这里找到答案。


（1）tf.name_scope()和tf.variable_scope()命令，用于定义命名区间，都是context manager。区别在于，在当前作用域中，使用tf.Varialbe()定义变量和定义运算（比如tf.add()）时，都会在variable或者op前添加此prefix；但使用tf.get_variable()定义变量时，该变量处于variable_scope作用域中时会添加prefix，而处于name_scope中时则不会添加；


（2）tf.variable_scope()和tf.get_variable(name, shape, initializer)命令可以组合使用，用于解决变量共享问题，因为对于tf.get_variable()命令， 可以读一个已经存在的变量或者创建一个新的变量，具体来说，它会在名字前面添加当前的variable scope，并读取当前variable_scope()函数中的‘reuse’属性。当reuse='True'时，对于当前scope以及所有sub-scopes都进入reuse模式，此时会从当前域中读一个已经存在的变量；当reuse=‘None’时，则会继承parent
 scope的reuse取值，父域值为True时同上，为False时则会在当前域中创建新的变量。另外，可以调用tf.get_variable_scope().reuse_variables()命令设置reuse标志为True。为了更加方便实际应用的场景，tensorflow规定不能直接设置reuse标志为False，只能通过“enter reusing variable scope -> exit -> go back to a non-reusing one"的方式实现这一目的，突然想到了“曲线救国”，有点像哈～


（3）错误提示“TypeError: not all arguments converted during string formatting”

         错误原因：for grad_all_gpu in zip(grads_all_gpu)应改为for grad_all_gpu in zip(*grads_all_gpu):


          这里设计到了zip函数的语法。 zip([seql, ...])接受一系列可迭代对象作为参数，将对象中对应的元素打包成一个个tuple（元组），然后返回由这些tuples组成的list（列表）。若传入参数的长度不等，则返回list的长度和参数中长度最短的对象相同。

          zip()配合*号操作符,可以将已经zip过的列表对象解压；


（4）错误提示“SystemError: <built-in function delete_Status> returned a result with an error set”

         错误原因： 定义的运算中存在不能在GPU上执行的操作，比如 RandomShuffleQueue等，因为这些操作的底层实现只有CPU版本，而没有GPU版本；


         解决办法：在创建Session时，设置属性allow_soft_placement为True，那么所有的操作会默认使用其被指定的设备，比如gpu，但当该tensorflow操作函数没有gpu实现时，会自动使用cpu设备； 


 （5）警告提示“random_shuffle_queue: Skipping cancelled enqueue attempt with queue not closed，

                         “ERROR:tensorflow:Exception in QueueRunner: <built-in function delete_Status> returned a result with an error set”


         解决办法：




                方法一：将with tf.Session() as sess改为sess = tf.Session(),这样就不会打印这段信息了；


                方法二：升级TensorFlow的版本，在新版本的tensorflow中已经自动屏蔽掉了这段提示信息 


参考资料：https://github.com/tensorflow/tensorflow/blob/r0.11/tensorflow/models/image/cifar10/cifar10_multi_gpu_train.py


                  https://www.tensorflow.org/versions/r0.11/how_tos/using_gpu/index.html



