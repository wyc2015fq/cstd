
# Some papers about NLP - lpty的博客 - CSDN博客

置顶2019年01月14日 19:38:44[lpty](https://me.csdn.net/sinat_33741547)阅读数：182



### Attention
[1、Neural Machine Translation by Jointly Learning to Align and Translate](https://blog.csdn.net/sinat_33741547/article/details/85099103)
`NLP使用attention的第一篇论文`[2、Effective Approaches to Attention-based Neural Machine Translation](https://blog.csdn.net/sinat_33741547/article/details/85293872)
`提出很多attention使用方法`[3、ABCNN: Attention-Based Convolutional Neural Network for Modeling Sentence Pairs](https://blog.csdn.net/sinat_33741547/article/details/85330080)
`attention用于CNN中`[4、Attention is all you need](https://blog.csdn.net/sinat_33741547/article/details/85884052)
`transformer，正式定义了attention`
### Representation
[1、A Neural Probabilistic Language Model](https://blog.csdn.net/sinat_33741547/article/details/78744723)
`神经网络语言模型，开山之作`[2、Efficient estimation of word representations in vector space](https://blog.csdn.net/sinat_33741547/article/details/78759289)
`word2vec，词向量
(1) Distributed Representations of Sentences and Documents`[3、Deep contextualized word representations](https://lpty-nlp.blog.csdn.net/article/details/88965753)
`ELMO，基于上下文可变的词向量`4、Improving Language Understanding by Generative PreTraining
`GPT，使用transformer作为特征抽取`[5、BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding](https://blog.csdn.net/sinat_33741547/article/details/86311310)
`BERT，nlp的ImageNet时刻`
### TextClassifier
[1、Convolutional Neural Networks for Sentence Classification](https://blog.csdn.net/sinat_33741547/article/details/84106589)
`textCNN`[2、A Sensitivity Analysis of Convolutional Neural Networks for Sentence Classification](https://blog.csdn.net/sinat_33741547/article/details/84260482)
`CNN调参指南`[3、Recurrent Neural Network for Text Classification with Multi-Task Learning](https://blog.csdn.net/sinat_33741547/article/details/84838877)
`多任务的RNN模型，直接改善模型表达`[4、A Convolutional Neural Network for Modelling Sentences](https://blog.csdn.net/sinat_33741547/article/details/84960361)
`DCNN，构造类似语法树的CNN模型`[5、Text Matching as Image Recognition](https://blog.csdn.net/sinat_33741547/article/details/80649542)
`MatchPyramid，像识别图像一样进行文本分类`[6、Deep Sentence Embedding Using Long Short-Term Memory Networks](https://blog.csdn.net/sinat_33741547/article/details/80651473)
`LSTM进行分类`[7、Bag of Tricks for Efficient Text Classification](https://blog.csdn.net/sinat_33741547/article/details/78784234)
`fasttext，小任务不需要复杂结构
(1) Enriching Word Vectors with Subword Information
(2) Joint Learning of Character and Word Embeddings`
### ResNet
1、Deep Residual Learning for Image Recognition
`正式提出resnet`[2、Identity Mappings in Deep Residual Networks](https://lpty-nlp.blog.csdn.net/article/details/88657263)
`对resnet的进一步分析`
### IR
[1、An Information Retrieval Approach to Short Text Conversation](https://blog.csdn.net/sinat_33741547/article/details/86742342)
`IR用于QA`2、Learning Deep Structured Semantic Models for Web Search using Clickthrough Data
`（1）CNN-DSSM：A Latent Semantic Model with Convolutional-Pooling Structure for Information Retrieval 
（2）LSTM-DSSM：SEMANTIC MODELLING WITH LONG-SHORT-TERM MEMORY FOR INFORMATION RETRIEVAL
（3）MV-DSSM：A Multi-View Deep Learning Approach for Cross Domain User Modeling in Recommendation Systems`
### Chatbot
1、AliMe Chat: A Sequence to Sequence and Rerank based Chatbot Engine
`阿里小蜜实现`2、The Design and Implementation of XiaoIce, an Empathetic Social Chatbot
`微软小冰机器人（social chatbot）的设计与实现`3、SDNet: Contextualized Attention-based Deep Network for Conversational Question Answering
`SDNet，基于attention结合bert解决对话问答任务`4、A Survey on Dialogue Systems: Recent Advances and New Frontiers
`对话系统综述`
### KBQA
1、Semantic Parsing on Freebase from Question-Answer Pairs
`语义解析方法baseline，2013EMNLP`2、Information Extraction over Structured Data: Question Answering with Freebase
`信息抽取方法baseline，2014ACL`3、 Question answering with subgraph embeddings
`向量建模方法baseline，2014EMNLP`4、Question Answering over Freebase with Multi-Column Convolutional Neural Networks
`深度学习用于向量建模，2015ACL`5、Semantic Parsing via Staged Query Graph Generation:Question Answering with Knowledge
`深度学习用于语义解析，2015ACL`6、Large-scale Simple Question Answering with Memory Networks
`记忆网络用于KBQA，2015`7、Question Answering over Knowledge Base with Neural Attention Combining Global Knowledge Information
`attention机制的KBQA，2016`8、Reading Wikipedia to Answer Open-Domain Questions
`阅读理解的DBQA， 2017`9、Learning to Compose Neural Networks for Question Answering
`动态组合神经网络`
### Normalization
1、Covariate Shift Adaptation by Importance Weighted Cross Validation
`ICS，正统的机器学习方式`2、Batch Normalization: Accelerating Deep Network Training by Reducing Internal Covariate Shift
`ICS，batch normalization`3、Layer Normalization
`ICS，layer normalization`4、Weight Normalization: A Simple Reparameterization to Accelerate Training of Deep Neural Networks
`ICS，weight normalization`5、Cosine Normalization: Using Cosine Similarity Instead of Dot Product in Neural Networks
`ICS，cosine normalization`
### Optimizer
[1、Reducing BERT Pre-Training Time from 3 Days to 76 Minutes](https://blog.csdn.net/sinat_33741547/article/details/89094521)
`将bert的训练时间减少到76分钟`[2、DECOUPLED WEIGHT DECAY REGULARIZATION](https://blog.csdn.net/sinat_33741547/article/details/89153794)
`提出AdamW，解耦学习率和权重衰减`
### Other
1、Semi-supervised sequence tagging with bidirectional language models
`提出双向语言模型`2、An efficient framework for learning sentence representations
`句子向量表示方法`3、Convolutional Sequence to Sequence Learning
`convS2S，精致的conv使用方法`[4、Replace or Retrieve Keywords In Documents At Scale](https://blog.csdn.net/sinat_33741547/article/details/80192004)
`FlashText，基于自动机的模式匹配方法`[5、How to Strike a Match](https://blog.csdn.net/sinat_33741547/article/details/80167616)
`一种距离算法`[6、From Word Embeddings To Document Distances](https://blog.csdn.net/sinat_33741547/article/details/80163719)
`词移距离`

