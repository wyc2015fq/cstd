# coursera-斯坦福-机器学习-吴恩达-第4周笔记-神经网络 - 博客堂 - CSDN博客





2017年11月30日 16:11:00[最小森林](https://me.csdn.net/u012052268)阅读数：8311
所属专栏：[斯坦福-吴恩达《机器学习》之路](https://blog.csdn.net/column/details/18191.html)









# coursera-斯坦福-机器学习-吴恩达-第4周笔记-神经网络



- [coursera-斯坦福-机器学习-吴恩达-第4周笔记-神经网络](#coursera-斯坦福-机器学习-吴恩达-第4周笔记-神经网络)- [提出神经网络的动机](#1-提出神经网络的动机)
- [神经网络算法](#2-神经网络算法)- [1 神经元](#21-神经元)
- [2 神经网络](#22-神经网络)

- [应用](#3-应用)- [1 例子1 and与or运算](#31-例子1-and与or运算)
- [2 例子2](#32-例子2)
- [3 多分类](#33-多分类)

- [复习作业](#4-复习作业)- [1测验](#41测验)
- [2编程作业-多分类](#42编程作业-多分类)- [21多分类](#421多分类)
- [22 Neural Networks](#422-neural-networks)







## 1 提出神经网络的动机

前面我们学习了，线性回归、逻辑回归，他们可以很好的解决一些预测问题。但是面对一些多特征的问题，比如以下的情况，他们并不能很好的画出分类边界线。

![](http://oqy7bjehk.bkt.clouddn.com/17-11-29/51984824.jpg)

这种时候需要用到多项式回归（非线性的），这种函数画出的曲线可以有任意角度。但是这种函数会因为特征量的增多导致二次项数的剧增。 比如在图像识别中，一个50×50像素的图片，拥有的特征量为2500，那么它的二次项数为2500×2500/2,大约为3百万个。

在这种情况下，神经网络在1970左右被提出。

## 2 神经网络算法

### 2.1 神经元

神经网络是有一个个的神经元组成的网络。那么什么是“神经元”？ 
![](http://oqy7bjehk.bkt.clouddn.com/17-11-29/2034250.jpg)

如图，这是一个最简单的神经元，模型的输入是x1,x2,x3通过参数（权重）θ1,θ2…，并使用逻辑函数激活（压缩），得到输出结果。
### 2.2 神经网络

下图为一个三层神经网络模型。第一层为输入层，第二层为隐藏层，第三层为输出层。 每条边上有一个权值θ。 
![image](https://img-blog.csdn.net/20170501171249892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
$a^{(j)}_i$：第j层单元i的“激励” 

$\theta^{(j)}$：第j层到第j+1层单元的权值矩阵。 

若第j层单元数为$s_j$，第j+1层单元数为$s_{j+1}$，则$\theta^{(j)}\epsilon s_{j+1}*(s_j+1)$。

+1来自于“偏置节点”的$\Theta^{(j)}$中的加法$x_0$和$\Theta_0^{(j)}$。

比如: If layer 1 has 2 input nodes and layer 2 has 4 activation nodes. Dimension of Θ(1) is going to be 4×3 where sj=2 and sj+1=4, so sj+1×(sj+1)=4×3.

![image](https://img-blog.csdn.net/20170501172859575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在bp神经网络中，我们使用sigmoid函数作为激励函数。即$g(z) = \frac{1}{1+e^{-z}}$，也就是我们逻辑回归中使用的函数。

记住：$z^{(j)} = \Theta^{(j-1)}a^{(j-1)}$`然后：`$a^{(j)} = g(z^{(j)})$

## 3 应用

### 3.1 例子1 and与or运算

我们前面一直说，神经网络可以计算复杂的分线性函数，那么到底是怎么计算的呢下面通过小例子一步步说明。

看图： 
![image](https://img-blog.csdn.net/20170501174032887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

X取0或1，并给定权重。此时这个神经元就表示“and”运算。
而图片![image](https://d3c33hcgiwev3.cloudfront.net/imageAssetProxy.v1/f_ueJLGnEea3qApInhZCFg_a5ff8edc62c9a09900eae075e8502e34_Screenshot-2016-11-23-10.03.48.png?expiry=1512086400000&hmac=Paswv_EoxTK5j4BP4Pyn7uWYoSPMFBS0p9Zn6Pymmuc)

X取0或1，并给定权重。此时这个神经元就表示“OR”运算。
同样的还可以表示not a and not b： 
![](http://oqy7bjehk.bkt.clouddn.com/17-11-29/78357732.jpg)
### 3.2 例子2

我们把上面三个式子总结一下： 
![](http://oqy7bjehk.bkt.clouddn.com/17-11-29/60161896.jpg)
这三个式子可以合起来，组成一个复杂的表达式。

![](http://oqy7bjehk.bkt.clouddn.com/17-11-29/71217634.jpg)

其中不同的颜色表示，从不同的神经元取得的参数。

这样，窥一斑而知全豹。我们举一反三，可以理解：

> 
通过组合不同计算功能的神经元，我们可以使神经网络表示复杂的表达式。—— GREAT TANG


### 3.3 多分类

为了将数据分类到多个类中，我们让我们的假设函数返回值的向量。假设我们想将我们的数据分为四类中的一类。

我们将使用下面的例子来看看这个分类是如何完成的。该算法将图像作为输入并相应地进行分类：

![image](https://d3c33hcgiwev3.cloudfront.net/imageAssetProxy.v1/9Aeo6bGtEea4MxKdJPaTxA_4febc7ec9ac9dd0e4309bd1778171d36_Screenshot-2016-11-23-10.49.05.png?expiry=1512086400000&hmac=iYPHe1ZTYo5ESfqYJGzgQWco5a-fBk7M5eLBdWTzuOw)

我们可以定义我们的结果类为y： 
![image](https://d3c33hcgiwev3.cloudfront.net/imageAssetProxy.v1/KBpHLXqiEealOA67wFuqoQ_95654ff11df1261d935ab00553d724e5_Screenshot-2016-09-14-10.38.27.png?expiry=1512086400000&hmac=rfKHbbCBDbZomvlTzWvmRXwwik3pz8C0ID4c0WoKyMQ)

每个y（i）代表对应于汽车，行人，卡车或摩托车的不同图像。内层为我们提供了一些新的信息，这些信息导致了我们最终的假设功能。设置看起来像：
![image](https://d3c33hcgiwev3.cloudfront.net/imageAssetProxy.v1/VBxpV7GvEeamBAoLccicqA_3e7f67888330b131426ecffd27936f61_Screenshot-2016-11-23-10.59.19.png?expiry=1512086400000&hmac=HCoozVLpSb6w1Qw2Opn-d1cxqlqJEy77jYxwQ3Lc2sQ)

后面我们会学习如何计算。

这里有个小作业 
![](http://oqy7bjehk.bkt.clouddn.com/17-11-29/37351381.jpg) 60= 10×（5+1）
## 4 复习作业

### 4.1测验
- 
Which of the following statements are true? Check all that apply.答案：BD
- Suppose you have a multi-class classification problem with three classes, trained with a 3 layer network. Let a(3)1=(hΘ(x))1 be the activation of the first output unit, and similarly a(3)2=(hΘ(x))2 and a(3)3=(hΘ(x))3. Then for any input x, it must be the case that a(3)1+a(3)2+a(3)3=1.（不对，因为每个a都是由前面的节点计算而来，同层之间的节点互不影响，只有加上softmax才有影响）
- The activation values of the hidden units in a neural network, with the sigmoid activation function applied at every layer, are always in the range (0, 1).
- A two layer (one input layer, one output layer; no hidden layer) neural network can represent the XOR function.（得不同的网络叠加）
- Any logical function over binary-valued (0 or 1) inputs x1 and x2 can be (approximately) represented using some neural network.

- 
Consider the following neural network which takes two binary-valued inputs x1,x2∈{0,1} and outputs hΘ(x). Which of the following logical functions does it (approximately) compute? -20 ，30，30答案：A
- OR
- and
- NAND (meaning “NOT AND”)
- XOR (exclusive OR)

- 
Consider the neural network given below. Which of the following equations correctly computes the activation a(3)1? Note: g(z) is the sigmoid activation function. 答案：A 
![](http://oqy7bjehk.bkt.clouddn.com/17-11-30/13968911.jpg)- 
You have the following neural network:


You’d like to compute the activations of the hidden layer a(2)∈R3. One way to do so is the following Octave code:

You want to have a vectorized implementation of this (i.e., one that does not use for loops). Which of the following implementations correctly compute a(2)? Check all that apply.答案：A
- a2 = sigmoid (Theta1 * x);（Theta1是个3×的矩阵，x是个列向量）
- a2 = sigmoid (x * Theta1);
- a2 = sigmoid (Theta2 * x);
- 
z = sigmoid(x); a2 = Theta1 * z;
- You are using the neural network pictured below and have learned the parameters Θ(1)=[110.51.21.92.7] (used to compute a(2)) and Θ(2)=[1−0.2−1.7] (used to compute a(3)} as a function of a(2)). Suppose you swap the parameters for the first hidden layer between its two units so Θ(1)=[111.20.52.71.9] and also swap the output layer so Θ(2)=[1−1.7−0.2]. How will this change the value of the output hΘ(x)?答案：A

- 
It will stay the same.

- It will increase.
- It will decrease
- Insufficient information to tell: it may increase or decrease.

### 4.2编程作业-多分类

在这个练习中，你将实现一对多 逻辑回归和神经网络来识别手写数字。 在开始编程练习之前，我们强烈建议您观看视频讲座并完成相关主题的复习问题。

要开始练习，您需要下载[初学者代码](https://s3.amazonaws.com/spark-public/ml/exercises/on-demand/machine-learning-ex3.zip)并将其内容解压到您希望完成练习的目录。 如果需要，在开始本练习之前，使用Octave / MATLAB中的cd命令切换到此目录。

下面是各个文件的作用

```
ex3.m - Octave / MATLAB脚本，指导您完成第1部分
ex3 nn.m - Octave / MATLAB脚本，指导您完成第2部分
ex3data1.mat - 手写数字的训练集
ex3weights.mat - 神经网络练习的初始权重
submit.m - 提交您的解决方案到我们的服务器的提交脚本
displayData.m - 帮助可视化数据集的功能
fmincg.m - 函数最小化例程（类似于fminunc）
sigmoid.m - 乙状结肠功能
-lrCostFunction.m - Logistic回归成本函数
-oneVsAll.m - 训练一对多多分类器
-predictOneVsAll.m - 使用一个对所有多类分类器进行预测
-predict.m神经网络预测函数
```

#### 4.2.1多分类

对于这个练习，你将使用逻辑回归和神经网络来识别手写数字（从0到9）。 自动手写数字识别在今天被广泛使用，从识别邮件信封上的邮政编码（邮政编码）到识别银行支票上的金额。 本练习将向您展示如何将您学习的方法用于此分类任务。

在练习的第一部分，您将扩展先前的逻辑回归实现，并将其应用于“one-vs-all”分类。

程序已经写好载入数据，我们修改lrCostFunction.m来计算代价函数 和梯度。

```
sigm = sigmoid(X*theta); 
J = -1/m * (y'*log(sigm) + (1-y')*log(1-sigm)) + lambda/2/m*sigm;

grad(1,:) = 1/m * (X(:,1)' * (sigm -y)); 
grad(1:end,:) = 1/m * (X(:,2:end)'*(sigm - y)) +lambda/m*theta(2:end, :);
```

下面修改 

oneVsAll.m 实现一对多分类。在里面加入

```
initial_theta = zeros(n+1, 1);
options = optimset('GradObj', 'on', 'MaxIter', 50);
for c=1:num_labels
    all_theta(c, :) = fmincg(@(t)(lrCostFunction(t, X, (y==c), lambda)), initial_theta, options);
end
```

下面进行预测，修改 predictOneVsAll.m添加

```
temp = all_theta * X';
[maxx, pp] = max(temp);
p = pp';
```

最后精度Training Set Accuracy: 94.880000

#### 4.2.2 Neural Networks

在本练习的前一部分中，您实现了多类逻辑回归来识别手写数字。 然而，逻辑回归不能形成更复杂的假设，因为它只是一个线性分类器。

在练习的这一部分，您将实现一个神经网络，使用与之前相同的训练集来识别手写数字。 神经网络将能够表示形成非线性假设的复杂模型。 本周，您将使用我们已经训练过的神经网络的参数。 您的目标是实现前馈传播算法以使用我们的权重进行预测。 在下周的练习中，您将编写用于学习神经网络参数的反向传播算法。

提供的脚本ex3 nn.m将帮助您逐步完成此练习。

首先修改predict.m来写

```
X = [ones(m, 1) X];
XX = sigmoid(X*Theta1');
pp = sigmoid([ones(size(XX, 1), 1) XX] * Theta2');
[a, p] = max(pp, [], 2);
```

最终预测准确性：Training Set Accuracy: 97.520000













