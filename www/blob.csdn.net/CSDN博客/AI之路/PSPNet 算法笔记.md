# PSPNet 算法笔记 - AI之路 - CSDN博客





2018年11月08日 21:52:00[AI之路](https://me.csdn.net/u014380165)阅读数：1961








论文：Pyramid Scene Parsing Network

论文链接：[https://arxiv.org/abs/1612.01105](https://arxiv.org/abs/1612.01105)

**Pyramid Scene Parsing Network（PSPNet）是CVPR2017上关于场景解析的文章，拿到了2016年ImageNet比赛中scene parsing任务的冠军，当然也常用来做语义分割。这篇文章出发点是在语义分割算法中引入更多的上下文信息（context information）， 这样能够避免许多误分割，这些误分割可以参考Figure2，PSPNet在FCN算法的基础上引入更多上下文信息是通过全局均值池化操作（global average pooling）和特征融合实现的，因此特征呈金字塔结构，这也是论文名叫pyramid的原因**。PSPNet算法是目前应用比较广泛的语义分割算法之一，该算法在PASCAL VOC2012测试集上的mIOU是82.6%。

**Figure2是FCN算法的误分割示例和PSPNet的分割结果对比，因为PSPNet是在FCN的基础上做改进，因此这组图片是对比二者的分割效果，从而引出优化点的设计**。第一行中FCN算法误将船分割成车，显然一辆车在水上的概率是很小的，这种是属于明显不匹配的误分割。第二行中FCN算法误将摩天大厦分割成建筑物，摩天大厦和建筑物这两个类别本身是比较接近的，这种是属于类别相近的误分割，这部分个人认为是和数据集相关的。第三行中FCN算法误将枕头分割成床，枕头本身区域较小，而且纹理和床较为接近，这种是属于难以觉察的误分割。作者认为这些误分割都可以通过引入更多的上下文信息进行解决，当分割层有更多全局信息时，出现上述几种误分割的概率就会相对低一些，这种思想目前在许多图像领域都有所应用，而引入更多上下文信息的方式也很多，比如：1、增大分隔层的感受野，这种方式是最直观的，视野越广，看到的东西也越多，而增大感受野也有许多方式，比如空洞卷积（dilated convolution），这是在deeplab算法上成功应用的实现方式，另外PSPNet的全局均值池化操作也是增加感受野的一种方式。2、深层特征和浅层特征的融合，增加浅层特征的语义信息，这样在浅层进行分割时就有足够的上下文信息，同时也有目标的细节信息，这种做法早在FCN中就有了，但是包括融合策略和分割层的选择都有一定的优化空间。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018110821502579.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
**Figure3是PSPNet算法的结构示意图**。首先输入图像经过一个特征提取网络提取特征，这部分作者采用的是添加了空洞卷积的ResNet网络，空洞卷积的作用前面也提到过了，主要是增大感受野，提取到的特征（具体而言stride=8）作为后面pyramid pooling模块的输入。在pyramid pooling模块中构建了深度为4的特征金字塔，不同深度的特征是基于输入特征通过不同尺度的池化操作得到的，池化的尺度是可以调整的，这篇文章中给出的池化后的特征尺寸分别是1*1、2*2、3*3和6*6。然后通过一个1*1卷积层将特征维度缩减为原来的1/4，最后将这些金字塔特征直接上采样到与输入特征相同尺寸，然后和输入特征做合并，也就是concat操作得到最终输出的特征图。特征合并的过程其实就是融合目标的细节特征（浅层特征）和全局特征（深层特征，也就是上下文信息）的过程，这里因为特征提取网络最后输出的特征层感受野足够大，所以有足够的全局信息（虽然网络的深度不算深），个人认为如果这里能够融合更多的浅层特征（比如stride=4的那一层），也许分割结果在细节方面会更好一些。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181108215105126.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
**实验结果：**

Table6是主流语义分割算法在PASCAL VOC2012数据集上的效果对比，整体看效果还是很明显的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181108215130693.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)





