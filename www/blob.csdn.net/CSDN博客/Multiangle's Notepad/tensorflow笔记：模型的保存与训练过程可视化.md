
# tensorflow笔记：模型的保存与训练过程可视化 - Multiangle's Notepad - CSDN博客


2016年12月28日 19:47:11[multiangle](https://me.csdn.net/u014595019)阅读数：20196所属专栏：[深度学习&tensorflow笔记](https://blog.csdn.net/column/details/13414.html)



tensorflow笔记系列：
（一）[ tensorflow笔记：流程，概念和简单代码注释](http://blog.csdn.net/u014595019/article/details/52677412)
（二）[tensorflow笔记：多层CNN代码分析](http://blog.csdn.net/u014595019/article/details/52728886)
（三）[tensorflow笔记：多层LSTM代码分析](http://blog.csdn.net/u014595019/article/details/52759104)
（四）[tensorflow笔记：常用函数说明](http://blog.csdn.net/u014595019/article/details/52805444)
（五）[tensorflow笔记：模型的保存与训练过程可视化](http://blog.csdn.net/u014595019/article/details/53912710)
（六）[tensorflow笔记：使用tf来实现word2vec](http://blog.csdn.net/u014595019/article/details/54093161)
---

# 保存与读取模型
在使用tf来训练模型的时候，难免会出现中断的情况。这时候自然就希望能够将辛辛苦苦得到的中间参数保留下来，不然下次又要重新开始。好在tf官方提供了保存和读取模型的方法。
保存模型的方法：
```python
# 之前是各种构建模型graph的操作(矩阵相乘，sigmoid等等....)
```
```python
saver = tf
```
```python
.train
```
```python
.Saver
```
```python
()
```
```python
# 生成saver
```
```python
with tf
```
```python
.Session
```
```python
() as sess:
    sess
```
```python
.run
```
```python
(tf
```
```python
.global
```
```python
_variables_initializer())
```
```python
# 先对模型初始化
```
```python
# 然后将数据丢入模型进行训练blablabla
```
```python
# 训练完以后，使用saver.save 来保存
```
```python
saver
```
```python
.save
```
```python
(sess,
```
```python
"save_path/file_name"
```
```python
)
```
```python
#file_name如果不存在的话，会自动创建
```
将模型保存好以后，载入也比较方便，如下所示：
```python
saver = tf
```
```python
.train
```
```python
.Saver
```
```python
()
with tf
```
```python
.Session
```
```python
() as sess:
```
```python
#参数可以进行初始化，也可不进行初始化。即使初始化了，初始化的值也会被restore的值给覆盖
```
```python
sess
```
```python
.run
```
```python
(tf
```
```python
.global
```
```python
_variables_initializer())     
    saver
```
```python
.restore
```
```python
(sess,
```
```python
"save_path/file_name"
```
```python
)
```
```python
#会将已经保存的变量值resotre到 变量中。
```
简单的说，就是**通过saver.save来保存模型，通过saver.restore来加载模型**
---

# 使用tensorboard来使训练过程可视化
tensorflow还提供了一个可视化工具，叫tensorboard.启动以后，可以通过网页来观察模型的结构和训练过程中各个参数的变化。如下图所示
![选区_059.png-12.7kB](http://static.zybuluo.com/multiangle/yyfuyumf0325sow6ydofd0ox/%E9%80%89%E5%8C%BA_059.png)
关于如何合理清楚的显示网络结构，我目前还不太搞得清楚，而且目前看来也不是太重要；但是要将训练的过程可视化还是比较方便的。简单的说，流程如下所示：
使用tf.scalar_summary来收集想要显示的变量
定义一个summury op, 用来汇总多个变量
得到一个summy writer，指定写入路径
通过summary_str = sess.run()
```python
# 1. 由之前的各种运算得到此批数据的loss
```
```python
loss = .....
```
```python
# 2.使用tf.scalar_summary来收集想要显示的变量,命名为loss
```
```python
tf.scalar_summary(
```
```python
'loss'
```
```python
,loss)
```
```python
# 3.定义一个summury op, 用来汇总由scalar_summary记录的所有变量
```
```python
merged_summary_op = tf.merge_all_summaries()
```
```python
# 4.生成一个summary writer对象，需要指定写入路径,例如我这边就是/tmp/logdir
```
```python
summary_writer = tf.train.SummaryWriter(
```
```python
'/tmp/logdir'
```
```python
, sess.graph)
```
```python
# 开始训练，分批喂数据
```
```python
for
```
```python
(i in range(batch_num)):
    #
```
```python
5.
```
```python
使用sess.run来得到merged_summary_op的返回值
    summary_str = sess.run(merged_summary_op)
    #
```
```python
6.
```
```python
使用summary writer将运行中的loss值写入
    summary_writer.add_summary(summary_str,i)
```
接下来，程序开始运行以后，跑到shell里运行
```python
$
```
```python
tensorboard
```
```python
-
```
```python
-
```
```python
logdir
```
```python
/tmp/logdir
```
开始运行tensorboard.接下来打开浏览器，进入`127.0.0.1:6006`就能够看到loss值在训练中的变化值了。

