# AlexNet 讲解及pytorch实现  ----1 AlexNet主要技术突破点 - 墨流觞的博客 - CSDN博客





2018年11月04日 16:50:03[墨氲](https://me.csdn.net/dss_dssssd)阅读数：821








## 一. AlexNet网络结构

2012年，该网络ILSVRC-2012图像分类的冠军，top-5的识别错误率为15.3%， 比第二名高出10个百分点。
- 下面是论文中的网络结构：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104152905324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
原始网络将模型分为两部分，分开在两个GPU上训练，与下面合并的网络结构等价：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104155911260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)- 各部分网络结构的参数及输入输出图像大小计算：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104160029515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
## 二. AlexNet的意义及技术优势

### 1. 在神经网络罗发展历史上的重要意义
- 证明了CNN在复杂模型下的有效性
- 使用GPU训练可以在可接受的时间内得到结果

以上两点推动了深层网络结构的构建以及采用GPU的加速训练方法 。

### 2. 技术上引进新的思想
- 
激活函数使用Relu， 不再使用sigmoid和tanh函数，其优势在于**收敛速度更快，使得训练时间更短**， 已成为卷积神经网络最常用的激活函数。[https://blog.csdn.net/NOT_GUY/article/details/78749509](https://blog.csdn.net/NOT_GUY/article/details/78749509)

函数形式： $f(x) = max(0, x)$

函数形状如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104160852604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
但是Relu函数最大的缺点是，`Dead ReLU Problem`（神经元坏死现象）：某些神经元可能永远不会被激活，导致相应参数永远不会被更新（在负数部分，梯度为0）

导致如此的原因有两个：
- 
参数初始化问题: 采用Xavier的变体`He initialization`,思想是保证输入和输出方差相同，故参数服从均值为0，方差为$\frac{2}{输入神经元个数}$的正态分布 。以及BN层的使用，简单的说就是在线性变化和非线性激活函数之间，将数值做一次高斯归一化和线性变化

- 
learning  rate太高导致在训练过程中参数更新太大: 设置晓得学习率以及再用adagrad， adam等自动调整学习率的优化算法

- 
局部响应归一化层（Local Response Normalization Layer）

LRN层只存在于第一层卷积层和第二层卷积层的激活函数后面，引入这一层		的主要目的，主要是为了防止过拟合，增加模型的泛化能力.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104162943763.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104163018632.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

但是对于这种方法对于算法的优化程度存在争议，后期的网络结构基本不再采用这种方法。
- 
采用重叠的最大池化层，来“稍微”减轻过拟合

kernal_size = 3*3, s=2 ，通过提取3*3小矩形框中的最大值来提取局部特征，减少冗余信息。

传统的卷积层中，相邻的池化单元是不重叠的。比如`stride>=kernel_size`，而如果`stride<kernel_size`，将使用重叠的池化层。
### 3. 论文减轻过拟合的方法
- 
数据集扩增

大部分算法过拟合的原因是数据集数量不够，通过翻转，裁剪等方法来增加数据集的数量

- 
采用Dropout，来减轻过拟合

实际类似于模型集合的方式，在全连接层中使用Dropout，比如设概率为0.5，则每个隐藏层神经元的输入以0.5的概率输出为0。输出为0的神经元相当于从网络中去除，不参与前向计算和反向传播。所以对于每次输入，神经网络都会使用不同的结构。注意在测试时需要将Dropout层去掉。

- 
使用权重衰减的损失函数优化算法
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104164242570.png)
$\epsilon$为学习率，$w_i$为第$i$训练的权重

接来下分析pytorch的AlexNet实现，注意AlexNet没有使用LRN层。

博客引用：

1… Xavier 初始化参数的推导
[http://andyljones.tumblr.com/post/110998971763/an-explanation-of-xavier-initialization](http://andyljones.tumblr.com/post/110998971763/an-explanation-of-xavier-initialization)

4. 参数初始化讨论[https://blog.liexing.me/2017/10/24/deep-learning-weight-initialization/](https://blog.liexing.me/2017/10/24/deep-learning-weight-initialization/)

3.激活函数讨论：[https://blog.csdn.net/NOT_GUY/article/details/78749509](https://blog.csdn.net/NOT_GUY/article/details/78749509)

5. LRN的探讨
[https://blog.csdn.net/hduxiejun/article/details/70570086](https://blog.csdn.net/hduxiejun/article/details/70570086)
[http://yeephycho.github.io/2016/08/03/Normalizations-in-neural-networks/](http://yeephycho.github.io/2016/08/03/Normalizations-in-neural-networks/)














