# 目标跟踪的深度学习方法与opencv下的KCF方法 - 站在巨人的肩膀上coding - CSDN博客





2018年07月11日 20:43:47[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：4133








# 目标跟踪的深度学习方法 与 opencv实现kcf方法


![](https://s3.51cto.com//wyfs02/M01/6F/6D/wKiom1WbpB7wn0w8AAA3s0TPpXg724_middle.jpg)
[necther](http://blog.51cto.com/yixianwei)6人评论7357人阅读2018-04-09 14:53:55





目录
- 概述 3
1.1. 背景 3
1.2. 目标跟踪问题描述 3
1.3. 目标跟踪测试数据集及性能评价标准 4
- 算法选型 10
2.1. 现有算法分类 10
2.2. 图像目标跟踪方法 11
1.概述
1.1.背景
行为识别的前提是需要对人体目标进行识别和跟踪，现阶段我们已经通过使用开源人体姿态估计系统openpose实现了人体姿态识别，现在需要对人体目标进行跟踪判断视频后续画面中的人和前面画面中的对应关系。本文就现阶段计算机视觉目标跟踪方面的研究成果做总结描述。
1.2.目标跟踪问题描述
简单来说，目标跟踪就是在连续的视频序列中，建立所要跟踪物体的位置关系，得到物体完整的运动轨迹。给定图像第一帧的目标坐标位置，计算在下一帧图像中目标的确切位置。跟踪过程中的 光照变化、目标尺度变化、目标被遮挡、目标的形变、运动模糊、目标的快速运动、目标的旋转、目标逃离视差、背景杂波、低分辨率 等现象，都是目标跟踪问题的挑战。
根据问题，目标跟踪算法主要有以下几个关键点：
![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/06d1cfe727c8930807b8727e8df3e515.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
1、选取合适的特征：HOG特征，CNN（卷积神经网络）特征
2、优秀的分类器：SVM等
3、合适的更新模型时间的策略：因为判别模型更新的太频繁会影响跟踪算法的速度，更新速度太慢又会导致目标丢失。
1.3.目标跟踪测试数据集及性能评价标准
现在比较流行的跟踪Benckmark（基准，数据集）有OTB、VOT、KITTI-tracking，本文主要介绍VOT的评价方法：
中心误差：Center Error
中心误差标准值考虑目标中心位置的误差，该方法具有明显的缺点，就是无法衡量目标尺度变化的影响。另外，目标中心的意义有时候不是很明确（如下图）。![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/51999f859586b41e1771bd6854b98bd1.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
区域重叠：Region Overlap
区域的重叠率是使用重叠面积的比例进行衡量的![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/3ce5142904167f5217d0f84033b3a03e.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
在区域重叠指标上，我们一般使用有效帧的重叠率做平均。除此之外，我们还可以使用重叠率阈值以上帧的比例作为标准：![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/954a17db0b6faef3b98def20cbb86cba.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
这一指标是基于中心位置误差的，从跟踪开始到中心像素误差下降到阈值以下的（帧数）长度定义为跟踪长度。
失败率：Failure Rate
![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/33c033f827ca1a0f2380e83e6bc987a6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
在视频跟踪处理的一段序列中，当重叠率低于阈值时，则判定为跟踪失败，并重新初始化目标框，在这段视频中，跟踪失败的次数以及跟踪失败的位置可以被记录下来，则失败率可以被表示为：![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/e83feaf4267286ee3c67afcade722794.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
效果图：Performance Plot
常用的效果图有area-under-the-curve (AUC)曲线，该曲线和ROC曲线表达的含义差不多，也就是OTB中成功率图的表示方法。
评价指标的效果评估
这一部分主要是介绍评估哪些标准可以更好的用来评估跟踪算法，我们才会推荐使用这些指标来衡量跟踪算法的性能。下面图中给出了几种指标可视化示意图，其中能反映出许多问题，也就是说标准选取的重要性。![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/08f71d0e19e30707261c824a7d3e318b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
评价标准的相关性分析
使用上述16个标准，对样本集中的所有指标的数值，计算协方差矩阵，协方差矩阵就代表了评价标准的相关性。下图是协方差矩阵的可视化：![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/1051c6fff79a41063243075fc3eed9dc.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/dfcbac075d86f1c0d6dcb16be26ba047.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
于是这就是我们最终在VOT相关的Paper上看到的图了。
EFO
Equivalent Filter Operations，EFO是用来评价性能的，因为性能的比较常常因为计算机性能不同而不够直观，EFO评价标准考虑到了这一点，使用600×600的图像，做30*30窗口的滤波，来得到机器的性能。然后使用跟踪算法处理每帧图像的评价时间除以以上滤波操作的时间，得到了一个归一化的性能参数，就是EFO，是VOT14提出的标准，一直沿用到现在。
EAO与EFO就构成了VOT16的跟踪算法评价标准。
2.算法选型
2.1.现有算法分类
在早期，Camshift、光流、背景差等图像跟踪算法比较流行，在静态背景条件下成功应用，大致在2008年以后，这类方法逐渐被淘汰，人们更多的研究动态背景、复杂场景环境下的图像跟踪，为了区别于原先几乎被淘汰的方法，博主将这一类算法归为现代图像跟踪算法，并做如下分类
（1）判别式：track-by-detection
判别式的图像跟踪算法将目标检测的思路用于目标跟踪。在线产生样本，在线学习，在线检测，找到目标出现概率最大的位置，然后重复这样的步骤，跟踪目标位置，这也是当下最流行的方法。
（2）产生式
 产生式就是在跟踪过程中，根据跟踪结果在参数空间产生样本（而非直接从图像空间采样），然后在这些样本中寻找最优可能的目标作为跟踪结果。粒子滤波框架是最常用的产生式目标检测模型。
（3）深度学习用于图像跟踪
深度学习的方法主要分为两类，一类是使用预先训练好的深度神经网络，CNN或RNN部分不做在线训练，使用另外的算法输出目标位置，另一类是在线训练的神经网络，由神经网络输出目标位置。
目前图像跟踪处于百家争鸣的状态，深度学习方法在目标跟踪方面并没有预期的凶猛，主要是体现在速度方面。在检测效果方面深度学习方法微微占优，但是速度上深度学习方法完全无法和传统方法相比较。
现阶段算法谱系图如下：
![目标跟踪的深度学习方法 与 opencv实现kcf方法](http://i2.51cto.com/images/blog/201804/09/bc0e6e9e70a2724a5bb1c247b6c163d7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.2.图像目标跟踪方法
- MIL
MIL，多实例在线学习，是一种典型的track-by-detection的方法。在MIL之前有OAB，OAB算法采用Online Adaboost算法进行在线学习，而MIL采用Online MILBoost进行在线学习，速度上更快， 并且可以抵抗遮挡。直观的感受下：

[1].  Babenko, B., M. Yang and S. Belongie. Visual tracking with online multiple instance learning. in Computer Vision and Pattern Recognition, 2009. CVPR 2009. IEEE Conference on. 2009: IEEE.
[2].  Babenko, B., M. Yang and S. Belongie, Robust object tracking with online multiple instance learning. IEEE Transactions on Pattern Analysis and Machine Intelligence, 2011. 33(8): p. 1619-1632.
- Struck
 一般的track-by-detection在做完检测之后，会把周围的图像小块分为两类，给上正负样本标签，而Struck不这样做，Struck通过在线SVM直接输出Score。

        SVM的输出空间就是Score空间，每个patch块提取完特征，直接经过核函数乘以W，就可以得到Score空间下的值了。Online-SVM通过SMO进行训练，速度较快。一下是pipline，可以感受一下：

[1].  Hare, S., A. Saffari and P.H. Torr. Struck: Structured output tracking with kernels. in 2011 International Conference on Computer Vision. 2011: IEEE.
[2].  Hare, S., et al., Struck: Structured output tracking with kernels. IEEE Transactions on Pattern Analysis and Machine Intelligence, 2015. 38(10): p. 2096-2109.
- MEEM
        图像跟踪常常受模型漂移的影响，可能某次检测失败就会导致后续的图像帧跟踪出现误差，并且误差会不断积累。于是这篇paper提出采用一种多专家模型，维护一个固定数量的专家集合，每当来新样本时，各个专家费别给出评价，然后挑选最优的专家给出的结果。这里的专家直接来自过去某个时间段内的分类器状态。MEEM也是采用的Online-SVM。

[1].  Zhang, J., S. Ma and S. Sclaroff. MEEM: robust tracking via multiple experts using entropy minimization. in European Conference on Computer Vision. 2014: Springer.
- 
KCF
KCF是很重要的算法。 相关滤波被用于图像跟踪领域始于2010年的MOSSE算法，而KCF算法将相关滤波的方法用到极致，其核心思想是利用循环矩阵乘以图片，使图像产生位移，从而得到大量样本。把位移的样本存在一个矩阵中会组成一个循环矩阵。

DFT中，循环矩阵具有一系列美好的性质，根据一系列推导，将样本空间变换到DFT空间内，可以实现大量样本的快速训练和学习。循环卷积在DFT变换后变成了矩阵元素的点积，这样就将算法的时间复杂度从O(n^3)减少到O(n*log(n))，KCF算法不仅速度快、效果好，被后来许多图像跟踪算法采用，产生了许多变种。
更细节的说，KCF通过平移产生大量样本，并且给每个样本赋予一个标签，这个标签根据离中心的距离，使用高斯分布描述，可以理解为置信度。另外，样本在做平移之前需要通过cos窗口加权，这样做的目的是避免在平移过程中，边缘太过强烈，引发不必要的麻烦。


        关于DFT（离散傅里叶变换）和循环矩阵部分的推导是这个算法的核心，在Paper后面也有推导，这篇paper一定要多看几遍，后面很多方法都和这个有关。
[1].  Henriques, J.F., et al. Exploiting the circulant structure of tracking-by-detection with kernels. in European conference on computer vision. 2012: Springer.
[2].  Henriques, J.F., et al., High-speed tracking with kernelized correlation filters. IEEE Transactions on Pattern Analysis and Machine Intelligence, 2015. 37(3): p. 583-596.
- SRDCF/deepSRDCF
该算法是对KCF算法的改进，因为KCF算法在截取目标区域时选取了2.5倍的padding，这就导致非目标部分也被纳入到相关滤波器的预测中了，边界部分带来的影响降低了模型的质量。

        改算法通过空间正则化来提高跟踪模型的质量。通过一个高斯分布的空间惩罚因子，对不同位置加入不同权重的惩罚，这就是空间正则化。对空间正则化和输出进行可视化，在边界处的输出被明显抑制了。SRDCF将空间正则化的表达融合进loss里面，然后直接最小二乘回归就可以了，很方便。

 第二篇paper，deepSRDCF就是把特征提取部分换成CNN来做，其余部分和SRDCF一样。这篇paper着重分析了使用各层特征带来的影响，发现使用第一层的特征就可以了，后面的层并不重要。这点和有些文献中的结论不一致，这条结论博主也持保留态度。
[1]. Danelljan, M., et al. Learning spatially regularized correlation filters for visual tracking. in Proceedings of the IEEE International Conference on Computer Vision. 2015.
[2]. Danelljan, M., et al. Convolutional features for correlation filter based visual tracking. in Proceedings of the IEEE International Conference on Computer Vision Workshops. 2015.
- SRDCFdecon
该方法是对SRDCF算法的改进，同样是为了解决样本污染问题。以往的模型学习过程在样本被遮挡时也一直在学习，这样就会导致模型被污染。之前提出过多专家的方案和boost泛化的方案，但是样本污染问题一直无法得到有效的解决。这篇paper提出的方法应该是目前解决的最好的，并且可以推广到很多方法上面，具有很高的价值。

[1]. Danelljan, M., et al. Adaptive decontamination of the training set: A unified formulation for discriminative visual tracking. 2016: CVPR.
- Staple
 Staple考虑了两种方法的结合：Hog特征+颜色直方图。因为相关滤波用Hog特征时对运动模糊和照度很鲁棒，但是对形变不够鲁棒。而颜色直方图对形变非常鲁棒。一个目标有了形变后，整个目标的颜色分布是基本不会变得。另一方面，颜色直方图对光照变化不鲁棒，这一点又可以由Hog特征进行补充。Staple同时考虑这两种特征，分成两个通道来做，形成优势互补。

[1]. Bertinetto, L., et al. Staple: Complementary learners for real-time tracking. in Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition. 2016.
- EBT
EBT是一种proposal方法，可与Struck等算法联合使用。在proposal阶段使用边缘特征（文中使用的是一个10维特征向量）进行全局检测，以此来应对物体运动过快的情况。

[1]. Zhu, G., F. Porikli and H. Li. Beyond local search: Tracking objects everywhere with instance-specific proposals. in Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition. 2016.

三、深度学习用于目标跟踪
- DLT
        Deep Learning Tracker, DLT提出于2013年，是第一个深度学习目标跟踪算法，大体上还是粒子滤波的框架，只是采用SDAE(栈式抑噪自编码器)提取特征。下乳就是SDAE的结构，SDAE是预先训练好的，不需要在线学习，然后后面跟着粒子滤波的一套东西。

[1]. Wang, N. and D. Yeung. Learning a deep compact image representation for visual tracking. in Advances in neural information processing systems. 2013.
- HDCF
        这篇Paper和后面的HDT算法有着很大关系，如果不是速度原因，HDCF算法将会是我相当推崇的，其思想简单，就是通过预先训练好的深度神经网络来提取特征，利用多个层的特征来共同定位，浅层特征位置准确，深层特征包含语义信息，在线学习部分当然是使用当下最受欢迎，又快又好又简洁的Correlation Filter了。这种思想其实是相当不错的，只是用了深度学习就不得不面对庞大的参数和GPU加速处理。

        博主测试了原文中的算法，在博主的GTX1070上跑到23fps，如果只用CPU的话6600k超频到4.24GHz也只有6fps，远未达到实时要求。另外，博主修改了下代码，分别只使用Conv3、Conv4、Conv5层，发现准确率下降的并不多。博主在想是否一定要这么多层的特征呢？这个问题目前是存在争论的，不过介绍这篇Paper是为了介绍接下来一篇Paper的。

[1]. Ma, C., et al. Hierarchical convolutional features for visual tracking. in Proceedings of the IEEE International Conference on Computer Vision. 2015.
- HDT
        Hedged Deep Tracking的缩写，上面一篇Paper提到，既然深层特征包含语义信息，浅层特征包含位置信息，那么如何将这些特征有效的结合起来呢？Hedge就是本文利用的方法，每一层特征训练出来的Correlation Filter作为一个弱分类器，通过Hedge的方法，平衡各个结果，给出预测位置（其实就是给每个层的位置结果一个权重，然后加权平均，主要问题是权重怎么算，Paper就是讲这个了）。

[1]. Qi, Y., et al. Hedged Deep Tracking. in Proceedings of IEEE Conference on Computer Vision and Pattern Recognition. 2016.
- STCT
因为跟踪算法需要在线学习，深度学习采用预训练的方式训练一个网络CNN-E提取特征，然后分层的特征再送到另一个网络CNN-A里面训练，每一层训练得到一个弱分类器，然后使用类似boost方法在顶端建立一个SPN网络预测结果，预测loss小的话就作为正样本来更新网络实现在线跟踪。

[1]. Wang, L., et al. Stct: Sequentially training convolutional networks for visual tracking. in Computer Vision and Pattern Recognition (CVPR), 2016 IEEE Conference on. 2016.
- C-COT
C-COT和TCNN是VOT-16中表现最好的算法。C-COT、后面的ECO都出自Danelljan，也就是前面SRDCF系列、DSST系列的作者。从越来越复杂的数学表达可以看出Danelljan对DCF的研究已经非常深入了。
        由于传统DCF算法在目标尺度和分辨率方面适应性太差，而考虑到CNN深层特征的语义表达能力，想法就是将多尺度和深层语义信息结合起来。根据不同空间分辨率的响应，在频域进行插值，得到连续空间分辨率的响应图，通过高斯赛德尔方法迭代求得最佳位置和尺度。这样成功的对抗了DCF在目标尺度变化时引起的模型漂移。在实践中使用了原始彩×××像三个通道和CNN两个特征层，五种分辨率(0.98,0.99,1.0,1.1,1.2)，分别学习这些层的卷积滤波器（下图第二列），然后将它们综合起来，得到最后的位置和尺度，因为是插值方式得到的，所以空间和尺度都是连续的。至于更多细节，就是其采用了样本净化技术（之前提到过的）、SRDCF等。由于C-COT算法在CPU上速度大概1fps，所以后面的ECO算法主要是进行速度的优化。

[1]. Danelljan, M., et al. Beyond correlation filters: Learning continuous convolution operators for visual tracking. in European Conference on Computer Vision. 2016: Springer.
- ECO
 ECO是在C-COT基础上的改进，要知道一个实用的算法不光要有好的性能，还要有好的实时性。ECO算法在效果比C-COT好的情况下，在CPU上可以到达60fps的高速，可以说是目前最牛的算法了。
 ECO算法的主要思想有三点，一是通过提出的卷积因式分解操作来降低C-COT提取出特征的维度，二是通过样本分组来归类训练集，一方面避免相同样本重复训练引起的过拟合，另一方面提高速度，三是在训练过程中模型每隔N帧更新一次，而不是每帧更新，这主要是考虑到相邻样本有着相同的表现，而更新过程太过耗时。

这篇论文已经被CVPR17接受了，下面的参考文献没有指向CVPR17
[1].Danelljan, M., et al., ECO: Efficient Convolution Operators for Tracking. arXiv preprint arXiv:1611.09224, 2016.
综上所述，因为已经识别了人体目标只需要判定在后续画面中的人对应的是前面画面当中的哪一个人即可，所以采用CNN结合HOG特征的方法来判定目标归属。

附上 opencv 实现 kcf目标跟踪算法 代码：

```
import cv2
import sys

(major_ver, minor_ver, subminor_ver) = (cv2.__version__).split('.')

if __name__ == '__main__':

    # Set up tracker.
    # Instead of MIL, you can also use

    tracker_types = ['BOOSTING', 'MIL', 'KCF', 'TLD', 'MEDIANFLOW', 'GOTURN']
    tracker_type = tracker_types[2]

    if int(minor_ver) < 3:
        tracker = cv2.Tracker_create(tracker_type)
    else:
        if tracker_type == 'BOOSTING':
            tracker = cv2.TrackerBoosting_create()
        if tracker_type == 'MIL':
            tracker = cv2.TrackerMIL_create()
        if tracker_type == 'KCF':
            tracker = cv2.TrackerKCF_create()
        if tracker_type == 'TLD':
            tracker = cv2.TrackerTLD_create()
        if tracker_type == 'MEDIANFLOW':
            tracker = cv2.TrackerMedianFlow_create()
        if tracker_type == 'GOTURN':
            tracker = cv2.TrackerGOTURN_create()

    # Read video
    video = cv2.VideoCapture("E:/sample/qiusai.mp4")

    # Exit if video not opened.
    if not video.isOpened():
        print
        "Could not open video"
        sys.exit()

    # Read first frame.
    ok, frame = video.read()
    if not ok:
        print
        'Cannot read video file'
        sys.exit()

    # Define an initial bounding box
    #bbox = (52, 28, 184, 189)#这里四个参数分别为 起始坐标xy 和 宽 高

    # Uncomment the line below to select a different bounding box
    bbox = cv2.selectROI(frame, False)

    # Initialize tracker with first frame and bounding box
    ok = tracker.init(frame, bbox)
    num = 0
    while True:
        num = num + 1
        if num % 500 == 0:
            tracker = cv2.TrackerKCF_create()
            bbox = cv2.selectROI(frame, False)
            # Initialize tracker with first frame and bounding box
            ok = tracker.init(frame, bbox)
        # Read a new frame
        ok, frame = video.read()
        if not ok:
            break

        # Start timer
        timer = cv2.getTickCount()

        # Update tracker
        ok, bbox = tracker.update(frame)

        # Calculate Frames per second (FPS)
        fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer);

        # Draw bounding box
        if ok:
            # Tracking success
            p1 = (int(bbox[0]), int(bbox[1]))
            p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
            cv2.rectangle(frame, p1, p2, (255, 0, 0), 2, 1)
        else:
            # Tracking failure
            cv2.putText(frame, "Tracking failure detected", (100, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 2)

        # Display tracker type on frame
        cv2.putText(frame, tracker_type + " Tracker", (100, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50, 170, 50), 2);

        # Display FPS on frame
        cv2.putText(frame, "FPS : " + str(int(fps)), (100, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50, 170, 50), 2);

        # Display result
        cv2.imshow("Tracking", frame)

        # Exit if ESC pressed
        k = cv2.waitKey(1) & 0xff
        if k == 27: break
```





