# Person Re-identification with Deep Similarity-Guided Graph Neural Network 论文笔记 - wxf19940618的博客 - CSDN博客





2018年11月01日 09:12:49[rxdffz](https://me.csdn.net/wxf19940618)阅读数：217








一、提出问题

行人重识别任务需要稳健地估计人物图像之间的视觉相似性。然而，现有的人物识别模型主要是独立地估计probe和gallery图像的不同图像对的相似性，而忽略了不同probe-gallery图像对之间的关系信息，这就造成了一些困难样本的相似性估计可能不准确。

二、论文贡献
- 提出了一种新的相似性引导图神经网络（SGGNN）用于行人重识别，该模型可以进行端到端的训练。与大多数利用后处理阶段中样本之间的图库间图像关系的现有方法不同，SGGNN在训练阶段中结合了图库间图像关系以增强特征学习过程，可以学习更多的辨别和准确的人物图像特征表示
- 与大多数图形神经网络（GNN）方法不同，SGGNN利用训练标签监督来学习更准确的特征融合权重，以更新节点的特征。 这种相似性引导方式确保特征融合权重更加精确，并进行更合理的特征融合。本文提出的方法的有效性通过对三个行人重识别数据集的广泛实验来验证。

三、模型结构

![](https://img-blog.csdnimg.cn/20181101091233556.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)
- 输入：一个probe图像和多个gallery图像
- 输出：每个probe-gallery图像对的相似性得分

3.1 图公式及节点特征

将行人重识别构造成以节点为中心的图，给定一个probe图像和N个gallery图像，构造无向完全图G(V,E)，其中V = {v1，v2，...，vN}表示节点集。每个节点代表一对probe-gallery图像。目标是估计每个probe-gallery图像对的相似性得分，因此可以将行人重识别问题看作节点分类问题。通常任意probe-gallery图像对之间的复杂关系都可以编码为其对应的节点的输入特征。

![](https://img-blog.csdnimg.cn/20181101091233553.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

**节点输入特征的产生：**
- 输入：probe-gallery图像对
- 中间层：1）Siamese-CNN：基于ResNet-50，用于生成probe和gallery图像的特征；2）逐元素相减：将来自ResNet-50的两个图像的全局平均池化特征图相减；3）逐元素平方：将相减后的特征做平方操作；4）BN层：最后经过一个BN层生成probe-gallery图像对的差异特征
- 输出：probe图像和第i个gallery图像之间差异特征di（i = 1,2，...，N），也就是图网络的第i个节点的输入特征。

由于要进行节点分类，即估计每个probe-gallery图像对的相似性得分，一个简单的方法就是为每个节点的输入提供信息。在不考虑不同节点之间的成对关系的情况下，将特征输入到线性分类器中以输出相似性得分。对于训练mini-batch中的每个个probe-gallery对，可以使用二元交叉熵损失函数:

![](https://img-blog.csdnimg.cn/20181101091233537.png)

       F()表示sigmoid函数后的线性分类器，yi表示第i个probe-gallery图像对的真实标签，其中1表示probe图像与第i个gallery图像属于相同的身份，而0表示不是相同身份。

3.2 相似性引导的图神经网络

图G是完全连接的，E代表不同probe-gallery对之间的关系集，其中Wij是图G的边权重，是一个标量，表示节点i和节点j之间的关系重要性，可以得到：

![](https://img-blog.csdnimg.cn/20181101091233533.png)

       其中gi和gj是第i和第j个gallery图像，S()是成对相似性估计函数，其估计gi和gj之间的相似性得分，并且可以以与上面讨论的原始节点（probe-gallery图像对）分类模型相同的方式建模。注意，在SGGNN中gallery-gallery图像对的相似性得分S(gi,gj)也是采用监督学习方式。将Wii设置为0的目的是避免自我增强。

       为了增强节点的初始成对关系特征与其他节点之间的信息，在所有连接节点之间传递深度学习的特征消息。然后将节点特征更新为所有输入消息和节点的原始特征的加权和融合。具体策略为：用gallery-gallery图像对的相似度得分来指导probe-gallery图像对关系特征的细化，由于涉及到不同对之间的丰富关系信息，因此probe-gallery对的关系特征更具辨别性和准确性。

这一部分的优势在于可以通过简单正样本的学习来指导困难正样本的学习，使得困难正样本学习效果提高，例如：给定一个probe图像p和两个gallery图像gi，gj。假设（p，gi）是困难正样本对（节点），而（p，gj）和（gi，gj）都是相对容易的正样本对。在节点（p，gi）和（p，gj）之间没有任何消息传递的情况下，（p，gi）的相似性得分可能不高。但是如果我们利用对（gi，gj）的相似性来指导困难正样本对（p，gi）的关系特征的细化，（p，gi）的细化特征将产生更高的相似性得分。这种关系特征融合可以认为是消息传递和特征融合的策略。

**深度特征信息的编码**

![](https://img-blog.csdnimg.cn/20181101091233564.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

在进行消息传递开始之前，每个节点首先对深度特征信息进行编码，以便可以发送到与其连接的其他节点。
- 输入：probe图像和第i个gallery图像之间差异特征di（i = 1,2，...，N）
- 中间层：两组全连接层+BN层+ReLU
- 输出：深度消息ti

![](https://img-blog.csdnimg.cn/20181101091233526.png)

       在从每个节点获得边权重Wij和深度消息ti之后，可以将节点关系特征di的更新方案表示为：

![](https://img-blog.csdnimg.cn/20181101091233624.png)

       其中![](https://img-blog.csdnimg.cn/20181101091233641.png)表示第i个精细关系特征，![](https://img-blog.csdnimg.cn/20181101091233640.png)表示第i个输入关系特征，![](https://img-blog.csdnimg.cn/20181101091233669.png)表示来自节点j的深度消息，α表示平衡融合特征和原始特征的加权参数。

       迭代方式如下：

![](https://img-blog.csdnimg.cn/20181101091233676.png)

实际上，多次迭代的迭代特征更新与一次迭代的更新之间的性能差距可以忽略不计。所以采用公式(4)作为训练和测试阶段的关系特征融合。在关联特征更新之后，将probe-gallery图像对的关系特征输入S形函数的线性分类器，以获得相似性得分并训练具有相同的二元交叉熵损失（如公式(1)）。





四、实验结果

![](https://img-blog.csdnimg.cn/20181101091233790.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181101091233807.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181101091233791.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181101091233836.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



