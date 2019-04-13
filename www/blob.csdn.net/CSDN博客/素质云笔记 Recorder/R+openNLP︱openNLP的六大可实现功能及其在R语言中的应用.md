
# R+openNLP︱openNLP的六大可实现功能及其在R语言中的应用 - 素质云笔记-Recorder... - CSDN博客

2016年08月30日 12:46:10[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3581所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
openNLP是NLP中比较好的开源工具，R语言中有openNLP packages，但是呢，貌似对中文的支持并不好，笔者试了试，发现结果并不如意。但是也算认识了一番，就来介绍一下。
一些内容转载于白宁超老师：[OpenNLP：驾驭文本，分词那些事](http://www.cnblogs.com/baiboy/p/opennlp.html)

**————————————————————————————————————————**

# 一、openNLP简介

**维基百科**：[Apache](https://zh.wikipedia.org/wiki/Apache)OpenNLP库是一个基于[机器学习](https://zh.wikipedia.org/wiki/%E6%9C%BA%E5%99%A8%E5%AD%A6%E4%B9%A0)的[自然语言](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%84%B6%E8%AA%9E%E8%A8%80%E8%99%95%E7%90%86)文本处理的[开发工具包](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%84%B6%E8%AA%9E%E8%A8%80%E8%99%95%E7%90%86)，它支持自然语言处理中一些共有的任务，例如：标记化、句子分割、词性标注、固有实体提取（指在句子中辨认出专有名词，例如：人名）、浅层分析（句字分块）、语法分析及指代。这些任务通常都需要较为先进的文字处理服务功能。
**官方文档**：Apache的OpenNLP库是自然语言文本的处理基于机器学习的工具包。它支持最常见的NLP任务，如断词，句子切分，部分词性标注，命名实体提取，分块，解析和指代消解。这些任务通常需要建立更先进的文字处理服务。OpenNLP还包括最大熵和基于感知机器学习。该OpenNLP项目的目标是创造上述任务的成熟工具包。一个附加的目的是提供一种大量预建模型为各种语言，以及这些模型衍生自注释文本资源。
开发者:Apache Software Foundation
稳定版本: 1.5.2-incubating（2011年11月28日，​5年前）
开发状态: Active
编程语言:Java
类型:自然语言处理
网站:[http://incubator.apache.org/opennlp/](http://incubator.apache.org/opennlp/)
————————————————————————————————————————


# 二、六大功能——openNLP packages

R中的openNLP主要有以下几个功能：
序列标注（maxent chunker）、实体命名识别（maxent entity annotator）、词性标注器（ops tag annotator）、句子探测器（sent toen annotator）、解析器（parse annotator）、标记生成器（maxent word token annotator）

## 1、序列标注（maxent chunker）
文本分块由除以单词句法相关部分，如名词基，动词基的文字，但没有指定其内部结构，也没有其在主句作用。
R语言中函数名称：Maxent_Chunk_Annotator()——Apache OpenNLP Maxent chunker.

## 2、实体命名识别（maxent entity annotator）
名称查找器可检测文本命名实体和数字。为了能够检测实体名称搜索需要的模型。该模型是依赖于语言和实体类型这是训练。所述OpenNLP项目提供了许多这些各种免费提供的语料库训练有素预训练名取景模式。后面会提到如何下载。
根据不同的模型可以查找人名、地名等实体名。
R语言中函数名称：Maxent_Entity_Annotator—— Apache OpenNLP Maxent namefinder.


## 3、词性标注器（ops tag annotator）
语音标记器的部分标记符号与基于符号本身和符号的上下文中它们的相应字类型。符号可能取决于符号和上下文使用多个POS标签。该OpenNLP POS标注器使用的概率模型来预测正确的POS标记出了标签组。为了限制可能的标记的符号标记字典可以使用这增加了捉人者的标记和运行时性能。
R语言中函数名称：pos_tag_annotator——Apache OpenNLP MaxentPart of Speech tagger.


## 4、句子探测器（sent toen annotator）
**功能介绍：**
**句子检测器是用于检测句子边界。**
**句子探测器返回一个字符串数组。**
R语言中函数名称：sent_token_annotator——Apache OpenNLP Maxentsentence detector

## 5、解析器（parse annotator）、针对english
尝试解析器最简单的方法是在命令行工具。该工具仅用于演示和测试。请从我们网站上的英文分块解析器模型，并用以下命令启动解析工具。
R语言中函数名称：Parse_Annotator—— Apache OpenNLPchunking parser for English


## 6、标记生成器/断句/分词（maxent word token annotator）
OpenNLP断词段输入字符序列为标记。常是这是由空格分隔的单词，但也有例外。例如，“isn't”被分割为“is”与“n't"，因为它是AA简要格式”isn't“我们的句子分为以下标记：符号通常是词语，标点符号，数字等OpenNLP提供多种标记生成器的实现：
空白标记生成器 - 一个空白标记生成器，非空白序列被确定为符号
简单的标记生成器 - 一个字符类标记生成器，同样的字符类的序列标记
可学习标记生成器 - 一个最大熵标记生成器，检测基于概率模型符号边界
R语言中函数名称：word_token_annotator——e Apache OpenNLP Maxenttokenizer

————————————————————————————————————————


# 三、openNLP在R中的实现以及使用心得

## 1、使用前预加载

openNLP基于众多语料训练了很多已有的实体词、取词场景等，在R中需要到指定网址下载（[http://datacube.wu.ac.at/](http://datacube.wu.ac.at/)），这里笔者也附上openNLP的官方链接（[http://opennlp.sourceforge.net/models-1.5/](http://opennlp.sourceforge.net/models-1.5/)）
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.da_1.5-2.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.da_1.5-2.tar.gz)|2015-04-08 09:56|1.2M|基于conllx ddt 数据的分词、句子探测器、POS标注|
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.de_1.5-2.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.de_1.5-2.tar.gz)|2015-04-08 09:56|8.0M|tiger data数据集分词、句子探测器、POS标注|
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.en_1.5-1.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.en_1.5-1.tar.gz)|2015-04-08 09:57|74M|opennlp training data分词、句子探测器、POS标注、命名实体识别、解析器、序列标注|
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.es_1.5-1.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.es_1.5-1.tar.gz)|2015-04-08 09:57|3.8M|conll02 shared task data的命名实体识别|
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.it_1.5-1.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.it_1.5-1.tar.gz)|2015-04-08 09:57|25M|
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.nl_1.5-2.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.nl_1.5-2.tar.gz)|2015-04-08 09:57|4.1M|conllx alpino data的分词、命名实体、POS标注|
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.pt_1.5-2.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.pt_1.5-2.tar.gz)|2015-04-08 09:57|2.1M|conllx bosque data的POS标注、句子探测、分词|
![[   ]](http://datacube.wu.ac.at/icons/compressed.gif)|[openNLPmodels.sv_1.5-2.tar.gz](http://datacube.wu.ac.at/src/contrib/openNLPmodels.sv_1.5-2.tar.gz)|2015-04-08 09:57|2.4M|
其中词性标注有：最大熵模型（Part of Speech Tagger）、感知器模型Perceptron model（pos标注），R只有前者。
从大小来看也知道en是最全面的，所以要下载。
下载的方式跟调用包一样方便：
`install.packages("foo", repos = "http://datacube.wu.ac.at/", type = "source")`其中foo代表着你要下载内容的名称，比如：openNLPmodels.en

## 2、R语言实践——官方案例

### （1）先得到序列：
`s <- paste(c("Pierre Vinken, 61 years old, will join the board as a ",
             "nonexecutive director Nov. 29.\n",
             "Mr. Vinken is chairman of Elsevier N.V., ","the Dutch publishing group."),
           collapse = "")
s <- as.String(s)   #library(NLP)`
### （2）设置分词器以及句子探测器：

`sent_token_annotator <- Maxent_Sent_Token_Annotator()
word_token_annotator <- Maxent_Word_Token_Annotator()
pos_tag_annotator <- Maxent_POS_Tag_Annotator()`
### （3）
### 解析分词、词性、整句到a3：

`a3 <- annotate(s,
               list(sent_token_annotator,
                    word_token_annotator,
                    pos_tag_annotator))`
### （4）
### 进行后续的分析（序列标注、命名实体识别）：
序列标注的code：
`annotate(s, Maxent_Chunk_Annotator(probs = TRUE), a3)`实体识别的code：
`annotate(s, Maxent_Entity_Annotator(probs = TRUE), a2)`
其中probs代表是否需要从最大熵模型中提取出概率。



# 参考文献：
1、open在案例：[OpenNLP Tutorial](http://www.programcreek.com/2012/05/opennlp-tutorial/)
2、openNLP在R中的数据集：http://datacube.wu.ac.at/
3、R中的openNLP文档:https://cran.r-project.org/web/packages/openNLP/openNLP.pdf
**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


