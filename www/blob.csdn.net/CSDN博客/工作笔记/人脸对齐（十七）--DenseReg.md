# 人脸对齐（十七）--DenseReg - 工作笔记 - CSDN博客





2018年08月21日 14:23:09[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5455
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### DenseReg: Fully Convolutional Dense Shape Regression In-the-Wild

**原文：**[CVPR 2017](https://arxiv.org/abs/1612.01202)
**摘要：** 在本文中，我们提出通过完全卷积网络学习从图像像素到密集模板网格的映射。我们将此任务作为一个回归问题，并利用手动注释的面部标注来训练我们的网络。我们使用这样的标注，在三维对象模板和输入图像之间，建立密集的对应领域，然后作为训练我们的回归系统的基础。我们表明，我们可以将来自语义分割的想法与回归网络相结合，产生高精度的“量化回归”架构。我们的系统叫DenseReg，可以让我们以全卷积的方式估计密集的图像到模板的对应关系。因此，我们的网络可以提供有用的对应信息，而当用作统计可变形模型的初始化时，我们获得了标志性的本地化结果，远远超过当前最具挑战性的300W基准的最新技术。我们对大量面部分析任务的方法进行了全面评估，并且还展示了其用于其他估计任务的用途，如人耳建模。

![](https://img-blog.csdn.net/20180821133544170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上标h表示水平方向（作者的CNN有两个分支，分别是按图像的水平和垂直方向），

![](https://img-blog.csdn.net/20180821135615578?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，u表示真值，d表示量化分辨率，q就是离散值，r表示连续值，意思就是q表示整数部分，r表示小数部分，u是位置x。网络模型预测r，q,最后合成u。

感觉语义分割的思路，用在人脸对齐上，受资源限制太大了。

结果：

![](https://img-blog.csdn.net/20180821144120223?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





