# 卷积神经网络—SPPNet(空间金字塔池化) - wsp_1138886114的博客 - CSDN博客





2018年08月17日 16:04:20[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1081
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)









### 一：介绍 SPPNet

目前流行的CNN都需要固定size和scale的输入图片；所以基本上都是通过剪裁（crop）和wrap。这种处理方式有三个弊端 
- 剪裁的图图片可能不包含整个物体
- wrap导致物体变形
- 当物体大小改变时，预定义的scale可能不适合物体的变化 

CNN网络对于固定输入的要求，主要在全连接的分类器层，而特征提取层可以通过控制子采样比例和filter尺寸来调节，来接受各种scale和size的输入，得到固定的特征输出。

Deep Residual Learning for Image Recognition ：  

引入Spatial PyramidPooling 层SPP，通过SPP来移除CNN对于固定输入的要求，SPP操作类似深层的信息“融合”。这种深层的信息融合类似人脑的分级信息处理方式；当我们看到一个物体时，在一开始我们把物体看成一个整体而不是剪裁一个部分；通过融合先前处理的信息，大脑处理在深层阶段识别各种形状的物体。(有点类似高斯金字塔/拉普拉斯金字塔对图像的处理)  

SPP结构从细密和粗糙级别上分割图像，然后融合局部的特征。SPP有3个优势：
- 任任意size和scale输入下，产生固定的输出特征
- 使用多级spatial bins（多个尺寸的pooling），多级pooling对于物体形变具有鲁棒性。 

SPP的优势，可以让我们：1，生产整个图片的特征，用于测试；2，允许我们在各种size和scale下训练网络，可以增加样本个数，防止过拟合。（类似数据增益） 
![这里写图片描述](https://img-blog.csdn.net/20180817152932964?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二：SPP-net
- SPP-layer
根据pooling规则，每个pooling   bin（window）对应一个输出，所以最终pooling后特征输出由bin的个数来决定。  

本文就是分级固定bin的个数，调整bin的尺寸来实现多级pooling固定输出。- 如下图所示，layer-5的unpooled FM维数为16*24，按照图中所示分为3级，
第一级bin个数为1，最终对应的window大小为16*24； 

第二级bin个数为4个，最终对应的window大小为4*8 

第三级bin个数为16个，最终对应的window大小为1*1.5（小数需要舍入处理）
![这里写图片描述](https://img-blog.csdn.net/20180817153030153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过融合各级bin的输出，最终每一个unpooled FM经过SPP处理后，得到了1+4+16维的SPPed FM输出特征，经过融合后输入分类器。 

      这样就可以在任意输入size和scale下获得固定的输出；不同scale下网络可以提取不同尺度的特征，有利于分类。 
### 总结与想法：

**优点**
- 
Spp是一个处理multi-scale和multi-size灵活的方法，增加CNN的鲁棒性，提升分类效果。也同时证明一些经典的计算机视觉算法，在CNN中也是有用的。

- 
SPP是一种可以提升CNN分类效果的实用方法，而且通过调整bin个数，还可以减少参数。感觉SPP结构使得最不同级别的bin对应的原始图像区域是不一样的，多个对应区域可以对应不同尺度的物体，从而使网络对于物体缩放的鲁棒性有所提升。感觉这个和GoogLeNet中的Inception多尺度提取信息，有些类似。通过调整filter或者pooling window的尺寸来调整每个激活值对应的区域，从而提取不同尺度的物体特征。 

- FC层需要固定长度输入 :全连接层的参数就和输入图像大小有关，因为它要把输入的所有像素点连接起来,需要指定输入层神经元个数和输出层神经元个数，所以需要规定输入的feature的大小。 
- 
传统的CNN由于全连接层的存在，限制了输入必须固定大小（比如AlexNet是224x224），所以在实际使用中往往需要对原图片进行crop或者warp的操作：

- 
crop：截取原图片的一个固定大小的patch，物体可能会产生截断

- warp：将原图片的RoI缩放到一个固定大小的patch，物体被拉伸，失去“原形” 

无论是crop还是warp，都无法保证在不失真的情况下将图片传入到CNN当中。

**缺点**

SPP-Net只解决了R-CNN卷积层计算共享的问题，但是依然存在着其他问题：
- 训练分为多个阶段，步骤繁琐: fine-tune+训练SVM+训练Bounding Box
- SPP-Net在fine-tune（微调）网络的时候固定了卷积层，**只对全连接层进行微调**，而对于一个新的任务，有必要对卷积层也进行fine-tune（微调）。（分类的模型提取的特征更注重高层语义，而目标检测任务除了语义信息还需要目标的位置信息）

针对这两个问题，RBG又提出Fast R-CNN, 一个精简而快速的目标检测框架。





