# 系统学习NLP（十七）--文本相似度 - 工作笔记 - CSDN博客





2019年03月13日 20:23:08[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：183








转自：[https://blog.csdn.net/qq_28031525/article/details/79596376](https://blog.csdn.net/qq_28031525/article/details/79596376) 看原文吧，这里公式改不过来，烂

　　在自然语言处理(Natural Language Processing, NLP)中，经常会涉及到如何度量两个文本的相似度问题。在诸如对话系统(Dialog system)和信息检索(Information retrieval)等的问题中，如何度量句子或者短语之间的相似度尤为重要。为方便对知识的梳理，写下这篇博客对部分传统方法、词向量、深度学习方法进行总结，遗漏之处还请大家补充。 

　　度量文本相似度包括如下三种方法：一是基于关键词匹配的传统方法，如N-gram相似度；二是将文本映射到向量空间，再利用余弦相似度等方法；三是深度学习的方法，如基于用户点击数据的深度学习语义匹配模型DSSM，基于卷积神经网络的ConvNet，以及目前state-of-art的Siamese LSTM等方法。 

　　随着深度学习的发展，文本相似度的各种方法已经逐渐不再是基于关键词匹配的传统方法，而转向深度学习，目前结合向量表示的深度学习使用较多。
#### 1 基于关键词匹配

##### 1.1 N-gram 相似度

　　基于N-Gram模型定义的句子（字符串）相似度是一种模糊匹配方式，通过两个长得很像的句子间的“差异”来衡量相似度。 

　　N-Gram相似度的计算是指按长度N切分原句得到词段，也就是原句中所有长度为N的子字符串。对于两个句子S和T，则可以从共有子串的数量上去定义两个句子的相似度。
　　


分别表示字符串S和T中N-Gram的集合，N一般取2或3。字符串距离越近，它们就越相似，当两个字符串完全相等时，距离为0。


##### 1.2 Jaccard 相似度

　　Jaccard相似度的计算相对简单，原理也容易理解。就是计算两个句子之间词集合的交集和并集的比值。该值越大，表示两个句子越相似，在涉及大规模并行运算的时候，该方法在效率上有一定的优势，公式如下： 






#### 2 基于向量空间

　　关于如何表达词意目前有三种方法，第一种的代表就是**WordNet**，它提供了一种词的分类资源但是缺少了词之间的细微区别，同时它也很难计算词之间的相似度；第二种就是**Discrete representation**，如One-hot representation，它的向量维度和词典的长度相同，因此向量维度可能十分高，同时由于向量之间本身正交，无法计算词之间的相似度；第三种就是**Distributed representation**，基本想法就是将每个词映射为一个固定长度的短向量（相对于one-hot representation而言），所有这些词构成一个词向量空间，每一个向量视为空间中的一个点，在这个空间引入“距离”，就可以根据词之间的距离来判断它们之间的相似性，代表方法如word2vec, LDA等。

##### 2.1 Word2vec

　　早在2003年Bengio等人在《A neural probabilistic language model》中提出的神经网络语言模型（NNLM）就用到了重要的工具-词向量，只是在这个模型中，目标是生成语言模型，词向量只是一个副产品。 

　　Mikolov et al. (2013)提出的word2vec核心思想是认为词的意思可以从它频繁出现的上下文信息中体现（A word’s meaning is given by the words that frequently appear close-by）。 

　　Word2Vec又可分为两种结构，一种是CBOW，利用窗口内的上下文预测中心词；另一种是Skip-gram，利用中心词预测窗口内的上下文。 

　　以Skip-gram为例，其结构如下： 
![这里写图片描述](https://img-blog.csdn.net/20180319105252559?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　目标函数（Object function） 
![这里写图片描述](https://img-blog.csdn.net/2018031910521746?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　下面是Skip-gram的整体框架图，其中W为V*N维矩阵，W’为N*V维矩阵，这两个矩阵都有V个向量，表示词表中的每个词都有2个向量，一个是作为中心词的向量，一个是作为上下文时的向量，这两个向量都是模型中的参数，最终的词向量可以将这两个向量拼接或者直接相加。 
![这里写图片描述](https://img-blog.csdn.net/20180319105454540?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　当然这里对Word2Vec的介绍十分粗略，限于篇幅有限，很多细节和trick等无法展现，以后单独写一篇介绍word2vec的博客。
##### 2.2 TF-IDF

**TF-IDF的核心思想**是：在一篇文章中，某个词语的重要性与该词语在这篇文章中出现的次数成正相关，同时与整个语料库中出现该词语的文章数成负相关. 
**TF(term frequency)**：词频，某一个给定的词语在该文件中出现的频率，表示一个词语与该文章的相关性。这个数字是对**词数（term count）**的归一化，以防止它偏向长的文件。 
**IDF(inverse document frequency)**：逆向文件词频，表示一个词语出现的普遍程度。某一特定词语的idf，可以由总文件数目除以包含该词语之文件的数目，再将得到的商取以10为底的对数得到。 

　　一篇文章中某个词语的重要程度，可以标记为词频和逆向文件词频的乘积，



　　通过计算出文档中每个词的TF-IDF值，然后结合相似度计算方法（一般采用余弦相似度）就可以计算两个文档的相似度。采用TF-IDF的前提是“文章的词语重要性与词语在文章中出现的位置不相关”。


##### 2.3 LSA

//TODO

##### 2.4 相似度计算

　　相似度就是比较两个事物的相似性。一般通过计算事物的特征之间的距离，如果距离小，那么相似度大；如果距离大，那么相似度小。 

　　问题定义：有两个对象X、Y，都包括N维特征，，计算X和Y的相似性。常用的方法如下:

###### 2.4.1 欧式距离

　　欧氏距离是最常用的距离计算公式，衡量的是多维空间中各个点之间的绝对距离，当数据很稠密并且连续时，这是一种很好的计算方式。 

　　因为计算是基于各维度特征的绝对数值，所以欧氏度量需要保证各维度指标在相同的刻度级别，如在KNN中需要对特征进行归一化。 





###### 2.4.2 曼哈顿距离

　　曼哈顿距离也称为城市街区距离(City Block distance)。 






###### 2.4.3 余弦相似度

　　余弦相似度用向量空间中两个向量夹角的余弦值作为衡量两个个体间差异的大小。相比距离度量，余弦相似度更加注重两个向量在方向上的差异，而非距离或长度上。 






###### 2.4.4 其他

　　其他的相似度计算方法包括皮尔森相关系数、一般化的闵可夫斯基距离（当p=1时为曼哈顿距离，当p=2时为欧式距离）、汉明距离等。

#### 3 基于深度学习

##### 3.1 深度学习

　　随着深度学习在图像和语音识别中取得不错的进展，近些年深度学习也开始应用于自然语言处理的不同应用中。 

　　语义相似性匹配问题已经逐渐从人工设计特征转向分布式表达和神经网络结构相结合的方式。(Recent work has moved away from **hand crafted features** and towards modeling with **distributed representations and neural network architectures**)

##### 3.2 DSSM

　　DSSM是一种深度学习语义匹配模型，在检索场景下，利用用户的点击数据来训练语义层次的匹配(The proposed deep structured semantic models are discriminatively trained by maximizing the conditional likelihood of the clicked documents given a query using the clickthrough data)。 
**DSSM利用点击率来代替相关性**，点击数据中包含大量的用户问句和对应的点击文档，这些点击数据将用户的问题和匹配的文档连接起来。  

　　下图就是利用DNN的DSSM结构。DNN的输入是用户问句和对应文档的高维未归一化向量（如图中的500K维），DNN的输出是一个低维的语义特征空间向量（如图中的128维）。 
![这里写图片描述](https://img-blog.csdn.net/20180320102033724?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　上图中的Multilayer non-linear projection采用了非线性的多层神经网络，利用表示DNN输出层问句和文档的低维语义向量，然后通过softmax得到条件概率分布。 

**Word hashing**

　　原始的问句或者文档是一个高维的向量，如向量的维度可能是词表的大小，当输入这些高维度的向量时神经网络可能无法训练。为了使得神经网络更好的训练，论文提出了减少DNN输入层向量维度的**Word hashing**方法，也即上图中的word hashing层。 

　　word hashing是一种基于字符的n-gram方法。如对good一词，首先添加开始和结尾的标记得到#good#，然后将该词分解为字符n-grams（如采用trigram: #go,goo,oog,od#），最后这个词就会被这些n-grams向量表示。 

　　一种可能存在的问题是认为不同词的词向量冲突(collision)。但是经过作者的统计，如在500k维的one-hot编码基础上，采用字符级trigram时，向量维度可以降到30k维，且冲突率仅为0.0044%。


　　DSSM的训练中，神经网络层的参数是softmax函数的平滑因子，是训练中的一个经验值。在实际训练中，对于每一个问句共有5个文档对应，其中一个是点击文档，其余4个是随机选择的非点击文档。 

　　文档的相关性采用人工标注，共分为5个分数(0-4)，其中4分表示最相关，0分表示最不相关。 


　　DSSM的优点在于1.直接利用了用户的点击数据；2. 得到的结果可以直接排序；但是缺点在于没有利用上下文信息。 


　　DSSM的扩展还包括CDSSM、DSSM-LSTM等，其中CDSSM在一定程度上弥补了上下文缺失的问题，结构上将DNN替换成CNN。 

　　DSSM-LSTM中使用LSTM记录上下文。


##### 3.3 ConvNet

　　ConvNet通过精心设计卷积神经网络，结合不同规格的卷积神经网络的差异性度量句子的相似度。（Proposing an elaborate convolutional network variant which infers sentences similarity by integrating various difference across many convolutions at various scale） 

　　在论文《Multi-Perspective Sentence Similarity Modeling with Convolutional Neural Networks》中，采用“Siamese”（孪生）CNN结构，分别对两个句子建模，然后利用一个句子相似度测量层计算句子相似度，最后通过一个全连接层输出softmax相似度得分。整体的网络结构如下所示： 
![这里写图片描述](https://img-blog.csdn.net/20180323091554207?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　可以看到，一个句子首先被转化为嵌入矩阵(embedding matrix)，然后输入卷积-池化层（论文提出使用2种卷积方式和3种池化操作，也即论文题目“多视角”的由来）得到处理后的句子向量，为更好计算句子之间的相似度，分别对向量不同的输出结果计算其相似性，最终将相似度向量输入全连接层得到相似性分数，与标签值相比较。 
![这里写图片描述](https://img-blog.csdn.net/20180323092901686?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　1.卷积层 

　　论文提出两种卷积方式，一种是**holistic filter**，如上图左上角比较。 

　　5.全连接层 

　　基于句子的相似度计算之后，得到相应的相似度向量；然后这组向量之后连接一个全连接层，最后softmax对应输出，如果是计算相似度度量值，可以用softmax输出类别概率。
　　模型参数： 

　　1. 为何只采用max-pooling和min-pooling，而不采用mean-pooling等问题。

##### 3.4 Skip-thoughts Vectors

　　Skip-thoughts的核心思想：将Word2vec中的skip-gram方法从词的层面扩展到句子的层面，利用seq2seq模型预测输入语句的上下句。（Extending the skip-gram approach of word2vec from the word to sentence.This model feeds each sentences into an RNN encoder-decoder(with GRU activations)which attempts to reconstructs the immediately preceding the following sentences） 

　　论文提到，在之前的各类监督方法中，模型通过确定的标签作为优化目标更新参数虽然能取得不错的效果，但是只能适用于不同的任务。 

　　模型的结构如下： 
![这里写图片描述](https://img-blog.csdn.net/20180404112723227?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　模型在一个连续的文本语料（小说）上进行训练，通过Encoder端将词转化为句子向量，然后在Decoder端结合Encoder端的句子向量生成上下文语句。 

　　Decoder端和Encoder端都采用了GRU，因为GRU不仅在序列模型任务上和LSTM表现一样好，而且在概念理解和计算上都要比LSTM简单。 

　　Decoder端是一个基于Encoder端输出，优化的目标函数是上下句的对数似然之和，整个训练集上的目标函数就是对所有的句子组加和。 
![这里写图片描述](https://img-blog.csdn.net/20180404114330664?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　对于这样一个模型，最大的问题在于如何把有限的词扩展到任意的词或句。论文中采用的方法是学习一种映射，将一个模型中的词表示映射到另外一个模型中。具体的操作是把CBOW中预训练得到的词向量映射到Encoder端的词空间。最终将词汇量扩展到1百万词。 


　　训练好的Skip-thoughs模型，会把Encoder端作为特征提取器，对所有的句子提取Skip-thoughs向量，得到的这些向量表示可以用在不同的任务中，如语义相似度计算。
##### 3.5 Tree-LSTM

　　Tree -LSTM的核心思想：将对语序敏感的标准LSTM序列推广为树状结构的网络拓扑图。(Generalizing the order-sensitive chain-structure of standard LSTMs to tree-structure network topologies) 

　　标准的LSTM仅仅考虑了句子的序列信息，但是在自然语言中句法结构能够自然的将词结合成短语或句子，因此作者在论文中提出了两种利用句法结构信息的LSTM扩展结构：The Child-Sum Tree-LSTM和N-arr Tree-LSTM。 

　　和标准的LSTM结构一样，每个Tree-LSTM（记为都有独立的参数矩阵，这使得N-ary Tree LSTM比Child-Sum Tree-LSTM能更好的从其子节点学习到一些细节。 
**Constituency Tree-LSTM**可以看做是N-ary Tree-LSTM在N为2的特例。
　　论文中提到基于Tree-LSTM结构的两种模型，一种是**Tree-LSTM Classification**，另一种是**Semantic Relatedness of Sentence Pairs**。 

　　（1）Tree-LSTM Classification 

　　对树中结点表示第k个句子对。 
![这里写图片描述](https://img-blog.csdn.net/20180403175234261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


　　对于Dependency Tree-LSTM模型，最大的问题在于训练数据的收集十分困难并且句法分析结果也不是在所有语言中都有。3.6 Siamese LSTM/Manhattan LSTM/MaLSTM

　　Siamese LSTM之前简要介绍一下Siamese Network。Siamese Network用来度量数据之间的相似性，两组数据（文本，图像等）同时输入到一个神经网络中，并经由这个神经网络转化为N*1维的向量，此后会通过一个数值函数（如余弦相似度）计算这两个向量的距离，通过得到的距离来度量原始输入数据的相似性。 
![这里写图片描述](https://img-blog.csdn.net/20180321220711998?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　在标准的Siamese Network中，两侧的神经网络需要使用相同的网络结构和参数；同时在进行梯度更新前，需要先对两侧的梯度平均；两个输出向量需要满足相似或者不相似的关系。 

Siamese LSTM

　　The model is applied to access for semantic similarity between sentences,a siamese adaptation of the LSTM network for labeled data comparised of pairs of variable-length sequences. 

　　Siamese LSTM结构大体与Siamese Network相似，其网络结构如下所示： 
![这里写图片描述](https://img-blog.csdn.net/20180321221446263?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3FxXzI4MDMxNTI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　　模型的输入：首先对句子分词，然后将词转化为预先训练好的Word2vec向量，并按序列输入网络。（按论文的意思，模型的Word2vec是利用外部数据语料训练的，并且词向量维度为300维） 

　　模型的输出：输入句子对的相似度分数。由于模型采用e−xe−x分别是两个LSTM模型的最后隐藏层状态，得到的向量作为特征空间中句子的embedding，随后在特征空间中计算向量相似度，以此来推断句子对的语义相似度。根据作者的意思，这两个LSTM可以是共享参数的，也可以是不共享参数的。 

　　模型的损失函数采用MSE(均方损失函数)，优化算法采用Adelta，为防止训练中梯度爆炸，采用gradient clipping。 


　　关于该模型有两点值得注意： 

　　1.LSTM，在Siamese Network中采用孪生LSTM，是因为LSTM能够解决RNN的长期依赖问题，通过使用记忆单元(memory cell)，LSTM能够储存更长输入序列的信息。当然对于特别长的句子而言，标准的LSTM能力也是有限的，对于长度超过30的长句子，通过模型得到的最终隐藏层状态，占据比重较大的还是后面的词，前面的词基本消失，因此需要Attention。 

　　2. Manhattan Similarity。在度量相似性的时候，采用了曼哈顿距离而不是欧式距离，根据论文和[](http://www.cnblogs.com/txq157/p/7425781.html)博客  的观点，一方面用Word2vec训练出来的词，存在大量欧式距离相等的情况，如果用L2范数去衡量，存在语义丢失的情况，而cosine similarity适合度量向量维度特别大的情况，因此采用Manhattan距离最合适；另一方面，采用L2范数会存在梯度消失的问题，在训练的早期，L2范数会错误的认为两个语义不相关的句子相似因为采用欧式距离时的梯度消失问题(This is because during early stages of training,a L2-based model is unable to correct errors where it erroneously believes semantically different sentence to be nearly identical due to vanishing gradients of the Euclidean distance)。3.7 Others

//TODO4 参考文献- 
[](http://www.cnblogs.com/xlturing/p/6136690.html)NLP点滴——文本相似度- 
[](http://blog.csdn.net/yixianfeng41/article/details/61917158)常用的相似度计算方法原理及实现- 
[](https://zhuanlan.zhihu.com/p/26996025)基于Simase_LSTM的计算中文句子相似度经验总结与分享- 
[](http://www.cnblogs.com/txq157/p/7425781.html)原创：Siamese LSTM解决句子相似度以及扩展(理论篇)- 
论文《[](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/cikm2013_DSSM_fullversion.pdf)Leaning Deep Structured Semantic Models for Web Search using Clickthrough Data》- 
论文《Siamese Recurrent Architecture for learning Sentence Similarity》- 
[](http://dev.dafan.info/detail/404566?p=54-72)CNN在句子相似性建模的应用–模型介绍篇- 
论文《[](http://www.aclweb.org/anthology/D15-1181)Multi-Perspective Sentence Similarity Modeling with Convolutional Neural Networks》- 
[](http://www.jeyzhang.com/cnn-apply-on-modelling-sentence.html)卷积神经网络(CNN)在句子建模上的应用- 
[](https://zhuanlan.zhihu.com/p/26261371)《Improved Semantic Representations From Tree-Structured Long Short-Term Memory Networks》阅读笔记- 
论文[](https://arxiv.org/pdf/1503.00075.pdf)《Improved Semantic Representations From Tree-Structured Long Short-Term Memory Networks》- 
[](https://zhuanlan.zhihu.com/p/35252733)基于TreeLSTM的情感分析- 
论文[](http://papers.nips.cc/paper/5950-skip-thought-vectors.pdf)《Skip-Thought Vectors》- 
[](https://blog.csdn.net/sinat_26917383/article/details/54882554)cips2016+学习笔记︱简述常见的语言表示模型（词嵌入、句表示、篇章表示）












