
# 本周NLP、CV、机器学习论文精选推荐 - Paper weekly - CSDN博客


2018年12月21日 15:07:06[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：147


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhglryG74dIr2B1019Yibv9PAGsWGGYBiaoSGbK2kzUnbIsicCEiazKMticicR0MPtmr1ynDovFe2kGicSydcg/640)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**126**篇文章
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVJrlEYicBFxKj6dJvnCtclX355QwOn4PKUO01nakQOKMJJr3TeviaDemQ/640)
**@paperweekly 推荐**
\#Knowledge Graph
本文是新加坡国立大学和 eBay 发表于 AAAI 2019 的工作，**论文提出了一种基于循环神经网络的推荐模型，对用户和物品之间的交互特征在知识图谱中存在的关联路径进行建模，为用户提供可解释性推荐。**
该模型基于 LSTM 学习关联路径的表示，充分考虑了实体、关系间产生的序列依赖性，具备较强的推理能力。实验表明，本文模型在电影数据集 MI 和音乐数据集 KKBox 上取得了当前最优结果。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV6cqPaSibLwMxFLgwa7ph1QE7OkJaOCOF6eUkTmEzlvQPvzNVZLoAicDQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV2PAEn14xQKQYKERW5AAtD9YZSQsbQwkGQ8FDASwR3GKibalOJz8Xbeg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVJK9gvx7Dvd1BdL1iahBStfN1phmvCfjnxHicsLT6frPIuibfuupOMlD6Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV8xq44oDVDicRNLgbwjylVI9dyAvmcsre1qBNfgQHUXyLc5R3D7MgC8g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVAjKW5EfmNUn2XE1X83MyTylw4lMC5edicbn3cX1xxObTE8BCEzC55bg/640)**▲**论文模型：点击查看大图
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVOaZ4OfesJZ4ur1UFEgtAThS9wAJ7cEqoBnjSLY5swvTjf9yttVeCcg/640)

论文链接
https://www.paperweekly.site/papers/2644

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/640?)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVchRnUl2JL79iaBPmGcJhZDR01NJkevmHGUHf85GMtcXzcvEOLibh5QpA/640)
**@zkt18 推荐**
\#Sequence to Sequence Learning
本文是北京大学和微软发表于 AAAI 2019 的工作。最大似然估计常被用作评估 Seq2Seq 生成序列的效果，根据实证研究，作者认为非平滑方法往往效果较差。
**本文提出了一种句子正则化方法，为目标序列中的所有 token 输出平滑的预测概率。**在三个神经机器翻译任务和一个文本摘要任务上的实验表明，本文效果超出传统MLE，每个token概率越平滑，BLEU分数越高。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVdJtZpE3I2jDFj47kGiaMZYw6h8cNG9aibreaqRatp2WAYhH3a8ul0icxg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVDVVmnomukADfnhGFd7JKRBz9BVG7w8vDYUY2onVluQSSkIQJMQpQ9Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVMyHLGDmoficYElOEUp2Iiante4Ys9taT4IPtQ7jhFiaPl30extIKGA5iag/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVE3icHovOiakxS1G6tlMASCFutEOd4qrBtt4u4uibc5IcOGiasOXjviazSJQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVhcibibUGRx9zMQtvqAlQ8Fhv21rFFzQYK7gMWPiar26bZGWGhmYpqXb7g/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVm2l7yZo7AAQyrVf0LCzBAw8P1W5OV1oKzT77Q7E7ASFS0tYT7qCXhw/640)

论文链接
https://www.paperweekly.site/papers/2640


![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVbxmUqiaD9icWeeY71WGLWMZyhlDhwNjX0vQks1PCshvAH6NP59ZoUXOg/640)
**@zkt18 推荐**
\#Contextual Augmentation
本文来自中国科学院大学。传统 GAN 与 VAE 生成句子在一致性与可读性方面难以保证质量，近年基于词语替换的方法取得了成功。
BERT 使用 Masked Language Model（MLM）任务来预训练，MLM 任务是利用上下文的填空任务（cloze）。这个任务和短文本的语境增强（Contextual Augmentation，使用语言模型来预测并替换句子中的部分词，生成新句子）任务非常贴近。
预训练好的 BERT 模型， 可以通过 MLM 任务来实现语境增强。**对于有类别标签的短文本，为了使预测出的替换词和原标签兼容，本文提出一个 Conditional MLM 的 fine-tuning 任务。**
BERT 输入中的 Segmentation Embeddings 被替换成 Label Embeddings，在下游目标任务数据集上训练 Conditional MLM 任务，得到 Conditional BERT。将 Conditional BERT 应用于语境增强，对 6 个短文本分类数据集进行文本增强，都得到了分类效果的明显提升。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVSWeb5w1xLzCwiaRjfD1TibCzNMibE0Oemuy1z4XicBqIcwL0pvUwZYNXCQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVSI8P8mwVZMjoyysUuZkIcvA7Aibd4mEC7GXZDEFETPAHhc6NX5OvhEQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV6W8bn64qcTZ51icgu7GRuiadoIdv2yw94yb8eTib3tp27vCdkBdlZC1ag/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVyO3owcQ89IhyhoKlia9lgkm3kblSYlce2YW7sicIdDJFGkQUzDI6dgGA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVtWP78wzCTzXFey33p8HDYUiaXIDczt6fzy7VTe2LJo10WiadGJDjMJ1A/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVC22jZykPuD35ZmbjJVib0icKzotA5MGUKAD064GGvyXtzjT4XONwE24A/640)

论文链接
https://www.paperweekly.site/papers/2655



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVUiaCk1KgxJMG7bEbU0mZ8TCfnOxafFmw6n5PKRpnqRaEdoa9OgSlMRA/640)
**@guohao916****推荐**
\#Machine Reading Comprehension
本文来自约翰霍普金斯大学和微软，**论文发布了一个带有常识性推理的机器阅读理解数据集。**实验结果表明目前的最新阅读理解模型在该数据集上的结果显著弱于人工评估结果。ReCoRD 数据集为研究如何缩小人与机器之间对于常识推理的理解提供了机会。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV8YWgy3TqkzqS45yia44icOGYUINkJrAbO69U33amx7qnxfHe0sAaIc1A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVCuz3QicsqXliaIHiahdmKg7Ow3xicnht4iafKN7IjpSibyCzdneaDODk093A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVh3jibFGPrkqa4tpgPXhkRVric3zBpjDPPMDiax1kYlEQQ2Hnia2lrLuPDw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV6GaPtq14OKwfqWN8wxv6JTruW7bmibKNAJfRDh0egSdic1LMgbdaibcdw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVzkBNNlhtx6OEegSkucTgrPWExpU5m7LvT4Dxo8MU0nMNlcqFIjT0xA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVRrAvCbferPg7Psehz8fGENl04vKLpR0Z1nf0vwO5Ns61SRXhicibzHxw/640)

论文链接
https://www.paperweekly.site/papers/2648

数据集链接
https://sheng-z.github.io/ReCoRD-explorer/



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/640?)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVYOI9T7z1DgpxicgP5oayvzeic7H212UXHImGCrlqCwKfWic1mwbvGhjBw/640)
**@Zsank 推荐**
\#Neural Language Model
本文来自斯坦福大学 Andrew Ng 团队。数据增强作为一种能够显著提升模型性能的方法，已经在 CV 领域证明了其有效性，但鲜有用于 NLP 领域中。在 NLP 领域，正则化一般是对权重（如 L2 正则化）或者隐层单元（如 dropout）进行处理的，而不是直接对数据本身。
**在本文中，作者证明了神经语言模型中，对数据施加噪声相当于n-gram的插值平滑，能够提升神经语言模型的性能。**BERT 也是用了神经语言模型，并且训练时使用了 Mask 方法。而 Mask 方法为什么有效正是这篇论文的一个重点。虽然两者方法不同，但也可以作为 BERT 的扩展阅读。况且，就凭该论文的作者们的名字也值得一看。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVncoicl2gQFvGXyINXgIbhh6DicGSxFA1vkxLyMm0xRIzZY4LRjdquugw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV3G7SgeHjJ2zjwxbWeYtiaE5Uuzp8TvVokYqJEEtqoJEH4pX1qrsSFaA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVRWRsXIibKS9I21ot44Wn84MMmlpib8v5EEkupXQThYsbtZry4QErXCSA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVH3cSgUd2JCUYxMeCCofKBeGzdI5ybHfJUANLsWXlOXZ7Fc5Y75VTXw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV85yWibTUmqpLVJiatC21POTVAnUckEal3yY0MQpXNyjrVIaf5RndSK4g/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVlOwMqVvrlI849BC9AUzccSG2bLX9SrNe9z7UI99OA1dP7NfoQMSmBw/640)

论文链接
https://www.paperweekly.site/papers/2641

源码链接
https://github.com/stanfordmlgroup/nlm-noising



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0Drvm1kKqodONJWdluKYXVSiaVksJv8JyrGzSsG6O8Nt5p6aYxkA7aFuLiaQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVNQ9hRRqIsJDaIOJtWAoR616icYVd8PgaWKRmYefVHZOnUQVsL31UgQA/640)
**@zkt18 推荐**
\#Text Classification
本文是杜克大学发表于 ACL 2018 的工作。**作者将文本分类看作 words and labels joint embedding：每一 label 嵌入到词向量的同一位置。使用 attention 训练，确保对于一个文本序列，相关词的权重比非相关词高。**Word Embedding 效果对于提高任务准确率十分重要，重点在于如何使用 label 信息构建文本序列表示。
NLP 任务通常有三步操作，embed->encode->predict。本文在 embed 中加入 label 信息，在 encode 中使用关于 word 和 label 的注意力，简单模型即可取得先进结果。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV0DxxibG5T5N1JhibptGgbxuWfGGYerWWztEpcLakXPUbIGvmlHqMc8Ng/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVd3mdBZ0dSRf7XHFE6F944S8P5ESfEFDxnUCH7hySQj9XicPbqfQP69Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVicdyia6Mbr1B1FVfkLQMPnRb42J4Cdh4ERdhd38hHPxNof5X1HeZVYHw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVXxMxfGE6Fiazvm2HlfaibxLN6pkgib9tAyt3NVueLm9kgVGrOVZHkAib5A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVIDfAblFc3W6Kdq3cAOtYb6HCNn5eycnyBicvLD6VpV7icTTibRVKcJXdw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVGr6DIwISaVPCDjrtRxChCK59PhWeMuiaqOibV8NsfQuXcb3waKrRibwaQ/640)

论文链接
https://www.paperweekly.site/papers/2459

源码链接
https://github.com/guoyinwang/LEAM



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvZkYxV68zOCas9csIEy9oS6Oop2huyXBUliaHFUVHicdamRgqibegicc0aA/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVEoDrnly9nMw0t7EnbOhwdzMBEq41bickTGfGHia55ktszdXt6kutDnvw/640)
**@darksoul 推荐**
\#Object Detection
本文是香港中文大学发表于 AAAI 2019 的工作，**文章从梯度的角度解决样本中常见的正负样本不均衡的问题。**从梯度的角度给计算 loss 的样本加权，相比与 OHEM 的硬截断，这种思路和 Focal Loss 一样属于软截断。
**文章设计的思路不仅可以用于分类 loss 改进，对回归 loss 也很容易进行嵌入。****不需要考虑 Focal Loss 的超参设计问题，同时文章提出的方法效果比 Focal Loss 更好。**创新点相当于 FL 的下一步方案，给出了解决 class-imbalance 的另一种思路，开了一条路，估计下一步会有很多这方面的 paper 出现。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVT7K0bicDwtS3X3LR8BGPjOuA5Euuk00OcvTrTd8FlStzWx2ib1nv56TQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV0ibM6Luak7Fp3oND4HUlAW8YjjzFN9I2oPaqWaqpHsHkTsibMj09Uo8A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVfkA8IWPeia3UD8nUct1aBLPx6G2IV9gcJ5uV9mRCa4U2jCmLsMOkhlA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yViaib2xMVF2LgNicWAUkc0cHjLkKqo1EySRsmANqicSPKMOT6gmLMGBtLOQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVwluXTSuRdUvYLw0qX3pyrSx3MKr1C7haicPqtrkPDuusg4kVGX3PPVg/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVic3K5FNLEs9Nk94YxU4Fa7mQ0iaibC0TPlBG98oTzwtg0x5JvhFQTvlsQ/640)

论文链接
https://www.paperweekly.site/papers/2654

源码链接
https://github.com/libuyu/GHM_Detection



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvibxtiaicW0ZRIwW0Kmkj9yU90UmGicL2jnnmaBY47NYicK2d7frJAcNP09w/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV6yNXuCFxDBZ6CGhkJp4tXZAX966FFvO37rb4D0Ub7wmNYlapwnQeQA/640)
**@wangshy 推荐**
\#Video Object Detection
本文是清华大学和商汤科技发表于 ECCV 2018 的工作。**在视频物体检测任务中，作者提出了一种关注运动信息（Fully Motion-Aware Network：MANet）的端到端模型，它在统一的框架中同时进行像素级和实例级的特征对准来提高检测精度。**
像素级校准可灵活地建模细节运动，而实例级校准更多地捕捉物体整体运动信息，以便对遮挡具有鲁棒性。实验表明，本文模型在ImageNet VID数据集上达到了领先的精度。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVJQ27gkCxejJIwwBaZ8eCof71ZMLe9wvUoP6FaHRkHSYI3eibtjZDroQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVZXFb5uD6IWajiambfjicRlq8RxLtq4D4CFnnZWGsGPjIBJVbm7k9ob1g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV7cWdaa3TfWdrfVp0y67gBBk9mhVCzS76ElPNsLkvbzhMMk3c4IAqDw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV30BGX9TxlvFBq74icvfjtlaDaOK8WMzYNLGNKsoO35OgmPXicBkLG1qA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVlH7IwXia5EEeZk5SNicMq6QMdeOAVPxaboepRN1MxLpmgdBYvRd9EiapQ/640)
**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVzicTAXGrNf5U53KgWWtxzrM0gr3L3xCNtBicqiaJVEkdnXkM3wSNLibV2Q/640)

论文链接
https://www.paperweekly.site/papers/2622

源码链接
https://github.com/wangshy31/MANet_for_Video_Object_Detection



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvHib5D8hcewE9gwNibrGkW1TC8v83Y89RITicqLb5N3URaM1wGsGBV27qQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVBQd9AT1icsRQ3icgkkftOVHmA0oKgBngRM7mPseqammmTWGR73Mibk6ag/640)
**@pyy 推荐**
\#Data Augmentation
本文来自神户大学，**论文提出了一种基于随机图像裁剪和修补的图像预处理方法 RICAP，随机裁剪四个图像并对其进行修补以创建新的训练图像。**该方法非常简单实用，把几张图拼在一起，然后 label 就是这几张图类别占图片大小的比率。实验表明，本文方法在 CIFAR-10 数据集上实现了 2.19％ 的测试误差。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV3tVPq5yCJ91tDfZCLIrT1ups4nRa6xEfzIT0UZQaRmNN424KpJkCwA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVHhOHibeXqArxoZGUnlqNPpb9ZIM4sOy45wjzjVCEveWkI3IuJC947sA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVcnibibOfzyQnNN2kPHEIsvk8wc80ND7ibB5K3Wq6ibhh7kU2cKg9aIzriaA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVNlu2dZbaWmI8hiaQ2akiaZ94wFpZBxKVJGXvPwgLdqHiaXXibCvVyRLvcg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVRqgDfTSJAuDqlPDYtMY6MM0DqZ1JCX6vNT3BG2PZrDtUvUDNa6buyg/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVIibOm1iaKWwcg1RJLbPuZ9BLeCQ7vrdXGCw8GsPyGDDAWsDQbq7dH0Ng/640)

论文链接
https://www.paperweekly.site/papers/2645



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8tiamiceTcrbl3UY25cTHibSgtJNZnMBCOUdcpTpSLK45Ya9RC8yDZsSEw/640?)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVtDQ3odyicrPoCgVPjxcGMT5V8mnzZgSJfDwknR7j6jGnvsLucqhHrng/640)
**@Kralkatorrik 推荐**
\#Image Classification
本文是清华大学和 UIUC 发表于 CVPR 2018 的工作。**这篇文章假定数据样本在特征空间服从高斯混合分布，使用 Gaussian Mixture 代替 Softmax 做概率表示，用交叉熵损失来做优化。**除此之外，遵循约束类内距离增加类间距离的思想，引入 L_likehood 损失，并在 GM 中增加 margin，构成了最终的 LGM Loss。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVGIIRAourOpg1VpNkY7N2ViaDvkZbJwrIicupaRWhcvdBbk8Byz0iaW9qA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVN1aGMYv4g7iazUjicVaeejeFwgu8TrSVicz0NW1Gn55qXUnmINY9hbc9A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVVuV3jibb0feEMhfHxOL6LROfUrl9nk9Nh8tmnVhzMw1YpIHL6L4RcgQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVZn4iabRtYibfVqfcSicyC3YyMZ8v424KTsicGBylTPIeibK7e7AM6EvzyzA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVpKVM72EciaKY1UVawmQkMSvqJ3ZHdHbu7J3K1pxvlEsOMWvC941XdXw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVYOiayXricq6gicZCFbuje0ElGJTQ5y8PZMQicz4F3n43hu5U2RdMZ30rmw/640)

论文链接
https://www.paperweekly.site/papers/2619

源码链接
https://github.com/WeitaoVan/L-GM-loss




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8IA3BMnKpHmwoB8kAc8CQC4UOSu2G0c5vFM7xpJZOcqLdFHch97tiaGg/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVfepYyBnUB0xibDfmlwHpvOiaFeje3v2rNN1nIJiaXRibIoTxVicK7dUdcEQ/640)
**@paperweekly 推荐**
\#Human Pose Estimation
本文来自电子科技大学和 Vision Semantics。现有的人体姿态估计方法通常更侧重于设计网络学习更适合的特征，从而使得定位精度更高。往往容易忽视在模型实际部署时，可能面临模型较大和计算复杂度过高的问题。**本文基于一个轻量级的 Hourglass 网络和知识蒸馏，提出了一种模型复杂度更低、模型 size 更小的全新方法——FPD。**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV6gVenyQTLEeNTyRjbKqAXriaXVcv9aIY9Gr1sQLlOicpp1F2GNqoo7XA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVKSUqcibGyyqocSrVBTqe3oK0HDA1sxE3EYgnEp30Gx8KbNnMI0ib8W3g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVr1zvHiafAY0Tm2HGZ4nNlu9yZ1QHZJh1xwicQOxuBukEBDWkmHcVicVtA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV5QWkPqIic49TFoB0KOmE1Bp7Og60wRvbrUgoibJ3e0TLU7ArQbKEiapVg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVRwfAoY5vBZENl7PoIoF3Hzkodnu9bxfCGmF2WXs5JzYU95IhGrPqqw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVPKAQngmSzHl2kd8RValoc3LNK9rziaUzpIovsJ9vuIATJ8oJExickxDA/640)

论文链接
https://www.paperweekly.site/papers/2647



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvSrUEOribtWtcbc5Bs8icSOWQPFxgpHLCrooqDs1LNC02qthicqiaUiaLzeg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVnuoicPaLGcMJ9fCUEtunZI5SexHIaBw1V1lic68xctjR1VcInFiayfwow/640)
**@haonan 推荐**
\#Generative Graph
本文是斯坦福大学和南加州大学发表于 ICML 2018 的工作，**论文关注的问题是基于图的概率生成模型，作者提出了一种全新的深度自回归模型 GraphRNN。**写作动机如下：1. 因为需要表示 node 之间的关系，所以 space 大；2. node 的表示顺序多种多样；3. node 的生成顺序需要依赖已经生成的 node 顺序。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVDwtpgRrW5TA3TJ6wib3TrdpiaNw1SG8QdMiatk9eLxwPiaicT6QgWdbr7pQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVzOQAFEHl3ALLicv9sqlbhHs4OJqJBicmvJvkiaEhIfFyWXQNH1ORovXIw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVibKHODrY6hz2l0JN3FXkVBbfNWOjbCC8kTRYAEfPSuj9nWgkM5AX5dw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV9OUDyibFXxx3h4eBEW5ic6NnGMnBk91vZXEaOjjYglakzgKYOsBlORRQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVvQ4eKkQCGjUI0lMK8PUuiaZQL2jHiczHz9tMqlfCuHpyv3rZyN4KNHhw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVA6GiclXVfD34Y2tJRTLtK46A3UqpVdA8dc8MWmMsa7ufht3cNPjKDvw/640)

论文链接
https://www.paperweekly.site/papers/2608

源码链接
https://github.com/JiaxuanYou/graph-generation



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUMyCvJ7nksObSLatO1UHuTLcw5KYWPhREehBpXWY0uqseRtib5rxuvBw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVaZwZnkQiaJdnAvOvr6vqbZgiaV6T5lCicffmXOBRR3kQX5ScJCxK6MjbA/640)
**@xuzhou 推荐**
\#Time Series Analysis
时间序列聚类是根据序列之间的相似性或者特征进行分类。之前的方法通常是将特定的距离衡量方法和标准的聚类算法结合起来。但是这些方法没有考虑每一个序列的子序列之间的相似性，这可以被用来更好地比较时间序列。
**本文提出了一个新颖的基于两个聚类步骤的聚类算法。**第一步，对每个时间序列进行最小平方多项式分割，该技术是基于逐渐变大窗口技术并返回不同长度的分割。然后基于模型近似分割和统计特征系数将所有的分割被映射到相同的维度空间。第二步，对所有时间序列对象进行组合。作者考虑到了算法自动调整主要参数的特点。实验证明了该方法的优越性。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVVyWkUoPibps9gLJP64j0nQZtkNocMmOSAfS7DNCZv3rjSxUXibhZ1tPg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVHgLTu7FHYsUVKFTibGp7s8Q86juh2W2diaRicqDia4hmnFJ3MX2I0kkLSw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVIjjstsWyvHLCPEcpRky3zWoHKnsSkaKsxEZ1NdLSYBFCqAAKxgKL5g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV2de7DSI0sicDOonicXr9HyMicaZx8JxDEDP9wjmlaV5EhroYlfQ1gKUSQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVt6XibEibNtBHvrC9ICo2EIUEROmtQGkkEz0pZIibwRZiaBKknj3zl091iaw/640)

论文链接
https://www.paperweekly.site/papers/2505



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjiaA5bbtkAnnJ2yLicAzlwmVdnAIic0THYptctQhZJRx7QYCx8TC9zwOow/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVvm665BN3CgQ8nA5LMQEswdxc0NXyFNOgCWcWRZhrgPYO8VpDRPlSkg/640)

**@paperweekly 推荐**
\#Recommender System
本文是 Schibsted Media Group 发表于 RecSys 2018 的工作。**论文关注的问题是推荐系统在二手市场上的应用，作者通过在线实验对三种基于深度神经网络的推荐模型进行了基准测试，对比它们在生产环境中的性能表现。**这三种模型分别为混合项目-项目推荐器、基于序列的用户项目推荐器以及一种更高级的多臂bandit算法。
结果表明，在冷启动和基于序列的模型中，将协同过滤和内容特性相结合可以更好地表示项目。此外，作者还在其他推荐算法的基础上，将bandit作为更高级的再排序工具，这种方法对于利用上下文信息和组合多个业务领域的推荐系统非常有用。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV27FAc947aq8yNJW7mQAAhIOicMJqHbXCIl0Kxwm3NIBbcIqV9FmnbzQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVibz2vbL65g35YwSUFzIBB2qBJe4zXCFUFzciaEUgJBaLYZLQSAJ8Qcew/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVNXUibNeibcIwXUQd9raX09As2cx5iaLhNO5PjnrpmQuvCdLCia8Axu3CKA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVKMSDIbicXQnFMDbGlibosnJSYOUQbc5mh7tBic5ibB1gBqhicLbV5UkRP3A/640)

论文链接
https://www.paperweekly.site/papers/2646



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjgzGNHdv2YBVm6bUicHjY2A8tV8hwJIiapvTYDGmFIyMclC4Xy6gD7krQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVmWokRkEjT6DGCW3HsrMq0BwCeS4BYyMiciau0lMHgSXUKfYQlUwlSTWg/640)
**@zhangjun****推荐**
\#Evolutionary Algorithm
本文是 IBM Research AI 发表于 NeurIPS 2018 的工作，**亮点在于将 SGD 和进化算法融合起来，提高机器学习优化算法的有效性。**
SGD 是一种常见的基于目标函数梯度的算法，常用于机器学习和深度学习模型参数学习，优化效率较高，但容易陷入局部最优解；而进化算法是一种全局优化算法，只要进化代数足够大，总会找到全局最优解，但计算效率相对低下。基于此，本文提出了两者结合的优化框架，并在 CV、NLP、ASR 任务上进行了测试，验证了本文方法的有效性。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV5xFbarLZPqGRGAQ1qpOMXjuMwzHSyXGiahTYiasHDUtsjpvnlWya0yIw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVePZsibrVUCZNmcsd5zBOx7mzQtiayebJWqBjiclpsfNxgP5guNgVuzoTA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVdcdyNbpvL4uhvaCWloY4iazvj2jJcUjNzeo0iboZCVvz8ckiatUDKscJQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVrf2ibHbqibYTV8zVHcNdOpKyj2ITvFLnnOkrVyY6uwCB1vB8v8sT5CVA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVCoBibXv0RKvEJel1R8GicACwIGibEG0siamuGENzorFXDicJhl3dVuGOqGA/640)

论文链接
https://www.paperweekly.site/papers/2615



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVgicbLfFfFUq5D0tPfYTibSxD2ZvWJFMCnVZJ3UvpE3V635rO33PkQkIg/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yV5aFoD3bXAe0cEfywZXgunvlQvicVQ0CG5CHLuco5K4Cu9END6zQPR5g/640)

**@ZSCDumin****推荐**
\#Recommender System
本文来自南洋理工大学。作为个体的用户和群组成员的行为是不同的，**作者基于用户评级历史的深度学习技术，提出了一个注意力群体推荐模型来解决群体推荐问题，模型自动学习群组中的每个用户的影响权重并根据其成员的权重偏好为群组推荐项目。**虽然基于图和概率的模型已经得到了广泛的研究，但本文是第一个将 Attention 机制应用到群体推荐中的。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVKia6BmaEof6WudeEJXsdp7JUqpgVxF0NfnCukc1aMxAjibdJ4GCeNQIg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVI55gGfextym0wUVQOQjpj7NTpsYTqQIzgUP2ppECFqmxR9icQnHiaNJA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVd6wY0Pbbw3re1SfciaPukrt7XLQZIw3n3clCG7wzrJ6BznUP9LYo38w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVwhEvJHnu3C6zF2HrFXbVMzMib0jXsGP7QahmsxthHDRkW311sxrWibAQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVnnqtJlcPUzM3w1bich0UfnKcib4g23sXc3PwIJPksTBTB7A9kCXibWTow/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmjzJtCDOe8XLVgMWs5E3yVZ7F2oj4Pdy90kXfJC5aURc1zKSZibIP8mEO6A1dIP0ftCic3s5uBN69g/640)

论文链接
https://www.paperweekly.site/papers/2561



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****推 荐 有 礼****\#**

本期所有入选论文的推荐人
均将获得**PaperWeekly纪念周边一份**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm5Wb1iaUHxx8mBh1Km3dWjfPlgYsxpxlV44icJWDVwuPorALMxCQglAC8Dx8JMeic5wHeNw29gJV8SA/640?)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUWQfATNyq8icodseL6gFjp8w4sQ1DBTuiaChXPEcQ0Q6tmRmz2jJjzic7g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUk6ibiaGfmJl0icaK5go84z9iaLysegxS06wkEIrCkuL1eV2dicVoBusY4aQ/640)
**▲**深度学习主题行李牌/卡套 + 防水贴纸

**礼物领取方式**

推荐人请根据**论文详情页底部留言**
添加小助手领取礼物
*每位用户仅限领取一次

**想要赢取以上周边好礼？**
**点击阅读原文**即刻推荐论文吧！

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击以下标题查看往期推荐：**

[又为写作思路熬到秃头？16篇最新论文打包送你](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492302&idx=1&sn=1efef6309e70dedd9c5380cb644fa4ea&chksm=96ea3d4ea19db458e7ce12f066e4e37c137ec67fdd5f7e851823660f9dccbd23bbad1ce2a255&scene=21#wechat_redirect)
[收下这 16 篇最新论文，周会基本不用愁](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492583&idx=1&sn=85ec5352079218745428d66ab8ee97d4&chksm=96ea3c67a19db5718f5412c64f4c11d28cab5eda2826350fd5f15ac3e888f6ae7a9137eb31bd&scene=21#wechat_redirect)
这 16 篇最新论文，帮你轻松积攒知识点
[本周份AI论文推荐新鲜出炉！真的很skr了~](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490599&idx=1&sn=7ab5ab73f89328901c47006674591753&chksm=96e9c3a7a19e4ab1f1d55ceb8d17a7b03622351c2f2bf49acd360b49664b3461d070e61c9077&scene=21#wechat_redirect)

[想了解推荐系统最新进展？请收好这些篇论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491818&idx=1&sn=311962e2e41119a565c252a19037dd76&chksm=96ea3f6aa19db67c3fbfa77fbec65797d0ccc8f2930290d57c2016a3e55a8bb18b77fd10180b&scene=21#wechat_redirect)
[论文多到读不完？不如看看我们为你精选的这15篇](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493452&idx=1&sn=93c7cc02af605e3d8b86fef6ec2cee26&chksm=96ea38cca19db1daddf658a56311c1da5448eabce36d7986598d80c5f02ed91d335fab1ba8a9&scene=21#wechat_redirect)
[本周有哪些值得读的AI论文？进来告诉你答案](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493220&idx=1&sn=b4c88ecfb42d081935c6deb548c681af&chksm=96ea39e4a19db0f26aede4b9e1e57208e97edfc50f736f0d4aaedaaf363939b774ac78f9ad35&scene=21#wechat_redirect)



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


