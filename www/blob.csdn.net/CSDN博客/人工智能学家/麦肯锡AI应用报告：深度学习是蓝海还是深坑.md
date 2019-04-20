# 麦肯锡AI应用报告：深度学习是蓝海还是深坑? - 人工智能学家 - CSDN博客
2018年04月24日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：281
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDM9t4P6wwVicd64Hqnyiciarks1ibONlHxldQnicEg0M7wOBPsiaSvrULWoBA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
来源：网络大数据
AI的价值并不在模型自身，而在于公司怎样用好它们。
麦肯锡出品，一份深度学习领域的分析报告，包含400个应用案例，横跨19个行业的9种业务功能。报告重点强调了前沿AI技术的应用范围之广，以及经济潜力之盛，也提到了发展中的一些局限和挑战。
还有，需要注意的是，就算AI技术的前景再光明，使用数据之前都要把数据安全、隐私以及可能出现的偏见问题考虑周全。
报告共分四大章节，那我们开始吧。
**AI技术与对应的任务类型**
随着AI技术的发展，其定义也跟着发生变化——AI到底由哪些技术构成。这里，AI代指使用人工神经网络的深度学习技术。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDr6ic2Tw9BxCnkbc78jKpV2LWhsOQgZeqMtakN1fanNuk4ib3EnEbfPtw/640?wx_fmt=jpeg)
具体来说，报告分析了三类神经网络的应用和价值——
**· 前馈神经网络 (FFNN)**
这是神经网络中最简单的一类。在FFNN的结构里，信息的流动是单向的、无环的，从输入层开始，延伸到隐藏层，最后到输出层。
第一个单神经元网络，是Frank Rosenblatt在1958年提出的。虽然，方法听来有些老旧，但计算能力、训练算法以及可用数据的演变，都让FFNN随着时间的推移变得更强大。
**· 循环神经网络 (RNN)**
神经元之间的连接是有环的，适合处理序列化输入。
2016年11月，牛津大学发表的研究成果中提到，有一种RNN读唇的准确率达到95%，远超人类的52%。
**· 卷积神经网络 (CNN)**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDh2jcViczWBvX7Seic0javiajnTkPTuB7L00HeytdPLPFsicUAt0pRZSbbw/640?wx_fmt=jpeg)
层与层之间的连接方式受到了动物视觉皮层结构的启发，那是动物大脑处理图像的地方，适合执行感知任务。
另外，本章节探讨了生成对抗网络 (GAN) 以及强化学习。不过，这两种技术并没有包含在AI的价值评估里面，因为，它们很年轻，还没有成为广泛应用的技术。
这一环节，也探讨了其他机器学习技术，以及传统数据分析方法。
**应用案例分析**
在19个行业、9种业务功能的400个案例中，可以看到的事情有很多——
深度神经网络在哪些领域能够产生最大的价值;与传统数据分析相比，神经网络能够带来多少营收增长;以及达到相应的目标，对数据的数量 (volume) 、速度 (velocity) 以及多样性 (variety) 有怎样的需求。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDic20IfHhsl8aDpgXFRLNmnK4pNF9ibwguAgaRgSATtwH14oK9dmAuNqA/640?wx_fmt=jpeg)
报告使用的案例库虽然庞大，但也并非应有尽有，依然可能夸大或低估AI在某些领域的潜力。
以下是一些应用栗子，可以一窥AI的力量。
**· 预测性维护 (predictive maintenance)**
机器学习拥有检测异常的能力。深度学习用来分析大量高维数据，可以把现有预防性维护系统的性能提升到一个新的高度。把许多传感器的数据层层叠起，神经网络能够增强或者取代许多传统方法。
AI可以预测故障，让干预计划有了用武之地，从而减少停机时间和运营成本，提高产量。
比如，AI可以通过整合飞机模型数据、维护历史记录、IoT传感器数据 (如用于故障检测的发动机数据) ，来延长货机的寿命。
**· AI驱动的物流优化**
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDpNeFpqMdfNmAqtKoWymB7rHgnUthvjBOU3toQF6hG3ib2DJgu3s0aYw/640?wx_fmt=gif)
AI可以通过实时预测和行为指导来降低物流成本。算法能够优化配送路径，从而提高能源利用效率，减少配送时间。
欧洲一间货运公司，利用传感器监测货车性能和驾驶员行为，司机会收到实时指导，何时加速或减速，优化油耗并减少维护开销。由此，燃料成本降低了15%。
**· 客服管理和个性化服务**
在呼叫中心，语音识别和来电分流，可以提升服务效率，并为用户提供更加流畅的体验。
比如，基于深度学习的音频分析系统，可以估计当前用户的情绪状态，并据此判断要不要切换到人工服务。
在销售和营销的其他方面，AI也有所作为。将用户个人信息和过往交易数据与社交媒体监测相结合，便可以生成个性化产品推送。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDJR8veAiaSG5oS1CG3UdHSoib1VhwDPqB8e7Txmd1MPO2TWPQ82ZicyBibQ/640?wx_fmt=jpeg)
在案例库69%的应用案例中，深度神经网络都是用于替代传统分析方法，提升系统表现。有16%的案例，神经网络的任务是传统方法无法完成的专属任务，这些领域称为green field。余下15%的案例中，神经网络带来的性能提升非常有限，原因包括数据方面的局限性等等。
毕竟，深度学习对数据的需求，远远大于任何传统分析方法。而如果要完全激发AI的潜力，则需要多种多样的数据，包括图像、视频、音频等。
**估计AI的潜在价值**
据估计，报告探讨过的所有AI技术加在一起，可能在19个行业的9种职能中，产生每年3.5-5.8万亿美元的价值。这在所有 (包括传统和AI) 数据分析技术一年产生的价值中，约占40%。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDr6ic2Tw9BxCnkbc78jKpV2LWhsOQgZeqMtakN1fanNuk4ib3EnEbfPtw/640?wx_fmt=jpeg)
报告中估计，在每个行业的2016年营业额中，AI的平均贡献约在1-9%。但在19个行业之间，AI贡献的营业额百分比差异较大，取决于具体应用案例、获取大量复杂数据的能力，以及规章制度等其他限制。
案例分析得到的结果是，AI能够产生最大价值的领域，既有顶线导向 (top-line-oriented) 的功能，比如营销和销售，也有底线导向 (bottom-line-oriented) 的功能，如供应链管理和生产制造。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDoia9hlryRVNDOYaewkP8ic20BgRVUAcicGEsiaicfE7R4IzpqLf3FfWbqEA/640?wx_fmt=jpeg)
在零售和高科技等消费行业，AI可能在营销和销售中拥有更大的潜力，因为商家与用户之间频繁的、数字化的互动会为AI技术提供庞大的数据集。电子商务平台将会尤其受益，点击数据与页面停留时间等用户信息都可以轻松获取，并用于生成实时动态的产品推送。
**以下又是一些栗子。**
· 在零售领域，定价和促销可以成为AI的天地。案例表示，使以用户数据来定制个性化促销活动，可以将实体零售商促销的增量销售额提升1-2%。
· 在消费品的供应链中，AI部署也能起到重要的作用。基于潜在因果驱动因素 (而非先前结果) 的预测，可以将预测准确度提升10-20%，从而带来2-3%的营业额增长。
· 在银行业，特别是零售业务中，AI依然可以在营销和销售中施展才能。另外，鉴于风险评估和管理的重要性，AI在这一方面的产生价值可能会更大。
**挑战在哪里**
AI正在吸引越来越多的企业投资，但到目前为止，拥有AI技术的公司里，只有约20%将自己的技术应用于核心业务或者大规模使用。
如今，AI技术的应用还面临着许多挑战——
**· 标注训练数据**
这在监督学习中必不可少。目前大多是手动标注，但新的技术正在涌现，比如强化学习和in-stream supervision，数据可以在自然使用过程中获得标注。
**· 大量全面的数据获取**
对许多行业案例来说，获取大量数据非常困难，比如用于预测治疗方案效果的临床数据。
**· 解释输出结果**
比如，疾病诊断过程中，AI可以利用患者信息来得出诊断结论，但无法解释这一结论是如何一步步取得的。
**· 学习的普遍性**
AI模型在将学习经验从一类情况向另外一类情况迁移时，经常遇到困难。这意味着，企业需要投入大量资金训练新模型，即便情况与之前的用例相似也很难避免。
这可能牵扯到一些社会性的问题。比如，训练数据无法代表一个更大的群体时，人脸识别模型便可能在无意之间产生偏见。
或许，AI是一个难以琢磨的商业案例，许多因素都会减缓深度学习的行业落地。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2RBKsibyVicqVxL3Xl00YT2rDf40UV9XGmDAmbHUs48xO2sOMOUgmqSC8RfwoJHpoKblduia7zXwebnQ/640?wx_fmt=jpeg)
最后一个小节，便是从许多案例的研究中得出的，AI技术提供者、应用者，以及政策制定者可能需要的启发。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
