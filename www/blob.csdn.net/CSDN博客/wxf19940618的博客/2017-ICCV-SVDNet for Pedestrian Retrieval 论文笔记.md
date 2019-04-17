# 2017-ICCV-SVDNet for Pedestrian Retrieval 论文笔记 - wxf19940618的博客 - CSDN博客





2018年12月13日 14:48:39[rxdffz](https://me.csdn.net/wxf19940618)阅读数：12








**SVDNet for Pedestrian Retrieval ****论文笔记**

一、提出问题

卷积神经网络在进行分类时全连接层的权重向量是高度相关的，如图1-1，训练数据中的粉色和红色两个高度相关的向量会影响对测试数据的表示（粉色和红色造成了冗余），这可能最终导致粉色和红色向量投影主导欧几里德距离，并导致较差的结果。

![](https://img-blog.csdnimg.cn/20181213144315497.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图1-1 相关向量表示及其负面影响

造成这个问题的两个原因分别是：
- 训练样本的非均匀分布
- 在CNN训练期间很少学习正交化约束

二、论文贡献
- 提出了SVDNet，其特征在于包含去相关权重向量的FC层；
- 提出一种新颖的三步训练方案。在第一步中，权重矩阵经历奇异向量分解（SVD）并且被左酉矩阵和奇异值矩阵的乘积代替。其次，保持正交化权重矩阵固定，并仅对剩余层进行微调。第三，权重矩阵是不固定的，并且网络被训练用于整体优化。迭代这三个步骤以近似权重矩阵的正交性。

三、论文方法

![](https://img-blog.csdnimg.cn/20181213144315479.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-1 模型结构

3. 1 模型结构
- 基础骨干网络（如CaffeNet或ResNet-50）+Eigenlayer（本征层）作为倒数第二个FC层
- 将Eigenlayer定位在倒数第二个FC层而不是最后一个FC层的原因是：当在最后一个FC层上强制执行正交性时，模型将无法收敛，这可能是由于最后一个FC层中权重向量的相关性由训练样本分布决定。
- 本征层包含正交权重矩阵，并且是没有偏置的线性层。
- 不用偏置的原因是：偏置会破坏学习的正交性，实验表明添加ReLU激活和偏置项会略微影响re-ID的性能。

在测试时，可以用Eigenlayer的输入特征或者输出的特征作为测试的特征表示，两者结果相差不大，之所以Eigenlayer的输入特征也有正交特性是由于训练时的BP反向传播导致的。

3. 2 训练策略

约束+松弛迭代（RRI），训练过程见图3-2。
- Step 0：首先在基础网络中添加一个线性层。 然后对网络进行微调直到收敛，在步骤0之后，线性层中的权重向量仍然高度相关。
- Step 1：去相关。在权重矩阵上执行SVD，如下所示：

![](https://img-blog.csdnimg.cn/20181213144315451.png)

其中，W是线性层的权重矩阵，U是左酉矩阵，S是奇异值矩阵，V是右酉矩阵。分解后，用US代替W. 然后线性层使用![](https://img-blog.csdnimg.cn/20181213144315469.png)的所有特征向量作为权重向量，并命名为Eigenlayer。
- Step 2：约束。基础主干模型经过微调直到收敛，但是Eigenlayer是固定的。
- Step 3：松弛。在Eigenlayer不固定时，迭代更多次继续微调模型。

在步骤1和步骤2之后，权重向量是正交的，即处于本征状态。但是在步骤3之后，即松弛训练，W偏离本征状态，因此训练时进行 “约束和松弛”的迭代t次![](https://img-blog.csdnimg.cn/20181213144315475.png)。

![](https://img-blog.csdnimg.cn/20181213144315494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-2 SVDNet 训练过程
- 机制研究

1、为什么使用SVD？
- 主要想法是根据CNN已经从训练集学到的东西中找到一组正交投影方向。 对于线性层，寻找W的值空间中的一组基（即由W的列向量张成的线性子空间）是一个潜在可行的方案，但是实际上存在多组正交基。因此，本文决定使用W的奇异向量作为新的投影方向，并用相应的奇异值对投影结果进行加权，即用US代替![](https://img-blog.csdnimg.cn/20181213144315492.png)。这样做可以保持特征表示在整个样本空间上的可分辨能力。
- 数学证明：给定两个图像![](https://img-blog.csdnimg.cn/20181213144315489.png)和![](https://img-blog.csdnimg.cn/20181213144315497.png)，![](https://img-blog.csdnimg.cn/20181213144315503.png)和![](https://img-blog.csdnimg.cn/20181213144315505.png)表示两图像在Eigenlayer前的对应特征，![](https://img-blog.csdnimg.cn/20181213144315511.png)和![](https://img-blog.csdnimg.cn/20181213144315517.png)是Eigenlayer的输出特征。![](https://img-blog.csdnimg.cn/20181213144315546.png)和![](https://img-blog.csdnimg.cn/20181213144315535.png)特征之间的欧几里德距离![](https://img-blog.csdnimg.cn/20181213144315534.png)为：

![](https://img-blog.csdnimg.cn/20181213144315547.png)

![](https://img-blog.csdnimg.cn/20181213144315559.png)

公式3表明当US代替![](https://img-blog.csdnimg.cn/20181213144315559.png)时，![](https://img-blog.csdnimg.cn/20181213144315567.png)保持不变，因此，训练第一步微调CNN模型的辨别能力（re-ID精度）保持100％。虽然还有其他去相关化的方法，但是其他方法都无法保持re-ID精度，比较结果见Table1。

![](https://img-blog.csdnimg.cn/20181213144315580.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

2、性能改善是在哪一步发生的？
- 训练算法中的第1步用US替换![](https://img-blog.csdnimg.cn/20181213144315595.png)不会立即提高精度，只能保持精度不变。而训练算法中的步骤2和步骤3可以进一步提高精度，这两步的主要作用是提高本征层的输入特征和输出特征的辨别能力。一方面，约束步骤学习了本征层的上游和下游层，仍然保留了正交性质，另一方面，松弛步骤将使模型再次偏离正交性，但它更接近收敛，经过多次迭代后，整体性能得到改善。

3、如何评估向量的相关性？
- 可以通过相关系数来估计两个矢量之间的相关性，但是相关系数缺乏用于判断向量集的整体相关性的评估协议。本文建议评估整体相关性如下：
- 给定权重矩阵W，将W的格拉姆矩阵定义为:

![](https://img-blog.csdnimg.cn/20181213144315596.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

其中k是W中权重向量的数量, ![](https://img-blog.csdnimg.cn/20181213144315592.png)是W中的元素，![](https://img-blog.csdnimg.cn/20181213144315608.png)是W中的权重向量。给定W，将S（·）定义为度量W的所有列向量之间相关程度的函数：![](https://img-blog.csdnimg.cn/20181213144315626.png)。 由公式（5）可见S(W)的值在![](https://img-blog.csdnimg.cn/20181213144315613.png)之间，当且仅当W是正交矩阵时，S(W)才达到最大值1，当S(W)趋近1/k时，向量相关性强，当S(W)趋近1时，向量相关性弱。

4、什么时候停止RRI？
- 当在松弛步骤之后评估W的正交性时，发现S(W)随着迭代增加，这表明W中权重向量之间的相关性随着RRI逐步降低。因此，当S(W)变得稳定时，模型收敛，就可以停止RRI。



四、实验结果

数据集：Market-1501,，CUHK03，DukeMTMCreID

![](https://img-blog.csdnimg.cn/20181213144315667.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144315753.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144315787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144315796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144315882.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144315858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



