# 支持53种语言预训练模型，斯坦福发布全新NLP工具包StanfordNLP - 知乎
# 



> 今日，Stanford NLP 团队发布了包含 53 种语言预训练模型的自然语言处理工具包 StanfordNLP，该工具包支持 Python 3.6 及之后版本，并基于 PyTorch，支持多种语言的完整文本分析管道，包括分词、词性标注、词形归并和依存关系解析，此外它还提供了与 CoreNLP 的 Python 接口。
![](https://pic3.zhimg.com/v2-9cd3e118f19372244cd0b7df81ba33c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='594' height='565'></svg>)- Github: [https://github.com/stanfordnlp/stanfordnlp](https://link.zhihu.com/?target=https%3A//github.com/stanfordnlp/stanfordnlp) … 
- Paper: [https://nlp.stanford.edu/pubs/qi2018universal.pdf](https://link.zhihu.com/?target=https%3A//nlp.stanford.edu/pubs/qi2018universal.pdf) … 
- PyPI: [https://pypi.org/project/stanfordnlp/](https://link.zhihu.com/?target=https%3A//pypi.org/project/stanfordnlp/) … 

以下内容介绍了 StanfordNLP 的相关论文、安装、使用以及设置方法。

StanfordNLP 结合了斯坦福团队参加 CoNLL 2018 Shared Task on Universal Dependency Parsing 使用的软件包，和 Stanford CoreNLP 软件的官方 Python 接口。StanfordNLP 不仅提供 CoreNLP 的功能，还包含一系列工具，可将文本字符串转换为句子和单词列表，生成单词的基本形式、词性和形态特征，以及适用于 70 余种语言中的句法结构。

StanfordNLP 用高度准确的神经网络构建而成，允许使用自己的标注数据进行高效训练和评估。这些模块基于 PyTorch 构建。

StanfordNLP 的特征：
- 可以轻松设置本地 Python 实现；
- 包含进行文本分析的完整神经网络管道（neural network pipeline），包括分词、多词 token（MWT）扩展、词形归并（lemmatization）、词性（POS）和形态特征标记以及依存关系解析；
- 提供在 73 个 treebanks 上的 53 种人类语言的预训练神经模型；
- 官方维护的接入到 CoreNLP 的稳定 Python 接口。

**论文：Universal Dependency Parsing from Scratch **
![](https://pic3.zhimg.com/v2-ef471ff633bf7da688b4faee00ed8486_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='319'></svg>)
论文地址：[https://nlp.stanford.edu/pubs/qi2018universal.pdf](https://link.zhihu.com/?target=https%3A//nlp.stanford.edu/pubs/qi2018universal.pdf)

依存关系解析是用于语义作用标记的各种自然语言处理系统、关系提取和机器翻译中的重要组成部分。然而，大多数研究都已经将依赖解析单独处理，并且在很大程度上忽略了为解析器准备相关数据的上游 NLP 组件，例如标记器和词形化器。然而，实际上，这些上游系统仍然远非完美。

为此，研究者构建了一个 raw-textto-CoNLL-U 管道系统，该系统执行 Shared Task 所需的所有任务。利用深度学习，该管道在每个相互关联的任务阶段中都实现了有竞争力的性能：分词、句子和词分割、词性（POS）/形态特征（UFeats）标记、词形归并，最后是依存关系解析。

研究者表示，该工作的主要贡献包括：
- 将符号统计知识与灵活、强大的神经系统相结合以提高稳健性的新方法；
- 用于联合 POS / UFeats 预测的 biaffine 分类器，可提高预测一致性；
- 使用编辑分类器增强的词形归并工具，可提高少样本序列上的序列到序列模型的稳健性；
- 扩展解析器到模型线性化。

该系统在 big-treebanks 上实现了竞争性的表现。在修复了一个关键的错误之后，更正后的系统在官方评估指标 LAS、MLAS 和 BLEX 上获得了第 2 名、第 1 名和第 3 名，并且在所有指标上大幅度地优于低资源 treebank 类别的所有提交系统。

**实验结果**

主要结果如表 1 所示。当对所有 treebank 进行宏观平均时，该系统几乎可以在所有指标上实现竞争性能。此外，当仅在 big-treebanks 上进行评估时，它在多个指标上实现了最佳性能。即使不是表现最好的系统，该系统也会在这些 treebank 上的每个指标上取得竞争结果。该系统由单系统组件组成，而一些表现最佳的模型则使用集成方法。

此外，该系统的 UFeats 分类器在这些 treebank 上也非常准确。它不仅在 UFeats F1 上实现了最佳性能，而且还帮助解析器在 big-treebanks 上实现了最佳 MLAS，即使解析器在其他指标评估上没有取得最佳性能。研究者强调了 POS 标记器/ UFeats 分类器中的一致性建模的贡献：在两种设置中，与 AllTags 度量标准相比，各个度量标准（UPOS、XPOS 和 UFeats）在参考系统上实现了更低的 advantage margin，表明参考系统尽管有时在每个单独的任务上更准确，但并不像本研究中的整体系统那样一致。

all-treebanks 和 big-treebanks 的结果之间的最大差异来自句子分割。在检查了较小 treebank 上的结果和实现后，研究者注意到标记器中处理数据的方式对这些 treebank 的泛化产生了负面影响。这对于这些 treebank 来说是毁灭性的，因为所有下游组件都在句子水平处理单词。

研究者解决了这个问题，并在提交时训练了新的分词器，其中所有超参数都与系统相同。他们进一步构建了一个非官方的评估管道，验证了它与官方系统达到的评估结果相同，并且仅通过替换分词器来评估整个管道。如表 1 所示，由此产生的系统（Stanford+）整体上更准确，分别在官方评估指标 LAS、MLAS 和 BLEX 上排名第 2、第 1 和第 3。在 big-treebanks 上，所有指标仅在 0.02％的 F1 内变化，因此不包括在内。然而，在 small treebanks 上这种影响更为明显：如表 2 所示，校正系统在所有低资源 treebank 的所有官方评估指标上都大大优于所有提交系统。
![](https://pic4.zhimg.com/v2-94b1acddfae3f7d4780ac9c59eae6003_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='399'></svg>)
表 1：测试集上的评估结果（F1），仅适用于所有 treebank 及大型 treebank 测试集。对于所有指标上的每一组结果，研究者将其与来自参照系统的结果进行对比。参照系统是指在那个指标上当前性能最好的系统。
![](https://pic2.zhimg.com/v2-62a3bc7250fc6bdf74d5578f797434f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='650' height='302'></svg>)表 2：在低资源 treebank 测试集上的评估结果（F1）
## **安装和使用**

**设置**

StanfordNLP 支持 Python 3.6 及之后版本。推荐从 PyPI 中安装 StanfordNLP。如果已经安装了 pip，运行以下命令：

`pip install stanfordnlp`

这有助于解决 StanfordNLP 的所有依赖项，例如 PyTorch 1.0.0 及以上版本。

或者，你还可以从该 git repo 中安装 StanfordNLP，这样你可以更加灵活地基于 StanfordNLP 开发，以及训练自己的模型。运行以下命令：


```
git clone git@github.com:stanfordnlp/stanfordnlp.gitcd stanfordnlp
pip install -e .
```


## **运行 StanfordNLP**

**启动神经网络管道**

要想运行第一个 StanfordNLP pipeline，只需在 Python 交互式解释器中进行以下步骤：

`>>> import stanfordnlp>>> stanfordnlp.download('en')   # This downloads the English models for the neural pipeline>>> nlp = stanfordnlp.Pipeline() # This sets up a default neural pipeline in English>>> doc = nlp("Barack Obama was born in Hawaii.  He was elected president in 2008.")>>> doc.sentences[0].print_dependencies()`

最后一个命令将输出输入字符串中第一个句子中的单词，以及在句子的通用依存解析中控制该单词的单词索引、单词之间的依赖关系。输出如下：


```
('Barack', '4', 'nsubj:pass')
('Obama', '1', 'flat')
('was', '4', 'aux:pass')
('born', '0', 'root')
('in', '6', 'case')
('Hawaii', '4', 'obl')
('.', '4', 'punct')
```


注意：如果你遇到了 OSError: [Errno 22] Invalid argument 这样的问题，很可能是因为 Python 的问题。推荐使用 Python 3.6.8 及之后版本和 Python 3.7.2 及之后版本。

StanfordNLP 还提供多语言 demo 脚本，展示了如何在非英语语言中使用 StanfordNLP，如繁体中文。

demo 地址：[https://github.com/stanfordnlp/stanfordnlp/blob/master/demo/pipeline_demo.py](https://link.zhihu.com/?target=https%3A//github.com/stanfordnlp/stanfordnlp/blob/master/demo/pipeline_demo.py)

`python demo/pipeline_demo.py -l zh`

详见：[https://stanfordnlp.github.io/stanfordnlp/installation_download.html#getting-started](https://link.zhihu.com/?target=https%3A//stanfordnlp.github.io/stanfordnlp/installation_download.html%23getting-started)

## **访问 Java Stanford CoreNLP Server**

除了神经网络管道，该项目还包括一个官方 wrapper，允许使用 Python 代码访问 Java Stanford CoreNLP Server。

初始设置如下：
- 下载 Stanford CoreNLP 和你想使用语言的模型。
- 将 model jar 放在分发目录中。
- 在 Python 代码中写明 Stanford CoreNLP 的地址：export CORENLP_HOME=/path/to/stanford-corenlp-full-2018-10-05。

该项目提供另一个 demo 脚本，展示如何使用 CoreNLP 客户端以及如何从中提取不同的标注。

demo 地址：[https://github.com/stanfordnlp/stanfordnlp/blob/master/demo/corenlp.py](https://link.zhihu.com/?target=https%3A//github.com/stanfordnlp/stanfordnlp/blob/master/demo/corenlp.py)

**神经网络管道中的模型**

该项目目前提供 CoNLL 2018 Shared Task 中所有 treebank 的模型。模型下载和使用说明详见：

[https://stanfordnlp.github.io/stanfordnlp/installation_download.html#models-for-human-languages。](https://link.zhihu.com/?target=https%3A//stanfordnlp.github.io/stanfordnlp/installation_download.html%23models-for-human-languages%25E3%2580%2582)

**批处理以最大化 Pipeline 速度**

为了最大化速度，对文档进行批量处理是必要的。一次仅对一个句子运行 for loop 太慢了。最好的方法是将文档串联到一起，每个文档由空白行分割（即两个换行符\n\n）。分词器会将空白行识别为断句。目前，研究者正在积极寻求改善多文档处理。

**训练自己的神经网络管道**

该库中的所有神经模块，包括分词器、多词标记 (MWT) 扩展器、POS/形态特征标注器、词形归并和依存解析器，都可以用你自己的 CoNLL-U 格式数据来训练。目前，该库还不支持通过 Pipeline 接口训练模型。因此，为了训练你自己的模型，你要 clone 这个 git repo，然后从源代码进行设置。

如果想详细了解如何一步步训练和评估自己的模型，请参考以下链接：

[https://stanfordnlp.github.io/stanfordnlp/training.html](https://link.zhihu.com/?target=https%3A//stanfordnlp.github.io/stanfordnlp/training.html)[Model Training and Evaluation​stanfordnlp.github.io](https://link.zhihu.com/?target=https%3A//stanfordnlp.github.io/stanfordnlp/training.html)[https://stanfordnlp.github.io/stanfordnlp/training.html](https://link.zhihu.com/?target=https%3A//stanfordnlp.github.io/stanfordnlp/training.html)

参考原文：[https://stanfordnlp.github.io/stanfordnlp/](https://link.zhihu.com/?target=https%3A//stanfordnlp.github.io/stanfordnlp/)






