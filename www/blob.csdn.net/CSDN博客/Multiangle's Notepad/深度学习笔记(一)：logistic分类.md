
# 深度学习笔记(一)：logistic分类 - Multiangle's Notepad - CSDN博客


2016年09月16日 11:28:49[multiangle](https://me.csdn.net/u014595019)阅读数：19922标签：[深度学习																](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)[神经网络																](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[logistic																](https://so.csdn.net/so/search/s.do?q=logistic&t=blog)[分类																](https://so.csdn.net/so/search/s.do?q=分类&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=logistic&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=算法&t=blog)个人分类：[机器学习&深度学习																](https://blog.csdn.net/u014595019/article/category/3051069)[算法																](https://blog.csdn.net/u014595019/article/category/6163865)[
							](https://blog.csdn.net/u014595019/article/category/3051069)
所属专栏：[深度学习&tensorflow笔记](https://blog.csdn.net/column/details/13414.html)
[
																	](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)

[深度学习笔记(一)：logistic分类](http://blog.csdn.net/u014595019/article/details/52554582)
[深度学习笔记(二)：简单神经网络，后向传播算法及实现](http://blog.csdn.net/u014595019/article/details/52557623)
[深度学习笔记(三)：激活函数和损失函数](http://blog.csdn.net/u014595019/article/details/52562159)
[深度学习笔记：优化方法总结(BGD,SGD,Momentum,AdaGrad,RMSProp,Adam)](http://blog.csdn.net/u014595019/article/details/52989301)
[深度学习笔记(四)：循环神经网络的概念，结构和代码注释](http://blog.csdn.net/u014595019/article/details/52571966)
[深度学习笔记(五)：LSTM](http://blog.csdn.net/u014595019/article/details/52605693)
[深度学习笔记(六)：Encoder-Decoder模型和Attention模型](http://blog.csdn.net/u014595019/article/details/52826423)
---
这个系列主要记录我在学习各个深度学习算法时候的笔记，因为之前已经学过大概的概念，所以这轮学习**比较着重于公式推导和具体实现**，而对概念上的描述不多，因此**比较适合对此有一定基础的同学**。
在正式开始写深度学习的知识之前，会有两节传统神经网络的内容，因为深度学习中大量运用了以往神经网络的知识。搞懂传统的神经网络如何工作是很有必要的，有助于对之后的学习打下坚实的基础。
---


# 1. logistic分类
几乎所有的教材都是从logistic分类开始的，因为logistic分类实在太经典，而且是神经网络的基本组成部分，每个神经元(cell)都可以看做是进行了一次logistic分类。
所谓logistic分类，顾名思义，逻辑分类，是一种二分类法，能将数据分成0和1两类。
logistic分类的流程比较简单，主要有线性求和，sigmoid函数激活，计算误差，修正参数这4个步骤。前两部用于判断，后两步用于修正。本文分为3部分，前2部分讲普通logistic分类的流程，第三部分则稍作扩展。
---


## 1.1 线性求和以及sigmoid函数
第1,2步是用于根据输入来判断分类的，所以放在一起说。假设有一个n维的输入列向量$\mathbf{x}$，也有一个n维的参数列向量$\mathbf{h}$， 还有一个偏置量b， 那么就可以线性求和得到z.

$$
z = \mathbf{h}^T\mathbf{x} + b
$$
此时因为z的值域是$[-\infty,+\infty]$，是无法根据z来判断$\mathbf{x}$到底是属于0还是1的。因此我们需要一个函数，来将z的值映射到[0,1]之间， 这就是**激活函数**。激活函数有很多种，这里的激活函数是sigmoid函数。

$$
\sigma(x)=\frac{1}{1+e^{-x}}\\
 \sigma'(x)=\sigma(x)(1-\sigma(x))
$$
其形状为
![这里写图片描述](https://upload.wikimedia.org/wikipedia/commons/thumb/8/88/Logistic-curve.svg/320px-Logistic-curve.svg.png)[ ](https://upload.wikimedia.org/wikipedia/commons/thumb/8/88/Logistic-curve.svg/320px-Logistic-curve.svg.png)
图1 sigmoid函数[ ](https://upload.wikimedia.org/wikipedia/commons/thumb/8/88/Logistic-curve.svg/320px-Logistic-curve.svg.png)
可以看到x越大，$\sigma(x)$越接近1，反之，则越接近0. 那么在判断的时候，我们首先对之前得到的z代入sigmoid函数

$$
a = \sigma(z)=\sigma(\mathbf{h}^T\mathbf{x} + b)
$$
当 a 大于0.5的时候，我们判定x应属于1类，如果小于0.5，则属于0类。这样，就完成了判断的工作
---


## 1.2 误差计算以及参数修正
上面完成的判断过程中用到了参数向量h和偏置量b。 可以说，h和b的值直接关系到logistic判断的准确性。那么这两组参数是如何获得的呢？这就涉及到了参数的修正。**在最开始的时候，h中的值是随机的，而b的值是0**. 我们通过不断的训练来使得h和b能够尽可能的达到一个较优的值。
那么如何训练呢？假设我们期望输入x的判定是y，而实际得到的判定值是a，那么我们定义一个损失函数C(a,y)，通过修正h和b的值来使得C最小化，这是一个优化问题。在凸优化问题中，可以通过

$$
\frac{\partial{C}}{\partial{\mathbf{h}}}=0 , \frac{\partial{C}}{\partial{b}}=0
$$
来直接算得h和b的最优解。然而在某些情况下，例如数据规模很大，或者非凸优化问题中，则不能这么做，而是用迭代的方法来得到局部最优解。

$$
\mathbf{h} := \mathbf{h} - \eta\frac{\partial{C}}{\partial{\mathbf{h}}}\\
 b := b - \eta\frac{\partial{C}}{\partial{b}}
$$
其中$\eta$表示学习率。在这里，我们把损失函数定为平方损失函数，即$C=\frac{1}{2}(a-y)^2$那么可以得到

$$
\begin{align}
 \frac{\partial{C}}{\partial{\mathbf{h}}} 
 =&C'\frac{\partial{a}}{\partial{\mathbf{h}}}\\
 =&(a-y)\frac{\partial{\sigma(z)}}{\partial{\mathbf{h}}}\\
 =&(a-y)\sigma'\mathbf{x}\\
 =&(a-y) a (1-a)\mathbf{x}
 \end{align}
$$

$$
\frac{\partial{C}}{\partial{b}} = (a-y)a(1-a)
$$
这样，就能够得到每次迭代的参数更新公式为

$$
\mathbf{h}:=\mathbf{h}-\eta(a-y)a(1-a))\mathbf{x} \\
b:=b-\eta(a-y)a(1-a))
$$

---


## 1.3 将logistic扩展到多分类
从之前可以看出，普通的logistic只能进行二分类，即只能够分为0或者1。那么如果这些样本属于多个类该怎么办呢？人们想了很多办法，例如一对多法，依次把某个类别的样本归为一类,其他剩余的样本归为另一类，这样k个类需要构建k个分类器。还有一对一法，在任意两类样本之间设计一个分类器，k个类需要k(k-1)/2个分类器。
在这里，**我们将输出由一个值更改为一个向量**。例如有3个类，那么输出就是一个长度为3 的列向量，对应项的值为1，其他为0.即

$$
\begin{bmatrix}1\\ 0\\ 0 \end{bmatrix} \;
 \begin{bmatrix}0\\ 1\\ 0 \end{bmatrix} \;
 \begin{bmatrix}0\\ 0\\ 1 \end{bmatrix}
$$
分别表示第0,1,2个类。 也可以看成是原来若干个logistic分类器组合在一起。对应的某个分类器只对该类输出1，其他情况都输出0.从这一点上来讲，这个做法有点类似于一对多法。此时，由于输出从一个数成为一个向量，**之前的公式都要加以修改**。首先，原来的y,a,z,b变成了列向量， 向量$\mathbf{h}$变成了矩阵W。这样，判断部分的公式变为

$$
\mathbf{z}=W\mathbf{x}+\mathbf{b}\\
 \mathbf{a}=\sigma(\mathbf{z})\\
$$
此时的$\sigma$函数表示对向量中的每一个元素单独做运算。即

$$
\sigma(\mathbf{x})=
 \begin{bmatrix}
 \sigma(x_1)\\
 \sigma(x_2)\\
 \vdots\\
 \sigma(x_n)
 \end{bmatrix}
$$
得到的a向量中，其最大值所在的位置索引即为判断出的分类。
参数修正部分的公式也是类似的，

$$
\frac{\partial{C}}{\partial{W}}=
(\mathbf a-\mathbf y).\times\mathbf a.\times(1-\mathbf a) \times \mathbf x^T\\
\frac{\partial{C}}{\partial{\mathbf b}}=
(\mathbf a-\mathbf y).\times\mathbf a.\times(1-\mathbf a)
$$
注意有些向量之间是进行点乘的。

