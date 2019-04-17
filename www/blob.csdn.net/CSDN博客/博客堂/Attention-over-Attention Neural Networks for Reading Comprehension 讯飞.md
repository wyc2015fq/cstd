# Attention-over-Attention Neural Networks for Reading Comprehension 讯飞 - 博客堂 - CSDN博客





2018年04月11日 11:10:51[最小森林](https://me.csdn.net/u012052268)阅读数：438








该论文是科大讯飞2016发的一篇定会论文，拜读之后翻译，并记录：


- [Attention-over-Attention Neural Networks for Reading Comprehension](#attention-over-attention-neural-networks-for-reading-comprehension)- [1 引言](#1-引言)
- [2 相关工作](#2-相关工作)- [2.1 本文的主要思路](#21-本文的主要思路)
- [2.2 本文的创新点](#22-本文的创新点)

- [3 Attention-over-Attention Reader 模型](#3-attention-over-attention-reader-模型)- [3.1 Contextual Embedding](#31-contextual-embedding)
- [3.2 Pair-wise Matching Score](#32-pair-wise-matching-score)
- [3.3 Individual Attentions](#33-individual-attentions)
- [3.4 Attention-over-Attention](#34-attention-over-attention)
- [3.5 Final Predictions](#35-final-predictions)

- [4 N最佳重新排名策略 N-best Re-ranking Strategy](#4-n最佳重新排名策略-n-best-re-ranking-strategy)
- [5 结果](#5-结果)




# Attention-over-Attention Neural Networks for Reading Comprehension

## 1 引言

阅读和理解人类语言是机器的挑战性任务，其中，填充式阅读理解问题已成为社区的热门任务。填充式查询（ cloze-style query）是一个问题，即在考虑上下文信息的同时，在给定的句子中填写适当的单词。这个单词答案是文档中的一个单词，它需人类利用文档和问题中的上下文信息 推理得来。

形式上，一般的Cloze-style式阅读理解问题可以被解释为一个三元组：D，Q，A三元组： 

- 文档D ：Mary sits beside him … he loves Mary 

- 查询Q：he loves *__*

- 查询答案A：Mary
为了教会机器进行填充式阅读理解，必需通过大规模训练数据，学习文档和查询之间的关系（document and query）。现有的数据集主要有两个： 

1. CNN /每日邮报新闻数据集（2015年），文档D由新闻报道组成，查询Q由文章摘要构成，其中一个实体词被特殊的占位符替换以指示缺失的词。被替换的实体词将是查询的答案。 

2. 儿童图书测试数据集（2016），文档D由书籍的连续20个句子生成的，并且查询由第21句形成。
## 2 相关工作

在这些数据集之后，已经有血症提出了各种各样的神经网络方法，其中大多数采用了注意力机制，它通过学习投入的“重要性”能力而广为人知。

### 2.1 本文的主要思路

本文中，我们提出了一种新的神经网络架构，称为attention-over-attention注意模型。我们可以从字面上理解含义：我们的模型旨在将另一种注意机制置于现有文档级别的注意力机制之上。

与以前的作品不同，我们的模型可以自动生成关注各种文档级别的“注意力”（document-level-attentions），所以，模型不仅可以从查询Q到文档D；还可以文档D到查询Q进行交互查看，这将从交互式信息中受益。

### 2.2 本文的创新点

综上所述，我们的工作主要贡献如下：
- 
据我们所知，这是第一次提出将注意力覆盖在现有关注点上的机制之上，即attention-over-attention mechanism。

- 
与以往有关向模型引入复杂体系结构或许多不可训练的超参数的工作不同，我们的模型要简单得多，但要大幅优于各种最先进的系统。

- 
我们还提出了N-best排名策略，以重新评估候选人的各个方面并进一步提高表现。


## 3 Attention-over-Attention Reader 模型

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/69095307.jpg)

在本文中，我们提出了一项新颖的工作，重点关注主要的“关注点”，指出每个关注点的“重要性”。

当给出完形三元训练D，Q，A时，所提出的模型将按照以下步骤构建。

### 3.1 Contextual Embedding

我们首先转换文档D中的每个单词和Q查询转换为one-hot表示，然后将它们转换为具有共享嵌入矩阵W e的连续表示。

之后，我们使用两个双向RNN来获取文档和查询的上下文表示，每个单词的表示形式通过连接前向和后向隐藏状态形成。在模型性能和训练复杂性之间进行权衡之后，我们选择门控循环单元（GRU）作为recurrent uni单元实施。步骤如下：

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/16264021.jpg)

我们取`$h_{doc}= R^{|D|*2d}$`和`$ h_{query}=R ^{|Q|*2d}$`来表示文档和查询的上下文表示，其中d是GRU（单向）的维数（然后正反拼接就是2*）。

### 3.2 Pair-wise Matching Score

在获得文档h doc和查询h query 的contextual embeddings之后，我们计算成对匹配矩阵，其表示一个文档词和一个查询词的成对匹配程度。 正式地，当给出第i个单词和第j个单词时，我们可以通过它们的点积来计算匹配分数

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/22529087.jpg)

通过这种方式，我们可以计算每个文档和查询词之间的每一对匹配分数，形成一个矩阵M∈R| D | * | Q | ，其中第i行第j列的值由M（i，j）填充。行数为文档D的长度，宽度为查询Q的长度。

### 3.3 Individual Attentions

在计算成对匹配矩阵M后，我们应用逐列（column-wise）softmax函数来获得每列中的概率分布，其中当考虑单个查询词时，每列是单独的文档级关注。 我们表示α（t）∈R |D| 作为在第t个查询词的文档级关注，这可以被视为query-to-document的关注。

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/40676386.jpg)

### 3.4 Attention-over-Attention

我们引入另一种注意机制来自动确定每个individual的注意力的重要性，而不是使用简单的启发式方法（如求和或平均）来将这些个体注意力集中到最后的注意力上。

首先，我们计算反向注意力，即对于时间t的每个文档词，我们计算查询中的“重要性”分布，以指出给定单个文档词时哪些查询词更重要。 我们将逐行方式的softmax函数应用于成对匹配矩阵M，以获得查询级别的关注。 我们表示β（t）∈R | Q | 作为关于时间t处的文档词的查询级关注，这可以被视为文档到查询的关注。(对M横向求softmax)

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/86194916.jpg)

到目前为止，我们已经获得了查询到文档的关注度α（把M竖着soft）和文档到查询的关注度β（把M横着soft）。 我们的动机是利用文档和查询之间的互信息。 。

然后我们对所有β（t）进行平均以得到平均的查询级别关注β。 请注意，我们不会将另一个softmax应用于β，因为平均个别关注不会破坏正常化条件。

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/71874439.jpg)

最后，我们计算α和β的点积得到“attended document-level attention”s∈R| D | 即attention-over-attention机制。 直观地说，该操作是在时间t查看查询词时计算每个单独文档级关注度α（t）的加权总和。 通过这种方式，可以明确地了解每个查询词的贡献，并且通过每个查询词的重要性通过投票结果来做出最终决定（文档级关注）。

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/97242101.jpg)

### 3.5 Final Predictions

我们使用sum attention mechanism机制来获得汇总结果。

![](http://oqy7bjehk.bkt.clouddn.com/18-4-10/28600291.jpg)

## 4 N最佳重新排名策略 N-best Re-ranking Strategy

直观地说，当我们人类进行填充式阅读理解时，我们经常将候选人填充到查询的空白处，以仔细检查其适当性，流畅性和语法，看看我们选择的候选人是否是最合适的候选人。如果我们确实在我们选择的最佳候选人中发现了一些问题，我们会选择第二个候选人并再次进行检查。

为了模仿双重检查的过程，我们建议在从我们的神经网络生成答案后使用N最佳重排序策略。程序可以说明如下。
- N-best解码

我们以在解码过程中提取后续候选项，而形成N个最佳列表。而不是仅挑选出具有最高可能性的候选项作为回答。
- 将候选人填入查询

作为填充式问题的一个特征，每个候选人都可以填充到查询的空白处以形成一个完整的句子。 这使我们能够根据其上下文来检查候选人。 

- 功能评分

候选句子可以在许多方面得分。在本文中，我们利用三个特征来评分N-最佳名单。

•Global N-gram LM：这是评分句子的基本指标，旨在评估其流利程度。 该模型在训练数据的文档部分进行了训练。

•Local N-gram LM：与全球LM不同，本地LM旨在通过给定文档来探索信息，因此统计信息是从测试时间文档中获得的。 应该注意的是，本地LM是逐个样本进行训练的，它并未在整个测试集上进行训练，这在真实测试案例中是不合法的。 当测试样本中有许多未知单词时，此模型非常有用。

•词类LM：与全局LM类似，词类LM也在训练数据的文档部分进行训练，但是这些词被转换为它的词类ID。可以通过使用聚类方法获得该词类。 在本文中，我们简单地使用mkcls工具来生成1000个词类。
- 重新评分和重新排名

我们计算N个最佳句子中每个特征的加权和，然后选择成本最低的候选者作为最终答案。

## 5 结果

提升了3%





