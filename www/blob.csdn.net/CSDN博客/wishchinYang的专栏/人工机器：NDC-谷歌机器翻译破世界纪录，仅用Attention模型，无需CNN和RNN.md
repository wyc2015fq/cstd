# 人工机器：NDC-谷歌机器翻译破世界纪录，仅用Attention模型，无需CNN和RNN - wishchinYang的专栏 - CSDN博客
2017年06月14日 20:07:15[wishchin](https://me.csdn.net/wishchin)阅读数：2774
终于找到ML日报的微信链接，抄之......................................
请拜访原文链接：[【谷歌机器翻译破世界纪录】仅用Attention模型，无需CNN和RNN](https://mp.weixin.qq.com/s?__biz=MzI3MTA0MTk1MA==&mid=2651999438&idx=1&sn=9ede16ab3de8ded3b603870ba988f8e0)。
[](https://mp.weixin.qq.com/s?__biz=MzI3MTA0MTk1MA==&mid=2651999438&idx=1&sn=9ede16ab3de8ded3b603870ba988f8e0)**评价**：
        NTM的成熟体DNC竟然达到了这种能力，不知道进化成完全体会是什么样子。竟然在机器翻译的准确率上超过了已经公布的所有模型，不愧是最接近现阶段最接近图灵机的有限图灵机。
        在数码宝贝中，我最喜欢的是阿和的加布兽进化的究极体数码宝贝——[钢铁加鲁鲁](http://baike.baidu.com/item/%E9%87%91%E5%B1%9E%E5%8A%A0%E9%B2%81%E9%B2%81%E5%85%BD/4991635?fromtitle=%E9%92%A2%E9%93%81%E5%8A%A0%E9%B2%81%E9%B2%81&fromid=7604605)，其使用的武器绝对冷冻气——就洋溢着极其欠揍的高冷味道。
        参考文章：[基于神经网络的混合计算](http://blog.csdn.net/wishchin/article/details/53008999)[-Hybrid
 computing using a neural network with dynamic external memory](http://blog.csdn.net/wishchin/article/details/53008999)。
        其中，关于NTM的译文：[人工机器-NTM-Neutral Turing Machine](http://blog.csdn.net/wishchin/article/details/53084685)。
![](https://img-blog.csdn.net/20170614200201604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170614200013814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**文章简介：**
大多数性能较好的神经序列转导模型都使用了编码器-解码器的结构。Transformer 也借鉴了这一点，并且在编码器-解码器上使用了全连接层。
编码器：由 6 个完全相同的层堆叠而成，每个层有 2 个子层。在每个子层后面会跟一个残差连接和层正则化（layer normalization）。第一部分由一个多头（multi-head）自注意力机制，第二部分则是一个位置敏感的全连接前馈网络。
解码器：解码器也由 6 个完全相同的层堆叠而成，不同的是这里每层有 3 个子层，第 3 个子层负责处理编码器输出的多头注意力机制。解码器的子层后面也跟了残差连接和层正则化。解码器的自注意力子层也做了相应修改。
2017-06-14 [新智元](https://mp.weixin.qq.com/s?__biz=MzI3MTA0MTk1MA==&mid=2651999438&idx=1&sn=9ede16ab3de8ded3b603870ba988f8e0#%23)
![](https://img-blog.csdn.net/20170614205655468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**新智元报道  **
来源：arXiv
编译：胡祥杰  文强
**【新智元导读】** 谷歌在机器翻译上的一项最新研究：仅使用注意力机制构建模型，不需要CNN和RNN。作者称，在机器翻译上这一模型效果超越了当下所有公开发表的机器翻译模型，在BLUE上取得创纪录的成绩。训练速度和效率上： 8 颗 P100 GPU 上3.5 天完成训练。该研究可以看成是对Facebook此前机器翻译突破的回应：不久前，Facebook发布了基于卷积神经网络的方法，取得了当时最高准确度，并且速度是谷歌基于循环神经网络(RNN)系统的9倍。
正文
谷歌大脑、谷歌研究院和多伦多大学学者合作的一项新研究称，使用一种完全基于注意力机制（Attention）的简单网络架构 Transformer 用于机器翻译，效果超越了当下所有公开发表的机器翻译模型，包括集成模型。值得一提的是，该研究没有使用任何循环或卷积神经网络，全部依赖注意力机制。正如文章的标题所说：“注意力机制是你需要的全部（Attention Is All You Need）。
![](https://img-blog.csdn.net/20170614205704265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
谷歌“新成员” Ian Goodfellow 在个人 twitter 上卖力转发这一论文。
回应 Facebook？巨头在机器翻译技术上的暗中较劲
上个月，Facebook AI 研究 （FAIR）团队发布了一项使用创新性的、基于卷积神经网络的方法来进行语言翻译的最新成果。Facebook 称，该研究取得了截止目前最高准确度，并且速度是基于循环神经网络(RNN)系统的9倍（谷歌的机器翻译系统使用的就是这一技术）。
现在，谷歌的这一篇论文说的是，我们不仅不用CNN，我们连RNN也不用了，就一个Attention 就能搞定机器翻译，而且效果最好，性价比最高。
巧合的是，与Facebook发布研究成果时一样，谷歌在本次研究成果的发布上，不仅强调BLUE测评分数上拿到最好，实现业界最佳，并且还特别提到了训练时间和GPU消耗：“模型训练是在 8 颗 P100 GPU 上3.5 天完成。”
注意力机制是你需要的全部
一直以来，在序列建模和序列转导问题中，比如涉及语言建模和机器翻译的任务，循环神经网络（RNN），尤其是长短时记忆（LSTM）及门循环网络，都被视为最先进的方法。研究人员也想方设法拓展循环语言建模和编码器-解码器架构。
其中，注意力机制自提出以来便成为序列建模和转导模型不可或缺的一部分，因为注意力机制能够在不关注输入输出序列之间距离的情况下，对依存（dependence）建模。只有在极少数的案例中，作者将注意力机制与一个循环网络作为整个网络中相等的结构，并行放置。
在谷歌大脑最新公开的一项研究中，研究人员提出了一个全新的架构 Transformer，完全依赖注意力机制从输入和输出中提取全局依赖，不使用任何循环网络。
谷歌大脑的研究人员表示，Transformer 能够显著提高并行效率，仅在 8 颗 P100 GPU 上训练 12 小时就能达到当前最高性能。
论文作者以 Extended Neural GPU、ByteNet 和 ConvS2S 为例，这些结构都使用卷积神经网络（CNN）作为基本的模块，并行计算所有输入和输出位置的隐藏表征，从而减少序列计算的计算量。在这些模型中，将来自两个任意输入或输出位置的信号相关联的运算次数会根据位置之间的距离增加而增加，对于 ConvS2S 这种增加是线性的，而对于 ByteNet 则是呈对数增长的。
这让学习距离较远的位置之间的依赖难度增大。在 Transformer 当中，学习位置之间的依赖被减少了，所需的运算次数数量是固定的。
这需要使用自注意力（Self-attention），或内部注意力（intra-attention），这是一种与单个序列中不同位置有关的注意力机制，可以计算出序列的表征。
以往研究表明，自注意力已被成功用于阅读理解、抽象概括等多种任务。
不过，谷歌大脑的研究人员表示，据他们所知，Transformer 是第一个完全依赖自注意力的转导模型，不使用 RNN 或 CNN 计算输入和输出的表征。
![](https://img-blog.csdn.net/20170614205712525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
摘要
当前主流的序列转导（transduction）模型都是基于编码器-解码器配置中复杂的循环或卷积神经网络。性能最好的模型还通过注意力机制将编码器和解码器连接起来。我们提出了一种简单的网络架构——Transformer，完全基于注意力机制，没有使用任何循环或卷积神经网络。两项机器翻译任务实验表明，这些模型质量更好、可并行化程度更高，并且能大大减少训练时间。
我们的模型在 WMT 2014 英德翻译任务上实现了 28.4 的 BLEU 得分，在现有最佳成绩上取得了提高，其中使用集成方法，超过了现有最佳成绩 2 个 BLEU 得分。在 WMT 2014 英法翻译任务中，我们的模型在单一模型 BLEU 得分上创下了当前最高分 41.0，而训练时间是在 8 颗 GPU 上训练 3.5 天，相比现有文献中的最佳模型，只是其训练成本的很小一部分。我们还显示了 Transformer 泛化性能很好，能够成功应用于其他任务，比如在拥有大规模和有限训练数据的情况下，解析英语成分句法解析（English
 constituency parsing）。
模型架构
大多数性能较好的神经序列转导模型都使用了编码器-解码器的结构。Transformer 也借鉴了这一点，并且在编码器-解码器上使用了全连接层。
编码器：由 6 个完全相同的层堆叠而成，每个层有 2 个子层。在每个子层后面会跟一个残差连接和层正则化（layer normalization）。第一部分由一个多头（multi-head）自注意力机制，第二部分则是一个位置敏感的全连接前馈网络。
解码器：解码器也由 6 个完全相同的层堆叠而成，不同的是这里每层有 3 个子层，第 3 个子层负责处理编码器输出的多头注意力机制。解码器的子层后面也跟了残差连接和层正则化。解码器的自注意力子层也做了相应修改。
![](https://img-blog.csdn.net/20170614205719821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（图1）Transformer 的架构示意图
![](https://img-blog.csdn.net/20170614205732775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（图2）具体采用的注意力机制。左边是 Scaled Dot-Roduct Attention，右边是多头注意力（Multi-Head Attention），由几个并行的层组成。
在编码器-解码器层当中，query 来自上一个解码层，编码器输出值（value）及 memory key。这样，解码器中所有位置都能照顾到输入序列中的所有位置。
编码器含有自注意力层。在自注意力层中，所有的 key、value 和 query 都来自同一个地方，那就是编码器的上一层输出。编码器中的每一个位置都能照顾到编码器上一层中所有的位置。
同样，解码器中的自注意力层让解码器中所有位置都能被注意到，包括那个位置本身。
创造了BLEU的最高分28.4
在WMT2014 英语到德语的翻译任务中，我们的大型transformer在性能上优于此前在有记录的所有模型（包括集成的模型），并且创造了BLEU的最高分28.4。
模型的配置详情见表3下的清单。训练过程为3.5天，在8颗P100 GPU上运行。即便是我们最基础的模型，也超越了此前所有发布的和集成的模型，但是训练的成本却只是此前最好的一批模型中的一小部分。
![](https://img-blog.csdn.net/20170614205739540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
表2：Transformer 在英语到德语和英语到法语新闻测试2014数据集上，比此前最好的模型获得的BLEU分数都要高。 
表2 总结了我们的结果，并与其他模型在翻译质量和训练成本上进行对比。我们评估了被用于训练模型的浮点操作数量，用来乘以训练时间，使用的GPU的数量，并评估了每一颗GPU中，可持续的单精度浮点承载量。
![](https://img-blog.csdn.net/20170614205746072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
表4 Transformer 在英语成分句法解析任务上也取得了较好的效果。（基于WSJ  Section 23 数据库）
为了测试Transformer能否用于完成其他任务，我们做了一个在英语成分句法解析上的实验。这一任务的难度在于：输出受到结构限制的强烈支配，并且比输入要长得多得多。另外，RNN序列到序列的模型还没能在小数据领域获得最好的结果。
会尽快开源论文代码，供人训练和评估模型
谷歌大脑研究人员提出了首个完全基于注意力的序列转导模型 Transformers，用自注意力取代了编码器-解码器架构中最常用的循环层。
对于翻译任务，Transformer 的训练时间比基于循环或卷积层的架构明显更快。在 WMT 2014 英德翻译和 WMT 2014 英法翻译任务中，Transformer 实现了当前最好的结果。
论文作者还通过实验证明了 Transformer 具有良好的泛化性能，能够用于其他任务。
作者在论文的结论中写道，他们十分关注基于注意力模型的架构，并计划将其应用于其他任务，比如文本之外的输入和输出模式问题，以及探索如何将本地及受限制注意机制用于有效处理图像、音频和视频等拥有大量输入和输出的问题。
最后，作者表示将尽快开源论文代码，供人训练和评估模型。
