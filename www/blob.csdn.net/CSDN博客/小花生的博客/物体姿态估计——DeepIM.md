
# 物体姿态估计——DeepIM - 小花生的博客 - CSDN博客


2018年10月06日 11:36:28[Peanut_范](https://me.csdn.net/u013841196)阅读数：2954


《DeepIM: Deep Iterative Matching for 6D Pose Estimation》
2018，Yi Li et al. DeepIM
1.引言：
本文，作者提出了一种新的深度神经网络对物体的6D姿态（3D位置和3D方向）进行估计，命名为DeepIM。采用对图像进行直接回归物体姿态的方式，准确率是有限的，通过匹配物体的渲染图像可以进一步提高准确率。即给定初始姿态估计，对合成RGB图像进行渲染来和目标输入图像进行匹配，然后在计算出新的更准的姿态估计。
1）Depth信息局限性：最新的技术已经使用深度相机（depth cameras）为物体的姿态估计，但这种相机在帧速率、视场、分辨率和深度范围等方面还存在相当大的局限性，一些小的、薄的、透明的或快速移动的物体检测起来还非常困难。
2）RGB信息局限性：基于RGB的6D物体姿态估计仍然具有挑战性，因为目标图像的表观会受光照、姿态变化、遮挡等影响
此外，鲁棒的6D姿态估计方法还需要能处理有纹理和无纹理的目标。
传统的6D姿态估计：将2D图像中提取的局部特征与待检测物体3D基准模型中的特征相匹配来求解R和T，也就是基于2D-3D对应关系求解PnP问题。但是，这种方法对局部特征依赖太强，不能很好地处理无纹理的目标。
为了处理无纹理目标，目前的文献有两类方法：
1）学习估计输入图像中的目标关键点或像素的3D模型坐标。
2）通过离散化姿态空间将6D姿态估计问题转化为姿态分类问题，或转化为姿态回归问题。
这些方法虽然能够处理无纹理目标，但是精度不够高。
2.DeepIM迭代结构：
![在这里插入图片描述](https://img-blog.csdn.net/20181006112021227?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
DeepIM：一种基于深度神经网络的迭代6D姿态匹配的新方法。给定测试图像中目标的初始6D姿态估计，DeepIM能够给出相对SE（3）变换符合目标渲染视图与观测图像之间的匹配关系。提高精度后的姿态估计迭代地对目标重新渲染，使得网络的两个输入图像会变得越来越相似，从而网络能够输出越来越精确的姿态估计。上图展示了作者提出网络用于姿态优化的迭代匹配过程。
这项工作主要的贡献有：
1）将深度网络引入到基于图像的迭代姿态优化问题，而无需任何手工制作的图像特征，其能够自动学习内部优化机制。
2）提出了一种旋转和平移解耦的SE（3）变换表示方法，能够实现精确的姿态估计，并且能使提出的方法适用于目标不在训练集时的姿态估计问题。
3）作者在LINEMOD和Occlusion数据集上进行了大量实验，以评估DeepIM的准确性和各种性能。
3.算法流程
如果目标在输入图像上是非常小的，它是困难的去提取有用的特征。如下图所示，作者为了获得足够的信息进行姿态匹配，对观测图像进行放大，并在输入网络前进行渲染。要注意的是，在每次迭代过程中，都会根据上一次得到的姿态估计来重新渲染，这样才能通过迭代来增加姿态估计的准确度。
Zoom in:
![在这里插入图片描述](https://img-blog.csdn.net/20181006112208535?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
DeepIM网络结构图：
![在这里插入图片描述](https://img-blog.csdn.net/20181006112252328?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
输入观测图像、渲染图像以及对应的掩模。使用FlowNetSimple网络第11个卷积层输出的特征图作为输入，然后连接两个全连接层FC256，最后旋转和平移的估计分别使用两个全连接层FC3和FC4作为输入。（作者也尝试使用VGG16图像分类网络来作为基础网络，但效果很差，直觉：估计光流是有用的对于姿态匹配。）
[
](https://img-blog.csdn.net/20181006112252328?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)4.Untangled Transformation Representation
通常使用四元数或欧拉角来表示旋转矩阵
$$
R_{\bigtriangleup}
$$
和一个向量表示平移
$$
t_{\bigtriangleup}
$$
，因此一个完成的转换矩阵可以写成
$$
[R_{\bigtriangleup}|t_{\bigtriangleup}]
$$
，给予一个原始目标姿态
$$
[R_{\bigtriangleup}|t_{src}]
$$
。
通常目标从初始位置到新位置的旋转与平移变换关系如下所示：
![在这里插入图片描述](https://img-blog.csdn.net/20181006112711758?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一般来说：旋转变换会影响影响最后的平移变换，即两者是耦合在一起的。
在本文中，作者让坐标轴平行于当前相机坐标轴，这样可以算得相对旋转，后续实验证明这样效果更好。还要解决相对平移估计的问题，一般的方法是直接在三维空间中计算原位置与新位置的xyz距离，但是这种方式既不利于网络训练，也不利于处理大小不一、边关相似的目标或未经训练的新目标。
估计相对的平移：
![在这里插入图片描述](https://img-blog.csdn.net/20181006112748905?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
本文采用在二维图像空间进行回归估计平移变换，vx和vy分别表示图像水平和垂直方向上像素移动的距离，vz表示目标的尺度变化。其中，fx和fy是相机的焦距，由于是常数，在实际训练中作者将其设置为1。
![在这里插入图片描述](https://img-blog.csdn.net/20181006112822978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样一来，旋转和平移进行解耦了，这种表示表示方法不需要目标的任何先验知识，并且能处理一些特殊情况，比如两个外观相似的物体，唯一的区别就是大小不一样。
[
](https://img-blog.csdn.net/20181006112822978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)5.Matching Loss
模型训练的损失函数，通常直接的方法是将旋转和平移分开计算，比如用角度距离表示旋转误差，L1距离表示平移误差，但这种分离的方法很容易让旋转和平移两种损失在训练时失衡。本文作者提出了一种同时计算旋转和平移的Point Matching Loss函数，来表示姿态真值和估计值之间的损失。其中，xj表示目标模型上的三维点，n表示总共用来计算损失函数的点个数，本文中n=3000。
![在这里插入图片描述](https://img-blog.csdn.net/20181006112905835?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后总的损失函数由
![在这里插入图片描述](https://img-blog.csdn.net/20181006112939279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
组成，其中
$$
(\alpha ,\beta ,\gamma )
$$
分别为（0.1,0.25,0.03）。
[
](https://img-blog.csdn.net/20181006112939279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)6.实验结果：
作者主要使用了LINEMOD和OCCLUSION数据集。
在LINEMOD数据集上作者分别使用了PoseCNN和Faster R-CNN初始化DeepIM网络，发现即使两个网络性能差异很大，但是经过DeepIM之后仍能得到差不多的结果。
![在这里插入图片描述](https://img-blog.csdn.net/20181006113114257?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
LINEMOD数据集上的方法对比结果如下表所示，本文的方法是最好的。
![在这里插入图片描述](https://img-blog.csdn.net/20181006113142940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
小结：
使用一种同时计算旋转和平移的Point Matching Loss 函数，来表示姿态真值和估计值之间的损失。
参考：
[https://mp.weixin.qq.com/s?__biz=MzAxMzc2NDAxOQ==&mid=2650368997&idx=1&sn=8db28f163c639153e32a18b855762ec8&chksm=83905f39b4e7d62fd6f35b1e71d8b74277ebffd9d918c7b960bbd6bdd9383386971bb6ede35b&scene=0\#rd](https://mp.weixin.qq.com/s?__biz=MzAxMzc2NDAxOQ==&mid=2650368997&idx=1&sn=8db28f163c639153e32a18b855762ec8&chksm=83905f39b4e7d62fd6f35b1e71d8b74277ebffd9d918c7b960bbd6bdd9383386971bb6ede35b&scene=0#rd)
将门创投公众号
---
注：博众家之所长，集群英之荟萃。

