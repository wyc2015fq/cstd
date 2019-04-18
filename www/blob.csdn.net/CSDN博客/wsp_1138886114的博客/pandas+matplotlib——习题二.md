# pandas+matplotlib——习题二 - wsp_1138886114的博客 - CSDN博客





2018年07月19日 17:59:13[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：311










- - - [作业一](#作业一)- - [1.1 读取nlp文件夹下的labeledTraniData.tsv文件](#11-读取nlp文件夹下的labeledtranidatatsv文件)
- [1.2 去掉html标签—切分成词/token—重组为新的句子](#12-去掉html标签切分成词token重组为新的句子)
- [1.3 将数据中的标点符号去掉（正则）](#13-将数据中的标点符号去掉正则)
- [1.4 文字转成小写–去掉停用词](#14-文字转成小写去掉停用词)
- [1.5 定义函数实现（1.2-1.4）的文本处理](#15-定义函数实现12-14的文本处理)
- [1.6 用apply方法，使用上步的函数重新处理加载数据](#16-用apply方法使用上步的函数重新处理加载数据)









`运行环境 jupyter notebook`


```python
import  matplotlib.pyplot as plt 
from pandas import DataFrame,Series
import pandas as pd
import numpy as np  

plt.rcParams["font.sans-serif"]=['SimHei']  # 用于正常显示中文标签
plt.rcParams['axes.unicode_minus']=False    # 用来正常显示负号
```

### 作业一

##### 1.1 读取nlp文件夹下的labeledTraniData.tsv文件

```
df = pd.read_csv("nlp/labeledTrainData.tsv", sep='\t', escapechar='\\')
print('记录数: {}'.format(len(df)))
df.head()

输出：
记录数: 25000
```
|$~$|**id**|**sentiment**|**review**|
|----|----|----|----|
|**0**|5814_8|1|With all this stuff going down at the   moment w…|
|**1**|2381_9|1|“The Classic War of the Worlds”   by Timothy Hin…|
|**2**|7759_3|0|The film starts with a manager (Nicholas   Bell)…|
|**3**|3630_4|0|It must be assumed that those who praised   this…|
|**4**|9495_8|1|Superbly trashy and wondrously   unpretentious 8…|

##### 1.2 去掉html标签—切分成词/token—重组为新的句子

```python
1. #提取出原始数据中的第一行review列中的文本数据，并用display函数显示 
display(df["review"],"原始数据")
输出：
0        With all this stuff going down at the moment w...
1        "The Classic War of the Worlds" by Timothy Hin...
2        The film starts with a manager (Nicholas Bell)...
3        It must be assumed that those who praised this...
4        A friend of mine bought this film for £1, and ...
5        <br /><br />This movie is full of references. ...
--------------------------------------------
display(df["review"][1],"原始数据") 
输出：
"The Classic War of the Worlds" by Timothy Hines is a very entertaining film that  
obviously goes to great effort and lengths to faithfully recreate H. G. Wells' classic book.  
Mr. Hines succeeds in doing so...
```

```python
2. #用BeautifulSoup将第四步中获取到的数据中的html标签去除 
df_01 = df["review"][1]
df_02 = BeautifulSoup(df_01,"lxml")
[s.extract() for s in df_02('script')]
df_03 = df_02.get_text()
display(df_03, "去掉HTML标签的数据") 
输出：
"The Classic War of the Worlds" by Timothy Hines is a very entertaining film that obviously  
goes to great effort and lengths to faithfully recreate H. G. Wells' classic book.  
Mr. Hines succeeds in doing so.
```

##### 1.3 将数据中的标点符号去掉（正则）

```
df_04 = df_03.replace(",", "").replace(".", "").replace('"', '').replace('\'', '')
df_04 
输出：
'The Classic War of the Worlds by Timothy Hines is a very entertaining film that 
obviously goes to great effort and lengths to faithfully recreate H G Wells 
classic book Mr Hines succeeds in doing so I and those who watched his film with 
me appreciated the fact that it was not the standard predictable Hollywood...'
```

##### 1.4 文字转成小写–去掉停用词

```
#文字转成小写
str_02 = df_04.lower().split(' ')
str_03 = list(str_02)
display(str_03, "纯词列表数据") 

输出：
纯词列表数据
['the', 'classic', 'war', 'of', 'the', 'worlds', 'by', 'timothy', 'hines', 'is', 'a', 'very',  
'entertaining', 'film', 'that', 'obviously', 'goes', 'to', 'great', 'effort', 'and', 'lengths',  
'to', 'faithfully', 'recreate', 'h', 'g', 'wells', 'classic', 'book',...]
```

```python
#去掉上步数据中的英文停用词
"""
    first = [1,2,3,4,5,6]
    second = {}.fromkeys([4,5])
    [w for w in first if w not in second] 
"""
#加载英文停用词
stopwords = {}.fromkeys([line.rstrip() for line in open('nlp/stopwords.txt')]) 

#用加载的英文停用词，去除第七部数据中的英文停用词
words_nostop = [w for w in str_03 if w not in stopwords]
display(words_nostop, '去掉停用词数据') 

#为确保所加载的英文停用词没有重复数据 set()去重
eng_stopwords = set(stopwords)
```

##### 1.5 定义函数实现（1.2-1.4）的文本处理

```python
def clean_text(text):
    text = BeautifulSoup(text, 'html.parser').get_text() #去除网页标签
    text = re.sub(r'[^a-zA-Z]', ' ', text)               #去除文本中的特殊字符：‘’ ." 、' 
    words = text.lower().split()                         #文字转成小写词
    words = [w for w in words if w not in eng_stopwords] #去除停用词
    return ' '.join(words)                               #词语用空格分开
```

##### 1.6 用apply方法，使用上步的函数重新处理加载数据

```python
df['clean_review'] = df.review.apply(clean_text)
df.head()
```
|$~$|**id**|**sentiment**|**review**|**clean_review**|
|----|----|----|----|----|
|**0**|5814_8|1|With all this stuff going down at the   moment w…|stuff moment mj ve started listening music   wat…|
|**1**|2381_9|1|“The Classic War of the Worlds”   by Timothy Hin…|classic war worlds timothy hines   entertaining …|
|**2**|7759_3|0|The film starts with a manager (Nicholas   Bell)…|film starts manager nicholas bell investors   ro…|
|**3**|3630_4|0|It must be assumed that those who praised   this…|assumed praised film filmed opera didn read   do…|
|**4**|9495_8|1|Superbly trashy and wondrously   unpretentious 8…|superbly trashy wondrously unpretentious   explo…|



