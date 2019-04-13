
# NLP 专题论文解读：从 Chatbot 到 NER | PaperDaily \#11 - Paper weekly - CSDN博客


2017年11月08日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：397



![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**11**篇文章
这里是第 11 期 PaperDaily。
本期论文笔记是由 PaperWeekly 社区用户**@Jeffreygao**，**@robertdlut**和**@lc222**带来的 NLP 专题，涵盖**对话系统**、**命名实体识别（NER）**和**QA 系统**。
如果有合你心意的论文，复制链接到浏览器即可查看原文。

# 对话系统
■ 论文 | Affective Neural Response Generation
■ 链接 | http://www.paperweekly.site/papers/1043
■ 作者 | Jeffreygao
**1. 论文动机**
论文来自华为诺亚方舟实验室。
都说人工智能要有情感，能体会到人的喜怒哀乐，今天就来介绍一篇带有情绪的 chatbot。在以往的研究中，大部分对话系统都只关注生成对话的语法语义是否合理，这里面有考虑上下文的，有结合主题的，有生成长句子的等等。但很少有对话系统关注情感，这是很不合理的。因为在聊天中，当一个人表示难过的时候，另一方经常会回应适当的安慰；当一方感到开心时，另一方也会为其感到快乐。就好比
 A 说“我的宠物狗去世了”，B 很自然应该回复“我为你感到难过”这样类似的话，同时这种带有情感交互的例子在我们日常对话中数不胜数。
实际上个人看来这个也是开放域对话之后一个很好的研究方向，毕竟有太多的 paper 去解决如何不产生“呵呵”这种无意义回答的问题了，只有真正的模拟人类的交谈才是好的 chatbot。
**2. 相关工作**
在这篇文章中作者提到了两个相关工作一个是 ACL2017 上的 Affect Language Model [Ghosh et al. 2017]，另一个是 Emotion Chatting Machine（ECM）[Zhou et al. 2017]，看
 arXiv 的排版应该是投给了 AAAI2018。
ECM 的模型相当漂亮，但是问题的定义很不合理，不合理在哪呢？这个模型是在给出 context，同时给出你想要生成的 response 的 emotion，然后生成一句带有这个 emotion 的 response。但是在日常对话中，并不会有上帝去给你指定要回复的
 emotion。
**3. 模型**
这篇论文的模型以 seq2seq 为背景，主要在三个点上做改进。（1）在 embedding 上加情感信息（2）改进 loss function（3）beam search 时考虑情感。
**3.1 带有情绪的词向量**
在这里使用了 VAD 字典，解释如下：*“Valence (V, the pleasantness of a stimulus), Arousal (A, the intensity of emotion produced, or the degree of arousal evoked, by a stimulus), and Dominance (D, the degree of power/control exerted by a stimulus). V-- 1, 5 and 9 corresponds to a word being very negative (e.g., pedophile), neutral (e.g., tablecloth) and very positive (e.g., happiness), respectively. A-- 1, 5 and 9 corresponds to a word having very low (e.g., dull), moderate (e.g., watchdog), and very high (e.g., insanity) emotional intensity, respectively. D-- 1, 5 and 9 corresponds to a word/stimulus that is very powerless (e.g., dementia), neutral (e.g., waterfall) and very powerful (e.g., paradise), respectively.”*
简单地说就是现在有个已经其他人评估好的词向量表了，词向量维度三维，每一维度分别采用感情的一个方面，表示形如：（V_score, A_score, D_score）。如果训练集中的单词在这个 VAD 表里，就取这个词向量；如果是不在表中的 oov 词，取 [5,
 1, 5] 作为比较中性的词，其实就相当于我们平时处理 oov 词时候的 unk 一样。
文章中具体的做法是将传统的词向量和这个情绪向量 W2VA 连在一起，作为 encoder 和 decoder 的 input。其实这个方法和 emotion chatting machine 的第一步是异曲同工的，只不过在 ecm 中这个情绪向量是学来的，没有用外部词典。
**3.2 带有情绪的目标函数**
作者在这里提出了三个不同的 loss function，其实在我看来也是不得已而为之，因为它没有更好的办法融合这些东西。
**3.2.1 最小化情感失调**
在这个 loss function 处，作者做的假设是：两个人聊天，情感不会太迅速或者频繁转变。举个栗子：你说一句比较友好的话，我也会回你一句比较友好的话，就是客套呗；你说一句挑衅的话，我会回你一句比较愤怒的话。很自然的，loss function 不但要考虑交叉熵还要考虑生成
 response 的情感和 input 的情感是不是接近，这里用欧氏距离来衡量。如下：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJkECfgnjibN8wnvE8sKhLnBo8Iv3fFxBLUCnQHGL2U21z0LOmibp8GnHA/?)
**3.2.2 最大化情感失调**
在这里，作者又做了个假设，比如两个不太熟的人聊天，一个人过分友好可能会引起另一个人的反感。然后就尽可能让两句话情感不一致，把上面公式的第二项的符号换一下就好了。loss function 符号为*LDMAX*。
**3.2.3 最大化情感内容**
这里是想要模型生成出的句子具有很明显的情感特征，以避免生成那种无聊的话，但并不需要指定情感是积极还是消极。变向地，这也是一种避免产生”呵呵“，”我不知道“的一种方法。loss function 如下：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJp0vO3zBDicwe2D7ukibGBfsLAibyCD5CZrYqo4uoEAn43t9S9licgmcPzQ/?)
**3.3 带有情绪多样的解码**
这个解码是基于 diverse beam search 的，diverse beam search 是在 beam search 的基础上，把 top B 序列分成 G 个 group，然后加一个额外的惩罚项，这个惩罚项的目的在于让 group A 中下一时刻的
 search 出来的东西尽可能和其他 group 下一时刻 search 出来的东西不一样，这就达到了 diverse 的目的。这里也从两个 level 来衡量，一个是单个词的情绪，一个是整个句的情绪，标准就用向量的 cosine 函数来衡量相似度。
**4. 实验部分**
文章用的是康奈尔电影对话数据集，评估没有采用 BLEU，ROUGE，METEOR 等作为指标，而是采用找人来评估，能够理解这种做法，因为 BLEU 等指标对于这种对话来说实际上意义不大，而且可能效果还不如别人的好。注意，在使用情感的 loss function
 方法上，先使用普通交叉熵目标函数训练 40 个 epoch，然后再使用特定的 loss function 训练 10 个 epoch。文中提到如果上来就使用特定的 loss function 训练，生成回答的语法会很差。
**5. 对模型的感想**
总体看来在 seq2seq 的三个必要的步骤里都做了情感的改进，不过方法看上去依然有点勉强，没有明显建模情感的交互，不过也算是对话系统里的一个情感的初步尝试。
**6. 参考文献**
[Ghosh et al. 2017] Ghosh, S.; Chollet, M.; Laksana, E.; Morency, L.-P.; and Scherer, S. 2017. Affect-LM: A neural
 language model for customizable affective text generation. In ACL, 634–642.
[Zhou et al. 2017] Zhou, H.; Huang, M.; Zhang, T.; Zhu, X.; and Liu, B. 2017. Emotional chatting machine: Emotional
 conversation generation with internal and external memory. arXiv preprint arXiv:1704.01074.


# 命名实体识别
■ 论文 | End-to-end Sequence Labeling via Bi-directional LSTM-CNNs-CRF
■ 链接 | http://www.paperweekly.site/papers/367
■ 作者 | robertdlut
近年来，基于神经网络的深度学习方法在自然语言处理领域已经取得了不少进展。作为 NLP 领域的基础任务—命名实体识别（Named Entity Recognition，NER）也不例外，神经网络结构在 NER 中也取得了不错的效果。其中，正如这篇论文，同一时期出现了很多类似的
 NN-CRF 结构来进行 NER，并成为目前主流 NER 模型，取得了不错的效果。在此进行一下总结，和大家一起分享学习。
**1. 引言**
命名实体识别（Named Entity Recognition，NER）就是从一段自然语言文本中找出相关实体，并标注出其位置以及类型，如下图。它是 NLP 领域中一些复杂任务（例如关系抽取，信息检索等）的基础。NER 一直是 NLP 领域中的研究热点，从早期基于词典和规则的方法，到传统机器学习的方法，到近年来基于深度学习的方法，NER
 研究进展的大概趋势大致如下图所示。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJ4NM1eOoFicGYiboFx2WY0Zhr8l6dicqVIa9FCadXZXyqJjVGUU3knAMRQ/?)
在基于机器学习的方法中，NER 被当作是序列标注问题。与分类问题相比，序列标注问题中当前的预测标签不仅与当前的输入特征相关，还与之前的预测标签相关，即预测标签序列之间是有强相互依赖关系的。例如，使用 BIO 进行 NER 时，正确的标签序列中标签 O 后面是不会接标签I的。
在传统机器学习中，条件随机场（Conditional Random Field，CRF）是 NER 目前的主流模型。它的目标函数不仅考虑输入的状态特征函数，而且还包含了标签转移特征函数。在训练时可以使用 SGD 学习模型参数。在已知模型时，给输入序列求预测输出序列即求使目标函数最大化的最优序列，是一个动态规划问题，可以使用维特比算法进行解码。不过
 CRF 依赖于特征工程，常用的特征如下：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJRl8KkWfg5aNV2e5O8ficuN8kYnic9EKHT95NC98fE7rxAia0iagxfF9UVw/?)
接下里我们重点看看如何使用神经网络结构来进行 NER。
**2. NER中主流的神经网络结构**
**2.1 NN/CNN-CRF模型**
其实，比这篇论文更早，《Natural language processing (almost) from scratch》就使用神经网络进行 NER。
在这篇论文中，作者提出了窗口方法与句子方法两种网络结构来进行 NER。这两种结构的主要区别就在于窗口方法仅使用当前预测词的上下文窗口进行输入，然后使用传统的 NN 结构；而句子方法是以整个句子作为当前预测词的输入，加入了句子中相对位置特征来区分句子中的每个词，然后使用了一层卷积神经网络
 CNN 结构。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJpGOQAJIuhibvsIXHnFHLnV8ZiaI6vjR7QdB1gtkiaSQTyXNgD5n9kN6Sg/?)
在训练阶段，作者也给出了两种目标函数：一种是词级别的对数似然，即使用 softmax 来预测标签概率，当成是传统一个分类问题；另一种是句子级别的对数似然，其实就是考虑到 CRF 模型在序列标注问题中的优势，将标签转移得分加入到了目标函数中。后来许多相关工作把这个思想称为结合了一层
 CRF 层，所以我这里称为 NN/CNN-CRF 模型。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJtdzcnzibnsd0FwzxqBevibJJDtfLjjtnforXmZnKpWlfibW7TJWzsrVlg/?)
在作者的实验中，上述提到的 NN 和 CNN 结构效果基本一致，但是句子级别似然函数即加入 CRF 层在 NER 的效果上有明显提高。
**2.2 RNN-CRF模型**
借鉴上面的 CRF 思路，在 2015 年左右出现了一系列使用 RNN 结构并结合 CRF 层进行 NER 的工作，其中就包括了这篇论文。代表工作主要有：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJObvvRlqqLqepvpVZiaFQODVQQUmzsq22iaXnm7ya1YSMPZKyyVOoaPdw/?)
将这些工作总结起来就是一个 RNN-CRF 模型，模型结构如下图：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJ5K3euH387a2EyDZmUZkBPEm8IicssfnpMmibAd60aGKDAnoVasb6uibZw/?)
它主要有 Embedding 层（主要有词向量，字符向量（在字符向量实现的具体方法上，有使用 CNN 比如这篇论文，还有使用 RNN，不过效果我感觉差不多）以及一些额外特征），双向 RNN 层，tanh 隐层以及最后的 CRF 层构成。它与之前 NN/CNN-CRF
 的主要区别就是他使用的是双向 RNN 代替了 NN/CNN。这里 RNN 常用 LSTM 或者 GRU。
实验结果表明 RNN-CRF 获得了更好的效果，已经达到或者超过了基于丰富特征的 CRF 模型，成为目前基于深度学习的 NER 方法中的最主流模型。在特征方面，该模型继承了深度学习方法的优势，无需特征工程，使用词向量以及字符向量就可以达到很好的效果，如果有高质量的词典特征，能够进一步获得提高。
**3. 近期的一些工作**
最近的一年在基于神经网络结构的 NER 研究上，主要集中在两个方面：一是使用流行的注意力机制来提高模型效果（Attention Mechanism），二是针对少量标注训练数据进行的一些研究。
**3.1 Attention-based**
《Attending to Characters in Neural Sequence Labeling Models》该论文还是在 RNN-CRF 模型结构基础上，重点改进了词向量与字符向量的拼接。使用 attention 机制将原始的字符向量和词向量拼接改进为了权重求和，使用两层传统神经网络隐层来学习
 attention 的权值，这样就使得模型可以动态地利用词向量和字符向量信息。实验结果表明比原始的拼接方法效果更好。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJgY0yq96v5FRYOPcC5CqkjyYGzE5miakEU1e5ib54EWFM6TmX1rAPgMAA/?)

另一篇论文《Phonologically aware neural model for named entity recognition
 in low resource transfer settings》，在原始 BiLSTM-CRF 模型上，加入了音韵特征，并在字符向量上使用 attention 机制来学习关注更有效的字符，主要改进如下图。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJwcAnEubKJyPb5bPcT6UvpIsOY7W8xV1RGblPia3IQJMEUSjPlJ0NIqA/?)
**3.2 少量标注数据**
对于深度学习方法，一般需要大量标注数据，但是在一些领域并没有海量的标注数据。所以在基于神经网络结构方法中如何使用少量标注数据进行 NER 也是最近研究的重点。其中包括了迁移学习《Transfer Learning for Sequence Tagging with
 Hierarchical Recurrent Networks》和半监督学习。
这里我提一下最近 ACL2017 刚录用的一篇论文《Semi-supervised sequence tagging with bidirectional language models》。该论文使用海量无标注语料库训练了一个双向神经网络语言模型，然后使用这个训练好的语言模型来获取当前要标注词的语言模型向量（LM
 embedding），然后将该向量作为特征加入到原始的双向 RNN-CRF 模型中。
实验结果表明，在少量标注数据上，加入这个语言模型向量能够大幅度提高 NER 效果，即使在大量的标注训练数据上，加入这个语言模型向量仍能提供原始 RNN-CRF 模型的效果。整体模型结构如下图：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJo5BwqJUXgpNjflj9ibSoyhmwAUgnqGkAxS4k2uBkeRXrpxv7BfgRfEA/?)
**4. 总结**
论文的实验部分我就不介绍了，最后进行一下总结，目前将神经网络与 CRF 模型相结合的 NN/CNN/RNN-CRF 模型成为了目前 NER 的主流模型。我认为对于 CNN 与 RNN，并没有谁占据绝对的优势，各自有相应的优点。由于 RNN 有天然的序列结构，所以
 RNN-CRF 使用更为广泛。
基于神经网络结构的 NER 方法，继承了深度学习方法的优点，无需大量人工特征。**只需词向量和字符向量就能达到主流水平，加入高质量的词典特征能够进一步提升效果****。**对于少量标注训练集问题，迁移学习，半监督学习应该是未来研究的重点。
**5. 参考文献**
[1] Lafferty J, McCallum A, Pereira F. Conditional random fields: Probabilistic models for segmenting and labeling sequence data. Proceedings of the eighteenth international conference
 on machine learning, ICML. 2001, 1: 282-289.
[2] Sutton C, McCallum A. An introduction to conditional random fields. Foundations and Trends® in Machine Learning, 2012, 4(4): 267-373.
[3] Collobert R, Weston J, Bottou L, et al. Natural language processing (almost) from scratch. Journal of Machine Learning Research, 2011, 12(Aug): 2493-2537.
[4] Lample G, Ballesteros M, Subramanian S, et al. Neural Architectures for Named Entity Recognition. Proceedings of NAACL-HLT. 2016: 260-270.
[5] Huang Z, Xu W, Yu K. Bidirectional LSTM-CRF models for sequence tagging. arXiv preprint arXiv:1508.01991, 2015.
[6] Ma X, Hovy E. End-to-end sequence labeling via bi-directional lstm-cnns-crf. arXiv preprint arXiv:1603.01354, 2016.
[7] Chiu J P C, Nichols E. Named entity recognition with bidirectional LSTM-CNNs. arXiv preprint arXiv:1511.08308, 2015.
[8] Rei M, Crichton G K O, Pyysalo S. Attending to Characters in Neural Sequence Labeling Models. arXiv preprint arXiv:1611.04361, 2016.
[9] Akash Bharadwaj, David Mortensen, Chris Dyer, Jaime G Carbonell. Phonologically aware neural model for named entity recognition in low resource transfer settings. EMNLP, pages 1462–1472,
 2016.
[10] Yang Z, Salakhutdinov R, Cohen W W. Transfer Learning for Sequence Tagging with Hierarchical Recurrent Networks. ICLR, 2017.
[11] Matthew E. Peters, Waleed Ammar, Chandra Bhagavatula, Russell Power. Semi-supervised sequence tagging with bidirectional language models. ACL, 2017.

# QA系统
■ 论文 | Gated End-to-End Memory Networks
■ 链接 | http://www.paperweekly.site/papers/1073
■ 作者 | lc222
本文是在 End-To-End Memory Networks 这篇论文的基础上做了一些修改。因为 End-To-End Memory Networks 在 multi-fact QA、 positional reasoning、 dialog 等领域的效果还不是很好，所以本文参考
 CV 领域中 HighWay Networks 和 Residual Networks 涉及到的 shortcut connections，引入 Gated 机制，以实现对 memory 的正则化，从而让模型可以动态的修改 memory。
因为 End-To-End Memory Networks 已经很熟悉了，所以我们先来介绍一下 Highway Networks 的想法。其主要是在网络输出下一层之前引入了一个 transform gate T 和一个 carry Gated C，以让网络学习什么、多少信息应该被传到下一层。我们假设本层网络的输出为：y=H(x)，那么就加入下面的映射函数：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJZXP7UXwP6DEfy0T9RbREmQrRp2UTMdzDOIeQCHvibKD7umvWYFwiaSicw/?)
往往我们会选择 C = 1-T，所以上面的公式可以转化为：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJycfaDBzjvtaKT5RAaKbXHMicepwfbWVdTOEOsF1gucGMqLrN4Ymc7sA/?)
而残差网络则可以视为是Highway网络的一种特例，因为其直接把 T 和 C 都当做 I，所以就相当于 y=H(x) + x。但是这里背后的原理我还没来得及搞明白，为什么这样就可以让更深的网络很容易就训练成功，等有时间再看看相关的论文学习下。
然后我们来看一下如何将其融入到 End-To-End Memory Networks 中，由于其每个 hop 的功能都可以视为 u'=H(u)，所以对应到上面的公式，u 就相当于输入 x，o 就相当于输出 y，所以代入上式得：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJfspicT5jCeMaZb7ibTevgMibGsP6wic0HuVmV9ibZS4QOdHwWBqmOSrcKSg/?)
也就是修改一下原来模型中输出层的公式即可。然后参数 W 和 b 有全局和每个 hop 独立两种方式，后面实验结果证明，每个 hop 保持独立效果会比较好。论文的创新点倒不是很大，只不过是将两篇论文结合一下，但是看有实验效果好像还有挺大的提升。最终的模型架构图如下所示：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJrcnWyzyc3HQnRw5RMibx7AlVQSiaegpMee1J0dUL05Aht4yGa2kbe6dA/?)
**实验结果**
本文所提出的模型不仅仅在 bAbI 数据集上取得了很好的效果，而且在 dialog bAbI 对话数据集上也取得了很好的效果。这个数据集应该会在后面的文章中进行介绍，这里就不赘述了。这里也贴上两张实验结果的图：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJG6WibqIkEkOARbU1LWwnRmUu7ObREXre74TNHKhS7bS4tMYDA87wtFw/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnwLopkg177jgoQCbq2j2UJGGZ7j8zssJHV3pNQGtESsrvrj4jibzgUbEZ9wPExG1tveHCVkSvMMkg/?)
第二张图揭示得是 MemNN 与本文提出模型各个 hop 对每个句子的权重计算，可以看出本文的模型更加集中在最重要的那个句子上面，而 MemNN 则比较分散，也说明了本文模型效果更好。
**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
我是彩蛋

**解锁新姿势：用微信刷论文！**

PaperWeekly小程序上线啦

**今日arXiv√猜你喜欢√热门资源****√**

随时紧跟最新最热论文
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)

**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)


