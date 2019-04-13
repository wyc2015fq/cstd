
# 多模态(RGB-D)——FuseNet - 小花生的博客 - CSDN博客


2018年10月04日 18:59:07[Peanut_范](https://me.csdn.net/u013841196)阅读数：567


《FuseNet: Incorporating Depth into Semantic Segmentation via Fusion-based CNN Architecture》
2016，Hazirbas C, Ma L, Domokos C, et al. FuseNet
FuseNet 是一种基于RGBD图像做语意分割的网络，在网络的前端采用两个CNN结构分别提取RGB图像和深度图像的特征，采用Sparse Fusion的方式将深度网络的部分特征融合到RGB网络中。
1.FuseNet网络结构：
![在这里插入图片描述](https://img-blog.csdn.net/20181004184509270?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
作者提出了一种解决方法：如何去合并互补的深度信息到语义分割框架。双分支的网络同时从RGB和Depth提取特征，并将Depth特征融入RGB的特征图中作为下一层的输入。
[
](https://img-blog.csdn.net/20181004184509270?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)2. 融合策略
![在这里插入图片描述](https://img-blog.csdn.net/20181004184555274?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
作者提出了两种不同的RGB和Depth channels的融合方法，分别是dense and sparse fusion。
sparse fusion：融合层仅插入池化层之前
dense fusion：融合层插入CBR block之后
作者在文章中提出了一个理论，网络的浅层主要提取边缘特征，而RGB图像和深度图像所表现的边缘特征有很大不同。比如对与无结构的边缘主要依靠颜色区分，对于无纹理的边缘主要依靠结构深度区分。所以作者选择从浅层开始对特征进行融合并且通过实验证明浅层的特征融合对性能有很大的提升。
![在这里插入图片描述](https://img-blog.csdn.net/20181004184636349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.结果分析
图像输入224*224，RGB的resize采用双线性插值，Depth的resize采用最近邻插值。
网络结构：VGG-16
测试结果：
![在这里插入图片描述](https://img-blog.csdn.net/20181004184711902?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
SparseFusion and DenseFusion by SF，DF，respectively。
从结果可以看出：堆叠Depth和HHA进入RGB能带来轻微的提升，相比于单独的color、depth或HHA。融合的方式提升明显。
因为原始的VGG-16有5层池化，所以有相同数量的fusion layers SF1到SF5，然而在前4层池化层之前融合都有提升，可能原因是准确率饱和，Depth已经提供了非常有区分力的特征在低维去补充RGB的纹理信息。同样的趋势在DF中也存在。
小结：
与以往的多模态融合方式输入端、特征端融合略有不同，作者提出了dense and sparse fusion两种新的融合策略，在分支（双流）网络中进行间接性特征融合。从结果来看，采用Sparse Fusion的方式将深度网络的部分特征融合到RGB网络中效果比较好。
---
注：博众家之所长，集群英之荟萃。

