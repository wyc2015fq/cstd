# Deeply-Learned Part-Aligned Representations for Person Re-Identification 论文笔记 - wxf19940618的博客 - CSDN博客





2018年11月01日 16:18:19[rxdffz](https://me.csdn.net/wxf19940618)阅读数：126










一、提出问题

本文主要针对行人重识别过程中的身体部位错位匹配导致识别精度有限的问题进行研究。

由图1-1可以看出行人姿势多变，人体空间分布鲁棒性低，而且现有空间分割算法建立在人体空间分布大致相同的假设下，但这种假设并不总符合实际情况。

![](https://img-blog.csdnimg.cn/20181101161754679.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图1-1 身体部位对齐的必要性

二、论文贡献
- 提出了一种深度神经网络方法，它联合身体部位特征提取和特征计算，无需标记有关人体部位的信息，而是通过端到端方式最大化重识别精度用以学习模型的参数。与空间划分相比，本文的方法采取人体部分划分，因此对于人体姿势变化和边界框中的各种人体空间分布更加鲁棒。
- 实证结果表明，本文的方法在标准数据集Market -1501，CUHK03，CUHK01和VIPeR上有着优越的表现。
- 模型结构

       设训练数据为![](https://img-blog.csdnimg.cn/20181101161754601.png)，构造一个三元组![](https://img-blog.csdnimg.cn/20181101161754664.png)，![](https://img-blog.csdnimg.cn/20181101161754670.png)是正样本对（同一个人的图像），![](https://img-blog.csdnimg.cn/20181101161754682.png)是负样本对（不同人的图像）。三元组的损失函数如下：

![](https://img-blog.csdnimg.cn/20181101161754674.png)

       m是负样本对之间的距离大于正样本对之间距离的余量，本文将m设置为0.2。![](https://img-blog.csdnimg.cn/20181101161754711.png)是欧几里德距离，![](https://img-blog.csdnimg.cn/20181101161754742.png)是折页损失函数（hinge loss）。 h(I)是图像I提取出来的特征，因此整个损失函数如下：

![](https://img-blog.csdnimg.cn/20181101161754762.png)

3.1 人体部位对齐表示
- 输入：行人图像I
- 模型：全卷积神经网络（FCN）+部位网络（part net）
- 输出：行人的各部位的联合特征f



部位网络（part net）

![](https://img-blog.csdnimg.cn/20181101161754779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)
- 输入：经过全卷积网络输出的行人特征图
- 中间层：K个分支，每个分支包括一个部位图检测器（类似卷积）、全局池化层、降维层，K个分支生成的部位特征连接成一个特征向量，经过L2正则化得到最终行人表示
- 输出：行人特征表示h(I)

设三维张量T表示从FCN输出的图像特征图，因此![](https://img-blog.csdnimg.cn/20181101161754774.png)表示位置(x,y)上的第c个响应。部位特征图检测器可以估计2维特征图Mk，其中mk(x,y)从图像特征图T的指示位置(x,y)位于第k区域中的程度：

![](https://img-blog.csdnimg.cn/20181101161754784.png)

       通过加权策略计算第k个区域的部分特征映射Tk为：

![](https://img-blog.csdnimg.cn/20181101161754799.png)

接着是平均池化算子，f(¯)k= AvePooling(Tk)，其中![](https://img-blog.csdnimg.cn/20181101161754828.png)。 后面连接一个线性降维层（由全连接层实现），将f(¯)k降低到维度特征向量![](https://img-blog.csdnimg.cn/20181101161754837.png)。最后，将所有部位特征连接起来：

![](https://img-blog.csdnimg.cn/20181101161754857.png)

       最后，进行L2归一化，输出行人表示h(I)。

![](https://img-blog.csdnimg.cn/20181101161754916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

3.2 优化器

通过最小化公式2中的三元组损失函数的和来优化网络参数，参数用θ表示。梯度计算如下：

![](https://img-blog.csdnimg.cn/20181101161754892.png)

![](https://img-blog.csdnimg.cn/20181101161754887.png)

因此，梯度函数可以转化为：

![](https://img-blog.csdnimg.cn/20181101161754912.png)

       其中αn是取决于当前网络参数的权重向量，并计算如下：

![](https://img-blog.csdnimg.cn/20181101161754930.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

       公式7表明三元组损失的梯度计算与一元分类损失的梯度一样。因此，在SGD（随机梯度下降）的每次迭代中，选择一小批（M个）样本而不是对三元组的子集进行采样：前向传播的一次通过以计算每个样本的表示h（In），计算 在小批量上的权重αn，计算梯度![](https://img-blog.csdnimg.cn/20181101161754978.png)，最后在聚合小批量样本的梯度。 直接选择一个三元组通常会导致包含更多（超过M个）样本，因此计算量比小批量采样更大。

四、实验结果

在各个数据集上选择8通道

![](https://img-blog.csdnimg.cn/201811011617557.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018110116175518.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018110116175527.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018110116175596.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



