
# DenseNet详解 - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年08月27日 16:47:24[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：4050


本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
[书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
[书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)
## 一、概述
作为CVPR2017年的Best Paper, DenseNet脱离了加深网络层数(ResNet)和加宽网络结构(Inception)来提升网络性能的定式思维,从特征的角度考虑,通过特征重用和旁路(Bypass)设置,既大幅度减少了网络的参数量,又在一定程度上缓解了gradient vanishing问题的产生.结合信息流和特征复用的假设,DenseNet当之无愧成为2017年计算机视觉顶会的年度最佳论文.
卷积神经网络在沉睡了近20年后,如今成为了深度学习方向最主要的网络结构之一.从一开始的只有五层结构的LeNet, 到后来拥有19层结构的VGG, 再到首次跨越100层网络的Highway Networks与ResNet, 网络层数的加深成为CNN发展的主要方向之一.
随着CNN网络层数的不断增加,gradient vanishing和model degradation问题出现在了人们面前,BatchNormalization的广泛使用在一定程度上缓解了gradient vanishing的问题,而ResNet和Highway Networks通过构造恒等映射设置旁路,进一步减少了gradient vanishing和model degradation的产生.Fractal Nets通过将不同深度的网络并行化,在获得了深度的同时保证了梯度的传播,随机深度网络通过对网络中一些层进行失活,既证明了ResNet深度的冗余性,又缓解了上述问题的产生. 虽然这些不同的网络框架通过不同的实现加深的网络层数,但是他们都包含了相同的核心思想,既将feature map进行跨网络层的连接.
![](https://img-blog.csdn.net/20180827164003429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
DenseNet作为另一种拥有较深层数的卷积神经网络,具有如下优点:
(1) 相比ResNet拥有更少的参数数量.
(2) 旁路加强了特征的重用.
(3) 网络更易于训练,并具有一定的正则效果.
(4) 缓解了gradient vanishing和model degradation的问题.
何恺明先生在提出ResNet时做出了这样的假设:若某一较深的网络多出另一较浅网络的若干层有能力学习到恒等映射,那么这一较深网络训练得到的模型性能一定不会弱于该浅层网络.通俗的说就是如果对某一网络中增添一些可以学到恒等映射的层组成新的网路,那么最差的结果也是新网络中的这些层在训练后成为恒等映射而不会影响原网络的性能.同样DenseNet在提出时也做过假设:与其多次学习冗余的特征,特征复用是一种更好的特征提取方式.

## 二、DenseNet
假设输入为一个图片![X_{0}](https://www.zhihu.com/equation?tex=X_%7B0%7D), 经过一个L层的神经网络, 其中第i层的非线性变换记为![H_{i}](https://www.zhihu.com/equation?tex=H_%7Bi%7D)(*),![H_{i}](https://www.zhihu.com/equation?tex=H_%7Bi%7D)(*)可以是多种函数操作的累加如BN、ReLU、Pooling或Conv等. 第i层的特征输出记作![X_{i}](https://www.zhihu.com/equation?tex=X_%7Bi%7D).
> ResNet
传统卷积前馈神经网络将第i层的输出![X_{i}](https://www.zhihu.com/equation?tex=X_%7Bi%7D)作为i+1层的输入,可以写作![X_{i}](https://www.zhihu.com/equation?tex=X_%7Bi%7D)=![H_{i}](https://www.zhihu.com/equation?tex=H_%7Bi%7D)(![X_{i-1}](https://www.zhihu.com/equation?tex=X_%7Bi-1%7D)). ResNet增加了旁路连接,可以写作
![X_{l}](https://www.zhihu.com/equation?tex=X_%7Bl%7D)=![X_{l}](https://www.zhihu.com/equation?tex=X_%7Bl%7D)(![X_{l-1}](https://www.zhihu.com/equation?tex=X_%7Bl-1%7D))+![X_{l-1}](https://www.zhihu.com/equation?tex=X_%7Bl-1%7D)
ResNet的一个最主要的优势便是梯度可以流经恒等函数来到达靠前的层.但恒等映射和非线性变换输出的叠加方式是相加, 这在一定程度上破坏了网络中的信息流.
> Dense Connectivity
为了进一步优化信息流的传播,DenseNet提出了图示的网络结构
![](https://img-blog.csdn.net/20180827164127931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如图所示,第i层的输入不仅与i-1层的输出相关,还有所有之前层的输出有关.记作:
![X_{l}](https://www.zhihu.com/equation?tex=X_%7Bl%7D)=![H_{l}](https://www.zhihu.com/equation?tex=H_%7Bl%7D)([![X_{0}](https://www.zhihu.com/equation?tex=X_%7B0%7D),![X_{1}](https://www.zhihu.com/equation?tex=X_%7B1%7D),…,![X_{l-1}](https://www.zhihu.com/equation?tex=X_%7Bl-1%7D)]),
其中[]代表concatenation(拼接),既将![X_{0}](https://www.zhihu.com/equation?tex=X_%7B0%7D)到![X_{l-1}](https://www.zhihu.com/equation?tex=X_%7Bl-1%7D)层的所有输出feature map按Channel组合在一起.这里所用到的非线性变换H为BN+ReLU+ Conv(3×3)的组合.
> Pooling Layers
由于在DenseNet中需要对不同层的feature map进行cat操作,所以需要不同层的feature map保持相同的feature size,这就限制了网络中Down sampling的实现.为了使用Down sampling,作者将DenseNet分为多个Denseblock,如下图所示:
![](https://img-blog.csdn.net/20180827164206908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在同一个Denseblock中要求feature size保持相同大小,在不同Denseblock之间设置transition layers实现Down sampling, 在作者的实验中transition layer由BN + Conv(1×1) ＋2×2 average-pooling组成.
> Growth rate
在Denseblock中,假设每一个非线性变换H的输出为K个feature map, 那么第i层网络的输入便为K*0*+(i-1)×K, 这里我们可以看到DenseNet和现有网络的一个主要的不同点:DenseNet可以接受较少的特征图数量作为网络层的输出,如下图所示
![](https://img-blog.csdn.net/20180827164238251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
原因就是在同一个Denseblock中的每一层都与之前所有层相关联,如果我们把feature看作是一个Denseblock的全局状态,那么每一层的训练目标便是通过现有的全局状态,判断需要添加给全局状态的更新值.因而每个网络层输出的特征图数量K又称为Growth rate,同样决定着每一层需要给全局状态更新的信息的多少.我们之后会看到,在作者的实验中只需要较小的K便足以实现state-of-art的性能.
> Bottleneck Layers
虽然DenseNet接受较少的k,也就是feature map的数量作为输出,但由于不同层feature map之间由cat操作组合在一起,最终仍然会是feature map的channel较大而成为网络的负担.作者在这里使用1×1 Conv(Bottleneck)作为特征降维的方法来降低channel数量,以提高计算效率.经过改善后的非线性变换变为BN-ReLU-Conv(1×1)-BN-ReLU-Conv(3×3),使用Bottleneck layers的DenseNet被作者称为DenseNet-B.在实验中,作者使用1×1卷积生成channel数量为4k的feature map.
> Compression
为了进一步优化模型的简洁性,我们同样可以在transition layer中降低feature map的数量.若一个Denseblock中包含m个feature maps,那么我们使其输出连接的transition layer层生成⌊θm⌋个输出feature map.其中θ为Compression factor, 当θ=1时,transition layer将保留原feature维度不变.
作者将使用compression且θ=0.5的DenseNet命名为DenseNet-C, 将使用Bottleneck和compression且θ=0.5的DenseNet命名为DenseNet-BC

## 三、 算法分析
> (1) Model compactness
由于DenseNet对输入进行cat操作,一个直观的影响就是每一层学到的feature map都能被之后所有层直接使用,这使得特征可以在整个网络中重用,也使得模型更加简洁.
![](https://img-blog.csdn.net/20180827164317605?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从上图中我们可以看出DenseNet的参数效率:左图包含了对多种DenseNet结构参数和最终性能的统计,我们可以看出当模型实现相同的test error时,原始的DenseNet往往要比DenseNet-BC拥有2-3倍的参数量.中间图为DenseNet-BC与ResNet的对比,在相同的模型精度下,DenseNet-BC只需要ResNet约三分之一的参数数量.右图为1001层超过10M参数量的ResNet与100层只有0.8M参数量的DenseNet-BC在训练时的对比,虽然他们在约相同的训练epoch时收敛,但DenseNet-BC却只需要ResNet不足十分之一的参数量.
> (2) Implicit Deep Supervision
解释DenseNet为何拥有如此高性能的另一个原因是网络中的每一层不仅接受了原始网络中来自loss的监督,同时由于存在多个bypass与shortcut,网络的监督是多样的.Deep supervision的优势同样在deeply-supervised nets (DSN)中也被证实.(DSN中每一个Hidden layer都有一个分类器,强迫其学习一些有区分度的特征).与DSN不同的是,DenseNet拥有单一的loss function, 模型构造和梯度计算更加简易.
> (3) Feature Reuse
在设计初,DenseNet便被设计成让一层网络可以使用所有之前层网络feature map的网络结构,为了探索feature的复用情况,作者进行了相关实验.作者训练的L=40,K=12的DenseNet,对于任意Denseblock中的所有卷积层,计算之前某层feature map在该层权重的绝对值平均数.这一平均数表明了这一层对于之前某一层feature的利用率,下图为由该平均数绘制出的热力图:
![](https://img-blog.csdn.net/20180827164359337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从图中我们可以得出以下结论:
a) 一些较早层提取出的特征仍可能被较深层直接使用
b) 即使是Transition layer也会使用到之前Denseblock中所有层的特征
c) 第2-3个Denseblock中的层对之前Transition layer利用率很低,说明transition layer输出大量冗余特征.这也为DenseNet-BC提供了证据支持,既Compression的必要性.
d) 最后的分类层虽然使用了之前Denseblock中的多层信息,但更偏向于使用最后几个feature map的特征,说明在网络的最后几层,某些high-level的特征可能被产生.

## 四、实验结果
作者在多个benchmark数据集上训练了多种DenseNet模型,并与state-of-art的模型(主要是ResNet和其变种)进行对比:
![](https://img-blog.csdn.net/20180827164443120?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由上表我们可以看出,DenseNet只需要较小的Growth rate(12,24)便可以实现state-of-art的性能,结合了Bottleneck和Compression的DenseNet-BC具有远小于ResNet及其变种的参数数量,且无论DenseNet或者DenseNet-BC,都在原始数据集和增广数据集上实现了超越ResNet的性能.
推荐阅读
[1][机器学习-波澜壮阔40年](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0413.
[2][学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0417.
[3][人脸识别算法演化史](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0420.
[4][基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0424.
[5][卷积神经网络为什么能够称霸计算机视觉领域？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0426.
[6][用一张图理解SVM的脉络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0428.
[7][人脸检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0503.
[8][理解神经网络的激活函数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI2018.5.5.
[9][深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0508.
[10][理解梯度下降法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0511.
[11][循环神经网络综述—语音识别与自然语言处理的利器](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0515
[12][理解凸优化](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】 SIGAI0518
[13][【实验】理解SVM的核函数和参数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0522
[14][【SIGAI综述】行人检测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484523%26idx%3D1%26sn%3Dddaa70c4b92f6005d9bbd6b3a3fe4571%26chksm%3Dfdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0525
[15][机器学习在自动驾驶中的应用—以百度阿波罗平台为例(上)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0529
[16][理解牛顿法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0531
[17][【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI 0601
[18][大话Adaboost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0602
[19][FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0604
[20][理解主成分分析(PCA)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0606
[21][人体骨骼关键点检测综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0608
[22][理解决策树](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0611
[23][用一句话总结常用的机器学习算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0611
[24][目标检测算法之YOLO](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0615
[25][理解过拟合](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0618
[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484981%26idx%3D1%26sn%3Dd3003468b9853851923844812993e060%26chksm%3Dfdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0620
[27][场景文本检测——CTPN算法介绍](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485005%26idx%3D1%26sn%3D0d4fb43b8db2a8046c64a9cfcbf3f478%26chksm%3Dfdb69bdacac112cce05c8b735b4f8b1ccf2348bea55a30af2055fc328958bb8f1ffd0f819bd2%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0622
[28][卷积神经网络的压缩和加速](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485042%26idx%3D1%26sn%3Dcdcf8d4b07acf64c7a6f5f7c1a731a12%26chksm%3Dfdb69be5cac112f377766984afb87313c1e1c58d94c80005f0f6f6af61ee5a4bd1bf6c6157b6%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0625
[29][k近邻算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485074%26idx%3D1%26sn%3D0ebf1bf8f49e9c46075fe3803d04c95d%26chksm%3Dfdb69b05cac112132d280c70af3923ca4c3cccfa5fcd8628b79d4b246b3b2decbc80a180abb3%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0627
[30][自然场景文本检测识别技术综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485142%26idx%3D1%26sn%3Dc0e01da30eb5e750be453eabe4be2bf4%26chksm%3Dfdb69b41cac11257ae22c7dac395e9651dab628fc35dd6d3c02d9566a8c7f5f2b56353d58a64%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0627
[31][理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485155%26idx%3D1%26sn%3D990cc7400751c36e9fef0a261e6add2a%26chksm%3Dfdb69b74cac112628bdae14c6435120f6fece20dae9bf7b1ffc8b8b25e5496a24160feca0a72%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0704
[32][机器学习算法地图](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485306%26idx%3D1%26sn%3Dfc8cc8de313bdb61dcd39c1dedb240a4%26chksm%3Dfdb69aedcac113fb4b18c74248a313536ded50bade0e66b26f332ab247b148519da71ff2a3c0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0706
[33][反向传播算法推导-全连接神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485446%26idx%3D1%26sn%3D57d7d866443810c20c4ea2c6ee8018cc%26chksm%3Dfdb69591cac11c8773638b396abe43c0161e4d339f0fa845e54326be3e8c4933a3b6a2713dae%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709
[34][生成式对抗网络模型综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485551%26idx%3D1%26sn%3D213f48c4e55bee688cf0731097bb832c%26chksm%3Dfdb695f8cac11ceef3ef246c54d811dd64d8cc45fc75488c374c7aa95f72c1abfb55555ef0b7%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709.
[35][怎样成为一名优秀的算法工程师](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485570%26idx%3D1%26sn%3D5e71437a6d3ddf6d05144fc99e7633cc%26chksm%3Dfdb69515cac11c030cf713ec85293b7ad3bbe0f20d43fc2729cc976ff628aabf636834ccd904%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0711.
[36][理解计算：从根号2到AlphaGo——第三季 神经网络的数学模型](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485592%26idx%3D1%26sn%3D1c5236972402ea8cb168161bc41e8e7e%26chksm%3Dfdb6950fcac11c19ad047e7cb9ced96447a85b41e21b10789a86ae4a211e4fb2ca1f911a7fc5%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0716
[37][【技术短文】人脸检测算法之S3FD](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485609%26idx%3D1%26sn%3Dd9068aecfbf150b40103210de538fea9%26chksm%3Dfdb6953ecac11c28361435306a7a09632ea79000abf1bf626e50afb3cda48eb3e47b96c6e7cd%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0716
[38][基于深度负相关学习的人群计数方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485617%26idx%3D1%26sn%3D7955bfefc8e4b28221ae5247812f8235%26chksm%3Dfdb69526cac11c30e1051edc4378d7dfdedf46925236dbe33e7912b4bea882e515f074eee4c9%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0718
[39][流形学习概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485668%26idx%3D1%26sn%3Df70547fc671d164e28fddcea6473524d%26chksm%3Dfdb69573cac11c65ee9fc98ac8fad093282a3d244748e7c88541c133ac55a32cb07472dc80e0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0723
[40][关于感受野的总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485700%26idx%3D1%26sn%3Dc0425495fe0ae9cb2120dbcb246f49b1%26chksm%3Dfdb69493cac11d8542f7a8e662a7ecdeece4fd2270c71504023e8b58128575d1e4fdadf60cf5%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0723
[41][随机森林概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485718%26idx%3D1%26sn%3Dc05c217af81173ae2c1301cbda5f7131%26chksm%3Dfdb69481cac11d975d86ff2e280371963d04b5709dfa0a9e874d637b7cf3844cad12be584094%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0725
[42][基于内容的图像检索技术综述——传统经典方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485741%26idx%3D1%26sn%3Df8217e523d54842daaa4be38fa1792eb%26chksm%3Dfdb694bacac11dacfd8d7e4882166e2774c4685c043379ce0d2044e99c3b3c8b0140480bbf8e%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0727
[43][神经网络的激活函数总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485762%26idx%3D1%26sn%3De1e9fc75b92999177d3c61c655b0e06e%26chksm%3Dfdb694d5cac11dc37dac1a7ce32150836d66f0012f35a7e04e3dceaf626b8453dc39ee80172b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0730
[44][机器学习和深度学习中值得弄清楚的一些问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485811%26idx%3D1%26sn%3Da87082f0e47b01bb8c22443ba5b1139c%26chksm%3Dfdb694e4cac11df20ea1deb8b55cf297ad44e48a4c7ca45cfce387284143403fcd342ac98ec0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0802
[45][基于深度神经网络的自动问答系统概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485842%26idx%3D1%26sn%3Dd7485054d6e93225b6ac0c77f8706cf7%26chksm%3Dfdb69405cac11d1355b84f692c2cbe49a3852a10e074b6941c95618598caea6ed64103c4ee4c%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0806
[46][机器学习与深度学习核心知识点总结 写在校园招聘即将开始时](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486105%26idx%3D1%26sn%3Dd0b33e7e23b0e2fc442bd6b3e2a9d952%26chksm%3Dfdb6970ecac11e18085ea36f3b654028b2d4ba33a0cdc89c4ea25ac81570969f95f84c6939ac%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取 码】SIGAI0808
[47][理解Spatial Transformer Networks](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486133%26idx%3D1%26sn%3D31c64e83511ad89929609dbbb0286890%26chksm%3Dfdb69722cac11e34da58fc2c907e277b1c3153a483ce44e9aaf2c3ed468386d315a9b606be40%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0810
[48][AI时代大点兵-国内外知名AI公司2018年最新盘点](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486140%26idx%3D1%26sn%3D6157951b026199422becca8863f18a17%26chksm%3Dfdb6972bcac11e3d7427847df818bd6cc7893a261daa0babe0b11bd01a403dc4f36e2b45650e%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0813
[49][理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486202%26idx%3D1%26sn%3Dbf66be1e71272be42508e557ed93acbf%26chksm%3Dfdb6976dcac11e7b9d0f878233e8d9907825e57851e7a095f1be3a23abc9327370a77f4e2c03%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0815
[50][基于内容的图像检索技术综述--CNN方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486268%26idx%3D1%26sn%3Ddff53feb3d78ea7698f70bede08b3b19%26chksm%3Dfdb696abcac11fbdcde5f4acc72d34c14119a21234b9e6cd0c1886b85c330e7f77d6e3da9122%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0817
[51][文本表示简介](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486289%26idx%3D1%26sn%3Da312c951c943ad104c722e6c681823b6%26chksm%3Dfdb696c6cac11fd01224a68b9d67fcf0043ff2de0ec67f7bfd98bacf691c8eaf221cbca9b4d6%26token%3D1485183944%26lang%3Dzh_CN%23rd)【获取码】SIGAI0820
[52][机器学习中的最优化算法总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486354%26idx%3D2%26sn%3D1afd5f272536b6771b5e1e224e8414ec%26chksm%3Dfdb69605cac11f13f2a16c8748e333045e99497dc03ca9f4741723204e7d8348e969375379ae%26token%3D1580317399%26lang%3Dzh_CN%23rd)【获取码】SIGAI0822
[53][【AI就业面面观】如何选择适合自己的舞台？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486371%26idx%3D1%26sn%3D955286243fe23ff58d5a090c166d83f6%26chksm%3Dfdb69634cac11f2251793b40432b5429828f5e72411e2ae48c1ae282117f2d1b4067c14fc889%26token%3D1383761413%26lang%3Dzh_CN%23rd)【获取码】SIGAI0823
[54][浓缩就是精华-SIGAI机器学习蓝宝书](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486392%26idx%3D1%26sn%3D81cc65e42418bf846167ab80ae1dd4f4%26chksm%3Dfdb6962fcac11f39d3c62bc06e8a0708a24a024e53aef38e1bd716dcd1433fa89a0c0fe422df%26token%3D867271861%26lang%3Dzh_CN%23rd)【获取码】SIGAI0824
原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的

