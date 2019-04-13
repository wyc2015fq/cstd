
# 年龄估计——Ranking-CNN - 小花生的博客 - CSDN博客


2018年08月06日 20:03:14[Peanut_范](https://me.csdn.net/u013841196)阅读数：965


《Using Ranking-CNN for Age Estimation》
CVPR2017，Shixing Chen et al，Ranking-CNN
本文提出了一种考虑与年龄有关的**顺序信息**，把年龄估计转化为一个排序问题，具体是通过一系列的二分类来实现，最后，通过合计二分类的结果来得到年龄预测结果。
（注：考虑了年龄之间的**时间顺序相关性**）
**框架结构：**
![这里写图片描述](https://img-blog.csdn.net/20180806195738420?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
论点：年龄的多分类完全忽略了年龄标签的顺序信息，年龄的回归过度简化成了线性模型。
Ranking-CNN针对不同的年龄段独立学习特征，使得学习的特征具有更有效的表现能力。
**一个基本的网络结构：**
![这里写图片描述](https://img-blog.csdn.net/20180806195807126?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806195807126?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由３个卷积和下采样，以及３个全连接层组成。C1 = 96个5*5卷积核，ReLU，S2=max pooling
3*3，stride=2，保留feature maps中最显著的特征，LRN，C3=256个5*5卷积核，ReLU，S4=S2，
LRN，C5=384个3*3的卷积核，S6=S2，F7=512个神经元，紧跟着ReLU和droput，F8=F7，
F9是二分类的输出。图像的输入256*256*3。
**一系列二分类后的年龄预测：**
![](https://img-blog.csdn.net/2018080619583452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018080619583452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**损失函数：**
![](https://img-blog.csdn.net/20180806195902324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806195902324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
文章中有很多公式推导，没有仔细研读。
[
](https://img-blog.csdn.net/20180806195902324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**测试结果：**
![](https://img-blog.csdn.net/20180806195927833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806195927833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从测试结果来看，效果优于DEX网络，但模型构建有点复杂。
[
](https://img-blog.csdn.net/20180806195927833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

###### 注：博众家之所长，集群英之荟萃。
[
						](https://img-blog.csdn.net/20180806195927833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/20180806195927833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
