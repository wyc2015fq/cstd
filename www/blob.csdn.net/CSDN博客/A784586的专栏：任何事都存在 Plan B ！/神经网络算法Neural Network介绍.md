# 神经网络算法Neural Network介绍 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月25日 20:04:26[QuJack](https://me.csdn.net/A784586)阅读数：967








**神经网络算法Neural Network介绍**

1. 背景:

     1.1 以人脑中的神经网络为启发，历史上出现过很多不同版本


     1.2 最著名的算法是1980年的** backpropagation **




2. 多层向前神经网络(Multilayer Feed-Forward Neural Network)

     2.1 Backpropagation被使用在多层向前神经网络上

     2.2 多层向前神经网络由以下部分组成：

输入层(input layer), 隐藏层 (hidden layers), 输入层 (output layers)




![](https://img-blog.csdn.net/20170425193340909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


     2.3 每层由单元(units)组成


     2.4 输入层(input layer)是由**训练集的实例特征向量**传入

     2.5 经过连接结点的**权重(weight)**传入下一层，一层的输出是下一层的输入

     2.6 隐藏层的个数可以是任意的，输入层有一层，输出层有一层

     2.7 每个单元(unit)也可以被称作**神经结点**，根据生物学来源定义

     2.8 以上成为**2层的神经网络（输入层不算）**

     2.8 一层中加权的求和，然后根据非线性方程转化输出

     2.9 作为多层向前神经网络，理论上，如果有**足够多的隐藏层**(hidden layers) 和**足够大的训练集**, 可以模     

          拟出任何方程

3. 设计神经网络结构

     3.1 使用神经网络训练数据之前，必须确定**神经网络的层数**，以及**每层单元的个数**

     3.2 特征向量在被传入输入层时通常被**先标准化**(normalize）到0和1之间 （为了加速学习过程）

     3.3 离散型变量可以被编码成每一个输入单元对应一个特征值可能赋的值

          比如：特征值A可能取三个值（a0, a1, a2), 可以使用**3个输入单元来代表A。**

                    如果A=a0, 那么代表a0的单元值就取1, 其他取0；

                    如果A=a1, 那么代表a1的单元值就取1，其他取0，以此类推

     3.4 神经网络即可以用来做**分类(classification）问题**，也可以解决**回归(regression)问题**

          3.4.1 对于分类问题，如果是2类，可以用**一个输出单元**表示（0和1分别代表2类）

                                         如果多余2类，每一个类别用一个输出单元表示

                   所以输出层的单元数量通常等于类别的数量

          3.4.2 没有明确的规则来设计最好有多少个隐藏层

                    3.4.2.1 根据实验测试和误差，以及准确度来实验并改进



4. 交叉验证方法(Cross-Validation)：K-fold
 cross validation 

![](https://img-blog.csdn.net/20170425194836912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






5. Backpropagation算法

     5.1 通过迭代性的来处理训练集中的实例

     5.2 对比经过神经网络后**输入层预测值**(predicted value)与**真实值**(target value)之间

     5.3 反方向（从输出层=>隐藏层=>输入层）来**以最小化误差(error)来更新每个连接的权重(weight)**

     5.4 算法详细介绍

           输入：D：数据集，l 学习率(learning rate)， 一个多层前向神经网络

           输出：一个训练好的神经网络(a trained neural network)

          5.4.1 初始化**权重(weights)和偏向(bias):** 随机初始化在-1到1之间，或者-0.5到0.5之间，每个单元有          

                    一个偏向

          5.4.2 对于每一个训练实例X，执行以下步骤：

                    5.4.2.1： 由输入层向前传送

![](https://img-blog.csdn.net/20170425193340909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170425195535282?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170425195541697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170425195602682?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170425195758793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




6. **Backpropagation 算法举例【计算一遍，加深理解BP算法】**
![](https://img-blog.csdn.net/20170425200048342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170425200203103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








