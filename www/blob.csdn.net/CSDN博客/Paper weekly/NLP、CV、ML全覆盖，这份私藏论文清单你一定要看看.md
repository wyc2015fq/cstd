# NLP、CV、ML全覆盖，这份私藏论文清单你一定要看看 - Paper weekly - CSDN博客





2019年01月25日 08:52:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：595









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhglryG74dIr2B1019Yibv9PAGsWGGYBiaoSGbK2kzUnbIsicCEiazKMticicR0MPtmr1ynDovFe2kGicSydcg/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **128** 篇文章

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?)


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrXJtcG3jtqoUMGhW0vGNADhCq4GlYUcvmmbrVIibKk4tlTtiaFHJTCoibg/640?wx_fmt=png)

**@xwzhong 推荐**

#Sequence Modeling

本文来自斯坦福大学和 Google Brain，**论文主要的想法是深化句子层面的语义表征**，跟近期的 BERT 有相通之处，但使用的数据量相对于 BERT 少很多。针对有监督任务中数据量少的情况，作者提出使用半监督的方式强化句子层面的表示。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrLlGTiahU9edRKE4Zmf7LyvQsWqq1xJAx0DVOZbbAeXaj5GS9z3PQS5A/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrMOgWOFCAWZLD7IibuZPH1K4UAz91lmqfpuT85twHKzcagzl5Iw7ahvg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrtCcB14Y9MLPNckTqvZMCJoeyLZ1w7AOxKnIM8xXFkUvG6fiaOKEvK8g/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrpBE2V3g5TmdvlibgrH6ib7pyN08na6PJYBwVMnNwI0tTX2B2omTiarTAw/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrFPzr8NkAD4rmG4icKHH7rR2icEmn4SFEGAo2QNRJ27VMZ6uQLNxaKCIQ/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2749




源码链接

https://github.com/tensorflow/models/tree/master/research/cvt_text




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/640?)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrPcDEfaMJkwcCKJR0SbOutcfyIBDIeJX463YmChXEZ0ZibKxIPDDpvZA/640?wx_fmt=png)

**@paperweekly 推荐**

#Sentiment Classification

本文是香港中文大学、阿里巴巴和腾讯发表于 AAAI 2019 的工作。传统的基于评论目标的情感分析（Target/Aspect-Based Sentiment Analysis, TBSA）通常涉及到**评论目标抽取**和**目标情感分类**两个子问题，它们作为独立的任务在近几年受到了研究者的广泛关注。然而，孤立地研究这两个子任务与完整的基于评论目标的情感分析任务是有一定冲突的（例如：目标情感分类任务假设评论目标是给定的但是在 TBSA 任务中评论目标是未知的）。 

因此，**本文提出了一种端到端的方案同时解决评论目标抽取和目标情感预测问题。**作者将完整的 TBSA 问题定义为一个信息抽取的任务并仔细设计了标签集合，使得评论目标信息和情感信息能够包含在一个标签序列中。

除此之外，**作者还提出了一个新的堆叠式 RNN 来解决 TBSA 问题。**这个模型包含了两层堆叠的 RNN，上层的 RNN 用于解决完整的 TBSA 问题（预测 TBSA 的标签序列），下层的 RNN 负责检测评论目标的边界。

为了充分利用评论目标的边界信息，本文提出了一个组件显式地把下层 RNN 提取的边界信息加入到 TBSA 的序列决策中。在序列预测过程中，作者也尝试加强当前词和过去一个时刻的词的关系，来缓解同一个评论目标短语内情感标签不一致的问题。

作者在 3 个标准数据集上进行了实验，结果表明本文模型一致优于基准模型和现有的序列标注模型。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrXsicjVic6re0xD93A2asK9JWuVQiaNxX1XbaZaTpFicOYwIlGiahMVOEDJQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr2lDJibQwiadb9PgzplOlB6bOq1ZxPs6j4lXf0HR1LUuMrBr6BvFK0KCA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrVUu750nwMDENzkbJ9z5dUvrd1sDKgm1NcKQRCh9hdTiabUVGpW98ZqQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrb8t59DFEBXibQblSibKq3obPibGlIoFSm7CjIE4VdUcSJHIXMsibPvI05A/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrMV2HaEdKicnTXicDoeWwBs3icuUYtX2KWOGibKyfHjzftnjpHLjaWRtScg/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2718




源码链接

https://github.com/lixin4ever/E2E-TBSA




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/640?)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrd4tib8JkP39K7icF7kc5JS4xtt1ib4N1DhyV1YdpN6GibyEzPWlO2gbGPQ/640?wx_fmt=png)

**@jingyihiter 推荐**

#Language Model Pretraining

本文是来自 Facebook AI Research 的最新工作，**论文针对的是跨语言的语言模型预训练任务**。本文方法在无监督机器翻译任务上提升 9 个 BLEU 值，在有监督机器翻译上也得到超过 4 个 BLEU 值的提升。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr1BAysfDL8QNOtaZw9zUMV779bLkLU0a9Ol7M3MQ1fUm5pV6n2fUKBg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrUC5Tfibd4Wdu0oFdOoHm5WunbgBNTNzJDNaLHzicbXSg8wmJD1dSdw7g/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr31sUwglP2JpRJTyPXcaJSuWudIQaOVDlzzxYBFyibfu4h30Ygc4EEJA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr8nMtlntQcNBXwI7Z7Kw1lhbM5IooPACmjT2CIELj0hHuuL9PeVmtpA/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrMcceEbgZagWZh53dVzw6WuvIY5iamuibgP9vMwQQaWnPRt3qRq30CS0A/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2790







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/640?)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrxEssrJM7jX8CEA87nIdx0oibnt5HGPhCYQWBYVlfYyojIfzd8ic1bu4g/640?wx_fmt=png)

****@aforever** 推荐**

#Word Embeddings

本文是华盛顿大学和 FAIR 合作的论文，**通过最大化词对与其上下文的点间互信息（pointwise mutual information）来预训练词对表示（word pair representations）。**这鼓励模型学习更有意义的词对表示，而不是使用语言建模等更通用的目标。在 SQuAD 和 MultiNLI 等要求跨句推理的任务中，预训练表示是有效的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrPgicU7I7OIU1qnTmibIzj5qSpKtGmL9aOX7sBHtel3lTLHnCCbicuar9g/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrJRBwkK3e1TjTZUtxnkAExjIdoevjmECvkYg7nnX95S6micqJ54J9dQA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrnH8uE7BAFM0nsLqYQcWKHaB0EY4dRYibNDOPN4ibTCI8hgMCGroDo6hw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrfZ6ic8WhaeuedJ7fgpG9k8cXMmD7aWaicfcfEqolnV5VKEXJiaia9QWFIg/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrG1xpzmeibFRtVic7Ntia8wB8K2vt0wguOG0icR3nkItzDTEa5iaTic7FgBHQ/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2745







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/640?)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrtf4DLfyQOKEozm4jiaQ2K5pZ9t1B7hFqHurHCKhXKXaGb0FicZlCZwWQ/640?wx_fmt=png)

**@paperweekly 推荐**

#Text Summarization

本文是北京大学、腾讯、京东和阿里巴巴发表于 AAAI 2019 的工作。在基于神经网络的生成式文本摘要研究领域中，传统的基于序列到序列的模型对文档主题的建模经常出错。为了解决这个问题，**本文提出了读者感知的摘要生成任务**，它利用读者的评论来帮助生成符合文档主题的更好的摘要。

与传统的生成式摘要任务不同，**读者感知型摘要面临两个主要挑战：**1）评论是非正式的、有噪音的；2）联合建模新闻文档和读者评论具有一定挑战性。

为了解决上述挑战，**作者设计了一个基于对抗学习的读者感知型摘要生成模型（RASG）**，它由四个部分组成：1）基于序列到序列的摘要生成模块；2）读者注意力建模模块来捕捉读者关注的主题；3）督导建模模块来描述生成的摘要和读者关注主题之间的语义差距；4）目标跟踪模块来产生每个生成步骤的目标。

督导和目标定位模块以对抗学习的方式来指导框架的训练。作者在自己的文摘数据集进行了大量实验，结果表明 RASG 在自动评测和人工评测方面都取得了最好的效果。实验结果还证明了本文框架中每个模块的有效性。此外，作者还发布了一个大规模数据集供进一步研究。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrX9Rqdz2uaTialibUiafq30Fe6kvUSzK2Id3QKkB9EUKIdX8bgPTM4Mg4Q/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XricCp7sOXgL9iawSnicD85tavf2Dl9AuJ2WL1XSZMwFruZzAuHGdLd2E6Q/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrrtXXDRhl7JWmestyjhnWD6kJNgVZr7EyJjNVPl7h8NBEAo1QRNuteQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr89xmwSTk40h3xNicUkHD2icLdVPP2NG747icPer7H7UVfefjlHR2rJ67g/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrEKJLm7oJSQiaNZAnFH5rLf9ueib2LLaLppoiaAkH0jHjPyd65szZ8uXuA/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2716




数据集链接

http://t.cn/EAH5JxS







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0Drvm1kKqodONJWdluKYXVSiaVksJv8JyrGzSsG6O8Nt5p6aYxkA7aFuLiaQ/640)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrAibc1n9Mqs5mI9Y4IiaicKUcibI8lfMn2awTkBWPY4uibiaXudibRf89FkiaNQ/640?wx_fmt=png)

**@priver 推荐**

#Named Entity Recognition

本文是苏州大学发表于 COLING 2018 的工作，**作者尝试解决了远程监督存在的两类问题：**1）错标；2）漏标。对于错标的情况，作者使用了强化学习的机制实现了一个 instance selector。对于漏标的情况，作者考虑了所有可能的 label 序列，再对他们进行打分，从而给出最有可能的序列结果，而不是武断地就标记为 O。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrsaswrnXrGb7d60q6RQCNptk5HMsUAmNA710vibdOM1sibmgjNm3ljV5A/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrz1ict9LLSdNbVy2gpcsPDPbtev8x77ymfocykZFnUTYYEk7a0FxAicYg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrTn3bSClSgb4XBsd3TcS5ptoU3nIjuAKwic2bUvcNO8oUbWibuEZ6rKtA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr5G9wLslJfwGFObD0ib3VkUeyw7a381CZux4ukicIdkoOG8XcA1QyI4wQ/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrW4PwFlRkkqRKs1iarWngw0F6KS3KzGc1iag0p0ZeKmFs4picxzpDNNX1g/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2769




源码链接

https://github.com/rainarch/DSNER







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvZkYxV68zOCas9csIEy9oS6Oop2huyXBUliaHFUVHicdamRgqibegicc0aA/640)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrDnu0jAzKdiaO1lrSKc1K0dicGphbXY0ibWk7POrjWP2n4y2P2ACNb9CQw/640?wx_fmt=png)

**@figo 推荐**

#Super-Resolution

本文出自小米原班人马（上一篇是 ***MoreMNAS***：https://www.paperweekly.site/papers/2740），属于目前很火的 AutoML Neural Architecture Search。

**论文基于弹性搜索（宏观+微观）在超分辨率问题上取得了令人震惊的结果。**在相当的 FLOPS 下生成了多个模型，结果完胜 ECCV 2018 明星模型 CARNM（乘加数参数数量少，PNSR/SSIM 指标高，文中称 dominate），这应该是**截止至 2018 年可比 FLOPS 约束下的 SOTA**（涵盖 ICCV 2017 和 CVPR 2018）。而达到这样的效果，论文基于一台 V100 用了不到 3 天时间。

此外，论文还给出了几个前向模型。要知道上一篇论文中他们初步结果是击败 CVPR 2016，才半个月时间就提升这么大，这也进一步验证了 AutoML NAS 技术的强大和可怕。需要指出的是，该项技术具有一定的普适性，理论上可以应用于任何监督学习，值得关注和学习。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrxxIMohibW7PXW1xhxa4wiahgw7zLOntuvjkFMBNzib2BPaZqWuHxcJb8w/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrsxnqic0ZLmoQFrXuARLShUwAa5cA8fKmgMxhicUIz03H0m5y8VzxPJeA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrahJ2VGMUEJn6laeTCRSIZYApV5uT2YAibOoqhbzHOLskhAPHPOCkrTA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrENuOIJV6iakWkKFUMdtoZJDkVeIGPTXsKib32hY7AxDqEreK0hsR8zow/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrbx86ibWROw5bibaGdaCI9jPkwhPLO6uP70u89OXDz9Riaiady1JgMYe5kg/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2786




源码链接

https://github.com/falsr/FALSR







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvibxtiaicW0ZRIwW0Kmkj9yU90UmGicL2jnnmaBY47NYicK2d7frJAcNP09w/640)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrDEDXr0hlv3icMJAcNfDwOn4hviaw9jbsPDq9ubgf8X4qTqNPkvzMiaopQ/640?wx_fmt=png)

**@paperweekly 推荐**

#Person Re-identification

本文来自中科院自动化所和地平线，**论文验证了部件对齐在跨域重识别中起着重要的作用。**通过强化模型的对齐，作者通过提高模型的泛化性能来提升模型直接跨库测试的性能；另一方面，本文提出的部件对齐模型可以很自然地利用目标域无标签数据实现领域自适应，使得模型可以适配目标域。

作者在 Market1501、CUHK03、DukeMTMC-reID 和 MSMT17 四个目前最大的行人重识别数据库上证明了本文方法的有效性，并且取得了 SOTA 结果。同时，出于完整性，论文还通过实验证明了和现有跨域方法的互补性。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr91OS3EFic9yTme8arl3xaEPR40b4cibP5TNMfPxibjWvMiafrbDVTUsY4Q/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrYan1Tr52cX2vx7MCz298P9achibg2icwpcane5icB7QQ0qmkhsdibWgKow/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrZIzdE07v91XlKuOtKeFNic3ZMbnWzdNuVG6o3LAJfz80Kwkc1iaCgHmg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrTTxRZibiaEVKLFzVhwOIrxiaRewjhmPvy24ME1qKZ5ZnDvng6rk6Msk2Q/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrboDdJhva990dicibKuUDWrl7UAibWQhs3YK3DJ6b1JAmsgOF1HxEhCgzw/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2736




源码链接

https://github.com/huanghoujing/EANet







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvHib5D8hcewE9gwNibrGkW1TC8v83Y89RITicqLb5N3URaM1wGsGBV27qQ/640)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrYSRiankg1cr5YKu65hdBHcZWTBtETO1y4GWfotEARJoQNNb7Tp4WPug/640?wx_fmt=png)

**@paperweekly 推荐**

#Object Detection

本文是密歇根大学发表于 ECCV 2018 的工作。当前的目标检测算法大多基于 Anchor，引入 Anchor 容易导致正负样本不均衡和引入更多超参数。**本文在不采用 Anchor 的前提下取得了不错效果**，是一篇非常有意思的探索工作。

具体来说，**论文借鉴了人体关键点检测的思路来做目标检测，通过检测目标框的左上角和右下角两个关键点就能得到预测框。**其次，整个检测网络训练是从头开始的，且不基于预训练的分类模型，这使得用户能够自由设计特征提取网络，不用受预训练模型的限制。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr94u4jRJvIODTl2RUHRLoV2Au99icu2fjj9VgL1V8W1neGA1q3S15KGA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr9lctbREzvRT3VFx4U7EOtGvkKwt9Lq7OT9uoZGIvQaHQbLuFOF0G9Q/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrapWXvQX0uyAYqJ5LgialcylGQK17D6GibA1ibrVXre5K4jYb2eiafkyF9w/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XreAMRmYFIiaqRuDGmTRW1xelQUCFAb2vHDQd1kINdpxEtDVjT6On9wQA/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XruEscjicic3sYKicCBuFlwbS28a5NDJMMJ2EA5e6bEgZeffN8t5O8MDFBA/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2771




源码链接

https://github.com/princeton-vl/CornerNet







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8tiamiceTcrbl3UY25cTHibSgtJNZnMBCOUdcpTpSLK45Ya9RC8yDZsSEw/640?)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrd3a8jvsn0zzRx0kfGvPyh8Y8WmfquYvjwbCtJh0YK9mj3G50Vv2hBg/640?wx_fmt=png)

**@WHUSteven 推荐**

#3D Reconstruction

本文是普林斯顿、Google 和 MIT 发表于 CVPR 2018 的工作，**论文使用无监督训练的方法基于 3DMM 进行人脸三维重建。**作者主要使用了基于编码器和解码器模型，创新性地将人脸识别网络引入训练的损失函数，使得生成的 3D 人脸能很好地保留了输入图片的人脸个体特征。

该模型旨在拟合形状和纹理，并没有学习姿态表情和光照。算法的编码器接受图像作为输入，输出用于 3DMM 模型的参数。解码器接受参数后合成 3D 人脸。

为了使网络不仅能保持个体信息，还能生成自然真实的人脸，**作者提出了 3 个新的损失函数**，即批分布损失（batch distribution loss）、回环损失（loopback loss）和多视角身份损失（multi-view identity loss）。 批分布损失可使每个批的统计量与 3DMM 的统计量一致。回环损失可保证生成的 3D 人脸模型的 2D 成像图片重新进入编码器得到的参数和原图的参数尽量一致。多视角身份损失能使得模型学习到独立于观察角度的个体特征。 

实验结果说明，模型不仅仅可以生成与输入图像高度相似的 3D 人脸，而且生成的人脸独立于输入的表情和姿态，甚至被遮挡的人脸也可以达到不错的生成效果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrFWFicP72h4TlAJgZWxnJ5gexS9ayB0EVric5Q36jSsxmCmAiclo66zjTA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr4W5XibIUHWbVkprzL4XpwqAtBZOPGIL2MOkFs91PUCDaR4FLDzfDw8w/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrJJkEYSYDiahrq9r74VRF5DgXhay1icZLyNH9QyVdIukdPVc6ZSkjYc6A/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrkUQUmwBaKrTcg3X5iaWjYcibibRibYG7NP67dIxaaqsNYicvOc1Vkh9PxDQ/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrX2Ww0YwXQL5L8T13hakoRLNxH0wz4ptzORzk9cKlxmyDDBIQGI7oFQ/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2747




源码链接

https://github.com/google/tf_mesh_renderer










![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8IA3BMnKpHmwoB8kAc8CQC4UOSu2G0c5vFM7xpJZOcqLdFHch97tiaGg/640?)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xryx4JejvdP6VeCattXh8utBiaaZbeLak5twk7pibMZPEqTkVsJiaLQ372w/640?wx_fmt=png)

**@whyguu 推荐**

#Active Learning

**本文提出了两种训练数据集抽取子集的方法 Facility-Location & Disparity-Min。**通过抽取的子集来初步表示整个样本集可以加速网络超参数的优化时间。通过从大量无标签样本中抽取数据减少标注成本。此外，文章还提供了一种贪婪搜索算法来减少搜索的时间。做视频或超大样本学习的同学可以关注一波，看是否有用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrYdtu6vdeevPoFWUF8NrwWbz9YI4PZBSaYelghDSu7B9ejf4Wia3XwpQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr7pdt3Ff1GKTBxZ8TGrWN8iaf2wGqENfBXZOXq7YUdvteHTzl5M3tL9A/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XricibicmnG8JnfL0MfYrOCgCtCMh86mDl8Jo2D6vZlJRdDCQfLvaDYFXBg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XreibKcqicRIER9AysMYpcB1qZyH3Eg5NlC8bQ1VrQgx30s12rW0ibv2MAg/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr5y79uQkIZT6icic1xTe13O4UmkM4RFFSyrjpn7PBC9Ttc0plhk4wciaPg/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2742







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvSrUEOribtWtcbc5Bs8icSOWQPFxgpHLCrooqDs1LNC02qthicqiaUiaLzeg/640)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrBCvjq5EDGEPzTd2Piaz1IBPLEbUPIcQnjrbDjq0icnCmfe4kFxxprvBg/640?wx_fmt=png)

** @zhangjun 推荐**

#Bayesian Deep Learning

**本文的亮点在于用 Bayes By Backprop 算法对 bayesian CNN 的每个参数分布进行了学习**，不同于 Gal 15 年提出的 MC Dropout 的方法，是一种基于贝叶斯后验推断的方法。

在此之前，Bayesian MLP 和 RNN 都已有了相应的变分近似求解方法，本文针对 CNN 的卷积层进行了相应的改进，取得了不错的效果。相比于 Gal 的 MC Dropout 优势不明显，而 MC Dropout 的计算效率则快非常多。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrhIAHt2OAz96dEzSTHvHxcia3NmcpichJXnhuKHn1VicQBsKyz0JbXYI1g/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrVL1eibvZxia1ZxUZTpXicQevmAAyGjgUM8YeoCmSCx4OoWAC2JofnGmqg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrQ0dFhLvHwibCtiatkQsgFJwYmGbp3icZLiczw4MneEPBrndEkRQ6Yw742A/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrhfHamofoTkN1tjtBQ3YbO4iavibFpUvKQYicjmBjpLludpX38KZhSI7fA/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr08QqwibBQLRl0Og5mazibEmtXtGn2JTzCcia40Tk2MdU1lJCzbujKsrHA/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2731




源码链接

https://github.com/felix-laumann/Bayesian_CNN







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUMyCvJ7nksObSLatO1UHuTLcw5KYWPhREehBpXWY0uqseRtib5rxuvBw/640)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrM4fcLROVP9LTB1ZzUbVJHxSjBJoPk9Sf0V0PMxYXibBuZ1aEohpfEEQ/640?wx_fmt=png)

**@QAQ 推荐**

#Neural Networks

本文是 Uber AI Labs 发表于 ICLR 2019 的工作，**论文提出了一种受生物学启发的训练网络自我修改权重的方法****，并且提出了一种可靠的可塑性框架**，该框架已经在各种监督学习和强化学习任务上提高了性能，可通过梯度下降进行训练，为大规模自优化神经网络开辟了新的研究方向。实现了学习如何学习（Learning to learn）的网络的基础。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrnmEb7kd51FiccC8yicdBVjTwVfTa6ZIJ3gKW6GoVop9CDdF3icGXjyYZg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrSquBhO0Y1EH8Oic36yfRZHZJBRIY0iarfDnicrMI3qwF42h2YbKdTElmA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrscDIorP6wvDvkxqibmXHlGW3DhKtlmUOYGsaxIhoT0U59UhAy4mXKAA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrTGaPic311mfphC7y9gvniajFufOf97yPRteqvWQJldsX66fKebbMibBEA/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrMTa9oJSYewbK27EusUibzoLkryc5CClVPBpoq7vtwjS76JCL9ibeeB1g/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2774







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjiaA5bbtkAnnJ2yLicAzlwmVdnAIic0THYptctQhZJRx7QYCx8TC9zwOow/640)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr6MjdIJWhbsv7RdbGKdXKkXWEOX5LNdUvKWsbs5mPFGuUhQcwMkzWqg/640?wx_fmt=png)

**@paperweekly 推荐**

#Recommender Systems

本文来自 CMU。**针对协同过滤中的冷启动问题，作者提出了一种基于 DQN 的问卷调查法来学习如何在电影推荐系统中处理用户冷启动。**DQN 生成问卷问题，由冷启动用户来回答。根据用户对前一个问题的响应，DQN 将动态生成后续问题。一旦问卷调查完成，问答信息将被传递到多层感知器中（MLP），并生成一个预测的 user embedding。一旦有了 user embedding 和电影 embedding，系统就可以对电影评级进行建模。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrV0FZoPHxIgySuvCBxPzKE6MIdYkj9JJponictoqS9e4opwHNzmHaafw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xre4dZGB1oiaaT03s8bEEEIUrp1l9afQfjkT10qyF6QlJ61nxJfPdAaicw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrx2XVvUHSUZpich0NIxDC3pKicfnFmWvNkdLF95icKcwARbd43zCsopd6g/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrCpNibHH22IC058c8MvP58UXp4ymQXXwwq0LNjfAjq7y0cpmOucCVKTw/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrl9iaDDic28GbjbImT5XmU5l6HqeGaqIBLwia24nJg8Ov95BobMGzFXib9Q/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2691










![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjgzGNHdv2YBVm6bUicHjY2A8tV8hwJIiapvTYDGmFIyMclC4Xy6gD7krQ/640)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrL5PMkslooPzKYxUJDY5hUswjiaoZnllOflfWupAvtibY2hBqYfxILYeQ/640?wx_fmt=png)

****@wwwangzhch** 推荐**

#Memory Networks

本文是北京大学、IBM 与佐治亚理工发表在 AAAI 2019 上的论文，**作者将电子病历数据（Electronic Health Records, EHR）与药物相互反应数据（Drug-Drug Interaction, DDI）通过图卷积网络转换成 Memory Bank**，并结合病人的历史病历记录生成 Dynamic Memory，动态调整损失函数以使得系统在较高精确度与较低 DDI rate 之间权衡，在 MIMIC-III 数据集上使用雅卡尔相似系数、Average Precision、Average Recall 等多种评价指标均取得了最高的推荐准确率与极低的 DDI rate（仅次于逻辑回归）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrdpicjAPfJ94D7v7uAcicRUiakP86m4LhzialcRALA6mench3Cs2IHrdGHQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrkpAfpLTakicwm5ibNfW3sia9tDHDZXXib5YdgGcm5SB6ma9ibOUKWZicicBqA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrBgTBO8qTvVLTGUg8B6tAmnnAuobc0N2mOicCQQsPwNwicFnUtlIWf4xQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr0Mtw1Um0RfMjdtYlPia9lT1HiaEkEmmxAv6IlCjNQyictKmoVuEYJSaRQ/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrDianricpuIpBbOET3PJWesxeDhgChzVB7YvgG96EhSiaS5ys8IhuiaM1Ag/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2651




源码链接

https://github.com/sjy1203/GAMENet







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVgicbLfFfFUq5D0tPfYTibSxD2ZvWJFMCnVZJ3UvpE3V635rO33PkQkIg/640)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrvVxcGLjIxrqMs57m15DqmuKA6p7FSaLC6DoqWu0vCUoMCrEbhgo8iaQ/640?wx_fmt=png)

****@zkt18** 推荐**

#Neural Networks

本文是 UIUC、Snap 和字节跳动发表于 ICLR 2019 的工作，**论文提出了一个可瘦身的神经网络**，针对不同硬件和运行环境，可以快速提供与之相适应的 CNN 模型来完成任务，支持在移动设备上进行训练。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrTibOouxEoEj7uf4uZbuSOveiaYxmo5fibTGMdFFM1rvcTkv7X01aJy4yA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrCGBPwA6rKSwG1hD1VV771qicCFjyJjzU2CLVgRX64bsT5p3jf70UbDw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrheD3TQYLxPr00O2bUQ0Wm5UIRMYMhcawCXt1caPustl1tPXClaCCzw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrmnkF47fk8JSaklJia2pkZNiaJpcsuAKynLwIT3onYiaFhRdY7VkPvntvg/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xrj89DffFx0SvKAmatcvWKd4wdBVYXeC0xjdcrobscja0BhYG0tiaN1uw/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2689




源码链接

https://github.com/JiahuiYu/slimmable_networks







![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnntmjtq9fdPspS4WeiaVQSy8gx4ZNX5Tiaeicno09TPNjSVd4zMnx9hxYcw2n350iaTFOOe6u2F7GLoA/640?wx_fmt=png)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrAia9Mv8eqJjhJmRd2JnpJXBjn9AYYXxN1MOiaqta1sM9mUicLc3kXyYmQ/640?wx_fmt=png)

****@ZSCDumin** 推荐**

#Group Recommendation

本文是湖南大学和新加坡国立大学发表于 SIGIR 2018 的工作，**论文应用神经协同网络和注意力机制为群组用户进行 Top-N 商品推荐**，主要解决了群组用户兴趣的动态组合、群组与个人用户的协同商品推荐，以及新用户的冷启动问题。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrTuoiaBzkew8v998m6TqZUkm0vbXyicRArv17e9ibTgRAGib6bgg87nBBnQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrCnOnx4hVYVrT8CicaRogSiamykDBQ4GL1YJOX8a95fuI0aQ5IU2OHCZw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3XrS1xCy2CbMroyWkNdicYE55KvMx9NlBgFIO563XkfKhUCJjxMDNpibWyg/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr643z9GD0Wu9BIlkpEwibEbA27jL6xdLz1u4pWo28a0HVnkMfd0WBXmA/640?wx_fmt=png)
**▲**论文模型：点击查看大图




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljMFyo8NBFMJ5GF0HXj3Xr2KfOmYtEQpWjxrkYefHaKuOnbzr5cfsLNGR70SudWnqCVibs6PDKW6A/640?wx_fmt=png)




论文链接

https://www.paperweekly.site/papers/2732




源码链接

https://github.com/LianHaiMiao/Attentive-Group-Recommendation










**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****推 荐 有 礼#**




本期所有入选论文的推荐人

均将获得**PaperWeekly纪念周边一份**



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm5Wb1iaUHxx8mBh1Km3dWjfPlgYsxpxlV44icJWDVwuPorALMxCQglAC8Dx8JMeic5wHeNw29gJV8SA/640?)


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUWQfATNyq8icodseL6gFjp8w4sQ1DBTuiaChXPEcQ0Q6tmRmz2jJjzic7g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUk6ibiaGfmJl0icaK5go84z9iaLysegxS06wkEIrCkuL1eV2dicVoBusY4aQ/640)


**▲ **深度学习主题行李牌/卡套 + 防水贴纸




****礼物领取方式****



推荐人请根据**论文详情页底部留言**

添加小助手领取礼物

*每位用户仅限领取一次




**想要赢取以上周边好礼？**

**点击阅读原文**即刻推荐论文吧！




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击以下标题查看往期推荐：**




- 
[又为写作思路熬到秃头？16篇最新论文打包送你](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492302&idx=1&sn=1efef6309e70dedd9c5380cb644fa4ea&chksm=96ea3d4ea19db458e7ce12f066e4e37c137ec67fdd5f7e851823660f9dccbd23bbad1ce2a255&scene=21#wechat_redirect)

- 
[收下这 16 篇最新论文，周会基本不用愁](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492583&idx=1&sn=85ec5352079218745428d66ab8ee97d4&chksm=96ea3c67a19db5718f5412c64f4c11d28cab5eda2826350fd5f15ac3e888f6ae7a9137eb31bd&scene=21#wechat_redirect)

- 
[这 16 篇最新论文，帮你轻松积攒知识点](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492912&idx=1&sn=e223b0bf570148493313ea8780cef2fc&chksm=96ea3ab0a19db3a6eb87b8c8d6cb41d1a4ae0d85b5fd7b616baa970234124c320fda1cdcc7d9&scene=21#wechat_redirect)

- 
[本周NLP、CV、机器学习论文精选推荐](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493834&idx=1&sn=f5b7de713568324bf3b96742e36f5e30&chksm=96ea374aa19dbe5caf333cfa01ca77ef3e36485b9b9241a5488d47509cea5e79620a223b7932&scene=21#wechat_redirect)


- 
[本周有哪些值得读的AI论文？进来给你答案](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494225&idx=1&sn=888e5b2eda5b3b64da92d2ce2fe383ff&chksm=96ea35d1a19dbcc72f19636a2686d80a87042a7028e7aa2a68efbe6010f325f19f11ee068b68&scene=21#wechat_redirect)

- 
[论文多到读不完？不如看看我们为你精选的这15篇](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493452&idx=1&sn=93c7cc02af605e3d8b86fef6ec2cee26&chksm=96ea38cca19db1daddf658a56311c1da5448eabce36d7986598d80c5f02ed91d335fab1ba8a9&scene=21#wechat_redirect)











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

▽ 点击 | 阅读原文| 获取更多论文推荐




