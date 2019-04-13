
# 多模态(RGB-D)——CIMDL - 小花生的博客 - CSDN博客


2018年10月04日 18:35:18[Peanut_范](https://me.csdn.net/u013841196)阅读数：280


《Correlated and Individual Multi-Modal Deep Learning for RGB-D Object Recognition》
2016, Ziyan Wang，Jiwen Lu et al. CIMDL
作者提出了针对RGB-D物体识别的相关和个体多模态深度学习方法（CIMDL）。
1.网络结构：
RGB-D，sharable and modalspecific information can be simultaneously and explicitly exploited.
![在这里插入图片描述](https://img-blog.csdn.net/20181004182725530?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
CIM Layer的设计有3个目的：
1）产生两种模态相关联的部分
2）提取两种模态具有区分力部分的特征
3）自主的学习相关和独立部分的权重，为了特征融合
本文采用ResNet作为基本的网络结构，单独的对RGB和Depth进行训练。
对于depth network，作者采用法向图（surface normals）代替深度图作为网络的输入。
2.CIMDL Layer细节
其中Xi表示RGB-ResNet和SN-ResNet一个bact_size（N张图片）倒数第二层输出的激活值；
Qi和Vi表示（MxM）的特征映射矩阵，转变原始的特征到modal-specific domain and the correlated domain；L表示输出的类别。
多模态学习模型是利用RGB和Depth相关的特性，外加单独模态的特性和调整特征不同部分的权重来提高识别率。
模型中3个关键的特性：
1）一种多模态学习策略，自动的分解特征进去相关部分和独立部分。
2）确保相关部分和独立部分的区分度和正交性
3）在数据驱动的方式学习不同部分的权重去提高识别表现
我们的目标是去学习具有区分力的特征表示去达到两个目的：
1）不同模态的一些信息是共享的
2）一些特定模态的信息是利用为每一个单独的模态
![在这里插入图片描述](https://img-blog.csdn.net/20181004182919558?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
映射矩阵Vi（i=1,2），分别对应着RGB和Depth
共性特征的约束：
![在这里插入图片描述](https://img-blog.csdn.net/20181004182959907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
，||F表示Frobenius norm。
[
](https://img-blog.csdn.net/20181004182959907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)3.算法流程：
![在这里插入图片描述](https://img-blog.csdn.net/20181004183128469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.实验结果：
![在这里插入图片描述](https://img-blog.csdn.net/20181004183205541?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
小结：
加入了共性特征约束和不同部分权重系数自主学习。
---
注：博众家之所长，集群英之荟萃。

