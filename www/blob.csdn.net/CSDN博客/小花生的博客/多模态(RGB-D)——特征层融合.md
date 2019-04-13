
# 多模态(RGB-D)——特征层融合 - 小花生的博客 - CSDN博客


2018年09月25日 23:06:32[Peanut_范](https://me.csdn.net/u013841196)阅读数：2632标签：[Multimodal																](https://so.csdn.net/so/search/s.do?q=Multimodal&t=blog)[RGB-D																](https://so.csdn.net/so/search/s.do?q=RGB-D&t=blog)[多模态																](https://so.csdn.net/so/search/s.do?q=多模态&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=RGB-D&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Multimodal&t=blog)个人分类：[人脸识别																](https://blog.csdn.net/u013841196/article/category/7769950)
[
																								](https://so.csdn.net/so/search/s.do?q=Multimodal&t=blog)


《Multimodal Deep Learning for Robust RGB-D Object Recognition》
2015，Andreas Eitel et al. 特征层融合
1.提出了一种新的RGB-D结构为目标识别（最后一层融合concat）：
![在这里插入图片描述](https://img-blog.csdn.net/20180925225227276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2. 数据准备
由网络结构图可知，Depth image的输入为3通道。
几种不同的深度图编码方式，如下图所示
几种最常见的方式：
（1）渲染depth数据到grayscale和重复grayscale到3个通道作为网络的输入
（2）使用表法向（x,y,z）来作为网络的输入
（3）HHA：encodes in the three channels the height above ground, horizontal disparity and the pixelwise angle between a surface normal and the gravity direction.
（4）本文提供的方法，效果优于HHA，首先归一化深度值到0-255，然后采用jet colormap 把1通道图像转成3通道图像（彩色化深度图），数值小（近）为红色，数值大（远）为蓝色，中间为渐变色。
![在这里插入图片描述](https://img-blog.csdn.net/20180925225326197?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.数据处理：
CNNs网络结构需要固定的输入尺寸，此时不同尺度的图像就要resize到固定的大小，但是会带来形变。本文是在不改变形状信息的前提下，对边界进行背景填充。
![在这里插入图片描述](https://img-blog.csdn.net/20180925225411147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.网络训练：
1）Training the stream networks
首先训练两个单独的网络（RGB和D），网络的最后一层为fc7（Fig.1所示），使用在ImageNet数据集上训练的参数进行fine tune。
2）Training the fusion network:
使用两个网络（RGB和D）来初始化融合的网络，两个fc7层进行concat后并入 融合网络。
5.实验结果：
对比融合网络和其他方法在RGB-D数据集上进行测试
![在这里插入图片描述](https://img-blog.csdn.net/20180925225527163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
小结：
多模态特征层融合的方法优于之前的方法
多模态的结果优于单模态的结果，提升较为明显
---
注：博众家之所长，集群英之荟萃。

[
  ](https://img-blog.csdn.net/20180925225527163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)