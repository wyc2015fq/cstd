# CS 20SI|Lecture 7 Playing with convolutions in TensorFlow - 浅梦的博客 - CSDN博客





2017年02月26日 13:33:54[浅梦s](https://me.csdn.net/u012151283)阅读数：804标签：[Tensorflow																[深度学习																[卷积神经网络](https://so.csdn.net/so/search/s.do?q=卷积神经网络&t=blog)
个人分类：[TensorFlow																[深度学习](https://blog.csdn.net/u012151283/article/category/6698461)](https://blog.csdn.net/u012151283/article/category/6698460)





# Agenda
- Convolution support in TF
- More MNIST!!!
- Variable scope
- Autoencoder

# Convolution in TF

```python
tf​.​nn​.​conv2d​(​input​,​ filter​,​ strides​,​ padding​,​ use_cudnn_on_gpu​=​None​,​ data_format​=​None​, name​=​None)

Input​:​ ​Batch​ size x ​Height​ x ​Width​ x ​Channels 
Filter​:​ ​Height​ x ​Width​ x ​Input​ ​Channels​ x ​Output​ ​Channels (​e​.​g​.​ ​[​5​,​ ​5​,​ ​3​,​ ​64​]) 
Strides​:​ ​4​ element ​1​-​D tensor​,​ strides ​in​ each direction (​often ​[​1​,​ ​1​,​ ​1​,​ ​1​]​ ​or​ ​[​1​,​ ​2​,​ ​2​,​ ​1​]) 
Padding​:​ ​'​SAME​'​ ​or​ ​'​VALID' Data_format​:​ ​default​ to NHWC
```

`Filter`就是权重weight，`shape=HWIO`
`bias`的`shape=O`

对于`strides` 参数来说，第一个和第四个一般都是1。 

VALID Padding计算：
```python
out_height = ceil(float(in_height - filter_height + 1) / float(strides[1]))
out_width  = ceil(float(in_width - filter_width + 1) / float(strides[2]))
```

SAME Padding计算：

```python
out_height = ceil(float(in_height)/float(strides[1]))
out_width = ceil(float(in_width)/float(strides[2]))
```

# Conv on MNIST

![这里写图片描述](https://img-blog.csdn.net/20170226131144290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# Variable scope 变量域

因为我们要处理多网络层，下面介绍变量域，类似于命名空间。比如，一个定义在作用域`conv1`中的变量`weights`将变为`conv1-weights`。通常的做法是为每一层创建一个变量域，这样不同层相同名称的变量不会产生命名冲突。 

在变量域中，我们不使用`tf.Variable`创建变量，而用`tf.get_variable`
`tf​.​get_variable​(<​name​>,​ ​<shape>​,​ ​<initializer>)`

如果一个变量在该作用域中已经存在，那么直接使用。否则，创建变量。 

这种机制使得在不用网络架构中共享变量变得容易，可以在一个地方初始化所有的变量。 

在同一个变量域中的结点会组织在一起，因此不再需要使用`name scope`。声明一个变量域，`with tf.variable_scope('conv1') as scope:`

例如：
```python
with​ tf​.​variable_scope​(​'conv1'​)​ ​as​ scope:       
    w ​=​ tf​.​get_variable​(​'weights'​,​ ​[​5​,​ ​5​,​ ​1​,​ ​32​])     
    conv ​=​ tf​.​nn​.​conv2d​(​images​,​ w​,​ strides​=[​1​,​ ​1​,​ ​1​,​ ​1​],​ padding​=​'SAME')     
    conv1 ​=​ tf​.​nn​.​relu​(​conv ​+​ b​,​ name​=​scope​.​name) 
with​ tf​.​variable_scope​(​'conv2'​)​ ​as​ scope:       
    w ​=​ tf​.​get_variable​(​'weights'​,​ ​[​5​,​ ​5​,​ ​32​,​ ​64​])     
    b ​=​ tf​.​get_variable​(​'biases'​,​ ​[​64​],​ initializer​=​tf​.​random_normal_initializer​())     
    conv ​=​ tf​.​nn​.​conv2d​(​conv1​,​ w​,​ strides​=[​1​,​ ​1​,​ ​1​,​ ​1​],​ padding​=​'SAME')     
    conv2 ​=​ tf​.​nn​.​relu​(​conv ​+​ b​,​ name​=​scope​.​name)
```

# Autoencoder](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)](https://so.csdn.net/so/search/s.do?q=Tensorflow&t=blog)




