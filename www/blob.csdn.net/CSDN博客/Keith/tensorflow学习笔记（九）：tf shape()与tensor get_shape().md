# tensorflow学习笔记（九）：tf.shape()与tensor.get_shape() - Keith - CSDN博客





2016年10月23日 22:19:44[ke1th](https://me.csdn.net/u012436149)阅读数：48522
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









**两种方法都可以获得变量的shape**

## tf.shape(x)

**其中x可以是tensor，也可不是tensor，返回是一个tensor.**

```python
shape=tf.placeholder(tf.float32, shape=[None, 227,227,3] )
```

我们经常会这样来`feed`数据,如果在运行的时候想知道`None`到底是多少,这时候,只能通过`tf.shape(x)[0]`这种方式来获得.

## tensor.get_shape()

**只有tensor有这个方法， 返回是一个tuple**



