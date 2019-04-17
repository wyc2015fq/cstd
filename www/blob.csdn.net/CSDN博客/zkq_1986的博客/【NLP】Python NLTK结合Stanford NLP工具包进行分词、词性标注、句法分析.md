# 【NLP】Python NLTK结合Stanford NLP工具包进行分词、词性标注、句法分析 - zkq_1986的博客 - CSDN博客





2018年08月11日 10:15:08[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1491








## 1 NLTK和StandfordNLP简介

**NLTK**：由宾夕法尼亚大学计算机和信息科学使用python语言实现的一种自然语言工具包，其收集的大量公开数据集、模型上提供了全面、易用的接口，涵盖了分词、词性标注(Part-Of-Speech tag, POS-tag)、命名实体识别(Named Entity Recognition, NER)、句法分析(Syntactic Parse)等各项 NLP 领域的功能。

**Stanford NLP：**由斯坦福大学的 NLP 小组开源的 Java 实现的 NLP 工具包，同样对 NLP 领域的各个问题提供了解决办法。斯坦福大学的 NLP 小组是世界知名的研究小组，能将 NLTK 和 Stanford NLP 这两个工具包结合起来使用，那对于自然语言开发者是再好不过的！在 2004 年 Steve Bird 在 NLTK 中加上了对 Stanford NLP 工具包的支持，通过调用外部的 jar 文件来使用 Stanford NLP 工具包的功能。本分析显得非常方便好用。

本文在主要介绍NLTK 中提供 Stanford NLP 中的以下几个功能:
- 中英文分词: StanfordTokenizer
- 中英文词性标注: StanfordPOSTagger
- 中英文命名实体识别: StanfordNERTagger
- 中英文句法分析: StanfordParser
- 中英文依存句法分析: StanfordDependencyParser, StanfordNeuralDependencyParser



## 2 安装配置过程中注意事项

本文以Python 3.5.2和java version "1.8.0_111"版本进行配置，具体安装需要注意以下几点：
- Stanford NLP 工具包需要 Java 8 及之后的版本，如果出错请检查 Java 版本
- 本文的配置都是以 Stanford NLP 3.6.0 为例，如果使用的是其他版本，请注意替换相应的文件名
- 本文的配置过程以 NLTK 3.2 为例，如果使用 NLTK 3.1，需要注意该旧版本中 StanfordSegmenter 未实现，其余大致相同
- 下面的配置过程是具体细节可以参照：[http://nlp.stanford.edu/software/](http://nlp.stanford.edu/software/)



## 3 StandfordNLP必要工具包下载

必要包下载：只需要下载以下3个文件就够了，stanfordNLTK文件里面就是StanfordNLP工具包在NLTK中所依赖的jar包和相关文件
- [stanfordNLTK](https://pan.baidu.com/s/1nvEYdfj)：自己将所有需要的包和相关文件已经打包在一起了，下面有具体讲解
- [Jar1.8](http://pan.baidu.com/s/1miubwq0) ：如果你本机是Java 8以上版本，可以不用下载了
- [NLTK](https://pan.baidu.com/s/1pKA9XuN) ：这个工具包提供Standford NLP接口

> 
以上文件下载后，Jar如果是1.8的版本可以不用下载，另外两个压缩包下载到本地，解压后拷贝文件夹到你的python安装主路径下，然后cmd进入NLTK下通过python setup.py install即可。后面操作讲路径简单修改即可。（如果不能正常分词等操作，查看python是否是3.2以上版本，java是否是8以后版本，jar环境变量是否配置正确）


**StanfordNLTK****目录结构如下**：（从各个压缩文件已经提取好了，如果读者感兴趣，下面有各个功能的源码文件）

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106203255315-1814061136.jpg)
- 分词依赖：stanford-segmenter.jar、 slf4j-api.jar、data文件夹相关子文件
- 命名实体识别依赖：classifiers、stanford-ner.jar
- 词性标注依赖：models、stanford-postagger.jar
- 句法分析依赖：stanford-parser.jar、stanford-parser-3.6.0-models.jar、classifiers
- 依存语法分析依赖：stanford-parser.jar、stanford-parser-3.6.0-models.jar、classifiers 

**压缩包下载和源码分析**：
- 分词压缩包：StanfordSegmenter和StanfordTokenizer:下载[stanford-segmenter-2015-12-09.zip (version 3.6.0)](https://pan.baidu.com/s/1kVc20ib) 解压获取目录中的 stanford-segmenter-3.6.0.jar 拷贝为 stanford-segmenter.jar和 slf4j-api.jar
- 词性标注压缩包：下载[stanford-postagger-full-2015-12-09.zip](http://pan.baidu.com/s/1hrVMSE4)(version 3.6.0) 解压获取stanford-postagger.jar
- 命名实体识别压缩包：下载[stanford-ner-2015-12-09.zip](http://pan.baidu.com/s/1skOJb5r)(version 3.6.0) ，将解压获取stanford-ner.jar和classifiers文件
- 句法分析、句法依存分析：下载[stanford-parser-full-2015-12-09.zip](http://pan.baidu.com/s/1nv6Q2bZ)(version 3.6.0) 解压获取stanford-parser.jar 和 stanford-parser-3.6.0-models.jar



## 4 StandfordNLP相关核心操作

### 4.1 分词

**StanfordSegmenter 中文分词**：下载52nlp改过的NLTK包[nltk-develop](http://pan.baidu.com/s/1misFxna) ，解压后将其拷贝到你的python目录下，进去E:\Python\nltk-develop采用python 编辑器打开setup.py文件，F5运行，输入以下代码：
|1234567891011|`>>> ``from``nltk.tokenize.stanford_segmenter import StanfordSegmenter``>>> segmenter = StanfordSegmenter(``    ``path_to_jar=r``"E:\tools\stanfordNLTK\jar\stanford-segmenter.jar"``,``    ``path_to_slf4j=r``"E:\tools\stanfordNLTK\jar\slf4j-api.jar"``,``    ``path_to_sihan_corpora_dict=r``"E:\tools\stanfordNLTK\jar\data"``,``    ``path_to_model=r``"E:\tools\stanfordNLTK\jar\data\pku.gz"``,``    ``path_to_dict=r``"E:\tools\stanfordNLTK\jar\data\dict-chris6.ser.gz"``)``>>> str=``"我在博客园开了一个博客，我的博客名叫伏草惟存，写了一些自然语言处理的文章。"``>>> result = segmenter.segment(str)``>>> result`|
|----|----|

**执行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106210204908-61151173.jpg)

**程序解读**：StanfordSegmenter 的初始化参数说明:
- path_to_jar: 用来定位jar包，本程序分词依赖stanford-segmenter.jar（注: 其他所有 Stanford NLP 接口都有 path_to_jar 这个参数。）
- path_to_slf4j: 用来定位slf4j-api.jar作用于分词
- path_to_sihan_corpora_dict: 设定为 stanford-segmenter-2015-12-09.zip 解压后目录中的 data 目录， data 目录下有两个可用模型 pkg.gz 和 ctb.gz 需要注意的是，使用 StanfordSegmenter 进行中文分词后，其返回结果并不是 list ，而是一个字符串，各个汉语词汇在其中被空格分隔开。

**StanfordTokenizer 英文分词 **[：相关参考资料](http://nlp.stanford.edu/software/tokenizer.shtml)
|12345678|`Python 3.5.2 (v3.5.2:4def2a2901a5, Jun 25 2016, 22:01:18) [MSC v.1900 32 bit (Intel)] ``on``win32``Type ``"copyright"``, ``"credits"``or ``"license()"``for``more information.``>>> ``from``nltk.tokenize import StanfordTokenizer``>>> tokenizer = StanfordTokenizer(path_to_jar=r``"E:\tools\stanfordNLTK\jar\stanford-parser.jar"``)``>>> sent = ``"Good muffins cost $3.88\nin New York.  Please buy me\ntwo of them.\nThanks."``>>> print(tokenizer.tokenize(sent))``[``'Good'``, ``'muffins'``, ``'cost'``, ``'$'``, ``'3.88'``, ``'in'``, ``'New'``, ``'York'``, ``'.'``, ``'Please'``, ``'buy'``, ``'me'``, ``'two'``, ``'of'``, ``'them'``, ``'.'``, ``'Thanks'``, ``'.'``]``>>>`|
|----|----|

**执行结果：**

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106211210627-97177452.jpg)

### 4.2 命名实体识别

**StanfordNERTagger 英文命名实体识别**
|1234|`>>> ``from``nltk.tag import StanfordNERTagger``>>> eng_tagger = StanfordNERTagger(model_filename=r``'E:\tools\stanfordNLTK\jar\classifiers\english.all.3class.distsim.crf.ser.gz'``,path_to_jar=r``'E:\tools\stanfordNLTK\jar\stanford-ner.jar'``)``>>> print(eng_tagger.tag(``'Rami Eid is studying at Stony Brook University in NY'``.split()))``[(``'Rami'``, ``'PERSON'``), (``'Eid'``, ``'PERSON'``), (``'is'``, ``'O'``), (``'studying'``, ``'O'``), (``'at'``, ``'O'``), (``'Stony'``, ``'ORGANIZATION'``), (``'Brook'``, ``'ORGANIZATION'``), (``'University'``, ``'ORGANIZATION'``), (``'in'``, ``'O'``), (``'NY'``, ``'O'``)]`|
|----|----|

**运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106212038299-1005102801.jpg)

**StanfordNERTagger 中文命名实体识别**
|123456|`>>> result``'四川省 成都 信息 工程 大学 我 在 博客 园 开 了 一个 博客 ， 我 的 博客 名叫 伏 草 惟 存 ， 写 了 一些 自然语言 处理 的 文章 。\r\n'``>>> ``from``nltk.tag import StanfordNERTagger``>>> chi_tagger = StanfordNERTagger(model_filename=r``'E:\tools\stanfordNLTK\jar\classifiers\chinese.misc.distsim.crf.ser.gz'``,path_to_jar=r``'E:\tools\stanfordNLTK\jar\stanford-ner.jar'``)``>>> ``for``word, tag ``in``chi_tagger.tag(result.split()):``    ``print(word,tag)`|
|----|----|

**运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106212839877-1886802558.jpg)

### 4.3 词性标注

**StanfordPOSTagger 英文词性标注**
|123|`>>> ``from``nltk.tag import StanfordPOSTagger``>>> eng_tagger = StanfordPOSTagger(model_filename=r``'E:\tools\stanfordNLTK\jar\models\english-bidirectional-distsim.tagger'``,path_to_jar=r``'E:\tools\stanfordNLTK\jar\stanford-postagger.jar'``)``>>> print(eng_tagger.tag(``'What is the airspeed of an unladen swallow ?'``.split()))`|
|----|----|

**运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106213030486-1785536093.jpg)

**StanfordPOSTagger 中文词性标注**
|12345|`>>> ``from``nltk.tag import StanfordPOSTagger``>>> chi_tagger = StanfordPOSTagger(model_filename=r``'E:\tools\stanfordNLTK\jar\models\chinese-distsim.tagger'``,path_to_jar=r``'E:\tools\stanfordNLTK\jar\stanford-postagger.jar'``)``>>> result``'四川省 成都 信息 工程 大学 我 在 博客 园 开 了 一个 博客 ， 我 的 博客 名叫 伏 草 惟 存 ， 写 了 一些 自然语言 处理 的 文章 。\r\n'``>>> print(chi_tagger.tag(result.split()))`|
|----|----|

** 运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106213420440-309601494.jpg)

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106213427065-1377867380.jpg)

### **4.4 句法分析**：[参考文献资料](https://github.com/treppo/stanfordparser/tree/master/stanford-parser/models%20)

**StanfordParser英文语法分析**
|123|`>>> ``from``nltk.parse.stanford import StanfordParser``>>> eng_parser = StanfordParser(r``"E:\tools\stanfordNLTK\jar\stanford-parser.jar"``,r``"E:\tools\stanfordNLTK\jar\stanford-parser-3.6.0-models.jar"``,r``"E:\tools\stanfordNLTK\jar\classifiers\englishPCFG.ser.gz"``)``>>> print(list(eng_parser.parse(``"the quick brown fox jumps over the lazy dog"``.split())))`|
|----|----|

**运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106213615830-1177112559.jpg)

**StanfordParser 中文句法分析**
|1234|`>>> ``from``nltk.parse.stanford import StanfordParser``>>> chi_parser = StanfordParser(r``"E:\tools\stanfordNLTK\jar\stanford-parser.jar"``,r``"E:\tools\stanfordNLTK\jar\stanford-parser-3.6.0-models.jar"``,r``"E:\tools\stanfordNLTK\jar\classifiers\chinesePCFG.ser.gz"``)``>>> sent = u``'北海 已 成为 中国 对外开放 中 升起 的 一 颗 明星'``>>> print(list(chi_parser.parse(sent.split())))`|
|----|----|

**运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106214753252-869011390.jpg)

### 4.5 依存句法分析

**StanfordDependencyParser 英文依存句法分析**
|12345|`>>> ``from``nltk.parse.stanford import StanfordDependencyParser``>>> eng_parser = StanfordDependencyParser(r``"E:\tools\stanfordNLTK\jar\stanford-parser.jar"``,r``"E:\tools\stanfordNLTK\jar\stanford-parser-3.6.0-models.jar"``,r``"E:\tools\stanfordNLTK\jar\classifiers\englishPCFG.ser.gz"``)``>>> res = list(eng_parser.parse(``"the quick brown fox jumps over the lazy dog"``.split()))``>>> ``for``row ``in``res[0].triples():``    ``print(row)`|
|----|----|

**运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106214922330-512726666.jpg)

**StanfordDependencyParser 中文依存句法分析**
|12345|`>>> ``from``nltk.parse.stanford import StanfordDependencyParser``>>> chi_parser = StanfordDependencyParser(r``"E:\tools\stanfordNLTK\jar\stanford-parser.jar"``,r``"E:\tools\stanfordNLTK\jar\stanford-parser-3.6.0-models.jar"``,r``"E:\tools\stanfordNLTK\jar\classifiers\chinesePCFG.ser.gz"``)``>>> res = list(chi_parser.parse(u``'四川 已 成为 中国 西部 对外开放 中 升起 的 一 颗 明星'``.split()))``>>> ``for``row ``in``res[0].triples():``    ``print(row)`|
|----|----|

**运行结果**：

![](https://images2015.cnblogs.com/blog/380252/201611/380252-20161106215055565-465328766.jpg)



# 5 句法分析缩写语含义

ROOT：要处理文本的语句

IP：简单从句

NP：名词短语

VP：动词短语

PU：断句符，通常是句号、问号、感叹号等标点符号

LCP：方位词短语

PP：介词短语

CP：由‘的’构成的表示修饰性关系的短语

DNP：由‘的’构成的表示所属关系的短语

ADVP：副词短语

ADJP：形容词短语

DP：限定词短语

QP：量词短语

NN：常用名词

NR：固有名词

NT：时间名词

PN：代词

VV：动词

VC：是

CC：表示连词

VE：有

VA：表语形容词

AS：内容标记（如：了）

VRD：动补复合词

CD: 表示基数词

DT: determiner 表示限定词

EX: existential there 存在句

FW: foreign word 外来词

IN: preposition or conjunction, subordinating 介词或从属连词

JJ: adjective or numeral, ordinal 形容词或序数词

JJR: adjective, comparative 形容词比较级

JJS: adjective, superlative 形容词最高级

LS: list item marker 列表标识

MD: modal auxiliary 情态助动词

PDT: pre-determiner 前位限定词

POS: genitive marker 所有格标记

PRP: pronoun, personal 人称代词

RB: adverb 副词

RBR: adverb, comparative 副词比较级

RBS: adverb, superlative 副词最高级

RP: particle 小品词 

SYM: symbol 符号

TO:”to” as preposition or infinitive marker 作为介词或不定式标记 

WDT: WH-determiner WH限定词

WP: WH-pronoun WH代词

WP$: WH-pronoun, possessive WH所有格代词

WRB:Wh-adverb WH副词


关系表示

abbrev: abbreviation modifier，缩写

acomp: adjectival complement，形容词的补充；

advcl : adverbial clause modifier，状语从句修饰词

advmod: adverbial modifier状语

agent: agent，代理，一般有by的时候会出现这个

amod: adjectival modifier形容词

appos: appositional modifier,同位词

attr: attributive，属性

aux: auxiliary，非主要动词和助词，如BE,HAVE SHOULD/COULD等到

auxpass: passive auxiliary 被动词

cc: coordination，并列关系，一般取第一个词

ccomp: clausal complement从句补充

complm: complementizer，引导从句的词好重聚中的主要动词

conj : conjunct，连接两个并列的词。

cop: copula。系动词（如be,seem,appear等），（命题主词与谓词间的）连系

csubj : clausal subject，从主关系

csubjpass: clausal passive subject 主从被动关系

dep: dependent依赖关系

det: determiner决定词，如冠词等

dobj : direct object直接宾语

expl: expletive，主要是抓取there

infmod: infinitival modifier，动词不定式

iobj : indirect object，非直接宾语，也就是所以的间接宾语；

mark: marker，主要出现在有“that” or “whether”“because”, “when”,

mwe: multi-word expression，多个词的表示

neg: negation modifier否定词

nn: noun compound modifier名词组合形式

npadvmod: noun phrase as adverbial modifier名词作状语

nsubj : nominal subject，名词主语

nsubjpass: passive nominal subject，被动的名词主语

num: numeric modifier，数值修饰

number: element of compound number，组合数字

parataxis: parataxis: parataxis，并列关系

partmod: participial modifier动词形式的修饰

pcomp: prepositional complement，介词补充

pobj : object of a preposition，介词的宾语

poss: possession modifier，所有形式，所有格，所属

possessive: possessive modifier，这个表示所有者和那个’S的关系

preconj : preconjunct，常常是出现在 “either”, “both”, “neither”的情况下

predet: predeterminer，前缀决定，常常是表示所有

prep: prepositional modifier

prepc: prepositional clausal modifier

prt: phrasal verb particle，动词短语

punct: punctuation，这个很少见，但是保留下来了，结果当中不会出现这个

purpcl : purpose clause modifier，目的从句

quantmod: quantifier phrase modifier，数量短语

rcmod: relative clause modifier相关关系

ref : referent，指示物，指代

rel : relative

root: root，最重要的词，从它开始，根节点

tmod: temporal modifier

xcomp: open clausal complement

xsubj : controlling subject 掌控者





## 6 参考文献和知识扩展
- [NLTK官方网站](http://www.nltk.org/)
- [NLTK的API](http://www.nltk.org/api/nltk.tokenize.html#module-nltk.tokenize.stanford_segmenter)
- [NLTK中使用斯坦福中文分词器](http://www.52nlp.cn/python%E8%87%AA%E7%84%B6%E8%AF%AD%E8%A8%80%E5%A4%84%E7%90%86%E5%AE%9E%E8%B7%B5-%E5%9C%A8nltk%E4%B8%AD%E4%BD%BF%E7%94%A8%E6%96%AF%E5%9D%A6%E7%A6%8F%E4%B8%AD%E6%96%87%E5%88%86%E8%AF%8D%E5%99%A8)
- [GitHub上NLTK源码](https://github.com/nltk/nltk)
- [https://www.cnblogs.com/baiboy/p/nltk1.html](https://www.cnblogs.com/baiboy/p/nltk1.html)
- [https://blog.csdn.net/cuixianpeng/article/details/16864785](https://blog.csdn.net/cuixianpeng/article/details/16864785)





