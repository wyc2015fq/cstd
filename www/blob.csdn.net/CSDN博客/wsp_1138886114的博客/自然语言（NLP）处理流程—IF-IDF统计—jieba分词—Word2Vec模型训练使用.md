# 自然语言（NLP）处理流程—IF-IDF统计—jieba分词—Word2Vec模型训练使用 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:54:17[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：328











### 文章目录
- [一、数据感知—训练与测试数据](#_3)
- [二：文档分词（精确—全—搜索引擎模式）—词性标注](#_55)
- [2.1  三种不同分词方式](#21___56)
- [2.2  词性标注](#22___85)
- [三、用 TF-IDF 和词袋表示文档特征](#_TFIDF__108)
- [3.1 方法一：使用 CounterVectorizer 和 TfidfTransformer 计算 TF-IDF](#31__CounterVectorizer__TfidfTransformer__TFIDF_117)
- [3.2 方法二：直接使用 TfidfVectorizer](#32__TfidfVectorizer_159)
- [四、jieba 分词](#jieba__200)
- [五、训练word2vec模型](#word2vec_219)
- [5.1 word2vec模型的使用](#51_word2vec_245)



 开发环境 jupyter notebook

### 一、数据感知—训练与测试数据

```python
```python
import numpy as np
import pandas as pd

# 创建输出目录
output_dir = u'output_word2vec'    
import os
if not os.path.exists(output_dir):
    os.mkdir(output_dir) 

# 查看训练数据
train_data = pd.read_csv('data/sohu_train.txt', sep='\t', header=None, 
                         dtype=np.str_, encoding='utf8', names=[u'频道', u'文章'])
train_data.head()

# 查看每个频道下文章数量
train_data.groupby(u'频道')[u'频道'].count() 

# 查看每个频道下最短、最长文章字数
train_data[u'文章长度'] = train_data[u'文章'].apply(len)
train_data.groupby(u'频道')[u'文章长度'].agg([np.min, np.max])
```
```
|$~~~~$|频$~~$道|文章|
|----|----|----|
|0|娱乐|《青蛇》造型师默认新《红楼梦》额妆抄袭（图） 凡是看过电影《青蛇》的人，都不会忘记青白二蛇的…|
|1|娱乐|６．１６日剧榜　＜最后的朋友＞　亮最后杀招成功登顶 《最后的朋友》本周的电视剧排行榜单依然只…|
|2|娱乐|超乎想象的好看《纳尼亚传奇２：凯斯宾王子》 现时资讯如此发达，搜狐电影评审团几乎人人在没有看…|
|3|娱乐|吴宇森：赤壁大战不会出现在上集 “希望《赤壁》能给你们不一样的感觉。”对于自己刚刚拍完的影片…|
|4|娱乐|组图：《多情女人痴情男》陈浩民现场耍宝 陈浩民：外面的朋友大家好，现在是搜狐现场直播，欢迎《…|

```python
```python
# 查看测试数据
test_data = pd.read_csv('data/sohu_test.txt', sep='\t', header=None, 
                        dtype=np.str_, encoding='utf8', names=[u'频道', u'文章'])
test_data.head() 

# 查看每个频道下文章数量
test_data.groupby(u'频道')[u'频道'].count()

# 查看每个频道下最短、最长文章字数
test_data[u'文章长度'] = train_data[u'文章'].apply(len)
test_data.groupby(u'频道')[u'文章长度'].agg([np.min, np.max])
```
```

```python
```python
# 载入停用词
stopwords = set()
with open('data/stopwords.txt', 'rb') as infile:
    for line in infile:
        line = line.decode('utf8').rstrip('\n')
        if line:
            stopwords.add(line.lower())
```
```

### 二：文档分词（精确—全—搜索引擎模式）—词性标注

##### 2.1  三种不同分词方式

```python
```python
import jieba                 # 载入jieba分词包

text = u'小明硕士毕业于中国科学院计算所，后在日本京都大学深造'

"""
精确模式：   试图将句子最精确的分开，适合文本分析
            jieba.lcut，结果返回一个列表
            jieba.cut，结果返回一个迭代器 

全模式：     把句子中所有可以成词的词语都扫描出来，速度非常快，但是不能解决歧义 

搜索引擎模式：在精确模式的基础上对长词再次切分，适合用于搜索引擎分词
"""

print (u'精确模式分词: ' + u'/'.join(jieba.lcut(text))) 
print (u'全模式分词: ' + u'/'.join(jieba.lcut(text, cut_all=True)))
print (u'搜索引擎模式: ' + u'/'.join(jieba.lcut_for_search(text)))
--------------------------------------------------------------------
依次输出：
精确模式分词: 小明/硕士/毕业/于/中国科学院/计算所/，/后/在/日本京都大学/深造 

全模式分词:  小/明/硕士/毕业/于/中国/中国科学院/科学/科学院/学院/计算/计算所///
           后/在/日本/日本京都大学/京都/京都大学/大学/深造 

搜索引擎模式: 小明/硕士/毕业/于/中国/科学/学院/科学院/中国科学院/计算/计算所/，
            /后/在/日本/京都/大学/日本京都大学/深造
```
```

##### 2.2  词性标注

```python
```python
from jieba import posseg as psg          # 载入词性标注模块

text = u'小明硕士毕业于中国科学院计算所，后在日本京都大学深造'
"""
	jieba.posseg.lcut 进行词性标注
	结果为 jieba.posseg.pair 类型的列表
	每个pair对象中，word属性表示词语，flag表示词性
	词性符合的解释可见：https://gist.github.com/luw2007/6016931
"""

res = psg.lcut(text)
print ('repr: ' + repr(res[0]))
print ('词: {}, 词性: {}'.format(res[0].word, res[0].flag))
print ('词性标注: ' + ' '.join(['{}/{}'.format(x.word, x.flag) for x in res]))
---------------------------------------------------------------------------------
输出：
repr:  pair('小明', 'nr')
词: 小明, 词性: nr
词性标注: 小明/nr 硕士/n 毕业/n 于/p 中国科学院/nt 计算所/n ，/x 后在/t 日本京都大学/nt 深造/v
```
```

### 三、用 TF-IDF 和词袋表示文档特征

```python
```python
import jieba
import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer,TfidfTransformer, TfidfVectorizer 

contents = [u'小明喜欢看电影，小红也喜欢看电影。',u'小明还喜欢看足球比赛。']
stopwords = {u'，', u'。'}     # 定义【，。】为停用词
```
```

##### 3.1 方法一：使用 CounterVectorizer 和 TfidfTransformer 计算 TF-IDF

```python
```python
"""
	计算TF（每个词的出现次数，未归一）
	tokenizer: 定义一个函数，接受文本，返回分词的list
	stop_words: 定义停用词词典，会在结果中删除词典中包含的词
"""

tf = CountVectorizer(tokenizer=jieba.lcut, stop_words=stopwords)
res1 = tf.fit_transform(contents)        # 使用函数拟合转置contents
tf.vocabulary_                           # 查看词汇对应关系 
 
pd.DataFrame(res1.toarray(),             # 查看TF结果
             columns=[x[0] for x in sorted(tf.vocabulary_.items(),
                                           key=lambda x: x[1])]) 
# 查看TF结果如下表：
```
```
|$~~$|也|喜欢|小明|小红|电影|看|足球比赛|还|
|----|----|----|----|----|----|----|----|----|
|0|1|2|1|1|2|2|0|0|
|1|0|1|1|0|0|1|1|1|

```python
```python
"""
	use_idf:      表示在TF矩阵的基础上计算IDF，并相乘得到TF-IDF
	smooth_idf:   表示计算IDF时，分子上的总文档数+1
	sublinear_tf: 表示使用 1+log(tf)替换原来的tf
	norm:         表示对TF-IDF矩阵的每一行使用l2范数归一化
"""

tfidf = TfidfTransformer(norm='l2', use_idf=True, smooth_idf=True, sublinear_tf=False)
res2 = tfidf.fit_transform(res1) 

tfidf.idf_                                # 查看每个词的IDF
pd.DataFrame(res2.toarray(), 
             columns=[x[0] for x in sorted(tf.vocabulary_.items(), 
                                           key=lambda x: x[1])]) 
# 查看TF-IDF结果如下表：
```
```
|$~$|也|喜欢|小明|小红|电影|看|足球比赛|还|
|----|----|----|----|----|----|----|----|----|
|**0**|0.307784|0.437982|0.218991|0.307784|0.615568|0.437982|0.000000|0.000000|
|**1**|0.000000|0.379303|0.379303|0.000000|0.000000|0.379303|0.533098|0.533098|

##### 3.2 方法二：直接使用 TfidfVectorizer

```python
```python
# 参数为 CounterVectorizer 和 TfidfTransformer 的所有参数
tfidf = TfidfVectorizer(tokenizer=jieba.lcut,
                        stop_words=stopwords, 
                        norm='l2', 
                        use_idf=True, 
                        smooth_idf=True, 
                        sublinear_tf=False)
res = tfidf.fit_transform(contents) 


tfidf.idf_                 # 查看每个词的IDF，顺序和 tfidf.vocabulary_ 对应（ndarray类型）
tfidf.vocabulary_          # 查看每一列所代表的词（字典）
输出：{'也': 0, '喜欢': 1, '小明': 2, '小红': 3, '电影': 4, 
       '看': 5, '足球比赛': 6, '还': 7} 

pd.DataFrame({'词': [x[0] for x in sorted(tfidf.vocabulary_.items(),key=lambda x: x[1])],
              'IDF': tfidf.idf_}, columns=['词', 'IDF']) 
输出：
|      | 词       | IDF      |
| ---- | -------- | -------- |
| 0    | 也       | 1.405465 |
| 1    | 喜欢     | 1.000000 |
| 2    | 小明     | 1.000000 |
| 3    | 小红     | 1.405465 |
| 4    | 电影     | 1.405465 |
| 5    | 看       | 1.000000 |
| 6    | 足球比赛  | 1.405465 |
| 7    | 还       | 1.405465 | 

pd.DataFrame(res.toarray(),
             columns=[x[0] for x in sorted(tfidf.vocabulary_.items(), 
                                           key=lambda x: x[1])])
```
```
|$~~~$|也|喜欢|小明|小红|电影|看|足球比赛|还|
|----|----|----|----|----|----|----|----|----|
|**0**|0.307784|0.437982|0.218991|0.307784|0.615568|0.437982|0.000000|0.000000|
|**1**|0.000000|0.379303|0.379303|0.000000|0.000000|0.379303|0.533098|0.533098|

### 四、jieba 分词

```python
```python
import jieba
article_words = []

for article in train_data[u'文章']:         # 遍历每篇文章
    curr_words = []
    for word in jieba.cut(article):         # 遍历文章中的每个词并分词
        if word not in stopwords:           # 去除停用词
            curr_words.append(word)
    article_words.append(curr_words) 

# 分词结果存储到seg_word_file.txt文件
seg_word_file = os.path.join(output_dir, 'seg_words.txt')
with open(seg_word_file, 'wb') as outfile:
    for words in article_words:
        outfile.write(u' '.join(words).encode('utf8') + '\n')
print (u'分词结果保存到文件：{}'.format(seg_word_file))
```
```

### 五、训练word2vec模型

```python
```python
from gensim.models import Word2Vec
from gensim.models.word2vec import LineSentence 

# 创建一个句子迭代器，一行为一个句子，词和词之间用空格分开
# 这里我们把一篇文章当作一个句子
sentences = LineSentence(seg_word_file) 
"""
	训练word2vec模型 参数说明：
		sentences: 包含句子的list，或迭代器
		size:      词向量的维数，size越大需要越多的训练数据，同时能得到更好的模型
		alpha:     初始学习速率，随着训练过程递减，最后降到 min_alpha
		window:    上下文窗口大小，即预测当前这个词的时候最多使用距离为window大小的词
		max_vocab_size: 词表大小，如果实际词的数量超过了这个值，过滤那些频率低的
		workers:   并行度
		iter:      训练轮数
		min_count: 忽略出现次数小于该值的词
"""

model = Word2Vec(sentences=sentences, size=100, iter=10, min_count=20)

# 保存模型
model_file = os.path.join(output_dir, 'model.w2v')
model.save(model_file)
```
```

##### 5.1 word2vec模型的使用

```python
```python
# 读取模型 
model2 = Word2Vec.load(model_file)     

# 查找语义相近的词
def invest_similar(*args, **kwargs):
    res = model2.most_similar(*args, **kwargs)
    print u'\n'.join([u'{}:{}'.format(x[0], x[1]) for x in res]) 
invest_similar(u'摄影', topn=1)
	"""
		女人 + 先生 - 男人 = 女士
		先生 - 女士 = 男人 - 女人，这个向量的方向就代表了性别!
	"""
invest_similar(positive=[u'女人', u'先生'], negative=[u'男人'], topn=1)

# 计算两个词的相似度
model2.similarity(u'摄影', u'摄像') 

# 查询某个词的词向量
model2[u'摄影']
```
```



