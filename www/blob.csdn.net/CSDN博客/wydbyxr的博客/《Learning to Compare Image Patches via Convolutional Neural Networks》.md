# 《Learning to Compare Image Patches via Convolutional Neural Networks》 - wydbyxr的博客 - CSDN博客
2018年11月27日 10:22:26[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：34
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# 《Learning to Compare Image Patches via Convolutional Neural Networks》
  即2-channel networks。关于图像相似度计算的文章，本篇文章对经典的算法Siamese Networks 做了改进。
  因为文献基本上是在 Siamese Networks的基础上做修改，然后也要借助于空间金字塔池化实现不同大小图片的输入网络。
## 创新点1：从siamese 到2-channel
  在创新点方面，我觉得主要是把Siamese 网络的双分支，合在一起，从而提高了精度，如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181127102014546.png)
  这样算法的最后一层直接是全连接层，输出神经元个数直接为1，直接表示两张图片的相似度。
  把两张图片关联在一起的好处，作者的原话：This is something that indicates that it is important to jointly use information from both patches right from the first layer of the network.
## 创新点2：结合Central-surround two-stream network
  在网络输入部分，把输入图片改成多尺度输入。
  Central-surround two-stream network的意思就是把图片64*64的图片，处理成两张32*32图片，然后再输入网络，那么这两张32*32的图片是怎么计算得到的？这就是Central-surround方法，  第一张图片是通过以图片中心，进行裁剪出32*32的图片；
  第二张图片是怎么计算的：这张图片是直接通过对整张图片下采样的方法得到的，也就是说直接把64*64的图片进行下采样得到32*32的图片。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181127102057650.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 创新点3：结合空间金字塔池化SPP
  总之这一步是为了使得网络可以输入各种大小的图片，提高网络的实用性，鲁棒性等。		
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181127102205376.png)
