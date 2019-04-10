# 从SRCNN到EDSR，总结深度学习端到端超分辨率方法发展历程

![96](https://upload.jianshu.io/users/upload_avatars/13499843/2ec6f5d8-ba47-48e2-93e8-e98259726dda?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96)

 

[Sunglam](https://www.jianshu.com/u/fd901e2c45a2)

 

关注

 0.1 2018.10.17 19:45 字数 5656 阅读 311评论 0喜欢 7

此文转自    知乎专栏  棉花糖的工坊      作者 棉花糖    （侵删）

一直在学习图像超分辨率和深度学习，看到此文写得很好，故此转载分享。如有侵权，私信联系我，我将作删除。

超分辨率技术（Super-Resolution, SR）是指从观测到的低分辨率图像重建出相应的高分辨率图像，在监控设备、卫星图像和医学影像等领域都有重要的应用价值。

本文针对端到端的基于深度学习的单张图像超分辨率方法(Single Image Super-Resolution, SISR)，总结一下从SRCNN到EDSR的发展历程。(排列顺序大致按论文中给出的4倍上采样结果的峰值信噪比(Peak Signal to Noise Ratio, PSNR)从低到高排列)

#### **1. SRCNN** 

([Learning a Deep Convolutional Network for Image Super-Resolution](https://link.zhihu.com/?target=http%3A//personal.ie.cuhk.edu.hk/%7Eccloy/files/eccv_2014_deepresolution.pdf), ECCV2014)

SRCNN是深度学习用在超分辨率重建上的开山之作。SRCNN的网络结构非常简单，仅仅用了三个卷积层，网络结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-28fd0ec0ec5bcdaa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

SRCNN首先使用双三次(bicubic)插值将低分辨率图像放大成目标尺寸，接着通过三层卷积网络拟合非线性映射，最后输出高分辨率图像结果。本文中，作者将三层卷积的结构解释成三个步骤：**图像块的提取和特征表示，特征非线性映射和最终的重建**。

三个卷积层使用的卷积核的大小分为为9x9,，1x1和5x5，前两个的输出特征个数分别为64和32。用Timofte数据集（包含91幅图像）和ImageNet大数据集进行训练。使用均方误差(Mean Squared Error, MSE)作为损失函数，有利于获得较高的PSNR。

code: [http://mmlab.ie.cuhk.edu.hk/projects/SRCNN.html](https://link.zhihu.com/?target=http%3A//mmlab.ie.cuhk.edu.hk/projects/SRCNN.html)

github(tensorflow): <https://github.com/tegg89/SRCNN-Tensorflow>

------

#### **2. FSRCNN** 

**(Accelerating the Super-Resolution Convolutional Neural Network, ECCV2016)**

FSRCNN与SRCNN都是香港中文大学Dong Chao， Xiaoou Tang等人的工作。FSRCNN是对之前SRCNN的改进，主要在三个方面：一是在最后使用了一个反卷积层放大尺寸，因此可以直接将原始的低分辨率图像输入到网络中，而不是像之前SRCNN那样需要先通过bicubic方法放大尺寸。二是改变特征维数，使用更小的卷积核和使用更多的映射层。三是可以共享其中的映射层，如果需要训练不同上采样倍率的模型，只需要fine-tuning最后的反卷积层。

由于FSRCNN不需要在网络外部进行放大图片尺寸的操作，同时通过添加收缩层和扩张层，将一个大层用一些小层来代替，因此FSRCNN与SRCNN相比有较大的速度提升。FSRCNN在训练时也可以只fine-tuning最后的反卷积层，因此训练速度也更快。FSRCNN与SCRNN的结构对比如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-9254a7d6af532013.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

FSRCNN

FSRCNN可以分为五个部分。**特征提取**：SRCNN中针对的是插值后的低分辨率图像，选取的核大小为9×9，这里直接是对原始的低分辨率图像进行操作，因此可以选小一点，设置为5×5。**收缩**：通过应用1×1的卷积核进行降维，减少网络的参数，降低计算复杂度。**非线性映射**：感受野大，能够表现的更好。SRCNN中，采用的是5×5的卷积核，但是5×5的卷积核计算量会比较大。用两个串联的3×3的卷积核可以替代一个5×5的卷积核，同时两个串联的小卷积核需要的参数3×3×2=18比一个大卷积核5×5=25的参数要小。FSRCNN网络中通过m个核大小为3×3的卷积层进行串联。**扩张**：作者发现低维度的特征带来的重建效果不是太好，因此应用1×1的卷积核进行扩维，相当于收缩的逆过程。**反卷积层**：可以堪称是卷积层的逆操作，如果步长为n，那么尺寸放大n倍，实现了上采样的操作。

FSRCNN中激活函数采用PReLU，损失函数仍然是均方误差。对CNN来说，Set91并不足够去训练大的网络结构，FSRCNN提出general-100 + Set91进行充当训练集。并且进行数据增强，1）缩小尺寸为原来的0.9, 0.8, 0.7和0.6。2）旋转 90°，180°和270°，因此获得了数据量的提升。

code: [http://mmlab.ie.cuhk.edu.hk/projects/FSRCNN.html](https://link.zhihu.com/?target=http%3A//mmlab.ie.cuhk.edu.hk/projects/FSRCNN.html)

github(tensorflow): <https://github.com/yifanw90/FSRCNN-TensorFlow>

------

#### **3. VDSR** 

([Accurate Image Super-Resolution Using Very Deep Convolutional Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1511.04587), CVPR2016)

在介绍VDSR之前，首先想先提一下何恺明在2015年的时候提出的残差网络ResNet。ResNet的提出，解决了之前网络结构比较深时无法训练的问题，性能也得到了提升，ResNet也获得了CVPR2016的best paper。残差网络结构(residual network)被应用在了大量的工作中。

正如在VDSR论文中作者提到，输入的低分辨率图像和输出的高分辨率图像在很大程度上是相似的，也就是指低分辨率图像携带的低频信息与高分辨率图像的低频信息相近，训练时带上这部分会多花费大量的时间，实际上我们只需要学习高分辨率图像和低分辨率图像之间的高频部分残差即可。残差网络结构的思想特别适合用来解决超分辨率问题，可以说影响了之后的深度学习超分辨率方法。VDSR是最直接明显的学习残差的结构，其网络结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-8bad6cd6b0aaaea0.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

VDSR

VDSR将插值后得到的变成目标尺寸的低分辨率图像作为网络的输入，再将这个图像与网络学到的残差相加得到最终的网络的输出。VDSR主要有4点贡献。1.加深了网络结构(20层)，使得越深的网络层拥有更大的感受野。文章选取3×3的卷积核，深度为D的网络拥有(2D+1)×(2D+1)的感受野。2.采用残差学习，残差图像比较稀疏，大部分值都为0或者比较小，因此收敛速度快。VDSR还应用了自适应梯度裁剪(Adjustable Gradient Clipping)，将梯度限制在某一范围，也能够加快收敛过程。3.VDSR在每次卷积前都对图像进行补0操作，这样保证了所有的特征图和最终的输出图像在尺寸上都保持一致，解决了图像通过逐步卷积会越来越小的问题。文中说实验证明补0操作对边界像素的预测结果也能够得到提升。4.VDSR将不同倍数的图像混合在一起训练，这样训练出来的一个模型就可以解决不同倍数的超分辨率问题。

code: [https://cv.snu.ac.kr/research/VDSR/](https://link.zhihu.com/?target=https%3A//cv.snu.ac.kr/research/VDSR/)

github(caffe): [https://github.com/huangzehao/caffe-vdsr](https://link.zhihu.com/?target=https%3A//github.com/huangzehao/caffe-vdsr)

github(tensorflow): [https://github.com/Jongchan/tensorflow-vdsr](https://link.zhihu.com/?target=https%3A//github.com/Jongchan/tensorflow-vdsr)

github(pytorch): [https://github.com/twtygqyy/pytorch-vdsr](https://link.zhihu.com/?target=https%3A//github.com/twtygqyy/pytorch-vdsr)

------

#### **4. DRCN** 

([Deeply-Recursive Convolutional Network for Image Super-Resolution](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1511.04491), CVPR2016)

DRCN与上面的VDSR都是来自首尔国立大学计算机视觉实验室的工作，两篇论文都发表在CVPR2016上，两种方法的结果非常接近。DRCN第一次将之前已有的递归神经网络(Recursive Neural Network)结构应用在超分辨率处理中。同时，利用残差学习的思想(文中的跳跃连接（Skip-Connection）)，加深了网络结构(16个递归)，增加了网络感受野，提升了性能。DRCN网络结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-0ab3a811ee9d456a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

DRCN

DRCN输入的是插值后的图像，分为三个模块，第一个是**Embedding network**，相当于特征提取，第二个是**Inference network**, 相当于特征的非线性映射，第三个是**Reconstruction network**,即从特征图像恢复最后的重建结果。其中的Inference network是一个递归网络，即数据循环地通过该层多次。将这个循环进行展开，等效于使用同一组参数的多个串联的卷积层，如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-570866ee083542a0.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

code: [https://cv.snu.ac.kr/research/DRCN/](https://link.zhihu.com/?target=https%3A//cv.snu.ac.kr/research/DRCN/)

githug(tensorflow): [https://github.com/jiny2001/deeply-recursive-cnn-tf](https://link.zhihu.com/?target=https%3A//github.com/jiny2001/deeply-recursive-cnn-tf)

------

#### **5. RED** 

([Image Restoration Using Convolutional Auto-encoders with Symmetric Skip Connections](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1606.08921), NIPS2016)

这篇文章提出了由对称的卷积层-反卷积层构成的网络结构，作为一个编码-解码框架，可以学习由低质图像到原始图像端到端的映射。网络结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-299224472a154764.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

RED

RED网络的结构是对称的，每个卷积层都有对应的反卷积层。卷积层用来获取图像的抽象内容，反卷积层用来放大特征尺寸并且恢复图像细节。卷积层将输入图像尺寸减小后，再通过反卷积层上采样变大，使得输入输出的尺寸一样。每一组镜像对应的卷积层和反卷积层有着跳线连接结构，将两部分具有同样尺寸的特征(要输入卷积层的特征和对应的反卷积层输出的特征)做相加操作(ResNet那样的操作)后再输入到下一个反卷积层，操作过程如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-8b336a51ed6ad928.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

这样的结构能够让反向传播信号能够直接传递到底层，解决了梯度消失问题，同时能将卷积层的细节传递给反卷积层，能够恢复出更干净的图片。可以看到，网络中有一条线是将输入的图像连接到后面与最后的一层反卷积层的输出相加，也就是VDSR中用到的方式，因此RED中间的卷积层和反卷积层学习的特征是目标图像和低质图像之间的残差。RED的网络深度为30层，损失函数用的均方误差。

------

#### **6. DRRN** 

([Image Super-Resolution via Deep Recursive Residual Network](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_cvpr_2017/papers/Tai_Image_Super-Resolution_via_CVPR_2017_paper.pdf), CVPR2017)

DRRN的作者应该是受到了ResNet、VDSR和DRCN的启发，采用了更深的网络结构来获取性能的提升。作者也在文中用图片示例比较了DRRN与上述三个网络的区别，比较示例图如下所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-8bbf76485d7ec89b.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

DRRN

DRRN中的每个残差单元都共同拥有一个相同的输入，即递归块中的第一个卷积层的输出。每个残差单元都包含2个卷积层。在一个递归块内，每个残差单元内对应位置相同的卷积层参数都共享(图中DRRN的浅绿色块或浅红色块)。作者列出了ResNet、VDSR、DRCN和DRRN四者的主要策略。ResNet是链模式的局部残差学习。VDSR是全局残差学习。DRCN是全局残差学习+单权重的递归学习+多目标优化。DRRN是多路径模式的局部残差学习+全局残差学习+多权重的递归学习。

文章中比较了不同的递归块和残差单元数量的实验结果，最终选用的是1个递归块和25个残差单元，深度为52层的网络结构。总之，DRRN就是通过对之前已有的ResNet等结构进行调整，采取更深的网络结构得到结果的提升。

github(caffe): [tyshiwo/DRRN_CVPR17](https://link.zhihu.com/?target=https%3A//github.com/tyshiwo/DRRN_CVPR17)

github(pytorch): [https://github.com/jt827859032/DRRN-pytorch](https://link.zhihu.com/?target=https%3A//github.com/jt827859032/DRRN-pytorch)

------

#### **7. SRDenseNet** 

([Image Super-Resolution Using Dense Skip Connections](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_ICCV_2017/papers/Tong_Image_Super-Resolution_Using_ICCV_2017_paper.pdf), ICCV2017)

DenseNet是CVPR2017的best papaer获奖论文。DenseNet在稠密块(dense block)中将每一层的特征都输入给之后的所有层，使所有层的特征都串联(concatenate)起来，而不是像ResNet那样直接相加。这样的结构给整个网络带来了减轻梯度消失问题、加强特征传播、支持特征复用、减少参数数量的优点。一个稠密块的结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-d663f7d3f4f43658.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

dense block

SRDenseNet将稠密块结构应用到了超分辨率问题上，取得了不错的效果。网络结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-92d3e155ab40d4e0.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

SRDenseNet

SRDenseNet可以分成四个部分。首先是用一个卷积层学习低层的特征，接着用多个稠密块学习高层的特征，然后通过几个反卷积层学到上采样滤波器参数，最后通过一个卷积层生成高分辨率输出。

文章中针对用于最后重建的输入内容不同，设计了三种结构并做了比较。一是反卷积层只输入最顶层稠密块的输出。二是添加了一个跳跃连接，将最底层卷积层的输出特征和最顶层稠密块的输出特征串联起来，再输入反卷积层。三是添加了稠密跳跃连接，就是把稠密块看成一个整体，第一个卷积层的输出以及每个稠密块的输出，都输入给在之后的所有稠密块，像是把在反卷积层之前的整个网络也设计成像稠密块那样的结构。由于这样做，所有的特征都串联起来，这样直接输入反卷积层会产生巨大的计算开销，因此添加了一个核大小为1×1的卷积层来减小特征数量，这个卷积层被称为瓶颈层。最后的结果是越复杂的越好，3>2>1。文章中分析的是，受益于低层特征和高层特征的结合，超分辨率重建的性能得到了提升。像第三种结构把所有深度层的特征都串联起来，得到了最佳的结果，说明不同深度层的特征之间包含的信息是互补的。

------

#### **8. SRGAN(SRResNet)** 

([Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1609.04802), CVPR2017)

在这篇文章中，将生成对抗网络(Generative Adversarial Network, GAN)用在了解决超分辨率问题上。文章提到，训练网络时用均方差作为损失函数，虽然能够获得很高的峰值信噪比，但是恢复出来的图像通常会丢失高频细节，使人不能有好的视觉感受。SRGAN利用感知损失(perceptual loss)和对抗损失(adversarial loss)来提升恢复出的图片的真实感。感知损失是利用卷积神经网络提取出的特征，通过比较生成图片经过卷积神经网络后的特征和目标图片经过卷积神经网络后的特征的差别，使生成图片和目标图片在语义和风格上更相似。一个GAN所要完成的工作，GAN原文举了个例子：生成网络(G)是印假钞的人，判别网络(D)是检测假钞的人。G的工作是让自己印出来的假钞尽量能骗过D，D则要尽可能的分辨自己拿到的钞票是银行中的真票票还是G印出来的假票票。开始的时候呢，G技术不过关，D能指出这个假钞哪里很假。G每次失败之后都认真总结经验，努力提升自己，每次都进步。直到最后，D无法判断钞票的真假……SRGAN的工作就是： G网通过低分辨率的图像生成高分辨率图像，由D网判断拿到的图像是由G网生成的，还是数据库中的原图像。当G网能成功骗过D网的时候，那我们就可以通过这个GAN完成超分辨率了。

文章中，用均方误差优化SRResNet(SRGAN的生成网络部分)，能够得到具有很高的峰值信噪比的结果。在训练好的VGG模型的高层特征上计算感知损失来优化SRGAN，并结合SRGAN的判别网络，能够得到峰值信噪比虽然不是最高，但是具有逼真视觉效果的结果。SRGAN网络结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-f4c2ac3c53e4a62b.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

在生成网络部分(SRResNet)部分包含多个残差块，每个残差块中包含两个3×3的卷积层，卷积层后接批规范化层(batch normalization, BN)和PReLU作为激活函数，两个2×亚像素卷积层(sub-pixel convolution layers)被用来增大特征尺寸。在判别网络部分包含8个卷积层，随着网络层数加深，特征个数不断增加，特征尺寸不断减小，选取激活函数为LeakyReLU，最终通过两个全连接层和最终的sigmoid激活函数得到预测为自然图像的概率。SRGAN的损失函数为：



![img](https://upload-images.jianshu.io/upload_images/13499843-3fc9ba763ecb8ac3.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/541/format/webp)

其中内容损失可以是基于均方误差的损失的损失函数：



![img](https://upload-images.jianshu.io/upload_images/13499843-128e4b3f9a666816.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/634/format/webp)

也可以是基于训练好的以ReLU为激活函数的VGG模型的损失函数:



![img](https://upload-images.jianshu.io/upload_images/13499843-2fef18aa67d9991e.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/614/format/webp)

i和j表示VGG19网络中第i个最大池化层(maxpooling)后的第j个卷积层得到的特征。对抗损失为：



![img](https://upload-images.jianshu.io/upload_images/13499843-e5e5b1b9e6535075.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/475/format/webp)

文章中的实验结果表明，用基于均方误差的损失函数训练的SRResNet，得到了结果具有很高的峰值信噪比，但是会丢失一些高频部分细节，图像比较平滑。而SRGAN得到的结果则有更好的视觉效果。其中，又对内容损失分别设置成基于均方误差、基于VGG模型低层特征和基于VGG模型高层特征三种情况作了比较，在基于均方误差的时候表现最差，基于VGG模型高层特征比基于VGG模型低层特征的内容损失能生成更好的纹理细节。

github(tensorflow): [https://github.com/zsdonghao/SRGAN](https://link.zhihu.com/?target=https%3A//github.com/zsdonghao/SRGAN)

github(tensorflow): [https://github.com/buriburisuri/SRGAN](https://link.zhihu.com/?target=https%3A//github.com/buriburisuri/SRGAN)

github(torch): [https://github.com/junhocho/SRGAN](https://link.zhihu.com/?target=https%3A//github.com/junhocho/SRGAN)

github(caffe): [https://github.com/ShenghaiRong/caffe_srgan](https://link.zhihu.com/?target=https%3A//github.com/ShenghaiRong/caffe_srgan)

github(tensorflow): [https://github.com/brade31919/SRGAN-tensorflow](https://link.zhihu.com/?target=https%3A//github.com/brade31919/SRGAN-tensorflow)

github(keras): [https://github.com/titu1994/Super-Resolution-using-Generative-Adversarial-Networks](https://link.zhihu.com/?target=https%3A//github.com/titu1994/Super-Resolution-using-Generative-Adversarial-Networks)

github(pytorch): [ai-tor/PyTorch-SRGAN](https://link.zhihu.com/?target=https%3A//github.com/ai-tor/PyTorch-SRGAN)

------

#### **9. EDSR** 

([Enhanced Deep Residual Networks for Single Image Super-Resolution](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1707.02921), CVPRW2017)

EDSR是NTIRE2017超分辨率挑战赛上获得冠军的方案。如论文中所说，EDSR最有意义的模型性能提升是去除掉了SRResNet多余的模块，从而可以扩大模型的尺寸来提升结果质量。EDSR的网络结构如下图所示。



![img](https://upload-images.jianshu.io/upload_images/13499843-dc2a7cd1dc6b0941.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

EDSR

可以看到，EDSR在结构上与SRResNet相比，就是把批规范化处理(batch normalization, BN)操作给去掉了。文章中说，原始的ResNet最一开始是被提出来解决高层的计算机视觉问题，比如分类和检测，直接把ResNet的结构应用到像超分辨率这样的低层计算机视觉问题，显然不是最优的。由于批规范化层消耗了与它前面的卷积层相同大小的内存，在去掉这一步操作后，相同的计算资源下，EDSR就可以堆叠更多的网络层或者使每层提取更多的特征，从而得到更好的性能表现。EDSR用L1范数样式的损失函数来优化网络模型。在训练时先训练低倍数的上采样模型，接着用训练低倍数上采样模型得到的参数来初始化高倍数的上采样模型，这样能减少高倍数上采样模型的训练时间，同时训练结果也更好。

这篇文章还提出了一个能同时不同上采样倍数的网络结构MDSR，如下图。



![img](https://upload-images.jianshu.io/upload_images/13499843-7fa1fc1e06c6e0b1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

MDSR

MDSR的中间部分还是和EDSR一样，只是在网络前面添加了不同的预训练好的模型来减少不同倍数的输入图片的差异。在网络最后，不同倍数上采样的结构平行排列来获得不同倍数的输出结果。

从文章给出的结果可以看到，EDSR能够得到很好的结果。增大模型参数数量以后，结果又有了进一步的提升。因此如果能够解决训练困难的问题，网络越深，参数越多，对提升结果确实是有帮助吧。

github(torch): [https://github.com/LimBee/NTIRE2017](https://link.zhihu.com/?target=https%3A//github.com/LimBee/NTIRE2017)

github(tensorflow): [https://github.com/jmiller656/EDSR-Tensorflow](https://link.zhihu.com/?target=https%3A//github.com/jmiller656/EDSR-Tensorflow)

github(pytorch): [https://github.com/thstkdgus35/EDSR-PyTorch](https://link.zhihu.com/?target=https%3A//github.com/thstkdgus35/EDSR-PyTorch)

通过以上11篇有关深度学习超分辨率方法的论文，可以看到通过网络结构、损失函数以及训练方式的演变，深度学习超分辨率方法在结果、速度以及应用性上都有了不断的提高。这里再放上一篇深度学习超分辨率方法综述的链接([Super-Resolution via Deep Learning](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.09077))以及github上一个超分辨率方法的总结([https://github.com/YapengTian/Single-Image-Super-Resolution](https://link.zhihu.com/?target=https%3A//github.com/YapengTian/Single-Image-Super-Resolution))。

非常感谢许多知乎和博客上的文章，由于比较多，这里列出参考得比较多的几个资源：

<https://zhuanlan.zhihu.com/p/25532538?utm_source=tuicool&utm_medium=referral>

[http://blog.csdn.net/u011692048/article/category/7121139](https://link.zhihu.com/?target=http%3A//blog.csdn.net/u011692048/article/category/7121139)

[http://blog.csdn.net/wangkun1340378/article/category/7004439](https://link.zhihu.com/?target=http%3A//blog.csdn.net/wangkun1340378/article/category/7004439)

[ TensorFlow学习笔记](https://www.jianshu.com/nb/30388337)