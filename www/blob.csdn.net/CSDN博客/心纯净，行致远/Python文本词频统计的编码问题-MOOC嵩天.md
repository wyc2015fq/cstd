# Python文本词频统计的编码问题-MOOC嵩天 - 心纯净，行致远 - CSDN博客





2018年09月25日 09:54:52[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：466








# 1 Python文本词频统计代码

## 1.1Hamlet词频统计（含Hamlet原文文本）

```
#CalHamletV1.py
def getText():
    txt = open("hamlet.txt", "r").read()
    txt = txt.lower()
    for ch in '!"#$%&()*+,-./:;<=>?@[\\]^_‘{|}~':
        txt = txt.replace(ch, " ")   #将文本中特殊字符替换为空格
    return txt
 
hamletTxt = getText()
words  = hamletTxt.split()
counts = {}
for word in words:           
    counts[word] = counts.get(word,0) + 1
items = list(counts.items())
items.sort(key=lambda x:x[1], reverse=True) 
for i in range(10):
    word, count = items[i]
    print ("{0:<10}{1:>5}".format(word, count))
```

## 1.2《三国演义》人物出场统计（上）（含《三国演义》原文文本）

```
#CalThreeKingdomsV1.py
import jieba
txt = open("threekingdoms.txt", "r", encoding='utf-8').read()
words  = jieba.lcut(txt)
counts = {}
for word in words:
    if len(word) == 1:
        continue
    else:
        counts[word] = counts.get(word,0) + 1
items = list(counts.items())
items.sort(key=lambda x:x[1], reverse=True) 
for i in range(15):
    word, count = items[i]
    print ("{0:<10}{1:>5}".format(word, count))
```

## 1.3《三国演义》人物出场统计（下）（含《三国演义》原文文本）

```
#CalThreeKingdomsV2.py
import jieba
excludes = {"将军","却说","荆州","二人","不可","不能","如此"}
txt = open("threekingdoms.txt", "r", encoding='utf-8').read()
words  = jieba.lcut(txt)
counts = {}
for word in words:
    if len(word) == 1:
        continue
    elif word == "诸葛亮" or word == "孔明曰":
        rword = "孔明"
    elif word == "关公" or word == "云长":
        rword = "关羽"
    elif word == "玄德" or word == "玄德曰":
        rword = "刘备"
    elif word == "孟德" or word == "丞相":
        rword = "曹操"
    else:
        rword = word
    counts[rword] = counts.get(rword,0) + 1
for word in excludes:
    del counts[word]
items = list(counts.items())
items.sort(key=lambda x:x[1], reverse=True) 
for i in range(10):
    word, count = items[i]
    print ("{0:<10}{1:>5}".format(word, count))
```

## 1.4 文本词频统计的文本

该资源是《Python文本词频统计的编码问题-MOOC嵩天》的文本词频统计的文本。包内包含三国演义中文版TXT和哈姆雷特英文版TXT。

资源地址：[文本词频统计的文本](https://download.csdn.net/download/zhanshen112/10685301)

# 2 文本词频统计的编码问题

## 2.1 文本词频编码对应代码

将文本词频统计的文本和代码放于同一文件夹下，运行上述代码，会出现以下报错：

```
UnicodeDecodeError: 'gbk' codec can't decode byte 0x80 in position 14: illegal multibyte sequence
```

```
UnicodeDecodeError: 'utf-8' codec can't decode byte 0xb5 in position 3: invalid start byte
```

根据提示，是TXT文本编码的格式问题。因此只需要将代码中识别编码格式的代码进行更改即可。Hamlet以及ThreeKingdom的识别编码的代码行如下：

Hamlet

```
txt = open("hamlet.txt", "r").read()
```

ThreeKingdomV1

```
txt = open("threekingdoms.txt", "r", encoding='utf-8').read()
```

ThreeKingdomV2

```
txt = open("threekingdoms.txt", "r", encoding='utf-8').read()
```

## 2.2 查看TXT编码并更改代码

查看TXT编码只需要打开TXT并另存为，就会出现现在文档的编码格式。编码格式也可以更改。关键是要保持文本的编码格式和代码读取的编码格式相同。

由于我上传的TXT编码格式都是utf-8编码，因此相应代码只需要更改为以下代码即可成功运行。

Hamlet
```
txt = open("hamlet.txt", "r", encoding='utf-8').read()
```

ThreeKingdomV1

```
txt = open("threekingdoms.txt", "r", encoding='utf-8').read()
```

ThreeKingdomV2

```
txt = open("threekingdoms.txt", "r", encoding='utf-8').read()
```




