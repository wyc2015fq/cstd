# 对李弘毅的DL讲解PPT的总结 - wydbyxr的博客 - CSDN博客
2017年03月30日 16:22:15[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：771
适合DL初学者 
关于李弘毅的PPT，网上有资源。。
# lecture one：
## DL的三步走：
define function（set net structure）；learning target；best function（learn）
## 1.网络结构决定函数的结构：
z=w1a1+w2a2+…+wnan+b;wn是权重weights，b是偏置bias 
z通过激活函数，得到下一个a
output layer：softmax回归分为两个部分，原有的代价函数、添加的一个权重衰减项（这个衰减项会惩罚过大的参数值，这样就可以保证得到唯一的解了。） 
其实logistic函数也就是经常说的sigmoid函数，它的几何形状也就是一条sigmoid曲线。
怎样决定网络结构（选多少层，层间多少节点）：尝试+直觉
## 2.参数决定不同种的函数，
## 3.最好的function是使loss minimize（调优）
梯度下降法（gradient descent）：w <= w -(学习率)*梯度 ；只能局部最优，且与初始点的选择有关
## 使用keras模型
使用keras模型，这是一个超好用的神经网络框架，我们都知道深度的神经网络，Python一开始有theano这个框架用来写神经网络，不过后来我们发现keras这个比theano更加容易构建 
先吐槽一下这个基于theano的keras有多难装，反正我是在windows下折腾到不行，所以自己装了一个双系统。这才感到Linux系统的强大之初，难怪大公司都是用这个做开发 
（keras、sequential） 
keras的中文文档：[http://keras-cn.readthedocs.io/en/latest/getting_started/sequential_model/](http://keras-cn.readthedocs.io/en/latest/getting_started/sequential_model/)
# lecture two：（选参、调参的技巧）
具体实现是keras模型
## 1.choosing loss function
常使用均方根误差MSE
## 2.mini batch（分批处理；Mini Batch是原始数据集中的子集，这个子集是在每次训练迭代时抽取抽取的样本，而通常方法需see all），提速、更好精度但不稳定
Mini Batch K-Means，这个基于K-Means的变种聚类算法应运而生。 
大数据量是什么量级？通过当样本量大于1万做聚类时，就需要考虑选用Mini Batch K-Means算法。Mini Batch KMeans是一种能尽量保持聚类准确性下但能大幅度降低计算时间的聚类模型。 
使用了一个种叫做Mini Batch（分批处理）的方法对数据点之间的距离进行计算。Mini Batch的好处是计算过程中不必使用所有的数据样本，而是从不同类别的样本中抽取一部分样本来代表各自类型进行计算。
## 3.activation function（激活函数）
多次迭代可造成梯度消失；过去使用RBM预训练；现在ReLU、maxout等传输函数代替了sigmoid
## 4.可调节的学习率
目的是更快地使loss更小,其实就是曲线的斜率。 
基于经验的手动调整。 通过尝试不同的固定学习率，如0.1, 0.01, 0.001等，观察迭代次数和loss的变化关系，找到loss下降最快关系对应的学习率。 
基于策略的调整。 fixed 、exponential、polynomial；自适应动态调整。adadelta、adagrad、ftrl、momentum、rmsprop、 
主要是sgd、adam；以上都是优化算法 
太大: loss爆炸；太小：收敛不了 
另外一提，sgd算法实际应用中可能收敛不了，故设最大迭代次数。其中?0是初始学习率, ?τ是最后一次迭代的学习率. τ自然代表迭代次数.一般来说,?τ 设为?0的1%比较合适.而τ一般设为让训练集中的每个数据都输入模型上百次比较合适.那么初始学习率?0怎么设置呢?书上说,你先用固定的学习速率迭代100次,找出效果最好的学习速率,然后?0设为比它大一点就可以了.
## 5.动量
其实是斜率的变化率。用来限制4.学习率的，当然与mini batch也有关。 
可使迭代次数更少即更快 
特点: 前后梯度方向一致时,能够加速学习 ；前后梯度方向不一致时,能够抑制震荡。 
需要:学习速率 ?, 初始参数 θ, 初始速率v, 动量衰减参数α  
每步迭代过程: 2.计算梯度和误差,并更新速度v和参数θ: 
为防止过拟合：6 7 8 9，他们也都是正则化的一种方式
## 6.提前停止
即限制最大迭代次数，不然在测试集的效果会越来越差
## 7.权重衰退
类似大脑神经的用进废退，即w的系数
## 8.丢弃神经节点，dropout
训练过程：在每一次更新参数之前丢弃，每个节点都有p%的概率丢弃，故每次训练的网络结构不同。 
    对于每次mini-batch，都要重采样丢弃的神经 
测试过程：用整个神经网络，不需要dropout。测试的w =（ 1 - 丢弃率 ）* 训练的w 
keras实现：model.add（dropout=0.8） 
训练时使用dropout会降低accuracy，但会提高测试时的accuracy
## 9.网状结构
CNN是好的例子,见后文。
# lecture three：（不同的神经网络）
## 一、CNN：
相似的局部图像使用同样的参数
1. 
缩小并不能改变图像，但可以让参数更少 
结构是卷积层-》池化层（max pooling）； 
卷积层：卷积使图像更小。对于边界，使用zero padding来补零。对彩色图像，其实就是3通道、3模帮、3种核。有无这一层对参数数量影响非常大，因为这里使用了权值共享。卷积核的权值的梯度就可以用BP算法来计算。需要上采样upsample 
使用非线性激活函数 
池化层（下采样层）：对卷积后的图像，取模板中的max。alpha Go没有使用这一层 
这两步过后，得到了new image
2. 
flatten（变平）变成一维的数据。 
通过一维数据，不需要全连接(部分链接)就可以复原，但卷积可以使参数更少
再通过全连接前馈网络（BP即是），最后输出结果
## 二、RNN
在语言和语义中较经常使用 
1.LSTM见笔记本
2.有时候很难训练。权重的微小变化可能导致极大后果，故可以使用LSTM解决梯度消失（前提是forget gate处于open状态），另外GRU比之更简单。 
特点之一是输入和输出长度（数量）一致，当然这在它的拓展中可以不一致
3.BPTT（Back Propagation Through Time）算法用来优化。因为整个模型在每个steps都共享参数parameters，计算gradient的时候不只需要计算当前step的值，还要包含前面所有step的gradient的值
4.应用广泛（主要是和语义相关，one to many…）
# lecture four：（未来的发展）
## 不同模型：
1.supervised（监督，管理） learning 
ulter（极） deep network包括：highway network，见笔记本；attention model
2.reinforcement（强化） learning 
随迭代的进行，收益会越来越高。
3.无监督学习 
image，可以用来生成图像，画漫画 
text，理解word的意思 
audio：学习人类的语言
另外,alpha go是1+2
作者  [wu](www.baidu.com)
