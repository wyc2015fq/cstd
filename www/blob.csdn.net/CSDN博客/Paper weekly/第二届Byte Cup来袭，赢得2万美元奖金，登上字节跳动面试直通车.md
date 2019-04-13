
# 第二届Byte Cup来袭，赢得2万美元奖金，登上字节跳动面试直通车 - Paper weekly - CSDN博客


2018年08月21日 08:34:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：731


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCrszXP71KUJaSMbAR6kujdp3yibvnDp2J8XlLkWibK7O1swQ8jxIUEaLw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCJR37ETCEYJR9Qdw8ic24r42djzpfJ0rX2PfkfzjDcyhC7tFEPzZnLCw/640)
2018 Byte Cup 国际机器学习竞赛（以下简称 ByteCup）是一项面向全球的机器学习竞赛，由中国人工智能学会和字节跳动联合主办，数据竞赛平台 biendata.com 承办，旨在促进机器学习的学术研究和具体应用。2016 年，首届 Byte Cup 举办，共吸引全球 1000 多支队伍参赛，共同完成“为头条问答的问题在今日头条专家用户中寻找潜在的答案贡献者”这一竞赛任务。**2018 年 ByteCup 发布的新任务是“自动为文章生成标题”。**
我们正处于一个信息爆炸的时代。据 IDC 统计，互联网数据量已跃至 ZB 级别（1ZB=240GB），预计 2020 年达到 35ZB。我们每天也能看到大量的信息，包括新闻快讯、社交网络更新、事件评论，以及专业教程等等。此外，搜索引擎并不能彻底信息过载问题，而移动设备的普及和碎片化的阅读方式让这个问题更加严重。因此，如果可以开发出自动提取文本摘要的机器学习模型，就可以帮助消化海量的信息。
文本摘要一直都是机器学习领域一个重要的热点，但是却有很大的难度。例如，给单篇文章起标题/摘要的时候，很难有词频作保证，而是需要模型可以理解内容，甚至做一些推理。在很多地方，摘要生成和机器翻译有类似之处。然而，和机器翻译不同的是，自动文本摘要的输入和输出很不平衡，此外机器翻译任务的输入输出序的列通常有一些词义层面上的直接对应，这种对应在摘要任务中却没那么明显。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCmT0F8hkK03JtickvviaJPgWL6ZNQ0DDngEZV9VjxTDdrj815Aa99zfWg/640)

字节跳动于 2012 年成立，旗下有大量产品可以让用户阅读、消费和创作内容，包括大家熟悉的今日头条和抖音短视频，以及很多海外产品，如 TopBuzz 和 Tik tok。此外，字节跳动还在 2016 年成立了 AI 实验室，实验室覆盖了很多不同的领域，包括机器学习、自然语言处理、计算机视觉、语音识别、数据挖掘、人机交互等，而自然语言处理领域又有机器写作、机器翻译、NLP 基础、问答、对话以及推荐/搜索等方向。
**竞****赛任务**
Topbuzz是字节跳动为北美和巴西的用户创造的一站式内容消费平台，它利用机器学习算法为用户提供个性化视频、GIF图、本地新闻及重大新闻。目前，Topbuzz每天都会发布大量的文章，利用人工智能为创作者提供更好的标题选择，是Topbuzz优化用户体验的探索方向之一。**此次竞赛任务便是为****Topbuzz****提供的英文文章自动生成标题。**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCDmqphkCMuA9SwunkyiackKznibwyQtMUT1lOlu8ib1JeQUf9VJmpGibCsA/640)
▲本次比赛邀请了数十位学界和业界的顶尖学者作为顾问
**比赛奖励**
**前****10****名的队伍将获得证书和字节跳动招聘面试直通车的机会。**周冠军将获得周冠军证书。

**1.****总奖金****2****万美元**

一等奖1名：10,000美金
二等奖2名：每名3,000美金
三等奖4名：每名1,000美金

2.周冠军自2018年8月27日起，评测系统将选出周冠军。周冠军由过去7天内的提交成绩决定。
**比赛时间**
**8 月 15 日 ：**比赛开放提交答案入口
**8 月 15 日- 11 月 15 日：**比赛期（**比赛期间可随时报名**）
**11 月 16 日：**发布最终测试集
**11 月 17 日：**公布测试集排名，排名前 10 的队伍一周内提交一篇不超过 4 页的参赛方法说明（ACM 双列标准模板，中英文皆可）。
**11 月 30 日：**公布最终排名。
**数据集**
本次竞赛使用的训练集包括了约130万篇文本的信息，每篇文本都是一个类似json格式的行，如以下内容：
{"content": "Being the daughter of Hollywood superstar TomCruise and America's sweetheart Katie Holmes...",
"id": 1198440,
"title": "Suri Cruise 2018: Katie Holmes Bonds With DaughterDuring Dinner Date While Tom Cruise Still MIA"}
**文档一共包含三类信息：**
1. 文章id（id）：每篇文本对应一个unique id；
2. 文章内容（content）：即文章的内容字符串；
3. 文章标题（title）：文章的标题，参赛选手需要自己生成验证集和测试集的标题。
验证集和测试集格式和训练集相似，但是没有title，需要参赛选手预测。
**说明：**训练集用于模型的学习，验证集用于在线实时评估算法效果，测试集用于最终的效果评测。
**相关研究**

目前，自动文档摘要技术主要分为抽取式（extractive）和摘要式（又叫生成式）（abstractive）两种。
抽取式摘要相对较为成熟。这种方法利用如 text rank 这样的排序算法，对处理后的文章语句进行排序。不过抽取式摘要在语义理解方面考虑较少，无法建立文本段落中的完整的语义信息。
相较而言，生成式技术需要让模型理解文章语义后总结出摘要，更类似人类的做法。不过这种技术需要使用机器学习技术，长期以来并不成熟。转折点出现在2014年。这一年，Bengio等人发表论文**Learning Phrase Representations using RNN Encoder-Decoder for Statistical Machine Translation**，正式引入了sequence-to-sequence模型。这一模型通过两个循环神经网络，分别把输入文本转化成向量，再把向量转成输出序列。这种模型在论文中主要用来完成机器翻译任务，并且后来被应用在谷歌翻译中，但后续在文摘生成任务中也产生了广泛的应用。此后，这种利用深度学习的 sequence-to-sequence 方法不断被改进，在一些标准的评测数据集（如 DUC-2004）上，已经超过了传统的抽取式方法。
例如，2016 年，Facebook AI 实验室（FAIR）的学者发表论文**A Convolutional Encoder Model for Neural Machine Translation**，在编码的时候用 CNN 取代 RNN，获得不错的效果。Salesforce 的研究人员 2017 年发表的论文**A Deep Reinforced Model for Abstractive Summarization**中，使用了增强学习，在 CNN/Daily Mail 数据集上的 ROUGE-1 分数达到 41.16 分。同年，又是 FAIR 发表了论文**Convolutional Sequence to Sequence Learning**，引入 attention 机制，不仅提高了评测分数，还极大地提升了速度。
2016 年，来自 IBM 沃森的研究人员发表论文**Abstractive Text Summarization Using Sequence-to-Sequence RNNs and Beyond**，和之前的论文不同，这篇论文把机器翻译和文本摘要任务完全分开，专门针对文本摘要提出了更合适的模型，除此之外，文章还发布了两个新的数据集。
中国也有许多学者在从事相关工作，例如北大的万小军老师。他和姚金戈的综述**《自动文摘研究进展与趋势》**把摘要技术框架总结成 4 个步骤：内容表示 → 权重计算 → 内容选择 → 内容组织，并对每个步骤都作了介绍。

> 自动文摘研究进展与趋势：


> http://qngw2014.bj.bcebos.com/zhuankan/12/%E8%87%AA%E5%8A%A8%E6%96%87%E6%91%98%E7%A0%94%E7%A9%B6%E8%BF%9B%E5%B1%95%E4%B8%8E%E8%B6%8B%E5%8A%BF.pdf

**比赛组委会**
**主办：**中国人工智能学会
**联合主办：**字节跳动
**联合组织：**IEEE 中国代表处、Biendata.com
**参赛方式**
点阅读原文链接或扫描参赛二维码可直达赛事页面，注册网站 -下载数据，即可参赛。

友情提示，因涉及到数据下载，强烈建议大家**登录 PC 页面**报名参加～～

**大赛页面地址：**
https://biendata.com/competition/bytecup2018/



