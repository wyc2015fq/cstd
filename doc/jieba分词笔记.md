# jieba分词笔记





## 中文分词

​      在介绍结巴中文分词前，先简单介绍一下中文分词。中文分词(Chinese Word Segmentation) 指的是将一个汉字序列切分成一个一个单独的词。分词就是将连续的字序列按照一定的规范重新组合成词序列的过程。

最常见的分词算法可以分为三大类：基于字符串匹配的分词方法、基于理解的分词方法、基于统计的分词方法

### 基于字符串匹配的分词方法

这种方法又叫做机械分词方法，它是按照一定的策略将待分析的汉字串与一个“充分大的”机器词典中的词条进行配，若在词典中找到某个字符串，则匹配成功（识别出一个词）

​    1）正向最大匹配法（由左到右的方向）

   2）逆向最大匹配法（由右到左的方向）:

   3）最少切分（使每一句中切出的词数最小）

   4）双向最大匹配法（进行由左到右、由右到左两次扫描）

### 基于理解的分词方法

这种分词方法是通过让计算机模拟人对句子的理解，达到识别词的效果。其基本思想就是在分词的同时进行句法、语义分析，利用句法信息和语义信息来处理歧义现象。它通常包括三个部分：分词子系统、句法语义子系统、总控部分。在总控部分的协调下，分词子系统可以获得有关词、句子等的句法和语义信息来对分词歧义进行判断，即它模拟了人对句子的理解过程。这种分词方法需要使用大量的语言知识和信息。由于汉语语言知识的笼统、复杂性，难以将各种语言信息组织成机器可直接读取的形式，因此目前基于理解的分词系统还处在试验阶段。

### 基于统计的分词方法

给出大量已经分词的文本，利用统计机器学习模型学习词语切分的规律（称为训练），从而实现对未知文本的切分。例如最大概率分词方法和最大熵分词方法等。随着大规模语料库的建立，统计机器学习方法的研究和发展，基于统计的中文分词方法渐渐成为了主流方法。



## 结巴中文分词简介

1）支持三种分词模式：

- 精确模式：将句子最精确的分开，适合文本分析
- 全模式：句子中所有可以成词的词语都扫描出来，速度快，不能解决歧义
- 搜索引擎模式：在精确的基础上，对长词再次切分，提高召回

2）支持繁体分词

3）支持自定义词典

4）基于Trie树结构实现高效的词图扫描，生成句子汉字所有可能成词情况所构成的有向无环图（DAG）

5)  采用了动态规划查找最大概率路径，找出基于词频的最大切分组合

6）对于词库中不存在的词，也就是未登录词，采用了基于汉字成词能力的HMM模型，使用了Viterbi算法



## 流程图

## ![img](https://img-blog.csdn.net/2018071718075458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM5ODI5MjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## GitHub

https://github.com/fxsjy/jieba

## 安装

```shell
pip install jieba
```

## 测试

```python
import jieba
seg_list = jieba.cut("我来到北京清华大学", cut_all=True)
print("Full Mode: " + "/ ".join(seg_list))
```



## 常用方法

- `jieba.cut` 方法接受三个输入参数: 需要分词的字符串；cut_all 参数用来控制是否采用全模式；HMM 参数用来控制是否使用 HMM 模型
- `jieba.cut_for_search` 方法接受两个参数：需要分词的字符串；是否使用 HMM 模型。该方法适合用于搜索引擎构建倒排索引的分词，粒度比较细
- 待分词的字符串可以是 unicode 或 UTF-8 字符串、GBK 字符串。注意：不建议直接输入 GBK 字符串，可能无法预料地错误解码成 UTF-8
- `jieba.cut` 以及 `jieba.cut_for_search` 返回的结构都是一个可迭代的 generator，可以使用 for 循环来获得分词后得到的每一个词语(unicode)，或者用
- `jieba.lcut` 以及 `jieba.lcut_for_search` 直接返回 list
- `jieba.Tokenizer(dictionary=DEFAULT_DICT)` 新建自定义分词器，可用于同时使用不同词典。`jieba.dt` 为默认分词器，所有全局分词相关函数都是该分词器的映射。



代码示例

```python
# encoding=utf-8
import jieba

seg_list = jieba.cut("我来到北京清华大学", cut_all=True)
print("Full Mode: " + "/ ".join(seg_list))  # 全模式

seg_list = jieba.cut("我来到北京清华大学", cut_all=False)
print("Default Mode: " + "/ ".join(seg_list))  # 精确模式

seg_list = jieba.cut("他来到了网易杭研大厦")  # 默认是精确模式
print(", ".join(seg_list))

seg_list = jieba.cut_for_search("小明硕士毕业于中国科学院计算所，后在日本京都大学深造")  # 搜索引擎模式
print(", ".join(seg_list))
```

输出:

```
【全模式】: 我/ 来到/ 北京/ 清华/ 清华大学/ 华大/ 大学
【精确模式】: 我/ 来到/ 北京/ 清华大学
【新词识别】：他, 来到, 了, 网易, 杭研, 大厦    (此处，“杭研”并没有在词典中，但是也被Viterbi算法识别出来了)
【搜索引擎模式】： 小明, 硕士, 毕业, 于, 中国, 科学, 学院, 科学院, 中国科学院, 计算, 计算所, 后, 在, 日本, 京都, 大学, 日本
```

## 添加自定义词典



- 开发者可以指定自己自定义的词典，以便包含 jieba 词库里没有的词。虽然 jieba 有新词识别能力，但是自行添加新词可以保证更高的正确率
- 用法： jieba.load_userdict(file_name) # file_name 为文件类对象或自定义词典的路径
- 词典格式和 `dict.txt` 一样，一个词占一行；每一行分三部分：词语、词频（可省略）、词性（可省略），用空格隔开，顺序不可颠倒。`file_name` 若为路径或二进制方式打开的文件，则文件必须为 UTF-8 编码。
- 词频省略时使用自动计算的能保证分出该词的词频。



**例如：**

```
创新办 3 i
云计算 5
凱特琳 nz
台中
```

- 更改分词器（默认为 `jieba.dt`）的 `tmp_dir` 和 `cache_file` 属性，可分别指定缓存文件所在的文件夹及其文件名，用于受限的文件系统。
- 范例：
  - 自定义词典：<https://github.com/fxsjy/jieba/blob/master/test/userdict.txt>
  - 用法示例：<https://github.com/fxsjy/jieba/blob/master/test/test_userdict.py>
    - 之前： 李小福 / 是 / 创新 / 办 / 主任 / 也 / 是 / 云 / 计算 / 方面 / 的 / 专家 /
    - 加载自定义词库后：　李小福 / 是 / 创新办 / 主任 / 也 / 是 / 云计算 / 方面 / 的 / 专家 /

## 调整词典

- 使用 `add_word(word, freq=None, tag=None)` 和 `del_word(word)` 可在程序中动态修改词典。
- 使用 `suggest_freq(segment, tune=True)` 可调节单个词语的词频，使其能（或不能）被分出来。
- 注意：自动计算的词频在使用 HMM 新词发现功能时可能无效。

代码示例：

```
>>> print('/'.join(jieba.cut('如果放到post中将出错。', HMM=False)))
如果/放到/post/中将/出错/。
>>> jieba.suggest_freq(('中', '将'), True)
494
>>> print('/'.join(jieba.cut('如果放到post中将出错。', HMM=False)))
如果/放到/post/中/将/出错/。
>>> print('/'.join(jieba.cut('「台中」正确应该不会被切开', HMM=False)))
「/台/中/」/正确/应该/不会/被/切开
>>> jieba.suggest_freq('台中', True)
69
>>> print('/'.join(jieba.cut('「台中」正确应该不会被切开', HMM=False)))
「/台中/」/正确/应该/不会/被/切开
```

- "通过用户自定义词典来增强歧义纠错能力" --- <https://github.com/fxsjy/jieba/issues/14>



## 基于 TF-IDF 算法的关键词抽取

```python
import jieba.analyse

jieba.analyse.extract_tags(sentence, topK=20, withWeight=False, allowPOS=())
  # sentence 为待提取的文本
  # topK 为返回几个 TF/IDF 权重最大的关键词，默认值为 20
  # withWeight 为是否一并返回关键词权重值，默认值为 False
  # allowPOS 仅包括指定词性的词，默认值为空，即不筛选
 jieba.analyse.TFIDF(idf_path=None) # 新建 TFIDF 实例，idf_path 为 IDF 频率文件
```
代码示例 （关键词提取）

```python
import jieba.analyse
 
kWords = jieba.analyse.extract_tags("此外，公司拟对全资子公司吉林欧亚置业>有限公司增资4.3亿元，增资后，吉林欧亚置业注册资本由7000万元增加到5亿元。>吉林欧亚置业主要经营范围为房地产开发及百货零售等业务。目前在建吉林欧亚城>市商业综合体项目。2013年，实现营业收入0万元，实现净利润-139.13万元。", topK=5,withWeight=True)
for word in kWords:
   # print(word+":"+weight)
   print (word)
```

```
('欧亚', 0.7458841454643479)
('吉林', 0.6733651014252174)
('置业', 0.49933765769413047)
('万元', 0.3466477318421739)
('增资', 0.3431245420230435)
```



<https://github.com/fxsjy/jieba/blob/master/test/extract_tags.py>

关键词提取所使用逆向文件频率（IDF）文本语料库可以切换成自定义语料库的路径

- 用法： jieba.analyse.set_idf_path(file_name) # file_name为自定义语料库的路径
- 自定义语料库示例：<https://github.com/fxsjy/jieba/blob/master/extra_dict/idf.txt.big>
- 用法示例：<https://github.com/fxsjy/jieba/blob/master/test/extract_tags_idfpath.py>

关键词提取所使用停止词（Stop Words）文本语料库可以切换成自定义语料库的路径

- 用法： jieba.analyse.set_stop_words(file_name) # file_name为自定义语料库的路径
- 自定义语料库示例：<https://github.com/fxsjy/jieba/blob/master/extra_dict/stop_words.txt>
- 用法示例：<https://github.com/fxsjy/jieba/blob/master/test/extract_tags_stop_words.py>

关键词一并返回关键词权重值示例

- 用法示例：<https://github.com/fxsjy/jieba/blob/master/test/extract_tags_with_weight.py>

## 基于 TextRank 算法的关键词抽取

```python
jieba.analyse.textrank(sentence, topK=20, withWeight=False, allowPOS=('ns', 'n', 'vn', 'v')) ##直接使用，接口相同，注意默认过滤词性。
jieba.analyse.TextRank() # 新建自定义 TextRank 实例
```

  

算法论文： [TextRank: Bringing Order into Texts](http://web.eecs.umich.edu/~mihalcea/papers/mihalcea.emnlp04.pdf)

基本思想

1. 将待抽取关键词的文本进行分词
2. 以固定窗口大小(默认为5，通过span属性调整)，词之间的共现关系，构建图
3. 计算图中节点的PageRank，注意是无向带权图

使用示例

见 [test/demo.py](https://github.com/fxsjy/jieba/blob/master/test/demo.py)

## 词性标注

```
jieba.posseg.POSTokenizer(tokenizer=None) # 新建自定义分词器，`tokenizer` 参数可指定内部使用的 `jieba.Tokenizer` 分词器。`jieba.posseg.dt` 为默认词性标注分词器。
# 标注句子分词后每个词的词性，采用和 ictclas 兼容的标记法。
```

用法示例

```python
import jieba.posseg as pseg
words = pseg.cut("我爱北京天安门")
for word, flag in words:
    print('%s %s' % (word, flag))
```
```
我 r
爱 v
北京 ns
天安门 ns
```

## 并行分词


- 原理：将目标文本按行分隔后，把各行文本分配到多个 Python 进程并行分词，然后归并结果，从而获得分词速度的可观提升
- 基于 python 自带的 multiprocessing 模块，目前暂不支持 Windows

用法：

```python
jieba.enable_parallel(4) # 开启并行分词模式，参数为并行进程数
jieba.disable_parallel() # 关闭并行分词模式
```
- 例子：<https://github.com/fxsjy/jieba/blob/master/test/parallel/test_file.py>
- 实验结果：在 4 核 3.4GHz Linux 机器上，对金庸全集进行精确分词，获得了 1MB/s 的速度，是单进程版的 3.3 倍。
- **注意**：并行分词仅支持默认分词器 `jieba.dt` 和 `jieba.posseg.dt`。

## Tokenize：返回词语在原文的起止位置


注意，输入参数只接受 unicode

默认模式

```python
result = jieba.tokenize(u'永和服装饰品有限公司')
for tk in result:
    print("word %s\t\t start: %d \t\t end:%d" % (tk[0],tk[1],tk[2]))
```
```
word 永和                start: 0                end:2
word 服装                start: 2                end:4
word 饰品                start: 4                end:6
word 有限公司            start: 6                end:10 
```

搜索模式

```python
result = jieba.tokenize(u'永和服装饰品有限公司', mode='search')
for tk in result:
    print("word %s\t\t start: %d \t\t end:%d" % (tk[0],tk[1],tk[2]))
```
```
word 永和                start: 0                end:2
word 服装                start: 2                end:4
word 饰品                start: 4                end:6
word 有限                start: 6                end:8
word 公司                start: 8                end:10
word 有限公司            start: 6                end:10
```

## ChineseAnalyzer for Whoosh 搜索引擎

------

- 引用： `from jieba.analyse import ChineseAnalyzer`
- 用法示例：<https://github.com/fxsjy/jieba/blob/master/test/test_whoosh.py>

## 命令行分词

------

使用示例：`python -m jieba news.txt > cut_result.txt`

命令行选项（翻译）：

```shell
使用: python -m jieba [options] filename
 
结巴命令行界面。
 
固定参数:
  filename              输入文件
 
可选参数:
  -h, --help            显示此帮助信息并退出
  -d [DELIM], --delimiter [DELIM]
                        使用 DELIM 分隔词语，而不是用默认的' / '。
                        若不指定 DELIM，则使用一个空格分隔。
  -p [DELIM], --pos [DELIM]
                        启用词性标注；如果指定 DELIM，词语和词性之间
                        用它分隔，否则用 _ 分隔
  -D DICT, --dict DICT  使用 DICT 代替默认词典
  -u USER_DICT, --user-dict USER_DICT
                        使用 USER_DICT 作为附加词典，与默认词典或自定义词典配合使用
  -a, --cut-all         全模式分词（不支持词性标注）
  -n, --no-hmm          不使用隐含马尔可夫模型
  -q, --quiet           不输出载入信息到 STDERR
  -V, --version         显示版本信息并退出
 
如果没有指定文件名，则使用标准输入。
```

`--help` 选项输出：

```shell
$> python -m jieba --help
Jieba command line interface.
 
positional arguments:
  filename              input file
 
optional arguments:
  -h, --help            show this help message and exit
  -d [DELIM], --delimiter [DELIM]
                        use DELIM instead of ' / ' for word delimiter; or a
                        space if it is used without DELIM
  -p [DELIM], --pos [DELIM]
                        enable POS tagging; if DELIM is specified, use DELIM
                        instead of '_' for POS delimiter
  -D DICT, --dict DICT  use DICT as dictionary
  -u USER_DICT, --user-dict USER_DICT
                        use USER_DICT together with the default dictionary or
                        DICT (if specified)
  -a, --cut-all         full pattern cutting (ignored with POS tagging)
  -n, --no-hmm          don't use the Hidden Markov Model
  -q, --quiet           don't print loading messages to stderr
  -V, --version         show program's version number and exit
If no filename specified, use STDIN instead.
```

## 延迟加载机制

jieba 采用延迟加载，`import jieba` 和 `jieba.Tokenizer()` 不会立即触发词典的加载，一旦有必要才开始加载词典构建前缀字典。如果你想手工初始 jieba，也可以手动初始化。

```python
import jieba
jieba.initialize()  # 手动初始化（可选）
```

在 0.28 之前的版本是不能指定主词典的路径的，有了延迟加载机制后，你可以改变主词典的路径:

```python
jieba.set_dictionary('data/dict.txt.big')
```

例子： <https://github.com/fxsjy/jieba/blob/master/test/test_change_dictpath.py>









## 词性类型



| **Ag** | 形语素   | 形容词性语素。形容词代码为 a，语素代码ｇ前面置以A。          |
| ------ | -------- | ------------------------------------------------------------ |
| **a**  | 形容词   | 取英语形容词 adjective的第1个字母。                          |
| **ad** | 副形词   | 直接作状语的形容词。形容词代码 a和副词代码d并在一起。        |
| **an** | 名形词   | 具有名词功能的形容词。形容词代码 a和名词代码n并在一起。      |
| **b**  | 区别词   | 取汉字“别”的声母。                                           |
| **c**  | 连词     | 取英语连词 conjunction的第1个字母。                          |
| **dg** | 副语素   | 副词性语素。副词代码为 d，语素代码ｇ前面置以D。              |
| **d**  | 副词     | 取 adverb的第2个字母，因其第1个字母已用于形容词。            |
| **e**  | 叹词     | 取英语叹词 exclamation的第1个字母。                          |
| **f**  | 方位词   | 取汉字“方”                                                   |
| g      | 语素     | 绝大多数语素都能作为合成词的“词根”，取汉字“根”的声母。       |
| h      | 前接成分 | 取英语 head的第1个字母。                                     |
| i      | 成语     | 取英语成语 idiom的第1个字母。                                |
| j      | 简称略语 | 取汉字“简”的声母。                                           |
| k      | 后接成分 |                                                              |
| **l**  | 习用语   | 习用语尚未成为成语，有点“临时性”，取“临”的声母。             |
| **m**  | 数词     | 取英语 numeral的第3个字母，n，u已有他用。                    |
| **Ng** | 名语素   | 名词性语素。名词代码为 n，语素代码ｇ前面置以N。              |
| **n**  | 名词     | 取英语名词 noun的第1个字母。                                 |
| **nr** | 人名     | 名词代码 n和“人(ren)”的声母并在一起。                        |
| **ns** | 地名     | 名词代码 n和处所词代码s并在一起。                            |
| **nt** | 机构团体 | “团”的声母为 t，名词代码n和t并在一起。                       |
| **nz** | 其他专名 | “专”的声母的第 1个字母为z，名词代码n和z并在一起。            |
| **o**  | 拟声词   | 取英语拟声词 onomatopoeia的第1个字母。                       |
| **p**  | 介词     | 取英语介词 prepositional的第1个字母。                        |
| **q**  | 量词     | 取英语 quantity的第1个字母。                                 |
| **r**  | 代词     | 取英语代词 pronoun的第2个字母,因p已用于介词。                |
| **s**  | 处所词   | 取英语 space的第1个字母。                                    |
| **tg** | 时语素   | 时间词性语素。时间词代码为 t,在语素的代码g前面置以T。        |
| **t**  | 时间词   | 取英语 time的第1个字母。                                     |
| u      | 助词     | 取英语助词 auxiliary                                         |
| **vg** | 动语素   | 动词性语素。动词代码为 v。在语素的代码g前面置以V。           |
| v      | 动词     | 取英语动词 verb的第一个字母。                                |
| **vd** | 副动词   | 直接作状语的动词。动词和副词的代码并在一起。                 |
| **vn** | 名动词   | 指具有名词功能的动词。动词和名词的代码并在一起。             |
| **w**  | 标点符号 |                                                              |
| x      | 非语素字 | 非语素字只是一个符号，字母 x通常用于代表未知数、符号。       |
| **y**  | 语气词   | 取汉字“语”的声母。                                           |
| **z**  | 状态词   | 取汉字“状”的声母的前一个字母。                               |
| **un** | 未知词   | 不可识别词及用户自定义词组。取英文Unkonwn首两个字母。**(非北大标准，CSW分词中定义)** |



## 注意：

​      jieba分词器在处理中文分词中是非常好的，但是，jieba分词器默认的分词语料库dict.txt确存在问题： 
​      dict.txt中存在许多单字，例如：”大”,”午”,”我”，而且给予单字的词频特别高，都具有万以上的词频数，我们都知道词频与权重是成正比的，那么在分词的过程中就会优先处理这些单词，而与其相关的词语就会被忽略，这就导致分词结果不准确的问题。 

​      为了更好的使用jieba分词，建议将dict.txt中的单字全部去掉，或者将其词频设置非常小。





```python
# -*--coding:utf-8*-

import jieba
import jieba.analyse
stopwords = {}.fromkeys([line.rstrip() for line in open('stopkey_zh.txt')])

"""
    jieba 中文分词：
        支持三种分词模式：
            精确模式：将句子最精确的分开，适合文本分析
            全模式：把句子中所有的可以成词的词语都扫描出来，速度非常快，但是不能解决歧义
            搜索引擎模式：在精确搜索的基础上，对长词进行再次切分，提高召回率，适合用于搜索引擎分词
        支持繁体分词：
        支持自定义词典：
        MIT授权协议

    算法：
        基于前缀词典实现高效的词图扫描，生成句子中汉字所有可能成词情况所构成的有向无环图（DAG）
        采用了动态规划查找最大概率路径，找出基于词频的最大切分组合
        对于未登录词，采用了基于汉字成词能力的HMM模型，使用了Viterbi算法

    主要功能：
        1、分词
            jieba.cut():三个参数，分词的字符串；cut_all用来控制是否采用全模式；HMM控制是否使用HMM模型
            jieba.cut_for_search():两个参数，分词的字符串；是否使用HMM模型；
                上述两个分词方法都可以使用for循环来获取分词后的每个词语。
            jieba.lcut():直接返回list
            jieba.lcut_for_search():直接返回list
            jieba.Tokenizer(dictionary=DEFAULT_DICT):新建自定义分词器
        2、添加自定义词典
            jieba.load_userdict(file_name):file_name若为打开的文件，文件必须为UTF8格式的
                词典格式与dict.txt一样，一个词占一行，每一行分三部分：词语、词频、词性，用空格隔开
            jieba.add_word():动态增加词典
            jieba.del_word():动态删除词典
            jieba.suggeat_freq(segment, tune=True) 调节单个词语的词频，True使其能够分出来, False使其不能分出来
                注意jieba默认的dict中的词语，要结合默认dict进行中文分词
        3、关键词提取
            jieba.analyse.extract_tags(sentence, topK=20, withWeight=False, allowPOS=()):
                sentence:待提取的文本
                topK:返回几个TF-IDF权重最大的关键词，默认值为20
                withWeight:是否一并返回关键词权重值，默认值为False
                allowPOS:仅包括指定词性的词，默认值为空
            jieba.analyse.TFIDF(idf_path=None):
            jieba.analyse.set_idf_path(file_name):
            jieba.analyse.set_stop_words(file_name):
            jieba.analyse.textrank(sentence, topK=20,withWeight=False,allowPOS=()):
            jieba.analyse.TextRank():
        4、词性标注
            jieba.posseg.POSTokenizer(tokenizer=None):
        5、并行分词
            jieba.enable_parallel(4):开启并行模式
            jieba.disable_parallel():关闭并行模式
            注意：并行分词不能在windows下运行
        6、Tokenize：返回词语在原文的起止位置
            jieba.tokenize():
        7、ChineseAnalyzer for Whoosh 搜索引擎
        8、命令行分词
"""
# 0、读取文本
f = open('sentences.txt')
s = f.read()
print s
print type(s)
print '----------------------------------------------'

final = ''
for st in s:
    # st = st.encode('utf8')
    if st not in stopwords:
        final += st
        final += ' '
print final
print type(final)
print '----------------------------------------------'

# 1、分词
seg_list = jieba.cut(s, cut_all=True)  # 全模式
print '[01 Full Model]:' + '/'.join(seg_list)
print '--------------'
seg_list = jieba.cut(s, cut_all=False)  # 精确模式
print '[02 Accuracy Model:]:' + '/'.join(seg_list)
print '--------------'
seg_list = jieba.cut(s, cut_all=False, HMM=True)  # 精确模式，HMM模型
print '[03 Accuracy Model:]:' + '/'.join(seg_list)
print '--------------'
seg_list = jieba.cut(s)  # 默认为精确模式
print '[04 Accuracy Model in Default:]:' + '/'.join(seg_list)
print '--------------'
seg_list = jieba.cut_for_search(s)  # 搜索引擎模式
print '[05 Search Engine Model:]:' + '/'.join(seg_list)
print '-----------------------------------------------'

# 2、添加自定义词典
jieba.load_userdict('userdict.txt')
jieba.add_word('两国元首')  # 添加词典
jieba.del_word('两国元首')  # 删除词典
jieba.suggest_freq(('日', '电'), True)
jieba.suggest_freq('日电', False)
words = jieba.cut(s)  # 默认方式
print '/'.join(words)
print '-----------------------------------------------'

# 3、关键词提取
# 基于TF-IDF算法的关键词提取
import jieba
import jieba.analyse

# # 关键词提取
tags = jieba.analyse.extract_tags(s, topK=10)  # 提取关键词最高的10个词
print ','.join(tags)
print '--------------'
# # 自定义逆向文件频率(IDF)语料库
jieba.analyse.set_idf_path('../extra_dict/idf.txt.big')
tags = jieba.analyse.extract_tags(s, topK=10)
print ','.join(tags)
print '--------------'
# # 自定义停用词(Stop words)文本语料库
jieba.analyse.set_idf_path("../extra_dict/idf.txt.big")
tags = jieba.analyse.extract_tags(s, topK=10)
print ",".join(tags)
print '--------------'
# # 返回关键词权重值
tags = jieba.analyse.extract_tags(s, topK=15, withWeight=True)
for tag in tags:
    print 'tag:%s\t\t weight:%f' % (tag[0], tag[1])  # 输出关键词及其权重

print '--------------'

# 基于TextRank算法的关键词抽取
# 基本思想:
#   将待抽取关键词的文本进行分词
#   以固定窗口大小(默认为5，通过span属性调整)，词之间的共现关系，构建图
#   计算图中节点的PageRank，注意是无向带权图
tags = jieba.analyse.textrank(s, topK=10)
print ','.join(tags)
print '-----------------------------------------------'

# 4、词性标注
import jieba.posseg as pseg

words = pseg.cut('我爱北京天安门')
for word, flag in words:
    print '%s  %s' % (word, flag)
print '-----------------------------------------------'
print '-----------------------------------------------'

# 5、并行分词
# file = open('QinCountry.txt')  # 大秦帝国六部全集
# ss = file.read()
# print ss

# import time

# jieba.enable_parallel()  # windows下不能并行处理
# t1 = time.time()
# seg_list = jieba.cut(ss)
# words = '/'.join(seg_list)
# t2 = time.time()
# cost = t2-t1
# print 'Cost Time:%s' % cost  # Cost Time:61.7459998131
print '----------------------------------------------'

# 6、Tokenize  返回词语在原文的起止位置，输入参数只接受unicode
result = jieba.tokenize(s.decode('utf8'))
for t in result:
    print 'word:%s\t\tstart:%s\t\tend:%s' % (t[0], t[1], t[2])
print '----------------------------------------------'

# 7、ChineseAnalyzer for Whoosh 搜索引擎
# import jieba
# from jieba.analyse import ChineseAnalyzer
#
# analyser = ChineseAnalyzer()
# for t in analyzer("我的好朋友是李明;我爱北京天安门;IBM和Microsoft; I have a dream. this is intetesting and interested me a lot"):
#     print t.text
```

