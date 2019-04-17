# python结巴分词、jieba加载停用词表 - 博客堂 - CSDN博客





2017年09月03日 21:50:19[最小森林](https://me.csdn.net/u012052268)阅读数：69664








# python结巴分词




### 文章目录
- [python结巴分词](#python_0)
- [1 jieba中文分词简介](#1_jieba_2)
- [2 中文分词的原理](#2__10)
- [2.1 基于规则](#21__14)
- [2.2 基于统计](#22__24)
- [2.3 jieba的原理](#23_jieba_32)
- [3 安装结巴jieba](#3_jieba_43)
- [4 jieba三种分词模式以及其应用](#4_jieba_55)
- [5 jieba增强功能-加载自定义词典](#5_jieba_93)
- [5.1 载入新自定义词典](#51__95)
- [5.2 载入停用词表](#52__118)
- [6 jieba分词的其他应用](#6_jieba_153)
- [6.1 关键词提取](#61__155)
- [6.2 词性标注](#62__211)
- [7 用jieba分词实战（含文件的读取与存储）](#7_jieba_235)




## 1 jieba中文分词简介

中文分词是中文NLP的第一步，一个优秀的分词系统取决于足够的语料和完善的模型，很多机构和公司也都会开发和维护自己的分词系统。

这里推荐的是一款完全开源、简单易用的分词工具，jieba中文分词。官网在这里，[https://github.com/fxsjy/jieba](https://github.com/fxsjy/jieba) 里面提供了详细的说明文档。虽然jieba分词的性能并不是最优秀的，但它开源免费、使用简单、功能丰富，并且支持多种编程语言实现。

## 2 中文分词的原理

中文分词的模型实现主要分类两大类：基于规则和基于统计。

### 2.1 基于规则

基于规则是指根据一个已有的词典，采用前向最大匹配、后向最大匹配、双向最大匹配等人工设定的规则来进行分词。

例如对于“上海自来水来自海上”这句话，使用前向最大匹配，即从前向后扫描，使分出来的词存在于词典中并且尽可能长，则可以得到“上海/自来水/来自/海上”。这类方法思想简单且易于实现，对数据量的要求也不高。

当然，分词所使用的规则可以设计得更复杂，从而使分词效果更理想。但是由于中文博大精深、语法千变万化，很难设计足够全面而通用的规则，并且具体的上下文语境、词语之间的搭配组合也都会影响到最终的分词结果，这些挑战都使得基于规则的分词模型愈发力不从心。

### 2.2 基于统计

基于统计是从大量人工标注语料中总结词的概率分布以及词之间的常用搭配，使用有监督学习训练分词模型。

对于“上海自来水来自海上”这句话，一个最简单的统计分词想法是，尝试所有可能的分词方案，因为任何两个字之间，要么需要切分，要么无需切分。

对于全部可能的分词方案，根据语料统计每种方案出现的概率，然后保留概率最大的一种。很显然，“上海/自来水/来自/海上”的出现概率比“上海自/来水/来自/海上”更高，因为“上海”和“自来水”在标注语料中出现的次数比“上海自”和“来水”更多。

### 2.3 jieba的原理

jieba分词结合了基于规则和基于统计两类方法。

首先基于前缀词典进行词图扫描，前缀词典是指词典中的词按照前缀包含的顺序排列，例如词典中出现了“上”，之后以“上”开头的词都会出现在这一块，例如“上海”，进而会出现“上海市”，从而形成一种层级包含结构。

如果将词看作节点，词和词之间的分词符看作边，那么一种分词方案则对应着从第一个字到最后一个字的一条分词路径。因此，基于前缀词典可以快速构建包含全部可能分词结果的有向无环图，这个图中包含多条分词路径，有向是指全部的路径都始于第一个字、止于最后一个字，无环是指节点之间不构成闭环。

基于标注语料，使用动态规划的方法可以找出最大概率路径，并将其作为最终的分词结果。

## 3 安装结巴jieba

以下我们使用Python中的jieba分词完成一些基础NLP任务，如果对jieba分词感兴趣，希望了解更多内容，可以参考官方使用文档。首先没有jieba分词的话需要安装，使用pip即可。

```
pip install jieba
```

## 4 jieba三种分词模式以及其应用

jieba提供了三种分词模式：
- 精确模式：试图将句子最精确地切开，适合文本分析；cut_all=False
- 全模式：把句子中所有可以成词的词语都扫描出来, 速度非常快，但是不能解决歧义；cut_all=True
- 搜索引擎模式：在精确模式的基础上，对长词再次切分，提高召回率，适合用于搜索引擎分词。jieba.cut_for_search()

以下代码使用jieba实现中文分词，使用jieba.cut()函数并传入待分词的文本字符串即可，使用cut_all参数控制选择使用全模式还是精确模式，默认为精确模式。如果需要使用搜索引擎模式，使用jieba.cut_for_search()函数即可。运行以下代码之后，jieba首先会加载自带的前缀词典，然后完成相应的分词任务。

```python
import jieba

seg_list = jieba.cut("我来到北京清华大学", cut_all=True)
# join是split的逆操作
# 即使用一个拼接符将一个列表拼成字符串
print("/ ".join(seg_list))  # 全模式

seg_list = jieba.cut("我来到北京清华大学", cut_all=False)
print("/ ".join(seg_list))  # 精确模式

seg_list = jieba.cut("他来到了网易杭研大厦")  # 默认是精确模式
print("/ ".join(seg_list))

seg_list = jieba.cut_for_search("小明硕士毕业于中国科学院计算所，后在日本京都大学深造")  # 搜索引擎模式
print("/ ".join(seg_list))
```

结果：

```python
我/ 来到/ 北京/ 清华/ 清华大学/ 华大/ 大学
我/ 来到/ 北京/ 清华大学
他/ 来到/ 了/ 网易/ 杭研/ 大厦
小明/ 硕士/ 毕业/ 于/ 中国/ 科学/ 学院/ 科学院/ 中国科学院/ 计算/ 计算所/ ，/ 后/ 在/ 日本/ 京都/ 大学/ 日本京都大学/ 深造
```

## 5 jieba增强功能-加载自定义词典

### 5.1 载入新自定义词典

开发者可以指定自己自定义的词典，以便包含 jieba 词库里没有的词。虽然 jieba 有新词识别能力，但是自行添加新词可以保证更高的正确率
- 用法： jieba.load_userdict(file_name) # file_name 为文件类对象或自定义词典的路径
- 词典格式和 dict.txt 一样，一个词占一行；每一行分三部分：词语、词频（可省略）、词性（可省略），用空格隔开，顺序不可颠倒。file_name 若为路径或二进制方式打开的文件，则文件必须为 UTF-8 编码。
- 词频省略时使用自动计算的能保证分出该词的词频。

例如：

```
创新办 3 i
云计算 5
凱特琳 nz
台中
```

用法示例：[https://github.com/fxsjy/jieba/blob/master/test/test_userdict.py](https://github.com/fxsjy/jieba/blob/master/test/test_userdict.py)

```
之前： 李小福 / 是 / 创新 / 办 / 主任 / 也 / 是 / 云 / 计算 / 方面 / 的 / 专家 /

加载自定义词库后：　李小福 / 是 / 创新办 / 主任 / 也 / 是 / 云计算 / 方面 / 的 / 专家 /
```

### 5.2 载入停用词表

主要思想是分词过后，遍历一下停用词表，去掉停用词。

```python
import jieba  
  
  
# jieba.load_userdict('userdict.txt')  
# 创建停用词list  
def stopwordslist(filepath):  
    stopwords = [line.strip() for line in open(filepath, 'r', encoding='utf-8').readlines()]  
    return stopwords  
  
  
# 对句子进行分词  
def seg_sentence(sentence):  
    sentence_seged = jieba.cut(sentence.strip())  
    stopwords = stopwordslist('./test/stopwords.txt')  # 这里加载停用词的路径  
    outstr = ''  
    for word in sentence_seged:  
        if word not in stopwords:  
            if word != '\t':  
                outstr += word  
                outstr += " "  
    return outstr  
  
  
inputs = open('./test/input.txt', 'r', encoding='utf-8')  
outputs = open('./test/output.txt', 'w')  
for line in inputs:  
    line_seg = seg_sentence(line)  # 这里的返回值是字符串  
    outputs.write(line_seg + '\n')  
outputs.close()  
inputs.close()
```

## 6 jieba分词的其他应用

### 6.1 关键词提取

jieba还实现了TF-IDF和TextRank这两种关键词提取算法，直接调用即可。

当然，提取关键词的前提是中文分词，所以这里也会使用到jieba自带的前缀词典和IDF权重词典。

```python
import jieba.analyse

# 字符串前面加u表示使用unicode编码
content = u'中国特色社会主义是我们党领导的伟大事业，全面推进党的建设新的伟大工程，是这一伟大事业取得胜利的关键所在。党坚强有力，事业才能兴旺发达，国家才能繁荣稳定，人民才能幸福安康。党的十八大以来，我们党坚持党要管党、从严治党，凝心聚力、直击积弊、扶正祛邪，党的建设开创新局面，党风政风呈现新气象。习近平总书记围绕从严管党治党提出一系列新的重要思想，为全面推进党的建设新的伟大工程进一步指明了方向。'

# 第一个参数：待提取关键词的文本
# 第二个参数：返回关键词的数量，重要性从高到低排序
# 第三个参数：是否同时返回每个关键词的权重
# 第四个参数：词性过滤，为空表示不过滤，若提供则仅返回符合词性要求的关键词
keywords = jieba.analyse.extract_tags(content, topK=20, withWeight=True, allowPOS=())
# 访问提取结果
for item in keywords:
    # 分别为关键词和相应的权重
    print item[0], item[1]

# 同样是四个参数，但allowPOS默认为('ns', 'n', 'vn', 'v')
# 即仅提取地名、名词、动名词、动词
keywords = jieba.analyse.textrank(content, topK=20, withWeight=True, allowPOS=('ns', 'n', 'vn', 'v'))
# 访问提取结果
for item in keywords:
    # 分别为关键词和相应的权重
    print item[0], item[1]
```

结果展示：

```
才能 1.0
管党 0.7999933934163805
全面 0.7325692441985737
社会主义 0.6327916888315029
围绕 0.60594603358887
总书记 0.5945625023471114
凝心 0.5840883789052874
政风 0.5792034335473362
新气象 0.5772168490112909
党风 0.5728262292165519
呈现 0.5700456186486299
推进 0.5548361394986431
方向 0.5150324602730256
指明 0.5113586590717408
治党 0.5062232626208965
局面 0.4744549207999055
聚力 0.46596165707522896
积弊 0.4646149902996275
直击 0.46314922535402286
国家 0.46179235227324805
```

### 6.2 词性标注

jieba在进行中文分词的同时，还可以完成词性标注任务。根据分词结果中每个词的词性，可以初步实现命名实体识别，即将标注为nr的词视为人名，将标注为ns的词视为地名等。所有标点符号都会被标注为x，所以可以根据这个去除分词结果中的标点符号。

```
# 加载jieba.posseg并取个别名，方便调用
import jieba.posseg as pseg
words = pseg.cut("我爱北京天安门")
for word, flag in words:
    # 格式化模版并传入参数
    print('%s, %s' % (word, flag))
```

结果展示：

```
我, r
爱, v
北京, ns
天安门, ns
```

参考视频：[全栈数据工程师养成攻略-18-结巴分词](http://study.163.com/course/courseLearn.htm?courseId=1003520028#/learn/video?lessonId=1004015740&courseId=1003520028)

## 7 用jieba分词实战（含文件的读取与存储）

```python
# -*- coding: utf-8 -*-
#本程序主要用于jieba分词，以及去除停用词

import os
import jieba


# 保存文件的函数
def savefile(savepath,content):
    fp = open(savepath,'w',encoding='utf8',errors='ignore')
    fp.write(content)
    fp.close()

# 读取文件的函数
def readfile(path):
    fp = open(path, "r", encoding='utf8', errors='ignore')
    content = fp.read()
    fp.close()
    return content

## 去除停用词的2个函数
# 创建停用词list
def stopwordslist(filepath):
    stopwords = [line.strip() for line in open(filepath, 'r', encoding='utf-8').readlines()]
    return stopwords

# 对句子去除停用词
def movestopwords(sentence):
    stopwords = stopwordslist('语料/hlt_stop_words.txt')  # 这里加载停用词的路径
    outstr = ''
    for word in sentence:
        if word not in stopwords:
            if word != '\t'and'\n':
                outstr += word
                # outstr += " "
    return outstr


corpus_path = "语料/train/"  # 未分词分类预料库路径
seg_path = "语料/train_seg/"  # 分词后分类语料库路径

catelist = os.listdir(corpus_path)  # 获取未分词目录下所有子目录
for mydir in catelist:
    class_path = corpus_path + mydir + "/"  # 拼出分类子目录的路径
    seg_dir = seg_path + mydir + "/"  # 拼出分词后预料分类目录
    if not os.path.exists(seg_dir):  # 是否存在，不存在则创建
        os.makedirs(seg_dir)

    file_list = os.listdir(class_path) # 列举当前目录所有文件
    for file_path in file_list:
        fullname = class_path + file_path # 路径+文件名
        print("当前处理的文件是： ",fullname)  # 语料/train/pos/pos1.txt
                        #  语料/train/neg/neg1.txt

        content = readfile(fullname).strip()  # 读取文件内容
        content = content.replace("\n", "").strip()  # 删除换行和多余的空格
        content_seg = jieba.cut(content)    # jieba分词
        print("jieba分词后：",content_seg)
        listcontent = ''
        for i in content_seg:
            listcontent += i
            listcontent += " "
        print(listcontent[0:10])
        listcontent = movestopwords(listcontent)    # 去除停用词
        print("去除停用词后：", listcontent[0:10])
        listcontent = listcontent.replace("   ", " ").replace("  ", " ")
        savefile(seg_dir + file_path, "".join(listcontent)) # 保存
```

结果展示：

```
分词前：和秦一对比就是弱爆了的技术

分词后： 和 秦一 对比 就是 弱 爆 了 的 技术 ， 都 有点

去除停用词后：秦 弱 爆 技术 都 点 不好意思 出 口碑
```



