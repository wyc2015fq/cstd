# TensorFlow之九—损失函数 - wsp_1138886114的博客 - CSDN博客





2019年02月19日 09:23:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：123标签：[TensorFlow之九—损失函数](https://so.csdn.net/so/search/s.do?q=TensorFlow之九—损失函数&t=blog)
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)












### 文章目录
- [损失函数（loss）](#loss_1)
- [TensorFlow损失函数](#TensorFlow_4)




### 损失函数（loss）

用来表示预测（y）与已知答案（y_）的差距。在训练神经网络时，通过不断改变神经网络中所有参数，使损失函数不断减小，从而训练出更高准确率的神经网络模型，常用的损失函数有均方误差，自定义和交叉熵等。

### TensorFlow损失函数

TensorFlow内置了四个损失函数：
- Tensor=`tf.nn.softmax_cross_entropy_with_logits(logits= Network.out, labels= Labels_onehot)`
上面是softmax交叉熵loss，参数为网络最后一层的输出和onehot形式的标签。切记输入一定不要经过softmax，因为在函数中内置了softmax操作，如果再做就是重复使用了。在计算loss的时候，输出Tensor要加上tf.reduce_mean(Tensor)或者tf.reduce_sum(Tensor)，作为tensorflow优化器（optimizer）的输入。- Tensor=`tf.nn.sparse_softmax_cross_entropy_with_logits (logits=Network.out, labels= Labels)`
这个函数和上面的区别就是labels参数应该是没有经过onehot的标签，其余的都是一样的。另外加了sparse的loss还有一个特性就是标签可以出现-1，如果标签是-1代表这个数据不再进行梯度回传。- Tensor=`tf.nn.sigmoid_cross_entropy_with_logits (logits= Network.out, labels= Labels_onehot)`
sigmoid交叉熵 loss，与softmax不同的是，该函数首先进行sigmoid操作之后计算交叉熵的损失函数，其余的特性与tf.nn.softmax_cross_entropy_with_logits一致。- Tensor=`tf.nn.weighted_cross_entropy_with_logits (logits=Network.out, labels=Labels_onehot, pos_weight=decimal_number)`
这个loss与众不同的地方就是加入了一个权重的系数，其余的地方与tf.nn. sigmoid_cross_entropy_with_logits这个损失函数是一致的，加入的pos_weight函数可以适当的增大或者缩小正样本的loss，可以一定程度上解决正负样本数量差距过大的问题。对比下面两个公式我们可以更加清晰的看到，他和sigmoid的损失函数的区别，- 普通的sigmoid计算的形式如下：
$\rm targets * -log(sigmoid(logits)) + (1 - targets) * -log(1 - sigmoid(logits))$- 加入了pos_weight之后的计算形式如下：
$\rm targets * -log(sigmoid(logits)) * pos_weight + (1 - targets) * -log(1 - sigmoid(logits))$


