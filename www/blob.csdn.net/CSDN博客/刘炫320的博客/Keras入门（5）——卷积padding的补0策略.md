# Keras入门（5）——卷积padding的补0策略 - 刘炫320的博客 - CSDN博客
2018年04月25日 15:03:45[刘炫320](https://me.csdn.net/qq_35082030)阅读数：3763
# 0. 前言
作为最基础的卷积层——CNN，我们应当对他最为熟悉。但是在实现的时候，忽然发现对于其第一步骤，就有困惑的地方，那就是padding，也就是补0策略。
在Keras中，卷积层的定义是如下：
```python
keras.layers.convolutional.Conv1D(filters, kernel_size, strides=1, padding='valid', dilation_rate=1, activation=None, use_bias=True, kernel_initializer='glorot_uniform', bias_initializer='zeros', kernel_regularizer=None, bias_regularizer=None, activity_regularizer=None, kernel_constraint=None, bias_constraint=None)
```
可以看到padding是一个参数，它有可选值，根据官方文档说明，它总共包含3种可选值：’valid’,’same’和‘casual’.那么这每个意思也进行了描述，但是具体如何操作呢？
“valid”代表只进行有效的卷积，即对边界数据不处理。这个是非常容易理解的，即不进行补0，能有多少卷积就有多少卷积，但是会抛弃一些数据，而且每次总是最后的数字被抛弃。这也可能造成右边界数据没有顾忌到的缺陷。
例如，一个序列有13长，我们的kernel_size为6，步长为5，那么采用valid的补0策略的卷积后的大小为2，即 
$\left \lfloor \frac{13-6}{5} \right \rfloor+1$
“same”代表进行补0卷积，但是它的补0也是有上限的，它的第一步卷积补0为kernel_size-strides，然后到最后剩多少补多少0，如果采用same补0策略的卷积，那么上面例子中的卷积后大小为3，即 
$\left \lceil \frac{13-6}{5}\right \rceil+1$
“casual”则是代表具有时序关系的卷积，即output[t]不依赖于input[t+1：]。当对不能违反时间顺序的时序信号建模时有用。一般不使用这种。
