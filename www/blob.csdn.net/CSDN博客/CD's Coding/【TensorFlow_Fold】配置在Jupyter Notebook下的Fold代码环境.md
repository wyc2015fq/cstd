# 【TensorFlow_Fold】配置在Jupyter Notebook下的Fold代码环境 - CD's Coding - CSDN博客





2017年04月07日 17:05:56[糖果天王](https://me.csdn.net/okcd00)阅读数：1193








## TensorFlow_Fold

### TensorFlow_Fold in Jupyter Notebook

![Jupyter on CentOS7](https://img-blog.csdn.net/20170407164851426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

效果预览如上图，环境为 CentOS7 + Python with TensorFlow1.0(Fold include)
### How to add kernel for Jupyter

jupyter里一般是没有我们自己设定的env下的python的，比如我包含tensorflow_fold库的python就是在`source activate tensorflow1.0`后才能获取到，所以在为notebook添加这个环境下的python才可以在notebook里获取到。

```
sudo pip install -U ipykernel
# source activate tensorflow1.0
python -m ipykernel install --user
```

简要的解释一下，如果是Linux环境，有一个叫做ipykernel的包可以轻易的添加kernel，不过需要注意的是，一定要确认当前python是不是想加进去的那个python哦，可以用`which python`来check一下。

![KernelList](https://img-blog.csdn.net/20170407165743586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

导入成功之后，就可以在New的时候看到这个kernel啦。
### 关于GPU的选择使用

假如你们的机器上，有多个GPU，鉴于tensorflow会擅自把所有GPU都拿来用，你想要只用里面的某一个来跑就足够了（比如我们组的四路泰坦，我就跑点小程序请不动这么多尊大神）

![GPU](https://img-blog.csdn.net/20170407170154104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么，在代码前（请注意，是在import前！）加入如下代码即可，比如下面我用的是序号为0的GPU：

```python
import os
os.environ["CUDA_DEVICE_ORDER"]="PCI_BUS_ID"
os.environ["CUDA_VISIBLE_DEVICES"]="0"
```

Tips：`import tensorflow`和`import tensorflow_fold`都会在import的时候就决定了使用哪些GPU，所以一定要先设置好environ，再import这俩兄弟。

## About TensorFlow_Fold

### [封装在静态框架上的动态接口](https://baijiahao.baidu.com/po/feed/share?context=%7B%22sourceFrom%22%3A%22bjh%22%2C%22nid%22%3A%22news_4260043313023307038%22%7D)
- Dynamic Batching的算法很繁琐，但不用当心，这个过程是由框架自动完成的，作为框架的使用者，我们只要知道怎么调用官方给出来的接口就可以了。
- 新推出的TensorFlow Fold就是一个TensorFlow的封装，设计参考了函数式编程的一些思想，目的就是方便用户快速地构建动态计算图。下面我们来简单地浏览一下，要进一步了解可以去看官方的教学文档。[Documents](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py)
- TensorFlow Fold提供了一些函数专门用来处理序列(x1,…,xn)： 
- Map(f)：计算[f(x1) ,…,f(xn)]将函数f应用到每一个序列的元素，比如将句子中的每一个词转化为词向量；
- Fold(g, z)：计算g(…,g(z, x1), x2), …,xn )，比如说展开一个RNN（循环神经网络）；
- Reduce (g)：计算g(Reduce(g)[x1 ,…,xn/2],Reduce(g)[xn/2 ,…,xn]，将函数g应用到一颗平衡二叉树上，比如对序列中的元素作max或sum-pooling。

- 由于TensorFlow原本的基本单元Tensor不适合用于构建动态图，所以Fold引入新的基本组件Block。Block有明确的一个输入类型和一个输出类型，包括： 
- Input：来着编程语言如Python中元素，比如字典等；
- Tensor：拥有数据类型和形状的TensorFlow基本模块；
- Tuple (t1 ,…,tn )：括号中的每一个t表示对应位置的类型；
- Sequence (t)：一个不定长的拥有类型为t的元素的序列；
- Void：单元类型。这些基本类型可以相互嵌套，比如一个Block的输入类型可以是Input类型的Tuple。

- 用来创建Block的基本函数有： 
- Scalar：将Python标量转化为Tensor；Tensor：将Numpy数组转化为Tensor；Function (h )：创建一个Operation；
- InputTransform (h)：用于预处理Python类型。

- 用来组合Block的基本函数有： 
- b1>>b2，流水线（pipeline）：将b1 的输出作为b2 的输入；
- Record({l1：b1,…, ln：bn})： 接受一个Python字典为输入，对字典中key值为li 的value应用；
- OneOf (b1,…,bn)：根据输入条件应用b1,…bn中的一个；
- Optional (b)：OneOf的特例，如果输入不为None，应用b；
- AllOf (b1,…,bn)：输入应用中的每一个。

- 用来组合Block的高级函数有： 
- Composition()：流水线的升级版，流水线只能处理串行的流程，Composition()创建一个Scope对象，在这个Scope的缩进范围内，采用b.reads(b1,…,bn )来读取多个数据流，可以用于构建多分支结构；
- ForwardDeclaration()：用来创建递归结构，这个函数可以先定义一个预先占位的表达式expr，等这个表达式定义完再用expr.resolve_to(expr_def)，将表达式递归地代入，这是用来创建树结构计算图必不可少的工具。


## Coding Logs

敲几个试试手……

```python
"""
td.InputTransform(fn):
Python Function to Blocks
"""

def func(alist):
    return (alist[3], alist[0]+alist[1])

b = td.InputTransform(func)
b.eval([1,2,3,4])  # => (4,3)
```

```python
"""
tf.split(value, num_or_size_splits, axis=0, num=None, name='split'):
Splits a tensor into sub tensors.
"""

# 'value' is a tensor with shape [5, 30]
# Split 'value' into 3 tensors with sizes [4, 15, 11] along dimension 1
split0, split1, split2 = tf.split(value, [4, 15, 11], 1)
tf.shape(split0) ==> [5, 4]
tf.shape(split1) ==> [5, 15]
tf.shape(split2) ==> [5, 11]
# Split 'value' into 3 tensors along dimension 1
split0, split1, split2 = tf.split(value, num_or_size_splits=3, axis=1)
tf.shape(split0) ==> [5, 10]
```





