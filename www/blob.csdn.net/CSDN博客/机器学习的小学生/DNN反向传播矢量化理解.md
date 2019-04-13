
# DNN反向传播矢量化理解 - 机器学习的小学生 - CSDN博客


2018年04月23日 11:14:03[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：94


下面只是方便理解，在神经网络的实现中，假定输入$\mathbf X$，每一行是一个样本，即形状可以表示为$n\times d_1$, 其经过线性层得到$\mathbf Y$，其每一行是一个样本，则形状可以表示为$n\times d_2$，则用公式可以表示为：

$$
\mathbf Y = \mathbf X * W
$$
形状为：

$$
n \times d_2 = n \times d_1 * d_1 \times d_2
$$
理解反向传播的矢量化：
![这里写图片描述](https://img-blog.csdn.net/20180427160559969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

