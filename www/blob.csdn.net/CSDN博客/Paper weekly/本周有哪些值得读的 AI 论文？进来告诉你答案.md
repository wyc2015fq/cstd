
# 本周有哪些值得读的 AI 论文？进来告诉你答案 - Paper weekly - CSDN博客


2018年11月29日 14:23:18[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：241


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhglryG74dIr2B1019Yibv9PAGsWGGYBiaoSGbK2kzUnbIsicCEiazKMticicR0MPtmr1ynDovFe2kGicSydcg/640)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**121**篇文章
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLeyibzwzxbXX33LXkGrl6XLr7bU661LjwBpuCWCA5nADtRZkQnPttrfQ/640)
**@Zsank 推荐**
\#Attention Mechanism
本文来自佐治亚理工学院，**这是一篇 Attention 机制在自然语言处理方面的综述文章**，包括基本概念和 Attention 在不同 NLP 任务上的模型变种。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL1CjBpOxicby3BzR1S6o8tAotc5unGe7FGzAW8snW0ot6PIibjxXBK6nw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL58FvDJqTppfCR2UBOrC4yvvLJPLiaGFVOrJZoBzwgoQCKeyJ4OaKnKQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLNA9GIKHMqnohgsE8iaG9EFyfLYTporiaFHOXB4co1kABichNUnRx0FlicA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLrIOJaZuGMEClrSYqf8Doia9wFQlLesMBMohPkfs2R8lHrWd26CIUicZA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL2dG0ShUxArG6tWkgpWgGVneGeMtGMSEMorXwz4uG95UWWr3z5QBHWw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLWl7OsfN4ECAjPTt9JLbLjuaDlvwgYFRU81LOeyCpSIGGfSVRick19IQ/640)

论文链接
https://www.paperweekly.site/papers/2520

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/640?)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLT772ayxQ5lH6vJIzYSXSI4qA1Dk1N7d2aK7UmZKWL9JEHibQTZgcvdQ/640)
**@tobiaslee 推荐**
\#Text Generation
本文是北京大学孙栩老师组和微软亚洲研究院发表在 AAAI 2019 上的工作。**文章介绍了“弹幕生成”任务，根据一定时间范围内的视频帧和评论文本进行弹幕评论的生成，并且构建了基于 B 站的数据集。**
通过对数据集的分析发现：弹幕评论长度较短；相邻时间间隔的评论具有较高的关联度。基于此，文章先是利用传统的 Seq2Seq 架构，设计了 Video Encoder 和 Text Encoder 进行图像和文字信息的融合，再交由 Comment Decoder 进行生成；进一步地，利用 Transformer 替代 RNN Encoder，提出了一个 Unified Transformer Model。
并且，为了更好的评估模型的性能，文章提出了一个基于生成概率对候选评论进行排序的指标，对比之前的 Seq2Seq 模型，文章提出的两个模型能够取得更好的效果。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLnVKYbOnxiaQG7jozWs6XcnWbdpQBd31EzFnXDkR1YcLIlSLJ4HQSUHA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLcRRiae1nia6Va32UhFdhYyKOjbq8A296VOJzUEBu8WOOpEAP6y5HAreg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLnoC37jLcCjYDShR5AZib7OkIyNQ2rm3EFqaVvSUEpMcvEU53LHibvic8g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLWYA558XImh3HmWTU5x0fa0oUJYPwY3biaAmnKgGQYcI1heibFXpoJ3Zg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL96efUeXwz7wZG12t2dgCvAd0bMsiaQDdz5OQDibMbRicOrGBgHp4Kg2PA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLQic9lu2QGsW3uZpsFibFOOFscUhvFicP6rRrCM7SegJ2a6FNKIiaqDwpag/640)

论文链接
https://www.paperweekly.site/papers/2524

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLTZpTfbju4L3LAep7ia8bMOVOibAsB334SmMctKxC3DqAczrichq7t20Dw/640)
**@paperweekly 推荐**
\#Relation Classification
本文是清华大学刘知远老师组发表于 EMNLP 2018 的工作。和 SQuAD、QAngaroo、CoQA 和 SWAG 等 QA 数据集不同的是，**本文基于 Few-Shot Learning 提出了一个大规模有监督关系分类数据集——FewRel。**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLKnPaWvUbr8iac7FHkFkU7EiaTRQ0XxLWVZT7X7NX8UWZEXQmm421kCQQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL0Do8kYEUcP2MhZAjMVyXAt8wf1iaVKI1G4cwgn7pdgAh4RAL08nZV5w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL52NJlib0La4ic1xLn5H4KllBibd3OcUpujhogFpacAyGlAXEqcP5uxE1w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLcs5tiaCGhztRjicH2q8m3PZKc6Efab6b25CGwsGzCHXTLk9dMl5pPqxw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL7Vd0Jyyw1zCIOibRRPDHJhNaXwRChnWhcvX3uamicbX2PoXKiaMdjZ96g/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLmqPnyJS3zm16tvic4OVVxGhZNoWrpicBmyJtf1iaGhUJT6SkVTSmF8kZA/640)

论文链接
https://www.paperweekly.site/papers/2465

源码链接
https://github.com/thunlp/FewRel



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL65g0XdAsRQcdfnymzYPNFJzlB1S7fCicxFeukwJ8ibSSBYPLwnUMP1Cg/640)
**@Arborday****推荐**
\#Word Embeddings
本文是 Zalando Research 发表于 COLING 2018 的工作，**论文提出了一种全新产生 embedding 的 BiLSTM 模型结构**，模型特点：
1. 模型以 character 为原子单位，在网络中，每个 character 都有一个对应的 hidden state。这个特点对需要多一步分词的中文来说可能有避免因为分词错误导致下游 function 继续错误的弊端；
2. 输出以 word 为单位的 embedding，这个 embedding 由前向 LSTM 中，该词最后一个字母的 hidden state 和反向 LSTM 中该词第一个字母的 hidden state 拼接组成，这样就能够兼顾上下文信息。
这种动态 embedding 的方法在序列标注上取得了良好效果，特别值得一提的是，**在 NER 上的表现甚至超越了 BERT，但训练成本只是一个 GPU*一周**，训练数据在十亿个词量，与 BERT 相比对硬件的要求极大降低，训练成本的大幅减少却仍有性能上的提升，动态 embedding 的思路值得借鉴和尝试。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLav2tJVZ6RnSQWSpHM1L6fl61EibTDmjO8RJgsjJLyxSaJ0CMVnGIH7w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLYthcssibyLQ2Te4u5VgGp9dsPnr2VsjHEmjbvKEubHpv6Lfia1LhAJ6w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLaX0BFw8sicNwjNgMZbBibCLsZoxgk5Lo3TK4icAJPD37LLc38jdoGoxdQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLlrBEvP8u2HYjwn0sUy2LaBRibJL09xiadhMJ7AVCNOxtEte4w2DSo5KA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLNLH7DicEp3ibJZIPRJcZliaxvORuXs1icg1iaic3DEiaf0ibjiaylmoic13q01iaA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLT7IwajvLmyQPpic41BSLHRThGciaRPiahE8nuLvhXGdYZ6SAV4dW1ziaEw/640)

论文链接
https://www.paperweekly.site/papers/2519

源码链接
https://github.com/zalandoresearch/flair



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/640?)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLJDWzCmPfSASibCq34MOtxIbwSveQSbhGyf50XRkBDTibPYTQESEo94lw/640)
**@paperweekly 推荐**
\#Dialog Systems
本文是微软和台湾大学发表于 ACL 2018 的工作，**论文提出了一种针对任务型对话系统的学习方式——Deep Dyna-Q，在仅使用少量真实用户交互数据的前提下，基于集成规划的方法进行对话策略学习。**作者将 world model 引入对话 agent，模仿真实的用户响应并不断学习生成模拟的用户对话，利用真实和模拟的对话经验对 agent 进行优化。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLYhiaI5iaTPluia1F0RibBhGGxtBp7KkkV4mcce7EfMxuxYXbzEPSXQ2Cmw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLfthyhgxibl4ndK6JibFtuqzPO1C1JYt2F1yViclO6pz4mriafEYoJia4LJg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLjTH4qneib4ecNSicXN1YyNF3CaP1kWX14xXAIBZSIicDhdVPCpiaPvDOicQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLdHHnegiaxfCxJb6ia8y1MPYtNAC9NOWXByibdhvhF6I6iahweZM80L0uzw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLhhQltoh3H6QOEvOmXyG66vp3icuIWlE9icE5mqicuqRWKnYVIYyykuxDQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLHNiaYjCft7ibnh3A1MndHXCqIDQsTOR95Ka3F3Y0vjo5KricQickOrdGRg/640)

论文链接
https://www.paperweekly.site/papers/2545

源码链接
https://github.com/MiuLab/DDQ



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0Drvm1kKqodONJWdluKYXVSiaVksJv8JyrGzSsG6O8Nt5p6aYxkA7aFuLiaQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLXuaiasp2O9sT0ibVAiaMrOAGibwBxJYXBibGc0IGuz0KcfiaibKXkF1MoyPkw/640)
**@CYRYOU 推荐**
\#Natural Language Inference
**本文介绍了一种便携式表型系统，它能够集成基于规则和基于统计机器学习的方法。**论文利用 UMLS 从非结构化文本中提取临床相关特征，然后通过结合 OHDSI 的 OMOP 促进不同机构和数据系统的可移植性通用数据模型（CDM），用于标准化必要的数据元素。论文提出的系统还可以以 OMOP-CDM 的格式存储基于规则的系统的关键组件（例如，正则表达式匹配），从而实现许多现有的基于规则的临床 NLP 系统的重用、适应和扩展。
论文在 i2b2 的肥胖挑战赛中对便携式表型系统进行了实验，作为一项试点研究。系统基于非结构化患者出院总结，促进肥胖的便携式表型分析及其15种合并症，同时实现了通常在挑战参与者中排名前十的表现。这种标准化使得能够在下游一致地应用许多基于规则和基于机器学习的分类技术。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLwKtFLTBmOnWAdFKM3rXfibA1rk8v7arfCiczEJmD8Fz8OBTzrGEoHnzg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLZgKOyWXiclEPTce010RPl3QyJibUjd7LEezibk5HMcJjfxFrHtsRKAy8g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLPlf3JQbgoEf4FRgsxpAJbZ1YChTgJDMNHB1MklBateaQ9qwIRBPOzg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLaQwUe7Tia9nHX5Z6p8rRFOFBb3ibjiardNQ1cR3WyDO3OVWGLNMl24oOA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLUWSswcNhU0qA074yLJ4btRicCojiam31uuLwHfmP0sEneriaPGmrh43cQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLFibQWicF3PGFXQ6icicYmw6Eo0bbxicGUAYMuBickXic6ic5OKjbtiaf2qia8fWg/640)

论文链接
https://www.paperweekly.site/papers/2455

源码链接
https://github.com/mocherson/portableNLP



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvZkYxV68zOCas9csIEy9oS6Oop2huyXBUliaHFUVHicdamRgqibegicc0aA/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLnXfPBwicZpF8gqfkJDw6Px0ibocrUfpQ99UOCPHKyAuyVVNmp6PNDtAg/640)
**@Luolc 推荐**
\#Goal-Oriented Dialog
本文是微软亚洲研究院和北京大学孙栩组共同完成即将发表在 AAAI 2019 的工作。为对话系统引入个性化是今年对话领域最为火热的研究方向之一。此前有一些工作研究在开放式对话（chitchat）中引入个性化，使得对话生成中可以包含一定的对话者身份的信息，使对话质量更高。而**本文关注在鲜有人研究且更加困难的****任务型对话领域**，探索如何能针对用户的不同身份采取不同的对话策略，提高任务完成率和用户满意度。
**本文提出了 Personalized MemN2N 模型，引入 profile embedding，并在对话模型和 KB 之间建立联系，有效地提升了对话系统质量，达到任务型对话个性化数据集上新的 state-of-the-art。**在针对任务型研究较少的环境下，这一工作具有非常高的参考价值。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLwq5ALUuTjC7OhVvwVjHRwrXa56EphQicNMicKAS0JZ3jpHuKZvhKTZJQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLRQfb9G9ksf9J3TnZhqF3ZkM50cRYAKumZptJo6Do04KB6PcGtKw05A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLHWwcWM3coiaoeMMNukMQ4BrfBia6sDNYfM3ZDutLCibzic1zt4gmHqR5Iw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLGnibDZKJ0LxpoYianBxb88nv5eeicWV4iavKdIhymagKX09NEUzSw5dBibg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLtbLicmATUYPnJgSV9f46libOK8uxMwYbTrzohzZWeKFMxuXDolhAIC9A/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLPxtnhKMVF1NlGia5lxIBEVXaocnTzne0awkUQhC0DV6oGjooH8wBYag/640)

论文链接
https://www.paperweekly.site/papers/2522



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvibxtiaicW0ZRIwW0Kmkj9yU90UmGicL2jnnmaBY47NYicK2d7frJAcNP09w/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLNvFfTjVFf91HUYbUDOibEdj3FHfMYNicffYtLhHUibp5SVl1swicLfzq7A/640)
**@hsu 推荐**
\#Semantic Segmentation
本文是阿德莱德大学发表于 BMVC 2018 的工作，论文关注的问题为实时语义分割。**作者将源自 CVPR 2017 的 RefineNet 作为基础网络，结合两种残差模块 RCU（残差卷积单元）和 CRP（链式残差池化）有效减少模型参数和计算量**，在 512 x 512 大小的输入图像上将分割速度从 20 FPS 显著提升至 55 FPS。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLk1MogfNucpibVoDv1K6UhYG3QjUkxRLX58u4c1P6lic6dZ5xvC8k7Kvg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLLGItrcZibHVs2VFeA9CmTzl0x2nnDAyL68t7fnxTIqIUgEez21J1IFw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL1yPMnVuoiaEsoGUlic7hVaIIM0akd4rUxhkcDrzWsDxMBZ6GcRDWxxzA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLl2z2Xv4OmVXsNq7Bc7zaFRuqtonQ2icibG3CBOZiarBZ0xwiacwMFCf4iaw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLutE4v1FBuY4raho0ObEMaD8cGHt2TAYk4Xf7021fD7kicYYsMYaz4iag/640)
**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL6MdGYtqNo7HeyIWib4mMRxD7iaLASmBZAU61KPgtwIB4BW3cGQE9C1bA/640)

论文链接
https://www.paperweekly.site/papers/2555

源码链接
https://github.com/DrSleep/light-weight-refinenet



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvHib5D8hcewE9gwNibrGkW1TC8v83Y89RITicqLb5N3URaM1wGsGBV27qQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLPVU7wgsru86icI0hPyUjympEWs8tgNoyFj4ic8tiabkJuAkpNKb9K5BMQ/640)
**@jhy1993 推荐**
\#Gaussian Embedding
本文来自 Google AI Research。之前的 Gaussian Embedding 都是将 Object 映射为一个高斯分布来捕捉不确定性，但是一个高斯分布足够吗？**本文将 Object 映射为一组高斯分布，即混合高斯。****本文提出了 HIB 来解决上述问题，所谓的”hedging its bets”就是为了避免损失，多处下注。**对应到本文，对于有歧义的输入，将其映射为多个高斯分布，每个高斯分布代表其中一个意思。
针对图像的歧义需求，作者设计了 N−digitsMNIST 数据集，即将MNIST中的数字组合成一张新图，这样每张图就有歧义了。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLMsHaVB1YlYQudBXPNZ5UyOq1miaZLtoicPWvQHGxpObFZasOJ0Y51Uvg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL4t8aHlsuI8VpFFpCjUqzB7mLL38neBNIvUUdalQzkpefHPTuDCs2Ug/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLicD6jd5UVkLG8wCWsYc1bx3onlYUicmibY0gOFH223GzTBibiciaKuSwnk3Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLOfwo8oLTPz9MhXKfowAJX1pBmGnLXVrkApN1425Q8Uf4maWiclfVzyA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLZ5BY6MoiaJ7TfrEXZJd9r0Jib5CbIDGkJMJxfpCiaGeE6j7TNnMh7CerA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLyjl1NGDgoId112aeRoHHU6hIM6aAcKQbic1Pef8wsI5ezicJgYpu5ruw/640)

论文链接
https://www.paperweekly.site/papers/2506

数据集链接
https://github.com/google/n-digit-mnist



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8tiamiceTcrbl3UY25cTHibSgtJNZnMBCOUdcpTpSLK45Ya9RC8yDZsSEw/640?)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLrsWNyTAibqzYJoWHabw2uonTiaWy3L5GqbV0N9WAO8yfxnIKKTcoa85A/640)
**@Kralkatorrik 推荐**
\#Face Recognition
本文是商汤科技、香港中文大学和南洋理工大学发表于 ECCV 2018 的工作。在人脸识别数据获取成本越来越高的情况下，**作者提出了一种称为 Consensus-Driven Propagation 的模型，采用少量标注数据和大量无标签数据训练，即可获得接近所有数据都有标注的精度。**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLvWdQCbgF6c54ian0BC0KngKRbemwJwjoKcZrhwKiczvcHsUArBc2zKdA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLehUonqafxTQVaHXHibcfiarFoAFpibcQnDhr7XGTmCI0j9hDx09QGzltQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLLf490a39UzhngYJicPHpgibpe4sRRA8lkRthQmXtheq0ibquDa0p6YCXQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL75E3hp6zgvgmd2GoXEc1Znmw3JXX8dO65fwiao75t0KcZzgcYRDOWeg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLXIWyVkwBaI1MZnnaa04FqPib9dF1UR3Sjh0NUpibqADR0yrpEsz5FoDw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLhY7N1j49sqh7bicYKYus5fyondq3Ksqa4EArYG8sMBg8KDdtRZOp8Jw/640)

论文链接
https://www.paperweekly.site/papers/2527

源码链接
https://github.com/XiaohangZhan/cdp/



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8IA3BMnKpHmwoB8kAc8CQC4UOSu2G0c5vFM7xpJZOcqLdFHch97tiaGg/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLfc1YP0Sn7cUNqWgZexeNsmVEVedToyPX5picM2VLkMricqiceVssaMDcA/640)
**@vimjian推荐**
\#Object Pose Estimation
本文来自欧姆龙和中京大学，论文关注的问题是实时 6D 物体姿态估计。**作者提出了一个基于 model 的匹配算法，效果卓越。**在 CPU 上就可以达到实时，并且训练只需 CAD 模型。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL4VmgLxnexs01YtiaaS3tobT26wuVRTwy84UmqZLXNCUznUzeccrRPyg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLxDqxmjd4v62n3CayGBF6fARhPngx7kk3dNoxldyqBgWLkAWCKB5tlg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLAsvaUQDKknzYTzNK342snH9pZEaTVxjFNtvibDHgfJnzvjQ4Bh2c3QA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLfUicD7YMScIOWp6rY7iaGqa8RgaVzhqZtjibIwJnAg03epohU6QMUlpWg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLlNpH0k2nHMtxOqqicQFsd6U9GHlYq6g9stib0Zy2yUwgBDCzoruDOOKA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLplU9sdjhuIE8OIlicWfE96ezspqDPwVqgNq3ibCoTY7w8zgBZ2YRW7VA/640)

论文链接
https://www.paperweekly.site/papers/2572


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvSrUEOribtWtcbc5Bs8icSOWQPFxgpHLCrooqDs1LNC02qthicqiaUiaLzeg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLW1Y3DxM4OEicE4iayYpoLDGn0vwoqWibzcNphbKVwy80TLZ8j97auG8qQ/640)
**@SXKDZ 推荐**
\#Session-based Recommendation
本文是中国科学院自动化所智能感知与计算中心被 AAAI 2019 接收的论文。会话（session）是服务器端用来记录识别用户的一种机制。在推荐系统中，基于会话的推荐（session-based recommendation）是依据一个时间窗口内用户连续的行为进行推荐。比如，一位用户在登录淘宝的这段时间里连续点击了 10 个商品，那么这 10 个商品就构成了一个会话短序列。由于会话数据具有海量、匿名的特点，基于会话的推荐近来受到了广泛的关注。
**为了更好地捕获会话的结构并考虑节点间的转换，该论文提出了一种基于图形神经网络（graph neural network, GNN）的会话推荐（SR-GNN）方法。**该方法将所有会话序列聚合在一起并建模为图结构。基于该图，GNN 可以捕获项目的转换关系。之后 SR-GNN 使用注意力机制将每个会话表示为全局偏好和会话当前兴趣的组合并据此进行推荐。在两个开源数据集上，SR-GNN 模型的性能持续并显著地好于其他 SOTA 方法。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLibWwNibAr0lJ6WSKf6OW7v8oMVDlYHelu6nKsI4fBO857WJIrxgC7dow/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLAnUWzKpEswGRK3rmpk5JJuibRg7BEzsBNe6T4lzC3quBsUd0LDbfvlA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLRyBzufSolZX8ljSxZvV13bkZIeWhxmr2pGrsX6o0EZzhzj8xVjapWg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLPQc8agGEOULYL57lHRYef0FTEVfwWegrACQPIhaPvq6SJr3lEs8HyQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLhibCkBBzjSDs28l8Sqiawe3Mutz4s5ibW3vwX0nbH2jH0hJAlDzJfvDlw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLHgpaaDTgKJaFbRynpqEfHibzRNMNX4VkATLUA6tGUicFuaxaD3uZibEXw/640)

论文链接
https://www.paperweekly.site/papers/2518

源码链接
https://github.com/CRIPAC-DIG/SR-GNN


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUMyCvJ7nksObSLatO1UHuTLcw5KYWPhREehBpXWY0uqseRtib5rxuvBw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLqC9EuvKLMhiccmb2cqTibayMmsycQmw9x0K482R4XKBlSoLyRegBzZ1g/640)
**@paperweekly 推荐**
\#Graph Convolutional Networks
本文是华盛顿大学发表于 KDD 2018 的工作。**图卷积神经网络当前面临着两大挑战**：1）图中节点的邻节点数量具有不确定性；2）邻节点之间杂乱无序。
**本文提出了一个可学习图卷积层（LGCN）来解决上述挑战。**LGCL 基于数值排序自动选择固定数目的邻居节点作为图卷积神经网络的输入特征，使用常规的卷积操作来进行图编码，并借助子图训练方法使 LGCL 能够适用于大图训练。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLvpbfw7rlt9ZOWg4o1uoqcnZAKx1UfEdCG6yhrrNr5GiaAiak4jo5C6aw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL1DMEr1RCdf7YGD3hFHQib0lF14sDKh8BRaW2KGA6ZjRSg6PJS48bxEw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLt7oun5ve77aOJicX83YicAuQ8yxw2g1tJZCfQUEkc0SBCaOiabe3VCaJg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLScFEWu2m8kUN84bWfUExIZJaoWfzCkrofQgSh6KMzsZiaPbs8rulthA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLSSbG8PsT3denwxcicMvdkGmfLgm5jqdpiaY1wPRjH7FvaxoBz1t2ygbA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLxmLmCWhOefnEUf0cCibIF3Z7NfYtZIAXsaS6FNHsbxjyKM7icKtv1CRQ/640)

论文链接
https://www.paperweekly.site/papers/2487

源码链接
https://github.com/divelab/lgcn/



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjiaA5bbtkAnnJ2yLicAzlwmVdnAIic0THYptctQhZJRx7QYCx8TC9zwOow/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLDNP45A5yhkfdqTbNGQe5wI5KA6z5UEibnk0xhtZicBpktVCmDvAyMq9g/640)
**@Rcypw 推荐**
\#Network Embedding
本文是南开大学发表于 KDD 2018 的工作，**该论文首次将网络表示学习问题转化为机器翻译任务，解决了目前网络表示学习方法中向量组合策略结果不理想问题，以及复杂网络难以捕获长距离路径结构的问题。**作者在不同的应用上对几个模型进行比较，取得了较好的效果。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLCTz39Cle6ZKcjYDSBkyoVkrLptOyJSrib8ibgyZPsr8jDxpMs3Mtb1mA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLsibMdsRlObofqedlqGQMhfzLyuft8ewA7joARXSFQqjiaibB3UIWpslKQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLsp4cplhdK7VbTDEKkEiachftzEgFZib4MCYOicYSJyzCjyREDqXt9u9uA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLSKT3ngSe2EKSYXNK3rlTSA53FeJIBrGV4ConfzPSfdMAwFdZiayY0rg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLjTf3RbHA9AE0q8yqHVb1icG3PCvU1CzgtKBWNOQBE7qgnkYZYc0hbJQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL80GV2Bhlxd5XT8XOqznIrz41GVjAAT8tWtJND9Vl9g62XR3gWhKupw/640)

论文链接
https://www.paperweekly.site/papers/2530

源码链接
http://dm.nankai.edu.cn/code/STNE.rar



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjgzGNHdv2YBVm6bUicHjY2A8tV8hwJIiapvTYDGmFIyMclC4Xy6gD7krQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLdVyMQEE1hxVEzib05bE9YdCaHy5w3c3uF1NAXMfb2OTaoHXF3zqnnjw/640)
**@paperweekly****推荐**
\#Recommender System
本文是新加坡国立大学发表于 WWW 2018 的工作。在推荐系统任务上，基于 embedding 的方法虽然具有良好的推荐表现，但其整体仿若一个黑盒，难以解释具体推荐原因。相反，基于决策树的推荐方法则能够从数据中进行规则推理，进而给出具体的决策原因。
**本文的写作动机便是将二者的优势加以结合，****提出一个全新的 Tree-enhanced Embedding 方法，既保持基于决策树方法的可解释性，又具备基于 embedding 方法的良好推荐效果**，进而保证了整个推荐过程的透明、可解释性。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLcHnEzhNRZxSw9XupSdNuUsfxndDmqcAr5aRG3iaNG5ypejbyibMSnybw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLZf7ziblOfON8vmhK0ShLQY5lGolEH01FGKGyVgeX30sKSYRgsiafOb5w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLAkGyDT2qYfOPJza02hp9DzjMuVk9wZIMb4smXNBibCFRzqYvfDiaJVibA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL6bhvd7hKahVPXFGlEzWibxIl4hicqelzYVU81yF51N2gRz2KMBDPVgibA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLEibxhbQ0PFMs0dYb3lgCFuy35QsVibdqicoibcGLbw4ibxicicNmMG6pDaWlQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL087TAw8Q3YN8abicE57fHnOjzSCc33AFHcbNiarEbicsAbib4KImuJ6CcQ/640)

论文链接
https://www.paperweekly.site/papers/2548



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhXUf47ic4GfJDLia8icEqZtGqF7Fy9O7GWRcGXzaHiaJ9TOpwBhL1MQrEvA/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLt03oOTXKTVazOmhzL358C3scwMLVNPQtYLJnnnLwgXCfrXBia9Q5q5Q/640)
**@HarryLiu****推荐**
\#Convolutional Neural Network
本文是北京大学发表于 AAAI 2019 的工作。**作者使用 Grammar-based CNN 对代码进行生成**，CNN + shortcut connect 进行 encode，对于树结构输入数据，使用 pre-order 遍历后做编码，并结合使用 tree-path 做编码，为 input 为结构化数据提供了思路。 在 decoder 阶段通过对 max-pooling 结果和 encoding 结果做 attention 计算。
文章对 code generation 现有做法了做了一些回顾，网络结构有对现有组件的直接使用，也有做创新性的调整，内容较为丰富。 在结构化相关任务的 end2end 模型思路上值得借鉴。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLfjDVNYj3CYl2Fic4lfl9Qmyq6OQyBmOCRzCicDfia8gctgZNCJXOXTWYA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL5Cv3UM7rp5YaGyDBktfcRITKY9PiaRy3PxeBicq9icnE9a4zoqF0UvZeQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLZia16FlK4icaM9oMS8q3mIYzuPSNpnUAibQUX7D6tpB8ibDT8re4I5y6rg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghL8qwzYKI3TeS2UMPicShlRmKwMVhJJ5ony9OXLZNawpO8E7pAbSfIF2w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLE3G4ianzD6ticcSYeC5ibYofLrUygJfWbMw9phnfke9jbNXTic1MS7RaUg/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkYUKqXzRT4tZOapibBMcghLsdmANB4O3g7W58sVlcq3ib03NvTCGBUmTPiceehEIfxh2EDoQ8GDzeBA/640)

论文链接
https://www.paperweekly.site/papers/2571

源码链接
https://github.com/zysszy/GrammarCNN



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****推 荐 有 礼****\#**

本期所有入选论文的推荐人
均将获得**PaperWeekly纪念周边一份**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm5Wb1iaUHxx8mBh1Km3dWjfPlgYsxpxlV44icJWDVwuPorALMxCQglAC8Dx8JMeic5wHeNw29gJV8SA/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUWQfATNyq8icodseL6gFjp8w4sQ1DBTuiaChXPEcQ0Q6tmRmz2jJjzic7g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUk6ibiaGfmJl0icaK5go84z9iaLysegxS06wkEIrCkuL1eV2dicVoBusY4aQ/640)
**▲**深度学习主题行李牌/卡套 + 防水贴纸

**礼物领取方式**

推荐人请根据**论文详情页底部留言**
添加小助手领取礼物

**想要赢取以上周边好礼？**
**点击阅读原文**即刻推荐论文吧！

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击以下标题查看往期推荐：**

[又为写作思路熬到秃头？16篇最新论文打包送你](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492302&idx=1&sn=1efef6309e70dedd9c5380cb644fa4ea&chksm=96ea3d4ea19db458e7ce12f066e4e37c137ec67fdd5f7e851823660f9dccbd23bbad1ce2a255&scene=21#wechat_redirect)
[收下这 16 篇最新论文，周会基本不用愁](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492583&idx=1&sn=85ec5352079218745428d66ab8ee97d4&chksm=96ea3c67a19db5718f5412c64f4c11d28cab5eda2826350fd5f15ac3e888f6ae7a9137eb31bd&scene=21#wechat_redirect)
这 16 篇最新论文，帮你轻松积攒知识点
[本周份AI论文推荐新鲜出炉！真的很skr了~](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490599&idx=1&sn=7ab5ab73f89328901c47006674591753&chksm=96e9c3a7a19e4ab1f1d55ceb8d17a7b03622351c2f2bf49acd360b49664b3461d070e61c9077&scene=21#wechat_redirect)

[想了解推荐系统最新进展？请收好这些篇论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491818&idx=1&sn=311962e2e41119a565c252a19037dd76&chksm=96ea3f6aa19db67c3fbfa77fbec65797d0ccc8f2930290d57c2016a3e55a8bb18b77fd10180b&scene=21#wechat_redirect)
[正经炼丹师如何完美安排国庆长假？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491876&idx=1&sn=e431c2291f4c985777be4d9b7bdfefa6&chksm=96ea3ea4a19db7b2c266db83b63438a9ad6fe0c0d9429864cda528fdaa601d4f55c6831e3dd3&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
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
▽ 点击 |阅读原文|获取更多论文推荐


