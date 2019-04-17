# Natural language processing: Deep Neural networks with multitask learning - 家家的专栏 - CSDN博客





2014年07月14日 21:08:05[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1443








**In**: A sentence

**Output**: a host of language processing predictions: part-of-speech tags, chunks, with a language model

The network is trained **jointly **on all these tasks using weight-sharing, an instance of
**multitask learning**.  (using labeled data)

Deep neural network   Trained jointly all of these tasks 

**Semi-supervised learning** for the shared tasks (using unlabeled text). --->the language model




Natural language processing tasks:

     Part of speech tagging; Chunking; Named entity recognition; Semantic role labeling; Language model; Semantically related words.

**The architecture of deep neural network**

![](https://img-blog.csdn.net/20140714205035742)





In order to handle sequences of variable length, the simplest solution is to use a w**indow approach:**

Consider a window of fixed size *ksz *around each work we want to label.  (NO)

Time-Delay Neural networks (TDNNs)















