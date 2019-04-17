# 云摘录︱Word2Vec 作者Tomas Mikolov 的三篇代表作解析 - 素质云笔记/Recorder... - CSDN博客





2016年09月18日 20:39:45[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8800








本文来源于公众号paperweekly 




谈到了word2vec作者的三篇论文：






1、Efficient Estimation of Word Representation in Vector Space, 2013

2、Distributed Representations of Sentences and Documents, 2014

3、Enriching Word Vectors with Subword Information, 2016



（原标题：一周论文   Word2Vec 作者Tomas Mikolov 的三篇代表作）



——————————————————————————————————————————————




# 一、Efficient Estimation of Word Representation in Vector Space, 2013






## 词向量word2vec诞生稿！



传统的NNLM模型包含四层，即输入层、映射层、隐含层和输出层，计算复杂度很大程度上依赖于映射层到隐含层之间的计算，而且需要指定上下文的长度。RNNLM模型被提出用来改进NNLM模型，去掉了映射层，只有输入层、隐含层和输出层，计算复杂度来源于上一层的隐含层到下一层隐含层之间的计算。




本文提出的两个模型CBOW (Continuous Bag-of-Words Model)和Skip-gram (Continuous Skip-gram Model)结合了上面两个模型的特点，都是只有三层，即输入层、映射层和输出层。CBOW模型与NNLM模型类似，用上下文的词向量作为输入，映射层在所有的词间共享，输出层为一个分类器，目标是使当前词的概率最大。Skip-gram模型与CBOW的输入跟输出恰好相反，输入层为当前词向量，输出层是使得上下文的预测概率最大，如下图所示。训练采用SGD。




Bengio[1]在2003年就提出了language model的思路，同样是三层（输入层，隐含层和输出层）用上下文的词向量来预测中间词，但是计算复杂度较高，对于较大的数据集运行效率低；实验中也发现将上下文的n-gram出现的频率结合进去会提高性能，这个优点体现在CBOW和Skip-gram模型的输出层中，用hierarchical
 softmax（with huffman trees）来计算词概率。





**CBOW和skip-gram比较与延伸**




本文的实验结果显示CBOW比NNLM在syntactic和semantic上的预测都要好，而Skip-gram在semantic上的性能要优于CBOW，但是其计算速度要低于CBOW。结果显示用较大的数据集和较少的epoch，可以取得较好的效果，并且在速度上有所提升。与LSI和LDA相比，word2vec利用了词的上下文，语义信息更加丰富。基于word2vec，出现了phrase2vec,
 sentence2vec和doc2vec，仿佛一下子进入了embedding的世界。NLP的这些思想也在用于recommendation等方面，并且与image结合，将image跟text之间进行转换。








——————————————————————————————————————————————





# 二、Distributed Representations of Sentences and Documents




句向量以及段落向量如何表示：




句向量：利用one-hot的表示方法作为网络的输入，乘以词矩阵W，然后将得到的每个向量通过平均或者拼接的方法得到整个句子的表示，最后根据任务要求做一分类，而这过程中得到的W就是词向量矩阵，基本上还是word2vec的思路。




段落向量：依旧是相同的方法，只是在这里加上了一个段落矩阵，用以表示每个段落，当这些词输入第i个段落时，通过段落id就可以从这个矩阵中得到相对应的段落表示方法。需要说明的是，在相同的段落中，段落的表示是相同的。文中这样表示的动机就是段落矩阵D可以作为一个memory记住在词的context中遗失的东西，相当于增加了一个额外的信息。这样经过训练之后，我们的就得到了段落表示D，当然这个段落就可以是一段或者一篇文章。




## 小结




这样一方面好训练，另一方面减少了参数，避免模型过拟合。优点就是在训练paragraph vector的时候加入了一个paragraph
 matrix，这样在训练过程中保留了一部分段落或者文档信息。这点在目前看来也是有一定优势的。但是目前深度学习发展迅速，可以处理非常大的计算量，同时word2vec以及其变种被应用得非常普遍，因此该文章提出的方法思路大于模型，思路我们可以借鉴，模型就不具有优势了。





——————————————————————————————————————————————








# 三、Enriching Word Vectors with Subword Information




**问题：如何解决word2vec方法中罕见词效果不佳的问题，以及如何提升词形态丰富语言的性能？**





如果一个word出现次数较少那么学到的vector质量也不理想。针对这一问题作者提出使用subword信息来弥补这一问题，简单来说就是通过词缀的vector来表示词。比如unofficial是个低频词，其数据量不足以训练出高质量的vector，但是可以通过un+official这两个高频的词缀学习到不错的vector。






方法上，本文沿用了word2vec的skip-gram模型，主要区别体现在特征上。word2vec使用word作为最基本的单位，即通过中心词预测其上下文中的其他词汇。而subword model使用字母n-gram作为单位，本文n取值为3~6。这样每个词汇就可以表示成一串字母n-gram，一个词的embedding表示为其所有n-gram的和。这样我们训练也从用中心词的embedding预测目标词，转变成用中心词的n-gram
 embedding预测目标词。




实验分为三个部分，分别是（1）计算两个词之间的语义相似度，与人类标注的相似度进行相关性比较；（2）与word2vec一样的词类比实验；（3）与其他考虑morphology的方法比较。结果是本文方法在语言形态丰富的语言（土耳其语，法语等）及小数据集上表现优异，与预期一致。




## 小结




文章中提出的思路对于morphologically rich languages（例如土耳其语，词缀的使用极为普遍而有趣）来说十分有意义。词缀作为字母与单词之间的中层单位，本身具有一定的语义信息。通过充分利用这种中层语义来表征罕见词汇，直观上讲思路十分合理，也是应用了compositionality的思想。




利用形态学改进word embedding的工作十分丰富，但中文NLP似乎很难利用这一思路。其实个人感觉中文中也有类似于词缀的单位，比如偏旁部首等等，只不过不像使用字母系统的语言那样容易处理。期待今后也有闪光的工作出现在中文环境中。











