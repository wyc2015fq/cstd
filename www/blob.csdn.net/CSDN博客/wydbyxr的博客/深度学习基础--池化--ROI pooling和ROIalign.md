# 深度学习基础--池化--ROI pooling和ROIalign - wydbyxr的博客 - CSDN博客
2018年11月16日 09:37:15[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：231
# Rol pooling
  RoI Pooling Layer是SPP-Layer的简化形式。SPP-Layer是空间金字塔Pooling层，包括不同的尺度；RoI Layer只包含一种尺度，如论文中所述7*7。
  在硬件上，ROIpooling相当于resize。
  这样对于RoI Layer的输入（r,c,h,w），RoI Layer首先产生7*7个r*c*(h/7)*(w/7)的Block(块)，然后用Max-Pool方式求出每一个Block的最大值，这样RoI Layer的输出是r*c*7*7。(r,c是原点的坐标)。
  参考资料：[http://blog.csdn.net/lanran2/article/details/60143861](http://blog.csdn.net/lanran2/article/details/60143861)
## Rol pooling layer的作用主要有两个
  1）将image中的rol定位到feature map中对应区域（patch），
  2）用一个单层的SPP layer将这个feature map patch下采样为大小固定的feature再传入全连接层。
## 具体操作
  ROI Pooling的输入有两部分组成：
  1）data：指的是进入RPN层之前的那个Conv层的Feature Map，通常我们称之为“share_conv”；
  2）rois：指的是RPN层的输出，一堆矩形框，形状为1x5x1x1（4个坐标+索引index），其中值得注意的是：坐标的参考系不是针对feature map这张图的，而是针对原图的（神经网络最开始的输入）。
  ROI Pooling的输出：
  输出是batch个vector，其中batch的值等于roi的个数，vector的大小为channelxwxh；ROI Pooling的过程就是将一个个大小不同的box矩形框，都映射成大小为wxh的矩形框；
  过程：
  先把roi中的坐标映射到feature map上，映射规则比较简单，就是把各个坐标除以输入图片与feature map的大小的比值，得到了feature map上的box坐标后，我们使用pooling得到输出。
# ROIalign
  ROIalign的实现方法是在每一个bin（float型）中采用适当次数的双线性插值，然后再做maxpooling，论文中采用的是每个bin采用四次双线性插值，论文中也指出每一个bin可以采用一次双线性插值，也可以采用大于四次的插值再做maxpooling。
