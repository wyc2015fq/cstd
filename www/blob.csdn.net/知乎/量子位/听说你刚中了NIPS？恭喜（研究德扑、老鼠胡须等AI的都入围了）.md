# 听说你刚中了NIPS？恭喜（研究德扑、老鼠胡须等AI的都入围了） - 知乎
# 



> 允中 发自 凹非寺
量子位 出品 | 公众号 QbitAI



![](https://pic3.zhimg.com/v2-8c535a751ff28bbf0cd8f61389210dc6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='153'></svg>)



今天凌晨，NIPS 2017开始揭榜。

作为机器学习领域的顶级会议，NIPS 2017共收到3240篇论文投稿，毫不意外这个数字又创了新高。有678篇论文被选中作为大会论文，比例20.9%。

其中有40篇被选中进行口头报告（oral），112篇选为spotlight进行展示。

接收到NIPS 2017入选通知邮件的研究者，纷纷都迫不及待的开始与外界分享这一喜悦。或曰：“**登斯NIPS也，则有心旷神怡，宠辱偕忘，把酒临风，其喜洋洋者矣。**”

当然也有遗憾被拒的，比方量子位在微博上看到的这一幕：形成对比的两重天。



![](https://pic3.zhimg.com/v2-2ac6a992ddd8f82fbc870a4468cbddda_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='412'></svg>)



很多入围者在庆祝之余，还附带了arXiv链接。

什么？已经在arXiv上发表？真的可以么？

对，没关系。NIPS 2017要求提交的论文，不能是已经公开/接受出版，或者曾经投递给其他会议/期刊。但是允许提前发表在[http://arXiv.org](https://link.zhihu.com/?target=http%3A//arXiv.org)上。

当然还有很多人并不喜欢这么做，所以他们入选的论文稍后会公开发布。

现在到底有哪些入围论文已经可以看到了？量子位手动统计了一下目前的情况，数据来自微博、推特、reddit等公开渠道。

## **Oral**

**Safe and Nested Subgame Solving for Imperfect-Information Games**

在NIPS 2017接受的口头报告展示论文中，我们发现了一篇“明星”论文。来自卡内基梅隆大学博士生Noam Brown和Tuomas Sandholm教授。



**![](https://pic4.zhimg.com/v2-3a39a8e7b5ad3529ad118d1a6e66ebdf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='317'></svg>)**



还记得年初[叱咤德州扑克界的AI么](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247483881%26idx%3D1%26sn%3D84b5df899a22ef532098d9779212a6dc%26chksm%3De8d3b69bdfa43f8d5f53658400b598cfff3eaf7ea72f8f602eca6059d8e3e1ab18784df9bbd7%26scene%3D21%23wechat_redirect)？量子位[3月还在CMU独家专访了这两位学者](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247484988%26idx%3D1%26sn%3D541f58b87df27fde2ca53fd2b757faa1%26chksm%3De8d3b14edfa43858d7760cba93a4f460568859b2b5c70bee7c86bbc5cd636317b1e512970b18%26scene%3D21%23wechat_redirect)。这次的论文，就是他们最新的研究成果总结。

摘要简介：不完美信息博弈问题，虽然不能通过拆解为子博弈解决，但可以通过不相交的子博弈来逼近解决方案，或者改进现有解决方案。



![](https://pic2.zhimg.com/v2-98cd4564bb2d80b32039cf73f760977d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='214'></svg>)



论文地址：

[https://arxiv.org/pdf/1705.02955](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1705.02955)

**Toward Goal-Driven Neural Network Models for the Rodent Whisker-Trigeminal System**

这是另一篇看起来很好玩的论文。第一作者是来自斯坦福的博士生Chengxu Zhuang（庄程旭），其他作者来自MIT、西北大学以及斯坦福。



![](https://pic3.zhimg.com/v2-a23bd1d3f7f692ddd628e7c16b5fd8e6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='322'></svg>)



摘要简介：很大程度上，啮齿动物（老鼠什么的）通过胡须“看到”世界。在这次的研究中，我们采用目标驱动的深度神经网络对胡须-三叉神经系统进行建模。

首先，我们构建了一个生物物理学实现的老鼠胡须阵列模型。然后，生成一个胡须扫过各种3D对象的大型数据集，包括各种不同的姿势、角度和速度。接下来，我们训练了几个不同架构的DNN来解决这个数据集中的形状识别任务。

最后我们证明，胡须-三叉神经系统的DNN模型值得深入研究。



![](https://pic2.zhimg.com/v2-b858fa2ac18e249cc35d7503884cfb61_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='258'></svg>)






论文地址： 

[https://arxiv.org/abs/1706.07555](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.07555)

**TernGrad: Ternary Gradients to Reduce Communication in Distributed Deep Learning**

这篇论文的作者是杜克大学的博士生温伟、陈怡然教授等人。

摘要简介：用于同步梯度和参数的高网络通信成本，是众所周知的分布式训练瓶颈。在这个研究中，我们提出TernGrad：使用三元梯度来在数据并行中加速分布式深度学习。

论文地址：

[https://arxiv.org/pdf/1705.07878](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1705.07878)



![](https://pic4.zhimg.com/v2-22a09d79eb2c5beb13fa59d0d2e43b93_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='1138'></svg>)



## **△** NIPS 2017发给陈教授的邮件

**Robust and Efficient Transfer Learning with Hidden-Parameter Markov Decision Processes**

这篇论文的作者三位来自哈佛大学，一位来自布朗大学。

摘要简介：这是一个新的隐马尔可夫决策过程（HiP-MDP）方案，使用贝叶斯神经网络替换之前基于高斯高程的模型。新框架将HiP-MDP的应用范围扩大到更高维度和更复杂动态的领域。

论文地址：

[https://arxiv.org/abs/1706.06544](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.06544)

**End-to-end Differentiable Proving**

两位作者分别来自牛津和伦敦大学学院。

摘要简介：这是一个端到端的微分定理证明神经网络，运算基于符号的密集向量表示。具体来说，我们使用径向基函数内核，基于符号向量表示构建了一个微分计算，用以取代符号统一（symbolic unification）。

论文地址：

[http://arxiv.org/abs/1705.11040](https://link.zhihu.com/?target=http%3A//arxiv.org/abs/1705.11040)

**A Linear-Time Kernel Goodness-of-Fit Test**

作者来自伦敦大学学院、巴黎综合理工大学、日本统计数学研究所。

摘要简介：我们提出了一种拟合优度的新型自适应测试。我们证明了新的测试，比以前的线性时间内核测试具有更大的相对效率。

论文地址：

[https://arxiv.org/abs/1705.07673](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1705.07673)

**On Structured Prediction Theory with Calibrated Convex Surrogate Losses**

作者来自法国国家信息与自动化研究所、蒙特利尔大学。

摘要简介：这篇论文提出了一种结构化预测的新见解。对于任务丢失，我们构建了一个可以通过随机梯度下降优化的凸替代，并证明了所谓“校准函数”的紧界，将过量替代风险与实际风险相关联。

论文地址：

[https://arxiv.org/abs/1703.02403](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.02403)

## **Spotlight**

**Overcoming Catastrophic Forgetting by Incremental Moment Matching**



![](https://pic1.zhimg.com/v2-4484e8b0a1f779e53c9eb470cbd98d1c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='391'></svg>)



作者来自首尔国立大学等韩国研究机构。

摘要简介：为了解决灾难性遗忘这个问题，我们提出了使用贝叶斯神经网络框架构建的增量矩匹配（IMM）。

论文地址：

[https://arxiv.org/abs/1703.08475](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.08475)

入选oral和Spotlight展示环节的论文，量子位初步找到这些。更多的内容，期待官方统一公布。以下还有一些NIPS 2017接受的论文。

## **其他入选论文**

**Dynamic Safe Interruptibility for Decentralized Multi-Agent Reinforcement Learning**

分散式多智能体强化学习的动态安全中断

论文地址：

[https://arxiv.org/abs/1704.02882](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.02882)

**Byzantine-Tolerant Machine Learning**

拜占庭容错机器学习

论文地址：

[https://arxiv.org/abs/1703.02757](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.02757)

**Causal Effect Inference with Deep Latent-Variable Models**

因果效应推理与深度潜变量模型

论文地址：

[https://arxiv.org/abs/1705.08821](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1705.08821)

**Learning Hierarchical Information Flow with Recurrent Neural Modules**

用循环神经模块学习分层信息流

论文地址：

[https://arxiv.org/pdf/1706.05744](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1706.05744)

**SVCCA: Singular Vector Canonical Correlation Analysis for Deep Understanding and Improvement**

SVCCA：深度理解和改进的奇异向量规范相关分析

论文地址：

[https://arxiv.org/abs/1706.05806](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.05806)

**Structured Bayesian Pruning via Log-Normal Multiplicative Noise**

通过对数正态乘法噪声构造贝叶斯修剪

论文地址：

[https://arxiv.org/abs/1705.07283](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1705.07283)

**SafetyNets: Verifiable Execution of Deep Neural Networks on an Untrusted Cloud**

SafetyNets：在不可信云上可信赖的运行深度神经网络

论文地址：

[https://arxiv.org/pdf/1706.10268](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1706.10268)

**Working hard to know your neighbor’s margins: Local descriptor learning loss**

努力学习临近的边缘：本地描述符学习损失

论文地址：

[https://arxiv.org/abs/1705.10872](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1705.10872)

**Emergence of Language with Multi-agent Games: Learning to Communicate with Sequences of Symbols**

多智能体游戏语言的出现：学习与符号序列进行沟通

论文地址：

[https://arxiv.org/abs/1705.11192](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1705.11192)



![](https://pic3.zhimg.com/v2-6eed082779b7b5340fdcef4eb80f0bee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='393'></svg>)



**Practical Bayesian Optimization for Model Fitting with Bayesian Adaptive Direct Search**

贝叶斯自适应直接搜索模型拟合的实际贝叶斯优化

论文地址：

[https://arxiv.org/abs/1705.04405](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1705.04405)

GitHub地址：

[https://github.com/lacerbi/bads](https://link.zhihu.com/?target=https%3A//github.com/lacerbi/bads)

**On Tensor Train Rank Minimization: Statistical Efficiency and Scalable Algorithm**

Tensor Train等级最小化：统计效率和可扩展算法

论文地址：

[https://arxiv.org/abs/1708.00132](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1708.00132)

**PixelGAN Autoencoders**

PixelGAN自动编码器

论文地址：

[http://arxiv.org/abs/1706.00531](https://link.zhihu.com/?target=http%3A//arxiv.org/abs/1706.00531)



![](https://pic2.zhimg.com/v2-b2e00fa73b1c4794691a20d7d83393e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='336'></svg>)



好啦，先到这里。最后，还是恭喜各位。

以及，如果有更多关于这次NIPS 2017想说的东西，不管是谈谈论文，发发感想，做做总结，都欢迎联系量子位~

— 完 —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)

վ'ᴗ' ի 追踪AI技术和产品新动态




