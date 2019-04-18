# Deeper Bottleneck(瓶颈) Architectures - wydbyxr的博客 - CSDN博客
2018年11月28日 10:07:53[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：90
# Deeper Bottleneck(瓶颈) Architectures
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128100550621.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  看50-layer那一栏，在进入到DBA层之前的网络比较简单，分别是：①卷积层"7×7, 64, stride 2"、②BN层、③ReLU层、④池化层"3×3 max pool, stride 2"，最终的输出结果是一个大小为 [batch_size, height, width, kernels] 矩阵。	
  再看conv2_x层（即第一个bottleneck），该结构总共3×3=9层，可是原作毕竟篇幅有限，网络实现的细节不是很清楚，于是我就参考了Ryan Dahl的tensorflow-resnet程序源码，按照Ryan Dahl实现的ResNet，画出了DBA内部网络的具体实现，这个DBA是全网络中第一个DBA的前三层，输入的image大小为[batch_size,56,56,64]，输出大小为[batch_size,56,56,256]，如下图是DBA的结构（Bottleneck V1 ）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128100729844.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128100739557.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
