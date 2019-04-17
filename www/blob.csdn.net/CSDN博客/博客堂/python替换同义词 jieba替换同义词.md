# python替换同义词 jieba替换同义词 - 博客堂 - CSDN博客





2019年03月26日 09:55:33[最小森林](https://me.csdn.net/u012052268)阅读数：140标签：[jieba																[同义词																[python																[同义词替换](https://so.csdn.net/so/search/s.do?q=同义词替换&t=blog)
个人分类：[自然语言处理																[Python](https://blog.csdn.net/u012052268/article/category/6625160)](https://blog.csdn.net/u012052268/article/category/6830884)





# python替换同义词 jieba替换同义词




### 文章目录
- [python替换同义词 jieba替换同义词](#python_jieba_0)
- [0 描述：](#0__3)
- [1构建同义词文件txt](#1txt_9)
- [2代码](#2_19)




## 0 描述：

在构建基于知识图谱的问答系统过程中，我们发现 知识图谱对于实体的识别是非常敏感的， 如果用户输入的关键词 不对（哪怕只差一个词） 知识图谱就找不到对应的三元组。

所以我们打算现将用户的输入做一个预处理 ， 将一些同义词 手动 转化为 知识图谱的中标准输入。

## 1构建同义词文件txt

构建同义词文件tongyici_tihuan.txt，每一个同义词列为一行，每行第一个为希望分隔后呈现的词语，后几个为第一个词的同义词，用tab键分隔，比如：

```
年休假	年假	年休
北京	北平	首都
```

在这个例子里“北平	首都”都会被替换为 “北京”

## 2代码

```python
# encoding=utf-8
import jieba
def tihuan_tongyici(string1):
    # tongyici_tihuan.txt是同义词表，每行是一系列同义词，用tab分割
    # 1读取同义词表：并生成一个字典。
    combine_dict = {}
    for line in open("tongyici_tihuan.txt", "r"):
        seperate_word = line.strip().split("\t")
        num = len(seperate_word)
        for i in range(1, num):
            combine_dict[seperate_word[i]] = seperate_word[0]

    # 2提升某些词的词频，使其能够被jieba识别出来
    jieba.suggest_freq("年假", tune = True)

    # 3将语句切分
    seg_list = jieba.cut(string1, cut_all = False)
    f = "/".join(seg_list).encode("utf-8")  # 不用utf-8编码的话，就不能和tongyici文件里的词对应上
    # print f

    # 4
    final_sentence = ""
    for word in f.split("/"):
        if word in combine_dict:
            word = combine_dict[word]
            final_sentence += word
        else:
            final_sentence += word
    # print final_sentence
    return final_sentence



string1 = '年假到底放几天？'
print tihuan_tongyici(string1)
```](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=同义词&t=blog)](https://so.csdn.net/so/search/s.do?q=jieba&t=blog)




