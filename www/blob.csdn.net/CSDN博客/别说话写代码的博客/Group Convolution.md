# Group Convolution - 别说话写代码的博客 - CSDN博客





2019年03月06日 15:48:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：29








Group convolution: 群卷积。

群卷积最早出现在alexnet中，作者认为group conv的方式能够增加filter之间的对角相关性，而且能够减少训练参数，不容易过拟合，类似于正则效果。

**假设上一层的输出feature mao有N个，也就是N个channel（卷积核）。再假设群卷积的群数目M。那么该卷积层的操作就是，先将channel分为M份，每个group对应N/M个channel。各个group卷积完成后将输出concat在一起，作为这一层的输出channel。**

比如下图是一个正常的、没有分组的卷积层结构。下图用三维图形展示了CNN的结构，一个filter对应一个输出channel。随着网络层数加深，通道数急剧增加，而空间维度随之减少，因为卷积层的卷积核越来越多，随着池化操作特征图越来越小。所以在深层网络中channel的重要性越来越大。

![](https://img-blog.csdnimg.cn/20190306154620778.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

下图是一个群卷积的CNN结构。filters呗分成了两个froup。每一个group都只有原来一般的feature map

![](https://img-blog.csdnimg.cn/20190306154633397.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

**其实使用group convolution会导致信息不流畅，也就是输出的feature map没有包含所有输入feature map的信息。在mobilenet中，depth-wise convolution也会导致这个问题，在mobilenet中使用point-wise （逐点卷积也就是分离卷积）来解决这个问题。shufflenet中作者使用channel shuffle来 解决这个问题。**参考[https://blog.csdn.net/qq_21997625/article/details/88240828](https://blog.csdn.net/qq_21997625/article/details/88240828)

下图为何凯明的group normalization中的图，大家可以立体理解一下

![](https://img-blog.csdnimg.cn/2019030615464663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

参考：

[https://blog.yani.io/filter-group-tutorial/](https://blog.yani.io/filter-group-tutorial/)

[https://blog.csdn.net/hhy_csdn/article/details/80030468](https://blog.csdn.net/hhy_csdn/article/details/80030468)

[https://blog.csdn.net/qq_21997625/article/details/88239165](https://blog.csdn.net/qq_21997625/article/details/88239165)



