# 怎么加载训练好的词向量bin - 博客堂 - CSDN博客





2018年01月05日 15:28:55[最小森林](https://me.csdn.net/u012052268)阅读数：5502








# 怎么加载训练好的词向量bin

word2vec 有的时候需要加载之际之前训练好的词向量，或者加载下载的别人的词向量，加载如下：

## 1默认方法

如果你是用默认方法训练的，则可以直接load：

```
model.save('/model/word2vec_model')

new_model=gensim.models.Word2Vec.load('/model/word2vec_model')
```

也可以获取每个词的词向量

```bash
model['computer']
```

## 2下载的二进制bin类型

如果及自己保存的二进制类型，则需要这样：

```python
'''
Created on 2018年1月5日

@author: qingtxu
'''
import gensim
# 导入模型  
model = gensim.models.KeyedVectors.load_word2vec_format('GoogleNews-vectors-negative300.bin',binary=True) 

# 返回一个词 的向量：
print(model['word'])

# 返回和一个词语最相关的多个词语以及对应的相关度
items = model.most_similar('happy')
for item in items:
    # 词的内容，词的相关度
    print(item[0], item[1])
```

## 3 词向量的下载地址：

### 3.1 英文
- 谷歌新闻词向量，1.2G，地址：[https://code.google.com/archive/p/word2vec/](https://code.google.com/archive/p/word2vec/)   下载文件：[GoogleNews-vectors-negative300.bin.gz](https://drive.google.com/file/d/0B7XkCwpI5KDYNlNUTTlSS21pQmM/edit?usp=sharing).

### 3.2中文向量

中文还没找到好，下次把我自己训练的维基百科上传



