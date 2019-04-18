# 自然语言处理（NLP）——分词统计itertools.chain—nltk工具 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 20:40:16[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：102
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)












### 文章目录
- [一、itertools.chain( *[ ] )](#itertoolschain____1)
- [二、NLTK工具：条件频率分布、正则表达式、词干提取器和归并器。](#NLTK_14)
- [2.1 nltk  分句—分词](#21_nltk___15)
- [Sentences Segment（分句）](#Sentences_Segment_25)
- [Tokenize sentences （分词）](#Tokenize_sentences__38)
- [2.2 nltk提供了两种常用的接口：`FreqDist` 和 `ConditionalFreqDist`](#22_nltkFreqDist__ConditionalFreqDist_66)
- [`FreqDist` 使用](#FreqDist__67)
- [`ConditionalFreqDist`  使用](#ConditionalFreqDist___80)
- [2.3 正则表达式及其应用](#23__120)
- [2.4 词干提取器 和 归并器](#24____150)
- [利用词干提取器实现索引文本(concordance)](#concordance_162)




### 一、itertools.chain( *[ ] )

```python
import itertools

a= itertools.chain(['a','aa','aaa'])
b= itertools.chain(*['a','aa','aaa'])
print(list(a))
print(list(b))

输出：
[‘a’, ‘aa’, ‘aaa’]
[‘a’, ‘a’, ‘a’, ‘a’, ‘a’, ‘a’]
```

### 二、NLTK工具：条件频率分布、正则表达式、词干提取器和归并器。

##### 2.1 nltk  分句—分词
- NLTK文本分割：
`nltk.sent_tokenize(text)` #对文本按照句子进行分割
`nltk.word_tokenize(sent)`  #对句子进行分词- NLTK进行词性标注
`nltk.pos_tag(tokens)` #tokens是句子分词后的结果，同样是句子级的标注- NLTK进行命名实体识别（NER）
`nltk.ne_chunk(tags)` #tags是句子词性标注后的结果，同样是句子级
###### Sentences Segment（分句）

```python
sent_tokenizer = nltk.data.load('tokenizers/punkt/english.pickle')

paragraph = "The first time I heard that song was in Hawaii on radio. 
             I was just a kid, and loved it very much! What a fantastic song!"
             
print(sent_tokenizer.tokenize(paragraph))
输出：
['The first time I heard that song was in Hawaii on radio.', 
 'I was just a kid, and loved it very much!', 
 'What a fantastic song!']
```

###### Tokenize sentences （分词）

```python
from nltk.tokenize import WordPunctTokenizer

sentence = "Are you old enough to remember Michael Jackson attending 
	   the Grammys with Brooke Shields and Webster sat on his lap during the show?"

print(WordPunctTokenizer().tokenize(sentence))
输出：
['Are', 'you', 'old', 'enough', 'to', 'remember', 'Michael', 'Jackson', 'attending',
 'the', 'Grammys', 'with', 'Brooke', 'Shields', 'and', 'Webster', 'sat', 'on', 'his',
 'lap', 'during', 'the', 'show', '?'] 
 
----------------------------------------------------

text = 'That U.S.A. poster-print costs $12.40...'

pattern = r"""(?x)                  # set flag to allow verbose regexps
		     (?:[A-Z]\.)+           # abbreviations, e.g. U.S.A.
		     |\d+(?:\.\d+)?%?       # numbers, incl. currency and percentages
		     |\w+(?:[-']\w+)*       # words w/ optional internal hyphens/apostrophe
		     |\.\.\.                # ellipsis
		     |(?:[.,;"'?():-_`])    # special characters with meanings
		   """  

nltk.regexp_tokenize(text, pattern)
['That', 'U.S.A.', 'poster-print', 'costs', '12.40', '...']
```

##### 2.2 nltk提供了两种常用的接口：`FreqDist` 和 `ConditionalFreqDist`

###### `FreqDist` 使用

```python
from nltk import *
import matplotlib.pyplot as plt

tem = ['hello','world','hello','dear']
print(FreqDist(tem))

输出:
FreqDist({'dear': 1, 'hello': 2, 'world': 1}) 

通过 plot(TopK,cumulative=True) 和 tabulate() 可以绘制对应的折线图和表格
```

###### `ConditionalFreqDist`  使用

以一个配对链表作为输入，需要给分配的每个事件关联一个条件，

输入时类似于 (条件,事件) 的元组。

```python
import nltk
from nltk.corpus import brown 

cfd = nltk.ConditionalFreqDist((genre,word) \
                               for genre in brown.categories()\
                               for word in brown.words(categories=genre))
print("conditions are:",cfd.conditions()) #查看conditions
print(cfd['news'])
print(cfd['news']['could'])               #类似字典查询

输出:
conditions are: ['adventure', 'belles_lettres', 'editorial', 'fiction', 
                 'government', 'hobbies', 'humor', 'learned', 'lore', 'mystery', 
                 'news', 'religion', 'reviews', 'romance', 'science_fiction']
<FreqDist with 14394 samples and 100554 outcomes>
86  

"""
尤其对于plot() 和 tabulate() 有了更多参数选择：
    conditions：指定条件
    samples：   迭代器类型，指定取值范围
    cumulative：设置为True可以查看累积值

"""

cfd.tabulate(conditions=['news','romance'],samples=['could','can'])
cfd.tabulate(conditions=['news','romance'],samples=['could','can'],cumulative=True)
输出：
        could   can 
news       86    93 
romance   193    74 

        could   can 
news       86   179 
romance   193   267
```

##### 2.3 正则表达式及其应用

输入法联想提示（9宫格输入法）

```python
import re
from nltk.corpus import words

#查找类似于hole和golf序列（4653）的单词。
wordlist = [w for w in words.words('en-basic') if w.islower()]
same = [w for w in wordlist if re.search(r'^[ghi][mno][jlk][def]$',w)]
print(same)
```

寻找字符块 —查找两个或两个以上的元音序列，并且确定相对频率。

```python
import nltk

wsj = sorted(set(nltk.corpus.treebank.words()))
fd = nltk.FreqDist(vs for word in wsj for vs in re.findall(r'[aeiou]{2,}',word))
fd.items()
```

查找词干—apples和apple对比中，apple就是词干。写一个简单脚本来查询词干。

```python
def stem(word):
    for suffix in ['ing','ly','ed','ious','ies','ive','es','s','ment']:
        if word.endswith(suffix):
            return word[:-len(suffix)]
    return None 

或者使用正则表达式，只需要一行：
re.findall(r'^(.*?)(ing|ly|ed|ious|ies|ive|es|s|ment)$',word)
```

##### 2.4 词干提取器 和 归并器

nltk提供了`PorterStemmer` 和 `LancasterStemmer` 两个词干提取器，

Porter比较好，可以处理lying这样的单词。

```python
porter = nltk.PorterStemmer()
print(porter.stem('lying'))
---------------------------------------
词性归并器：WordNetLemmatizer

wnl = nltk.WordNetLemmatizer()
print(wnl.lemmatize('women'))
```

###### 利用词干提取器实现索引文本(concordance)

用到nltk.Index这个函数:`nltk.Index((word , i) for (i,word) in enumerate(['a','b','a']))`

```python
class IndexText:
    def __init__(self,stemmer,text):
        self._text = text
        self._stemmer = stemmer
        self._index = nltk.Index((self._stem(word),i) for (i,word) in enumerate(text))
    def _stem(self,word):
        return self._stemmer.stem(word).lower()
    def concordance(self,word,width =40):
        key = self._stem(word)
        wc = width/4 #words of context
        for i in self._index[key]:
            lcontext = ' '.join(self._text[int(i-wc):int(i)])
            rcontext = ' '.join(self._text[int(i):int(i+wc)])
            ldisplay = '%*s' % (width,lcontext[-width:])
            rdisplay = '%-*s' % (width,rcontext[:width])
            print(ldisplay,rdisplay) 
            
porter = nltk.PorterStemmer()                   #词干提取
grail = nltk.corpus.webtext.words('grail.txt')  
text = IndexText(porter,grail)
text.concordance('lie')
```



