# 解密谷歌Gmail新功能：结合BoW模型和RNN-LM，帮助用户快速写邮件 - 知乎
# 



选自Google AI，作者：Yonghui Wu，机器之心编译。

> 谷歌在上周的 I/O 大会上，推出了 Gmail 新功能 Smart Compose，其结合了 BoW 模型和 RNN-LM，在用户打字时提出建议，帮助完成句子，提高电子邮件的写作速度。

在上周的 I/O 大会上，谷歌推出了 Gmail 新功能——Smart Compose，该功能使用机器学习在用户打字时提出建议，帮助用户完成句子，从而提高电子邮件的写作速度。Smart Compose 基于为 Smart Reply 开发的技术，为电子邮件写作提供了一种新的方式——无论是回复收到的电子邮件还是从头写作新邮件。
![](https://pic2.zhimg.com/v2-d7578541e0cb5b0ca9fd05c5e6dabfc5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='457'></svg>)



开发 Smart Compose 过程中面临的主要挑战包括：
- 延迟：Smart Compose 基于每次击键提供预测，因此它必须在 100 毫秒内做出理想响应，用户才不会注意到任何延迟。如何平衡模型复杂度和推断速度是一个关键问题。
- 规模：Gmail 有超过 14 亿用户。要为所有用户提供有效的自动填充建议，模型必须具备足够的建模能力，才能够在不同的语境中提出恰当的建议。
- 公平性和隐私性：在开发 Smart Compose 时，谷歌需要解决训练过程中潜在偏见的源头，还必须遵守与 Smart Reply 相同的严格用户隐私标准，确保模型不会泄露用户的隐私信息。此外，研究人员无法访问电子邮件，这意味着他们必须开发、训练一个机器学习系统来处理他们无法读取的数据集。




**找到合适的模型**

典型的语言生成模型，如 n-gram、神经词袋模型（BoW）和 RNN 语言模型（RNN-LM），基于前面的单词序列学习预测下一个单词。但是，用户在当前电子邮件中正在打出的单词只是模型可用于预测下一个单词的「信号」。为了整合用户想表达内容的更多语境，谷歌的模型还基于电子邮件主题和之前的电子邮件主体内容（如果用户正在回复收到的电邮的话）。

包含这一额外语境的一种方法是将该问题看作序列到序列（seq2seq）机器翻译任务，其中源句子是主题和之前电子邮件主体的结合，目标序列是用户正在写的当前邮件。尽管该方法在预测质量方面表现良好，但它远远无法满足谷歌严格的延迟约束。

为了改善这种情况，谷歌研究者将 BoW 模型与 RNN-LM 结合起来，其速度快于 seq2seq 模型，且仅出现轻微的模型预测质量损失。在这种混合方法中，谷歌研究者通过把每个字段中的词嵌入取平均，对主题和之前的电子邮件进行编码。然后在每个解码步中，将这些平均词嵌入输入到目标序列 RNN-LM。模型架构如下图所示。
![](https://pic4.zhimg.com/v2-d37b5a610719561e5f1bc0b7ac574633_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='237'></svg>)Smart Compose RNN-LM 模型架构。通过对每个字段中的词嵌入取平均，对主题和之前的电子邮件信息进行编码。然后在每个解码步将平均词嵌入输入到 RNN-LM
**加速模型训练 & 服务**

当然，确定使用该建模方法后，谷歌仍然需要调整不同的模型超参数，并在数十亿样本上训练模型，这些样本需要消耗大量时间。为了加速，谷歌使用完整的 TPUv2 Pod 进行实验，能够在不到一天的时间中训练模型至收敛。

即使在训练较快的混合模型之后，在标准 CPU 上运行的 Smart Compose 最初版本仍然具备数百毫秒的平均服务延迟，这对于一个试图节约用户时间的功能来说是不可接受的。幸运的是，TPU 在推断时间可以使用，从而极大地加速用户体验。通过将大量计算卸载到 TPU 上，谷歌将平均延迟减少到几十毫秒，同时极大地提高了单个机器可服务的请求数量。




**公平性和隐私**

机器学习中的公平性非常重要，因为理解语言的模型可以反映出人类的认知偏见，从而导致惹人厌的词关联和句子。正如 Caliskan 等人在其近期论文《Semantics derived automatically from language corpora contain human-like biases》中所指出的，这些关联与自然语言数据有很深的纠缠，这是构建语言模型的挑战。谷歌正在积极探索继续减少训练步骤中的潜在偏见的方式。同时，由于 Smart Compose 在数十亿词组和句子上训练而成，类似于垃圾邮件机器学习模型的训练方式，因此谷歌已经进行大量测试，以确保只有多个用户使用的共同词组才会被模型记住，此举使用了论文《The Secret Sharer: Measuring Unintended Neural Network Memorization & Extracting Secrets》的研究成果。




**未来工作**

谷歌一贯致力于通过先进架构（如 Transformer、RNMT+等）提高语言生成模型的建议质量，并使用最近、最先进的训练技术进行实验。一旦这些模型满足谷歌严格的延迟约束，谷歌将把它们部署到生产。谷歌还研究整合人类语言模型，旨在更准确地在其系统中模仿人类的写作风格。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://ai.googleblog.com/2018/05/smart-compose-using-neural-networks-to.html](https://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/05/smart-compose-using-neural-networks-to.html)


