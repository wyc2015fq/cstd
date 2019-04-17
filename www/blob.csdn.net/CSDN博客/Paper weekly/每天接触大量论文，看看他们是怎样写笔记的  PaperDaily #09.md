# 每天接触大量论文，看看他们是怎样写笔记的 | PaperDaily #09 - Paper weekly - CSDN博客





2017年11月01日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：468












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。

这是 PaperDaily 的第 **9** 篇文章

你是不是也经常在读完一篇论文之后没多久就忘记内容了？

其实这只是人类记忆曲线的正常表现，不要因此而出现「真没用」「白读了」之类的想法，**读论文也并不是为了「记忆」，而是为了「思考」**。当然，有一个准确清晰的记忆可以让思考变得更加轻松。

也许你会想「论文笔记？没写过！我水平不够啊！」别担心，**本期我们精选了****四位PaperWeekly 社区用户的论文笔记**，大家在增长知识点的同时，还能快速 get 怎样写一份「**真正有用的论文笔记**」。




# 多文本摘要

■ 论文 | Improving Multi-Document Summarization via Text Classification

■ 链接 | http://www.paperweekly.site/papers/982

■ 作者 | JingwenJessica

**提出问题**

文章指出目前 MDS 遇到的问题是用于训练的 datasets 太少，而文本分类相关的 datasets 更多一些。 

**作者观察**

作者观察到的一个事实是不同分类的文章，摘要的风格也不同。飓风类的文章摘要侧重于描述飓风的路径和带来的损伤；而自传类的文章摘要侧重于人物的简历和贡献。 

**提出模型 **

基于上述两点，作者提出了 TCSum 系统，TCSum 是结合文本分类的多文本摘要，TCSum 系统的 contributions ： 

a. 利用文本分类的 datasets 优化了用于摘要的 document representations； 

b. 不同分类的文章生成不同风格的摘要；

c. 不需要人工标注 features。

**TCSum 系统**

包括 text classification model 和 summarization model 两部分，用同一个 document embedding。 

• 利用 CNN 模型生成 document embedding：在 pre-trained word embedding 上用 tanh 做 convolution
 operations，用 max-over-time polling 做 polling operations 生成 sentence embedding，Sentence embedding 做 average pooling 即为 documentt embedding。

• Text Classification Model：document embedding 上用 softmax 后作为文本分类的概率分布。用
 cross entropy 做为 cost function。

• Summarization Model：Document embedding 上用 tanh 后作为文本摘要“含义”的特征向量，这就是文中提到的
 transformed embedding （summary embedding）。 做 tanh 时用到的权重 w 也和文本分类相关。句子 salience score：summary embedding 和 sentence embedding 做 cosine similarity，pairwise ranking strategy。

• Datasets：DUC，the New York Times (NYT) Annotated Corpus

# 情感分类

■ 论文 | Learning to Generate Reviews and Discovering Sentiment

■ 链接 | http://www.paperweekly.site/papers/683

■ 作者 | xwzhong

**Note**

1. 文章使用 Amazon 商品评论数据（38G）训练了一个 1 层 4096 个 unit 的语言模型，这 4096 个 unit 中，发现了一个 sentiment unit，能指示待 encoded 中每个字（或词）的情感极性（正面或负面），另外该句子 encoded 后，还能判断整个句子的情感极性，在
 IMDB 数据集上，错误率降低到 7.7%（state of art 方法为 5.91%）。 

2. 训练好语言模型后，通过很少的标注数据（30个）就能超过在 Stanford sentiment treebank 数据集下的 state of art 方法。

3. 在使用语言模型生成句子时，能通过人工直接控制 sentiment unit 的值来决定所生成文本的情感。 

**Comment**

1. 语言模型仍有很多未知的潜力，对于 hidden unit，我们对其仍知之甚少。

2. 文本中没提及如何寻找这个 sentiment unit，但是可以尝试使用已标注的相近领域情感分类数据来找。

3. 其它 unit 是不是也反映了数据在某方面的特点，已知的有句子长度，会包含语义上的转折 unit？ 

4. seq2seq 会不会有同样的 unit？ 

5. 如果 language model 有很多理想的 unit（大家想通过这些 unit 来控制生成），是不是会有 lang2seq 模型（language to sequence），这样既利用了 language model 能用大量数据无监督学习的特点，还能利用 seq2seq end2end 的特性。 

**Practice**

1. 通过领域训练得到的 language model 在特定领域使用时，如果语料的 overlap 不高，效果不一定特别好，因此可在通用领域训练好的 model 基础上，用待解决问题领域的数据进行 fine tuning。其它运用还有 word2vec。 

2. 利用好这个已发现的 sentiment unit，不仅可以减少人工标注数据来训练情感分类器，还能直接控制文本生成等等。 

**More Reading**

[1] http://it.sohu.com/20170407/n486996650.shtml 

[2] https://github.com/openai/generating-reviews-discovering-sentiment




# 生成式对抗网络

■ 论文 | Adversarial Generation of Training Examples for Vehicle License Plate Recognition

■ 链接 | http://www.paperweekly.site/papers/421

■ 作者 | WarBean

在 OCR 文字识别中，可利用的数据主要有三类：采集自真实世界的有标注样本（labelled real sample），采集自真实世界的无标注样本（unlabelled
 real sample），以及人工生成的有标注样本（labelled generated sample）。一般来说，labelled real 需要标注人员手动标注，获得成本高，数量没法太多；unlabelled real 采集成本低，可以比前者多很多；而 labelled generated 则要多少有多少。 

如果光用 labelled real 训练模型，很容易因为数量太少而使模型过拟合，所以一个常见的做法是先用大量的 labelled generated
 预训练，再用少量 labelled real 去 fine tune，这种做法往往可以获得不错的效果。 

问题是人工生成的样本往往和真实样本有差异，所以用它们来预训练会给模型参数带来一定的先验偏差，从而损害其在真实样本分布上的性能；另一方面，如此大量的
 unlabelled real 用不上，也实在是一种浪费。如果能够利用上这些 unlabelled real 数据，来帮助模型更好地学习真实分布下的识别模型，就能够取得更好的识别准确率；或者反过来说，在达到同等准确率的条件下，需要的人工标注量可以更少。 

这篇论文就尝试在车牌识别这个具体任务上实现上述效果，其大体思路是： 

1. 先生成一堆 labelled generated 的图像；

2. 将 labelled generated 和 unlabelled real 两堆图片放一起，训练一个 CycleGAN 实现这两个 domain
 之间的相互转换； 

3. 利用训练好的 CycleGAN 将大量 generated 的图像转换为 real 图像样式，同时又保持其图像中的文本内容不变，因此我们能够知道其
 ground truth label；

4. 如此一来就可以得到大量的仿照真实样本的 labelled data，用这些转换后的数据去预训练识别模型；

5. 再在 labelled real 上 fine tune，最终可以获得明显的性能提升。 

下面 4 幅图分别是人工生成样本、CycleGAN 转换样本、CycleWGAN（本文改进的一个变种）转换样本、真实样本的样子：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgliatQT7YoMkCXr9toQ0wTcNMca1O8Ynn7k9GtGSgLlibiazoicjzSbjVVdJVzwVn6QkkbFIniakJzhjNw/?)




不得不说这篇论文对付的场景还是偏简单的，首先是真实数据的样式其实没有太复杂，其次是人工生成的样本就已经很接近真实数据了，要转换过去并不会太困难。

文中给出了多项对比实验结果，下面是两个比较有代表性的：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgliatQT7YoMkCXr9toQ0wTcNWDzXuOnJZtmYfZL8Xejia6KD1aW9A882D4Ag3NXgbL6UsYHXsQZib3gA/?)

第一个表是在没有用到 labelled real 的条件下，识别模型分别在随机初始化、在人工生成样本上训练、在 CycleGAN 转换样本上训练、在 CycleWGAN 转换样本上训练后，在真实测试集上的整行识别准确率。 

可以看出这四种情况是一个比一个更加接近真实分布的。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgliatQT7YoMkCXr9toQ0wTcNRDMb1HjZ4x9RSANRHibZXQTkuAicPMjuTg4wYyd6TrDsQF4x6fCgW4wQ/?)

第二个表就用到 labelled real 去 fine tune 了，同样可以看出人工生成样本加进来有提升，但是 CycleWGAN 转换样本加进来后提升得更多。

# 生成式对抗网络

■ 论文 | Progressive Growing of GANs for Improved Quality, Stability, and Variation

■ 链接 | http://www.paperweekly.site/papers/1008

■ 作者 | lgpang

这篇文章就像 YY 小说中的主角一样自带主角光环，从 GAN 存在的众多问题出发，提出了一个个新颖但是无往不利的方法。只是看结果就震撼莫名，通篇读下来更是觉得
 NVIDIA 一下子放了好多大招（至少对于 GAN 这个分支来说）。 

1. 以往的 GAN 生成低分辨率图片稳定迅速，但生成高分辨率图片困难重重。这篇文章从简单的低分辨率图片开始同时训练生成器和判别器，然后逐层增加分辨率，让训练的难度每层只增加一点点。感觉就像是算法里面的暴力搜索到二分法搜索，大大提高了高分辨率图片的生成速度及质量。 

2. 以往没有好的办法去判断 GAN 生成的图片是好是坏，很多时候需要肉眼来看，有很大的主观性，而且人能检查的样本空间不够大。文章的第 5 节介绍了如何用统计的方法来直观的判断生成样本的好坏，采用的思路是在各个尺度上抽取
 7x7 个像素的局域碎片，比较生成图片与训练图片的局域结构相似性。 

3. GAN 生成图像的多样性不好量化，当判别器过强时生成器可能会塌缩到单个类。这篇文章不添加任何超参数，只是将所有属性在所有空间位置的统计标准差求平均，作为卷积神经网络
 Feature Map 的一个常量通道，就得到了更好的多样性 -- 赞。 

4. 使用了一种“local response normalization”方法来对 FeatureMap 做归一化，不清楚与 BatchNormalization
 的效果相比有没有提升。 

5. 在 WGAN-GP 的正规化项中使用非常大的 gamma 因子，从公式上看当生成图片与训练图片相差过大时，大的 gamma 因子可以让生成分布快速漂移到训练图像分布。

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**



 游戏时间



** GAN 还是不 GAN？**




GAN主题论文共读小组




**论文提名√在线投票√论文共读**√****




活动形式：语音直播 + 协同批注



** 游戏规则 **

1. 自由推荐任何与GAN有关的论文

2. 投票支持你想读的论文

3. 每周六根据投票结果选出本期论文




**长按识别二维码，马上报名**

*添加好友请注明“GAN”



![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglfCMtmAezkuNsFdqguLwph9EEYhFfQVNpDcb9w8Icg2IDNlh0YmOn95QYentTt8JA3mlxa9hNb3Q/?)






******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)




