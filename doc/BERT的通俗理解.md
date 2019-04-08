# BERT的通俗理解

2019年02月26日 16:27:45 [小白的进阶](https://me.csdn.net/laobai1015) 阅读数：298



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/laobai1015/article/details/87937528

# 1、预训练模型

​      BERT是一个预训练的模型，那么什么是预训练呢？举例子进行简单的介绍
​      假设已有A训练集，先用A对网络进行预训练，在A任务上学会网络参数，然后保存以备后用，当来一个新的任务B，采取相同的网络结构，网络参数初始化的时候可以加载A学习好的参数，其他的高层参数随机初始化，之后用B任务的训练数据来训练网络，当加载的参数保持不变时，称为"frozen"，当加载的参数随着B任务的训练进行不断的改变，称为“fine-tuning”，即更好地把参数进行调整使得更适合当前的B任务

​     优点：当任务B的训练数据较少时，很难很好的训练网络，但是获得了A训练的参数，会比仅仅使用B训练的参数更优

## Task #1: Masked LM

​     为了训练双向特征，这里采用了Masked Language Model的预训练方法，随机mask句子中的部分token，然后训练模型来预测被去掉的token。

具体操作是：

随机mask语料中15%的token，然后将masked token 位置输出的final hidden vectors送入softmax，来预测masked token。

这里也有一个小trick，如果都用标记[MASK]代替token会影响模型，所以在随机mask的时候采用以下策略：

1）80%的单词用[MASK]token来代替

my dog is hairy → my dog is [MASK]
2）10%单词用任意的词来进行代替

my dog is hairy → my dog is apple

3）10%单词不变

my dog is hairy → my dog is hairy

##  Task 2#: Next Sentence Prediction

​       为了让模型捕捉两个句子的联系，这里增加了Next Sentence Prediction的预训练方法，即给出两个句子A和B，B有一半的可能性是A的下一句话，训练模型来预测B是不是A的下一句话
Input = [CLS] the man went to [MASK] store [SEP]
​             penguin [MASK] are flight ## less birds [SEP]
Label = NotNext
​             he bought a gallon [MASK] milk [SEP]
Label = IsNext
Input = [CLS] the man [MASK] to the store [SEP]
训练模型，使模型具备理解长序列上下文的联系的能力

# 2、BERT模型

BERT：全称是Bidirectional Encoder Representation from Transformers，即双向Transformer的Encoder，BERT的模型架构基于多层双向转换解码，因为decoder是不能获要预测的信息的，模型的主要创新点都在pre-traing方法上，即用了Masked LM和Next Sentence Prediction两种方法分别捕捉词语和句子级别的representation

其中“双向”表示模型在处理某一个词时，它能同时利用前面的词和后面的词两部分信息，这种“双向”的来源在于BERT与传统语言模型不同，它不是在给你大牛股所有前面词的条件下预测最可能的当前词，而是随机遮掩一些词，并利用所有没被遮掩的词进行预测

下图展示了三种预训练模型，其中 BERT 和 ELMo 都使用双向信息，OpenAI GPT 使用单向信息
![img](https://img-blog.csdnimg.cn/2019022616252198.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhb2JhaTEwMTU=,size_16,color_FFFFFF,t_70)

# 3、BERT的输入部分

![img](https://img-blog.csdnimg.cn/20190226162545298.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhb2JhaTEwMTU=,size_16,color_FFFFFF,t_70)

​     bert的输入部分是个线性序列，两个句子通过分隔符分割，最前面和最后增加两个标识符号。每个单词有三个embedding:位置信息embedding，这是因为NLP中单词顺序是很重要的特征，需要在这里对位置信息进行编码；单词embedding,这个就是我们之前一直提到的单词embedding；第三个是句子embedding，因为前面提到训练数据都是由两个句子构成的，那么每个句子有个句子整体的embedding项对应给每个单词。把单词对应的三个embedding叠加，就形成了Bert的输入。

​      如上图所示，输入有A句[my dog is cute]和B句[he likes playing]这两个自然句，我们首先需要将每个单词及特殊符号都转化为词嵌入向量，因为神经网络只能进行数值计算。其中特殊符[SEP]是用于分割两个句子的符号，前面半句会加上分割码A，后半句会加上分割码B
​      因为要建模句子之间的关系，BERT 有一个任务是预测 B 句是不是 A 句后面的一句话，而这个分类任务会借助 A/B 句最前面的特殊符 [CLS] 实现，该特殊符可以视为汇集了整个输入序列的表征。
最后的位置编码是 Transformer 架构本身决定的，因为基于完全注意力的方法并不能像 CNN 或 RNN 那样编码词与词之间的位置关系，但是正因为这种属性才能无视距离长短建模两个词之间的关系。因此为了令 Transformer 感知词与词之间的位置关系，我们需要使用位置编码给每个词加上位置信息。

总结一下：
（1）token embeddings表示的是词向量，第一个单词是CLS，可以用于之后的分类任务
（2）segment embeddings用来区别两种句子，因为预训练不光做LM还要做以两个句子为输入的分类任务
（3）position embeddings表示位置信息

# 4、NLP的四大类任务

（1）序列标注：分词、实体识别、语义标注……
（2）分类任务：文本分类、情感计算……
（3）句子关系判断：entailment、QA、自然语言推理
（4）生成式任务：机器翻译、文本摘

上图给出示例，对于句子关系类任务，很简单，和GPT类似，加上一个起始和终结符号，句子之间加个分隔符即可。对于输出来说，把第一个起始符号对应的Transformer最后一层位置上面串接一个softmax分类层即可。对于分类问题，与GPT一样，只需要增加起始和终结符号，输出部分和句子关系判断任务类似改造；对于序列标注问题，输入部分和单句分类是一样的，只需要输出部分Transformer最后一层每个单词对应位置都进行分类即可。从这里可以看出，上面列出的NLP四大任务里面，除了生成类任务外，Bert其它都覆盖到了，而且改造起来很简单直观。（https://zhuanlan.zhihu.com/p/49271699）

# 5、模型的评价

（1）优点

BERT是截止至2018年10月的最新的的state of the art模型，通过预训练和精调可以解决11项NLP的任务。使用的是Transformer，相对于rnn而言更加高效、能捕捉更长距离的依赖。与之前的预训练模型相比，它捕捉到的是真正意义上的bidirectional context信息

（2）缺点

作者在文中主要提到的就是MLM预训练时的mask问题：

1）[MASK]标记在实际预测中不会出现，训练时用过多[MASK]影响模型表现;

2)每个batch只有15%的token被预测，所以BERT收敛得比left-to-right模型要慢（它们会预测每个token）

# 6、GLUE语料集的介绍

实验数据以及对应的NLP任务
MNLI：蕴含关系推断
QQP：问题对是否等价
QNLI：句子是都回答问句
SST-2：情感分析
CoLA：句子语言性判断
STS-B：语义相似
MRPC：句子对是都语义等价
RTE：蕴含关系推断
WNLI：蕴含关系推断

# 7、git网址https://github.com/google-research/bert

关于bert知识干货的汇总<https://zhuanlan.zhihu.com/p/50717786>

本文转自<https://blog.csdn.net/yangfengling1023/article/details/84025313>