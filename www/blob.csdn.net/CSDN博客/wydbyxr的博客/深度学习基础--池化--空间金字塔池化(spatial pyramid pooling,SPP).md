# 深度学习基础--池化--空间金字塔池化(spatial pyramid pooling,SPP) - wydbyxr的博客 - CSDN博客
2018年11月15日 10:15:09[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：409
# 空间金字塔池化(spatial pyramid pooling,SPP)
## 解决的问题
  先前方法，输入图片会经过裁切（Crop）或者变形缩放（Warp），这都在一定程度上导致图片信息的丢失和变形，限制了识别精确度。
  SPP可以输入任意大小的图片（不需要经过裁剪缩放等操作，去除网络输入是固定大小的限制），输出固定长度的representation（一维特征向量）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116093605307.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 意义
  R-CNN是直接从原始图片中提取特征，它在每张原始图片上提取2000个Region Proposal，然后对每一个候选区域框进行一次卷积计算，差不多要重复2000次。
  假设最后一个卷积层的输出大小为a×a，若给定金字塔层有n×n 个bins，进行滑动窗池化，窗口尺寸为win=a/n，步长为str=a/n，使用一个网络完成一个完整epoch的训练，之后切换到另外一个网络。
  只是在训练的时候用到多尺寸，测试时直接将SPPNet应用于任意尺寸的图像。
  多尺度特征提取出固定大小的特征向量。
## 具体操作
  SPP层作为pooling的代替。是取max的pooling。前一层是主干网络的最后一个卷积层，然后SPP产生固定大小的输出，后一层是第一个FC层。
  具体过程，其实就是从这21个图片块中，分别计算每个块的最大值，从而得到一个输出神经元。
最后把一张任意大小的图片转换成了一个固定大小的21维特征。
