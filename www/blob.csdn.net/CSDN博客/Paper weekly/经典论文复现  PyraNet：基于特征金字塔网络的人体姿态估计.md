# 经典论文复现 | PyraNet：基于特征金字塔网络的人体姿态估计 - Paper weekly - CSDN博客





2018年10月17日 12:28:29[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：416









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




过去几年发表于各大 AI 顶会论文提出的 400 多种算法中，公开算法代码的仅占 6%，其中三分之一的论文作者分享了测试数据，约 54% 的分享包含“伪代码”。这是今年 AAAI 会议上一个严峻的报告。 人工智能这个蓬勃发展的领域正面临着实验重现的危机，就像实验重现问题过去十年来一直困扰着心理学、医学以及其他领域一样。**最根本的问题是研究人员通常不共享他们的源代码。**




可验证的知识是科学的基础，它事关理解。随着人工智能领域的发展，打破不可复现性将是必要的。为此，**PaperWeekly 联手百度 PaddlePaddle 共同发起了**[**本次论文有奖复现**](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490664&idx=1&sn=c716049d2a63a4ffef0d25e35f3168fb&chksm=96e9c3e8a19e4afe04f844dc25fee0adbae5345eefe177c611e18b37761d8d9a8aed1769a943&scene=21#wechat_redirect)，我们希望和来自学界、工业界的研究者一起接力，为 AI 行业带来良性循环。




作者丨黄泽宇

学校丨深圳大学

研究方向丨计算机图形学、深度学习




***Learning Feature Pyramids for Human Pose Estimation*** 是发表在 ICCV 2017 的一篇有关人体姿态估计的论文，提出利用特征金字塔来进行人体姿势预测。作者是 Wei Yang，香港中文大学博士生。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRMkCkk7e7apzTicCZ4YWWU62c0IEDAAKgj9vJhDNkMg7DU1w60mFcBFQ/640)




**论文复现代码： **




http://aistudio.baidu.com/aistudio/#/projectdetail/24019




# 人体姿态估计介绍




人体姿态估计是计算机视觉领域一个较有挑战性的任务，问题的**输入是一张图片，输出是图片中的人体各个关节点的位置**，如下图所示。人体姿态任务可以是单人姿态估计，或者是多人姿态估计，而本文给出的方法是单姿态估计，即一张图片只预测一个人的姿态。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnR92Y85tGakq6aPjEE0KpQFX52iaDXf4AI1VctJMf7ULswiakibdvQRUqNg/640)




在本文之前，在人体姿态估计效果较好的工作是 2016 年 Alejandro Newell 等人的 ***Stacked Hourglass Networks for Human Pose Estimation*** [1]，而本文的网络结构是在此之上的改进， 因此 Hourglass Network 的相关设计对理解本文网络非常重要。




# 重要工作介绍




**Stacked Hourglass Network**




Stacked Hourglass Network 是一种堆叠沙漏型的全卷积网络，能够很好地捕捉图片的多尺度特征，并由粗到细地预测关节点位置的热力图 Heatmap，即关节点出现在各个位置的概率。最终的关节点的位置预测结果取 Heatmap 中概率最大的索引。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRRDAibZoy4Hhj6ibdUQibtSXXkuPXRDKn08wPRvOFbQKDUQxgHYDicD1a2w/640)




网络的基本结构如下图所示，可以看到网络后面都是由一个个沙漏型的结构堆叠而成的。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRuy1sLnZlvxDtpOtto8mEFiavd1Gmv4E3vJC2yc0ByYnDXgGzicg1dteA/640)




**Hourglass Module **




沙漏模块则是一种编码器-解码器加短接层的设计，其动机是捕获多尺度信息。因为对于人体的各个不同部位的大小尺度是不一样的，通过短接层将不同尺度下的特征图加入到解码阶段可以获得更尺度的信息，从而得到更精准的预测。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRWohY21UedibbXezPLHuc5h43WN5Tl3fRRZeeWzpIXl2ribdMVg9xAWnQ/640)




Hourglass 模块和 Networks 中的白色方块表示的都是类似于 ResNet 中的残差模块 [2]，其作用是在保留原特征信息的同时进一步提取更深层次的特征，同时也能使得网络变得更深又不至于梯度消失。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnR62ujgOmcF5a9cibZpYuoxUXguLqcjyugSWY4bcy9BpMQjlerTvC4HsA/640)




像堆叠残差模块一样堆叠沙漏模块就得到了**堆叠沙漏网络**。值得注意的是，沙漏模块的输入和输出大小可以是一样的，也就是说在每个沙漏模块之后都可以进行最终结果的预测并计算损失，起到**中间监督作用**。




另外，上层模块的预测结果也可以作为下层模块的输入，从而更好的帮助下层模块进行预测，因此预测结果也可以通过 1*1 的卷积重新加入到原来的特征中，进行**由粗糙到细致的估计**。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRVWliclXDXQyKOjFR72aVQxvibaENSGZ5xBBUrVJzDK9Xb6ZViaibnVz5jg/640)




**改进方向 **




为了捕捉不同尺度的，除了使用短接层，还可以使用不同的卷积核同时进行卷积，再将得到的特征进行叠加，比如 **Inception 模块**[3]。Inception 模块通过使用不同大小的卷积核以及 1*1 的卷积使得网络能够捕捉不同分辨率的特征，并减少参数数量。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRDvA67ONDeaAF0iaRGfNtWL5w52RhVnMkyPeRpoZjhZib3H6shu23Zs9g/640)




而在减少参数数量方面，**ResNeXt** 又在 ResNet 更进一步 [4]，将初始的输入分裂成多条分支进行卷积，其中每条分支的卷积核大小都是一样的。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRowomHUeEU1c92vfd2IicXntjvnU8tm0jbObf3XiaGDV1jgvqw5nUQKyg/640)




另外，使用**空洞卷积**也可以获得多尺度的特征 [5]，空洞卷积是通过使用具有间隔的卷积核在特征图上进行卷积从而避免对原特征图进行下采样的步骤。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRibck3icZJGBrU6rpibpWWxUibN47e9JJ3vNYMUibpnVicoy9IHyFb2jwhEjQ/640)




# 本文方法




**Pyramid Residual Modules (PRMs)**




可能是受到上述三种模块的启示，本文作者设计出了四种特征金字塔模块如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRIRnLXibibFIqbWNyAYaRzzBAZqflp3QUicNvSkaC0AkCfbhqpR0lNQ0Og/640)




**PRM-A 是在原先的残差模块的分支基础上，直接增加多个分辨率的分支**，其分辨率的不同主要是通过下采样实现的，而由于残差模块的结果需要将不同分支的结果相加，因此下采样后的特征要通过上采样恢复原来分辨率。




**PRM-B 则是将 PRM-A 中不同分辨率的分支开始的 1*1 卷积进行参数共享**，从而减少参数数量。




**PRM-C 则是将 PRM-B 中多分辨率特征的相加改为了串联**，由于串联后的特征通道数与原来不同，因此可能需要再进行一个 1*1 的卷积对齐特征通道后再与原特征相加。




**PRM-D 则是使用空洞卷积代**，替下采样和上采样得到多尺度的特征。




根据后面的实验结果可以看到在准确率、参数数量和复杂度的权衡之下，PRM-B 模块是较好的选择。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRiaYTPvm5DRTKas08zgAaCfLiaibLsPnpPJXicytJPzfuekVUczRvAleibmA/640)




**Output Variance Accumulation **




除了上述的改进，本文作者还提到原始的残差模块有输出方差积累的问题，当堆叠多个残差块时，将原始特征直接与卷积后的特征相加时会有较大的方差，通过对原始特征添加一个 Bn-ReLu-Conv 操作可以较好的控制这个问题。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRwyJKia6rqvZKfcFGMltWKgnBD1b1C5P6F5xZ60S6qJRfIkKLylf9DbA/640)




**PyraNet **




本文网络框架使用 Stacked Hourglass Network 的基本框架，但将其中的残差模块都替换成了上述特征金字塔模块 PRMs，网络结构图如下。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRicuco7GpBLKEFfHSAwjb549zWFBaibFKqVoz94icRo4LQ7ABPibr6lBickQ/640)




**再谈初始化 **




网络参数的初始化对网络的训练以及结果会有一定影响，为了使得网络更顺利的开始训练，有许多不同的初始化方案，其中较常见的即是 ***Xavier***[6]。




**Xavier **




Xavier 的提出者指出，第 i 层的参数方差从正向和反向传播的角度考虑，应分别满足如下式子。其中的 n_i 和 n_(i+1) 分别是该层输入的元素个数以及输出的元素个数。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRwjHuAlj1dKpsnPQDqetqIXyiaKibZU4H8TCPxfnItz0e0jGfGCljtslA/640)




折衷考虑，Xavier 初始化的参数方差同时考虑输入和输出元素个数，即将上述两式相加后得到的结果。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRnhoIIIwVGx7kvLCm4E77z45nye6dacpT9RGUZkHaD2HITuwQhicIvmA/640)




**Initialization Multi-Branch Networks**




本文作者考虑到提出 Xavier 时的大部分网络并没有多条分支，因此对多分支网络的初始化方案重新考量，得出了如下的更泛化的结果。其中 l 表示网络层数，C_i 和 C_o 分别表示输入和输出分支数，n_i 和 n_o 分别表示各输入和输出分支的元素个数，α 根据激活函数有不同取值，ReLu 取 0.5。即从前向和反向传播的角度考虑，各层参数初始化时的方差应与各输入分支合并前的总元素个数、各输出分支分离后的总元素个数有关。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRjmib4Bqo3muVl3FAs0qTqayHa56f9HTpO7l0XtrRoMdJ0qy6oZlapdw/640)




折衷考虑，多分支参数初始化的方差在文中应为满足如下式子。**注意到 α 带有平方，特殊情况下，若 α 取 0.5，输入输出均只有 1 条分支，结果与 Xavier 不一致，因此笔者认为 α 不应取平方，这样在上述情况下仍能 Xavier 保持一致，作为 Xavier 的泛化。**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRo9dX7pv437LEvB3BFJPPbgjKFibm9hqMtKp4wkXmzcbLFfLlzGwV5Ow/640)




# 实验结果与分析




**人体姿态估计准确性 **




本文在 MPII 和 LSP、LSPEt 数据集上进行训练，使用 PCK (Percentage of Correct Keypoints) 和 PCKh 进行评估，PCK 计算估计的关键点与真实值间的归一化距离小于设定阈值的比例，PCKh 则以头部长度为参考的归一化。实验结果如下，可以看到使用 PRM-B 的 PyraNet 在所有对比的方法中都取得了最好的准确率。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRlJuCBknSkFs0BjU4uCK4ATrI0v4nUnm6ONX3eXpw237sUroEXpKC4w/640)




**控制变量比较**




在网络结构的对比实验中可以看到，相比于 Baseline 即普通的 Stacked Hourglass Network，PyraNet 使用的特征金字塔和多分支参数初始化方案都有提高结果的准确性。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRKfE555mjjUkuVCxEqP97rPeGSUXTdH8ge9fd2gVicmb2OjHEKfGMs7w/640)




** 其他实验 **




本文作者使 PRM 替代相应网络的残差模块在 CIFAR-10 上进行训练，得到最低的 Top-1 测试误差，但网络的大小和运算量稍有增加。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRXqEtJjlOvra0oVnFJFF5FLFeEn42aRrEic9YnEgqdT4iaHTBwvMdDicOA/640)




# 总结




本文的主题虽然是人体姿态估计，但提出的改进和创新较为普适，在其他任务上也可以进尝试。主要思想有如下： 




1. 本提出了特征金字塔残差模块，增强了深度神经网络的尺度不变性；




2. 本文提出了多分支参数初始化方案，使得网络训练更顺利；




3. 本文提出了通过在短接层增加一次卷积来减少残差模块输出方差积累的问题。




# PaddleFluid模型复现




注：代码中的 bn_relu_convn*n 是对依次使用 PaddleFluid 的 batch norm、relu 和卷积核为 n 的 conv2d 的封装。 




**1. 定义特征金字塔模块：**特征金字塔首先将输入进行不够规模的下采样，再进行特征提取，然后将下采样后提取的特征上采样回输入大小。原文使用了 Fraction pool 进行下采样，使得下采样更加平滑，而 PaddleFluid 并没有实现 Fraction pool，故只能使用简单的二线性插值 Resize bilinear 进行代替。





```
def pyramid(input, out_ch, ngroup):
    output_res = input.shape[-1]
    scale_base = pow(2.0, 1.0/ngroup)

    # extract features in different resolution
    features = []
    for i in range(1, ngroup+1):
        # subsample
        scale = round(1.0/pow(scale_base, i),4)
        feature = resize_bilinear(input, scale=scale)

        # extract the feature
        feature = bn_relu_conv3x3(feature,out_ch)

        # upsample
        feature = resize_bilinear(feature, out_shape=[output_res, output_res])

        features.append(feature)

    # sum up features
    output = features[0]
    for i in range(1, ngroup):
        output = elementwise_add(output, features[i])   
    return output
```




** 2. 定义 PRM 特征金字塔残差块：**在一般的 ResNet 残差模块的基础上进行扩展，在进行完 3*3 的卷积后再加入一个特征金字塔，这里实现了上述的 PRM-B，即所有特征金字塔分支使同一个输入。




```
def conv_block(input, out_ch, type='prm-b', base_width=6, cardinality=30):
    # 1*1 conv
    conv_out = bn_relu_conv1x1(input,out_ch//2)
    # 3*3 conv
    conv_out = bn_relu_conv3x3(conv_out,out_ch//2)

    if type == 'res':
        output = bn_relu_conv1x1(conv_out,out_ch) 
        return output
    elif type == 'prm-b':
        pyra_depth = out_ch//base_width
        ngroup = cardinality

        # extract feature pyramid
        # 1 branch in, ngroup branches out
        pyra_out = bn_relu_conv1x1(input, pyra_depth,in_branches=1,out_branches=ngroup)

        pyra_out = pyramid(pyra_out, pyra_depth, ngroup)

        # ngroup braches in, 1 branch out
        pyra_out = bn_relu_conv1x1(pyra_out, out_ch//2,in_branches=ngroup,out_branches=1)

        # 2 branches in, 1 branch out 
        output = elementwise_add(conv_out, pyra_out)
        output = bn_relu_conv1x1(output, out_ch,in_branches=2,out_branches=1)
        return output        
    # TODO: PRM-A/PRM-C/PRM-D
    return output
```




**3. 按照 Stacked Hourglass Network 的方式定义沙漏模块和堆叠沙漏模块**（代码略）。值得注意的是 Hourglass 是一个递归的结构，因此可以使用递归函数来建立网络结构。




**4. 定义初始化方案 Branch initializer**，笔者使用 Xavier 进行泛化，加入了输入输出分支数。




```
def branch_initializer(in_units=1,out_units=1,in_branches=1,out_branches=1,act='relu',uniform=False,seed=0):
    # it might be alpha instead of alpha**2 in formula.15
    # in this case, when in_branches=out_branches=1, it degenrates to Xavier
    alphax2 = 0.5*2 if act=='relu' else 1.0*2
    fan_in = in_units*in_branches*alphax2
    fan_out = out_units*out_branches*alphax2
    return Xavier(uniform,fan_in,fan_out)
```




**5. 网络训练：**实验使用 PaddleFluid v0.14 环境，Titan Xp 单 GPU，在 MPII 数据集上进行训练，训练图片 20k 张，测试图片 2k 张，训练时进行了数据增强。PyraNet 堆叠沙漏数 nstack=2，残差模块使用 PRM-B 结构，特征金字塔分支数 cardinality=4，通道基数 base_width=9，批大小 batch_size=8，训练轮述 epoch=150，使用 Adam 优化器，学习率 2.5*10^-4 且每 10 轮衰减至 90%，初始化使用 Xavier 泛化后的多分支初始化。对照组除了残差模块为普通 res 模块其他参数均相同。 




**6. 实验结果：**本次复现结果使用 PRM-B 模块的 PyraNet 并没有比使用 Res 模块的堆叠沙漏网络效果要好。准确率使用 PCKh@0.5 进行评估。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnROHyXjJgRYK1qfWS3BEQ35vbo7hphlSdgWicib39GQkrw3jbwY8MbeXmQ/640)




准确率曲线如下：使用 PRM-B 由于发生了过拟合，在 Epoch=110 处进行了早停，而使用 Res 则在 Epoch=110 时接近收敛。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRhDJmojY6up1gXQ6o8NKA0KqHomaLVkzNjSx3TCIuL9RlDBo2FDFgibw/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRO1rOVfPhVSQhiaAZUaX9Vc2SHuiaRERSicfJPETRt7bibucqKpIGh0RGrA/640)




**7. 结果对比分析：**复现结果本应是使用 PRM-B 模块的 PyraNet 要好于使用 Res 模块的堆叠沙漏网络，但结果却相反。 




在训练时发现一个问题，即 PyraNet 很容易过拟合，调整多次都没有得到很好的结果，而普通的堆叠沙漏网络训练则非常稳定。推测是由于加入了特征金字塔结构，分支数太多，网络变得复杂，所以难以训练。由于时间关系，也没有将网络调试到最好状态。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRyMkX1acxeZ0TNfzWVqcxBvdCiajEeGSxgwiaBmbGtKKmtWUEfsmKerRg/640)




**8. 结果可视化：**下面是几个使用 Res 模块的堆叠沙漏网络实验结果的可视化，可以看到预测出了基本的人体姿势。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRCIIJ02DOzeRGs4fpd5amP3uXNdFQWNcZq2QoOwicIKXu4mv6SvXrzxg/640)




# 关于PaddlePaddle




这是笔者第一次接触并复现有关人体姿态预测的论文，也是首次尝试使用 PaddlePaddle，并没有取得很好的结果。




人体姿态检测相比于简单的图片识别、生成，数据处理过程更复杂，计算量更大，网络结构一旦变得复杂，就会变得难以训练。同时网络变得更大参数数量急剧增加，只能用很小的批大小进行训练，也容易使得网络陷入局部最小值。




而 PaddlePaddle 则是极具潜力的深度学习框架，很容易上手，目前还只是实现了最基本、最常见的一些操作，对于实现纯卷积网络来说非常便利，期待未来版本有更强大的功能和更好的使用体验。



# 参考文献




[1]. Newell A, Yang K, Deng J. Stacked Hourglass Networks for Human Pose Estimation[J]. 2016:483-499. 

[2]. He K, Zhang X, Ren S, et al. Deep Residual Learning for Image Recognition[C]// IEEE Conference on Computer Vision and Pattern Recognition. IEEE Computer Society, 2016:770-778. 

[3]. Szegedy C, Liu W, Jia Y, et al. Going deeper with convolutions[J]. 2014:1-9. 

[4]. Xie S, Girshick R, Dollar P, et al. Aggregated Residual Transformations for Deep Neural Networks[J]. 2016:5987-5995. 

[5]. Chen L C, Papandreou G, Kokkinos I, et al. DeepLab: Semantic Image Segmentation with Deep Convolutional Nets, Atrous Convolution, and Fully Connected CRFs.[J]. IEEE Transactions on Pattern Analysis & Machine Intelligence, 2018, 40(4):834-848. 

[6]. Glorot X, Bengio Y. Understanding the difficulty of training deep feedforward neural networks[J]. Journal of Machine Learning Research, 2010, 9:249-256.




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击标题查看更多论文解读：**




- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[ECCV 2018 | 从单帧RGB图像生成三维网格模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491465&idx=1&sn=a6871b72d460debf90d2daa0bed719c8&chksm=96e9c009a19e491f8c247f36a53bad0a6812c462f3fb5b2d9ae74b38be673b946b82a4b44330&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[ECCV 2018 | 腾讯AI Lab提出视频再定位任务](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491347&idx=1&sn=e1f2cc16c9fcfcc5d2935118f09ee094&chksm=96e9c093a19e49855931cf621ec7f715c1d2dd5041e3343bc311aea2e5069ae03aaa1367e8f9&scene=21#wechat_redirect)

- 
[KDD 18 | 斯坦福大学提出全新网络嵌入方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491364&idx=1&sn=afa577a1bebfd148ac20cde05a927e24&chksm=96e9c0a4a19e49b2458f451c6e201277c509905201a1f751b30fbfa1e2ba31bc2711b2ca1b7a&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向） 

• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接 

• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志



**📬 投稿邮箱：**

• 投稿邮箱：hr@paperweekly.site

• 所有文章配图，请单独在附件中发送 

• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通










🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 收藏复现代码




