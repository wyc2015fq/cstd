# 三十七、一张图了解tensorflow中的线性回归工作原理 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 14:01:42[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：649
个人分类：[动手做聊天机器人](https://blog.csdn.net/jiangjingxuan/article/category/6694788)













![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_452_default_big.png)



用梯度下降求解线性回归问题是tensorflow最简单的入门例子（10行关键代码），本节结合上一节讲到的tensorflow中的graph来更加形象地了解tensorflow中的线性回归工作原理

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 10行关键代码实现的线性回归

```python
# -*- coding: utf-8 -*-

import numpy as np
import tensorflow as tf

# 随机生成1000个点，围绕在y=0.1x+0.3的直线周围
num_points = 1000
vectors_set = []
for i in xrange(num_points):
    x1 = np.random.normal(0.0, 0.55)
    y1 = x1 * 0.1 + 0.3 + np.random.normal(0.0, 0.03)
    vectors_set.append([x1, y1])

# 生成一些样本
x_data = [v[0] for v in vectors_set]
y_data = [v[1] for v in vectors_set]


# 生成1维的W矩阵，取值是[-1,1]之间的随机数
W = tf.Variable(tf.random_uniform([1], -1.0, 1.0), name='W')
# 生成1维的b矩阵，初始值是0
b = tf.Variable(tf.zeros([1]), name='b')
# 经过计算得出预估值y
y = W * x_data + b

# 以预估值y和实际值y_data之间的均方误差作为损失
loss = tf.reduce_mean(tf.square(y - y_data), name='loss')
# 采用梯度下降法来优化参数
optimizer = tf.train.GradientDescentOptimizer(0.5)
# 训练的过程就是最小化这个误差值
train = optimizer.minimize(loss, name='train')

sess = tf.Session()
# 输出图结构
#print sess.graph_def

init = tf.initialize_all_variables()
sess.run(init)

# 初始化的W和b是多少
print "W =", sess.run(W), "b =", sess.run(b), "loss =", sess.run(loss)
# 执行20次训练
for step in xrange(20):
    sess.run(train)
    # 输出训练好的W和b
    print "W =", sess.run(W), "b =", sess.run(b), "loss =", sess.run(loss)
# 生成summary文件，用于tensorboard使用
writer = tf.train.SummaryWriter("./tmp", sess.graph)
```



整个收敛过程可以从输出结果中看出：

```
W = [-0.67839384] b = [ 0.] loss = 0.293898
W = [-0.41792655] b = [ 0.30027848] loss = 0.0907883
W = [-0.24463286] b = [ 0.30015084] loss = 0.0407606
W = [-0.12923941] b = [ 0.30006593] loss = 0.0185783
W = [-0.05240078] b = [ 0.30000937] loss = 0.00874262
W = [-0.00123519] b = [ 0.29997173] loss = 0.00438147
W = [ 0.03283515] b = [ 0.29994667] loss = 0.00244773
W = [ 0.05552204] b = [ 0.29992998] loss = 0.00159031
W = [ 0.07062886] b = [ 0.29991883] loss = 0.00121013
W = [ 0.08068825] b = [ 0.29991144] loss = 0.00104155
W = [ 0.08738664] b = [ 0.29990652] loss = 0.000966807
W = [ 0.09184699] b = [ 0.29990324] loss = 0.000933665
W = [ 0.09481706] b = [ 0.29990104] loss = 0.00091897
W = [ 0.09679478] b = [ 0.29989958] loss = 0.000912454
W = [ 0.09811172] b = [ 0.29989862] loss = 0.000909564
W = [ 0.09898864] b = [ 0.29989797] loss = 0.000908283
W = [ 0.09957258] b = [ 0.29989755] loss = 0.000907715
W = [ 0.09996141] b = [ 0.29989725] loss = 0.000907463
W = [ 0.10022032] b = [ 0.29989707] loss = 0.000907352
W = [ 0.10039273] b = [ 0.29989696] loss = 0.000907302
W = [ 0.10050753] b = [ 0.29989687] loss = 0.00090728
```





## 一张图展示线性回归工作原理

执行上面的代码后会在本地生成一个tmp目录，里面会产生用于tensorboard读取的数据，下面我们执行：
`tensorboard --logdir=./tmp/`
打开[http://localhost:6006/](http://localhost:6006/)的GRAPHS，并展开里面的一系列关键节点，如下图所示：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/5871965a86662fe6cb671079a0fd60f213257695.png)

这张图就是上面这段代码生成的graph结构，这个graph描述了整个梯度下降解决线性回归问题的整个过程，每一个节点都代表了代码中的一步操作，我们来具体看一下每一部分的内容



## 详细分析线性回归的graph

首先来看我们要训练的W和b，如下图：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/341265f51bc6cb8d297b5e9a4a2461e3d2c50e83.png)

![](http://www.shareditor.com/uploads/media/my-context/0001/01/eaf5c65eee8a480dafc404615cbfc5997463ea58.png)

我们代码中对W共有三种操作：Assign、read、train。

其中assign是基于random_uniform赋值的，对应着下面这句代码：

```bash
W = tf.Variable(tf.random_uniform([1], -1.0, 1.0), name='W')
```

这里的tf.random_uniform的graph部分如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/03943289c82fd419409a7939c151a71338c7cecd.png)

其中read对应的这句代码：
`y = W * x_data + b`
其中train对应的是梯度下降训练过程的操作

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

我们代码中对b同样也有三种操作：Assign、read、train。与W不同的是它是用zeros赋初始化值的

我们的W和b就是通过梯度下降计算update_W和update_b，从而更新W和b的值的。这里的update_W和update_b都是基于三个输入来计算得出的：学习率learning_rate、W/b当前值、梯度gradients，过程如下图：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/73bfb2bbb6d4ac45b071f1167ca427366bb1b8b2.png)

最关键的梯度下降过程由下面几个过程实现：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/1a0912ec916c7bf10cbee287f597c03de70d0701.png)

下面我们分析一下详细的计算过程，首先看这句代码：

```bash
loss = tf.reduce_mean(tf.square(y - y_data), name='loss')
```

这里的y-y_data对应着这部分：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/863530b489c4887dbbaa3cd1d2957f640c745a6a.png)

那么tf.squre(y-y_data)对应着那条长长的曲线

![](http://www.shareditor.com/uploads/media/my-context/0001/01/a7ba842eb3a6b576c7251e53b750b58589cfea53.png)

下面我们看这里：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/94f99c7bed1c9983740a577b4f968d2d74835a90.png)

这里的右下角是以y-y_data为输入的，但这里的x不是我们的x_data，而是一个临时常量：2。也即是2(y-y_data)，这明显是(y-y_data)^2的导数

继续往上看：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/7fff09bd971e1b9f5a9d28ab57b7c6e9e1bbbf51.png)

这里是以2(y-y_data)为输入经过各种处理最终生成参数b的增量update_b，至此，我们完成了一半了，因为能够更新b了

我们继续往上看：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/b3792e90681e7d2f5dc33a998f742edb4f3dbd01.png)

这里可以生成update_W用来更新W，反向追溯可以看出这个值是依赖于add_grad(也就是基于y-y_data的部分)和W以及y生成的，至此梯度下降整个过程就分析完了，

在这个大图中，详细计算过程如：[http://stackoverflow.com/questions/39580427/how-does-tensorflow-calculate-the-gradients-for-the-tf-train-gradientdescentopti](http://stackoverflow.com/questions/39580427/how-does-tensorflow-calculate-the-gradients-for-the-tf-train-gradientdescentopti)所说，一步简单的操作很普遍的被tensorflow转成了很多个节点的图，所以里面有一些细节的节点就不深入分析，因为只是一些操作的图表达，没有太重要的意义




