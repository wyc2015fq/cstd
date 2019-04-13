
# 本周份AI论文推荐新鲜出炉！真的很skr了~ - Paper weekly - CSDN博客


2018年07月27日 17:38:50[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：758


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhglryG74dIr2B1019Yibv9PAGsWGGYBiaoSGbK2kzUnbIsicCEiazKMticicR0MPtmr1ynDovFe2kGicSydcg/640)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**91**篇文章
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?)
**Sliced Recurrent Neural Networks**
**@Ttssxuan 推荐**
\#Recurrent Neural Networks
本文是上海交大发表于 COLING 2018 的工作，**论文提出了一种对 RNN 进行加速的方法，相对标准 RNN 其可以加速达到 136 倍**，如果针对长序列，可以得到更大的加速比，此外从数学上证明了，RNN 是 SRNN 的一个特例。
SRNN 把 RNN 序列分成每份 K 份，这样每份可以各自运行（并行），得到长度为 N 的新序列，后续可以新序列上继续如上操作，一直到序列长度等于 1 或满足需求。通过在 6 个较大规模的情感分类数据集上测试，相比于 RNN，SRNN 都取得很大的性能优势。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkpFdbsBAIRVO3T3FKwN1fYNe1oTicuN8XqGry1NkhRicgBMlbBOWDxxAA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkUa4j8hqb9ZbZZrNWMQJgdTlMWIE2TL1XicMQjIusia3pknS0YYz3bWBQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkvW52QzmklgwaUgKeDibEcgiaMficmibkicuicH632WTB4lZlv9AnHyhscdcw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkY6CLH4nIyAaMMs4jWZiaVOibLKGEiar0X4Xoib1hy0ibExZeMV1Jwes7uYw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkFEeEosVMKdDicnicBsymRlvYFLnJeDpPIibVG1SiaxjiaCWHIbu5Ijkh9tQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkOTyZmll7dBYrKN9U9P4C5mcLj3zQgr8ZNYdK84cRFxQUwakMrkWW2g/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUko1icO1JjbZCY9PncoCtcqpkgsZ1N1hknDOics1tlruTppNvDiaxzP43QQ/640)

论文链接
https://www.paperweekly.site/papers/2143
源码链接
https://github.com/zepingyu0512/srnn

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/640?)
**SentEval: An Evaluation Toolkit for Universal Sentence Representations**
**@pwathrun 推荐**
\#Sentence Representations
**SentEval 是 Facebook AI Research 推出的一个用于测评 Sentence representation model 性能的 framework**。其中自定义了至少 13 个任务（后续有添加），任务数据包括了文本文类，文本相似性检测，NLI，图片摘要的任务。其着眼点是目前 NLP 领域内不同模型的跑分不能很好的进行复现，进而导致横向比较模型性能比较困难的痛点。
任何已训练的模型都已放在 SentEval 框架内，SentEval 会使用目标任务来对提供的模型进行测试，因为测试环境统一，以此获得的评测结果可以和其它的模型的测试分数进行统一的比较，而不是只是比较论文中的纸面数据，有利于复现相关模型的真实性能，而且是相关模型针对不同 NLP 任务的性能。
具体实现是通过自定义和修改 SentEval 的固定命名方法（batcher ()，prepare ()等）使得 SentEval 可以将目标任务的文本导入模型，获得向量输出，进而进行相关任务的比较。比如文本分类就是获得向量输出之后训练一个分类器来比较分类的准确率。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkuLKORA5Vibc4GnYbsjGD5Eed00bKZLyAKUemlOyHW6ag9a7pxjecx2A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkjMPvogwvxpjc5QFTPwEnwB7ZeCu3yOh7yDMIvQ6ibkZDiaqlATwerpKQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk6wBUwegAr4vC1EEzpAIQ9cluZAh4n7r27S9hkUpo6Ly34S4U5YDicZQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUko7EtMePKJXNGZib9ISPiadEEA1Q647jT3D9pAj9B7uPTPvIjI8ZG8AVA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUka0kx6icFsmY6KmuphncicYmERqpiaONVTcy7XdvdIdjhSiaZvOeYKZxFTw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk2pICpk8BIJ1dHXOzp1zI5xFjun9NoVopLL1jIsPJO69Y0tPtDIhGVQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk4ZX9l5QNIW6ic1AQwE37kEgKeNWgRlBOLm7Y9UXA59BstG95PoibDBnw/640)

论文链接
https://www.paperweekly.site/papers/2105
源码链接
https://github.com/facebookresearch/SentEval

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/640?)
**Learning Chinese Word Representations From Glyphs Of Characters**
**@mwsht 推荐**
\#Chinese Word Representations
本文是台湾大学发表于 EMNLP 2017 的工作，**论文提出了一种基于中文字符图像的 embedding 方法，将中文字符的图像输入卷积神经网络，转换成为词向量**。因为中文是象形文字，这样的词向量能够包含更多的语义信息。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkgP0tknrQ669B6LuxNY0FHkwGf9Vyrzp0KOpa0NQkcrbxsOHXVlSPXA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkemllksjSkSSicU8EVUPVq9ZjVic2jBBRuiaQcwAsCNvEbJGgoCcf5rKfw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk8fCVa4iaSseTb8UKyicx8lC0FhicQ5WHEia1dzxNeOT9uCmdelj3g0iaeeQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk6iawKy78QPIib2NDnsWicgk2jajBQp7rj7ZGico0XQgua67IqfqD2ufFicg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkQ53NaFPLxX9ZnibcEEdIpicuWSxHp1UFPWticlibcHPItoB87NNbINXwAw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk1BTxcKcQ50FRiboibT4QEshKdzoywiaTqN2ZNZkydxEicAg1hnjUpaPfsA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkpm3yqcuNwctZcBB2ick9DQdR3qcCx0oB3LuEIvoZbib2GMaX3L081EMA/640)

论文链接
https://www.paperweekly.site/papers/2141


![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/640?)
**Competitive Inner-Imaging Squeeze and Excitation for Residual Network**
**@superhy 推荐**
\#Residual Network
本文来自华南理工大学，论文在 SE-Net（Squeeze-Excitation）的基础上，根据已有的残差映射作为恒等映射的一种补充建模的论点，**提出了残差和恒等流进行竞争建模共同决定 channel-wise attention 输出的改进**，使得恒等流能自主参与对自身补充的权重调控。
另外，**本文针对网络中间特征的精炼建模任务，提出了对 channel squeezed 特征进行重成像**，将各个信道的 global 特征重组为一张新的 feature map，并使用卷积（21，11 两种）操作完成对重组特征图的建模，以此学习来自残差流和恒等流所有 channels 的信道关系信息，将之前的：1. 分别建模 chanel-wise 关系；2. 再合并建模残差映射和恒等映射竞争关系，这样两步融合为建模综合信道关系一步；称这一机制为 CNN内部特征的重成像（Inner-Imaging），探索了一种新的，考虑位置关系的，使用卷积进行 SE（channel-wise attention）建模的 CNNs 设计方向。
本文实验在 CIFAR-10，100，SVHN，ImageNet2012 数据集上进行，验证了提出方法的有效性，并发现提出方法对较小规模网络的潜力挖掘作用，使其能用更少的参数达到甚至超越原先更大网络才能达到的效果。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkCQULrKiaIkIMfa5G3Jgemx37SRoTy4pftsb3SIE3QFU7kuJFQDia7yOA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkZqakvvjiaHk1Bic2LzYu63DwUicnIbBGmvlIDVc8C9hYHrribYrsWiccWRg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkXbzwiaFzO2RdWmzgH01hq2ib7icURkRga1efxh09qyv9wkrvjpVDUmPnQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkz0LIE6BLqTH9lT8Voe3XJQ0bQ8UahSXpo9oeyoYR8526ReyvSahs1A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkfAfVkvn6BGSxUxeeEAQiahcsQ3H7BkNEFdsPbuUJqgktbGNrPsGC2LQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkUVEq4PpUGYgn4BgwEy3szvyJmebibZHcWQoDysEiav9mFHicfzibHEdzMw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkSsHXO4WyBxUxM9MddLEuIzKicw5yHjdO6gYITV5TrSQHLiaic2l5y3MZA/640)

论文链接
https://www.paperweekly.site/papers/2161

源码链接
https://github.com/scut-aitcm/CompetitiveSENet


![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/640?)
**TequilaGAN: How to easily identify GAN samples**
**@TwistedW 推荐**
\#GAN
本文来自 UC Berkeley，GAN 生成的样本在视觉方面已经达到与真实样本很相近的程度了，有的生成样本甚至可以在视觉上欺骗人类的眼睛。区分生成样本和真实样本当然不能简单的从视觉上去分析，**TequilaGAN 从图像的像素值和图像规范上区分真假样本，证明了真假样本具有在视觉上几乎不会被注意到的属性差异从而可以将它们区分开。**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkhldtAv0wibgbvsdHNawGMq0lLY1wsAKZoTdym35qia2xKsNuq1wgib68w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkHstYw0rXjfJsDgvrpHIML8ibibTsbbL2icN1qapEV0ibbfTsLu2yXz9quQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkhP3kMXxLW8lIdLI5wpcMwVwmHtgzEwyQ21sicE2GWPN4I7BF8MHib06Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkSKylURSmJCyjaM3CJHkawBsfd0fOfxibsdaZGPdr3O1AEsY28rq7otA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkSlrjKlAZuuODSR6w9raSL9ZmopiaUia5JwibryOUomChAL5TxXZ0uV71w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUklV9uYKg5RzqAxSPBmagb9BELntg18vDIUl77gO8WG91HJiaT8mQMvXA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUko3nSicic5mFf9qtzaxKDpDH2Q8xe8yBJ80TQD0kOPkwVrubibqf4mgkiag/640)

论文链接
https://www.paperweekly.site/papers/2116


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0Drvm1kKqodONJWdluKYXVSiaVksJv8JyrGzSsG6O8Nt5p6aYxkA7aFuLiaQ/640)
**SemStyle: Learning to Generate Stylised Image Captions using Unaligned Text**
**@born2 推荐**
\#Image Caption
**本文提出了一个 caption 数据库，特点是能够生成故事性的描述句子**。相对于传统一般的 caption 任务，这个新提出的任务，生成的句子更具有故事性的特色，而不是单纯无感情的描述。为了解决这个问题，首先需要构建一个数据库，相对于一般的需要人工标记的数据库而言，这里采用无监督的方式，直接使用一些既有的自然语言处理的技术来解决这个问题。
作者提出了一套框架，以此来构建从传统的句子到故事性句子的映射。为了方便训练和解决这个问题，作者提出两段式的训练，从图像到关键词，再从关键词到故事性的句子，避免了标注，方便了训练，值得一读。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkDPvNPqYGSZ6ns5XNzKVnCIYvXaHdP7QI58DHgHyz1EG2fkW2FGVF7g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkY4HLLudyk8QCaJOMQrv3l8sNNvCDkfyGEQrmibRVMSBKHFUiblIAtuLw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkyUibA1loYfGJibHFYyicaoVT5icHREL7Tna4zeXhXU3fzxrIejjb3CB0icg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk5OsmjRS06KqTCogqdM7kK0EXACdDIzuia7USqqJVc5mjZMW9lXgpZ0A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk11yrghsnswpJPdxajQ0Wx7qhkwSNJhyL7ODJjRPkH7Op1Ncn7GKe9g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk4JyvFCnvxBOOetgBSicHNv6Agh4rqg8cy0qKRfTMtVQOpLTbIVMwQRQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkNzQ9ID1EZw5wmicK3DrGOIU60h4QycrSzw6cWia5WxBxVDCLTwMBOAEw/640)

论文链接
https://www.paperweekly.site/papers/2160

源码链接
https://github.com/computationalmedia/semstyle


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvZkYxV68zOCas9csIEy9oS6Oop2huyXBUliaHFUVHicdamRgqibegicc0aA/640)
**Log-DenseNet: How to Sparsify a DenseNet**
**@mwsht 推荐**
\#DenseNet
本文是 CMU 和微软发表于 ICLR 2018 的工作，DenseNet 是 ResNet 的改进，**本文在 DenseNet 的基础上提出了一种较为接近的但同时更少连接数目的 skip connecting 的方法：选择性地连接更多的前几个层，而不是全部连接**。相较于 DenseNet，本文拥有更少的连接数目，较大幅度地减少了计算量，但同时拥有较为接近的 performance。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkAmHhrRwTpNQhWE0TwO5Bk439Tv9yibzy1K99YtkZ5TH9CY0ibWtgKGsQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkzTsv15NjL1eoib4PNJw7n1LrrnH5fCvLASYBWRpglnib8fbk2h05wo9w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkUcIZ08glHZKL37DoRsxVCU6Sicj8KxA4OFp7SJcXEmiatq1CHM50RGpg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkV9icfmNhA88V9PIicpPOicJVNrrC5nhtuTT7E57sjTfJeVJmWPnibI3aicA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk3l6Vu1Egm3sc7zicZJpVAEo5NiaOVxJFJBickbt0oJD0aTqz86d1ub4Sg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkBrzd17hO2LdcvibCDhwK46UiaT2mP4dut0mom51GZ9a2rice2Kdticm99A/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUknG9AjDbcWxrn8lOSJNeCmErrX9iclNnickuibC2Pl4d5kEBZxUWrWeZjg/640)

论文链接
https://www.paperweekly.site/papers/2103


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvibxtiaicW0ZRIwW0Kmkj9yU90UmGicL2jnnmaBY47NYicK2d7frJAcNP09w/640)
**Training Neural Networks Using Features Replay**
**@jhs 推荐**
\#Neural Networks
本文来自匹兹堡大学，**论文提出了一种功能重播算法，对神经网络训练有更快收敛，更低内存消耗和更好泛化误差****。**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkWMpODCSzQibWicuq5zInAaznDQf35bw6e8k6vrCAgxNbxsyEwZiaEGQOA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkeksmJFbCLKIdzmVtF94VTvrsoKcERcbUoGWpwrTVt5ZFich66j5xcoA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk6dJN4uNXKcwKvibIVrGq1TFJ1S08EFSahK6yGLibwfhwBVVJQAw9rV0Q/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUknA8g060A3hzGib0NMicx5S2a6yLfDs1ekQTU5MKgL9YicslboDhVJaSNw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkwzKR5JvPn2qz0Yia0wngicZmZNz7QJqr1xM21oFlvyxR5ibGEfibicgs8eQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkjwghghn8v8DjWcSxBVqHkU5bAxfJb0Svxogmqib2qubIWPgvib35rUgg/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkLJJfRtMbZYd3vFdBcuesH6iaaKOsquFHe24pE7AQqkyO1iaSqehA8h1w/640)

论文链接
https://www.paperweekly.site/papers/2114


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvHib5D8hcewE9gwNibrGkW1TC8v83Y89RITicqLb5N3URaM1wGsGBV27qQ/640)
**Learning to Ask Good Questions: Ranking Clarification Questions using Neural Expected Value of Perfect Information**
**@paperweekly 推荐**
\#Question Generation
本文是马里兰大学发表于 ACL 2018 的工作，**论文基于完全信息期望值（EVPI，expected value with perfect information）架构构建了一个用于解决澄清问题排序的神经网络模型**，并利用问答网站“StackExchange”构建了一个新的三元组（post, question, answer）数据集，用于训练一个能根据提问者所提出的问题来给出澄清问题的模型。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkmib0rHIHLic5DX423mUWm1sbyHXztkVYxVeqLr8k9N0g3dsMbKzlfLkg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkkrOg0UrYzEYLQXyelvicaIvLGn7fFstCAHpicicfjx83XAOic0BVJtXE9w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkM4ahUkYZdMSpiaF3eVVzRPAl3jy9p6ibcV3cIK7hEPiciaFyBDic1xBZ6qw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkqVuzMewTXBUsOAarvKTngibAITZ6sQ5b2rQVxJZCL12Jxfwqyos2Wpw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkmRnTUNM852OulmUxetYQ6gLTibRpjW2JAkVFkAYVvAIWotnNsqkXlmg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkXpYLpS3Nc2EJnc006qkE5Ng6sbwqOGicpHqibcpwCYZrVt2vgOS8nNkA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkLBt50nll9BO52ibibzTURXvlNjjUaWjkEj0QdiaCjdygwkB1aZXN49pUw/640)

论文链接
https://www.paperweekly.site/papers/2146

源码链接
https://github.com/raosudha89/ranking_clarification_questions


![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8tiamiceTcrbl3UY25cTHibSgtJNZnMBCOUdcpTpSLK45Ya9RC8yDZsSEw/640?)
**NCRF++: An Open-source Neural Sequence Labeling Toolkit**
**@PatrickLiu 推荐**
\#Sequence Labeling
本文主要介绍了神经网络序列标注工具包“NCRF++”。**NCRF++ 被设计用来快速实现带有 CRF 推理层的不同神经序列标注模型**。用户可以通过配置文件灵活地建立模型。工具包使用 PyTorch 实现。解释该工具包架构的论文已被 ACL 2018 作为 demonstration paper 接受，即本篇论文。**使用 NCRF++ 的详细实验报告和分析已被 COLING 2018 接受**，详细论文解读：[COLING 2018 最佳论文解读：序列标注经典模型复现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490099&idx=1&sn=2d2497999186b979dd557fe3133b7606&chksm=96e9c5b3a19e4ca550a7ae55705af84e941b1aba14cb21f3f2ffc366df837d387575f8529cf2&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkRjp1n3yicTOIgHWL5dcHd3CfvXsRPCtCQxwAiavF7fXic1lU3mWtiabApA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkMmEFbQiccVNwpYm8E3dAtsT0OAFGEgtib5RE8ibLjqceNsyBeFUe4GFvQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk7z75jDOgibgoWKhIP7f894ukhlG85QcpibZyHuorLHsAX1pHtRMs0oWw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkdpqwCxvEDfZBSPYkqKLyCBibolz3p0BFlRicJYBWxIXlliaiayGtOYoc9g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkM3icd2RWIJHxOPb0V7CKF1pH56sm9LmiaqRUSAnxCZL2LgXmoqolLo3A/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUktSCqpRIcrEge9rDiaMMlXCJSMqWqiaSQs5jHq5PVJ2WA1KnPYjtCxxeA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkAqV2vngUibvtoXWDic6ibic4LBQqFCsOhppMzAbHd6GvsrQPQicIVU0lS3A/640)

论文链接
https://www.paperweekly.site/papers/2138
源码链接
https://github.com/jiesutd/NCRFpp


![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmqMicvB9tX4H6dEJbe0TLM8IA3BMnKpHmwoB8kAc8CQC4UOSu2G0c5vFM7xpJZOcqLdFHch97tiaGg/640?)
**SEE: Towards Semi-Supervised End-to-End Scene Text Recognition**
**@jxlxt 推荐**
\#Object Recognition
**本文设计了一个端到端的半监督文本检测和识别模型，通过在 SVNH 和 FSNS 数据集上验证了该模型的 work**。文章的模型不需要提供文本检测的 bounding box 只需要提供正确的 label，然后通过预测误差反向传播修正文本检测结果。
端到端的模型 loss 设计困难，通常识别只专注于文本检测或文本识别，但本文使用了 STN 来进行文本检测结合 ResNet 进行识别。先通过 STN 检测文本位置，输出特定区域的文本图片后再通过 CNN 识别文本。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkIYxGOx2SKcx3DHuoUSOYRQvWeHzM7OCv5SOCq51RicbHpGMJOmGPRSw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk2GoRbdxWBICoH0ZeRmAM8kto4c6FkjiaguS73ia0ia3IpVPyr7hVurOMg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk4o6a3eAslxRuoCqg6OYrQfia4BYRPLsAypgLs5gqVzUb3mH8vRItWaQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkSSKGRYV8f3jiaOibkrveQPM7kL4eGbJPNYFsLo8cLmdekNH3SGAqsHHA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkgicWJmJiaFHQIiamV0Zqg7gYHqJIUKYDpk0ibJ9kBia0Q73TR9xdg66KdpQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkT3sQCFqfjpUDaw3m2rfhFq6JEGzvfGCWcSkEdnkpnSaArUsxj8JXZA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkKyWmnpSEnqNdogCxqlMbbvu516kIke58QSUoczm4K71QP2xGCAeiaOg/640)

论文链接
https://www.paperweekly.site/papers/2113
源码链接
https://github.com/Bartzi/see


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmnj5HVR9ickEOHxUiaKM0DrvSrUEOribtWtcbc5Bs8icSOWQPFxgpHLCrooqDs1LNC02qthicqiaUiaLzeg/640)
**Inferring Semantic Layout for Hierarchical Text-to-Image Synthesis**
**@jamiechoi 推荐**
\#Image Synthesis
**本文是密歇根大学和 Google Brain 发表于 CVPR 2018 的工作**，论文首先从文字用一个 box generator 预测出物体类别所在的 bounding box 的位置，然后用 shape generator 在 box 的基础上生成更细腻的 mask，最后把 mask 和文字信息输送到 image generator 进行图片的生成。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkKap0mYkRnX8frG4oYrXyricEqkL5fPJGKOAp9gFcsp16oclUVSpR3Dg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk4qo4ZjeYj9OHQhiaRlyCKs2VAMYaG2Ir6cWT4icwJkSrq64DhjJ81SIw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkq54wPwVjZjzNPYN1LukMqGWngoJfwJ8kic1k6sq1VdEZaqkkbLSBcJQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkwm8FUOBJXIlHG2xSMScgia4PRFbCKMSrhjriapEiaf7CKP3DgibIiaYSdAA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkUuUO5C11MxoNd7qPhggq3LqiawWnCRQNnmDgcgUkrvhGrrqiaWiaTQX8g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkyQHeHvjoVUSgfibcF0LmFM5icFMLKDtISoibKSnFGMyW7PZicqZrTZrGOQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkiaBVeJRxViahxZmu0r8FOKsxSZf4VeVasSOZB9521LUg3QEbECtAtxnQ/640)

论文链接
https://www.paperweekly.site/papers/2112


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkZibRsNMpvxCZSoNUjqBFPUMyCvJ7nksObSLatO1UHuTLcw5KYWPhREehBpXWY0uqseRtib5rxuvBw/640)
**Deep Inductive Network Representation Learning**
**@xuzhou 推荐**
\#Network Representation Learning
本文是由 Adobe Research、Google 和 Intel Labs 发表于 WWW ’18 上的工作，**论文提出了通用的归纳图表示学习框架 DeepGL**，DeepGL 具有多种优势，对今后的研究具有一定的参考意义。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkLqHOvDmEpdYWAFvaazWYuqCIjVv7MgqOQ5VyJwUG1uEWz5AKrY7dBQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk8IwdfEZBbBjyQmGT0fdOmz607PyZT1YUBgBvSialuJmefc2vjwQ4FZg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkUB0BxxqK9dHZZkZ32R6cYXpBzH6zl23QWOwsahbSmicPOMR7EicETYFA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkvOZ4PNKAxSH3KKqBzNaMNONshicvh9jden7EhSauNr9YfEpf62BRokQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk3qoYaQagP8s5pnJlBHmVx075PrkZIiatt5DmiamohkDeKiclLBWeyjzgQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkniblYD3WQH2qnvxkicGibaYticicOewM3Rh5dMPzwicgKXjbpNAqCEZNPXGw/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkQ8CPfyq7IIh9WyA053g6AX5Joe1yUbBsRVejHOEgDqzQzFphFe7d7Q/640)

论文链接
https://www.paperweekly.site/papers/2151


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjiaA5bbtkAnnJ2yLicAzlwmVdnAIic0THYptctQhZJRx7QYCx8TC9zwOow/640)
**Multi-Cast Attention Networks for Retrieval-based Question Answering and Response Prediction**
**@paperweekly 推荐**
\#Attention Mechanism
本文是南洋理工大学发表于 KDD 2018 的工作，**论文在问答和对话建模方向为各种检索和匹配任务提出了一个通用神经排序模型**。该模型在神经网络模型的基础上引入了 attention 机制，不再仅仅将 attention 作为 pooling 操作，而是将其作为一种特征增强方法。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkBMR74q9syG1yun8icagD2w1HSOyS2tWrRf6ZpAznjhBXUY1qExNSgZA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkleRuQp6sSF4Mx5EAnVehG63NHAHMHwX4tpAs411uLMJ8r036lLNeGQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkDrSlUrOCzeACgFfkzqVWo1qKlJ3QmUvrleXibNWTgjqXxnJ2ogAWVibQ/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk3oQyb2ZmfwXYRbsykL62OLRJ2OYqKruwxq8KFMm6o76uh8MiaCMpCgA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkj60dClZorXUAXFzKtnhx7fKFr7tRLx18Aeic9icBEWoTBuHIMWEibAVWw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkHjDhNWTJVupvibrm7ic2AqsiaH7JQg98OibhTvajCZcqlibD3Mh7PndUkdA/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkMuS8HnqrZA1q3zUbAo0MehfS99pCGmZGaDicZSDCC0PoMYuUjBJZUGQ/640)

论文链接
https://www.paperweekly.site/papers/2159


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglvE3hJ6tARHlod6ez4ATnjgzGNHdv2YBVm6bUicHjY2A8tV8hwJIiapvTYDGmFIyMclC4Xy6gD7krQ/640)
**Modeling Polypharmacy Side Effects with Graph Convolutional Networks**
**@xuehansheng 推荐**
\#Bioinformatics
本文来自斯坦福大学，**论文将 Graph Convolution Neural Network 应用于药物副作用研究中**。本文提出了一种通用的多关联链路预测方法——Decagon，该方法可以应用于任何多模态网络中，能够处理大量边缘类型的多模态网络。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk9Rcc4MVTFlruokBOZ7Dp1FxkROiamuDhGW8icnJ521M1NtictGLtelqrA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkCfIg9YiaiaEENvZdQavib6X4fPN7RaQYbjjqCTHTbnNwZFgcCPHKsrAXg/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUk2buhxcJ4hnBicMM1iauW9NFHRHIWiby7KBdVsrZPRlU14VqSkrluvmM8w/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkFN1huZWwxicFjIcJE8sDWibsU98ib5orn3r2jKEYok53ibIbMy2HQoph9g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkiaItM2M36O6AFHl43Wajjx8XruarheWztC4BtAlSMX9Bs67u7Wic6J5g/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkF15EUARtW7qny5UHV7icoll7AvQJAmSkk6C0w6TZPIpIDpLZQQnN7yQ/640)**▲**论文模型：点击查看大图

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkibBtBh9bNXauJTZ7GJ3UUkfkOlpsQFSUk6VSuiaXQTsghuou7ScsmzbXtw3WavTb9IicTAWcDicyeicA/640)

论文链接
https://www.paperweekly.site/papers/2127

源码链接
https://github.com/Bartzi/see


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

[暑假没人带怎么办？还有我们为你推荐论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490180&idx=1&sn=7fb37b5687869d80ea4a2accb08dac41&chksm=96e9c504a19e4c1285697e90bae1510da435f54c19961438c7062cb89b067a094c31cc08daff&scene=21#wechat_redirect)
快醒醒，一大波最新 AI 论文加开源代码来袭！

[15 篇最新 AI 论文来袭！NLP、CV...人人有份](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489270&idx=1&sn=4fa88468dba51738df921da45573a927&chksm=96e9c976a19e4060c20453f9cb275966ba25522292b9b638d712963edf208822686486b2cbb7&scene=21#wechat_redirect)
[还在熬夜憋思路？这12篇最新论文打包送给你](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489608&idx=1&sn=1b9384cbb3550a61901521c91aa97628&chksm=96e9c7c8a19e4ede12c934b943ef2f40df220a936bafc8e50a290f1848567412abaed8f7441d&scene=21#wechat_redirect)
[本](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489436&idx=1&sn=111fefd080fd4459d2a80defa94880e3&chksm=96e9c81ca19e410a975df7747ea79fc9cfba4d8fa0910112e48bf66b8f2a8520f5eafe61630f&scene=21#wechat_redirect)[顶会论文轮番炸场，本周哪些论文最值得读？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489991&idx=1&sn=cb1203796186a2513f9ef91b0cbf16f7&chksm=96e9c647a19e4f513e46b99f41442bc88a556e43752ababceb80334f682bd27b80582f00718f&scene=21#wechat_redirect)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)
▽ 点击 |阅读原文| 加入社区刷论文


