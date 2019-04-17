# NAACL 2019最佳论文揭晓，谷歌BERT获最佳长论文 - 知乎
# 



> NAACL 是自然语言处理领域的盛会，NAACL 2019 将于 6 月 2 日-7 日在美国明尼阿波利斯市举行。据官方统计，NAACL 2019 共收到 1955 篇论文，接收论文 424 篇，录取率仅为 22.6%。其中长论文投稿 1198 篇，短论文 757 篇。

刚刚，大会放出了本届会议的最佳论文，谷歌 BERT 论文获得最佳长论文奖项。

以下是机器之心对获奖论文的摘要介绍，其中最佳可解释 NLP 论文还未公开。

**最佳专题论文（Best Thematic Paper）**
- **What's in a Name? Reducing Bias in Bios Without Access to Protected Attributes**
- 作者：Alexey Romanov, Maria De-Arteaga, Hanna Wallach, Jennifer Chayes, Christian Borgs, Alexandra Chouldechova, Sahin Geyik, Krishnaram Kenthapadi, Anna Rumshisky and Adam Kalai（CMU、微软研究院、领英）
- 论文链接：[https://128.84.21.199/abs/1904.05233](https://link.zhihu.com/?target=https%3A//128.84.21.199/abs/1904.05233)

**摘要**：越来越多的研究提出了减少机器学习系统中偏见的方法。这些方法通常依赖于对受保护属性（如人种、性别或年龄）的获取。然而，这引发了两大问题：1）受保护的属性可能无法获取或不能合法使用；2）通常需要同时考虑多个受保护属性及其交集。为了减少职业分类中的偏见，本文作者提出了一种可以抑制某人真实职业预测概率与其姓名词嵌入之间关系的方法。

该方法利用了编码在词嵌入中的社会偏见，消除了对受保护属性的需要。重要的是，该方法仅在训练时需要获取人名，部署时并不需要。作者使用一个大型的在线传记数据集评估了该方法的两种变体，发现二者都能同时减少种族和性别偏见，而且几乎不降低分类器的真正率（true positive rate）。

**最佳可解释 NLP 论文**
- **CNM: An Interpretable Complex-valued Network for Matching**
- 作者：Qiuchi Li, Benyou Wang and Massimo Melucci
- 该论文暂未放出

**最佳长论文**
- **BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding**
- 作者：Jacob Devlin, Ming-Wei Chang, Kenton Lee and Kristina Toutanova（谷歌 AI）
- 论文链接：[https://arxiv.org/pdf/1810.04805.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1810.04805.pdf)

**摘要**：本文介绍了一种新的语言表征模型 [BERT](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650760293%26idx%3D1%26sn%3D1228ac706779d2a21adb9efb8455d9b2%26chksm%3D871aa01bb06d290dd054f1c172803283cf7041f4264bc3ce2c704361f9771c2d4265926867b3%26token%3D645501993%26lang%3Dzh_CN)，意为来自 Transformer 的双向编码器表征（Bidirectional Encoder Representations from Transformers）。与最近的语言表征模型（Peters et al., 2018; Radford et al., 2018）不同，BERT 旨在基于所有层的左、右语境来预训练深度双向表征。因此，预训练的 BERT 表征可以仅用一个额外的输出层进行微调，进而为很多任务（如问答和语言推断任务）创建当前最优模型，无需对任务特定架构做出大量修改。

BERT 的概念很简单，但实验效果很强大。它刷新了 11 个 NLP 任务的当前最优结果，包括将 GLUE 基准提升至 80.4%（7.6% 的绝对改进）、将 MultiNLI 的准确率提高到 86.7%（5.6% 的绝对改进），以及将 SQuAD v1.1 的问答测试 F1 得分提高至 93.2 分（提高 1.5 分）——比人类表现还高出 2 分。
![](https://pic4.zhimg.com/v2-7775c1b99517d1462a1e78d7503c44e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='773' height='200'></svg>)BERT、OpenAI GPT 和 ELMo 的预训练模型架构对比
**参考文章：**
- [最强 NLP 预训练模型！谷歌 BERT 横扫 11 项 NLP 任务记录](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749886%26idx%3D1%26sn%3D87080bc474d144b286d4673383f4b6d6%26chksm%3D871affc0b06d76d6ab49b2828a8b795b18e3d762b3978a83704a01dd60fb6dbcde2ed00e163d%26scene%3D21%23wechat_redirect)
- [预训练 BERT，官方代码发布前他们是这样用 TensorFlow 解决的](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650750932%26idx%3D1%26sn%3Db511188efeccf8c825d9c9334842d0b4%26chksm%3D871afbaab06d72bc392a112532184c455f8b22b448d3232718d419e568554d002fce94688ce6%26scene%3D21%23wechat_redirect)
- [图解当前最强语言模型 BERT：NLP 是如何攻克迁移学习的？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650753776%26idx%3D3%26sn%3Dbf0116d9d1afdab34a9086474db17e99%26chksm%3D871a8e8eb06d0798c04568c0f0a0b87977f8f9e3a2b7dca2f4f9bdef99242b54d0f21cdc54cc%26scene%3D21%23wechat_redirect)
- [谷歌终于开源 BERT 代码：3 亿参数量，机器之心全面解读](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751075%26idx%3D2%26sn%3D0a3ecd1af5f8549051760775e34db342%26chksm%3D871a841db06d0d0bcf3cc4e620bb384e050ba6e92224d338a8ddc1543add97a4a4e7919ebf15%26scene%3D21%23wechat_redirect)
- [76 分钟训练 BERT！谷歌大脑新型优化器 LAMB 加速大批量训练](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759697%26idx%3D1%26sn%3Dfe72aacd339307eca4ca79da19375526%26chksm%3D871aa66fb06d2f796cfda4649fc084f31ec74281a0862af59a42b8ccacb57f4bbdce257de29e%26scene%3D21%23wechat_redirect)

**最佳短论文**
- **Probing the Need for Visual Context in Multimodal Machine Translation**
- 作者：Ozan Caglayan, Pranava Madhyastha, Lucia Specia and Loïc Barrault（勒芒大学、帝国理工学院）
- 论文链接：[https://arxiv.org/abs/1903.08678](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.08678)

**摘要**：当前关于多模态机器翻译（MMT）的研究表明，视觉模态要么是非必需的，要么作用有限。本文作者假设这是因为该任务唯一可用数据集 Multi30K 使用的句子太简单、简短和重复，这些语句将源文本渲染为充分的语境。然而，通常情况下，我们认为可以将视觉和文本信息结合起来，以实现基础翻译（ground translation）。

本文通过系统的分析来探讨视觉模态对当前最佳 MMT 模型的贡献，分析时作者部分地删除了源文本语境，使模型无法接收到完整的文本。结果表明，在有限的文本语境下，模型能够利用视觉输入生成更好的翻译结果。当前的研究认为视觉模态对 MMT 模型来说并不重要，要么是因为图像特征的质量，要么是因为将它们整合进模型的方式，但本研究颠覆了这一看法。

**最佳资源论文**
- **CommonsenseQA: A Question Answering Challenge Targeting Commonsense Knowledge**
- 作者：Alon Talmor, Jonathan Herzig, Nicholas Lourie and Jonathan Berant（以色列特拉维夫大学、艾伦人工智能研究所）
- 论文链接：[https://arxiv.org/abs/1811.00937](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1811.00937)

**摘要**：人们通常利用丰富的世界知识和特定语境来回答问题。近期研究主要聚焦于基于关联文档或语境来回答问题，对基础知识几乎没有要求。为了研究使用先验知识的问答，我们提出了一个关于常识问答的新型数据集 CommonsenseQA。为了捕捉关联之外的常识，我们从 ConceptNet (Speer et al., 2017) 中抽取了多个目标概念，它们与某个源概念具备同样的语义关系。

我们让众包工人编写提及源概念的选择题，并区分每个目标概念之间的差别。这鼓励众包工人编写具备复杂语义的问题，而问答这类问题通常需要先验知识。我们通过该步骤创建了 12,247 个问题，并用大量强基线模型做实验，证明该新型数据集是有难度的。我们的最优基线基于 BERT-large (Devlin et al., 2018)，获得了 56% 的准确率，低于人类准确率（89%）。
![](https://pic2.zhimg.com/v2-3a5897bd3e683e85c428fe7fa3b92389_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='381' height='479'></svg>)构建 CommonsenseQA 数据集的过程示例![](https://pic1.zhimg.com/v2-6f5d14767f088f0de19ed960d006e8fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='370'></svg>)表 5：所有模型的测试集准确率
*NAACL 论文接收列表：*[https://naacl2019.org/program/accepted/](https://link.zhihu.com/?target=https%3A//naacl2019.org/program/accepted/)


