# 人脸识别必读的N篇文章 - 人工智能学家 - CSDN博客
2018年01月17日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：308
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtItzibRQo8vKVfpABX8IAXiaf2KyjBJFj6LeQpSfl5NOSibc8mK5FmpRNd7ichWXVITZwtT395UXqmQ/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：机器视觉*
*概要：人脸检测/跟踪的目的是在图像/视频中找到各个人脸所在的位置和大小；对于跟踪而言，还需要确定帧间不同人脸间的对应关系。*
**一 、人脸检测/跟踪**
人脸检测/跟踪的目的是在图像/视频中找到各个人脸所在的位置和大小；对于跟踪而言，还需要确定帧间不同人脸间的对应关系。
**1. Robust Real-time Object Detection. ****Paul Viola, ****Michael Jones. IJCV 2004.**
入选理由：
Viola的人脸检测工作使得人脸检测真正变得实时可用。他们发表了一系列文章，这篇是引用率最高的一篇。
**2. Fast rotation invariant multi-view face detection based on real Adaboost. Bo Wu, Haizhou Ai, Chang Huang, Shihong Lao. AFGR 2004.**
人选理由:
自viola之后，有很多改进文章，这篇文章只是其中之一。之所以选择这篇，是因为其贡献点相对比较多：一是因为这篇文章第一次将real adaboost应用于物体检测，二是其提出了一个比较成熟实用的多姿态人脸检测框架，三是其提出的nest结构对cascade结构的改进确实有不错的效果。
**3. Tracking in Low Frame Rate Video: A Cascade Particle Filter with Discriminative Observers of Different Life Spans. Yuan Li, Haizhou Ai, ****Yamashita T., Shihong Lao. CVPR 2007.**
入选理由：
人脸跟踪是人脸识别中非常重要的模块。这篇文章是比较好的将人脸检测模型与跟踪进行结合，将离线模型和在线模型结合的工作，而且获得了CVPR 2007 Best Student Paper，是中国大陆学生第一次获此殊荣。
**二、人脸特征点定位**
人脸特征点定位的目的是在人脸检测/跟踪获取的人脸区域的基础上，进一步，确定脸部特征点（眼睛、嘴巴中心点、眼睛、嘴巴轮廓特征点、器官轮廓特征点等）的位置。人脸特征点定位的基本思路，主要是将人脸局部器官的纹理特征和器官特征点之间的位置约束进行结合来进行处理。
**4. Active Shape Models-Their Training and Application. T. F. COOTES, C. J. TAYLOR, D. H. COOPER, AND J. GRAHA. COMPUTER VISION AND IMAGE UNDERSTANDING. 1995.**
入选理由：
早期的人脸特征点定位，很多工作主要集中在定位眼球中心点和嘴巴中心点等两三个关键点上，但是，后来慢慢大家认为，引入更多的点，并加入相互约束会提高定位精度和稳定性。ASM是后来被大家所follow最多的将数十个脸部特征点的纹理和位置关系约束一起考虑来进行计算的模型，COOTES的这篇文章发表于1995年，绝对是开先河之作，绝对经典。
**5. Boosted Regression Active Shape Models. David Cristinacce and Tim Cootes. BMVC, 2007.**
入选理由：
ASM相关改进的文章非常多，最值得一提的当然是AAM（也是cootes最早提出）模型，除此之外还有一个非常重要的思路，就是改进原文章基于边缘的纹理模型。个人认为，这篇文章中所给出的基于回归方式表示纹理模型的方式比基于分类表观模型的方法更有前途，因为是cootes的文章，自然就更应该被推荐。
**6. Face Alignment by Explicit Shape Regression. Xudong Cao, Yichen Wei, Fang Wen, Jian Sun. CVPR 2012.**
入选理由：
这篇文章是上述提到的ASM相关改进的另外一个方向，就是对形状模型本身的改进。这篇文章没有用PCA去约束形状模型，而是基于训练样本的线性组合来约束形状。而且，其在alignment的效果(LFPW测试集)上是目前看到最好的，此外，这个方法的速度非常快。由于是MSRA孙剑组的文章，确实应该被关注更多。
**三、人脸表示**
人脸表示是指根据人脸特征点的位置，对人脸进行几何校正并割取人脸区域（归一化到固定大小）之后，得到最具有鉴别（区分）能力的特征的过程。
**7. Eigenfaces for recognition. M. Turk and A. Pentland. Journal of Cognitive Neuroscience. 1991.**
入选理由：
这需要理由吗？基于PCA的特征脸是人脸识别最经典的算法之一，虽然今天PCA在实际系统中更多的是用来降维，而不是用来分类，但是这么经典的方法还是要关注的。其实，基于LDA的方法也非常经典，不过，考虑到篇幅有限，就不再列举了。
**8. Local Gabor Binary Pattern Histogram Sequence (LGBPHS):**
**A Novel Non-Statistical Model for Face Representation and Recognition**
入选理由：
个人认为这可能是最接近于很多成熟商用系统思路的文章（不等同于实际系统就是这样）。在很多实际系统中，一个提取鉴别信息的框架就是PCA+LDA,用PDA进行降维避免LDA求解的矩阵奇异问题，然后用LDA提取更适合分类的特征；进一步，将各种原始特征(Gabor, LBP等)进行鉴别特征提取后进行决策级融合。此外，对人脸进行分块然后ensemble融合也是非常重要的提高系统效果的思路。
**9. Blessing of Dimensionality: High-dimensional Feature and Its Efficient Compression for Face Verification. Dong Chen. Xudong Cao. Fang Wen. Jian Sun. CVPR 2013.**
入选理由：
LFW是目前最接近实际数据的人脸识别库，虽然其测试协议有一些不尽合理的地方，但是如果能在LFW上面取得非常好的效果，说明方法还是比较好的。而这篇MSRA 孙剑组发表在CVPR2013的文章，在LFW上面取得了93%的准确率（未引入外部数据的情况下）。文章中的一个是采用精确定位点作为参考对人脸进行多尺度、多局部区域的表示思路很值得借鉴，可以和各种表示方法结合。
10，其实可以列在此处的论文非常多，有各种子空间方法的改进，有各种特征提取方法，有各种基于metric learning的方法，bunch graph（将gabor应用于人脸识别的经典工作），稀疏表示的方法，feature learning 的方法等等。但是，我都感觉不是特别满意。我总觉得，人脸表示方面的工作还做得远远不够，应该会有框架清晰简洁、易于实现、但效果绝佳的文章出现。现在的我会倾向于认为也许deep learning是一个方向。
其实尝试将deep learning用在人脸识别上面的工作已经有一些，但是结果都不是很惊艳（百度的工作没有公开结果），我列上一篇不够好但是效果勉强接近非deep learning方法的一篇：
Deep nonlinear metric learning with independent subspace analysis for face verification. X Cai, C Wang, B Xiao, X Chen, J Zhou. MM 2012.
**人脸识别在未来的一些可能方向：**
1，深度学习在人脸方面的应用，目前已经看到deep learning在人脸表示和人脸特征点定位方面的工作，相信后续会有更多更好的工作出现；
2，大规模人脸搜索相关的应用近来开始被大家关注（比如最近百度上线的人脸搜索），这些应用中除了需要传统的人脸表示，还需要关注如何能够快速准确地在大规模人脸数据库中搜索到相似人脸，当然这部分工作可以借鉴其他视觉搜索中的方法，但人脸可能也会有自己的特殊性；
3，基于3D模型和具有深度信息的人脸识别的方法，在允许使用特殊设备的实际应用中，可以考虑用3D模型和深度信息来提高系统的稳定性；
4，在做人脸识别实际系统时，可以更关注姿态、遮挡、表情变化对于识别效果的影响，对于人脸光照问题，虽然之前学术界关注很多，但是对于实际数据（非实验室采集的光照模拟数据），可能基于大规模训练数据和feature learning就可以比较好的解决，反而是由于目前的人脸表示框架，对于大的姿态变化，遮挡以及表情变化引起的表观改变，很多情况下表现并不好，可能需要重新改变目前的人脸表示方式，比如采用类似推荐论文9中的方式，采用多个局部模型而不是一个整体模型来进行表示，还可以考虑一些人脸姿态/表情矫正方法；
5，学术界的朋友，鉴于目前LFW上面已经做到准确率95%（引入外部训练数据且无约束训练协议），可以考虑创建一个更大的人脸库（如果能达到真正意义上的大规模数据就更赞了），设计一个更加合理全面的评测协议，这必将成为一个非常有影响力的工作；
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
