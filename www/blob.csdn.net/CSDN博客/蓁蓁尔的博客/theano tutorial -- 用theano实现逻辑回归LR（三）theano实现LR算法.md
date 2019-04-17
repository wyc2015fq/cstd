# theano tutorial -- 用theano实现逻辑回归LR（三）theano实现LR算法 - 蓁蓁尔的博客 - CSDN博客





2017年01月07日 13:46:07[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：292








```python
>>> import numpy
>>> import theano
>>> import theano.tensor as T
>>> rng = numpy.random

>>> N = 400                                   # 训练数据的数量 400个样本
>>> feats = 784                               # 特征数 784,每一个样本有784个特征

# 生成训练数据: D = ((N, feates), N个随机数值) ，随机数是0或者1
>>> D = (rng.randn(N, feats), rng.randint(size=N, low=0, high=2))
>>> training_steps = 10000

# 定义两个符号变量，x和y，其中x是一个double的matrix，y是一个double的vector
>>> x = T.dmatrix("x")
>>> y = T.dvector("y")

# 随机初始化参数w，它的大小是feats，向量
#
# 我们把w定义为共享变量，这样可以在多次迭代中共享。
>>> w = theano.shared(rng.randn(feats), name="w")

# b也是共享变量，我们不需要随机初始化，一般bias出初始化为0就行了。
>>> b = theano.shared(0., name="b")

>>> print("Initial model:")
>>> print(w.get_value()) #  得到共享变量的值
>>> print(b.get_value())

# 构造Theano表达式图

>>> p_1 = 1 / (1 + T.exp(-T.dot(x, w) - b))   
# 模型输出1的概率，因为x是一个矩阵，所以p_1是一个列向量，一次输出的是N个样本

>>> prediction = p_1 > 0.5                   
# 基于p_1预测分类，p_1>0.5则prediction 为1，否则为0。prediction 是一个只含有（0,1）两个值得向量

>>> xent = -y * T.log(p_1) - (1-y) * T.log(1-p_1) # Cross-entropy loss function
>>> cost = xent.mean() + 0.01 * (w ** 2).sum()
# loss函数，前面xent是一个向量，所以求mean，然后使用L2 正则化，w越大就惩罚越大

>>> gw, gb = T.grad(cost, [w, b])             # 计算cost对w和b的梯度


'''train是一个函数，它的输入是x和y，输出是分类预测prediction和xent，注意updates参数，因为w和b试共享变量，早就定义好了，
所以不用放到输入列表中。每次调用train函数之后都会更新w<-w-0.1*gw, b<-b-0.1*gb'''

>>> train = theano.function(
          inputs=[x,y],
          outputs=[prediction, xent],
          updates=((w, w - 0.1 * gw), (b, b - 0.1 * gb)))

# pridict是一个函数，输入x，输出prediction
>>> predict = theano.function(inputs=[x], outputs=prediction)

# 训练，就是用训练数据x=D[0], y=D[1]进行训练。
'''也就算调用train函数，train函数会使用当前的w和b“前向”计算出prediction和xent，同时也计算出cost对w和b的梯度。
然后再根据updates参数更新w和b'''
>>> for i in range(training_steps):
    pred, err = train(D[0], D[1])

>>> print("Final model:")
>>> print(w.get_value())
>>> print(b.get_value())
>>> print("target values for D:")
>>> print(D[1])
>>> print("prediction on D:")
>>> print(predict(D[0]))
```

### 我们可以看到，在Theano里，我们实现一个模型非常简单，我们需要如下步骤：

#### 1. 把输入和输出定义成符号变量【有时为了加速我们可能也会把它定义成共享变量从而让它放到gpu的显存里】

#### 2. 把模型的参数定义成共享变量

#### 3. 然后写出loss函数的公式，同时定义loss对参数的梯度

#### 4. 定义一个训练函数train，输入是模型的输入变量，输出是loss【或者prediction，这样我们可以在训练的时候打印出预测的准确率】，updates用来更新模型参数

#### 5. 写有一个for循环不停的调用train

### 当然这是全量的梯度下降，如果是batch的随机梯度下降，只需要每次循环传入一个batch的输入和输出就行。

#### 详见：[http://geek.csdn.net/news/detail/131362](http://geek.csdn.net/news/detail/131362)



