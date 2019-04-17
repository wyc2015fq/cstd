# 融合 MF 和 RNN 的电影推荐系统 - Paper weekly - CSDN博客





2017年12月28日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1021









全标题：Leveraging Long and Short-term Information in Context-aware movie recommendation － 融合 MF 和 RNN 的电影推荐系统




介绍本期的工作之前，先打一个紧急的招生广告，我们组现还有一个博士名额，想要来中科院深圳先进技术研究院读博的同学可以联系我，主要做一些关于自然语言处理、数据挖掘、计算机视觉、机器学习等的工作，报名结束日期是 2018 年 1 月 12 号。




联系方式和主页地址如下：




**邮箱：**min.yang@siat.ac.cn

**主页：**www.minyang.me




随着互联网技术飞速发展，在线数据越来越庞大，如何帮助用户从海量数据中找到所需信息是急需解决的问题。




个性化推荐系统能够有效的解决信息过载问题，推荐系统根据用户的历史偏好和约束为用户提供排序的个性化物品（item）推荐列表，更精准的推荐系统可以提升和改善用户体验。所推荐的物品可以包括电影、书籍、餐厅、新闻条目等等。




本文主要针对电影推荐做了深入研究，但所提出的方法可以很方便地迁移到其他物品推荐中。




目前，协同过滤技术已成为最广泛采用的推荐技术，已经被广泛应用到了很多商业系统中，比较著名的有 Amazon、Netflix、淘宝等。




传统的基于协同过滤的推荐系统是认为用户偏好和电影属性都是静态的，但他们实质是随着用时间的推移而缓慢变化的。例如，一个电影的受欢迎程度可能由外部事件（如获得奥斯卡奖）所改变。




除了对时间演化进行建模的需求外，协同过滤方法使用了未来的评分来评估当前的喜好，这一定程度违背了统计分析中的因果关系。




另一方面，随着深度学习应用的爆发式发展，特别是在计算机视觉、自然语言处理和语音方面的进展，基于深度学习的推荐系统越来越引发大家的关注。循环神经网络（RNN）理论上能够有效地对用户偏好和物品属性的动态性进行建模，基于当前的趋势，预测未来的行为。




为了有效地利用传统协同过滤推荐技术（i.e., 矩阵分解）和深度学习方法（i.e., 循环神经网络）各自的优点，捕获用户和电影之间的长期（全局）和短期（局部）关联，本文主要研究和探索矩阵分解（Matrix Factorization, MF）和循环神经网络（Recurrent Neural Network, RNN）在推荐系统上的互补性。




该项工作已经发表在 arXiv 上，更多细节可以点击本文底部的“阅读原文”查看原论文，我们会在论文发表后放出代码。




我们提出一种 LSIC 模型（Leveraging Long and Short-term Information in Context-aware movie recommendation），具体框架如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xqibkQckFUJKU7V0mGAHHYY7PsM5ZaH4U4zn4RsBroGEFqksRmVKCBYw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xqibkQckFUJKU7V0mGAHHYY7PsM5ZaH4U4zn4RsBroGEFqksRmVKCBYw/)




LSIC 模型采用了生成对抗网络（GAN）框架将基于 MF 和 RNN 的模型融合，同时捕获用户长期偏好和短期会话信息，从而最大限度地提高推荐系统的最终性能，达到 state-of-the-art 的效果。我们介绍了 4 种方法来融合 MF 模型和 RNN 模型，具体如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2x9N4KRC16H1DPOAClMbTMMtjgtafrMC9PFzicdVlzy811Q4Fl4EgQ2ibg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2x9N4KRC16H1DPOAClMbTMMtjgtafrMC9PFzicdVlzy811Q4Fl4EgQ2ibg/)




**LSIC-V1: Hard 机制**




采用简单的求和方法混合 MF 和 RNN 预测的分数，如图 Figure2(a)，公式如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xBO8H39GOD8w9rRBAd49LSPqbfPvFeUIdOEAk7Ie3RbiaukgdiaAnZFBg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xBO8H39GOD8w9rRBAd49LSPqbfPvFeUIdOEAk7Ie3RbiaukgdiaAnZFBg/)




**LSIC-V2 **




我们通过预训练 MF 得到用户和视频的 latent factors，再初始化用户 LSTM 和视频 LSTM 的隐状态，如图 Figure2(b)。




**LSIC-V3 **




我们对 LSIC-V2 进展扩展，采样 MF 得到的 latent factors 作为两个 LSTM 的 static context vectors 加到每个时刻 t 的输入中，如图 Figure2(c)。




**LSIC-V4 **




我们采用 attention 机制动态调整 MF 和 RNN 的融合方式，如图 Figure2(d)，公式如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xTdG0E00ZbmuaIH3WF6PXIFGw0pjgHOPp0yCSpYs1EjDfqaD25nRhWA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xTdG0E00ZbmuaIH3WF6PXIFGw0pjgHOPp0yCSpYs1EjDfqaD25nRhWA/)

![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xbvAGQib70DZXIwJibmuniciaiafcvI0l1MzTy1CdzWhONmbQ2GUxEDfNpUw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xbvAGQib70DZXIwJibmuniciaiafcvI0l1MzTy1CdzWhONmbQ2GUxEDfNpUw/)




生成对抗网络：判别器尝试区别视频的真假，它是来自训练集中的高分视频还是生成器生成出来的伪高分视频。生成器尝试去生成真高分视频来欺骗判别器。具体细节请参考【5】，我们以这篇工作为基础做了许多改进，比如通过 GAN 有效结合用户长期偏好的短期会话的模型等。




生成器：（Figure1 的左边）类似于条件 GAN，我们的生成器 G 输入用户偏好数据和时刻 t，给用户 i 生成推荐列表，具体公式如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xs2F1qiaS13oXsFiaUiaEcurK03axmPPCZ30xo0Joib9VZiaicglBsjicSqKaw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xs2F1qiaS13oXsFiaUiaEcurK03axmPPCZ30xo0Joib9VZiaicglBsjicSqKaw/)




其中，M 是视频集合，m_(g, t) 是在 t 时刻生成的视频 index。





判别器（Figure1 的右边）我们采用 Siamese 网络构建判别器 D，并且以 pair-wise 的方法融合长短时模型。具体来说，判别器 D 有两个对称的 point-wise 网络，她们共享参数并且采用 pair-wise 的损失函数来更新。具体的目标函数如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xqVlfumYBShsH2HP337wI7nIKcx8ibXMDbTZhIic8n9I0ko6OUzX6Ybqw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xqVlfumYBShsH2HP337wI7nIKcx8ibXMDbTZhIic8n9I0ko6OUzX6Ybqw/)




U 是用户集合，u_i 是用户 i，m_+ 是高分视频，m__ 是随机从视频集合中采样出的低分视频，最后我们采用 hinge 损失函数来优化判别器 D，具体公式如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xenwVjf6iaical3jzSZdjQOEg4PRqAH8xJXK3n1HNQKiauquUibnAoRqicBw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xenwVjf6iaical3jzSZdjQOEg4PRqAH8xJXK3n1HNQKiauquUibnAoRqicBw/)




强化学习：由于视频采样的过程是离散的，不能采用标准的 GAN 公式来优化。因此，我们采用 policy gradient 来优化生成器 G，使得 G 能生成高收益的推荐列表来欺骗判别器 D。具体来说，推导如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xS2qic1ZpicZ6MmJtWhYbic53y3BURr1UyjoeVp9lBFfibDPVHgyz3t5BPg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xS2qic1ZpicZ6MmJtWhYbic53y3BURr1UyjoeVp9lBFfibDPVHgyz3t5BPg/)




数据集：为了验证我们模型的有效性，我们在两个广泛使用的数据集进行测试 Movielens100K 和 Netflix，为了评估模型的鲁棒性，我们分别进行了 3 个月 Netflix 和全集 Netflix 的实验，数据集细节如下：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xw3BCaEAsG6KSWrSBpNoCrSicjCCy2C4NqTWUTMibFeicicdyhFmpVILFmg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xw3BCaEAsG6KSWrSBpNoCrSicjCCy2C4NqTWUTMibFeicicdyhFmpVILFmg/)




对比算法： 在实验中，我们和一些 baseline 和 state-of-art 进行对比：BPR [1]，PRFM [2]，LambdaFM [3]，RRN [4]，IRGAN [5]。




实验结果：




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xMuRhCHkqbKodWVIsc5ibf5cciagaQ4o3bMhAR98AL2M4voaYxtAJRCCw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xMuRhCHkqbKodWVIsc5ibf5cciagaQ4o3bMhAR98AL2M4voaYxtAJRCCw/)




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xeY6kaJQPlCHKFr6q7JFibHicm9ibQ6G5d7aKZqvHmG9FyAIJMQ24ibQyAA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xeY6kaJQPlCHKFr6q7JFibHicm9ibQ6G5d7aKZqvHmG9FyAIJMQ24ibQyAA/)




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xCvF6LPGdg9MH319LDR0JdG7WycKu9qXGbhibic65mpvcmZ9zIFVqnokA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xCvF6LPGdg9MH319LDR0JdG7WycKu9qXGbhibic65mpvcmZ9zIFVqnokA/)




此外，我们还进行了 case study 的分析。我们从 Netflix 数据集中随机选择两个用户并为其生成推荐列表。LSIC 模型可以更有效的进行推荐。例如，用户“8003”的电影“9 Souls”从排名5（LambdaFM）增加到排名 1（LSIC-V4）。




![OPzybmZ8HzicADZdPhys2BftiawlVicWN2xVcCrl0hA6xfwCrP4YxYJ0tyPAA8Td6HUiafxTIuve3icTEuQibuVI3pZA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/OPzybmZ8HzicADZdPhys2BftiawlVicWN2xVcCrl0hA6xfwCrP4YxYJ0tyPAA8Td6HUiafxTIuve3icTEuQibuVI3pZA/)




总结：我们提出了一种新颖的基于生成对抗网络的推荐系统，采用强化学习动态调整历史长期偏好和短期会话的模型，此外，我们加入了封面图片特征进一步提升系统性能，最后在两个数据集上做到 state-of-art 的性能。




**参考文献**




1. Steffen Rendle, Christoph Freudenthaler, Zeno Gantner, and Lars SchmidtThieme. 2009. BPR: Bayesian personalized ranking from implicit feedback. InProceedings of the twenty- fth conference on uncertainty in articial intelligence. AUAI Press, 452–461.

2. Runwei Qiang, Feng Liang, and Jianwu Yang. 2013. Exploiting ranking factorization machines for microblog retrieval. In Proceedings of the 22nd ACM international conference on Conference on information & knowledge management. ACM, 1783–1788.

3. Fajie Yuan, Guibing Guo, Joemon M Jose, Long Chen, Haitao Yu, and Weinan Zhang. 2016. Lambdafm: learning optimal ranking with factorization machines using lambda surrogates. In Proceedings of the 25th ACM International on Conference on Information and Knowledge Management. ACM, 227–236.

4. Chao-YuanWu, Amr Ahmed, Alex Beutel, Alexander J Smola, and How Jing. 2017. Recurrent recommender networks. In Proceedings of the Tenth ACM International Conference on Web Search and Data Mining. ACM, 495–503.

5. Jun Wang, Lantao Yu, Weinan Zhang, Yu Gong, Yinghui Xu, Benyou Wang, Peng Zhang, and Dell Zhang. 2017. IRGAN: A Minimax Game for Unifying Generative and Discriminative Information Retrieval Models. In Proceedings of the 40th International ACM SIGIR Conference on Research and Development in Information Retrieval. 515–524.







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)

▽ 点击 | 阅读原文| 查看论文




