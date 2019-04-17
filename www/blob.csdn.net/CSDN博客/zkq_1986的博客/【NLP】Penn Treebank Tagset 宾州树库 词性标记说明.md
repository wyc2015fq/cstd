# 【NLP】Penn Treebank Tagset 宾州树库 词性标记说明 - zkq_1986的博客 - CSDN博客





2018年08月18日 21:54:33[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：316








转自：[http://blog.csdn.net/wskings/article/details/17607021](http://blog.csdn.net/wskings/article/details/17607021)

> 
最近在做命名实体识别，用到Stanford-CoreNlp词性标记，由于不是语言学专业，对一些词性不是很熟悉，为更好的了解各词的词性，查了一些资料，总结如下，各类别如图1。 
![这里写图片描述](https://img-blog.csdn.net/20170510165832560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjc0Mzc5Njc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 各类别中包含的词性详注
|CC|Coordinating conjunction **e.g.** and,but,or…|
|----|----|
|CD|Cardinal Number|
|DT|Determiner|
|EX|Existential *there*|
|FW|Foreign Word|
|IN|Preposision or subordinating conjunction|
|JJ|Adjective|
|JJR|Adjective, comparative|
|JJS|Adjective, superlative|
|LS|List Item Marker|
|MD|Modal **e.g.** can, could, might, may…|
|NN|Noun, singular or mass|
|NNP|Proper Noun, singular|
|NNPS|Proper Noun, plural|
|NNS|Noun, plural|
|PDT|Predeterminer **e.g.** all, both … when they precede an article|
|POS|Possessive Ending **e.g.** Nouns ending in ‘s|
|PRP|Personal Pronoun **e.g.** I, me, you, he…|
|PRP$|Possessive Pronoun **e.g.** my, your, mine, yours…|
|RB|Adverb 			Most words that end in -ly as well as degree words like quite, too and very|
|RBR|Adverb, comparative 			Adverbs with the comparative ending -er, with a strictly comparative meaning.|
|RBS|Adverb, superlative|
|RP|Particle|
|SYM|Symbol 			Should be used for mathematical, scientific or technical symbols|
|TO|*to*|
|UH|Interjection **e.g.** uh, well, yes, my…|
|VB|Verb, base form 			subsumes imperatives, infinitives and subjunctives|
|VBD|Verb, past tense 			includes the conditional form of the verb to be|
|VBG|Verb, gerund or persent participle|
|VBN|Verb, past participle|
|VBP|Verb, non-3rd person singular present|
|VBZ|Verb, 3rd person singular present|
|WDT|Wh-determiner **e.g.** which, and *that* when it is used as a relative pronoun|
|WP|Wh-pronoun **e.g.** what, who, whom…|
|WP$|Possessive wh-pronoun **e.g.**|
|WRB|Wh-adverb **e.g.** how, where why|

```
以动词类别为例：
VB: 动词基础形式，包含必要条件，不定期和次要行为 
VBD: 动词过去式，包括动词的条件形式 
VBG: 动词gerund或persent分词 
VBN: 动词过去分词 
VBP: 动词非第三人称单数存在，如have 
VBZ: 动词第三人称单数，如has
```
- 



