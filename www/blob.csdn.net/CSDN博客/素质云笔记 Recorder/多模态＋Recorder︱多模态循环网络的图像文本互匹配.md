# 多模态＋Recorder︱多模态循环网络的图像文本互匹配 - 素质云笔记/Recorder... - CSDN博客





2017年05月01日 12:17:45[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5523








> 
来源于公众号：大数据与多模态计算


.

## 1.问题背景

图像文本匹配，顾名思义，就是度量一幅图像和一段文本的相似性，该技术是多个模式识别任务的核心算法。例如，在图像文本跨模态检索任务中，当给定查询文本，需要依据图像文本的相似性去检索内容相似的图像；在图像描述生成任务中，给定一幅图像，需要依据图像内容检索相似的文本，并以此作为(或者进一步生成)图像的文本描述；在图像问答任务中，需要基于给定的文本问题查找图像中包含相应答案的内容，同时查找的视觉内容反过来也需要检索相似文本预料作为预测答案。 

.

## 2.研究现状

尽管图像文本本质上表征了同一组语义概念，但是它们的表现形式由于不同数据模态间较大的差异性而相去甚远。如何鲁棒的表示图像和文本、精准的度量两者的相似性是一个棘手的问题。为了处理这一问题，现有方法按照对图像文本对应关系建模方式的不同主要可以被分为两大类：1)一对一匹配和2)多对多匹配，如图1所示。 
![这里写图片描述](https://img-blog.csdn.net/20170501121557007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图1：图像文本匹配常用方法之间的对比。

一对一匹配方法通常是分别提取图像和文本的全局特征表示，然后利用结构化或者典型相关分析的目标函数将它们的特征投影到一个共同空间，使得相似的成对图像文本在空间中的距离接近，即相似性高。但是这种匹配方式只是粗略度量的图像文本的全局相似度，并没有具体的考虑图像文本具体是哪些局部内容在语义上是相似的，因此在一些要求精准相似性度量的任务中，例如细粒度的跨模态检索等，其实验精度往往较低。 

多对多匹配方法则是尝试从图像文本中分别提取所包含的多个局部实例，然后对于多个成对实例度量其局部相似性并融合得到全局相似性。但是这些方法所提取的实例并不都刻画了语义概念，事实上，大部分实例都是语义上毫无意义且与匹配任务无关的，只有少部分显著的语义实例决定了匹配程度的好坏。那些冗余的实例也可认为是一些噪声干扰了少部分语义实例的匹配过程，并增加了模型计算量。此外，现有方法的在实例提取过程中通常需要显式的使用额外目标检测算法或者昂贵的人工标注。 

.
## 3.我们的方法

![这里写图片描述](https://img-blog.csdn.net/20170501121635131?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2：提出的选择式多模态循环网络用于图像文本匹配。
为了解决上述问题，我们提出了选择式多模态循环网络模型，它可以按照时间步循环选择显著的语义图像文本实例，并序列化度量和融合它们的局部相似性并最终得到全局相似性。如图2所示，当给定一对图像文本数据时，该模型首先粗略的提取多个候选实例，例如图像中的区域和文本中的词语。基于这些候选实例，该模型在每个时间步使用多模态上下文注意机制来选择性关注一对图像文本实例(标记为同样颜色的圆圈和矩形框)，并度量它们的相似性，具体流程可以参照图3。类似在多个时间步内，提出的模型利用循环网络的多个隐含状态捕捉和融合多个不同成对实例的局部相似性，最终得到图像文本的全局相似性。 
![这里写图片描述](https://img-blog.csdn.net/20170501121641771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3：在一个时间步t下的模型细节。
该模型在同一个框架下实现成对图像文本实例提取、局部相似性度量和融合，且其参数可以利用一个结构化损失函数进行联合端到端的学习。模型学习采用误差沿时间反向传播算法来获取目标函数关于所有权值参数的梯度，并利用随机梯度下降算法来优化目标函数。为了验证提出的选择式多模态循环神经网络的有效性，我们测试了该模型衍生出的多种网络结构，并在两个公开多模态数据库(Flickr30k和Microsoft COCO)上与当前最好方法进行了对比。更多详细内容请参考以下这篇文章： 

Yan Huang, Wei Wang, and Liang Wang, Instance-aware Image and Sentence Matching with Selective Multimodal LSTM, arXiv:1611.05588, 2016.

.

## 延伸一：Reading text in the Wild：用图片找文字

牛津视觉几何小组运用深度学习技术开发了一项应用，它可以读出视频中的文字，输入文字，就可以直接搜出图像中包含这些文字的BBC新闻视频。比如，这是对“London”的一部分搜索结果： 
![这里写图片描述](https://img-blog.csdn.net/20170501121909696?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Demo：

[Visual Geometry Group Home Page](http://www.robots.ox.ac.uk/~vgg/research/text/#sec-demo)

Paper：

[[1412.1842] Reading Text in the Wild with Convolutional Neural Networks](https://arxiv.org/abs/1412.1842)

.

## 延伸二：[An Empirical Study of Language CNN for Image Captioning](https://arxiv.org/abs/1612.07086)

> 
来源于[paperweekly:多模态小组阅读笔记精选 | 每周一起读](https://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484521&idx=1&sn=eb91bdf94b6d38ffa9fd60f1ad6e1088&chksm=96e9dbe9a19e52ffd7eaaf7d52e4453d2056cebc3609f76af245f772b51f6af54c8ba76d8713&mpshare=1&scene=1&srcid=0425Z6tzjQiDUKNrnPUY0UUa#rd)


本篇论文提出了用 CNN 模型来对单词序列进行表达，该 CNN 的输入为之前时刻的所有单词，进而可以抓住对生成描述很重要的历史信息。其中总体架构如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170501122053011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
该模型主要由四部分组成，用于图像特征提取的 CNN_I，用于自然语言建模的 CNN_L，用于结合 CNN_I 和 CNN_L 信息的多模态层 M，和一个用于单词序列预测的递归神经网络。 

总体过程如下：首先利用 CNN_I 提取图像特征，然后 CNN_L 利用之前时刻生成的单词对当前的单词信息进行表达，然后通过多模态层结合图像和单词信息，最后将融合的信息作为递归神经网络的输入来预测一下时刻的单词．该文与之前通过 one-hot 向量，然后经过词嵌入提取词向量的表达方法不同，利用了 CNN 网络来表达单词信息，进而能够很好的抓住过去的历史信息，用于指导当前时刻单词的生成。 

.
## 延伸三：基于选择式多模态循环网络的图像文本匹配

来源文章[《【技术分享】像人脑一样理解周围世界：脑启发的深度学习模型及其应用》](http://mp.weixin.qq.com/s?__biz=MzI1MzU2ODEwNg==&mid=2247483869&idx=1&sn=e871c465cc0832824a8e214b43b6aa19&chksm=e9d334d8dea4bdce6dc2853ff5c774078ef59c039e770f802d19603595fa3e3a206c52ac3af2&mpshare=1&scene=1&srcid=0606Yj8lCos5SLH2eHKUyA6C#rd)

图像文本匹配是多个模式识别任务，例如图像文本跨模态检索、图像描述生成以及视觉问答系统中的一个关键科学问题。当前用于图像文本匹配的算法通常对图像文本提取全局特征，然后利用典型关联分析或者子空间学习策略来拉近匹配的图像文本对。但考虑到图像文本都包含很多冗余信息，具体来说，只有图像文本中的部分显著的、表征同一语义概念的内容才能被关联起来。因此，我们提出了一种基于选择式多模态循环网络的图像文本匹配方法，可以选择性关注和匹配图像文本中的语义实例。所提出的选择式多模态循环网络是一个动态模型，在每一时间步，它利用基于上下文的多模态注意机制选择图像文本中语义上相同的目标和词语，并计算其相似性作为图像文本的局部相似性，然后进行序列化融合得到全局相似性。

来源文章： Yan Huang, Wei Wang, and Liang Wang, Instance-aware Image and Sentence Matching with Selective Multimodal LSTM, IEEE International Conference on Computer Vision and Pattern Recognition (CVPR), 2017.

.

## 延伸四：基于联合子空间学习的细粒度草图检索

草图，作为一种手绘的简化版的图像描述，在图像表达中具有重要的研究价值，我们针对草图到自然图像的检索问题进行了深入的研究。考虑到草图与自然图像可能存在多视角的特征表达，且不同的视角作用差异较大，我们提出了一种基于视角选择的多视角跨模态匹配算法。通过引入结构化稀疏约束，我们可以在多模态子空间学习的同时自动区分不同视角之间的判别性，进而有效建模草图与其对应自然图像之间的一致特性。我们在两个经典的细粒度草图-图像数据集上进行了大量的实验，结果表明所提出的方法可以有效利用多模态多视角特性并提升检索性能。 

——Peng Xu, Qiyue Yin, Yonggang Qi, Yi-Zhe Song, Zhanyu Ma, Liang Wang, Jun Guo, Instance-Level Coupled Subspace Learning for Fine-Grained Sketch-Based Image Retrieval, ECCV workshops, 2016. 
![这里写图片描述](https://img-blog.csdn.net/20170612104813435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.

## 延伸五：Poster论文：基于图模型神经网络的情景识别

来源：[【封面推荐】收藏慢慢看！“腾讯优图”这12篇论文，入选计算机视觉顶级会议ICCV 2017](https://mp.weixin.qq.com/s?__biz=MzIwMjY2NDA5Mg==&mid=2247484701&idx=1&sn=c8fb115d97b8bd6205cc4b2ec60130b0&chksm=96da74eea1adfdf8e7a6e4cb2724ba135ad10c520dc2b12e106fcf8d701bc358a53c691be9c4&mpshare=1&scene=1&srcid=1019Va1opn2RIBY9kcPR2xu2#rd)

（Situation Recognition with Graph Neural Networks） 
![这里写图片描述](https://img-blog.csdn.net/20171029112251743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

本论文与香港中文大学和多伦多大学合作完成，作者提出了一种基于图模型的神经网络用于情景识别任务。
在情景识别任务中，算法需要同时识别图中所展示的动作以及参与完成这个动作的各种角色，比如主语、宾语、目标、工具等等。为了显式地对不同角色间的关系建模，文中提出的图模型神经网络将表示不同角色的节点连接在了一起，并通过信息传递的方式使得网络可以输出一个结构化的结果。

作者在实验中比较了不同的连接方式，比如线形结构，树形结构和全连接结构，发现在情景识别任务中全连接结构的效果最好。最后，文中还展示网络所学习到的对于不同动作的特有的连接结构。

上图所示的结果图，比较了不同模型的检测结果。其中蓝底的表示参与动作的角色，绿底表示正确的预测结果，红底表示错误的预测结果。我们可以看到，使用全连接图模型能够纠正一些由其他模型产生的错误。

### 延伸六：“诗画合一”的跨媒体理解与检索——跨媒体理解：video captioning

[“诗画合一”的跨媒体理解与检索](https://mp.weixin.qq.com/s?__biz=MzI1NTE4NTUwOQ==&mid=2650330510&idx=1&sn=fe33e22986932ec05703d6f428d7ffef&chksm=f235b284c5423b925527a9b14e1616e3d5d7b103f5d5de5d805a9e6d465ef2ad2eef20a9db78&mpshare=1&scene=1&srcid=03066NirFmrjqj58ETLMcZxc#rd)

而video captioning领域的基础模型是ICCV 2015上的“Sequence to sequence-video to text”，是基于多对多的多层RNN网络来构建的。为了将视频中的事件解码为描述该事件的语句，这篇文章提出了一种双层LSTM方法，来学习如何表达视频帧序列。其中，上层LSTM(图中红色)用来建模输入视频序列的视觉特征，第二层LSTM(图中绿色)从文本输入以及视频序列的隐性特征，来建模最终的表达语言。图中表示语句的开头，表示句末的标签，表示该时间戳上输入为空。该模型可以同时学习视频帧的时序结构和生成语句的序列模型。

大多数现有的方法中，解码器为生成的所有单词都应用注意力机制，包括non-visual words（如“the”、“a”）。然而，事实上，对这些non-visual words使用自然语言模型已经能够很好地进行预测，为其强加注意力机制反而会误导解码器、并降低video captioning的整体性能。

针对这一问题，我们提出了一种层级LSTM框架（hLSTMat），它带有可调节功能的时间注意力机制，通过注意力机制选取一些特定的帧，并利用层级的LSTM来建模视频帧的低层视觉信息和高层语境信息，然后根据可调节的时间注意力机制，来选择解码是依赖于视觉信息还是语境信息。

##### 多模态搜索

网络上充斥着来自不同数据源的多模态多媒体数据；因此，亟需能够适应各种模态的信息检索系统，例如，在搜索“Dunkirk”电影时，应返回影评的相关文本数据、包含相关视频片段的视频数据、以及相关音频数据；而跨媒体检索指的是：给定一个模态的输入，来查找其他模态中与之最相近的匹配结果。 

以上图中两个例子来说明跨媒体检索，如Text2Img和Img2Text。 

深度方法的两个分支
在跨媒体检索领域，常利用深度方法对不同模态的数据进行多层非线性特征提取，并将其映射到公共子空间，而后进行相似性度量。

而在相似性度量上，其存在两个分支：

1）从统计的角度出发，例如采用典型关联分析（Canonical Correlation Analysis，CCA）方法来获取不同模态数据的匹配关联程度。

2）从数据语义的角度来进行不同模态数据的匹配，在此基础上，通过加入三元组的限制条件，来提高匹配精度。遵循的原则为：在最小化同一语义数据在不同模态之间距离的同时，最大化不同模态不同语义数据之间的距离。













