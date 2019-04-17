# 清华大学NLP组年末巨献：机器翻译必读论文列表 - 知乎
# 



机器之心编辑，参与：路、李泽南。

> 昨天，清华大学自然语言处理组（THUNLP）整理的机器翻译论文阅读清单在 GitHub 上上线了。对于元旦和寒假期间想要积累机器翻译背景知识、追踪前沿技术的同学来说，这份列表再合适不过了。

GitHub 链接：[https://github.com/THUNLP-MT/MT-Reading-List](https://link.zhihu.com/?target=https%3A//github.com/THUNLP-MT/MT-Reading-List)

机器翻译（MT）是借机器之力「自动地将一种自然语言文本（源语言）翻译成另一种自然语言文本（目标语言）」的方法。使用机器做翻译的思想最早由 Warren Weaver 于 1949 年提出。时间进入二十一世纪，研究人员开发了基于神经网络的语言模型。近年来，神经机器翻译已经进入了应用阶段。
![](https://pic3.zhimg.com/v2-dc863687849f5202f7aacce467c47062_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='200'></svg>)
2018 年即将结束，在 18 年的最后一个工作日，清华大学自然语言处理组（THUNLP）整理了一份机器翻译论文的阅读清单奉献给大家。

该项目的主要贡献者 Zonghan Yang 表示：

> 这份阅读清单里回顾了统计机器翻译（SMT）时代的亮点论文，并概括了近期神经机器翻译（NMT）方向下的各个子领域，其中包括：模型架构、注意力机制、开放词表问题与字符级别神经机器翻译、训练准则与框架、解码机制、低资源语言翻译、多语种机器翻译、先验知识融合、文档级别机器翻译、机器翻译中的鲁棒性、可视化与可解释性、公正性与多样性、机器翻译效率问题、语音翻译与同传翻译、多模态翻译、预训练方法、领域适配问题、质量估计、自动后处理、推导双语词典以及诗歌翻译。

过去三十多年来，人们见证了机器翻译的快速发展，尤其是数据驱动的方法，如统计机器翻译（SMT）和神经机器翻译（NMT），目前 NMT 占据了这个方向的主导地位。清华大学 NLP 组表示该列表目前可能还不完整，未来将继续增加论文并改进清单。

该论文列表接收内容可谓详尽。首先，对于时间不够充裕的人来说，THUNLP 为大家整理了 10 大必读论文。这十篇论文涉及统计机器学习的数学基础、机器翻译度量指标 BLEU、基于短语的统计机器翻译、Quoc V. Le 等提出的序列到序列学习方法（该方法引出了谷歌后来序列到序列（seq2seq）的 NMT 模型）、Adam 优化算法以及 Attention 机制等。
![](https://pic3.zhimg.com/v2-b4457c88b0928044d15c1ac04219279a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='550'></svg>)
其中最新的一篇自然是谷歌大脑那篇标题吓人的《Attention Is All You Need》，该研究提出了目前广泛流行的 Transformer 机器翻译架构，详情参见机器之心的报道：

[学界 | 机器翻译新突破：谷歌实现完全基于 attention 的翻译架构](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650727887%26idx%3D5%26sn%3Da5991e49a4b1df2ea70ddbb84631ff9c%26chksm%3D871b21b1b06ca8a71be9ec7ee1d7d5ef46eef1116c3d396f3cc467b95186d278bcdd8e6ac07c%26scene%3D21%23wechat_redirect)

在这之后，THUNLP 列出了机器翻译各细分领域的必读论文，分为统计机器学习和神经机器翻译两大类，其中最新数据已经统计到了 ICLR、AAAI 2019 在投的研究。

分类目录：

**Statistical Machine Translation**
- Tutorials
- Word-based Models
- Phrase-based Models
- Syntax-based Models
- Discriminative Training
- System Combination
- Evaluation

统计机器翻译部分介绍了基本的模型：基于单词、短语、句法的 SMT 模型，以及训练方法和评估方法。

**Neural Machine Translation**
- Tutorials
- Model Architecture
- Attention Mechanism
- Open Vocabulary and Character-based NMT
- Training Objectives and Frameworks
- Decoding
- Low-resource Language Translation
-  Semi-supervised Methods 
-  Unsupervised Methods
-  Pivot-based Methods
-  Data Augmentation Methods
-  Data Selection Methods
-  Transfer Learning & Multi-Task Learning Methods
-  Meta Learning Methods
- Multilingual Language Translation
- Prior Knowledge Integration
-  Word/Phrase Constraints
-  Syntactic/Semantic Constraints
-  Coverage Constraints
- Document-level Translation
- Robustness
- Visualization and Interpretability
- Fairness and Diversity
- Efficiency
- Speech Translation and Simultaneous Translation
- Multi-modality
- Pre-training
- Domain Adaptation
- Quality Estimation
- Automatic Post-Editing
- Word Translation and Bilingual Lexicon Induction
- Poetry Translation

神经机器翻译目前是机器翻译的主流。这部分介绍了 NMT 中的模型架构、注意力机制、训练准则和框架等与具体模型训练相关的内容，还介绍了低资源语言翻译、多语言翻译、篇章翻译等具体任务相关的研究，以及关于机器翻译系统鲁棒性、可视化、可解释性、公平性等的研究。

低资源语言翻译任务由于训练数据少而难度极高。这一两年来对低资源语言翻译任务的研究逐渐增多，相关研究大多采用半监督、无监督等方式。清华大学 NLP 组在这部分介绍了半监督、无监督、数据增强、迁移学习、多任务学习和元学习等方法。

篇章翻译同样是机器翻译领域的困难任务。这部分介绍了 2017 和 2018 年的新研究，包括清华大学和搜狗合作发表在 EMNLP 2018 的工作。在此文中，作者提出了一个基于自注意力翻译模型 Transformer 的篇章级别翻译模型，利用丰富的上下文信息来帮助进行句子的翻译，该模型对机器翻译领域篇章级别翻译质量的提升也有贡献。

这部分还涉及自动后编辑、诗歌翻译等内容，详细内容请查看该 GitHub repo。 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
前不久，清华大学 NLP 组还整理了图网络必读论文列表，参见：

[清华大学孙茂松组：图神经网络必读论文列表](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650754602%26idx%3D2%26sn%3D7d55e6c20bbf4d85d19d62fc67045b85%26chksm%3D871a8a54b06d03429b5fb811c7ef7f22d368c3c2db2ebb71b3abd3d6a9737088e54a402c5ac2%26scene%3D21%23wechat_redirect)


