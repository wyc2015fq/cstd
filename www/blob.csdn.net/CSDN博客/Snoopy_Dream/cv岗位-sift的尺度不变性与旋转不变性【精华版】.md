# cv岗位 - sift的尺度不变性与旋转不变性【精华版】 - Snoopy_Dream - CSDN博客





2019年03月18日 20:20:48[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：36








参考自：

[https://blog.csdn.net/zddblog/article/details/7521424](https://blog.csdn.net/zddblog/article/details/7521424)

[https://blog.csdn.net/dcrmg/article/details/52561656](https://blog.csdn.net/dcrmg/article/details/52561656)



### **1.尺度不变性**

使用不同方差（不同的尺度空间）的LoG （高斯拉普拉斯算子），这样进行卷积具有**尺度不变的**性质，但是计算量很大，证明DoG（高斯差分算子）可以近似替代。

具体的过程是：制造高斯金字塔（**1.简单降采样+2.不同方差的高斯滤波**），然后在每个组octave内，进行上下差分，得到的差值就是DoG。 

右下图中，为高斯金字塔图像效果，分别是第1组的4层和第2组的4层（可以看到不同的组不同的层，尺度都不一）：

![](https://img-blog.csdnimg.cn/20190304230337785.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



**尺度空间**的概念：在高斯金字塔中一共生成**O组L层**不同尺度的图像，这两个量合起来**（O，L）就构成了高斯金字塔的尺度空间**，也就是说以高斯金字塔的组O作为二维坐标系的一个坐标，不同层L作为另一个坐标，则给定的一组坐标（O,L）就可以唯一确定高斯金字塔中的一幅图像。

![](https://img-blog.csdnimg.cn/20190304230350316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

然后就在高斯金字塔中找**极值点**，上下本层都比，如果是最值，就找到了。

![](https://img-blog.csdnimg.cn/20190304230358287.png)

### **2. 旋转不变性的原因**

![](https://img-blog.csdnimg.cn/20190304230405801.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



方向直方图的峰值则代表了该特征点（所在尺度空间内）邻域梯度的方向，创造了36段（0~360），以此做直方图，以直方图中最大值作为该关键点的主方向。为了增强匹配的鲁棒性，只保留峰值大于主方向峰值80％的方向作为该关键点的辅方向。然后，将**坐标轴旋转为关键点的方向**，以确保旋转不变性

![](https://img-blog.csdnimg.cn/20190304230439468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

旋转后邻域内采样点的新坐标为：

![](https://img-blog.csdnimg.cn/20190304230431804.png)






