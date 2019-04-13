
# 【机器学习】激活函数（Activation Function） - 喜欢打酱油的老鸟 - CSDN博客


2018年08月16日 08:56:03[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：69


[https://blog.csdn.net/ChenVast/article/details/81382795](https://blog.csdn.net/ChenVast/article/details/81382795)
**激活函数是模型整个结构中的非线性扭曲力**
**神经网络的每层都会有一个激活函数**

## 1、逻辑函数（Sigmoid）：
使用范围最广的一类激活函数，具有指数函数形状，它在物理意义上最为接近生物神经元。
其自身的缺陷，最明显的就是饱和性。从函数图可以看到，其两侧导数逐渐趋近于0，杀死梯度。
![](https://img-blog.csdn.net/20171215175024368)
### 函数图像：
![](https://img-blog.csdn.net/20171215175027396)

## 2、正切函数（Tanh）：
非常常见的激活函数。与sigmoid相比，它的输出均值是0，使得其收敛速度要比sigmoid快，减少迭代次数。
相对于sigmoid的好处是他的输出的均值为0，克服了第二点缺点。但是当饱和的时候还是会杀死梯度。
![](https://img-blog.csdn.net/20171215175031284)
### 函数图：
![](https://img-blog.csdn.net/20171215175034807)

## 3、线性整流函数（Rectified Linear Unit，ReLU）：
最近几年比较受欢迎的一个激活函数
无饱和区，收敛快、计算简单、有时候会比较脆弱，如果变量的更新太快，还没有找到最佳值，就进入小于零的分段就会使得梯度变为0，无法更新直接死掉了。
![](https://img-blog.csdn.net/20171215175038298)
### 函数图：
![](https://img-blog.csdn.net/20171215175043365)

## 4、ELU函数（Exponential Linear Unit）：
融合了sigmoid和ReLU，左侧具有软饱和性，右侧无饱和性。
右侧线性部分使得ELU能够缓解梯度消失，而左侧软饱能够让ELU对输入变化或噪声更鲁棒。
因为函数指数项所以计算难度会增加
![](https://img-blog.csdn.net/20171215175049694)
### 函数图：
![](https://img-blog.csdn.net/20171215175056241)

## 5、Softplus函数：
![](https://img-blog.csdn.net/20171215175102280)
### 函数图：
![](https://img-blog.csdn.net/20171215175106843)

## 6、恒同映射（Identity）：
![](https://img-blog.csdn.net/20171215175113178)

## 7、Maxout：
他是ReLU和LReLU的一般化公式（如ReLU就是将w1和b1取为0）。所以他用于ReUL的优点而且没有死区，但是它的参数数量却增加了一倍。
maxout网络能够近似任意连续函数，且当w2,b2,…,wn,bn为0时，退化为ReLU。Maxout能够缓解梯度消失，同时又规避了ReLU神经元死亡的缺点，但增加了参数和计算量。
![](https://img-blog.csdn.net/20171215175117556)

## 8、其他几种激活函数的比较如图：
![](https://img-blog.csdn.net/20171215175124434)
**神经网络之所以为非线性模型的关键，关键在于激活函数。**

