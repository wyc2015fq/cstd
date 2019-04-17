# 本周 AI 论文良心推荐，你想 pick 谁？ - Paper weekly - CSDN博客





2018年05月25日 12:24:13[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：626

















在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **75** 篇文章



**Adaptive Scaling for Sparse Detection in Information Extraction**

**@roger 推荐**

#Information Extraction

**本文是中科院软件所发表于 ACL 2018 的工作，论文主要研究信息抽取中检测任务的类别不平等问题**。该问题具体体现在：1. 训练样本类别不均衡；2. 神经网络常用的交叉熵目标函数与评价指标（F-score）不平等。 

为解决这一问题，**本文提出了一种基于边际效用的动态调整训练实例权重的解决方案**，在 Batch-wise 的训练过程中直接针对 F-score 进行优化，同时不需要引入超参数。 

本文分别在 KBP2017 的中英文事件检测任务进行了实验验证，实验结果证明了 AS 方法的有效性和稳定性。类别不平等问题在自然语言处理领域中广泛存在，AS 方法可以被应用在更多的自然语言处理任务中。






论文链接

https://www.paperweekly.site/papers/1963



代码链接

https://github.com/sanmusunrise/AdaScaling





**Self-Attention Generative Adversarial Networks**

**@xwzhong 推荐**

#Generative Adversarial Networks

**本文来自谷歌大脑 Ian Goodfellow 组，论文提出了一个基于注意力机制的对抗网络（SAGAN）**，将自注意力机制引入到卷积 GAN 中作为卷积的补充，在 ImageNet 多类别图像合成任务中取得了最优结果。






论文链接

https://www.paperweekly.site/papers/1967






**A Unified Model for Extractive and Abstractive Summarization using Inconsistency Loss**

**@llamazing 推荐**

#Text Summarization

**本文是国立清华大学发表于 ACL 2018 的工作**，论文主要贡献如下：

1. **作者提出一个抽取式和提取式融合的模型来做 summarization**，利用抽取式模型获得 sentence-level 的 attention 进而影响 word-level 的 attention；提出 inconsistency loss；

2. CNN/Daily Mail 数据集 ROUGE 分数超过抽取式模型 lead-3，本文的模型可看作是 pointer-generator 和抽取式模型的融合；

3. 用Hierarchical 的结构（word-level encoding 和 sentence-level encoding），分别对 sentence 和word 做 attention；sentence 的 attention 权重使用 sigmoid；word 的 attention 权重计算时用 sentence-level 的 attention 权重进行 scale；

4. Inconsistency Loss 对 decode 每个 step 的 topK attention word 的 word-level 和 sentence-level 的 attention 乘积做 negative log；鼓励 word-level attention sharp，sentence-level 的 attention high。






论文链接

https://www.paperweekly.site/papers/1953








**DLBI: Deep learning guided Bayesian inference for structure reconstruction of super-resolution fluorescence microscopy**

**@lykaust15 推荐**

#Image Reconstruction

**本文是基于 GAN 和贝叶斯方法的荧光显微超分辨研究**。荧光显微超分辨是光学成像的突破性研究，获得 2014 年诺贝尔化学奖。它将光学显微镜的成像极限分辨率从 300nm 提高到了 40nm。

这个方向一个重要的计算问题是从一串时间序列的高噪声低分辨率的图像重构出一张高分辨低噪声的超分辨率图像。之前的方法是基于贝叶斯和 FHMM 模型建模重构，时间复杂度非常高，重构出一张 480*480 的图像需要 75 个小时。 

**在这个领域使用深度学习非常有希望继续提高极限分辨率以及提高重构速度**。但是有 4 个问题需要解决：1. 标准的图像超分辨是没有噪声的，但是这里的图片噪声非常大。2. 没有足够大的训练数据集。 3. 已有的训练数据集并没有 ground truth。4. 单纯使用深度学习，非常有可能在最终结果中引入实际不存在的细节，对于光学成像，即使看不清，也不希望看到错误的细节。 

**这篇文章使用了以下方法解决了上述问题：**

1. 基于荧光显微成像的物理学原理，构建了一个可以根据给定的高分辨图像产生低分辨高噪声图像的模拟器用于产生源源不断的数据；

 2. 在 GAN 的生成网络中加入了MC dropout，denoise shortcut 以及使用 multiscale training 的方式来抑制噪声和实际不存在的细节； 

3. 将深度学习的结果作为贝叶斯方法的先验，进一步重构同时抑制噪声。这一步虽然是使用了之前的方法，但因为深度学习的结果已经非常好，所以贝叶斯方法的收敛速度也比之前显著提高。 

文章通过大量的实验（包括模拟数据和真实数据）证明该文章提出的方法可以重构出比之前的方法（3B，2012， Nature Methods）细节更加丰富的超分辨图像，同时，速度有了及其显著的提高：如果直接使用深度学习的方法作为输出（重构一张 480*480 图片只需要 3 分钟），比 3B 方法快 1500 倍，如果进一步使用贝叶斯方法改善结果，也还是要比 3B 方法快 150 倍。这种显著的速度提升使实时重构和大范围重构成为可能。 

文章提出的训练方法和解决噪声的方法同样适用于其他类似问题和领域，比如医学图像（CT，fMRI）超分辨。






论文链接

https://www.paperweekly.site/papers/1964




代码链接

https://github.com/lykaust15/DLBI








**Sparsely Grouped Multi-task Generative Adversarial Networks for Facial Attribute Manipulation**

**@zhangjichao 推荐**

#Image-to-Image Translation

本文认为先前的基于 unpaired 数据的方法，比如 StarGAN 和 CycleGAN，仍然需要分类标签并且消耗一定的人力。因此**文章提出了一种 sparsely grouped 形式的数据形式，并且提出一种在此数据集上可以训练的类似半监督对抗网络的学习框架和目标函数**。

实验通过在 celebA 数据集上做面部参数的调整应用验证了方法的有效性。论文还通过定量定性的评估说明方法的优越性。最后，论文还给出了图像翻译在数据不平衡下遇到的问题，并且说明了本文提出的方法可以缓解这种问题。






论文链接

https://www.paperweekly.site/papers/1965








**Nugget Proposal Networks for Chinese Event Detection**

**@roger 推荐**

#Chinese Event Detection

本文是中科院软件所发表于 ACL 2018 的工作。**论文主要针对事件检测，尤其是中文事件检测任务当中的触发词块与词的不匹配问题，提出了一种名为 Nugget Proposal Networks (NPNs) 的字级别标注模型**。

不同于传统的字级别和词级别模型，该模型在每个字符上要求模型能够预测出整个完整的触发词块，从而使得预测结果有了更高的容错率。同时，文章在进行预测的时候，同时利用了字符级别和词级别的语义信息，使得预测结果更加准确。

本文分别在 KBP2017 和 ACE2005 的中文事件检测数据集进行了实验验证，与传统的基于字符级别 BIO 标签、基于词级别的方法相比，NPNs 取得了显著的提升。

词与待检测块之间的不匹配问题在自然语言处理领域中广泛存在，尤其在中文这样没有自然词汇边界的语言中更为明显，NPNs 模型可以被应用到更多相关的自然语言处理任务中，例如命名实体识别。






论文链接

https://www.paperweekly.site/papers/1962




代码链接

https://github.com/sanmusunrise/NPNs








**An Empirical Evaluation of Generic Convolutional and Recurrent Networks for Sequence Modeling**

**@Ttssxuan 推荐**

#Sequence Modeling

在面对序列数据时，一般立即想到的是使用 RNN 网络，但经过本文以及相关文章的研究表明，**面对序列数据时，卷积网络更应该纳入考虑范围**。 

**本文针对卷积网络在序列上的应用提出 Temporal Convolutional Networks (TCN)**。网络使用 1 维卷积处理序列问题，使用因果卷积（Causal Convolutions）处理序列中的顺序关系、使用扩展卷积（Dilated Convolutions）实现对历史信息的接收范围指数级扩张、使用 Residual Connections（实现时使用 1X1 卷积）处理深度网络信息传递问题。 

**TCN的主要优点有**：并行、灵活的接收域、稳定的梯度、训练占用内存少、可变长输入。 

本文在 The adding problem、Sequential MNIST and P-MNIST、Copy memory、JSB Chorales and Nottingham、PennTreebank、Wikitext-103、LAMBADA、text8 等多个数据集上进行试验，与 LSTM、GRU 等网络进行对比，在结果和性能上 TCN 都取得了相当不错的优势。






论文链接

https://www.paperweekly.site/papers/1940



代码链接

https://github.com/locuslab/TCN








**MobileFaceNets: Efficient CNNs for Accurate Real-time Face Verification on Mobile Devices**

**@halfmoontzh 推荐**

#Face Recognition

**本文提出了一个工业级精度和速度的轻量级人脸识别网络**，模型大小只有 4MB，速度超过了 MobileNetV2 和 ShuffleNet，专为人脸识别任务设计，精度比肩大模型。






论文链接

https://www.paperweekly.site/papers/1952




代码链接

https://github.com/moli232777144/mobilefacenet-mxnet








**Billion-scale Commodity Embedding for E-commerce Recommendation in Alibaba**

**@stevechoris 推荐**

#Recommender System

本文是阿里巴巴和香港科技大学发表于 SIGKDD 2018 的工作，**论文结合节点 side information，解决了图表示学习中稀疏性和冷启动问题，在电商 i2i 推荐上取得很好的效果**。






论文链接

https://www.paperweekly.site/papers/1939








**Paper Abstract Writing through Editing Mechanism**

**@llamazing 推荐**

#Text Generation

本文是伦斯勒理工学院和南加州大学发表于 ACL 2018 的工作，**论文研究的问题是给定 title 生成摘要**，一个 writing 网络一个 editing 网络，跟 deliberation network 有点类似，对 decoder 出的文本进行 refine。






论文链接

https://www.paperweekly.site/papers/1943



代码链接

https://github.com/EagleW/Writing-editing-Network








**Show, Tell and Discriminate: Image Captioning by Self-retrieval with Partially Labeled Data**

**@qliu 推荐**

#Image Captioning

**作者主要想解决的问题是生成的 caption 缺少多样性的问题**，即多幅相似的图片可能会生成几乎一样的 caption。

作者认为，如果 caption 足够具有多样性的话，那么这幅图生成的描述应当与本幅图最相似，与其他图像没那么相似。相反，如果两幅图生成的描述相同，那么这个描述与两幅图的相似性即是一样的或者是难以区分的。当 I1 生成的 C1 时，C1 与 I1 相似性应当高于 C1 与 In(n≠1)。 

**这篇文章的新颖之处有两点：**

**1. 使用了 unlabeled images（不含有 ground truth 的图像）进行训练**，之前的大部分 image caption 的工作都是基于各个图像数据库进行训练，如果能够使用无人工标注的图像进行训练，训练集将会大大增加。 

为什么能够用到 unlabeled images 呢？作者发掘出了 unlabeled images 里天然存在的“label”—即 In 与 Cn 的一一对应关系。即一幅图通过生成网络生成的描述肯定是与这幅图对应的，那么这个描述的标签就是这幅图。

**2. 使用了 self-retrieval 网络**。这篇文章的模型主要分为两个版块，第一个版块是 caption module，第二个版块是 self-retrieval module。

其中 self-retrieval module 通过计算由 caption module 生成的 ci 与 {I1,I2,I3……，In} 之间的相似性，来计算 self-retrievel module 对 caption module 的 reward。最后用强化学习的方法把 reward 反馈给 caption module。 

**3. 运用强化学习将 CIDer 指标作为 reward**。这个已经不算很新颖了，在 Self-critical Sequence Training for Image Captioning 这篇文章中已经运用强化学习将 CIDer 指标结果作为 reward 反馈给 caption module。

这里由于增加了一个 self-retrieval module，因此多了一个 reward—Rret。所以 labeled images 的 reward 会分为两部分 Rcider 和 Rret。由于 unlabeled images 没有 Rcider，因此 reward 只有 Rret。






论文链接

https://www.paperweekly.site/papers/1948








**Learning to See in the Dark**

** @paperweekly 推荐**

#Few-shot Learning

本文是 UIUC 和英特尔发表于 CVPR 2018 的工作。极低照明度下，传统增加亮度的方法会放大噪点和色彩失真。**本文通过全卷积网络处理这一类黑黑的照明度图像，并在抑噪和保真的条件下将它们恢复到正常亮度**。这种端到端的方法将为机器赋予强大的夜视能力，且还不需要额外的硬件。






论文链接

https://www.paperweekly.site/papers/1946



代码链接

https://github.com/cchen156/Learning-to-See-in-the-Dark







**#****推 荐 有 礼#**




本期所有入选论文的推荐人

均将获得**PaperWeekly纪念周边一份**









**▲ **机器学习主题行李牌/卡套





**▲ **深度学习主题防水贴纸



**想要赢取以上周边好礼？**

**点击阅读原文**即刻加入社区吧！









**点击以下标题查看往期推荐：**




- 
[来不及想标题了，我要去打包收藏了](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488060&idx=1&sn=8214e778bfc381bf684ab634a34dbf34&chksm=96e9cdbca19e44aac9ffb5bb402861e3295bb3e42b98fe7d964c422995b06875f92333fd85d1&scene=21#wechat_redirect)

- 
[快醒醒，一大波最新 AI 论文加开源代码来袭！](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488566&idx=1&sn=5af3e8b73b2d71003e9af12fb04a43b7&chksm=96e9cbb6a19e42a08722cba30ca9b663d38406f23f882fb123b5802f686391510036ad0a2da0&scene=21#wechat_redirect)


- 
[15 篇最新 AI 论文来袭！NLP、CV...人人有份](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489270&idx=1&sn=4fa88468dba51738df921da45573a927&chksm=96e9c976a19e4060c20453f9cb275966ba25522292b9b638d712963edf208822686486b2cbb7&scene=21#wechat_redirect)

- 
[选对论文，效率提升50% | 本周值得读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487741&idx=1&sn=b61741b47e602626a236f5984a0b1cb4&chksm=96e9cf7da19e466b953b4f0fb4e0003b868045fd1a4eb1b38a2b6cfe5316c60bcd368f4ee985&scene=21#wechat_redirect)

- 
[入坑AI却不知道该读什么？这15篇最新论文给你答案](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488947&idx=1&sn=2c58571b86370179d47d74eea01b3c36&chksm=96e9ca33a19e4325269c3c45b3049baedaa790befb309ae98bf8844a898bba693e364c9f8450&scene=21#wechat_redirect)






[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488603&idx=2&sn=7320cb23efba3e7b5a381be83b7fe3ad&chksm=96e9cbdba19e42cd5840d3d51e86da4709b3d5273b2cf2512c32d84ab2b42ac4e7f13bf9ba63&scene=21#wechat_redirect)

[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)

**▲**戳我查看招募详情




**#****作 者 招 募#**



****[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)****






**关于PaperWeekly**




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。









▽ 点击 | 阅读原文| 加入社区刷论文




