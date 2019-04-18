# 三十四、基于python的高效中文文本切词 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:54:30[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：640












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_138_default_big.jpeg)



对中文文本做自然语言处理一定设计到切词的操作，流行的切词工具有很多，本文介绍基于python的结巴中文分词，它是基于文本分析的，不是基于词库的，因此更精确

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装jieba分词工具

在[https://pypi.python.org/pypi/jieba/](https://pypi.python.org/pypi/jieba/)下载jieba-0.38.zip

解压后执行：
`python setup.py install`


## 试验切词效果



创建testjieba.py文件内容如下：

```python
# coding:utf-8
#!/usr/local/bin/python

import jieba

seg_list = jieba.cut("www.shareditor.com全栈工程师教程、讲述程序猿自己的故事，shareditor涉及机器学习、深度学习、大数据相关知识的系统学习方法及热文推荐")
for str in seg_list:
    print str
```



输出分词结果如下：

```
www
.
shareditor
.
com
全栈
工程师
教程
、
讲述
程序
猿
自己
的
故事
，
shareditor
涉及
机器
学习
、
深度
学习
、
大
数据
相关
知识
的
系统
学习
方法
及
热文
推荐
```



jieba.cut返回的是python的generator结构，遍历后得到分词结果中包括：中文词汇（单字、多字）、英文单词、标点符号

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 对批量文档分词



我们之前抓取的微信公众号文章都保存到了mysql的page表里，并且提取纯文本内容保存在了content列中，所以我们从mysql加载content并做切词，之后再利用scikit-learn计算tf-idf值

创建cut_and_cal_tfidf.py，内容如下：

```python
# coding:utf-8

from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
import jieba
import MySQLdb

conn = MySQLdb.connect(host="127.0.0.1",user="myuser",passwd="mypasswd",db="mydb",charset="utf8")
cursor = conn.cursor()

sql = "select content from page"
cursor.execute(sql)
corpus=[]
for content in cursor.fetchall():
    seg_list = jieba.cut(content[0])
    line = ""
    for str in seg_list:
        line = line + " " + str
    corpus.append(line)
conn.commit()
conn.close()

vectorizer=CountVectorizer()
csr_mat = vectorizer.fit_transform(corpus)
transformer=TfidfTransformer()
tfidf=transformer.fit_transform(csr_mat)
word=vectorizer.get_feature_names()
print tfidf.toarray()
```

执行后输出结果如下：

```
Building prefix dict from the default dictionary ...
Loading model from cache /var/folders/tq/c0kp5y857x138x5vf8bzxfc80000gp/T/jieba.cache
Loading model cost 0.447 seconds.
Prefix dict has been built succesfully.
[[ 0.          0.00567306  0.         ...,  0.          0.          0.        ]
 [ 0.          0.          0.         ...,  0.          0.          0.        ]
 [ 0.          0.          0.         ...,  0.          0.          0.        ]
 ...,
 [ 0.          0.          0.         ...,  0.          0.          0.        ]
 [ 0.          0.          0.         ...,  0.          0.          0.        ]
 [ 0.          0.          0.         ...,  0.          0.          0.        ]]
```

## 讲解一下

先说下代码逻辑：首先加载数据库的content数据，如果对于page数据库表不熟请回过头来看《[教你成为全栈工程师(Full Stack Developer) 三十二-scrapy爬虫抓取网页写入mysql数据库](http://www.shareditor.com/blogshow/45)》，然后用jieba.cut对每一条content做分词，把分词按空格分隔存到二维数组corpus中，最后用scikit-learn计算tfidf，如果对这部分不熟请回过头来看《[教你成为全栈工程师(Full
 Stack Developer) 三十三-利用scikit-learn计算tf-idf做文本词频分析](http://www.shareditor.com/blogshow/46)》



再说下输出的内容，前面几行文字是jieba分词器自动打印的加载词典的信息，下面是整个tf-idf矩阵，因为维度太高，scikit-learn库自动帮我们用...做了省略




