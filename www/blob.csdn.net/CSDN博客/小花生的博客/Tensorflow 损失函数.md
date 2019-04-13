
# Tensorflow 损失函数 - 小花生的博客 - CSDN博客


2019年01月23日 21:11:44[Peanut_范](https://me.csdn.net/u013841196)阅读数：108


## Tensorflow损失函数
### 1.预备知识：
**Labels：**标签，在分类或者分割等问题中的标准答案。可以是1,2,3,4,5,6 。
**Labels_onehot：**Onehot形式的标签，即如果有3类那么第一类表示为[1,0,0]，第二类为[0,1,0]，第三类为[0,0,1]。这种形式的标签更加的常见。
**Network.out:**网络最后一层的输出，注意是没有经过softmax的网络的输出，通常是softmax函数的输入值。
**Network.probs:**网络输出的概率结果，通常为网络最后一层输出经过softmax函数之后的结果，Network.probs=tf.nn.softmax(Network.out)
**Network.pred:**网络的预测结果，在onehot的形式中选择概率最大的一类作为最终的预测结果，Network.pred=tf.argmax(Network.probs , axis=n)。
**Tensor:**一个张量，可以简单的理解为是tensorflow中的一个数组。
**tf.reduce_sum(Tensor):**降维加和，比如一个数组是3*3*3大小的，那么经过这个操作之后会变为一个数字，即所有元素的加和。
**tf.reduce_mean(Tensor)：**降维平均，和上面的reduce_sum一样，将高维的数组变为一个数，该数是数组中所有元素的均值。
### 2.Tensorflow 分类损失函数（cross_entropy）：
**①Tensor=tf.nn.softmax_cross_entropy_with_logits(logits=Network.out,labels= Labels_onehot)**
**概述：**上面是softmax交叉熵loss，参数为网络最后一层的输出和onehot形式的标签。切记输入一定不要经过softmax，因为在函数中内置了softmax操作，如果再做就是重复使用了。在计算loss的时候，输出Tensor要加上tf.reduce_mean(Tensor)或者tf.reduce_sum(Tensor)，作为tensorflow优化器（optimizer）的输入。
**②Tensor=tf.nn.sparse_softmax_cross_entropy_with_logits(logits=Network.out, labels= Labels)**
**概述：**这个函数和上面的区别就是labels参数应该是没有经过onehot的标签，其余的都是一样的。另外加了sparse的loss还有一个特性就是标签可以出现-1，如果标签是-1代表这个数据不再进行梯度回传。
**③Tensor=tf.nn.sigmoid_cross_entropy_with_logits(logits=Network.out,labels= Labels_onehot)**
**概述：**sigmoid交叉熵loss，与softmax不同的是，该函数首先进行sigmoid操作之后计算交叉熵的损失函数，其余的特性与tf.nn.softmax_cross_entropy_with_logits一致。
**④Tensor=tf.nn.weighted_cross_entropy_with_logits(logits=Network.out,labels=Labels_onehot,  pos_weight=decimal_number)**
**概述：**这个loss与众不同的地方就是加入了一个权重的系数，其余的地方与tf.nn. sigmoid_cross_entropy_with_logits这个损失函数是一致的，加入的pos_weight函数可以适当的增大或者缩小正样本的loss，可以一定程度上解决正负样本数量差距过大的问题。
对比下面两个公式我们可以更加清晰的看到，它和sigmoid的损失函数的区别：
**（1）对于普通的sigmoid来说计算的形式如下：**
**targets * -log(sigmoid(logits)) + (1 - targets) * -log(1 - sigmoid(logits))**
**（2）加入了pos_weight之后的计算形式如下：**
**targets * -log(sigmoid(logits)) * pos_weight + (1 - targets) * -log(1 - sigmoid(logits))**
**3.Tensorflow 回归损失函数（L2 loss）：**
**均方差loss，也叫做L2（2范数）损失函数**
**Tensorlayer中的实现：**
**def mean_squared_error(output, target, is_mean=False, name="mean_squared_error"):
    """Return the TensorFlow expression of mean-square-error (L2) of two batch of data.
    Parameters
    ----------
    output : Tensor
        2D, 3D or 4D tensor i.e. [batch_size, n_feature], [batch_size, height, width] or [batch_size, height, width, channel].
    target : Tensor
        The target distribution, format the same with `output`.
    is_mean : boolean
        Whether compute the mean or sum for each example.
            - If True, use ``tf.reduce_mean`` to compute the loss between one target and predict data.
            - If False, use ``tf.reduce_sum`` (default).
    References
    ------------
    - `Wiki Mean Squared Error <https://en.wikipedia.org/wiki/Mean_squared_error>`__
    """
    with tf.name_scope(name):
        if output.get_shape().ndims == 2:  \# [batch_size, n_feature]
            if is_mean:
                mse = tf.reduce_mean(tf.reduce_mean(tf.squared_difference(output, target), 1))
            else:
                mse = tf.reduce_mean(tf.reduce_sum(tf.squared_difference(output, target), 1))
        elif output.get_shape().ndims == 3:  \# [batch_size, w, h]
            if is_mean:
                mse = tf.reduce_mean(tf.reduce_mean(tf.squared_difference(output, target), [1, 2]))
            else:
                mse = tf.reduce_mean(tf.reduce_sum(tf.squared_difference(output, target), [1, 2]))
        elif output.get_shape().ndims == 4:  \# [batch_size, w, h, c]
            if is_mean:
                mse = tf.reduce_mean(tf.reduce_mean(tf.squared_difference(output, target), [1, 2, 3]))
            else:
                mse = tf.reduce_mean(tf.reduce_sum(tf.squared_difference(output, target), [1, 2, 3]))
        else:
            raise Exception("Unknow dimension")
        return mse****参考：**
**作者：Liu-Kevin**
**原文：****[https://blog.csdn.net/limiyudianzi/article/details/80693695](https://blog.csdn.net/limiyudianzi/article/details/80693695)**

