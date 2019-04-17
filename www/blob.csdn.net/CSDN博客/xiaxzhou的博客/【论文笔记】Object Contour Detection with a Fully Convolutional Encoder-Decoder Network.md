# 【论文笔记】Object Contour Detection with a Fully Convolutional Encoder-Decoder Network - xiaxzhou的博客 - CSDN博客





2017年07月01日 11:26:27[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：817








### Object Contour Detection with a Fully Convolutional Encoder-Decoder Network

使用卷积编码解码网络检测主要目标的边缘

![这里写图片描述](https://img-blog.csdn.net/20170701112025867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

网络结构是： 

编码：VGG-16 

解码：反池化-卷积-激活-dropout
> 
卷积核:


The number of channels of every decoder layer is properly 

designed to allow unpooling from its corresponding maxpooling layer. 

> 
dropout:


We also add a dropout layer after each relu layer in the decoder. A complete decoder network setup is listed in Table 1 

> 
损失函数：


loss function is simply the pixel-wise logistic loss.

> 
反池化：


We initialize our encoder with VGG-16 net [48] (up to the “fc6” layer) and to achieve dense prediction of image size our decoder is constructed by alternating unpooling and convolution layers where unpooling layers **re-use the switches from max-pooling layers of encoder to upscale the feature maps**.

> 
反池化细节： 
[http://blog.csdn.net/xiaxzhou/article/details/74012137](http://blog.csdn.net/xiaxzhou/article/details/74012137)


训练： 

During training, we fix the encoder parameters (VGG-16) and only optimize decoder parameters. 




