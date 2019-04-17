# 数据载入过慢？这里有一份TensorFlow加速指南 - 知乎
# 



> 王小新 编译自 Towards Data Science
量子位 出品 | 公众号 QbitAI

机器学习算法烂熟于心，网络结构顺手拈来，但是如果数据集载入时耗费大量时间，那整个训练时间就会大大增加。

这个问题可能困扰着很多使用大型数据集训练的炼丹师们。最近，Francesco Zuppichini在medium上的一篇文章，从使用Dataset函数的三个步骤讲起，介绍了相应的解决方法。

以下内容译自他的文章。
![](https://pic4.zhimg.com/v2-3cec992129b45b9352d4a0f74792a18b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='468'></svg>)
看完这篇文章后，千万不要再用默认的输入函数feed-dict了。

本文以TensorFlow 1.5为标准函数库。根据以往经验，在TensorFlow中，**feed-dict函数可能是最慢的一种数据载入方法**，尽量少用。把数据输入到模型的最佳方法是使用输入流水线（input pipeline），来确保GPU无须等待新数据输入。

幸好，TensorFlow有一个内置接口，叫做Dataset。这个接口是为了更容易地实现数据输入，在1.3版本已被提出。这份教程将会介绍如何使用它来创建输入流水线，高效率地将数据输入到模型中。
![](https://pic1.zhimg.com/v2-765347862d8503188a0691fd3cedeecc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='492' height='928'></svg>)
本文还会解释**Dataset接口**的基本原理，包括最常见的一些用法。

所有代码可从这个网址获取：

[https://github.com/FrancescoSaverioZuppichini/Tensorflow-Dataset-Tutorial/blob/master/dataset_tutorial.ipynb](https://link.zhihu.com/?target=https%3A//github.com/FrancescoSaverioZuppichini/Tensorflow-Dataset-Tutorial/blob/master/dataset_tutorial.ipynb)

## **概述**

使用Dataset接口，有以下三个步骤：

**1. 导入数据**，从某些数据创建一个数据集实例；

**2. 创建迭代器iterator**，即使用已有的数据集来创建一个迭代器实例，对数据集进行迭代；

**3. 消耗数据**，即使用所创建的迭代器，从数据集中取出元素输入到模型。

## **导入数据**

首先，我们需要把数据导入到数据集中，有以下几种方式。

## **使用Numpy**

这是常用的一个方法，把一个numpy数组输入到tensorflow中：


```
# create a random vector of shape (100,2)
x = np.random.sample((100,2))
# make a dataset from a numpy array
dataset = tf.data.Dataset.from_tensor_slices(x)
```


我们也可以输入多个numpy数组。典型示例就是我们将一些数据根据特征和标签分类。


```
features, labels = (np.random.sample((100,2)), np.random.sample((100,1)))
dataset = tf.data.Dataset.from_tensor_slices((features,labels))
```


## **使用Tensors**

当然，我们可以用Tensor来初始化数据集：


```
# using a tensor
dataset = tf.data.Dataset.from_tensor_slices(tf.random_uniform([100, 2]))
```


## **使用Placeholder**

当我们需要多次修改Dataset函数中的数据时，这个方法是最合适的，稍后会详细介绍。


```
x = tf.placeholder(tf.float32, shape=[None,2])
dataset = tf.data.Dataset.from_tensor_slices(x)
```


## **使用generator**

我们也可以使用生成器generator来初始化Dataset，在处理长度不同的元素（如序列）时，这种方法很有用：


```
sequence = np.array([[1],[2,3],[3,4]])
def generator():
    for el in sequence:
        yield el
dataset = tf.data.Dataset().from_generator(generator,
                                           output_types=tf.float32, 
                                           output_shapes=[tf.float32])
```


在这种情况下，你还需要指定输入数据的类型和形状，来得到合适的Tensor。

## **创建迭代器**

上面已经介绍了如何创建一个数据集，但是如何拿出里面的数据呢？这里要使用迭代器Iterator，来遍历整个数据集并取出数据的实际值，有以下四种类型。

## **One shot迭代器**

这是最简单的一种迭代器，利用上节的示例一：


```
x = np.random.sample((100,2))
# make a dataset from a numpy array
dataset = tf.data.Dataset.from_tensor_slices(x)
# create the iterator
iter = dataset.make_one_shot_iterator()
```


接着，再调用get_next()函数来获取下一个数据张量：


```
...
# create the iterator
iter = dataset.make_one_shot_iterator()
el = iter.get_next()
```


然后，运行el函数来得到输出值：


```
with tf.Session() as sess:
    print(sess.run(el)) # output: [ 0.42116176  0.40666069]
```


## **可初始化迭代器**

如果要构建一个动态数据集，在运行时要更改其中的源数据，则应该选择占位符placeholder来创建数据集，然后使用经典的feed-dict方法来初始化占位符。这些可以用一个可初始化迭代器来完成，利用上节“使用Placeholder”部分的示例：


```
# using a placeholder
x = tf.placeholder(tf.float32, shape=[None,2])
dataset = tf.data.Dataset.from_tensor_slices(x)

data = np.random.sample((100,2))

iter = dataset.make_initializable_iterator() # create the iterator
el = iter.get_next()
with tf.Session() as sess:
    # feed the placeholder with data
    sess.run(iter.initializer, feed_dict={ x: data }) 
    print(sess.run(el)) # output [ 0.52374458  0.71968478]
```


这里调用了make_initializable_iterator函数。在这个sess范围内，运行initializer函数来传递数据，这里先以随机数组为例。

到这里，我们已经构建好训练集和测试集：


```
train_data = (np.random.sample((100,2)), np.random.sample((100,1)))
test_data = (np.array([[1,2]]), np.array([[0]]))
```


接下来，读入数据来训练模型，并在测试数据集上进行评估，这可通过训练后再次初始化迭代器来完成：


```
# initializable iterator to switch between dataset
EPOCHS = 10

x, y = tf.placeholder(tf.float32, shape=[None,2]), tf.placeholder(tf.float32, shape=[None,1])
dataset = tf.data.Dataset.from_tensor_slices((x, y))

train_data = (np.random.sample((100,2)), np.random.sample((100,1)))
test_data = (np.array([[1,2]]), np.array([[0]]))

iter = dataset.make_initializable_iterator()
features, labels = iter.get_next()

with tf.Session() as sess:
#     initialise iterator with train data
    sess.run(iter.initializer, feed_dict={ x: train_data[0], y: train_data[1]})
    for _ in range(EPOCHS):
        sess.run([features, labels])
#     switch to test data
    sess.run(iter.initializer, feed_dict={ x: test_data[0], y: test_data[1]})
    print(sess.run([features, labels])
```


## **可重初始化迭代器**

这个概念与上个类似，要在数据之间进行动态切换。但是，上面是将新数据输入到同一个数据集中，这里是在数据集之间切换。同样地，我们要构建一个训练数据集和一个测试数据集：


```
# making fake data using numpy
train_data = (np.random.sample((100,2)), np.random.sample((100,1)))
test_data = (np.random.sample((10,2)), np.random.sample((10,1)))
```


我们可以创建两个数据集：


```
# create two datasets, one for training and one for test
train_dataset = tf.data.Dataset.from_tensor_slices(train_data)
test_dataset = tf.data.Dataset.from_tensor_slices(test_data)
```


这里是关键，要构建一个通用型Iterator：


```
# create a iterator of the correct shape and type
iter = tf.data.Iterator.from_structure(train_dataset.output_types,
                                           train_dataset.output_shapes)
```


然后初始化数据集：


```
# create the initialisation operations
train_init_op = iter.make_initializer(train_dataset)
test_init_op = iter.make_initializer(test_dataset)
```


跟上面操作一样，得到下个元素：

`features, labels = iter.get_next()`

现在，可以使用构建的Session来直接运行这两个初始化操作，把这些程序组合起来：


```
# Reinitializable iterator to switch between Datasets
EPOCHS = 10
# making fake data using numpy
train_data = (np.random.sample((100,2)), np.random.sample((100,1)))
test_data = (np.random.sample((10,2)), np.random.sample((10,1)))
# create two datasets, one for training and one for test
train_dataset = tf.data.Dataset.from_tensor_slices(train_data)
test_dataset = tf.data.Dataset.from_tensor_slices(test_data)
# create a iterator of the correct shape and type
iter = tf.data.Iterator.from_structure(train_dataset.output_types,
                                           train_dataset.output_shapes)
features, labels = iter.get_next()
# create the initialisation operations
train_init_op = iter.make_initializer(train_dataset)
test_init_op = iter.make_initializer(test_dataset)
with tf.Session() as sess:
    sess.run(train_init_op) # switch to train dataset
    for _ in range(EPOCHS):
        sess.run([features, labels])
    sess.run(test_init_op) # switch to val dataset
    print(sess.run([features, labels]))
```


## **可馈送迭代器**

在我看来，这些方法可能效果不好，它们基本上不是在数据集之间切换，而是在迭代器之间切换。你可以分别用make_one_shot_iterator函数和make_initializable_iterator函数来创建两个迭代器。

## **消耗数据**

在前面例子中，我们使用过Session来输出数据集中元素next的值：


```
...
next_el = iter.get_next()
...
print(sess.run(next_el)) # will output the current element
```


为了将数据传递给模型，我们只要传递get_next函数生成的张量。

在下面代码段中，有一个包含两个numpy数组的数据集，这里用了第一节的例子。请注意，我们要用.random.sample函数来包装另一个numpy数组以满足数据批量化的维度要求：


```
# using two numpy arrays
features, labels = (np.array([np.random.sample((100,2))]), 
                    np.array([np.random.sample((100,1))]))
dataset = tf.data.Dataset.from_tensor_slices((features,labels)).repeat().batch(BATCH_SIZE)
```


接着，和上面一样，创建一个迭代器：


```
iter = dataset.make_one_shot_iterator()
x, y = iter.get_next()
```


下面，构建一个简单的神经网络模型：


```
# make a simple model
net = tf.layers.dense(x, 8) # pass the first value from iter.get_next() as input
net = tf.layers.dense(net, 8)
prediction = tf.layers.dense(net, 1)
loss = tf.losses.mean_squared_error(prediction, y) # pass the second value from iter.get_net() as label
train_op = tf.train.AdamOptimizer().minimize(loss)
```


我们直接用iter.get_next函数输出的张量作为第一层的输入和损失函数的标签，整理后得到：


```
EPOCHS = 10
BATCH_SIZE = 16
# using two numpy arrays
features, labels = (np.array([np.random.sample((100,2))]), 
                    np.array([np.random.sample((100,1))]))

dataset = tf.data.Dataset.from_tensor_slices((features,labels)).repeat().batch(BATCH_SIZE)

iter = dataset.make_one_shot_iterator()
x, y = iter.get_next()

# make a simple model
net = tf.layers.dense(x, 8, activation=tf.tanh) # pass the first value from iter.get_next() as input
net = tf.layers.dense(net, 8, activation=tf.tanh)
prediction = tf.layers.dense(net, 1, activation=tf.tanh)

loss = tf.losses.mean_squared_error(prediction, y) # pass the second value from iter.get_net() as label
train_op = tf.train.AdamOptimizer().minimize(loss)
with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    for i in range(EPOCHS):
        _, loss_value = sess.run([train_op, loss])
        print("Iter: {}, Loss: {:.4f}".format(i, loss_value))
```


输出为：


```
Iter: 0, Loss: 0.1328 
Iter: 1, Loss: 0.1312 
Iter: 2, Loss: 0.1296 
Iter: 3, Loss: 0.1281 
Iter: 4, Loss: 0.1267 
Iter: 5, Loss: 0.1254 
Iter: 6, Loss: 0.1242 
Iter: 7, Loss: 0.1231 
Iter: 8, Loss: 0.1220 
Iter: 9, Loss: 0.1210
```


## **更多内容**

## **批处理**

通常来说，批处理数据是一件麻烦的事。但是可以用Dataset函数中的批处理方法batch(BATCH_SIZE)，按照设定尺寸来自动批处理数据集，其中默认值为1。在下面示例中，批尺寸设置为4：


```
# BATCHING
BATCH_SIZE = 4
x = np.random.sample((100,2))
# make a dataset from a numpy array
dataset = tf.data.Dataset.from_tensor_slices(x).batch(BATCH_SIZE)
iter = dataset.make_one_shot_iterator()
el = iter.get_next()
with tf.Session() as sess:
    print(sess.run(el))
```


输出：


```
[[ 0.65686128  0.99373963]
 [ 0.69690451  0.32446826]
 [ 0.57148422  0.68688242]
 [ 0.20335116  0.82473219]]
```


## **Repeat操作**

使用repeat函数，可指定数据集中的迭代次数。若没有参数传递，它会一直循环。通常在持续循环后直接用一个标准循环来控制epoch大小。

## **Shuffle操作**

我们可使用shuffle函数来打乱数据集，该函数默认在每个epoch打乱数据集。

打乱数据集，这个操作是非常重要的，可以减弱过拟合效应。

我们也可以设置参数buffer_size，这是shuffle函数的一个内置参数，下个元素将在这个缓冲区中统一选择。下面举例：


```
# BATCHING
BATCH_SIZE = 4
x = np.array([[1],[2],[3],[4]])
# make a dataset from a numpy array
dataset = tf.data.Dataset.from_tensor_slices(x)
dataset = dataset.shuffle(buffer_size=100)
dataset = dataset.batch(BATCH_SIZE)
iter = dataset.make_one_shot_iterator()
el = iter.get_next()
with tf.Session() as sess:
    print(sess.run(el))
```


第一次输出：


```
[[4]
 [2]
 [3]
 [1]]
```


第二次输出：


```
[[3]
 [1]
 [2]
 [4]]
```


可以看出，数字被打乱了。当然，你也可以设置下参数seed看看效果。

## **Map操作**

你还可以使用map方法将自定义函数应用到数据集的每个元素中。在下面示例中，我们把每个元素都乘二：


```
# MAP
x = np.array([[1],[2],[3],[4]])
# make a dataset from a numpy array
dataset = tf.data.Dataset.from_tensor_slices(x)
dataset = dataset.map(lambda x: x*2)

iter = dataset.make_one_shot_iterator()
el = iter.get_next()
with tf.Session() as sess:
#     this will run forever
        for _ in range(len(x)):
            print(sess.run(el))
```


输出：


```
[2]
[4]
[6]
[8]
```


## **结论**

本文介绍的Dataset API给我们提供了一种快速且稳定的方法来创建最佳的输入流水线，以更好地训练、评估和测试网络模型。这篇文章介绍了这个API中的大部分常见操作。更多代码可参见本文对应的jupyter-notebook。
![](https://pic4.zhimg.com/v2-d76638f395ccb21345bd48a0c99a7173_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='565'></svg>)
## **相关链接**

原文：
[https://towardsdatascience.com/how-to-use-dataset-in-tensorflow-c758ef9e4428](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/how-to-use-dataset-in-tensorflow-c758ef9e4428)

本文对应jupyter notebook：
[https://github.com/FrancescoSaverioZuppichini/Tensorflow-Dataset-Tutorial/blob/master/dataset_tutorial.ipynb](https://link.zhihu.com/?target=https%3A//github.com/FrancescoSaverioZuppichini/Tensorflow-Dataset-Tutorial/blob/master/dataset_tutorial.ipynb)

TensorFlow dataset官方教程：
[https://www.tensorflow.org/programmers_guide/datasets](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/programmers_guide/datasets)

Dataset的API文档：
[https://www.tensorflow.org/api_docs/python/tf/data/Dataset](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/api_docs/python/tf/data/Dataset)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


