
# python︱六款中文分词模块尝试:jieba、THULAC、SnowNLP、pynlpir、CoreNLP、pyLTP - 素质云笔记-Recorder... - CSDN博客

2017年08月10日 17:42:39[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：50204



---**公众号“素质云笔记”定期更新博客内容：**

---![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

---THULAC

---四款python中中文分词的尝试。尝试的有：jieba、SnowNLP（MIT）、pynlpir（大数据搜索挖掘实验室（北京市海量语言信息处理与云计算应用工程技术研究中心））、thulac（清华大学自然语言处理与社会人文计算实验室）

---四款都有分词功能，本博客只介绍作者比较感兴趣、每个模块的内容。jieba在这不做介绍，可见博客：

---[python+gensim︱jieba分词、词袋doc2bow、TFIDF文本挖掘](http://blog.csdn.net/sinat_26917383/article/details/71436563)

![这里写图片描述](https://img-blog.csdn.net/20170810173813389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---.

---[
](https://img-blog.csdn.net/20170810173813389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---一、SnowNLP

---只处理的unicode编码，所以使用时请自行decode成unicode。来源：

---[https://github.com/isnowfy/snownlp](https://github.com/isnowfy/snownlp)

---以下功能都是笔者比较感兴趣的：

---情感分析（现在训练数据主要是买卖东西时的评价，所以对其他的一些可能效果不是很好，待解决）

---文本分类（Naive Bayes）

---转换成拼音（Trie树实现的最大匹配）

---繁体转简体（Trie树实现的最大匹配）

---提取文本关键词（TextRank算法）

---提取文本摘要（TextRank算法）

---Tokenization（分割成句子）

---文本相似（BM25）

---\# 转化为拼音

---from

---snownlp

---import

---SnowNLP
s

---=

---SnowNLP

---(

---"杭州西湖风景很好，是旅游胜地！"

---)

---s

---.

---pinyin

---['hang',
 'zhou',
 'xi',
 'hu',
 'feng',
 'jing',
 'hen',
 'hao',
 '，',
 'shi',
 'lv',
 'you',
 'sheng',
 'di',
 '！']

---\# 繁体转为简体

---s

---=

---SnowNLP

---(

---u

---'「繁體字」「繁體中文」的叫法在臺灣亦很常見。'

---)

---s

---.

---han

---'「繁体字」「繁体中文」的叫法在台湾亦很常见。'

---\# 自动摘要

---text

---=

---u

---'''自然语言处理是计算机科学领域与人工智能领域中的一个重要方向。它研究能实现人与计算机之间用自然语言进行有效通信的各种理论和方法。自然语言处理是一门融语言学、计算机科学、数学于一体的科学。因此，这一领域的研究将涉及自然语言，即人们日常使用的语言，所以它与语言学的研究有着密切的联系，但又有重要的区别。自然语言处理并不是一般地研究自然语言，而在于研制能有效地实现自然语言通信的计算机系统，特别是其中的软件系统。因而它是计算机科学的一部分。
'''

---s

---=

---SnowNLP

---(

---text

---)

---s

---.

---summary

---(

---5

---)

---['因而它是计算机科学的一部分',
 '自然语言处理是计算机科学领域与人工智能领域中的一个重要方向',
 '自然语言处理是一门融语言学、计算机科学、数学于一体的科学',
 '所以它与语言学的研究有着密切的联系',
 '这一领域的研究将涉及自然语言']

---\# 计算BM2.5 相似性

---s

---=

---SnowNLP

---(

---[

---[

---u

---'这篇'

---,

---u

---'文章'

---,

---u

---'非常好'

---]

---,

---[

---u

---'那篇'

---,

---u

---'文章'

---,

---u

---'一般般'

---]

---,

---[

---u

---'这个'

---]

---]

---)

---\#s.tf

---s

---.

---idf
s

---.

---sim

---(

---[

---u

---'这篇'

---,

---u

---'文章'

---,

---u

---'非常好'

---]

---)

---\# [0.3756070762985226, 0, 0]

---[0.45263029953948547, -0.45263029953948547, 0]

---[参考《文本相似度-bm25算法原理及实现》](http://www.jianshu.com/p/1e498888f505)

---BM25是通过q和s中的公共词汇进行相似度计算的算法,BM25算法的相关性得分公式可总结为：

![](http://upload-images.jianshu.io/upload_images/1713353-fc89dbc4421949c6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
---本篇博客自己写了一个bm2.5实现的软件。

---.

---二、thulac

---来源：

---[https://github.com/thunlp/THULAC-Python](https://github.com/thunlp/THULAC-Python)

---设置比较一般，内容也可能不够多样性。不过有:c++ java so，仅处理UTF8编码中文文本，之后会逐渐增加支持其他编码的功能，敬请期待，也有三个版本的分词模型：

---（1）简单的分词模型Model_1，仅支持分词功能。该模型由人民日报分词语料库训练得到。

---（2）分词和词性标注联合模型Model_2，支持同时分词和词性标注功能。该模型由人民日报分词和词性标注语料库训练得到。

---（3）我们还提供更复杂、完善和精确的分词和词性标注联合模型Model_3和分词词表。该模型是由多语料联合训练训练得到（语料包括来自多文体的标注文本和人民日报标注文本等）。由于模型较大，如有机构或个人需要，通过审核后我们会将相关资源发送给联系人。

---thulac

---(

---user_dict

---=

---None

---,

---model_path

---=

---None

---,

---T2S

---=

---False

---,

---seg_only

---=

---False

---,

---filt

---=

---False

---,

---deli

---=

---'_'

---)

---初始化程序，进行自定义设置

---user_dict	      	设置用户词典，用户词典中的词会被打上uw标签。词典中每一个词一行，UTF8编码

---T2S					默认False, 是否将句子从繁体转化为简体

---seg_only	   		默认False, 时候只进行分词，不进行词性标注

---filt		   		默认False, 是否使用过滤器去除一些没有意义的词语，例如“可以”。

---model_path	 	    设置模型文件所在文件夹，默认为models/

---deli	 	      	默认为‘_’, 设置词与词性之间的分隔符

---rm_space            默认为True, 是否去掉原文本中的空格后再进行分词

---初始化设置

---thulac(user_dict=None, model_path=None, T2S=False, seg_only=False, filt=False, deli=’_’)

---user_dict	      	设置用户词典，用户词典中的词会被打上uw标签。词典中每一个词一行，UTF8编码

---T2S					默认False, 是否将句子从繁体转化为简体

---seg_only	   		默认False, 时候只进行分词，不进行词性标注

---filt		   		默认False, 是否使用过滤器去除一些没有意义的词语，例如“可以”。

---model_path	 	    设置模型文件所在文件夹，默认为models/

---deli	 	      	默认为‘_’, 设置词与词性之间的分隔符

---rm_space            默认为True, 是否去掉原文本中的空格后再进行分词

---分词结果

---cut(文本, text=False) 对一句话进行分词

---cut_f(输入文件, 输出文件) 对文件进行分词

---cut和cut_f

---命令行模式

---python -m thulac input.txt output.txt

---从input.txt读入，并将分词和词性标注结果输出到ouptut.txt中

---如果只需要分词功能，可在增加参数"seg_only"

---python -m thulac input.txt output.txt seg_only

---词性解释

---n/名词 np/人名 ns/地名 ni/机构名 nz/其它专名

---m/数词 q/量词 mq/数量词 t/时间词 f/方位词 s/处所词

---v/动词 a/形容词 d/副词 h/前接成分 k/后接成分

---i/习语 j/简称 r/代词 c/连词 p/介词 u/助词 y/语气助词

---e/叹词 o/拟声词 g/语素 w/标点 x/其它

---thu1

---=

---thulac

---.

---thulac

---(

---model_path

---=

---'/usr/local/lib/python3.5/dist-packages/thulac/models'

---)

---\#默认模式

---text

---=

---thu1

---.

---cut

---(

---"杭州西湖风景很好，是旅游胜地！"

---,

---text

---=

---False

---)

---\#进行一句话分词

---print

---(

---text

---)

---Model loaded succeed
[['杭州', 'ns'], ['西湖', 'ns'], ['风景', 'n'], ['很', 'd'], ['好', 'a'], ['，', 'w'], ['是', 'v'], ['旅游', 'v'], ['胜地', 'n'], ['！', 'w']]

---\# 与Jieba对比一下

---import

---jieba
seg_list

---=

---jieba

---.

---cut

---(

---"杭州西湖风景很好，是旅游胜地！"

---,

---cut_all

---=

---False

---)

---print

---(

---"Full Mode: "

---+

---"/ "

---.

---join

---(

---seg_list

---)

---)

---\# 全模式

---Full Mode: 杭州/ 西湖/ 风景/ 很/ 好/ ，/ 是/ 旅游胜地/ ！

---.

---三、pynlpir

---文档地址：

---[http://pynlpir.readthedocs.io/en/latest/tutorial.html](http://pynlpir.readthedocs.io/en/latest/tutorial.html)

---加载之后就遇到了报错：

---RuntimeError: NLPIR function ‘NLPIR_Init’ failed.

---**问题提问地址：**

---[https://github.com/tsroten/pynlpir/issues/57](https://github.com/tsroten/pynlpir/issues/57)

---解决办法就是更换license:

---[https://github.com/NLPIR-team/NLPIR/tree/master/License](https://github.com/NLPIR-team/NLPIR/tree/master/License)

---License 这是授权文件夹，存放了十一个组件的授权文件 license for 7 days: 授权期限为7天 license for one month: 授权期限为一个月。

---也就是说，要不断更新license，开源的真不彻底，之后就没尝试其中的功能了。

---import

---pynlpir
s

---=

---'NLPIR分词系统前身为2000年发布的ICTCLAS词法分析系统，从2009年开始，为了和以前工作进行大的区隔，并推广NLPIR自然语言处理与信息检索共享平台，调整命名为NLPIR分词系统。'

---pynlpir

---.

---segment

---(

---s

---)

---.

---四、CoreNLP3.8.0——多功能

---github:

---[https://github.com/Lynten/stanford-corenlp](https://github.com/Lynten/stanford-corenlp)

---分词、词性标注(Part-Of-Speech tag, POS-tag)、命名实体识别(Named Entity Recognition, NER)、句法分析(Syntactic Parse)等各项 NLP 领域的功能。

---stanfordcorenlp是一个对Stanford CoreNLP进行了封装的Python工具包，GitHub地址，使用非常方便。

---4.1安装（Ubuntu）

---依赖，需求：

---1.Java 1.8+ (Check with command: java -version)

---2.Stanford CoreNLP 3.8.0（主模块，.rar的解压包，

---[安装路径](https://stanfordnlp.github.io/CoreNLP/download.html#steps)

---）

---3.语言模型，这里是中文模型，

---[安装路径](https://stanfordnlp.github.io/CoreNLP/download.html#steps)

---），其他还有的语言有：

---阿拉伯语、汉语、英语、法语、德语、西班牙语（5大类，.jar格式）

---下载完，解压Stanford CoreNLP 3.8.0，然后把语言模型放在解压目录下就ok了！！

---\# sudo pip install stanfordcorenlp

---简单使用，记得参数lang=‘zh’

---from stanfordcorenlp import StanfordCoreNLP
nlp = StanfordCoreNLP(r'/home/gld/stanford-corenlp-full-2016-10-31/', lang='zh')
sentence = '清华大学位于北京。'
print nlp.word_tokenize(sentence)
print nlp.pos_tag(sentence)
print nlp.ner(sentence)
print nlp.parse(sentence)
print nlp.dependency_parse(sentence)

---输出：

---[清华, 大学, 位于, 北京, 。]
[(清华, NR), (大学, NN), (位于, VV), (北京, NR), (。, PU)]
[(清华, ORGANIZATION), (大学, ORGANIZATION), (位于, O), (北京, GPE), (。, O)]
...

---4.2案例：

---from stanfordcorenlp import StanfordCoreNLP
nlp = StanfordCoreNLP(r'/.../corenlp/stanford-corenlp-full-2017-06-09/', lang='zh')

---lang='zh’代表中文，然后就会自动去搜索中文的语言模型的.jar文件。

---sentence =  '元芳你怎么看？我就趴窗口上看呗！'
print ('Tokenize:', nlp.word_tokenize(sentence))
print ('Part of Speech:', nlp.pos_tag(sentence))
print ('Named Entities:', nlp.ner(sentence))
print ('Constituency Parsing:', nlp.parse(sentence))
print ('Dependency Parsing:', nlp.dependency_parse(sentence))

---现在这个版本非常简洁，比用ltp要好不少，不过从笔者自己的实践来看，分词模块还只是一般，而且没看到有可以新加入词典，毕竟不是本土研究机构。

---现在的输出结果如下：

---Tokenize: ['元芳', '你', '怎么', '看', '？', '我', '就', '趴', '窗口', '上看呗', '！']
Part of Speech: [('元芳', 'NR'), ('你', 'PN'), ('怎么', 'AD'), ('看', 'VV'), ('？', 'PU'), ('我', 'PN'), ('就', 'AD'), ('趴', 'VV'), ('窗口', 'NN'), ('上看呗', 'NN'), ('！', 'PU')]
Named Entities: [('元芳', 'O'), ('你', 'O'), ('怎么', 'O'), ('看', 'O'), ('？', 'O'), ('我', 'O'), ('就', 'O'), ('趴', 'O'), ('窗口', 'O'), ('上看呗', 'O'), ('！', 'O')]
Constituency Parsing: (ROOT
  (IP
    (IP
      (NP (NR 元芳))
      (IP
        (NP (PN 你))
        (VP
          (ADVP (AD 怎么))
          (VP (VV 看))))
      (PU ？))
    (NP (PN 我))
    (VP
      (ADVP (AD 就))
      (VP (VV 趴)
        (NP (NN 窗口) (NN 上看呗))))
    (PU ！)))
Dependency Parsing: [('ROOT', 0, 4), ('nmod:topic', 4, 1), ('nsubj', 4, 2), ('advmod', 4, 3), ('punct', 4, 5), ('nsubj', 8, 6), ('advmod', 8, 7), ('conj', 4, 8), ('compound:nn', 10, 9), ('dobj', 8, 10), ('punct', 4, 11)]

---贴一下之前版本的一些实验结果图：

![这里写图片描述](http://images2015.cnblogs.com/blog/380252/201611/380252-20161106210204908-61151173.jpg)
![这里写图片描述](http://images2015.cnblogs.com/blog/380252/201611/380252-20161106212038299-1005102801.jpg)
![这里写图片描述](http://images2015.cnblogs.com/blog/380252/201611/380252-20161106213420440-309601494.jpg)
![这里写图片描述](http://images2015.cnblogs.com/blog/380252/201611/380252-20161106214753252-869011390.jpg)
![这里写图片描述](http://images2015.cnblogs.com/blog/380252/201611/380252-20161106215055565-465328766.jpg)
---Stanford CoreNLP提供了一个商用Python wrapper：

---[https://github.com/Wordseer/stanford-corenlp-python](https://github.com/Wordseer/stanford-corenlp-python)

---4.3 词性、句法分析、依存关系的符号解释

---（来源：

---[Stanford Parser的使用——进行词法语法分析](http://blog.csdn.net/pipisorry/article/details/42976457)

---）

---词性解释

---CC: conjunction, coordinatin 表示连词

---CD: numeral, cardinal 表示基数词

---DT: determiner 表示限定词

---EX: existential there 存在句

---FW: foreign word 外来词

---IN: preposition or conjunction, subordinating 介词或从属连词

---JJ: adjective or numeral, ordinal 形容词或序数词

---JJR: adjective, comparative 形容词比较级

---JJS: adjective, superlative 形容词最高级

---LS: list item marker 列表标识

---MD: modal auxiliary 情态助动词

---NN: noun, common, singular or mass

---NNS: noun, common, plural

---NNP: noun, proper, singular

---NNPS: noun, proper, plural

---PDT: pre-determiner 前位限定词

---POS: genitive marker 所有格标记

---PRP: pronoun, personal 人称代词

---PRP

$$
: pronoun, possessive 所有格代词
RB: adverb 副词
RBR: adverb, comparative 副词比较级
RBS: adverb, superlative 副词最高级
RP: particle 小品词
SYM: symbol 符号
TO:&quot;to&quot; as preposition or infinitive marker 作为介词或不定式标记
UH: interjection 插入语
VB: verb, base form
VBD: verb, past tense
VBG: verb, present participle or gerund
VBN: verb, past participle
VBP: verb, present tense, not 3rd person singular
VBZ: verb, present tense,3rd person singular
WDT: WH-determiner WH限定词
WP: WH-pronoun WH代词
WP
$$
---: WH-pronoun, possessive WH所有格代词

---WRB:Wh-adverb WH副词

---句法分析（句法树）

---ROOT：要处理文本的语句

---IP：简单从句

---NP：名词短语

---VP：动词短语

---PU：断句符，通常是句号、问号、感叹号等标点符号

---LCP：方位词短语

---PP：介词短语

---CP：由‘的’构成的表示修饰性关系的短语

---DNP：由‘的’构成的表示所属关系的短语

---ADVP：副词短语

---ADJP：形容词短语

---DP：限定词短语

---QP：量词短语

---NN：常用名词

---NR：固有名词：表示仅适用于该项事物的名词，含地名，人名，国名，书名，团体名称以及一事件的名称等。

---NT：时间名词

---PN：代词

---VV：动词

---VC：是

---CC：表示连词

---VE：有

---VA：表语形容词

---AS：内容标记（如：了）

---VRD：动补复合词

---CD: 表示基数词

---DT: determiner 表示限定词

---EX: existential there 存在句

---FW: foreign word 外来词

---IN: preposition or conjunction, subordinating 介词或从属连词

---JJ: adjective or numeral, ordinal 形容词或序数词

---JJR: adjective, comparative 形容词比较级

---JJS: adjective, superlative 形容词最高级

---LS: list item marker 列表标识

---MD: modal auxiliary 情态助动词

---PDT: pre-determiner 前位限定词

---POS: genitive marker 所有格标记

---PRP: pronoun, personal 人称代词

---RB: adverb 副词

---RBR: adverb, comparative 副词比较级

---RBS: adverb, superlative 副词最高级

---RP: particle 小品词

---SYM: symbol 符号

---TO:”to” as preposition or infinitive marker 作为介词或不定式标记

---WDT: WH-determiner WH限定词

---WP: WH-pronoun WH代词

---WP$: WH-pronoun, possessive WH所有格代词

---WRB:Wh-adverb WH副词

---关系表示

---abbrev: abbreviation modifier，缩写

---acomp: adjectival complement，形容词的补充；

---advcl : adverbial clause modifier，状语从句修饰词

---advmod: adverbial modifier状语

---agent: agent，代理，一般有by的时候会出现这个

---amod: adjectival modifier形容词

---appos: appositional modifier,同位词

---attr: attributive，属性

---aux: auxiliary，非主要动词和助词，如BE,HAVE SHOULD/COULD等到

---auxpass: passive auxiliary 被动词

---cc: coordination，并列关系，一般取第一个词

---ccomp: clausal complement从句补充

---complm: complementizer，引导从句的词好重聚中的主要动词

---conj : conjunct，连接两个并列的词。

---cop: copula。系动词（如be,seem,appear等），（命题主词与谓词间的）连系

---csubj : clausal subject，从主关系

---csubjpass: clausal passive subject 主从被动关系

---dep: dependent依赖关系

---det: determiner决定词，如冠词等

---dobj : direct object直接宾语

---expl: expletive，主要是抓取there

---infmod: infinitival modifier，动词不定式

---iobj : indirect object，非直接宾语，也就是所以的间接宾语；

---mark: marker，主要出现在有“that” or “whether”“because”, “when”,

---mwe: multi-word expression，多个词的表示

---neg: negation modifier否定词

---nn: noun compound modifier名词组合形式

---npadvmod: noun phrase as adverbial modifier名词作状语

---nsubj : nominal subject，名词主语

---nsubjpass: passive nominal subject，被动的名词主语

---num: numeric modifier，数值修饰

---number: element of compound number，组合数字

---parataxis: parataxis: parataxis，并列关系

---partmod: participial modifier动词形式的修饰

---pcomp: prepositional complement，介词补充

---pobj : object of a preposition，介词的宾语

---poss: possession modifier，所有形式，所有格，所属

---possessive: possessive modifier，这个表示所有者和那个’S的关系

---preconj : preconjunct，常常是出现在 “either”, “both”, “neither”的情况下

---predet: predeterminer，前缀决定，常常是表示所有

---prep: prepositional modifier

---prepc: prepositional clausal modifier

---prt: phrasal verb particle，动词短语

---punct: punctuation，这个很少见，但是保留下来了，结果当中不会出现这个

---purpcl : purpose clause modifier，目的从句

---quantmod: quantifier phrase modifier，数量短语

---rcmod: relative clause modifier相关关系

---ref : referent，指示物，指代

---rel : relative

---root: root，最重要的词，从它开始，根节点

---tmod: temporal modifier

---xcomp: open clausal complement

---xsubj : controlling subject 掌控者

---参考：

---[使用Stanford CoreNLP的Python封装包处理中文（分词、词性标注…](http://blog.5ibc.net/p/119031.html)

---[干货！详述Python NLTK下如何使用stanford NLP工具包](http://www.cnblogs.com/baiboy/p/nltk1.html)

---.

---五、 pyltp

---“语言云” 以哈工大社会计算与信息检索研究中心研发的 “语言技术平台（LTP）” 为基础，为用户提供高效精准的中文自然语言处理云服务。

---pyltp 是 LTP 的 Python 封装，提供了分词，词性标注，命名实体识别，依存句法分析，语义角色标注的功能。

---技术文档：

---[http://pyltp.readthedocs.io/zh_CN/latest/api.html\#id15](http://pyltp.readthedocs.io/zh_CN/latest/api.html#id15)

---介绍文档：

---[https://www.ltp-cloud.com/intro/\#introduction](https://www.ltp-cloud.com/intro/#introduction)

---介绍文档：

---[http://ltp.readthedocs.io/zh_CN/latest/appendix.html\#id5](http://ltp.readthedocs.io/zh_CN/latest/appendix.html#id5)

---需要先载入他们训练好的模型，

---[下载地址](https://pan.baidu.com/share/link?shareid=1988562907&uk=2738088569#list/path=/)

---1.分词

---\# -*- coding: utf-8 -*-
from pyltp import SentenceSplitter
sents = SentenceSplitter.split('元芳你怎么看？我就趴窗口上看呗！')  \# 分句
print '\n'.join(sents)
im
port os
LTP_DATA_DIR = '/path/to/your/ltp_data'  \# ltp模型目录的路径
cws_model_path = os.path.join(LTP_DATA_DIR, 'cws.model')  \# 分词模型路径，模型名称为`cws.model`
from pyltp import Segmentor
segmentor = Segmentor()  \# 初始化实例
segmentor.load(cws_model_path)  \# 加载模型
words = segmentor.segment('元芳你怎么看')  \# 分词
print '\t'.join(words)
segmentor.release()  \# 释放模型

---SentenceSplitter分句子，把段落拆分成句子；segmentor.segment分词

---如果要使用外部词典，需要，加载模型的时候，指定新的字典集：

---segmentor = Segmentor()  \# 初始化实例
segmentor.load_with_lexicon(cws_model_path, '/path/to/your/lexicon') \# 加载模型，第二个参数是您的外部词典文件路径

---2.词性标注

---import os
LTP_DATA_DIR = '/path/to/your/ltp_data'  \# ltp模型目录的路径
pos_model_path = os.path.join(LTP_DATA_DIR, 'pos.model')  \# 词性标注模型路径，模型名称为`pos.model`
from pyltp import Postagger
postagger = Postagger() \# 初始化实例
postagger.load(pos_model_path)  \# 加载模型
words = ['元芳', '你', '怎么', '看']  \# 分词结果
postags = postagger.postag(words)  \# 词性标注
print '\t'.join(postags)
postagger.release()  \# 释放模型

---3.命名实体识别

---\# -*- coding: utf-8 -*-
import os
LTP_DATA_DIR = '/path/to/your/ltp_data'  \# ltp模型目录的路径
ner_model_path = os.path.join(LTP_DATA_DIR, 'ner.model')  \# 命名实体识别模型路径，模型名称为`pos.model`
from pyltp import NamedEntityRecognizer
recognizer = NamedEntityRecognizer() \# 初始化实例
recognizer.load(ner_model_path)  \# 加载模型
words = ['元芳', '你', '怎么', '看']
postags = ['nh', 'r', 'r', 'v']
netags = recognizer.recognize(words, postags)  \# 命名实体识别
print '\t'.join(netags)
recognizer.release()  \# 释放模型

---4.依存句法分析

---依存语法 (Dependency Parsing, DP) 通过分析语言单位内成分之间的依存关系揭示其句法结构。 直观来讲，依存句法分析识别句子中的“主谓宾”、“定状补”这些语法成分，并分析各成分之间的关 系。

---\# -*- coding: utf-8 -*-
import os
LTP_DATA_DIR = '/path/to/your/ltp_data'  \# ltp模型目录的路径
par_model_path = os.path.join(LTP_DATA_DIR, 'parser.model')  \# 依存句法分析模型路径，模型名称为`parser.model`
from pyltp import Parser
parser = Parser() \# 初始化实例
parser.load(par_model_path)  \# 加载模型
words = ['元芳', '你', '怎么', '看']
postags = ['nh', 'r', 'r', 'v']
arcs = parser.parse(words, postags)  \# 句法分析
print "\t".join("%d:%s" % (arc.head, arc.relation) for arc in arcs)
parser.release()  \# 释放模型

---words 和 postags 分别为分词和词性标注的结果。输出结果：

---4:SBV       4:SBV   4:ADV   0:HED

---arc.head 表示依存弧的父节点词的索引。ROOT节点的索引是0，第一个词开始的索引依次为1、2、3…

---arc.relation 表示依存弧的关系。

---arc.head 表示依存弧的父节点词的索引，arc.relation 表示依存弧的关系。

---每个词的符号具体含义（可见

---[文档](http://ltp.readthedocs.io/zh_CN/latest/appendix.html#id5)

---）

![这里写图片描述](https://img-blog.csdn.net/20171031171601924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---5.语义角色分析

---语义角色标注 (Semantic Role Labeling, SRL) 是一种浅层的语义分析技术，标注句子中某些短语为给定谓词的论元 (语义角色) ，如施事、受事、时间和地点等。其能够对问答系统、信息抽取和机器翻译等应用产生推动作用。

---\# -*- coding: utf-8 -*-
import os
LTP_DATA_DIR = '/path/to/your/ltp_data'  \# ltp模型目录的路径
srl_model_path = os.path.join(LTP_DATA_DIR, 'srl')  \# 语义角色标注模型目录路径，模型目录为`srl`。注意该模型路径是一个目录，而不是一个文件。
from pyltp import SementicRoleLabeller
labeller = SementicRoleLabeller() \# 初始化实例
labeller.load(srl_model_path)  \# 加载模型
words = ['元芳', '你', '怎么', '看']
postags = ['nh', 'r', 'r', 'v']
netags = ['S-Nh', 'O', 'O', 'O']
\# arcs 使用依存句法分析的结果
roles = labeller.label(words, postags, netags, arcs)  \# 语义角色标注
\# 打印结果
for role in roles:
    print role.index, "".join(
        ["%s:(%d,%d)" % (arg.name, arg.range.start, arg.range.end) for arg in role.arguments])
labeller.release()  \# 释放模型

---[arg.name](http://arg.name)

---表示语义角色关系，arg.range.start 表示起始词位置，arg.range.end 表示结束位置。

---结果如下

---3 A0:(0,0)A0:(1,1)ADV:(2,2)

---第一个词开始的索引依次为0、1、2…

---由于结果输出一行，所以“元芳你怎么看”有一组语义角色。

---其谓词索引为3，即“看”。

---这个谓词有三个语义角色，范围分别是(0,0)即“元芳”，(1,1)即“你”，(2,2)即“怎么”，类型分别是A0、A0、ADV。

---核心的语义角色为 A0-5 六种，A0 通常表示动作的施事，A1通常表示动作的影响等，A2-5 根据谓语动词不同会有不同的语义含义。

---A2 动作范围，A3动作开始，A3动作结束。DIS，标记语。（生僻标签信息来源：

---[语义角色标注(Semantic Role Labelling)](https://zhuanlan.zhihu.com/p/35789254)

---）

---其余的15个语义角色为附加语义角色，如LOC 表示地点，TMP 表示时间等。附加语义角色列表如下：

![这里写图片描述](https://img-blog.csdn.net/20171031171835303?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181108114004425.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

