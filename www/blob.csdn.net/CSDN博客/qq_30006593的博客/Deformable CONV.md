# Deformable CONV - qq_30006593的博客 - CSDN博客





2017年09月24日 13:50:46[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：442








1.        Deform CNN

1.1.        Deformable Convolution Networks23



将输入经过卷积，生成一个与输入同样空间大小，2倍于卷积核大小深度的偏置，在卷积过程中对于不同的卷积核与不同的输入，使用相对应的偏置来采样新的点。

![](https://img-blog.csdn.net/20170924135123724)

对于3D情况:

In: NCLHW

 Kernel: C”CL’H’W’

 Out: NC”L”H”W”

 Offset: NCL’H’W’L”H”W”3

Y=WC: 将输入转换为col矩阵，与权重矩阵相乘就是结果

C=SUM(X*G(p,q)): 在转换过程中偏移后的点的值为其临近整数点值的加权平均。

G(p,q): 山峰状

![](https://img-blog.csdn.net/20170924135129745)

1.2.        Irregular Convolutional NeuralNetworks24

多一个参数P来记录卷积核各个点的位置，随着卷积核一起学习，相比于defrom不用添加额外的卷积核。

     文章相比于deform对卷积的细节有更多的描述。

1.3.        STN25

![](https://img-blog.csdn.net/20170924135140732)

1.4.        ISTN26

![](https://img-blog.csdn.net/20170924135145027)

相比与STN只做一次变换。

Reference：



23.   Dai,J. *et al.* Deformable Convolutional Networks.
*ArXiv Prepr.ArXiv170306211* (2017).

24.   Ma,J., Wang, W. & Wang, L. Irregular Convolutional Neural Networks.
*ArXivPrepr. ArXiv170607966* (2017).

25.   Jaderberg,M., Simonyan, K., Zisserman, A. & others. Spatial transformer networks. in
*Advancesin Neural Information Processing Systems* 2017–2025 (2015).

26.   Lin,C.-H. & Lucey, S. Inverse Compositional Spatial Transformer Networks.
*ArXivPrepr. ArXiv161203897* (2016).




