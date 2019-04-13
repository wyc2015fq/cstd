
# 8种优秀预训练模型大盘点，NLP应用so easy！ - 喜欢打酱油的老鸟 - CSDN博客


2019年04月03日 08:25:55[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：54


[https://www.toutiao.com/a6674855848902132235/](https://www.toutiao.com/a6674855848902132235/)
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p1.pstatp.com/large/pgc-image/de5fe7894e254810b176f427337ce525)

**大数据文摘出品**
**编译：李雷、蔡婕**
如今，自然语言处理（NLP）可谓遍地开花，可以说正是我们了解它的好时机。
NLP的快速增长主要得益于通过预训练模型实现转移学习的概念。在NLP中，转移学习本质上是指在一个数据集上训练模型，然后调整该模型以便在不同数据集上实现NLP的功能。
这一突破使NLP应用变得如此简单，尤其是那些没有时间或资源从头开始构建NLP模型的人。或者，对于想要学习或者从其他领域过渡到NLP的新手来说，这简直就是完美。
**为什么要使用预训练模型？**
模型的作者已经设计出了基准模型，这样我们就可以在自己的NLP数据集上使用该预训练模型，而无需从头开始构建模型来解决类似的问题
尽管需要进行一些微调，但这为我们节省了大量的时间和计算资源
在本文中展示了那些助你开始NLP之旅的顶级预训练模型，以及该领域的最新研究成果。要查看关于计算机视觉中的顶级预训练模型的文章，请参阅：
https://www.analyticsvidhya.com/blog/2018/07/top-10-pretrained-models-get-started-deep-learning-part-1-computer-vision/?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
**本文涵盖的NLP预训练模型**
我根据应用场景将预训练模型分为三类：
多用途NLP模型
ULMFiT
Transformer
谷歌的BERT
Transformer-XL
OpenAI的GPT-2
词嵌入NLP模型
ELMo
Flair
其他预训练模型
StanfordNLP
**多用途NLP模型**
多用途模型在NLP领域里一直为人们所关注。这些模型为提供了许多令人感兴趣的NLP应用 - 机器翻译、问答系统、聊天机器人、情感分析等。这些多用途NLP模型的核心是语言建模的理念。
简单来说，语言模型的目的是预测语句序列中的下一个单词或字符，在我们了解各模型时就会明白这一点。
如果你是NLP爱好者，那么一定会喜欢现在这部分，让我们深入研究5个最先进的多用途NLP模型框架。这里我提供了每种模型的研究论文和预训练模型的链接，来探索一下吧！
**ULMFiT模型**
ULMFiT由fast.ai（深度学习网站）的Jeremy Howard和DeepMind（一家人工智能企业）的Sebastian Ruder提出并设计。可以这么说，ULMFiT开启了转移学习的热潮。
正如我们在本文中所述，ULMFiT使用新颖的NLP技术取得了令人瞩目的成果。该方法对预训练语言模型进行微调，将其在WikiText-103数据集（维基百科的长期依赖语言建模数据集Wikitext之一）上训练，从而得到新数据集，通过这种方式使其不会忘记之前学过的内容。
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p1.pstatp.com/large/pgc-image/b8803bb481fa41eea09afd8482dfa596)

ULMFiT比许多先进的文本分类模型还要好。我喜爱ULMFiT是因为它只需要很少的数据就可以来产生令人印象深刻的结果，使我们更容易理解并在机器上实现它！
也许你不知道ULMFiT其实是Universal Language Model Fine-Tuning（通用语言模型微调）的简称。“通用”一词在这里非常贴切 - 该框架几乎可以应用于任何NLP任务。
想知道有关ULMFiT的更多信息，请参阅以下文章和论文：
使用ULMFiT模型和Python 的fastai库进行文本分类（NLP）教程
https://www.analyticsvidhya.com/blog/2018/11/tutorial-text-classification-ulmfit-fastai-library/?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
ULMFiT的预训练模型论文
https://www.paperswithcode.com/paper/universal-language-model-fine-tuning-for-text
其他研究论文
https://arxiv.org/abs/1801.06146
**Transformer模型**
Transformer架构是NLP近期最核心的发展，2017年由谷歌提出。当时，用于语言处理任务（如机器翻译和问答系统）的是循环神经网络（RNN）。
Transformer架构的性能比RNN和CNN（卷积神经网络）要好，训练模型所需的计算资源也更少，这对于每个使用NLP的人来说都是双赢。看看下面的比较：
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p9.pstatp.com/large/pgc-image/2b37ede017a847418f18b948b9cc11f5)

各模型的英德翻译质量
根据Google的说法，Transformer模型“应用了一种自注意力机制，可直接模拟句子中所有单词之间的关系，而无需理会其各自的位置如何”。它使用固定长度上下文（也就是前面的单词）来实现。太复杂了？没事，我们举一例子简单说明。
有句话“She found the shells on the bank of the river.”此时模型需要明白，这里的“bank”是指岸边，而不是金融机构（银行）。Transformer模型只需一步就能理解这一点。我希望你能阅读下面链接的完整论文，以了解其工作原理。它肯定会让你大吃一惊。
下面的动画很好地说明了Transformer如何处理机器翻译任务：
谷歌去年发布了一款名为Universal Transformer的改进版Transformer模型。它还有一个更新，更直观的名字，叫Transformer-XL，我们将在后面介绍。
想学习和阅读更多有关Transformer的信息，请访问：
谷歌官方博文
https://ai.googleblog.com/2017/08/transformer-novel-neural-network.html
Transformer预训练模型论文《Attention Is All You Need》
https://www.paperswithcode.com/paper/attention-is-all-you-need
其他研究论文
https://arxiv.org/abs/1706.03762
**BERT模型（谷歌）**
谷歌发布BERT框架并开放其源代码在业界掀起波澜，甚至有人认为这是否标志着“ NLP新时代”的到来。但至少有一点可以肯定，BERT是一个非常有用的框架，可以很好地推广到各种NLP任务中。
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p3.pstatp.com/large/pgc-image/57353ba88f5d445fae2dfeb8a02e8273)

BERT是Bidirectional Encoder Representations（双向编码器表征）的简称。这个模型可以同时考虑一个词的两侧（左侧和右侧）上下文，而以前的所有模型每次都是只考虑词的单侧（左侧或右侧）上下文。这种双向考虑有助于模型更好地理解单词的上下文。此外，BERT可以进行多任务学习，也就是说，它可以同时执行不同的NLP任务。
BERT是首个无监督的、深度双向预训练NLP模型，仅使用纯文本语料库进行训练。
在发布时，谷歌称BERT进行了11个自然语言处理（NLP）任务，并产生高水平的结果，这一壮举意义深远！你可以在短短几个小时内（在单个GPU上）使用BERT训练好自己的NLP模型（例如问答系统）。
想获得更多有关BERT的资源，请参阅：
谷歌官方博文
https://ai.googleblog.com/2018/11/open-sourcing-bert-state-of-art-pre.html
BERT预训练模型论文
https://www.paperswithcode.com/paper/bert-pre-training-of-deep-bidirectional\#code
其他研究论文
https://arxiv.org/pdf/1810.04805.pdf
**Transformer-XL模型（谷歌）**
从长期来看，谷歌发布的版本对NLP而言是非常重要的。如果你是初学者，这个概念可能会有点棘手，所以我鼓励你多读几遍来掌握它。我还在本节下面提供了多种资源来帮助你开始使用Transformer-XL。
想象一下——你刚读到一本书的一半，突然出现了这本书开头提到的一个词或者一句话时，就能回忆起那是什么了。但可以理解，机器很难建立长期的记忆模型。
如上所述，要达成这个目的的一种方法是使用Transformers，但它们是在固定长度的上下文中实现的。换句话说，如果使用这种方法，就没有太大的灵活性。
Transformer-XL很好地弥补了这个差距。它由Google AI团队开发，是一种新颖的NLP架构，能够帮助机器理解超出固定长度限制的上下文。Transformer-XL的推理速度比传统的Transformer快1800倍。
通过浏览下面谷歌发布的两个gif文件，你就会明白这其中的区别:
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p3.pstatp.com/large/pgc-image/682cc40ae6134fef90a610b33059cf05)

![8种优秀预训练模型大盘点，NLP应用so easy！](http://p1.pstatp.com/large/pgc-image/8b9a5bb4385b45d0a18fe1ebda16370b)

正如你现在可能已经预测到的，Transformer-XL在各种语言建模基准/数据集上取得了最新的技术成果。以下是他们页面上的一个小表格，说明了这一点:
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p9.pstatp.com/large/pgc-image/7d6d8a8952b048d790d22abecda49cd5)

之前给过链接并将在下面提到的Transformer-XL GitHub存储库包含了PyTorch和TensorFlow中的代码。
学习和阅读更多Transformer-XL有关信息的资源：
谷歌的官方博客文章
https://ai.googleblog.com/2019/01/transformer-xl-unleashing-potential-of.html
Transformer-XL的预训练模型
https://www.paperswithcode.com/paper/transformer-xl-attentive-language-models
研究论文
https://arxiv.org/abs/1901.02860
**GPT-2模型（OpenAI）**
这是一个十分有争议的模型，一些人会认为GPT-2的发布是OpenAI的营销噱头。我可以理解他们的想法，但是我认为至少应该要先对OpenAI发布的代码进行尝试。
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p3.pstatp.com/large/pgc-image/0043ccb5c81644b6989c5d160c1e75c6)

首先，为那些不知道我在说什么的人提供一些背景信息。OpenAI在2月份发表了一篇博客文章，他们声称已经设计了一个名为GPT-2的NLP模型，这个模型非常好，以至于担心被恶意使用而无法发布完整的版本，这当然引起了社会的关注。
GPT-2经过训练，可以用来预测40GB的互联网文本数据中的下一个出现的词。 该框架也是一个基于transformer的模型，而这个模型是基于800万个web页面的数据集来进行训练。他们在网站上发布的结果简直令人震惊，因为该模型能够根据我们输入的几个句子编写出一个完整的故事。看看这个例子：
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p1.pstatp.com/large/pgc-image/5bf28349fde5428c8ed1b46ce7b20470)

难以置信，是吧？
开发人员已经发布了一个更小版本的GPT-2，供研究人员和工程师测试。原始模型有15亿个参数——开放源码示例模型有1.17亿个参数。
学习和阅读更多GPT-2有关信息的资源：
OpenAI的官方博客文章
https://openai.com/blog/better-language-models/
GPT-2的预训练模型
https://github.com/openai/gpt-2
研究论文
https://d4mucfpksywv.cloudfront.net/better-language-models/language-models.pdf
**词嵌入（word embedding）模型**
我们使用的大多数机器学习和深度学习算法都无法直接处理字符串和纯文本。这些技术要求我们将文本数据转换为数字，然后才能执行任务（例如回归或分类）。
因此简单来说， 词嵌入（word embedding）是文本块，这些文本块被转换成数字以用于执行NLP任务。词嵌入（word embedding）格式通常尝试使用字典将单词映射到向量。
你可以在下面的文章中更深入地了解word embedding、它的不同类型以及如何在数据集中使用它们。如果你不熟悉这个概念，我认为本指南必读：
对词嵌入的直观理解：从计算向量到Word2Vec
https://www.analyticsvidhya.com/blog/2019/03/pretrained-models-get-started-nlp/
在本节中，我们将介绍NLP的两个最先进的词嵌入（word embedding）。我还提供了教程链接，以便你可以对每个主题有实际的了解。
**ELMo模型**
这个ELMo并不是《芝麻街》里的那个角色，但是这个ELMo（Embeddings from Language Models（语言模型嵌入）的缩写）在构建NLP模型的上下文中非常有用。
ELMo是一种用向量和嵌入表示单词的新方法。这些ELMo 词嵌入（word embedding）帮助我们在多个NLP任务上实现最先进的结果，如下图所示:
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p1.pstatp.com/large/pgc-image/dea6428f8588446a88d6561623554ebe)

让我们花点时间来了解一下ELMo是如何工作的。回想一下我们之前讨论过的双向语言模型。从这篇文章中我们能够得到提示，“ELMo单词向量是在双层双向语言模型（biLM）的基础上进行计算的。这个biLM模型有两层叠加在一起，每一层都有2个通道——前向通道和后向通道：
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p1.pstatp.com/large/pgc-image/95e599d554ab43eaa122ede25f97538e)

ELMo单词表示考虑计算词嵌入（word embedding）的完整输入语句。因此，“read”这单词在不同的上下文中具有不同的ELMo向量。这与旧版的词嵌入（word embedding）大不相同，旧版中无论在什么样的上下文中使用单词“read”，分配给该单词的向量是相同的。
学习和阅读更多ELMo有关信息的资源：
循序渐进的NLP指南，了解ELMo从文本中提取特征
https://www.analyticsvidhya.com/blog/2019/03/learn-to-use-elmo-to-extract-features-from-text/?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
预训练模型的GitHub存储库
https://github.com/allenai/allennlp/blob/master/tutorials/how_to/elmo.md
研究论文
https://arxiv.org/pdf/1802.05365.pdf
**Flair模型**
Flair不是一个词嵌入（word embedding），而是它的组合。我们可以将Flair称为结合了GloVe、BERT与ELMo等嵌入方式的NLP库。Zalando Research的优秀员工已经开发了开源的Flair。
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p3.pstatp.com/large/pgc-image/efc170c281f347d69006de5d377c577b)

该团队已经为以下NLP任务发布了几个预训练模型：
名称 - 实体识别（NER）
词性标注（PoS）
文本分类
培训定制模型
不相信吗？那么，这个对照表会帮你找到答案:
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p1.pstatp.com/large/pgc-image/bb83ecf5fb3d4e609f8b8545855913f5)

“Flair Embedding”是Flair库中打包的签名嵌入，它由上下文字符串嵌入提供支持。了解支持Flair的核心组件可以阅读这篇文章：
https://www.analyticsvidhya.com/blog/2019/02/flair-nlp-library-python/?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
我特别喜欢Flair的地方是它支持多种语言，而这么多的NLP发行版大多都只有英文版本。如果NLP要在全球获得吸引力，我们需要在此基础上进行扩展。
学习和阅读更多有关Flair的资源：
Flair for NLP简介：一个简单但功能强大的最先进的NLP库
https://www.analyticsvidhya.com/blog/2019/02/flair-nlp-library-python/?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
Flair的预训练模型
https://github.com/zalandoresearch/flair
**其他预训练模型**
StanfordNLP （斯坦福）
![8种优秀预训练模型大盘点，NLP应用so easy！](http://p3.pstatp.com/large/pgc-image/f3586a918a9f4678a0834bdd0abd691b)

提到扩展NLP使其不局限于英语，这里有一个已经实现该目的的库——StanfordNLP。其作者声称StanfordNLP支持超过53种语言，这当然引起了我们的注意。
我们的团队是第一批使用该库并在真实数据集上发布结果的团队之一。我们通过尝试，发现StanfordNLP确实为在非英语语言上应用NLP技术提供了很多可能性，比如印地语、汉语和日语。
StanfordNLP是一系列经过预先训练的最先进的NLP模型的集合。这些模型不仅是在实验室里进行测试——作者在2017年和2018年的CoNLL竞赛中都使用了这些模型。在StanfordNLP中打包的所有预训练NLP模型都是基于PyTorch构建的，可以在你自己的注释数据上进行训练和评估。
我们认为你应该考虑使用StanfordNLP的两个主要原因是：
用于执行文本分析的完整神经网络管道，包括。
标记化
多字令牌（MWT）拓展
词形还原
词性（POS）和词形特征标记
依存语法分析
稳定的Stanford CoreNLP软件的官方Python接口。
学习和阅读更多StanfordNLP有关信息的资源：
StanfordNLP简介：一个不可思议的支持53种语言的最先进NLP库 （使用Python代码）
https://www.analyticsvidhya.com/blog/2019/02/stanfordnlp-nlp-library-python/?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
StanfordNLP的预训练模型
https://github.com/stanfordnlp/stanfordnlp
**尾注**
这绝不是一个预训练NLP模型的详尽列表，有更多能用的可以在这个网站上找到：https://paperswithcode.com
以下是学习NLP的一些有用资源：
使用Python课程进行自然语言处理https://courses.analyticsvidhya.com/courses/natural-language-processing-nlp?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
认证项目：NLP初学者https://courses.analyticsvidhya.com/bundles/nlp-combo?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
自然语言处理（NLP）系列文章https://www.analyticsvidhya.com/blog/category/nlp/?utm_source=blog&utm_medium=top-pretrained-models-nlp-article
我很想听听你对这份清单的看法。你以前用过这些预训练过的模型吗？或者你已经探索过其他的模型？请在下面的评论区告诉我——我很乐意搜索它们并添加到这个列表中。
相关报道：
https://www.analyticsvidhya.com/blog/2019/03/pretrained-models-get-started-nlp/

