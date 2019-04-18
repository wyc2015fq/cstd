# 理解Spatial Transformer Networks - 知乎
# 

本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

获取全文PDF请查看：[理解Spatial Transformer Networks](https://link.zhihu.com/?target=http%3A//www.sigai.cn/paper_88.html)

## 概述

随着深度学习的不断发展,卷积神经网络(CNN)作为计算机视觉领域的杀手锏,在几乎所有视觉相关任务中都展现出了超越传统机器学习算法甚至超越人类的能力。一系列CNN-based网络在classification、localization、semantic segmentation、action recognization等任务中都实现了state-of-art的结果。

对于计算机视觉任务来说,我们希望模型可以对于物体姿势或位置的变化具有一定的不变性,从而在不同场景下实现对于物体的分析。传统CNN中使用卷积和Pooling操作在一定程度上实现了平移不变性,但这种人工设定的变换规则使得网络过分的依赖先验知识,既不能真正实现平移不变性(不变性对于平移的要求很高),又使得CNN对于旋转,扭曲等未人为设定的几何变换缺乏应有的特征不变性。
![](https://pic2.zhimg.com/v2-ae99ba665abf84551227700e92a8a2d5_b.jpg)
STN作为一种新的学习模块,具有以下特点:

(1)   为每一个输入提供一种对应的空间变换方式(如仿射变换)

(2)   变换作用于整个特征输入

(3)   变换的方式包括缩放、剪切、旋转、空间扭曲等等

具有可导性质的STN不需要多余的标注,能够自适应的学到对于不同数据的空间变换方式。它不仅可以对输入进行空间变换,同样可以作为网络模块插入到现有网络的任意层中实现对不同Feature map的空间变换。最终让网络模型学习了对平移、尺度变换、旋转和更多常见的扭曲的不变性,也使得模型在众多基准数据集上表现出了更好的效果。

## 空间变换网络:
![](https://pic4.zhimg.com/v2-f89a142991f0aa025dd567ff840e9f83_b.jpg)
ST的结构如上图所示,每一个ST模块由Localisation net, Grid generator和Sample组成, Localisation net决定输入所需变换的参数θ,Grid generator通过θ和定义的变换方式寻找输出与输入特征的映射T(θ),Sample结合位置映射和变换参数对输入特征进行选择并结合双线性插值进行输出,下面对于每一个组成部分进行具体介绍。

## Localisation net:

Localisation net输入为一张Feature map: ![U\in R^{H\times W\times C}](https://www.zhihu.com/equation?tex=U%5Cin+R%5E%7BH%5Ctimes+W%5Ctimes+C%7D) 。经过若干卷积或全链接操作后接一个回归层回归输出变换参数θ。θ的维度取决于网络选择的具体变换类型,如选择仿射变换则 ![\theta\in R^{2\times 3}](https://www.zhihu.com/equation?tex=%5Ctheta%5Cin+R%5E%7B2%5Ctimes+3%7D) 。如选择投影变换则 ![\theta\in R^{3\times 3}](https://www.zhihu.com/equation?tex=%5Ctheta%5Cin+R%5E%7B3%5Ctimes+3%7D) 。θ的值决定了网络选择的空间变换的”幅度大小”。

## Grid generator:

Grid generator利用localisation层输出的θ, 对于Feature map进行相应的空间变换。设输入Feature map U每个像素位置的坐标为( ![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D) , ![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D) ),经过ST后输出Feature map每个像素位置的坐标为( ![x_{i}^{t}，y_{i}^{t}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bt%7D%EF%BC%8Cy_%7Bi%7D%5E%7Bt%7D)  ), 那么输入和输出Feature map的映射关系便为(选择变换方式为仿射变换)
![](https://pic2.zhimg.com/v2-81e1863c176d93bcc81caf199c4666d9_b.jpg)
也就是说,对于输出Feature map的每一个位置,我们对其进行空间变换(仿射变换)寻找其对应与输入Feature map的空间位置,到目前为止,如果这一步的输出为整数值(往往不可能),也就是经过变换后的坐标可以刚好对应原图的某些空间位置,那么ST的任务便完成了,既输入图像在Localisation net和Grid generator后先后的确定了空间变换方式和映射关系。
![](https://pic1.zhimg.com/v2-5628792ceb14692341176bb91d79cc5c_b.jpg)
但是一些读者看到这可能有一个疑问,这个嵌入的ST网路如何通过反向传播进行参数的训练?没错,如果仅仅包含上述的两个过程,那么ST网络是无法进行反向传播的,原因就是我们上述的操作并不是直接对Feature map进行操作,而是对feature position进行计算,从而寻找输入到输出的对应关系。而feature position对应到feature score是离散的,即feature position进行微小变化时,输出O[x+△x,y]值是无法求解的(图像的计算机存储为离散的矩阵存储)。这里论文作者使用了笔者认为STN最精髓算法,双线性插值算法。

## Sample:

经过以上的两步操作后,输出的Feature map上每一个像素点都会通过空间变换对应到输入Feature map的某个像素位置,但是由于feature score对于feature position的偏导数无法计算,因而我们需要构造一种position->score的映射,且该映射具有可导的性质,从而满足反向传播的条件。即每一个输出的位置i,都有:
![](https://pic3.zhimg.com/v2-9c50c918c32eeff9a3226a2e9fe111ee_b.jpg)
其中 ![U_{nm}](https://www.zhihu.com/equation?tex=U_%7Bnm%7D) 为输入位置(n,m)对应的score值,k为某种可导函数, Φ为可导函数参数,通过如上的构造方式,我们便可以实现对于 ![\frac{\partial U}{\partial Xis^{’}}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+U%7D%7B%5Cpartial+Xis%5E%7B%E2%80%99%7D%7D) ， ![\frac{\partial U}{\partial \theta}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+U%7D%7B%5Cpartial+%5Ctheta%7D) 等网络参数的求导,从而满足反向传播的要求。如

论文使用的双线性插值法公式如下:
![](https://pic3.zhimg.com/v2-382c6532831b8a53ec0fa227acd1199a_b.jpg)
我们可以看到,通过max函数选择与输出(![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D) ,![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D) )距离小于1的像素位置,距离(![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D) ,![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D))越近的点被分配了越高的权重,实现了使用(![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D) ,![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D))周围四个点的score计算最终score,由于max函数可导,我们可以有如下偏导数计算公式:
![](https://pic4.zhimg.com/v2-88e0264a25b5eef21a1aae167842f087_b.jpg)![](https://pic2.zhimg.com/v2-af76b4370964f316615f1faede4bd5d5_b.jpg)
对于![y_{i}^{s}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5E%7Bs%7D)的求导与![x_{i}^{s}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%5E%7Bs%7D)类似,因而我们可以求得对于的偏导:
![](https://pic4.zhimg.com/v2-0f4dd84a021862df27f37da9a606185f_b.jpg)
到目前为止,我们证明了ST模块可以通过反向传播完成对于网络梯度的计算与参数的更新。

算法分析(STN)

> (1)   STN作为一种独立的模块可以在不同网络结构的任意节点插入任意个数并具有运算速度快的特点,它几乎没有增加原网络的运算负担,甚至在一些attentive model中实现了一定程度上的加速。
(2)   STN模块同样使得网络在训练过程中学习到如何通过空间变换来减少损失函数,使得模型的损失函数有着可观的减少。
(3)   STN模块决定如何进行空间变换的因素包含在Localisation net以及之前的所有网络层中。
(4)  网络除了可以利用STN输出的Feature map外,同样可以将变换参数作为后面网络的输入,由于其中包含着变换的方式和尺度,因而可以从中得到原本特征的某些姿势或角度信息等。
(5)   同一个网络结构中,不同的网络位置均可以插入STN模块,从而实现对与不同feature map的空间变换。
(6)   同一个网络层中也可以插入多个STN来对于多个物体进行不同的空间变换,但这同样也是STN的一个问题:由于STN中包含crop的功能,所以往往同一个STN模块仅用于检测单个物体并会对其他信息进行剔除。同一个网络层中的STN模块个数在一定程度上影响了网络可以处理的最大物体数量。

## 实验结果:

论文中在手写数字识别、街景数字识别、高维度物体变换、鸟类识别等多个任务上都进行了实验,如对于手写数字识别:
![](https://pic3.zhimg.com/v2-90fa339217782e8e54fffbb6515bf9fe_b.jpg)
原始数据集选择Mnist, 分别进行了旋转（R）、旋转、缩放、平移（RTS），透射变换(P）, 弹性变形（E）四种方式对数据集进行了预处理,选用FCN和CNN作为baseline,分别使用仿射变换（Aff )、透射变换（Proj )、以及薄板样条变换（TPS )的空间变换方式进行STN模块的构造,我们可以看出STN-based网络具有全面优于baseline的错误率。右图为部分输入数据经过STN变换后的结果。可以看出STN可以学习到多种原始数据的位置偏差并进行调整。

STN模块的Pytorch实现:

这里我们假设Mnist数据集作为网络输入: 

(1)首先定义Localisation net的特征提取部分,为两个Conv层后接Maxpool和Relu操作:
![](https://pic1.zhimg.com/v2-924aa2c27788a00718b9fbdd072431ec_b.jpg)
(2)定义Localisation net的变换参数θ回归部分,为两层全连接层内接Relu:
![](https://pic4.zhimg.com/v2-9fbf581b44d98a3edd7c7e11b63d4c87_b.jpg)
(3)在nn.module的继承类中定义完整的STN模块操作:
![](https://pic4.zhimg.com/v2-8f202cf2354af2c64c7c6f55246f9de7_b.jpg)

参考文献:

[1] Max Jaderberg, Karen Simonyan, Andrew Zisserman, Koray Kavukcuoglu. Spatial Transformer Networks. CVPR, 2016

[2] Ghassen HAMROUNI. Spatial Transformer Networks Tutorial:©Copyright2017,PyTorch.                                    [https://pytorch.org/tutorials/intermediate/spatial_transformer_tutorial.html](https://link.zhihu.com/?target=https%3A//pytorch.org/tutorials/intermediate/spatial_transformer_tutorial.html)

推荐阅读

[1][机器学习-波澜壮阔40年](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0413.

[2][学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0417.

[3] [人脸识别算法演化史](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0420.

[4][基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0424.

[5][卷积神经网络为什么能够称霸计算机视觉领域？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0426.

[6] [用一张图理解SVM的脉络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0428.

[7] [人脸检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0503.

[8] [理解神经网络的激活函数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI2018.5.5.

[9] [深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0508.

[10] [理解梯度下降法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0511.

[11] [循环神经网络综述—语音识别与自然语言处理的利器](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0515

[12] [理解凸优化](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】 SIGAI0518

[13] [【实验】理解SVM的核函数和参数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0522

[14][【SIGAI综述】行人检测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484523%26idx%3D1%26sn%3Dddaa70c4b92f6005d9bbd6b3a3fe4571%26chksm%3Dfdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0525

[15] [机器学习在自动驾驶中的应用—以百度阿波罗平台为例(上)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0529

[16][理解牛顿法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0531

[17] [【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI 0601

[18] [大话Adaboost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0602

[19] [FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0604

[20] [理解主成分分析(PCA)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0606

[21] [人体骨骼关键点检测综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0608

[22][理解决策树](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0611

[23] [用一句话总结常用的机器学习算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0611

[24] [目标检测算法之YOLO](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0615

[25] [理解过拟合](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0618

[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484981%26idx%3D1%26sn%3Dd3003468b9853851923844812993e060%26chksm%3Dfdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0620

[27] [场景文本检测——CTPN算法介绍](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485005%26idx%3D1%26sn%3D0d4fb43b8db2a8046c64a9cfcbf3f478%26chksm%3Dfdb69bdacac112cce05c8b735b4f8b1ccf2348bea55a30af2055fc328958bb8f1ffd0f819bd2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0622

[28] [卷积神经网络的压缩和加速](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485042%26idx%3D1%26sn%3Dcdcf8d4b07acf64c7a6f5f7c1a731a12%26chksm%3Dfdb69be5cac112f377766984afb87313c1e1c58d94c80005f0f6f6af61ee5a4bd1bf6c6157b6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0625

[29] [k近邻算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485074%26idx%3D1%26sn%3D0ebf1bf8f49e9c46075fe3803d04c95d%26chksm%3Dfdb69b05cac112132d280c70af3923ca4c3cccfa5fcd8628b79d4b246b3b2decbc80a180abb3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[30][自然场景文本检测识别技术综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485142%26idx%3D1%26sn%3Dc0e01da30eb5e750be453eabe4be2bf4%26chksm%3Dfdb69b41cac11257ae22c7dac395e9651dab628fc35dd6d3c02d9566a8c7f5f2b56353d58a64%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[31] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485155%26idx%3D1%26sn%3D990cc7400751c36e9fef0a261e6add2a%26chksm%3Dfdb69b74cac112628bdae14c6435120f6fece20dae9bf7b1ffc8b8b25e5496a24160feca0a72%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0704

[32] [机器学习算法地图](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485306%26idx%3D1%26sn%3Dfc8cc8de313bdb61dcd39c1dedb240a4%26chksm%3Dfdb69aedcac113fb4b18c74248a313536ded50bade0e66b26f332ab247b148519da71ff2a3c0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0706

[33] [反向传播算法推导-全连接神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485446%26idx%3D1%26sn%3D57d7d866443810c20c4ea2c6ee8018cc%26chksm%3Dfdb69591cac11c8773638b396abe43c0161e4d339f0fa845e54326be3e8c4933a3b6a2713dae%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709

[34] [生成式对抗网络模型综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485551%26idx%3D1%26sn%3D213f48c4e55bee688cf0731097bb832c%26chksm%3Dfdb695f8cac11ceef3ef246c54d811dd64d8cc45fc75488c374c7aa95f72c1abfb55555ef0b7%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709.

[35][怎样成为一名优秀的算法工程师](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485570%26idx%3D1%26sn%3D5e71437a6d3ddf6d05144fc99e7633cc%26chksm%3Dfdb69515cac11c030cf713ec85293b7ad3bbe0f20d43fc2729cc976ff628aabf636834ccd904%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0711.

[36] [理解计算：从根号2到AlphaGo——第三季 神经网络的数学模型](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485592%26idx%3D1%26sn%3D1c5236972402ea8cb168161bc41e8e7e%26chksm%3Dfdb6950fcac11c19ad047e7cb9ced96447a85b41e21b10789a86ae4a211e4fb2ca1f911a7fc5%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0716

[37][【技术短文】人脸检测算法之S3FD](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485609%26idx%3D1%26sn%3Dd9068aecfbf150b40103210de538fea9%26chksm%3Dfdb6953ecac11c28361435306a7a09632ea79000abf1bf626e50afb3cda48eb3e47b96c6e7cd%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0716

[38] [基于深度负相关学习的人群计数方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485617%26idx%3D1%26sn%3D7955bfefc8e4b28221ae5247812f8235%26chksm%3Dfdb69526cac11c30e1051edc4378d7dfdedf46925236dbe33e7912b4bea882e515f074eee4c9%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0718

[39] [流形学习概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485668%26idx%3D1%26sn%3Df70547fc671d164e28fddcea6473524d%26chksm%3Dfdb69573cac11c65ee9fc98ac8fad093282a3d244748e7c88541c133ac55a32cb07472dc80e0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0723

[40] [关于感受野的总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485700%26idx%3D1%26sn%3Dc0425495fe0ae9cb2120dbcb246f49b1%26chksm%3Dfdb69493cac11d8542f7a8e662a7ecdeece4fd2270c71504023e8b58128575d1e4fdadf60cf5%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0723

[41] [随机森林概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485718%26idx%3D1%26sn%3Dc05c217af81173ae2c1301cbda5f7131%26chksm%3Dfdb69481cac11d975d86ff2e280371963d04b5709dfa0a9e874d637b7cf3844cad12be584094%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0725

[42] [基于内容的图像检索技术综述——传统经典方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485741%26idx%3D1%26sn%3Df8217e523d54842daaa4be38fa1792eb%26chksm%3Dfdb694bacac11dacfd8d7e4882166e2774c4685c043379ce0d2044e99c3b3c8b0140480bbf8e%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0727

[43] [神经网络的激活函数总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485762%26idx%3D1%26sn%3De1e9fc75b92999177d3c61c655b0e06e%26chksm%3Dfdb694d5cac11dc37dac1a7ce32150836d66f0012f35a7e04e3dceaf626b8453dc39ee80172b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0730

[44] [机器学习和深度学习中值得弄清楚的一些问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485811%26idx%3D1%26sn%3Da87082f0e47b01bb8c22443ba5b1139c%26chksm%3Dfdb694e4cac11df20ea1deb8b55cf297ad44e48a4c7ca45cfce387284143403fcd342ac98ec0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0802

[45] [基于深度神经网络的自动问答系统概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485842%26idx%3D1%26sn%3Dd7485054d6e93225b6ac0c77f8706cf7%26chksm%3Dfdb69405cac11d1355b84f692c2cbe49a3852a10e074b6941c95618598caea6ed64103c4ee4c%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0803

[46] [反向传播算法推导——卷积神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485971%26idx%3D1%26sn%3D632e3c135ead5f0ac730eee5b6a05647%26chksm%3Dfdb69784cac11e9258f67312fa2798e9c8210b8f77741a3d7bab4c1ccfa1c1f66632183f4f26%26token%3D1469111007%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0806

[47] [机器学习与深度学习核心知识点总结 写在校园招聘即将开始时](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486105%26idx%3D1%26sn%3Dd0b33e7e23b0e2fc442bd6b3e2a9d952%26chksm%3Dfdb6970ecac11e18085ea36f3b654028b2d4ba33a0cdc89c4ea25ac81570969f95f84c6939ac%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取 码】SIGAI0808

[48] [理解Spatial Transformer Networks](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486133%26idx%3D1%26sn%3D31c64e83511ad89929609dbbb0286890%26chksm%3Dfdb69722cac11e34da58fc2c907e277b1c3153a483ce44e9aaf2c3ed468386d315a9b606be40%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0810

[49][AI时代大点兵-国内外知名AI公司2018年最新盘点](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486140%26idx%3D1%26sn%3D6157951b026199422becca8863f18a17%26chksm%3Dfdb6972bcac11e3d7427847df818bd6cc7893a261daa0babe0b11bd01a403dc4f36e2b45650e%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0813

[50] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486202%26idx%3D1%26sn%3Dbf66be1e71272be42508e557ed93acbf%26chksm%3Dfdb6976dcac11e7b9d0f878233e8d9907825e57851e7a095f1be3a23abc9327370a77f4e2c03%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0815

[51] [基于内容的图像检索技术综述--CNN方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486268%26idx%3D1%26sn%3Ddff53feb3d78ea7698f70bede08b3b19%26chksm%3Dfdb696abcac11fbdcde5f4acc72d34c14119a21234b9e6cd0c1886b85c330e7f77d6e3da9122%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0817

[52][文本表示简介](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486289%26idx%3D1%26sn%3Da312c951c943ad104c722e6c681823b6%26chksm%3Dfdb696c6cac11fd01224a68b9d67fcf0043ff2de0ec67f7bfd98bacf691c8eaf221cbca9b4d6%26token%3D1485183944%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0820

[53][机器学习中的最优化算法总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486354%26idx%3D2%26sn%3D1afd5f272536b6771b5e1e224e8414ec%26chksm%3Dfdb69605cac11f13f2a16c8748e333045e99497dc03ca9f4741723204e7d8348e969375379ae%26token%3D1580317399%26lang%3Dzh_CN%23rd)【获取码】SIGAI0822

[54][【AI就业面面观】如何选择适合自己的舞台？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486371%26idx%3D1%26sn%3D955286243fe23ff58d5a090c166d83f6%26chksm%3Dfdb69634cac11f2251793b40432b5429828f5e72411e2ae48c1ae282117f2d1b4067c14fc889%26token%3D1383761413%26lang%3Dzh_CN%23rd)【获取码】SIGAI0823

[55][浓缩就是精华-SIGAI机器学习蓝宝书](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486392%26idx%3D1%26sn%3D81cc65e42418bf846167ab80ae1dd4f4%26chksm%3Dfdb6962fcac11f39d3c62bc06e8a0708a24a024e53aef38e1bd716dcd1433fa89a0c0fe422df%26token%3D867271861%26lang%3Dzh_CN%23rd)【获取码】SIGAI0824

[56][DenseNet详解](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486433%26idx%3D2%26sn%3D9858164d290b60c03081ee0b04d57a76%26chksm%3Dfdb69676cac11f60f48b9bce3017e2776767f3f9c2cc980e03c07dabc076a9ce8da4b906760d%26token%3D1199765642%26lang%3Dzh_CN%23rd)【获取码】SIGAI0827

原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的

