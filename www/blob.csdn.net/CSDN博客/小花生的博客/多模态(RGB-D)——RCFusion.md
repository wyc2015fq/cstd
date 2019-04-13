
# 多模态(RGB-D)——RCFusion - 小花生的博客 - CSDN博客


2018年10月04日 23:22:41[Peanut_范](https://me.csdn.net/u013841196)阅读数：423


《Recurrent Convolutional Fusion for RGB-D Object Recognition》
2018, Mohammad Reza Loghmani et al. RCFusion
作者通过从网络的不同levels来抽取特征，结合RGB和Depth的互补信息，来产生紧凑的和高辨别力的多模态特征。
1.RCFusion网络结构：
![在这里插入图片描述](https://img-blog.csdn.net/20181004230607801?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网络从CNNs的多个隐层来为RGB和Depth提取特征，另外使用正交损失来作用于RGB和Depth的特征，加强双流网络学习互补信息。
[
](https://img-blog.csdn.net/20181004230607801?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)2.RCFusion有三个主要的阶段：
阶段1：多级特征抽取：双流卷积网络结构，从网络的不同levels来分别提取RGB和Depth特征。
阶段2：特征映射和拼接：从RGB和Depth单独提取到的特征通过映射模块（projection blocks）
后拼接（concat），形成相对应的RGB-D特征。
阶段3：循环多模态融合：从CNNs的不同levels提取顺序的RGB-D特征，后送入RNN网络，产生一个紧凑的和有区分力的多模态特征。
3.实现细节：
1）RGB-/Depth-CNN: 采用ResNet-18，约40000 parameters，在ImageNet上进行预训练。
2）Projection blocks：映射模块转换feature maps成一个1xpd的特征向量，通过两个全卷层和一个全局最大池化层。映射模块的一个卷积集中在利用空间维度（高和宽），pd个7x7的滤波器；第二个卷积利用深度（depth），pd个1x1的滤波器；最后，全局最大池化作用在每一个channels上，形成特定pd维数的特征向量。
RNN：为了平衡网络的性能和参数的数量，作者使用RNN（GRU）来提取更加紧凑和有区分力的多模态特征，利用不同层级的时序空间特征。
4.Loss function：a standard classification loss and an orthogonality loss.
![在这里插入图片描述](https://img-blog.csdn.net/2018100423065432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中，
$$
L_{cls}
$$
cross-entropy loss为:
![在这里插入图片描述](https://img-blog.csdn.net/20181004230715609?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
$$
L_{\perp }
$$
：orthogonality loss为：
![在这里插入图片描述](https://img-blog.csdn.net/20181004231407512?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
$$
\lambda_{i}
$$
表示为第i层的正则项权重参数，
$$
||\cdot||_{F}^{2}
$$
is the squared Frobenius norm，T表示转置。
$$
L_{\perp }
$$
表示最小化不同模态的重叠信息，学习更多的不同模态间的互补信息。
参考代码：
def ortho_loss(model1, model2):
model1_T = tf.transpose(model1)
mul = tf.matmul(model1_T,model2)
ortho_loss = tf.nn.l2_loss(mul)
return ortho_loss
5.结果比较:
![在这里插入图片描述](https://img-blog.csdn.net/20181004231856499?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可知：正交损失的作用很小，抽取不同阶段的特征进行RNN提取特征会有不同效果，实际可能会根据不同的网络结构进行调试。
实验中：正交损失的权重设置为0.0001，pd=512.
测试RGB-D Object Dataset和JHUIT-50结果：
![在这里插入图片描述](https://img-blog.csdn.net/20181004231934654?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
小结：
引入正交损失和RGB-D单网络多级特征抽取和GRU融合多模态特征
---
注：博众家之所长，集群英之荟萃。

[
](https://img-blog.csdn.net/20181004231934654?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
