
# TensorFlow基础 介绍 - lyc_yongcai的博客 - CSDN博客


2017年06月14日 21:34:23[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：250个人分类：[TensorFlow																](https://blog.csdn.net/lyc_yongcai/article/category/6970987)[深度学习																](https://blog.csdn.net/lyc_yongcai/article/category/6970795)[
							](https://blog.csdn.net/lyc_yongcai/article/category/6970987)


使用 TensorFlow, 你必须明白 TensorFlow:
使用图 (graph) 来表示计算任务.
在被称之为会话 (Session)的上下文 (context)
 中执行图.
使用 tensor 表示数据.
通过变量 (Variable)维护状态.
使用 feed 和 fetch 可以为任意的操作(arbitrary operation) 赋值或者从其中获取数据.
## 综述
TensorFlow 是一个编程系统, 使用图来表示计算任务. 图中的节点被称之为op(operation 的缩写). 一个 op 获得 0 个或多个`Tensor`,
 执行计算, 产生 0 个或多个`Tensor`. 每个 Tensor 是一个类型化的多维数组. 例如, 你可以将一小组图像集表示为一个四维浮点数数组, 这四个维度分别是`[batch,
 height, width, channels]`.
一个 TensorFlow 图描述了计算的过程. 为了进行计算, 图必须在`会话`里被启动.`会话`将图的
 op 分发到诸如 CPU 或 GPU 之类的`设备`上, 同时提供执行 op 的方法. 这些方法执行后, 将产生的 tensor 返回. 在[Python](http://lib.csdn.net/base/python)语言中, 返回的 tensor 是[IPython](http://www.numpy.org/)之类的[python](http://lib.csdn.net/base/python)交互环境, 可以使用`InteractiveSession`代替`Session`类,
 使用`Tensor.eval()`和`Operation.run()`方法代替`Session.run()`.
 这样可以避免使用一个变量来持有会话.
```python
# 进入一个交互式 TensorFlow 会话.
import tensorflow as tf
sess = tf.InteractiveSession()
x = tf.Variable([1.0, 2.0])
a = tf.constant([3.0, 3.0])
# 使用初始化器 initializer op 的 run() 方法初始化 'x' 
x.initializer.run()
# 增加一个减法 sub op, 从 'x' 减去 'a'. 运行减法 op, 输出结果 
sub = tf.sub(x, a)
print sub.eval()
# ==> [-2. -1.]
```
## Tensor
TensorFlow 程序使用 tensor[
数据结构](http://lib.csdn.net/base/datastructure)来代表所有的数据, 计算图中, 操作间传递的数据都是 tensor. 你可以把 TensorFlow tensor 看作是一个 n 维的数组或列表. 一个 tensor 包含一个静态类型 rank, 和 一个 shape. 想了解 TensorFlow 是如何处理这些概念的, 参见[Rank,
 Shape, 和 Type](http://wiki.jikexueyuan.com/project/tensorflow-zh/resources/dims_types.html).
## 变量
[Variables](http://wiki.jikexueyuan.com/project/tensorflow-zh/how_tos/variables/index.html)for more details. 变量维护图执行过程中的状态信息. 下面的例子演示了如何使用变量实现一个简单的计数器.
 参见[变量](http://wiki.jikexueyuan.com/project/tensorflow-zh/get_started/tensorflow-zh/how_tos/variables.html)章节了解更多细节.
```python
# 创建一个变量, 初始化为标量 0.
state = tf.Variable(0, name="counter")
# 创建一个 op, 其作用是使 state 增加 1
one = tf.constant(1)
new_value = tf.add(state, one)
update = tf.assign(state, new_value)
# 启动图后, 变量必须先经过`初始化` (init) op 初始化,
# 首先必须增加一个`初始化` op 到图中.
init_op = tf.initialize_all_variables()
# 启动图, 运行 op
with tf.Session() as sess:
  # 运行 'init' op
  sess.run(init_op)
  # 打印 'state' 的初始值
  print sess.run(state)
  # 运行 op, 更新 'state', 并打印 'state'
  for _ in range(3):
    sess.run(update)
    print sess.run(state)
# 输出:
# 0
# 1
# 2
# 3
```
代码中`assign()`操作是图所描绘的表达式的一部分, 正如`add()`操作一样.
 所以在调用`run()`执行表达式之前, 它并不会真正执行赋值操作.
通常会将一个统计模型中的参数表示为一组变量. 例如, 你可以将一个神经网络的权重作为某个变量存储在一个 tensor 中. 在训练过程中, 通过重复运行训练图, 更新这个 tensor.
## Fetch
为了取回操作的输出内容, 可以在使用`Session`对象的`run()`调用
 执行图时, 传入一些 tensor, 这些 tensor 会帮助你取回结果. 在之前的例子里, 我们只取回了单个节点`state`, 但是你也可以取回多个 tensor:
```python
input1 = tf.constant(3.0)
input2 = tf.constant(2.0)
input3 = tf.constant(5.0)
intermed = tf.add(input2, input3)
mul = tf.mul(input1, intermed)
with tf.Session() as sess:
  result = sess.run([mul, intermed])
  print result
# 输出:
# [array([ 21.], dtype=float32), array([ 7.], dtype=float32)]
```
需要获取的多个 tensor 值，在 op 的一次运行中一起获得（而不是逐个去获取 tensor）。
## Feed
上述示例在计算图中引入了 tensor, 以常量或变量的形式存储. TensorFlow 还提供了 feed 机制, 该机制 可以临时替代图中的任意操作中的 tensor 可以对图中任何操作提交补丁, 直接插入一个 tensor.
feed 使用一个 tensor 值临时替换一个操作的输出结果. 你可以提供 feed 数据作为`run()`调用的参数. feed 只在调用它的方法内有效, 方法结束,
 feed 就会消失. 最常见的用例是将某些特殊的操作指定为 "feed" 操作, 标记的方法是使用 tf.placeholder() 为这些操作创建占位符.
```python
input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)
output = tf.mul(input1, input2)
with tf.Session() as sess:
  print sess.run([output], feed_dict={input1:[7.], input2:[2.]})
# 输出:
# [array([ 14.], dtype=float32)]
```
如果没有正确提供 feed,`placeholder()`操作将会产生错误。MNIST 全连通[feed
 教程](http://wiki.jikexueyuan.com/project/tensorflow-zh/tutorials/mnist_tf.html)给出了一个更大规模的使用 feed 的例子。

