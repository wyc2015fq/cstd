# 【论文笔记】Learning Deconvolution Network for Semantic Segmentation - xiaxzhou的博客 - CSDN博客





2017年07月01日 10:50:32[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：2001








### Learning Deconvolution Network for Semantic Segmentation

### 首先介绍了基于FCN的语义分割，如论文：
- Fully Convolutional Networks for Semantic Segmentation
- Semantic Image Segmentation with Deep Convolutional Nets and Fully Connectred CRFs

#### **FCN解释如下:**

fully connected layers in the standard CNNs are interpreted as convolutions with large receptive fields, and segmentation is achieved using coarse class score maps obtained by feedforwarding an input image. 

An interesting idea in this work is that a simple interpolation filter is employed for deconvolution and only the CNN part of the network is fine-tuned to learn deconvolution indirectly.

#### **FCN存在以下问题：**

如图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170630205206033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
**first**, the network can handle only a single scale semantics within image due to the fixed-size receptive field 

  首先，由于固定大小的感受域，网络只能处理图像中的单个尺度语义。

**second**, the detailed structures of an object are often lost or smoothed because the label map, input to the deconvolutional layer is too coarse and deconvolution procedure is overly simple 

  其次，一个对象的详细结构常常丢失或被平滑掉，这是因为输入到反卷积层的标签图太小，且反卷积过程过于简单。

#### **反卷积网络引入：**

Deconvolution network is introduced in [25] to reconstruct input images. As the reconstruction of an input 

image is non-trivial due to max pooling layers, it proposes 

the unpooling operation by storing the pooled location. Using the deconvoluton network, the input image can be reconstructed from its feature representation。 

通过记录最大池化的位置信息，反卷积可重建图片
#### **本文网络结构：**

![这里写图片描述](https://img-blog.csdn.net/20170630210122894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

卷积网络用于提取特征，反卷积网络用于重建图片 

卷积网络为移除了最后分类层的VGG-16：包含13个卷积层，2个全连接层。而反卷积层与卷积网络是镜像对称的，拥有反池化、反卷积层。

#### **反池化：**

![这里写图片描述](https://img-blog.csdn.net/20170630213112404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

although it helps classification by retaining only robust activations in upper layers, spatial information within a receptive field is lost during pooling, which may be critical for precise localization that is required for semantic segmentation.

池化操作虽然它通过只保留上层的主要激活来帮助分类，但在池中丢失了接收字段中的**空间信息**，这个信息对于语义分割所需的精确定位却是至关重要的。

to resolve such issue, we employ unpooling layers in deconvolution network, which perform the reverse operation of pooling and reconstruct the original size of activations

要解决这样的问题，我们采用unpooling层卷积网络，进行统筹的反向操作和重建激活原始大小

![这里写图片描述](https://img-blog.csdn.net/20170630212954411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

it records the locations of maximum activations selected during pooling operation in switch variables, which are employed to place each activation back to its original pooled location

**记录池化操作选择的最大激活的位置，在反池化操作中将每个激活放回原来池化前的位置**。

#### **反卷积：**

![这里写图片描述](https://img-blog.csdn.net/20170630214055553?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

the output of an unpooling layer is an enlarged, yet sparse activation map. 

反池化层的输出是一个扩大但稀疏的激活图。

the deconvolution layers densify the sparse activations obtained by unpooling through convolution-like operations with multiple learned filters 

反卷积层利用多个**可学习的反卷积核**，执行类似卷积的运算将反池化层得到稀疏激活图变稠密。

![这里写图片描述](https://img-blog.csdn.net/20170630215312882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

however, contrary to convolutional layers, which connect multiple input activations within a filter window to a single activation, deconvolutional layers associate a single input activation with multiple outputs, as illustrated in figure

然而，**与卷积核的多个输入单个激活输出不同，反卷积核只有单个输入但有多个激活输出**

the filters in lower layers tend to capture overall shape of an object while the class-specific finedetails are encoded in the filters in higher layers

较低层的卷积核趋向于捕捉对象的整体形状，而类特定的细节在高层的卷积核进行编码

网络直接考虑到类特定形状信息的语义分割，这在FCN中常常被忽略。

反池化层、反卷积层激活图的可视化：

![这里写图片描述](https://img-blog.csdn.net/20170630220600303?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意到，背景噪声的激活通过传播抑制，而与目标类密切相关的激活被放大。它表明了滤波器在高层会捕捉反卷积类特定的形状信息。

#### **反卷积网络的解释：**

**Unpooling captures example-specific structures by tracing the original locations with strong activations back to image space. As a result, it effectively reconstructs the detailed structure of an object in finer resolutions. On the other hand, learned filters in deconvolutional layers tend to capture class-specific shapes**

反池化层会捕获实例图像空间信息。因此，它能以更精细的分辨率有效地重建对象的详细结构。另一方面，反卷积层往往学会捕捉类特定的形状。

![这里写图片描述](https://img-blog.csdn.net/20170630221929675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### **训练：**

> 
**批量标准化：Batch Normalization**


**在深层网络的训练过程中，存在内部协变量偏移问题：每个层的输入分布都会随着前一层的参数更新而变化！这对于优化非常深的网络是有问题的，因为分布的变化会通过跨层传播被放大。**
**我们利用BN批处理规范化，通过将每个层的输入分布标准化为标准高斯分布来减少内部协变量偏移。否则网络容易陷入局部最优，所以这一操作至关重要。**

> 
We remove the drop-out layers due to batch  normalization


> 
**分两阶段进行训练：**


**first,** limiting the variations in object location and size, we reduce search space for semantic segmentation significantly and train the network with much less training examples successfully

**首先**，限制对象位置和大小的变化，大大减少了语义分割的搜索空间，成功地训练了较少的训练样本。即使用边界框住真实标记的目标，只在框内进行训练。

in the **second** stage, we utilize object proposals to construct more challenging examples

在第**二**阶段中，我们利用 object proposals来构造更具挑战性的例子。

语义分割的空间与训练样本的数量相比仍然非常大，使用反卷积网络进行实例细分的好处将不复存在。

#### **聚集实例分割图 aggregating instance-wise segmentation maps**

the proposed network is trained to perform semantic segmentation for individual instances. given an input image, we first generate a sufficient number of candidate proposals, and apply the trained network to obtain semantic segmentation maps of individual proposals. then we aggregate the outputs of all proposals to produce semantic segmentation on a whole image.

所提出的网络被训练为个别实例执行语义分割。给定输入图像，我们首先生成足够数量的候选方案，并应用经过训练的网络**获取各个方案的语义分割图**。然后，我们**汇总**所有建议的输出。

construct the pixel-wise class score map of an image by aggregating the outputs of all proposals. 

通过汇总所有建议的输出构造图像的像素级得分图

finally, we apply the fully-connected crf [14] to the output maps for the final pixel-wise labeling, where unary potential are obtained from the pixel-wise class conditional probability maps 

最后，我们将全连接的**CRF**[14]应用于最终像素标记的输出，其中CRF一元位势是从像素级的条件概率图中得到的。

> 
[14]. efficient-inference-in-fully-connected-crfs-with-gaussian-edge-potentials


**整合FCN：**

反卷积和FCN互补：反卷积网络注重细节，FCN注重全局

Given two sets of class conditional probability maps of an input image computed independently by the proposed method and FCN, we compute the mean of both output maps and apply the CRF to obtain the final semantic  segmentation

#### **训练：**

We initialize the weights in the convolution network using VGG 16-layer net pre-trained on ILSVRC [4] dataset, while the weights in the deconvolution network are initialized with zero-mean Gaussians.

测试： 

We employ edge-box [26] to generate object proposals. For each testing image






