
# 理解Spatial Transformer Network - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年08月10日 18:58:22[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：2144


[SIGAI飞跃计划第二期等你来挑战](https://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247485996&idx=1&sn=a886dd559a073dd4e53fb6590c9b0488&chksm=fdb697bbcac11ead35d988942ce654b0d86119b798ab1a25d9c9b4fbd6f3c27504b468f091a4#rd)
概述
随着深度学习的不断发展,卷积神经网络(CNN)作为计算机视觉领域的杀手锏,在几乎所有视觉相关任务中都展现出了超越传统机器学习算法甚至超越人类的能力。一系列CNN-based网络在classification、localization、semantic segmentation、action recognization等任务中都实现了state-of-art的结果。
对于计算机视觉任务来说,我们希望模型可以对于物体姿势或位置的变化具有一定的不变性,从而在不同场景下实现对于物体的分析。传统CNN中使用卷积和Pooling操作在一定程度上实现了平移不变性,但这种人工设定的变换规则使得网络过分的依赖先验知识,既不能真正实现平移不变性(不变性对于平移的要求很高),又使得CNN对于旋转,扭曲等未人为设定的几何变换缺乏应有的特征不变性。
![](https://img-blog.csdn.net/20180810185102768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
STN作为一种新的学习模块,具有以下特点:
(1) 为每一个输入提供一种对应的空间变换方式(如仿射变换)
(2) 变换作用于整个特征输入
(3) 变换的方式包括缩放、剪切、旋转、空间扭曲等等
具有可导性质的STN不需要多余的标注,能够自适应的学到对于不同数据的空间变换方式。它不仅可以对输入进行空间变换,同样可以作为网络模块插入到现有网络的任意层中实现对不同Feature map的空间变换。最终让网络模型学习了对平移、尺度变换、旋转和更多常见的扭曲的不变性,也使得模型在众多基准数据集上表现出了更好的效果。
空间变换网络:
![](https://img-blog.csdn.net/2018081018512499?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
ST的结构如上图所示,每一个ST模块由Localisation net, Grid generator和Sample组成, Localisation net决定输入所需变换的参数θ,Grid generator通过θ和定义的变换方式寻找输出与输入特征的映射T(θ),Sample结合位置映射和变换参数对输入特征进行选择并结合双线性插值进行输出,下面对于每一个组成部分进行具体介绍。
Localisation net:
Localisation net输入为一张Feature map:![U\in R^{H\times W\times C}](https://www.zhihu.com/equation?tex=U%5Cin%20R%5E%7BH%5Ctimes%20W%5Ctimes%20C%7D)。经过若干卷积或全链接操作后接一个回归层回归输出变换参数θ。θ的维度取决于网络选择的具体变换类型,如选择仿射变换则![\theta\in R^{2\times 3}](https://www.zhihu.com/equation?tex=%5Ctheta%5Cin%20R%5E%7B2%5Ctimes%203%7D)。如选择投影变换则![\theta\in R^{3\times 3}](https://www.zhihu.com/equation?tex=%5Ctheta%5Cin%20R%5E%7B3%5Ctimes%203%7D)。θ的值决定了网络选择的空间变换的”幅度大小”。
Grid generator:
Grid generator利用localisation层输出的θ, 对于Feature map进行相应的空间变换。设输入Feature map U每个像素位置的坐标为(![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D),![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D)),经过ST后输出Feature map每个像素位置的坐标为(![x_{i}^{t}，y_{i}^{t}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bt%7D%EF%BC%8Cy_%7Bi%7D%5E%7Bt%7D)), 那么输入和输出Feature map的映射关系便为(选择变换方式为仿射变换)
![](https://img-blog.csdn.net/20180810185156606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
也就是说,对于输出Feature map的每一个位置,我们对其进行空间变换(仿射变换)寻找其对应与输入Feature map的空间位置,到目前为止,如果这一步的输出为整数值(往往不可能),也就是经过变换后的坐标可以刚好对应原图的某些空间位置,那么ST的任务便完成了,既输入图像在Localisation net和Grid generator后先后的确定了空间变换方式和映射关系。
![](https://img-blog.csdn.net/20180810185245923?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是一些读者看到这可能有一个疑问,这个嵌入的ST网路如何通过反向传播进行参数的训练?没错,如果仅仅包含上述的两个过程,那么ST网络是无法进行反向传播的,原因就是我们上述的操作并不是直接对Feature map进行操作,而是对feature position进行计算,从而寻找输入到输出的对应关系。而feature position对应到feature score是离散的,即feature position进行微小变化时,输出O[x+△x,y]值是无法求解的(图像的计算机存储为离散的矩阵存储)。这里论文作者使用了笔者认为STN最精髓算法,双线性插值算法。
Sample:
经过以上的两步操作后,输出的Feature map上每一个像素点都会通过空间变换对应到输入Feature map的某个像素位置,但是由于feature score对于feature position的偏导数无法计算,因而我们需要构造一
![](https://img-blog.csdn.net/20180810185326385?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
种position->score的映射,且该映射具有可导的性质,从而满足反向传播的条件。即每一个输出的位置i,都有:
其中![U_{nm}](https://www.zhihu.com/equation?tex=U_%7Bnm%7D)为输入位置(n,m)对应的score值,k为某种可导函数, Φ为可导函数参数,通过如上的构造方式,我们便可以实现对于![\frac{\partial U}{\partial Xis^{’}}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial%20U%7D%7B%5Cpartial%20Xis%5E%7B%E2%80%99%7D%7D)，![\frac{\partial U}{\partial \theta}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial%20U%7D%7B%5Cpartial%20%5Ctheta%7D)等网络参数的求导,从而满足反向传播的要求。如
论文使用的双线性插值法公式如下:
![](https://img-blog.csdn.net/20180810185357186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们可以看到,通过max函数选择与输出(![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D)x_{i}^{s} ,![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D)y_{i}^{s} )距离小于1的像素位置,距离(![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D)x_{i}^{s} ,![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D)y_{i}^{s})越近的点被分配了越高的权重,实现了使用(![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D)x_{i}^{s} ,![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D)y_{i}^{s})周围四个点的score计算最终score,由于max函数可导,我们可以有如下偏导数计算公式:
![](https://img-blog.csdn.net/20180810185411629?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180810185422115?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对于![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D)y_{i}^{s}的求导与![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D)x_{i}^{s}类似,因而我们可以求得对于的偏导:
![](https://img-blog.csdn.net/20180810185433236?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
到目前为止,我们证明了ST模块可以通过反向传播完成对于网络梯度的计算与参数的更新。
算法分析(STN)
> (1) STN作为一种独立的模块可以在不同网络结构的任意节点插入任意个数并具有运算速度快的特点,它几乎没有增加原网络的运算负担,甚至在一些attentive model中实现了一定程度上的加速。

> (2) STN模块同样使得网络在训练过程中学习到如何通过空间变换来减少损失函数,使得模型的损失函数有着可观的减少。

> (3) STN模块决定如何进行空间变换的因素包含在Localisation net以及之前的所有网络层中。

> (4) 网络除了可以利用STN输出的Feature map外,同样可以将变换参数作为后面网络的输入,由于其中包含着变换的方式和尺度,因而可以从中得到原本特征的某些姿势或角度信息等。

> (5) 同一个网络结构中,不同的网络位置均可以插入STN模块,从而实现对与不同feature map的空间变换。

> (6) 同一个网络层中也可以插入多个STN来对于多个物体进行不同的空间变换,但这同样也是STN的一个问题:由于STN中包含crop的功能,所以往往同一个STN模块仅用于检测单个物体并会对其他信息进行剔除。同一个网络层中的STN模块个数在一定程度上影响了网络可以处理的最大物体数量。

实验结果:
论文中在手写数字识别、街景数字识别、高维度物体变换、鸟类识别等多个任务上都进行了实验,如对于手写数字识别:
![](https://img-blog.csdn.net/20180810185507454?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
原始数据集选择Mnist, 分别进行了旋转（R）、旋转、缩放、平移（RTS），透射变换(P）, 弹性变形（E）四种方式对数据集进行了预处理,选用FCN和CNN作为baseline,分别使用仿射变换（Aff )、透射变换（Proj )、以及薄板样条变换（TPS )的空间变换方式进行STN模块的构造,我们可以看出STN-based网络具有全面优于baseline的错误率。右图为部分输入数据经过STN变换后的结果。可以看出STN可以学习到多种原始数据的位置偏差并进行调整。
STN模块的Pytorch实现:
这里我们假设Mnist数据集作为网络输入:
(1)首先定义Localisation net的特征提取部分,为两个Conv层后接Maxpool和Relu操作:
![](https://img-blog.csdn.net/20180810185624469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
(2)定义Localisation net的变换参数θ回归部分,为两层全连接层内接Relu:
![](https://img-blog.csdn.net/20180810185636181?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
(3)在nn.module的继承类中定义完整的STN模块操作:
![](https://img-blog.csdn.net/20180810185542778?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考文献:
[1] Max Jaderberg, Karen Simonyan, Andrew Zisserman, Koray Kavukcuoglu. Spatial Transformer Networks. CVPR, 2016
[2] Ghassen HAMROUNI. Spatial Transformer Networks Tutorial:©Copyright2017,PyTorch. https://pytorch.org/tutorials/intermediate/spatial_transformer_tutorial.html
推荐阅读
[1][机器学习-波澜壮阔40年](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0413.
[2][学好机器学习需要哪些数学知识？](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0417.
[3][人脸识别算法演化史](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0420.
[4][基于深度学习的目标检测算法综述](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0424.
[5][卷积神经网络为什么能够称霸计算机视觉领](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)[域？](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0426.
[6][用一张图理解SVM的脉络 ](https://zhuanlan.zhihu.com/p/htt%3C/u%3Eps://zhuanlan.zhihu.com/p/htt%3C/u%3Eps://zhuanlan.zhihu.com/p/41392664/htt%3C/u%3Eps://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0428.
[7][人脸检测算法综述](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0503.
[8][理解神经网络的激活函数](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI2018.5.5.
[9][深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0508.
[10][理解梯度下降法](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0511.
[11][循环神经网络综述—语音识别与自然语言处理的利器](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0515
[12][理解凸优化](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0518
[13][【实验】理解SVM的核函数和参数](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0522
[14][【SIGAI综述】行人检测算法](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0525
[15][机器学习在自动驾驶中的应用—以百度阿波罗平台为例](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)(上)【获取码】SIGAI0529
[16][理解牛顿法](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0531
[17][【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题 ](http://link.zhihu.com/%3C/u%3E%3C/u%3E?target=https://zhuanlan.zhihu%3C/u%3E.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI 0601
[18][大话Adaboost算法](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0602
[19][FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0604
[20][理解主成分分析(PCA)](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0606
[21][人体骨骼关键点检测综述](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0608
[22][理解决策树](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0611
[23][用一句话总结常用的机器学习算法](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0611
[24][目标检测算法之YOLO](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0615
[25][理解过拟合](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0618
[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0620
[27][场景文本检测——CTPN算法介绍](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0622
[28][卷积神经网络的压缩和加速](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0625
[29][k近邻算法](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0627
[30][自然场景文本检测识别技术综述](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0627
[31][理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//zhuan%253C/u%253El%253Cu%253Ean.zhihu.com/p/41392664/%253C/u%253Eedit%23wechat_redirect)【获取码】SIGAI0704
[32][机器学习算法地图](https://zhuanlan.zhihu.com/p/41392664/edit#wechat_redirect)【获取码】SIGAI0706
[33][反向传播算法推导-全连接神经网络](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0709
[34][生成式对抗网络模型综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/e9wMKj8SgjtEWB9U7MM-9w)【获取码】SIGAI0709.
[35][怎样成为一名优秀的算法工程师](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/YMtnBAVDZepsMTO4h-VRtQ)【获取码】SIGAI0711.
[36][理解计算：从根号2到AlphaGo——第三季 神经网络的数学模型](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485592%26idx%3D1%26sn%3D1c5236972402ea8cb168161bc41e8e7e%26chksm%3Dfdb6950fcac11c19ad047e7cb9ced96447a85b41e21b10789a86ae4a211e4fb2ca1f911a7fc5%23rd)【获取码】SIGAI0716
[37][【技术短文】人脸检测算法之S3FD](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0716
[38][基于深度负相关学习的人群计数方法](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0718
[39][流形学习概述](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0723
[40][关于感受野的总结](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0723
[41][随机森林概述](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0725
[42][基于内容的图像检索技术综述——传统经典方法](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0727
[43][神经网络的激活函数总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485762%26idx%3D1%26sn%3De1e9fc75b92999177d3c61c655b0e06e%26chksm%3Dfdb694d5cac11dc37dac1a7ce32150836d66f0012f35a7e04e3dceaf626b8453dc39ee80172b%23rd)【获取码】SIGAI0730
[44][机器学习和深度学习中值得弄清楚的一些问题](https://zhuanlan.zhihu.com/p/41392664/edit#rd)【获取码】SIGAI0802
[45][基于深度神经网络的自动问答系统概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485842%26idx%3D1%26sn%3Dd7485054d6e93225b6ac0c77f8706cf7%26chksm%3Dfdb69405cac11d1355b84f692c2cbe49a3852a10e074b6941c95618598caea6ed64103c4ee4c%23rd)【获取码】SIGAI0806
[46][机器学习与深度学习核心知识点总结 写在校园招聘即将开始时](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486105%26idx%3D1%26sn%3Dd0b33e7e23b0e2fc442bd6b3e2a9d952%26chksm%3Dfdb6970ecac11e18085ea36f3b654028b2d4ba33a0cdc89c4ea25ac81570969f95f84c6939ac%23rd)【获取 码】SIGAI0808
原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的。


