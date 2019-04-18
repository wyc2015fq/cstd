# 【数据平台】python中文分词工具jieba - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年10月26日 16:23:14[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：726
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、官网：

https://pypi.python.org/pypi/jieba/

https://github.com/fxsjy/jieba




自动安装：pip install jieba 





2、特点：


- 支持三种分词模式：
- 精确模式，试图将句子最精确地切开，适合文本分析；
- 全模式，把句子中所有的可以成词的词语都扫描出来,速度非常快，但是不能解决歧义；
- 搜索引擎模式，在精确模式的基础上，对长词再次切分，提高召回率，适合用于搜索引擎分词。

- 支持繁体分词
- 支持自定义词典
- MIT 授权协议



3、主要功能：

1）分词：


- `jieba.cut` 方法接受三个输入参数: 需要分词的字符串；cut_all 参数用来控制是否采用全模式；HMM 参数用来控制是否使用 HMM 模型
- `jieba.cut_for_search` 方法接受两个参数：需要分词的字符串；是否使用 HMM 模型。该方法适合用于搜索引擎构建倒排索引的分词，粒度比较细
- 待分词的字符串可以是 unicode 或 UTF-8 字符串、GBK 字符串。注意：不建议直接输入 GBK 字符串，可能无法预料地错误解码成 UTF-8
- `jieba.cut` 以及 `jieba.cut_for_search` 返回的结构都是一个可迭代的 generator，可以使用 for 循环来获得分词后得到的每一个词语(unicode)，或者用
- `jieba.lcut` 以及 `jieba.lcut_for_search` 直接返回 list
- `jieba.Tokenizer(dictionary=DEFAULT_DICT)` 新建自定义分词器，可用于同时使用不同词典。`jieba.dt` 为默认分词器，所有全局分词相关函数都是该分词器的映射。

2）自定义词典



### 载入词典
- 开发者可以指定自己自定义的词典，以便包含 jieba 词库里没有的词。虽然 jieba 有新词识别能力，但是自行添加新词可以保证更高的正确率
- 用法： jieba.load_userdict(file_name) # file_name 为文件类对象或自定义词典的路径
- 词典格式和 `dict.txt` 一样，一个词占一行；每一行分三部分：词语、词频（可省略）、词性（可省略），用空格隔开，顺序不可颠倒。`file_name` 若为路径或二进制方式打开的文件，则文件必须为 UTF-8 编码。
- 词频省略时使用自动计算的能保证分出该词的词频。



3）关键词抽取



`import jieba.analyse`
- jieba.analyse.extract_tags(sentence, topK=20, withWeight=False, allowPOS=())
- sentence 为待提取的文本
- topK 为返回几个 TF/IDF 权重最大的关键词，默认值为 20
- withWeight 为是否一并返回关键词权重值，默认值为 False
- allowPOS 仅包括指定词性的词，默认值为空，即不筛选

- jieba.analyse.TFIDF(idf_path=None) 新建 TFIDF 实例，idf_path 为 IDF 频率文件


有TFIDF和TextRank两种算法




4）词性标注功能



- `jieba.posseg.POSTokenizer(tokenizer=None)` 新建自定义分词器，`tokenizer` 参数可指定内部使用的 `jieba.Tokenizer` 分词器。`jieba.posseg.dt` 为默认词性标注分词器。
- 标注句子分词后每个词的词性，采用和 ictclas 兼容的标记法。




4、Demo代码：



```python
#encoding=utf-8
'''
Created on 2017年10月26日

@author: Administrator
'''

from __future__ import unicode_literals
import sys
sys.path.append("../")

import jieba
import jieba.posseg
import jieba.analyse

print('='*40)
print('1. 分词')
print('-'*40)

seg_list = jieba.cut("我来到北京清华大学", cut_all=True)
print("Full Mode: " + "/ ".join(seg_list))  # 全模式

seg_list = jieba.cut("我来到北京清华大学", cut_all=False)
print("Default Mode: " + "/ ".join(seg_list))  # 默认模式

seg_list = jieba.cut("他来到了网易杭研大厦")
print(", ".join(seg_list))

seg_list = jieba.cut_for_search("小明硕士毕业于中国科学院计算所，后在日本京都大学深造")  # 搜索引擎模式
print(", ".join(seg_list))

print('='*40)
print('2. 添加自定义词典/调整词典')
print('-'*40)

print('/'.join(jieba.cut('如果放到post中将出错。', HMM=False)))
#如果/放到/post/中将/出错/。
print(jieba.suggest_freq(('中', '将'), True))
#494
print('/'.join(jieba.cut('如果放到post中将出错。', HMM=False)))
#如果/放到/post/中/将/出错/。
print('/'.join(jieba.cut('「台中」正确应该不会被切开', HMM=False)))
#「/台/中/」/正确/应该/不会/被/切开
print(jieba.suggest_freq('台中', True))
#69
print('/'.join(jieba.cut('「台中」正确应该不会被切开', HMM=False)))
#「/台中/」/正确/应该/不会/被/切开

print('='*40)
print('3. 关键词提取')
print('-'*40)
print(' TF-IDF')
print('-'*40)

s = "此外，公司拟对全资子公司吉林欧亚置业有限公司增资4.3亿元，增资后，吉林欧亚置业注册资本由7000万元增加到5亿元。吉林欧亚置业主要经营范围为房地产开发及百货零售等业务。目前在建吉林欧亚城市商业综合体项目。2013年，实现营业收入0万元，实现净利润-139.13万元。"
for x, w in jieba.analyse.extract_tags(s, withWeight=True):
    print('%s %s' % (x, w))

print('-'*40)
print(' TextRank')
print('-'*40)

for x, w in jieba.analyse.textrank(s, withWeight=True):
    print('%s %s' % (x, w))

print('='*40)
print('4. 词性标注')
print('-'*40)

words = jieba.posseg.cut("我爱北京天安门")
for word, flag in words:
    print('%s %s' % (word, flag))

print('='*40)
print('6. Tokenize: 返回词语在原文的起止位置')
print('-'*40)
print(' 默认模式')
print('-'*40)

result = jieba.tokenize('永和服装饰品有限公司')
for tk in result:
    print("word %s\t\t start: %d \t\t end:%d" % (tk[0],tk[1],tk[2]))

print('-'*40)
print(' 搜索模式')
print('-'*40)

result = jieba.tokenize('永和服装饰品有限公司', mode='search')
for tk in result:
    print("word %s\t\t start: %d \t\t end:%d" % (tk[0],tk[1],tk[2]))
```


结果：



```
========================================
1. 分词
----------------------------------------
Building prefix dict from the default dictionary ...
Loading model from cache c:\users\admini~1\appdata\local\temp\jieba.cache
Loading model cost 1.342 seconds.
Full Mode: 我/ 来到/ 北京/ 清华/ 清华大学/ 华大/ 大学
Default Mode: 我/ 来到/ 北京/ 清华大��
Prefix dict has been built succesfully.
他, 来到, 了, 网易, 杭研, 大厦
小明, 硕士, 毕业, 于, 中国, 科学, 学院, 科学院, 中国科学院, 计算, 计算所, ，, 后, 在, 日本, 京都, 大学, 日本京都大学, 深造
========================================
2. 添加自定义词典/调整词典
----------------------------------------
如果/放到/post/中将/出错/。
494
如果/放到/post/中/将/出错/��
「/台/中/」/正确/应该/不会/被/切��
69
「/台中/」/正确/应该/不会/被/切开
========================================
3. 关键词提取
----------------------------------------
 TF-IDF
----------------------------------------
欧亚 0.730014270029
吉林 0.659038184374
置业 0.488713452211
万元 0.339272248186
增资 0.335824019852
4.3 0.254356755381
7000 0.254356755381
139.13 0.254356755381
2013 0.254356755381
实现 0.199009799004
综合体 0.194803096247
经营范围 0.193897572536
亿元 0.191442162359
在建 0.175418847684
全资 0.171801649885
注册资本 0.1712441526
百货 0.167344600414
零售 0.147505711706
子公司 0.145960452378
营业 0.13920178509
----------------------------------------
 TextRank
----------------------------------------
吉林 1.0
欧亚 0.996689335418
置业 0.643436031309
实现 0.589860669286
收入 0.43677859948
增资 0.409990053128
子公司 0.356782959477
城市 0.349713836674
商业 0.34817220716
业务 0.309223099262
在建 0.307792916403
营业 0.303577704932
全资 0.303540981053
综合体 0.295808691724
注册资本 0.290005194641
有限公司 0.280783079858
零售 0.278836208612
百货 0.278165762845
开发 0.26934887793
经营范围 0.264276217356
========================================
4. 词性标注
----------------------------------------
我 r
爱 v
北京 ns
天安门 ns
========================================
6. Tokenize: 返回词语在原文的起止位置
----------------------------------------
 默认模式
----------------------------------------
word 永和		 start: 0 		 end:2
word 服装		 start: 2 		 end:4
word 饰品		 start: 4 		 end:6
word 有限公司		 start: 6 		 end:10
----------------------------------------
 搜索模式
----------------------------------------
word 永和		 start: 0 		 end:2
word 服装		 start: 2 		 end:4
word 饰品		 start: 4 		 end:6
word 有限		 start: 6 		 end:8
word 公司		 start: 8 		 end:10
word 有限公司		 start: 6 		 end:10
```





