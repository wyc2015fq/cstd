# TensorFlow学习笔记之五——源码分析之最近算法 - SHINING的博客 - CSDN博客
2016年05月23日 16:54:19[snsn1984](https://me.csdn.net/snsn1984)阅读数：15918
所属专栏：[Tensorflow学习](https://blog.csdn.net/column/details/tensorflow.html)
```python
import numpy as np
import tensorflow as tf
# Import MINST data
import input_data
mnist = input_data.read_data_sets("/tmp/data/", one_hot=True)
#这里主要是导入数据，数据通过input_data.py已经下载到/tmp/data/目录之下了，这里下载数据的时候，需要提前用浏览器尝试是否可以打开
#http://yann.lecun.com/exdb/mnist/，如果打不开，下载数据阶段会报错。而且一旦数据下载中断，需要将之前下载的未完成的数据清空，重新
#进行下载，否则会出现CRC Check错误。read_data_sets是input_data.py里面的一个函数，主要是将数据解压之后，放到对应的位置。
# In this example, we limit mnist data
Xtr, Ytr = mnist.train.next_batch(5000) #5000 for training (nn candidates)
Xte, Yte = mnist.test.next_batch(200) #200 for testing
#mnist.train.next_batch，其中train和next_batch都是在input_data.py里定义好的数据项和函数。此处主要是取得一定数量的数据。
# Reshape images to 1D
Xtr = np.reshape(Xtr, newshape=(-1, 28*28))
Xte = np.reshape(Xte, newshape=(-1, 28*28))
#将二维的图像数据一维化，利于后面的相加操作。
# tf Graph Input
xtr = tf.placeholder("float", [None, 784])
xte = tf.placeholder("float", [784])
#设立两个空的类型，并没有给具体的数据。这也是为了基于这两个类型，去实现部分的graph。
# Nearest Neighbor calculation using L1 Distance
# Calculate L1 Distance
distance = tf.reduce_sum(tf.abs(tf.add(xtr, tf.neg(xte))), reduction_indices=1)
# Predict: Get min distance index (Nearest neighbor)
pred = tf.arg_min(distance, 0)
#最近邻居算法，算最近的距离的邻居，并且获取该邻居的下标，这里只是基于空的类型，实现的graph，并未进行真实的计算。
accuracy = 0.
# Initializing the variables
init = tf.initialize_all_variables()
#初始化所有的变量和未分配数值的占位符，这个过程是所有程序中必须做的，否则可能会读出随机数值。
# Launch the graph
with tf.Session() as sess:
    sess.run(init)
    # loop over test data
    for i in range(len(Xte)):
        # Get nearest neighbor
        nn_index = sess.run(pred, feed_dict={xtr: Xtr, xte: Xte[i,:]})
        # Get nearest neighbor class label and compare it to its true label
        print "Test", i, "Prediction:", np.argmax(Ytr[nn_index]), "True Class:", np.argmax(Yte[i])
        # Calculate accuracy
        if np.argmax(Ytr[nn_index]) == np.argmax(Yte[i]):
            accuracy += 1./len(Xte)
    print "Done!"
    print "Accuracy:", accuracy
#for循环迭代计算每一个测试数据的预测值，并且和真正的值进行对比，并计算精确度。该算法比较经典的是不需要提前训练，直接在测试阶段进行识别。
```
源代码地址:[https://github.com/aymericdamien/TensorFlow-Examples/blob/master/examples/2%20-%20Basic%20Classifiers/nearest_neighbor.py](https://github.com/aymericdamien/TensorFlow-Examples/blob/master/examples/2%20-%20Basic%20Classifiers/nearest_neighbor.py)
相关API：
### `tf.reduce_sum(input_tensor, reduction_indices=None, keep_dims=False, name=None)`
Computes the sum of elements across dimensions of a tensor.
Reduces `input_tensor` along the dimensions given in `reduction_indices`. Unless `keep_dims` is true, the rank of the tensor is reduced by 1 for each entry in `reduction_indices`. If `keep_dims` is true, the reduced dimensions are retained with length 1.
If `reduction_indices` has no entries, all dimensions are reduced, and a tensor with a single element is returned.
For example:
```python
# 'x' is [[1, 1, 1]
#         [1, 1, 1]]
tf.reduce_sum(x) ==> 6
tf.reduce_sum(x, 0) ==> [2, 2, 2]
tf.reduce_sum(x, 1) ==> [3, 3]
tf.reduce_sum(x, 1, keep_dims=True) ==> [[3], [3]]
tf.reduce_sum(x, [0, 1]) ==> 6
```
##### Args:
- `input_tensor`: The tensor to reduce. Should have numeric type.
- `reduction_indices`: The dimensions to reduce. If `None` (the default), reduces all dimensions.
- `keep_dims`: If true, retains reduced dimensions with length 1.
- `name`: A name for the operation (optional).
##### Returns:
The reduced tensor.
点评：这个API主要是降维使用，在这个例子中，将测试图片和所有图片相加后的二维矩阵，降为每个图片只有一个最终结果的一维矩阵。
