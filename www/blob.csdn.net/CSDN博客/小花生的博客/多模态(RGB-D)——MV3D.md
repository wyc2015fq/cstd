
# 多模态(RGB-D)——MV3D - 小花生的博客 - CSDN博客


2018年10月04日 20:43:24[Peanut_范](https://me.csdn.net/u013841196)阅读数：430


《Multi-View 3D Object Detection Network for Autonomous Driving》
2017，Xiaozhi Chen, Huimin Ma et al. MV3D
在这篇文章，作者提出了一个多视角（Multi-View）的3D物体识别网络（MV3D），采用多模态的数据作为输入和预测3D空间的目标。
1.网络结构：
![在这里插入图片描述](https://img-blog.csdn.net/20181004203143209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用RGB图像、雷达鸟瞰图、雷达前视图三种作为网络的输入：
![在这里插入图片描述](https://img-blog.csdn.net/20181004203242751?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
实现精确的汽车的识别和3D框回归。
[
](https://img-blog.csdn.net/20181004203242751?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)2.Region based Fusion Network
本文提出了一个基于区域的融合网络，有效的结合特征。
Three views：bird’s eye view(BV), front view (FV), and the image plane (RGB).
因为特征从不同的视角/模态通常有不同的分辨率，作者采用ROI pooling 为每一个模态获得相同长度的特征向量。
Deep Fusion：从不同的特征去结合信息，之前的工作通常使用early fusion或late fusion。作者采用了一种深度融合（deep fusion）的方法，分层的融合多视角的特征。
Eg：一个L layers的网络结构
1）Early fusion结合特征从多视角（模态）的输入阶段：
![在这里插入图片描述](https://img-blog.csdn.net/20181004203430970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
连接的方式可以是concatenation或summation 等等
[
](https://img-blog.csdn.net/20181004203430970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)2）Late fusion使用几个子网络去独立的学习特征后在预测阶段进行结合：
![在这里插入图片描述](https://img-blog.csdn.net/2018100420345815?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181004203515255?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3）Deep fusion：提升不同视角中间层更多的交互：
![在这里插入图片描述](https://img-blog.csdn.net/20181004203600575?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
连接操作采用了element-wise mean。
[
](https://img-blog.csdn.net/20181004203600575?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)3.Network Regularization：
作者采用了两种方法去正则化region-based fusion network：drop-path training and auxiliary losses。
![在这里插入图片描述](https://img-blog.csdn.net/20181004203629254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.不同融合方法比较：
![在这里插入图片描述](https://img-blog.csdn.net/20181004203704557?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注：w/o为是否使用auxiliary loss
[
](https://img-blog.csdn.net/20181004203704557?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)小结：
引入了Deep fusion策略，增加中间层不同模态特征间的交互
---
注：博众家之所长，集群英之荟萃。

[
](https://img-blog.csdn.net/20181004203704557?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
