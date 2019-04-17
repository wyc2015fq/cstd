# Python 繁体中文与简体中文相互转换 - 博客堂 - CSDN博客





2017年09月03日 17:30:00[最小森林](https://me.csdn.net/u012052268)阅读数：5902








# Python 繁体中文与简体中文相互转换

[toc] 

工作中需要将繁体中文转换成简体中文  

上网找了些资料，发现这个包最方便
## 1 安装

不需要什么安装方法，只需要把这两个文件下载下来，保存到与代码同一目录下即可

> 
[https://raw.githubusercontent.com/skydark/nstools/master/zhtools/langconv.py](https://raw.githubusercontent.com/skydark/nstools/master/zhtools/langconv.py)
[https://raw.githubusercontent.com/skydark/nstools/master/zhtools/zh_wiki.py](https://raw.githubusercontent.com/skydark/nstools/master/zhtools/zh_wiki.py)


打包下载地址：[http://download.csdn.net/download/u012052268/9996650](http://download.csdn.net/download/u012052268/9996650)

## 2 使用方法

### 2.1 繁体转简体

```python
from langconv import *

def Traditional2Simplified(sentence):
    '''
    将sentence中的繁体字转为简体字
    :param sentence: 待转换的句子
    :return: 将句子中繁体字转换为简体字之后的句子
    '''
    sentence = Converter('zh-hans').convert(sentence)
    return sentence

if __name__=="__main__":
    traditional_sentence = '憂郁的臺灣烏龜'
    simplified_sentence = Traditional2Simplified(traditional_sentence)
    print(simplified_sentence)

    '''
    输出结果：
        忧郁的台湾乌龟
    '''
```

### 2.2 简体转繁体

```python
from langconv import *

from langconv import *

def Simplified2Traditional(sentence):
    '''
    将sentence中的简体字转为繁体字
    :param sentence: 待转换的句子
    :return: 将句子中简体字转换为繁体字之后的句子
    '''
    sentence = Converter('zh-hant').convert(sentence)
    return sentence

if __name__=="__main__":
    simplified_sentence = '忧郁的台湾乌龟'
    traditional_sentence = Simplified2Traditional(simplified_sentence)
    print(traditional_sentence)

    '''
    输出结果：
        憂郁的臺灣烏龜
    '''
```

## 3实例：

第一步打开文本文件，for读取每句繁体

```
with open('question_labels.json', 'r',encoding='UTF-8') as f:
    question_labels = json.load(f)

q_zh = []   #   Data中问题的中文
for line in question_labels:
    q_zh.append(line['q_zh'])

print(q_zh)
# ['請問京都議定書規定幾個工業國家的二氧化碳排放量限制？', '請問首位自費太空旅行的觀光客為誰？',
```

第二步转换

```python
def Traditional2Simplified(sentence):
    sentence = langconv.Converter('zh-hans').convert(sentence)
    return sentence

q_zh_jian = []
for line in q_zh:
    q_zh_jian.append(Traditional2Simplified(line))

print(q_zh_jian)
# ['请问京都议定书规定几个工业国家的二氧化碳排放量限制？', '请问首位自费太空旅行的观光客为谁？', '请问
```




