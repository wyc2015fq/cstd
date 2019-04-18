# 图像局部显著性—点特征（FREAK） - wishchinYang的专栏 - CSDN博客
2017年03月16日 13:31:05[wishchin](https://me.csdn.net/wishchin)阅读数：2022
         参考文章：[Freak特征提取算法](http://blog.csdn.net/hujingshuang/article/details/47060677)  圆形区域分割
一、Brisk特征的计算过程（参考对比）：
1.**建立尺度空间**：产生8层Octive层。
2.**特征点检测**：对这8张图进行FAST9-16角点检测，得到具有角点信息的8张图，对原图像img进行一次FAST5-8角点检测（当做d(-1)层，虚拟层），总共会得到9幅有角点信息的图像。
3.**非极大值抑制**：
4.**亚像素插值**：进过上面步骤，得到了图像特征点的位置和尺度，在极值点所在层及其上下层所对应的位置，对FAST得分值（共3个）进行二维二次函数插值（x、y方向），得到真正意义上的得分极值点及其精确的坐标位置（作为特征点位置）；再对尺度方向进行一维插值，得到极值点所对应的尺度（作为特征点尺度）。
5.**特征点描述**：使用如下图的砖块模式
![](https://img-blog.csdn.net/20170316135908925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5.1 通过计算局部梯度来计算主方向
5.2 根据主方向进行旋转，得到，512Bit的二进制编码，也就是64个字节（BRISK64）的Brisk特征。
二、FRESK特征的计算过程
步骤5之处，采样模式发生了变化，如下图：
![](https://img-blog.csdn.net/20170316140358601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        从图中可以看出，该结构是由很多大小不同并有重叠的圆构成，最中心的点是特征点，其它圆心是采样点，采样点离特征点的距离越远，采样点圆的半径越大，也表示该圆内的高斯函数半径越大。
特点：
        由于FREAK描述符自身的圆形对称采样结构使其具有旋转不变性，采样的位置好半径随着尺度的变化使其具有尺度不变性，对每个采样点进行高斯模糊，也具有一定的抗噪性能，像素点的强度对比生成二进制描述子使其具有光照不变性。因此由上述产生的二进制描述子可以用来进行特征匹配。在匹配之前，再补充一下特征点的方向信息。
三、OpenCV使用Freak描述子
参考一个Freak与ORB的对比评测：[http://blog.csdn.net/yang_xian521/article/details/7732835](http://blog.csdn.net/yang_xian521/article/details/7732835)
关于主方向的确定和匹配模式，请参考原文：[特征检测Freak检测算法](http://blog.csdn.net/hujingshuang/article/details/47060677)。
参考资料：FREAK原始论文《[FREAK: Fast Retina Keypoint](http://infoscience.epfl.ch/record/175537/files/2069.pdf)》
