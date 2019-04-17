# Part-Aligned Bilinear Representations for Person Re-identification 论文笔记 - wxf19940618的博客 - CSDN博客





2018年11月01日 16:20:39[rxdffz](https://me.csdn.net/wxf19940618)阅读数：99








一、提出问题

行人身体各部位的对齐对于行人重识别问题是非常重要的，但实际上行人重识别的图像数据绝大多数是非对齐的，所以如何处理这种非对准的情况是本论文研究的重点。

二、论文贡献
- 提出一个基于身体部位对齐的双流网络模型，该模型可以将人体姿势表示为部分特征图，并将它们直接与外观整体特征图结合以计算对齐的行人部位表示；
- 训练网络不需要对行人重识别数据集进行任何身体部位注释。相反，本文使用预训练的权重简单地初始化部位特征图生成流，而预训练的权重是从标准姿势估计数据集训练得到的；
- 通过大量实验验证了本文方法提高了基线的准确性，并在标准图像数据集Market-1501，CUHK03，CUHK01和DukeMTMC以及一个标准视频数据集MARS上展现了优越的性能。

三、模型结构

![](https://img-blog.csdnimg.cn/20181101162015788.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-1 模型结构图

输入：图像I

输出：部位对齐的特征表示f

模型包括一个双流网络和一个聚合模块，双流网络包含两个单独的子网络，即外观图提取器*A*和部分图提取器*P*，它们分别提取外观图A和部位图P. 通过双线性池聚合两种类型的映射以生成部分对齐的特征f，随后将其归一化以生成最终特征向量f(~)

3.1 双流网络

外观图生成器：用Googlenet构成并初始化外观图生成器，生成的外观图为A：

![](https://img-blog.csdnimg.cn/20181101162015758.png)

部分图生成器：用姿态估计网络的子网络OpenPose构成和初始化部分图生成器，生成的部分图为P：

![](https://img-blog.csdnimg.cn/20181101162015785.png)

![](https://img-blog.csdnimg.cn/20181101162015912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/201811011620165.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018110116201620.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图 3-2 外观图与部位图可视化

3.2 双线性池化

![](https://img-blog.csdnimg.cn/20181101162015944.png)是位置点(x,y)在外观图A上的外观描述，![](https://img-blog.csdnimg.cn/2018110116201625.png)是位置点(x,y)在部分图P上的部分描述，经过双线性变换和空间全局池化得到部位对齐的特征表示：

![](https://img-blog.csdnimg.cn/2018110116201623.png)

Vec()为平均池化，可以将矩阵转换为向量，⊗表示两个向量的外积，输出为矩阵。

部位对齐的特征表示经过L2正则化可以归一化为：

![](https://img-blog.csdnimg.cn/2018110116201629.png)

联立公式(3)、(4)，得到：

![](https://img-blog.csdnimg.cn/2018110116201682.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

3.3 loss函数

损失函数采用三元损失函数进行训练，对于一个人体分别使用Iq表示查询数据，Ip表示正样本数据，In表示负样本数据。(Iq,Ip)表示是一个相同身份的人，而(Iq,In)表示不同身份的人。对于样本分别定义为：f(~)q，f(~)p和f(~)n。三元损失可以定义为：

![](https://img-blog.csdnimg.cn/2018110116201699.png)

![](https://img-blog.csdnimg.cn/20181101162016110.png)

四、实验结果

![](https://img-blog.csdnimg.cn/20181101162016176.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181101162016161.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181101162016152.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181101162016181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181101162016258.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



