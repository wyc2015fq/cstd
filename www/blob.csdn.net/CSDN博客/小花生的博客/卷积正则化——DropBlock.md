
# 卷积正则化——DropBlock - 小花生的博客 - CSDN博客


2018年11月11日 15:48:28[Peanut_范](https://me.csdn.net/u013841196)阅读数：279



##### 《DropBlock: A regularization method for convolutional networks》
NIPS 2018，Golnaz Ghiasi et al.  DropBlock
### 引言：
在目前过度参数化的深度神经网络中，Dropout被广泛的使用作为一种正则化技术在全连接层中，但在卷积层中却收效甚微。
**为什么Dropout在卷积层中难以奏效？**
作者认为卷积层中的激活单元（特征图上元素）在空间中是相关的，所有尽管Dropout可以进行信息丢失，但信息仍然可以通过卷积网络流通。
在本文，作者提出了**DropBlock**，一种结构化的Dropout形式，特征图上的邻域单元被一起丢弃。作者发现，除了卷积层之外，在skip connections应用DropBlock也提高了准确率。
大量的实验表明，DropBlock在正则化的卷积网络中性能优于Dropout。在ImageNet分类上，ResNet-50结构同DropBlock达到了78.13%的准确率，比基准模态提升1.6%。
**DropBlock**：按块（block）丢弃（drop）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153521307.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
当DropBlock丢弃相关区域特征时，网络必须寻找其他地方以适应数据。
[
](https://img-blog.csdnimg.cn/20181111153521307.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
### DropBlock思想：
[
](https://img-blog.csdnimg.cn/20181111153521307.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)**1.算法流程**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153612757.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
DropBlock有两个主要参数**block_size**和**γ**，block_size代表丢弃掉的块大小，γ控制着多少特征块将被丢弃。
**2.示意图**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153706595.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
上图中**绿框**以内是按照block_size计算得来的可以生成种子点的区域（为了保证block不出特征图），**红色X**标出的元素即为种子点，**黑色X**标出的元素即为置0的区域。
**设置block_size大小：**对于所有的feature maps，设置固定的block_size。
**设置γ大小：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153742486.png)
其中，Keep_prob = {0.75, 0.95}
**Scheduled DropBlock：**作者发现在训练期间固定keep_prob，训练效果不好。线性减小keep_prob的值从1到目标值，结果更加鲁棒。
### 实验结果：
本文在ImageNet图像分类、COCO目标检测、PASCAL VOC 2012语义分割等任务来验证DropBlock的有效性。
ResNet-50网络+不同的正则化技术和DropBlock进行比较，显著提升了分类精度：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153818931.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
下图是在最新的分类网络AmoebaNet+DropBlock的结果，top1和top5有略微提升:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153851307.png)
下图展示了DropBlock与dropout相比提高ImageNet分类精度的实验结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153925610.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
下图为使用RetinaNet检测网络+DropBlock在COCO数据集上的测试结果，依然表现优异:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111153956832.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
下图为RetinaNet语义分割网络+DropBlock，提升较为明显：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111154028271.png)
### 总结：
本文引入DropBlock来规范CNNs的训练。DropBlock是一种结构化的Dropout形式，它会降低空间相关的信息。
---

###### 注：博众家之所长，集群英之荟萃。

