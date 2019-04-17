# CS 20SI|Lecture 5 Managing experiments and process data - 浅梦的博客 - CSDN博客





2017年02月06日 15:35:21[浅梦s](https://me.csdn.net/u012151283)阅读数：343标签：[TensorFlow																[深度学习](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)](https://so.csdn.net/so/search/s.do?q=TensorFlow&t=blog)
个人分类：[TensorFlow																[深度学习](https://blog.csdn.net/u012151283/article/category/6698461)](https://blog.csdn.net/u012151283/article/category/6698460)







本章主要讲了如何管理实验，包括保存实验步骤，结果，重现实现结果等。由于深度学习的训练过程需要大量时间，如果训练过程中出现意外导致训练停止，那么重新进行训练的时间损失是很大的。我们需要一种机制能够随时停止我们的训练，并且随时恢复训练。另外，由于训练过程设计了很多随机操作，如何数据集划分，参数随机初始化等，我们也需要一种方法能够控制这些随机因子使得不同的人拿到我们的程序能够跑出相同的结果。

# Agenda
- More word2vec
- tf.train.Saver
- tf.summary
- Randomization
- Data Readers

# tf.train.Saver()

每进行一定次数的训练迭代，就将Session中的参数保存起来。这样就可以恢复或继续训练模型。 
`tf.train.Saver()`类可以将计算图中的变量保存到二进制文件中。 
`tf​.​train​.​Saver​.​save​(​sess​,​ save_path​,​ global_step​=​None​,​ latest_filename​=​None​, meta_graph_suffix​=​'meta'​,​ write_meta_graph​=​True​,​ write_state​=​True)`
**注意**该API保存的是默认图中到调用为止时的变量，所以一般在所有计算图构建完成之后再定义saver对象。 

举例，如果希望每训练1000次保存一次模型中的变量，可以
```python
# define model 

# create a saver object 
saver ​=​ tf​.​train​.​Saver​() 
# launch a session to compute the graph 
with​ tf​.​Session​()​ ​as​ sess:     ​
    # actual training loop 
    for​ step ​in​ range​(​training_steps​):
        sess​.​run​([​optimizer​]) 
        if​ (step + 1) ​%​ ​1000​==​0:
            saver.save(sess,' checkpoint_directory/model_name',global_step=model.global_step)
```

在TensorFlow中，保存模型变量的step称为checkpoint.因为通常会创建多个checkpoint,在模型中添加一个表示训练次数的变量会比较好。 

首先创建一个变量，初始化位0，并将其设置为不可训练变量`trainalble = False`，因为不希望TensorFlow对它进行优化。 
`self​.​global_step ​=​ tf​.​Variable​(​0​,​ dtype​=​tf​.​int32​,​ trainable​=​False​,​ name​=​'global_step')`

将`global_step`作为参数传递给优化器，优化器就会自动对它进行累加 
`self​.​optimizer ​=​ tf​.​train​.​GradientDescentOptimizer​(​self​.​lr​).​minimize​(​self​.​loss​,                                                                global_step​=​self​.​global_step)`

以`model-name-global-step`格式保存模型变量 
`saver​.​save​(​sess​,​ ​'checkpoints/skip-gram'​,​ global_step​=​model​.​global_step)`

使用`tf.train.Saver.restore(sess, save_path)`恢复模型变量 
`saver​.​restore​(​sess​,​ ​'checkpoints/skip-gram-10000')`

当然，只有给定的checkpoint有效时才可以恢复变量，我们想做的可能是如果有checkpoint，从它恢复，如果没有那么重新开始训练。`tf.train.get_checkpoint_state(‘directory-name’)`可以读取指定目录下的checkpoint,其执行代码如下：
```python
ckpt ​=​ tf​.​train​.​get_checkpoint_state​(​os​.​path​.​dirname​(​'checkpoints/checkpoint'​)) 
if​ ckpt ​and​ ckpt​.​model_checkpoint_path:      
    saver​.​restore​(​sess​,​ ckpt​.​model_checkpoint_path)
```

它会自动选择最新的checkpoint进行恢复。 

默认情况下，`saver.save()`会保存模型的所有变量。我们也可以在创建`saver`对象的到时候选择想要保存的变量作为一个`list`或`dict`传递给`saver`

```python
v1 ​=​ tf​.​Variable​(...,​ name​=​'v1'​)  
v2 ​=​ tf​.​Variable​(...,​ name​=​'v2'​)  
# pass the variables as a dict:  
saver ​=​ tf​.​train​.​Saver​({​'v1'​:​ v1​,​ ​'v2'​:​ v2​})  
# pass them as a list 
saver ​=​ tf​.​train​.​Saver​([​v1​,​ v2​])  
# passing a list is equivalent to passing a dict with the variable op names # as keys 
saver ​=​ tf​.​train​.​Saver​({​v​.​op​.​name​:​ v ​for​ v ​in​ ​[​v1​,​ v2​]})
```

**注意**`tf.train.Saver`只能保存变量，并不能保存完整的计算图 ，所以我们仍需自己创建计算图，然后加载变量。`checkpoint`指明了变量名到tensor的映射方式。 

通常并不会只保存最后一次迭代的参数，还会保存模型给出最优结果时的参数。

# tf.summary

之前我们用`matplotlib`来对loss和accuracy等指标进行可视化，其实TensorFlow已经提供给我们这些可视化工具。可以使用一个命名域来包括所有的summary ops。 
**Step 1:create summaries**

```python
def​ _create_summaries​(​self​): 
     ​with​ tf​.​name_scope​(​"summaries"​):
         tf​.​summary​.​scalar​(​"loss"​,​ ​self​.​loss
         tf​.​summary​.​scalar​(​"accuracy"​,​ ​self​.​accuracy​) 
         # because you have several summaries, we should merge them all 
         # into one op to make it easier to manage             ​
         self​.​summary_op ​=​ tf​.​summary​.​merge_all​()
```

`tf.summary`也是一个op,需要通过`sess.run()`来执行， 
**Step 2: run them**
`loss_batch​,​ _​,​ summary ​=​ sess​.​run​([​model​.​loss​,​ model​.​optimizer​,​ model​.​summary_op​],                                    feed_dict​=​feed_dict)`

最后使用`tf.summary.FileWriter`对象将`summary`写到文件中 
`writer​.​add_summary​(​summary​,​ global_step​=​step)`

使用 `tf.summary.image`以图片形式可视化统计信息 
**Step 3: write summaries to file**
`tf​.​summary​.​image​(​name​,​ tensor​,​ max_outputs​=​3​,​ collections​=​None)`
# Control randomization

## Set random seed at operation level

所有的`tensor`都可以在初始化的时候传递一个`seed`
`my_var ​=​ tf​.​Variable​(​tf​.​truncated_normal​((-​1.0​,​1.0​),​ stddev​=​0.1​,​ seed​=​0​))`
`session`会追踪`random state`，所以每一个新的`session`都会从原始的`random state`开始。
```python
c ​=​ tf​.​random_uniform​([],​ ​-​10​,​ ​10​,​ seed​=​2) 
with​ tf​.​Session​()​ ​as​ sess: 
    print​ sess​.​run​(​c) # >> 3.57493 
    print​ sess​.​run​(​c) # >> -5.97319
```

```python
c ​=​ tf​.​random_uniform​([],​ ​-​10​,​ ​10​,​ seed​=​2) 
with​ tf​.​Session​()​ ​as​ sess: 
    print​ sess​.​run​(​c) # >> 3.57493 
with​ tf​.​Session​()​ ​as​ sess: 
    print​ sess​.​run​(​c) # >> 3.57493
```

```python
c ​=​ tf​.​random_uniform​([],​ ​-​10​,​ ​10​,​ seed​=​2) 
d ​=​ tf​.​random_uniform​([],​ ​-​10​,​ ​10​,​ seed​=​2) 
with​ tf​.​Session​()​ ​as​ sess: 
    print​ sess​.​run​(​c) # >> 3.57493 
    print​ sess​.​run​(​d) # >> 3.57493
```

## Set random seed at graph level with tf.Graph.seed

`tf​.​set_random_seed​(​seed)`

 如果不在乎计算图中的每个op的随机化，而只想能够重复实验结果。可以使用`tf.set_random_seed`，该操作只影响当前的默认计算图。

```python
import​ tensorflow ​as​ tf 
tf​.​set_random_seed​(​2) 
c ​=​ tf​.​random_uniform​([],​ ​-​10​,​ ​10) 
d ​=​ tf​.​random_uniform​([],​ ​-​10​,​ ​10) 
with​ tf​.​Session​()​ ​as​ sess: 
    print​ sess​.​run​(​c) 
    print​ sess​.​run​(​d)
```

如果不设置随机种子，那么相同的代码运行两次会产生不同的结果，设置之后则会产生一致的结果。

# Reading Data in TensorFlow

有两种方式可以将数据加载到计算图中，一种是通过`feed_dcit`。另一种是通过`readers`，可以直接从文件中读取`tensors`。 

首先来看一下`feed_dict`的工作机制，它先从存储系统中读取数据到客户端，然后再将数据从客户端发送到服务进程。通信使得数据读取的速度变慢，尤其是当客户端和服务进程在不同的物理机器上的时候。 

TensorFlow的`readers`允许直接将数据读取到服务进程中。 

这种性能上的提升在分布式系统或大数据集上尤为显著。 

下图是不同文件类型的不同的Readers 
![这里写图片描述](https://img-blog.csdn.net/20170206163153487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









