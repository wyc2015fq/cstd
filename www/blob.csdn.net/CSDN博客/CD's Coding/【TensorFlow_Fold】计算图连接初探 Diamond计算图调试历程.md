# 【TensorFlow_Fold】计算图连接初探 Diamond计算图调试历程 - CD's Coding - CSDN博客





2017年04月20日 14:57:56[糖果天王](https://me.csdn.net/okcd00)阅读数：1762








## 0x00 前言

先前开了个TFF自学的坑，震惊于计算图连接的陷阱之多，代码之繁，头砸键盘于第二篇文章的Markdown前；

哦第二篇我指的是名字类似是《[【TensorFlow_Fold】深度探究 Blocks for Composition （未完成）](http://blog.csdn.net/okcd00/article/details/70174503)》的东西；

进行到计算图不同的block间的连接这一部分，但是td中琳琅满目的函数对输入输出却有着千奇百怪的格式需求

名为TypeError的Exception表示：

“二货点你给我走！我不想再看到你！”；

调试许久无果，耗时耗电不说，心力即将枯竭……

然后在我决定起身去吃饭的时候……



```python
Out[194]: array([2., 1., 3., 2.], dtype=float32)
```



哎？

于是我决定在啃完Composition之前，先来把坑了我不知道多久的Diamond计算图记录一下——

记录一下疯狂Debug（被虐）的历程




## 0x01 问题描述

当前——

我们有一个字典： 



```python
input_nodes = {
  1: [2, 1.0],
  2: [3, 2.0],
  3: [5, 3.0],
  # ......
}
```




我们有一个查询函数： 



```python
lookup = lambda idx: input_nodes.get(idx, 0)
```


我们想要这样的一个block，输入一个三元组，形似[left_idx, right_idx, father_idx]，如：[1, 2, 3]；



输出这个三元组中left_idx对应的list与right_idx对应的list拼成的新数组，形似[left_h, left_c, right_h, right_c], 如[2, 1, 3, 2]；

即借助lookup函数找出left和right在字典中对应的数组进行拼接。




对了，为什么叫Diamond形状的计算图呢，如下（Windows自带画图怎么画斜着的箭头求高手指点……）

![](https://img-blog.csdn.net/20170420142712197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## 0x02 声明与定义

之后的代码中会出现如下几个与框架构成无关但是可能影响理解的函数，预先声明其实现：

其实~~[Blocks第一篇](http://blog.csdn.net/okcd00/article/details/70169864)~~里都有写

block_info: 输出一个block的【type, input, output】



```python
def block_info(block):
    print("%s: %s -> %s" % (block, block.input_type, block.output_type))
```


td.InputTransform(py_function): 将一个python函数转化为block的方法





```python
"""
td.InputTransform(fn):
Python Function to Blocks
"""

def func_node(alist):
    return alist[0], {'h':alist[1], 'c':alist[2]}
    # return (alist[0], alist[1], alist[2])

def func_tree(alist):
    return (alist[0], alist[1], alist[2], alist[3])

IT_node = td.InputTransform(func_node)
IT_tree = td.InputTransform(func_tree)
IT_node.eval([1,2,'a']) # => (1, {'c':'a', 'h':2})
IT_tree.eval([1,2,'+',7]) # => (1, 2, '+', 7)

block_info(IT_node)
block_info(IT_tree)
# => <td.inputtransform py_fn='func_node'>: PyObjectType() -> PyObjectType()
# => <td.inputtransform py_fn='func_tree'>: PyObjectType() -> PyObjectType()
```




td.GetItem(idx) : 以传入的key来返回该键对应的值，输入需要是PyObject、Tuple或是有限长度的序列。


```python
# A block that calls Pythons getitem operator (i.e. [] syntax) on its input.
# The input type may be a PyObject, a Tuple, or a finite Sequence.
(GetItem(key) >> block).eval(inp) => block.eval(inp[key])
```






with c.scope():  c.connect(blockA, blockB): 不使用'>>'符号，而是用connect函数进行连接的操作，效果与>>相同。




td.Composition(): 一两句话说不清楚……等我把第二篇赶出来再去那边看应该会好一些，现在只需要理解这是一种计算图连接的方式就够了。


## 0x03 结论

国际惯例先贴结果：

可行方案如下（如下两种均可）：



```python
cd = td.Composition()
with cd.scope():
    fn1 = (td.GetItem(0)).reads(cd.input)
    fn2 = (td.GetItem(1)).reads(cd.input)
    h1 = (td.InputTransform(lookup) >> td.Vector(2)).reads(fn1)
    h2 = (td.InputTransform(lookup) >> td.Vector(2)).reads(fn2)
    cd.output.reads(td.Concat().reads(h1, h2))

block_info(cd) # => <td.Composition>: None -> TensorType((4,), 'float32')
cd.eval([1,2,'a',4]) # =>  array([2., 1., 3., 2.], dtype=float32)
```


```python
cd = td.Composition()
with cd.scope():
    fn1 = td.GetItem(0)
    fn2 = td.GetItem(1)
    h1 = td.InputTransform(lookup) >> td.Vector(2)
    h2 = td.InputTransform(lookup) >> td.Vector(2)
    cc = td.Concat()

    cd2.connect(cd2.input, fn1)
    cd2.connect(cd2.input, fn2)
    cd2.connect(fn1, h1)
    cd2.connect(fn2, h2)
    cd2.connect((h1, h2), cc)
    cd2.connect(cc, cd2.output)

block_info(cd) # => <td.Composition>: None -> TensorType((4,), 'float32')
cd.eval([1,2,'a',4]) # =>  array([2., 1., 3., 2.], dtype=float32)
```




## 0x04 坑点



嗯，坑点，没毛病，坑的就是我……

### 随处可见的TypeError

我甚至想把TFF直接改名叫TypeFFF算了……

初上手的时候请务必把Github打开，对照着td.md和blocks.md看清楚再敲，因为即便你觉得这个逻辑很简单，每个函数的效果我也知道了，拼一起不就Easy搞定的事情——

他可能没那么容易……

比如说： Nth() 和 GetItem() 这两者的输入是 PyObject，所以你传入的是TupleType、Tensortype都会报TypeError

又比如说： 你写了一个python函数，使用InputTransform将其变成了block，这时往往输入输出都是PyObject，所以前后与其相接的不是这个都不行，比如Concat()或者Sum()

在这里有一个小建议，不要试图像示例代码里那样一写就是一大串看起来duang的一下搞定一切，很快捷很帅气~~都是特效~~

我们老老实实一个一个block的写，写完一个就block_info()检查一下当前的这个block的输入输出数据类型，像拼图一样把正确对应的拼接在一起。

### 爹只有一个

另一个常见的错误则是：TypeError: block <td.xxxxx> is already a child of <td.xxxxx>


其实这也是TypeError的一种，不过本质不是太一样，~~反正TypeError_Fold~~

这个在Github上#[Issue40](https://github.com/tensorflow/fold/issues/40)中有提及过，因为对于一个block，它在计算图中的位置一旦决定，就固定下来了（基于Fold底层实现的原理），所以一旦将某个block连接到另一个block上，它就不能再被你链接到其他地方了。

那如果我有一种block到处都得用该怎么办呢，比如矩阵乘法？

Block不能复用，但是Function能呀，我们可以先把函数构造好用一个函数放着，每次想用这种block了，就通过call这个函数来返回一个Function用来构造这次要用的block。




## 0x05 官方例程

对于Diamond计算图，官方给出了一个这样的最简单的对于Scalar输入的示例，

如果对于我写的认为比较长也可以先看看这个，我写的对于输入以及对于非TensorType类型的操作做了令其稍微普适一点的工作而已~



```python
def test_composition_diamond(self):
    sc = tdb.Scalar()
    fn1 = times_scalar_block(2.0)
    fn2 = times_scalar_block(3.0)
    fn3 = tdb.Function(tf.add)

    # out = in*2 + in*3
    c = tdb.Composition([sc, fn1, fn2, fn3])
    c.connect(c.input, sc)
    c.connect(sc, fn1)
    c.connect(sc, fn2)
    c.connect((fn1, fn2), fn3)
    c.connect(fn3, c.output)

    self.assertBuilds(25., c, 5, max_depth=2)

def test_composition_diamond_with_block(self):
    # out = in*2 + in*3
    c = tdb.Composition()
    with c.scope():
        scalar = tdb.Scalar().reads(c.input)
        fn1 = times_scalar_block(2.0).reads(scalar)
        fn2 = times_scalar_block(3.0).reads(scalar)
        c.output.reads(tdb.Function(tf.add).reads(fn1, fn2))
    self.assertBuilds(25., c, 5, max_depth=2)

def test_composition_nested(self):
    fn1 = times_scalar_block(2.0)
    fn2 = times_scalar_block(3.0)
    c = tdb.Composition([fn1, fn2])
    c.connect(c.input, fn1)
    c.connect(c.input, fn2)
    c.connect((fn1, fn2), c.output)
    c2 = tdb.Scalar() >> c >> tdb.Function(tf.add)
    self.assertBuilds(5.0, c2, 1.0, max_depth=2)
```










