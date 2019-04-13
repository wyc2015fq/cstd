
# word2vec数学原理详解 - bitcarmanlee的博客 - CSDN博客


2017年08月23日 19:12:45[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1204


## 1.单词表示
单词需要用计算机可以理解的方式表达后，才可以进行接下来的操作。
### 1.One hot representation
程序中编码单词的一个方法是one hot encoding。
有1000个词汇量。排在第一个位置的代表英语中的冠词”a”，那么这个”a”是用[1,0,0,0,0,…]，只有第一个位置是1，其余位置都是0的1000维度的向量表示，如下图中的第一列所示。
![这里写图片描述](https://img-blog.csdn.net/20170823190050413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也就是说，
在one hot representation编码的每个单词都是一个维度，彼此independent。
### 2.Distributed representation
然而每个单词彼此无关这个特点明显不符合我们的现实情况。我们知道大量的单词都是有关。
语义：girl和woman虽然用在不同年龄上，但指的都是女性。
复数：word和words仅仅是复数和单数的差别
时态：buy和bought表达的都是“买”，但发生的时间不同。
所以用one hot representation的编码方式，上面的特性都没有被考虑到。
我们更希望用诸如“语义”，“复数”，“时态”等维度去描述一个单词。每一个维度不再是0或1，而是连续的实数，表示不同的程度。
但是说到底，为什么我们想要用Distributed representation的方式去表达一个单词？
![这里写图片描述](https://img-blog.csdn.net/20170823190436123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.神经网络分析
假设我们的词汇只有4个，girl, woman, boy, man，下面就思考用两种不同的表达方式会有什么区别。
One hot representation
尽管我们知道他们彼此的关系，但是计算机并不知道。在神经网络的输入层中，每个单词都会被看作一个节点。 而我们知道训练神经网络就是要学习每个连接线的权重。如果只看第一层的权重，下面的情况需要确定4*3个连接线的关系，因为每个维度都彼此独立，girl的数据不会对其他单词的训练产生任何帮助，训练所需要的数据量，基本就固定在那里了。
![这里写图片描述](https://img-blog.csdn.net/20170823190801119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Distributed representation
我们这里手动的寻找这四个单词之间的关系 f 。可以用两个节点去表示四个单词。每个节点取不同值时的意义如下表。 那么girl就可以被编码成向量[0,1]，man可以被编码成[1,1]（第一个维度是gender，第二个维度是age）。
![这里写图片描述](https://img-blog.csdn.net/20170823190915551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
那么这时再来看神经网络需要学习的连接线的权重就缩小到了2*3。同时，当送入girl为输入的训练数据时，因为它是由两个节点编码的。那么与girl共享相同连接的其他输入例子也可以被训练到（如可以帮助到与其共享female的woman，和child的boy的训练）。
![这里写图片描述](https://img-blog.csdn.net/20170823191005955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Word embedding也就是要达到第二个神经网络所表示的结果，降低训练所需要的数据量。
而上面的四个单词可以被拆成2个节点的是由我们人工提供的先验知识将原始的输入空间经过 f (上图中的黄色箭头)投射到了另一个空间（维度更小），所以才能够降低训练所需要的数据量。 但是我们没有办法一直人工提供，机器学习的宗旨就是让机器代替人力去发现pattern。
### 我们没有办法一直人工提供，机器学习的宗旨就是让机器代替人力去发现pattern。
### 训练方法
问题来了，我们该如何自动寻找到类似上面的关系，将One hot representation转变成Distributed representation。我们事先并不明确目标是什么，所以这是一个无监督学习任务。
![这里写图片描述](https://img-blog.csdn.net/20170823191611348?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
用输入单词作为中心单词去预测周边单词的方式叫做：[Word2Vec - The Skip-Gram Model](http://mccormickml.com/2016/04/19/word2vec-tutorial-the-skip-gram-model/)
用输入单词作为周边单词去预测中心单词的方式叫做：[Continuous Bag of Words(CBOW)](https://iksinc.wordpress.com/tag/continuous-bag-of-words-cbow/)
## 3.word embedding数学解释
word embedding的意思是：给出一个文档，文档就是一个单词序列比如 “A B A C B F G”, 希望对文档中每个不同的单词都得到一个对应的向量(往往是低维向量)表示。比如，对于这样的“A B A C B F G”的一个序列，也许我们最后能得到：A对应的向量为[0.1 0.6 -0.5]，B对应的向量为[-0.2 0.9 0.7]  （此处的数值只用于示意）
之所以希望把每个单词变成一个向量，目的还是为了方便计算，比如“求单词A的同义词”，就可以通过“求与单词A在cos距离下最相似的向量”来做到。
Embedding在数学上表示一个maping, f: X -> Y， 也就是一个function，其中该函数是injective（就是我们所说的单射函数，每个Y只有唯一的X对应，反之亦然）和structure-preserving (结构保存，比如在X所属的空间上X1 < X2,那么映射后在Y所属空间上同理 Y1 < Y2)。那么对于word embedding，就是将单词word映射到另外一个空间，其中这个映射具有injective和structure-preserving的特点。
通俗的翻译可以认为是单词嵌入，就是把X所属空间的单词映射为到Y空间的多维向量，那么该多维向量相当于嵌入到Y所属空间中，一个萝卜一个坑。
word embedding，就是找到一个映射或者函数，生成在一个新的空间上的表达，该表达就是word representation。
推广开来，还有image embedding, video embedding, 都是一种将源数据映射到另外一个空间
知乎相关链接：[https://www.zhihu.com/question/32275069](https://www.zhihu.com/question/32275069)
参考文献：1.[http://dataunion.org/9331.html](http://dataunion.org/9331.html)

