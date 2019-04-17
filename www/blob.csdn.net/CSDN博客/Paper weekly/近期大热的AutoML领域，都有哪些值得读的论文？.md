# 近期大热的AutoML领域，都有哪些值得读的论文？ - Paper weekly - CSDN博客





2018年09月20日 13:21:36[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：932









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhglryG74dIr2B1019Yibv9PAGsWGGYBiaoSGbK2kzUnbIsicCEiazKMticicR0MPtmr1ynDovFe2kGicSydcg/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **106** 篇文章

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?)

**Finding Better Topologies for Deep Convolutional Neural Networks by Evolution**

**@zhangjun 推荐**

#Neural Architecture Search

**本文通过用进化算法来做卷积神经网络的架构优化，最大的亮点是给出了影响架构性能的关键因素：1. 深度；2. 各节点之间的连接性。**通过进化算法优化网络结构最大的瓶颈在于计算效率上，网络架构的效果评价是一个耗时的工作，生成和变异出的大量个体都需要评价。

本文从一个最简单的架构作为初始个体，通过预设定的 5 种变异方式（添加边、节点、滤波器等操作）对原始个体进行变异优化，并通过可视化的方法对进化的过程进行了跟踪分析，找到了一些规律。完全自动化地去设计架构比较难，但通过自动设计分析出的一些结论可以帮助提高人工设计的效率。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhKMsa7MqfKicOfib4GneEduqibJckjTv4uFnq5w55sFLLr8oqK6ZVfl1SQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhGVgsKczo8OPbuyFM5uQ8mg9eLe3uFl7NvBaWcaXUdVGkXbjYOWfj5Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhlC3icRzDf7geIg9mVZExQMicLI1RVy18OPxoncfKAiaRgQXvibyT2qlv9g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhrQ3qzRuh7UNdMkMm2bq7SwX6v8gYCOjRXXVjch7dLnJYp0SPLWdpmA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhDngBm84C6MrGIiak0b3N0c1TR3enj0V2A2MbaoLQLAd02XlpHYnJTpg/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhicicWzicibqPe8Hl8kIBsxf6nvko2ricufjCJwRQZmPw4erfBbYt0fEW6yw/640)




论文链接

https://www.paperweekly.site/papers/2303




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/640?)
**Searching Toward Pareto-Optimal Device-Aware Neural Architectures**

**@zhangjun 推荐**

#Multi-objective Optimization

本文来自 Google Research 和国立清华大学。大多数 NAS 的工作都针对优化结果在 test dataset 上的准确性，而忽略了在一些硬件设备（比如：手机）上的模型还应考虑延迟和功耗，由此可将单目标优化问题转换成一个多目标优化问题，**本文深入分析了两种常见的多目标 NAS 模型 MONAS 和 DPP-Net，并在一些设备上进行了实验对比。**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhXoNfxWDDP2tCPwGm4Ll3zQXkgk6G7OfTxefIHXdT8L9LUhkGc3UOfA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhHmic78ZTAW58jVYj7LNsiaC2IQZm8HGf78nlqQCDt1ic2wtxe2G4cX2Rw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh4IATQFzRqb3qde05RNHFJ8c83uTQj824p6sH96l4SRJtRW3TSCvJAA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh6cm2Mm9fPccLic6Lv1ial9dWUUxUMvv8qd2Yo1axg22aYZLNVT4zZLhA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhw5AuyTQ2wrdbRMqCZQXGN6YLmro1QISxXFrz0aFgLoONqyEx7tYttA/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhPGe1unTtCzouQ0D1v8iaYqR4VSeKU5xhDq5laUW3ZIXplAsqYk27YeQ/640)




论文链接

https://www.paperweekly.site/papers/2259




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/640?)

**Neural Architecture Search: A Survey**


**@zhangjun 推荐**

#Neural Architecture Search

深度学习模型在很多任务上都取得了不错的效果，但调参对于深度模型来说是一项非常苦难的事情，众多的超参数和网络结构参数会产生爆炸性的组合，常规的 random search 和 grid search 效率非常低，因此最近几年神经网络的架构搜索和超参数优化成为一个研究热点。

**本文是一篇神经网络架构搜索综述文章**，从 Search Space、Search Strategy、Performance Estimation Strategy 三个方面对架构搜索的工作进行了综述，几乎涵盖了所有近几年的优秀工作。

**论文详细解读：**[神经网络架构搜索（NAS）综述 | 附AutoML资料推荐](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhVstO9RodpUpC5z7hLRLVPtVOg06jgiaHib0wXGmNLhibicPREMDgJO2r8g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhth9efLBnBAn9kR1IlGib3QzuxerqibSGDguV7nriaWuKY8JhTsrVaQicXA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhTrDnFAeTH2WGTBpPo5d6SciazlCHmveWboONicKZkoNHQibkmibqrVRY7A/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhS7k0WM0ceRaPx9pRhvawQsZLD2G1RLnZfSUfibnuDoiawDwJDQ7lAkaw/640)




论文链接

https://www.paperweekly.site/papers/2249







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/640?)
**Reinforced Evolutionary Neural Architecture Search**

****@zhangjun** 推荐**

#Neural Architecture Search

本文来自 arXiv，作者来自自动化所和地平线。用进化算法来搜索模型结构存在算法不稳定的问题，用强化学习来搜索存在效率存在一定的问题，于是本文将两者结合起来，各取其长。**整体框架是基于进化算法，而 mutation 操作是用到了强化学习来控制。**在 CIFAR-10 上用了更少的计算资源，得到了较优的结果。并在移动端进行了测试，在保证较高准确率的基础上，用了较少的参数量。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhiaQic9FEgI9CQcbrAXAIClViav0rTUn9EXjlFWDqS3TwjRlxonww2HRFA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhticTgqoAkwgR5EF3giaeBtXCOSbbBb8Gt7Lv2LnjwDKtUO1iapFpt5Yrg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhL1Q6pbiada9XKhR3NEcaWsLMV5ic6qTqmZiaictCVvsWS8HH3iayYFnwcrg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh1HQX5bP3WUUEvNQFHVlrJOxMXPxN35VDhknPtFb6LPciaTaOWGc8Qrw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhMCSJn7M5zRv21mhCas3soNCRA0890gDJxsbuiaouaqVMjeUlHicoAJ1Q/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhCwlKYeDyTZMCzmOj84Ob1lzDSubLxrSIzwXtQEfbdtME0mUuI0hjJg/640)




论文链接

https://www.paperweekly.site/papers/2311







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/640?)
**DARTS: Differentiable Architecture Search**

**@WildCat 推荐**

#Neural Architecture Search

本文来自 CMU 和 DeepMind。当前最佳的架构搜索算法尽管性能优越，但需要很高的计算开销。与传统的在离散的和不可微的搜索空间上采用进化或强化学习搜索结构的方法不同，**本文提出了一个高效架构搜索方法 DARTS（可微架构搜索），将搜索空间松弛为连续的，从而架构可以通过梯度下降并根据在验证集上的表现进行优化。**

作者在 CIFAR-10，ImageNet，Penn Treebank 和 WikiText-2 上进行了大量实验，表明本文算法擅于发现用于图像分类的高性能卷积结构和用于语言建模的循环神经网络结构，同时比现有技术的非微分搜索技术要快几个数量级。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhcJujIQF0micIhD6MFnTmTLGCzia9TaLaC2WmaU5dPXZ8ia4HTbL99tUrQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhCYTRqpFAcZnN81dBqQECLJPCvz0kgEP6Eg9mdGhkciaAOS24sibSFgjQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhmlmaets9PVm6fmUQOublAVKuYWf9xCgne7bSQAagZeNibqQCvWOZzUg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh9E0qvq5KiabX42ibddHXKDr3nHSG8kDxhqk8JffTT54ichXb0wKvvNq4A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhtU2uToC8nic6iaO0vpicbCCw1iaUQbvdX57NiceLPxMHXRk4vaoW4Af3Yfw/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhcdygHduaViauRNd7oNmib7BdicS5uOVWzyqHyZUMpk2TDXicMImhx9Ct2A/640)




论文链接

https://www.paperweekly.site/papers/2069




源码链接

https://github.com/quark0/darts







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0Drvm1kKqodONJWdluKYXVSiaVksJv8JyrGzSsG6O8Nt5p6aYxkA7aFuLiaQ/640)
**Efficient Neural Architecture Search via Parameter Sharing**

**@Synced 推荐**

#Neural Architecture Search

本文来自 Google Brain 和斯坦福大学，**论文提出超越神经架构搜索（NAS）的高效神经架构搜索（ENAS），这是一种经济的自动化模型设计方法**，通过强制所有子模型共享权重从而提升了 NAS 的效率，克服了 NAS 算力成本巨大且耗时的缺陷，GPU 运算时间缩短了 1000 倍以上。

在 Penn Treebank 数据集上，ENAS 实现了 55.8 的测试困惑度；在 CIFAR-10 数据集上，其测试误差达到了 2.89%，与 NASNet 不相上下（2.65% 的测试误差）。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh3tRQFdibeJnRPLdia7OLia2vyoIUDdfa3HrYqz4Y9lmBVKzoH78GH7ERg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhF3YGxjWwUfm1pcFfyIWy6fOUZbFibOAIDlxV8l04bo5LRogHBHTFfsQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhmGEmMY79ia51OvNyVHnMv4gs03TsKjwuJO1g1et9Yy1e1uQSnjGt8Dw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhrtEnxIKwFYj8hvMdvKIIY6IjAIXauJ2s3JJyHWXFthUqaHNsO944Jg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhaZnicOOCzJVb7tBN6GiaSwUYTRMw9tUoZQNafjMCt5z4bjZBR9LVUvjw/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh1JRrRnrqlGt6kbV1KC5o4zH0xA0icGxMFYMINete5wADOTUNOKf4ToQ/640)




论文链接

https://www.paperweekly.site/papers/1678




源码链接

https://github.com/melodyguan/enas







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvZkYxV68zOCas9csIEy9oS6Oop2huyXBUliaHFUVHicdamRgqibegicc0aA/640)
**A Flexible Approach to Automated RNN Architecture Generation**

**@Synced 推荐**

#Recurrent Neural Network

**本文来自 MIT 和 Salesforce Research，论文提出了一种用于循环神经网络灵活自动化架构搜索的元学习策略，明确涵盖搜索中的全新算子。**该方法使用了灵活的 DSL 搜索和强化学习，在语言建模和机器翻译等任务上表现良好。新方法可以让我们摆脱依靠直觉的费力模型设计方式，同时也大大扩展了循环神经网络的可能性空间。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhNN0n1peKh0rTjXWAgLumWofQO1gibiauygcHguUXPz1MQH0r0Q1TvEOg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhHGwzLoWqJL7rVVBDGoRicKOxlV6yS4qE8BfJdaxfMXxVGsVWIxs97Nw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhjxvPt9w6aN1GS1LpbI9LPAO56L5o24r4BqMZOzcES5CnKcgvjjaWSA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh75DbUgjcjb6kMtuS4mDvSUhgREahvgAKHwiakahUFmD30qXw14qo8Pw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh0GjDibGiayTIkic9fCVGeMCYrFaB0J6HRIOAicHFxETu5uSZRA4C6D5q1w/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhbp8kGCdaNbVu1wpdickAojhax4cb55ib8vFSeTJexSk33XEpUduZwBaA/640)




论文链接

https://www.paperweekly.site/papers/1406







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvibxtiaicW0ZRIwW0Kmkj9yU90UmGicL2jnnmaBY47NYicK2d7frJAcNP09w/640)
**Progressive Neural Architecture Search**

**@Synced 推荐**

#Convolutional Neural Network

本文是约翰霍普金斯大学、Google AI 和斯坦福大学联合发表于 ECCV 2018 的工作，**论文提出了一种比之前的方法更高效的用于学习 CNN 结构的方法，其中没有使用强化学习或遗传算法，而是使用了基于序列模型的优化（SMBO）策略。**在其中按复杂度逐渐增大的顺序搜索架构，同时学习一个用于引导该搜索的代理函数（surrogate function），类似于 A* 搜索。

该方法在 CIFAR-10 数据集上找到了一个与 Zoph et al. (2017) 的强化学习方法有同等分类准确度（3.41% 错误率）的 CNN 结构，但速度却快 2 倍（在所评估的模型的数量方面）。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhbdWcuctGx9Bdn7GYZSibpXG1SH4uhiasxxbxOPYUeR6Z0aL9q3sLK0Fg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhhgImdM6gX8HHTmoficKpZwNMJDNo3q6tjiaO8CTCX1fLpjicO3icUh4YuQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhdloOfmdNvwoVibHdb3A1QlvgFUT4VR7WeM0ftIiaMvQBkHsiaqMvxaKGg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhPnOTRDt0rxo1Mo2u5KRiaeicoiaTcgSCiaMeTs04dbXbSVZcZoLiaGs4S1w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhqwCSG9IuQyhYt2PcBgAm3qoZrxqVah9sESd2dpJmD8ovUwC4Hnib02Q/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhuGFuoic4klkiaMO8DOHwf2ibg9EqrMVicPI1drNJrs6mGOyD0RPm2Rfic6w/640)




论文链接

https://www.paperweekly.site/papers/1369




源码链接

https://github.com/chenxi116/PNASNet.TF







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvHib5D8hcewE9gwNibrGkW1TC8v83Y89RITicqLb5N3URaM1wGsGBV27qQ/640)
**Hierarchical Representations for Efficient Architecture Search**

**@Nevertiree 推荐**

#Neural Architecture Search

本文是 CMU 和 DeepMind 发表于 ICLR 2018 的工作，**论文提出了一种结合模型结构分层表示和进化策略的高效架构搜索方法**，可以模仿人类专家常用的模块化设计模式，和支持复杂拓扑的表达能力很强的搜索空间。该算法能够高效地发现性能超越大量人工设计的图像分类模型的架构，在 CIFAR-10 上获得了 top-1 误差率 3.6% 的结果，在 ImageNet 上取得了 20.3% 的结果。

通过比较人工设计的架构、使用强化学习找到的架构、使用随机或进化搜索找到的架构的实验结果，研究者发现这种搜索方法能有效找到性能超越人工设计的架构。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlheEicVNayBkiaHcF6z6ZcbdQnqJVoTOBEmCqWTbqfXowmDrrhB7szICBA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhtEHE9DMtTPaEf8yzwjVS67OTLpondq3TpmDyd2ffXeZgepQ1obOjbg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh1iao4I5ERicTCKuJ42AbXCDdXR8tvicAntG8YBegEMqjDY7jEHxwC5WPg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhF43nylmYicyEg1lbxekLzicxVRZZ2cVaK8ibLgkoV4kW7DCYkiaMUq1MdQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhyBEENRIt1UP7o3nqpnQ4kQKHiclkf7bCRj4sDF7yB1t3NYS9MHibkYYQ/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh5VPicStq7LtAbnpCzyGeTfHaaj82DpCMg2tB4vZ3udhkHsEBApczvIg/640)




论文链接

https://www.paperweekly.site/papers/1233







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8tiamiceTcrbl3UY25cTHibSgtJNZnMBCOUdcpTpSLK45Ya9RC8yDZsSEw/640?)
**Accelerating Neural Architecture Search using Performance Prediction**

**@velconia 推荐**

#Neural Architecture Search

本文来自 MIT 和哈佛，**论文针对 NN 的 AutoML 中遇到的 Early Stopping 问题，提出了一种新方法：用线性模型预测模型的最终训练结果。**AutoML 中，Hyperparameters 的搜索过程是一个 CPU Super Expensive 的过程，这个方法能够降低大约 50% 的搜索时间。

这是一个新思路，并且我对这个方法做了一些实验（主要在 CNN 上），效果确实还不错，说明这是可行的，我认为在不论是手工调优还是自动调优，这个方法都是以后的训练框架可以借鉴的。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhMPdRZPicc2OKEC5ps5DQo2tbSx527YhmicA5zEn8bcklmtSc8iaSWCALw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhtgDpBXnHX65TXrWyFTMick3k2mcqaibAiaJ4drvlhfO52YbzMulTiania8Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh1OaJicBXjBc1hNp2dYPBEKKA1Kjbviblv3RuI1x5WUjmwQn0ZKvTvbiag/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhlrS9FMM8iboRTtyduJve137xbhAicMAeI8hnQeph1IiajbV9glOXoJOQA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhzyATX4q1f9txoDWIXAbaEy8HamKS7WiaGJmqmrXVgrG99Dia5LK6ZDdw/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhDagsVQxLTkUTHuETeeZ09O1pC9gFJhIuwaxL9mI7Z9qPpaoLibDib8qA/640)




论文链接

https://www.paperweekly.site/papers/1897



数据集链接

https://github.com/MITAutoML/accelerating_nas







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8IA3BMnKpHmwoB8kAc8CQC4UOSu2G0c5vFM7xpJZOcqLdFHch97tiaGg/640?)
**Large-Scale Evolution of Image Classifiers**

**@chlr1995 推荐**

#Image Classification

本文是 Google 发表于 ICML 2017 的工作。要执行现实任务时，神经网络需要专家大量的研究设计，而且通常只能执行单个具体的任务，例如识别照片中的目标等。人们希望拥有自动化的方法，为任意给定的任务生成合适的网络架构。

先前有一些开创性的研究利用学习的方式来设计网络结构。Google 想利用计算资源以前所未有的计算规模和尽量少的专家干预，获得更好的解决方案。

**Google 结合进化算法，以简单的架构模块和常用的初始条件，设置了一个演化过程，取得了不错的结果。**整个过程简单粗暴，每次演化随机删除或添加一个结构模块，或者添加 skip connection；同时过程中，保留优质的网络作为亲代，抛弃效果不好的网络结构。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhumIHIwM9zfT0EibjaQxBMibMAAiccwtFKLY5RgUNickibbaWXNVwCmibiaOvA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhOvAokqAy7u1sr8NERaeALlsGDh5S35JcrkHQiasqCAOtGaicHG1kGPfg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhfR8zQFLXbFSecvHfASdkYLIoRO1Qk0DpyE1TooR0JczvyibibD9LiaawQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhZlh0xElmqBI1UeqQqicxBQyKEtvzficu3qhWTAl5Nxq7tL1GJDbIm0SQ/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhbqp3dib7JGibJ8kb0qBGZ22BzJ4n6nNBNF8eH0Bslf6icxuGxzfOMuZIw/640)




论文链接

https://www.paperweekly.site/papers/1850




源码链接

https://github.com/neuralix/google_evolution







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvSrUEOribtWtcbc5Bs8icSOWQPFxgpHLCrooqDs1LNC02qthicqiaUiaLzeg/640)
**Evolving Deep Neural Networks**

**@zhangjun 推荐**

#Evolutionary Algorithm

本文来自 GECCO 2016，**论文提出了一种改进 NEAT 的 DNN 网络架构搜索方法，并在 CNN、RNN 和两者的混合应用中进行了算法的验证，取得了不错的效果。**本方法属于现在比较火的 neuroevolution，包括 Google, DeepMind, OpenAI, Sentient, Uber 等在内的很多公司都在研究，用来设计效果更好的网络结构。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhVsgcNPAMZH9HOdLscnlnRFrSAHjZgQo5ic4gKI6CygJqqAbPcZh9WvA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhS8wibLem5k5QvpBxe7VfWLWaGPJaSJ27q7cSiaicDzLDRMZ0ibPVWCPKMw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhzjYYSjBZUGTWSAd1ia17bN4icy3ibBtYsmUWOqEjCx3kH5zvQibmCHXAHw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh7yCQWxcPHQTQKYRIGCfF4QnuTCtrhahqx3oa5wRSt7W7rDpO7tovAg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhv2KibAtgKqmtb2lJvldwNkhkmZ68WkOG0UcTlicp39NQGXqKuW7qib1pg/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhIkwFnlFicry3Q0JCJ2AwkyibkI6fuRD0KbIlzuHFKb6DFYmKVxPaqNFw/640)




论文链接

https://www.paperweekly.site/papers/2307







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUMyCvJ7nksObSLatO1UHuTLcw5KYWPhREehBpXWY0uqseRtib5rxuvBw/640)
**Genetic CNN**

**@zhangjun 推荐**

#Genetic Algorithms

**本文是约翰霍普金斯大学发表于 ICCV 2017 的工作，是一篇将遗传算法用于 CNN 架构搜索问题的论文。**文章探索了用遗传算法的几个标准步骤来解此问题，包括网络架构的编码，优秀个体的选择，交叉和变异方法等，在几个小型数据集上进行了实验，并将小型实验生成的优良结构应用于大型数据集取得了不错的效果。

用进化算法做网络架构搜索是一种很常见和经典的手段，但这类方法有个巨大的缺点就是计算量太大，因为太多的可行解需要进行评价，而每一次评价都是一个非常耗时的计算过程。所以，如何加速评价是解决目前网络架构搜索问题的核心问题之一。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhyG7M0yuNagNGJvIiczl5mrp24dVgjFf3QpWsaXyugF6sZFCxibZy8lOw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhAm5wk7ibwVQ2zmP1VEFiajUja2xibXvYczmXYic8hComDkkU8UONfogb6g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhI63wh4W4YTnW0nbm6DiaruC1ytaAKDbPJyY6KWMibiavb49bP7JvNNQxQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh6lO3DLZicmibmU0icXJFMVLPc9gHzXicIYoibaqEh9JnicSbhf3P0312U70Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhI5bQRZian3Z4VM6yS5AWvic7IpLeJEDx7ic1TTq2CJlziaQFRX5QrGtI4A/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhZFF9HyUiciaOTSNRWKKEkAwwCWtkXGLxn9h0eTcRIS82PWjtCO7r2ia9w/640)




论文链接

https://www.paperweekly.site/papers/2306




源码链接

https://github.com/aqibsaeed/Genetic-CNN







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjiaA5bbtkAnnJ2yLicAzlwmVdnAIic0THYptctQhZJRx7QYCx8TC9zwOow/640)
**Designing Neural Network Architectures using Reinforcement Learning**

**@zhangjun 推荐**

#Neural Architecture Search

本文是 MIT 发表于 ICLR 2017 的工作，**论文提出了一种基于 Q-learning 的 ****meta modeling**** 方法，对特定任务进行 CNN 网络架构的生成**，取得了不错的效果，属于用强化学习来生成网络架构中一种比较典型的做法。文章中考虑的情况比较简单，将网络搜索问题转化为用强化学习对一个有限域状态和动作的问题进行优化求解，优化变量考虑的比较简单且都为离散化，整体感觉应用意义不大。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhYic0flibkEviauhib4NGzBnqeKfrSB1HKmRdWOMqABUXIGql9kicTa1zFPQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhf9AAz4bic5JQgSibIyt4SAHs1gd8kGpQn1OWOy9fXJ76oP12ibz8SgTGg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhwgYT4HgXLzCicDtnHiaSxsoUmwEcQaILghO8iaVfhfD8hnKx9qWicXfvkQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhof7QmVbe6aW9mgJxENXpo9yYOuLF8h73pB6icdz63foWRicPticE78qOw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh7yiazPIAbBZa6PJRPt0hXnTxrcwAwC0T7LialD9yr3nc1Hz7EWS57qYg/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlh04dyF2tMiaZDb9xEmMfAWEPaj1UdWmhXicunTI9UbdKGrYTk9vQx2FFg/640)




论文链接

https://www.paperweekly.site/papers/2274




源码链接

https://github.com/MITAutoML/metaqnn







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjgzGNHdv2YBVm6bUicHjY2A8tV8hwJIiapvTYDGmFIyMclC4Xy6gD7krQ/640)
**CMA-ES for Hyperparameter Optimization of Deep Neural Networks**

****@zhangjun** 推荐**

#Bayesian Optimization

本文是一篇 2016 年 ICLR 的 workshop paper，文章内容只介绍了实验部分。**本文将所有超参数都换为连续变量，用比较经典的一种连续域无梯度优化算法 CMA-ES 对问题进行优化求解，并和贝叶斯优化算法进行对比。**本文的这种方法最大的优势在于非常易于并行化处理，用更多的 GPU 可以达到非常不错的效果。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhVWzIcXnJXIyoP0HRkwWWgIz20GWAA88ib0W7u6Z17m5e4qeGd82oYDA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhicWksLH8wcp9zWb7Wpp0HibNSgeia6YFEMK2ZoJsYgITb5U1Ut8RUVexw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhicZzg4u4Oiaic6IRiaYqOQQRV08Z5u6qxE9D9WsETVOnssoV6qkGtXNTQQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhsIXu2o5mOG0WElpsGEBxuAUFLxCXdjlDPgU3qaAVnwlsvH9EyQRJhw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhRUSu9kpEEsgiaGI5WF1ia1yQzpPORMYNpdkCwzFdlKHzlYXp0GTqbAEg/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhHA6sGzCiabgESVnrgSepZkFDWyiaJuC7Iw0dKgudoFgGJ8ibNQNYfmrCw/640)




论文链接

https://www.paperweekly.site/papers/2265







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhXUf47ic4GfJDLia8icEqZtGqF7Fy9O7GWRcGXzaHiaJ9TOpwBhL1MQrEvA/640)
**An Empirical Exploration of Recurrent Network Architectures**

****@zhangjun** 推荐**

#Recurrent Neural Networks

本文是 Google 发表于 ICML 2015 的工作，**论文研究了用进化算法设计更好的 RNN 结构**，以在更多的任务上表现超过 LSTM 和 GRU，但最终得到的三种最好结构和 GRU 的形式很像。

**文中方法的思路仍是在现有的 LSTM 基础上进行一些增加和删除，即所谓的 mutation**，这类做法相当于在非常多的排列组合中寻优，但问题维度空间的定义和变异方式的定义，已经从开始扼杀了创造新结构的动力。如何突破现有网络结构，设计出全新的高性能架构是一个非常有趣的问题。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhQCg8ibxFvkmzwySFAtdugGFgibDIszh9daN29Xhe9RxAiaZtUCHs4qiccg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhbxaHg2JMo69uhJgcEuAFKBsSSZ7iakyBV12GstcX9lxJdxm4Tzscmsw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhOvHzzuZ5wco3dISUVYV9jUcfOwIiaR3ia65HSsauaf6iagtiaAKBMZLjsQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhVnvYz6WlvJW1xhcYibTKQ7OWPAEIVib9vHCbT6pDWc30qLGCe3FM0w8g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhwIYb1XDLAqcJQtVpXomgFxrp3nNmuD2depSmhUZ4qhLujnmaFPzaBA/640)
**▲**论文模型：点击查看大图




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZhRQ8ia1S6rMnXvpWPHOlhgaC82kULZA8TSYXJicAxgU99useFOpWqgzM1Fz1MlLhs8jh29L9HPgw/640)




论文链接

https://www.paperweekly.site/papers/2308







**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****推 荐 有 礼#**




本期所有入选论文的推荐人

均将获得**PaperWeekly纪念周边一份**



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm5Wb1iaUHxx8mBh1Km3dWjfPlgYsxpxlV44icJWDVwuPorALMxCQglAC8Dx8JMeic5wHeNw29gJV8SA/640?)


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUWQfATNyq8icodseL6gFjp8w4sQ1DBTuiaChXPEcQ0Q6tmRmz2jJjzic7g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUk6ibiaGfmJl0icaK5go84z9iaLysegxS06wkEIrCkuL1eV2dicVoBusY4aQ/640)


**▲ **深度学习主题行李牌/卡套 + 防水贴纸




****礼物领取方式****



推荐人请根据**论文详情页底部留言**

添加小助手领取礼物




**想要赢取以上周边好礼？**

**点击阅读原文**即刻推荐论文吧！




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击以下标题查看往期推荐：**




- 
[15篇最新AI论文了解一下 | 本周值得读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491511&idx=1&sn=ac16f4d2ed82b87711529cbcf4f03dd2&chksm=96e9c037a19e4921d9fc043b416a7f2271aa6a594651bfe5ee46be3778193de04b9779a9f5e9&scene=21#wechat_redirect)

- 
[收下这12篇最新论文，炼丹不愁没灵感](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491159&idx=1&sn=3e3c7d554f27b6ec4d06121a9886c530&chksm=96e9c1d7a19e48c11c27e84d284ba4a9d3c7a3b2991cb907d422595ebed46e87c3c5c4f5e19e&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490599&idx=1&sn=7ab5ab73f89328901c47006674591753&chksm=96e9c3a7a19e4ab1f1d55ceb8d17a7b03622351c2f2bf49acd360b49664b3461d070e61c9077&scene=21#wechat_redirect)[暑假没人带怎么办？还有我们为你推荐论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490180&idx=1&sn=7fb37b5687869d80ea4a2accb08dac41&chksm=96e9c504a19e4c1285697e90bae1510da435f54c19961438c7062cb89b067a094c31cc08daff&scene=21#wechat_redirect)

- 
[本周份AI论文推荐新鲜出炉！真的很skr了~](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490599&idx=1&sn=7ab5ab73f89328901c47006674591753&chksm=96e9c3a7a19e4ab1f1d55ceb8d17a7b03622351c2f2bf49acd360b49664b3461d070e61c9077&scene=21#wechat_redirect)


- 
[本周最热 AI 论文大集合，还不快一键收藏？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490880&idx=1&sn=1890b0d23251e09884fc8953b273174b&chksm=96e9c2c0a19e4bd6d568daecdc3c8cbfd2d07acf39a957c79e06a6385f4767d639fb648485b5&scene=21#wechat_redirect)

- 
[还在熬夜憋思路？这12篇最新论文打包送给你](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489608&idx=1&sn=1b9384cbb3550a61901521c91aa97628&chksm=96e9c7c8a19e4ede12c934b943ef2f40df220a936bafc8e50a290f1848567412abaed8f7441d&scene=21#wechat_redirect)











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

▽ 点击 | 阅读原文| 打包下载本期论文




