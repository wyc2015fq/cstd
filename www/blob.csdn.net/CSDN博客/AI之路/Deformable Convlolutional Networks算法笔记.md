# Deformable Convlolutional Networks算法笔记 - AI之路 - CSDN博客





2018年12月08日 16:31:13[AI之路](https://me.csdn.net/u014380165)阅读数：738








论文：Deformable Convolutional Networks

论文链接：[https://arxiv.org/abs/1703.06211](https://arxiv.org/abs/1703.06211)

代码链接：[https://github.com/msracver/Deformable-ConvNets](https://github.com/msracver/Deformable-ConvNets)
**这一篇是ICCV2017关于目标检测的文章，出发点在于图像任务中目标的尺寸、形状变化不一，虽然现有的深度网络对此有一定的特征提取能力，但并不是最优的，因此通过在卷积层中引入offset（deformable convolution）和在RoI pooling层中引入offset（deformable RoI pooling）提高网络的特征提取效果，这个offset的作用是使网络在提取特征时更多的注意力能聚焦到和训练目标相关的位置，能够更好地覆盖不同尺度和形状的目标，而且由于offset也是在监督信息的指导下进行学习的，因此不用像数据增强一样需要先验知识才能有好的效果。**

**Figure2是deformable convolution的示意图，整体上可以分成2部分，第一部分是基于输入特征图生成offset，第二部分是基于输入特征图和offset通过deformable convolution生成输出特征图**。假设输入特征图高和宽分别是h和w，第二部分的卷积核尺寸是kh和kw。那么第一部分卷积层的卷积核数量是2×kh×kw，这个2表示x轴和y轴两个方向上的偏移值，而且输出特征图的宽高和输入特征图的宽高一样，这样offset的维度就是[batch_size, 2×kh×kw, h, w]，假如第二部分设置了group参数（默认代码中设置为4），那么第一部分的卷积核数量就是2×kh×kw×deformable_group，相当于每一个group用一套offset。第二部分的deformable convolution可以看作是先基于第一部分生成的offset执行插值计算，然后再执行普通卷积操作的过程。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162353797.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
**Figure5是常规卷积和deformable convolution的差异示意图**，其实也是这篇文章的出发点。图中以2个3×3卷积层为例，可以看出对于常规卷积层，卷积操作的位置都是固定的，而deformable convolution因为引入offset，所以卷积操作的位置会在监督信息的指导下进行选择，能够较好地适应输入目标的尺度、形状，因此提取到的特征更加丰富而且都能集中在目标上。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162420151.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
**Figure6是Figure5示意图在实际图像上的效果**，在Figure6的每张图像上都有绿色的和红色点，其中绿色点有1个，这个点表示conv5输出特征图上的1个位置，往前3个卷积层理论上就可以得到9^3=729个红色点，也就是卷积层计算的区域，当然这729个点有大部分是越界的，所以实际图中显示的红色点会少于729。可以看出当绿色点在目标上时，红色点所在的区域也集中在目标位置，而且基本能够覆盖不同尺寸的目标，因此deformable convolution不仅能够提取更加有效的特征，而且感受野也比常规的卷积层大。当绿色点在背景上时，红色点所在区域比较分散，应该是不断往外寻找并确认该区域是否是背景区域的过程。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162442925.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
**Figure3是deformable RoI pooling的示意图，整体上可以分为2部分，第一部分是基于输入特征图生成offset，第二部分是基于输入特征图和offset通过deformable RoI pooling生成输出RoI特征图**。第一部分先通过常规的RoI pooling得到RoI特征图（假设将每个RoI划分成k×k个bin，Figure3中是划分成3×3个bin的示意图），然后通过一个输出节点数为k×k×2的全连接层得到offset信息，最后再reshape成维度为[batch_size, 2, k, k]的offset。第二部分也是先基于offset执行插值操作，然后再执行常规的RoI pooling，这样就完成了deformable RoI pooling。**因此deformabl RoI pooling和传统的RoI pooling的差异在于每个RoI中bin的位置是根据监督信息进行学习的，而不是固定划分的**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162652923.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
**Figure4是deformable position-sensitive(PS)RoI pooling的示意图，和前面一样也是分为2部分**。第一部分先通过卷积核数量为2×k×k×(C+1)的卷积层得到输出特征图，其中k×k表示bin的数量，C表示目标的类别数，1表示背景，然后基于该特征图通过PS RoI pooling操作得到输出维度为[batch_size, 2×(C+1), k, k]的offset。第二部分先通过卷积核数量为k*k*(C+1)的卷积层得到输出特征图，这是R-FCN算法中的常规操作，然后基于该特征图和第一部分输出的offset执行deformable PS RoI pooling操作，deformable PS RoI pooling也可以看作是先执行插值计算，然后执行PS RoI pooling。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162741818.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
Figure7是deformable PS RoI pooling在实际图像上的效果（deformable RoI pooling也是同理），每张图中都有1个RoI（黄色框）和3×3=9个bin（红色框），也就是k设置成了3，在常规的PS RoI pooling中这9个bin的位置应该是均匀划分的，但是在deformable PS RoI pooling中是集中在目标区域的，说明deformable结构确实是可以让网络的attention更集中于目标区域。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162814733.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
在代码实现上，主网络采用ResNet101，但是原本stride=32的res5部分修改为stride=16，同时deformable convolution也只在res5部分的3×3卷积层添加（后续的Deformable ConveNet v2对这部分做了改进），另外为了弥补修改stride带来的感受野减小，在res5的deformable convolution部分将dilate参数设置为2。

**实验结果：**

Table1是在PASCAL VOC数据集上做的对比实验，包括在多种图像任务和网络的不同阶段添加deformable层的差异。可以看到将res5的3个卷积层替换成deformable convolution就有明显的效果提升，而且基本饱和了。这部分其实主要是数据集（VOC）的原因导致替换更多的deformable结构没有明显提升，在v2中作者将数据集换成COCO做这个对比实验就实现了效果提升，因此有时候除了思考网络设计外，也应该多思考数据集是否成为效果提升的瓶颈。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162857797.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
Table3主要是和atrous convolution（也就是dilated convolution）作对比，因为atrous convolution也是增加了传统卷积操作的感受野，所以这个对比实验是在证明都增加感受野的同时，以固定方式扩大感受野和更加灵活地聚焦到目标区域的优劣。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190228084604970.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
Table4是模型大小和速度的对比，整体而言并不会引入太多的存储和计算量。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208162954369.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)
Table5是在COCO数据集上关于添加deformable结构的效果对比，提升还是比较明显的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208163030364.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=,size_16,color_FFFFFF,t_70)













