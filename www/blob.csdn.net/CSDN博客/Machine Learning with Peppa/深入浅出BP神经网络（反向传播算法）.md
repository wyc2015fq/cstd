# 深入浅出BP神经网络（反向传播算法） - Machine Learning with Peppa - CSDN博客





2018年01月10日 20:01:13[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：430
所属专栏：[机器学习与数据挖掘](https://blog.csdn.net/column/details/18961.html)[有趣的算法](https://blog.csdn.net/column/details/19022.html)









       反向传播算法（Backpropagation）是目前用来训练人工神经网络（Artificial Neural Network，ANN）的最常用且最有效的算法。其主要思想是：

（1）将训练集数据输入到ANN的输入层，经过隐藏层，最后达到输出层并输出结果，这是ANN的前向传播过程；

（2）由于ANN的输出结果与实际结果有误差，则计算估计值与实际值之间的误差，并将该误差从输出层向隐藏层反向传播，直至传播到输入层；

（3）在反向传播的过程中，根据误差调整各种参数的值；不断迭代上述过程，直至收敛。






反向传播算法的思想比较容易理解，但具体的公式则要一步步推导，因此本文着重介绍公式的推导过程。







# **1. 变量定义**

![](https://img-blog.csdn.net/20160401202509000)





上图是一个三层人工神经网络，layer1至layer3分别是输入层、隐藏层和输出层。如图，先定义一些变量：

![](https://img-blog.csdn.net/20160401202738142)表示第![](https://img-blog.csdn.net/20160401202834876)层的第![](https://img-blog.csdn.net/20160401202850705)个神经元连接到第![](https://img-blog.csdn.net/20160401202901392)层的第![](https://img-blog.csdn.net/20160401202912017)个神经元的权重；

![](https://img-blog.csdn.net/20160401202925814)表示第![](https://img-blog.csdn.net/20160401202901392)层的第![](https://img-blog.csdn.net/20160401202912017)个神经元的偏置；

![](https://img-blog.csdn.net/20160401202949221)表示第![](https://img-blog.csdn.net/20160401202901392)层的第![](https://img-blog.csdn.net/20160401202912017)个神经元的输入，即：

![](https://img-blog.csdn.net/20160401203046815)

![](https://img-blog.csdn.net/20160401203055737)表示第![](https://img-blog.csdn.net/20160401202901392)层的第![](https://img-blog.csdn.net/20160401202912017)个神经元的输出，即：

![](https://img-blog.csdn.net/20160401203135690)

其中![](https://img-blog.csdn.net/20160401203156534)表示激活函数。




# **2. 代价函数**

 
       代价函数被用来计算ANN输出值与实际值之间的误差。常用的代价函数是二次代价函数（Quadratic cost function）：

![](https://img-blog.csdn.net/20160401203231972)




其中，![](https://img-blog.csdn.net/20160401203246065)表示输入的样本，![](https://img-blog.csdn.net/20160401203254284)表示实际的分类，![](https://img-blog.csdn.net/20160401203304753)表示预测的输出，![](https://img-blog.csdn.net/20160401203313706)表示神经网络的最大层数。




# **3. 公式及其推导**

 
       本节将介绍反向传播算法用到的4个公式，并进行推导。**如果不想了解公式推导过程，请直接看第4节的算法步骤。**

首先，将第![](https://img-blog.csdn.net/20160401202901392)层第![](https://img-blog.csdn.net/20160401202912017)个神经元中产生的错误（即实际值与预测值之间的误差）定义为：

![](https://img-blog.csdn.net/20160401203433129)







 
       本文将以一个输入样本为例进行说明，此时代价函数表示为：

![](https://img-blog.csdn.net/20160401203458879)







**公式1（计算最后一层神经网络产生的错误）：**



![](https://img-blog.csdn.net/20160401203521035)





其中，![](https://img-blog.csdn.net/20160401203529707)表示Hadamard乘积，用于矩阵或向量之间点对点的乘法运算。公式1的推导过程如下：

![](https://img-blog.csdn.net/20160401203552879)








**公式2（由后往前，计算每一层神经网络产生的错误）：**



![](https://img-blog.csdn.net/20160401203604942)





推导过程：

![](https://img-blog.csdn.net/20161230105754424)







**公式3（计算权重的梯度）：**



![](https://img-blog.csdn.net/20160401203624973)





推导过程：

![](https://img-blog.csdn.net/20160401203634161)








**公式4（计算偏置的梯度）：**



![](https://img-blog.csdn.net/20160401203641989)





推导过程：

![](https://img-blog.csdn.net/20160401203656833)








# **4. 反向传播算法伪代码**




- **输入训练集**


- **对于训练集中的每个样本x，设置输入层（Input layer）对应的激活值![](https://img-blog.csdn.net/20160401203715973)：**- **前向传播：**


![](https://img-blog.csdn.net/20160401203739849)， ![](https://img-blog.csdn.net/20160401203806927)

- - **计算输出层产生的错误：**


![](https://img-blog.csdn.net/20160401203820380)

- - **反向传播错误：**


![](https://img-blog.csdn.net/20160401203833583)




- **使用梯度下降（gradient descent），训练参数：**



![](https://img-blog.csdn.net/20160401203848037)





![](https://img-blog.csdn.net/20160401203859349)











