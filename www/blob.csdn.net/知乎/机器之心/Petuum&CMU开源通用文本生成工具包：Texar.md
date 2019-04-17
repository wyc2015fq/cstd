# Petuum&CMU开源通用文本生成工具包：Texar - 知乎
# 



**选自Petuum，机器之心编译，参与：张倩、王淑婷。**

> 为促成不同文本生成任务的快速搭建以及任务之间的技术共享，由 Petuum 公司和卡耐基梅隆大学 (CMU) 为主要贡献者的通用工具包——Texar 正式开源。该工具包支持广泛的机器学习应用，重点聚焦于文本生成任务，尤其适合从事快速模型原型设计和实验的研究人员及从业者。

## **文本生成简介**

文本生成旨在基于输入数据或机器表示生成自然语言, 包括广泛的自然语言处理（NLP）任务，例如机器翻译、对话系统、文本摘要、文章写作、文本复述及修改、图像加注等。尽管由于深度学习方法的整合，该领域在学术和产业方面都发展迅速，但为了改进技术、真正实现在现实世界的应用，还需要投入巨大的科研力量。

文本生成任务有很多共同的属性及两个中心目标：
- 生成人类水平的、合乎语法的可读文本。
- 生成包含所有从输入中推断出的相关信息的文本。例如，在机器翻译中，生成的翻译句子必须与原句表达相同的含义。

为了达到这些目标，几种关键技术的应用越来越广泛，如神经编码-解码器、注意力机制、记忆网络、对抗方法、强化学习、结构化监督，以及优化、数据预处理、结果后处理和评价等。这些技术经常以多种不同的方式结合在一起，解决不同的问题（见图 1）。
![](https://pic2.zhimg.com/v2-5e057998ae2294fdbcb80e14992a1a6d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='865' height='247'></svg>)
图 1：文本生成任务中用到的几种模型架构示例。E：编码器（encoder）；D：解码器（decoder）；C：分类器（classifier）；A：注意力（attention）；Prior：先验分布（prior distribution）；M：记忆（memory）。

因此需要一个开源平台将这些不同但紧密相关的文本生成应用整合起来，提供核心算法的清晰、一致的实现。这一整合平台将实现不同算法之间共有要素的重复使用；将设计、实现及实验标准化；促进可复现的研究；特别是促成不同文本生成任务之间的技术共享---基于此，为特定任务改进的算法将得以迅速评估并泛化到许多其他任务。

## **Texar 简介**

为此，我们开发了 Texar，一个专注于文本生成任务的开源工具包，使用 TensorFlow 语言。基于模块化、多功能及可扩展的核心设计理念，Texar 提取了文本生成中不同任务和方法的共同模式，并创建了一个高度可重复利用的模块和功能库。
![](https://pic4.zhimg.com/v2-0b460354dc2878d6f802094e2da03f0b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='865' height='388'></svg>)图 2：Texar 的主要模块和功能
**多功能性**

Texar 包含广泛的模块和功能，用于组成任意的模型架构并实现各种学习算法，如最大似然估计、强化学习、对抗性学习、概率建模等（图 2）。




**模块化**

Texar 将各种复杂的机器学习模型/算法分解成高度可重复利用的模型架构、损失函数和学习过程模块等。

用户可以像组装积木一样组装 Texar 的模块，轻松直观地构建任意自己想要的模型。模块化的设计使得各模块的插入和替换变得简单，例如，在最大似然学习和强化学习之间切换只需改变几行代码即可。




**可扩展性**

Texar 可以轻松地与任何用户定制的外部模块集成，并且与 TensorFlow 开源社区完全兼容，包括 TensorFlow 本身的接口、功能和其他资源都能直接与 Texar 一起使用。




**可用性**

使用 Texar，用户既可以通过简单的 Python/YAML 配置文件来定制已有的模型, 也可以通过 Texar Python API 编程实现任意复杂的模型，达到最大的可定制性。

Texar 提供了自动的变量重用机制（无需担心复杂的 TensorFlow 变量范围）、简单的函数式调用来执行各模块的逻辑功能，而且每个模块都有丰富的配置选项和合理的默认值。

Texar 本身的代码有良好的结构、统一的设计模式、一致的代码风格，可读性强。Texar 也提供了清晰的文档和丰富的教程示例。

Texar 目前已用于支持 Petuum 公司的若干个研究和工程项目。他们希望这一工具包也能帮助社区加速文本生成及其它技术的发展，并欢迎研究人员及从业者的加入社区，进一步丰富这一工具，共同推进文本生成研究及相关应用的进展。

打开以下链接，了解更多关于 Texar 的信息：
- 网站：[https://texar.io](https://link.zhihu.com/?target=https%3A//texar.io)
- GitHub：[https://github.com/asyml/texar](https://link.zhihu.com/?target=https%3A//github.com/asyml/texar)
- 示例：[https://github.com/asyml/texar/blob/master/examples](https://link.zhihu.com/?target=https%3A//github.com/asyml/texar/blob/master/examples)
- 文档：[https://texar.readthedocs.io/](https://link.zhihu.com/?target=https%3A//texar.readthedocs.io/)
- 博客: [https://medium.com/@texar](https://link.zhihu.com/?target=https%3A//medium.com/%40texar)
- 技术报告：[https://arxiv.org/pdf/1809.00794.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1809.00794.pdf)




原文链接：[https://medium.com/@Petuum/introducing-texar-a-modularized-versatile-and-extensible-toolkit-for-text-generation-and-beyond-b4e3289d5205](https://link.zhihu.com/?target=https%3A//medium.com/%40Petuum/introducing-texar-a-modularized-versatile-and-extensible-toolkit-for-text-generation-and-beyond-b4e3289d5205)




