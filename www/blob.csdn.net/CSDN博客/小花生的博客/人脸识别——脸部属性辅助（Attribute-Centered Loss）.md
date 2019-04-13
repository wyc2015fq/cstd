
# 人脸识别——脸部属性辅助（Attribute-Centered Loss） - 小花生的博客 - CSDN博客


2018年11月20日 23:04:34[Peanut_范](https://me.csdn.net/u013841196)阅读数：124



###### 《Attribute-Centered Loss for Soft-Biometrics Guided Face Sketch-Photo Recognition》
2018，Hadi Kazemi et al. Attribute-Centered Loss
## 引言：
脸部草图（sketches）能够捕捉脸部的空间拓扑结构，同时缺乏一些面部属性，如种族、皮肤或头发颜色。现有的草图照片识别方法大多忽略了人脸属性的重要性。
本文提出了一种新的损失函数——**attribute-centered loss**,**一种在共享嵌入空间中对具有不同属性组合的照片和草图学习几个不同中心的属性输入损失**。去训练Deep Coupled Convolutional Neural Network (DCCNN)面向脸部属性引导的素描与照片匹配。DCCNN同时输入脸部草图和彩色图片。
实验结果在E-PRIP和IIIT-D Semi-forensic 数据集上超过了之前最好的结果。
**注：**face sketch-to-photo identification 在执法、刑事和情报调查中等具有重要应用。
传统的草图识别算法可分为generative（生成）和discriminative（判别）两类。
Generative approaches：在匹配之前将一种模态转移到另一种模态。
Discriminative approaches：执行特征提取，e.g.:SIFT、MLBP。
一些其他方法:学习或提取模态不变特征。
基于深度学习的方法：通过学习两种模态之间的共同潜在嵌入空间来解决跨域人脸识别问题。
**本文的主要贡献：**
1）提出了一种新的人脸属性引导草图照片识别框架。
2）引入了一个新的损失函数，即attribute centered loss。将目击者提供的面部特征与法医草图的几何特征进行融合，以提高性能。
3）提出的损失函数以软方式（soft manner）来使用所提供的属性。
## Methodology
**1.Center Loss**
Softmax Loss （保证类之间的feature距离最大）与 Center Loss （保证类内的feature距离最小，更接近于类中心）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120221041343.png)
给每个label的数据定义一个center，大家要向center靠近，$c_{y_{i}}$表示$y_{i}$类别的特征中心。
c怎么定义呢？
batch训练的时候不断地计算更新它，每一轮计算一下当前数据和center的距离，然后把这个距离梯度叠加到center上：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120221538722.png)
每个batch的数据并不算多，这样更新会不会容易center产生抖动？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120221614182.png)
这个scale肯定是小于1的。
[
](https://img-blog.csdnimg.cn/20181120221614182.png)**2.Proposed loss function：**
由于在大多数可用的草图数据集中，每个标识只有一对草图-照片图像，因此为每个标识分配单独的中心没有好处。然而，在这项工作中，我们分配中心的面部属性的不同组合。中心的数目等于可能的面部属性组合的数目。为了定义我们的以属性为中心的损失，重要的是简要地描述识别网络的总体结构。
**2.1 Network Structure**
本文采用耦合DNN模型来学习这两种模式之间的深层共享潜在子空间特征，i.e., sketch and photo
**网络结构：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120221730848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
第一个网络photo-DCNN，获取一张彩色照片并嵌入到共享的潜在子空间$p_{i}$中；第二个网sketch-attribute-
DCNN，获取草图及其指定的类中心并找到它们的表示形式$s_{i}$。这两个网络应该被训练以找到共享的潜在子空间，使得每个草图及其相关的面部属性的表示尽可能接近其对应的照片，同时仍然保持与其他照片的距离。为此，我们提出并采用属性中心的损失为我们的属性引导共享表示学习。
**2.2 Attribute-Centered Loss**
人脸属性引导的草图照片识别问题，可以考虑不同的面部属性组合作为不同的类。
attribute-centered loss function：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120221901799.png)
其中$L_{attr}$是一种损失，用于最小化共享相似面部属性组合的照片或草图属性对的类内距离；$L_{id}$表示类内可分性的身份损失；$L_{cen}$迫使中心保持彼此之间的距离在嵌入子空间，以更好的类间差异。
**属性损失**
$$
L_{attr}
$$
**被表述为：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120222217569.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
其中，其中$\in_{c}$是margin促进收敛，$p_{i}$是photo-DCNN对输入照片产生的嵌入特征，$y_{i}$表示为属性组合。此外，$s_{i}^{g}$和$s_{i}^{im}$具有与$p_{i}$相同的属性组合但分别具有相同(真对)或不同(冒名顶替者对)身份的两个草图的嵌入特征。属性损失不尝试将样本一路推到中心，但保持它们在中心的边缘有一个半径$\in_{c}$，这给网络提供了灵活性，以便在类内可分离性内学习一个判别特征空间。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120222618739.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**身份损失**
$$
L_{id}
$$
**被定义为：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120222720285.png)
这是一个contrastive loss，$\in_{d}$的margin将同一个身份的照片和草图推向对方。在他们的中心的margin$\in_{c}$，并采取不同身份的照片和草图分开。显然，margin$\in_{d}$应该小于margin$\in_{c}$的两倍,如图2所示。
[
](https://img-blog.csdnimg.cn/20181120222720285.png)**保持中心最小距离，**
$$
L_{cen}
$$
**被制定为：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120223050572.png)
其中，$n_{c}$是centers总数，$c_{j}$和$c_{k}$是第j和k类的中心，$\in_{jk}$是$c_{j}$和$c_{k}$的距离间隔。这个损失最小的距离$\in_{jk}$。在每对中心之间，这两个中心与两个中心$c_{j}$和$c_{k}$之间的矛盾属性的数量有关。
现在，只有少数属性不同的两个中心比具有不同数量的属性的中心更接近。在相似性相关的界限背后的直觉是，目击者可能会误判一两个属性，但它不太可能混淆更多。因此，在测试过程中，与被害人提供的属性相比，最高级别的嫌疑人很可能具有一些相互矛盾的属性。
**2.3 A special case and connection to the data fusion**
所学的公共嵌入空间（Z）包括两个正交子空间。因此，Z可以写成：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120223501561.png)
其中，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120223530319.png)
学习嵌入空间被划分为两个子空间。第一个嵌入子空间表示属性中心，该属性中心提供关于面部属性的信息。
[
](https://img-blog.csdnimg.cn/20181120223530319.png)第二子空间表示主体或它们的身份信息的几何属性。
值得注意的是，所提出的以属性为中心的损耗指导网络在共享潜在表示学习过程中自动融合几何信息和属性信息。在所提出的框架中，sketchattribute-DCNN学习融合输入草图及其对应属性。这种融合过程是网络学习从每个草图属性对到中心附近的映射的必然任务。
在该方案中，草图和n个2分类属性,$a_{i}=1,...,n$,作为（n+1）-通道输入传递给网络。每个属性通道是通过重复分配给该属性的值来构造的。这种融合算法使用属性提供的信息来补偿不能从草图提取的信息（例如头发颜色）或者在绘制草图时丢失的信息。
## 实现细节
**1.网络结构**
Photo和sketch DCNNs网络的基础框架都采用VGG16，融入了batch-nomalization,最后三个全连接层替换成两个卷积层（channels=256、64），最后连接一个全局平均池化，则特征向量为64.
Photo-DCNN输入：RGB图像
Sketch-attribute-DCNN输入：多通道输入，第一输入通道是灰度草图，并且基于该属性在人中的存在或不存在，对于填充有0或1的每个二进制属性存在特定的通道。
**2.数据**
本文选择12个面部属性：black hair, brown hair, blond hair, gray hair, bald, male,Asian, Indian, White, Black, eyeglasses, sunglasses。
将所选属性分类为四个属性类别，hair (5 states), race (4 states), glasses (2 states),和 gender (2 states).对于每个类别，除了性别类别，我们还考虑了任何情况下，对于该类别不存在所提供的属性的额外状态。使用这个属性设置，我们最终有180个中心（属性的不同组合）。由于上述草图数据集中没有一个包括面部属性，所以我们手动标记了所有的数据集。
3.网络训练
对于centers的更新和centerloss：《A discriminative feature learning approach for deep face recognition》一致
由于具有大量可训练参数的深层神经网络容易在相对小的训练数据集上过拟合，因此我们采用了多种增强技术。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120223721832.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**评价**
所提出的算法在probe图像，优选的属性和一个gallery进行识别。在本节中，我们将我们的算法与多属性引导技术以及不使用任何额外信息的技术进行比较。
实验设置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112022375995.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**Rank10的识别准确率：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120223829337.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**总结：**
本文提出了一个新的框架，以解决跨模态人脸识别照片和法医草图的任务。通过引入以属性为中心的损失，训练一个耦合的深层神经网络，以学习几何属性信息和人脸属性信息共同参与相似性得分计算的两种模式之间的共享嵌入空间。
---

###### 注：博众家之所长，集群英之荟萃。

[
](https://img-blog.csdnimg.cn/20181120223829337.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
