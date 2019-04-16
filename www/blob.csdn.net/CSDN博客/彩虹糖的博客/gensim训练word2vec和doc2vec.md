# gensim训练word2vec和doc2vec - 彩虹糖的博客 - CSDN博客





2018年04月03日 00:03:57[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：900








word2vec和doc2vec是做NLP过程中经常使用的方法。

用向量表示词汇这种做法由来已久，最早使用的是one-hot向量，即只有一个维度为1，其余维度都为0，但这种做法有很多缺陷，过多的维度会导致数据处理的困难，而且这种表示方法无法体现词所在的上下文关系。于是便有了word embedding，将词汇映射到连续的，稠密的向量空间。doc2vec实现方法和word2vec类似，通过训练，我们可以推测出一句话的固定维度的向量表达。

本文使用python gensim包，在Django框架基础上简单说明word2vec训练和doc2vec训练方法。

## 一.word2vec训练

将目标语料库进行分词

```python
def train_data_build():
    file = r'F:\train_data.txt'
    names = file_name('F:\\data')
    for name in names:
        f = open(name, errors='ignore')
        st = f.read()
        with open(file, 'a+') as f:
            seg_list = jieba.cut(st, cut_all=False)
            f.write(" ".join(seg_list))
            f.write('\n')
        f.close()
```

对分词结果进行训练

```python
def train_data():
    from gensim.models import word2vec
    sentences = word2vec.Text8Corpus('F:\\train_data.txt')
    model = word2vec.Word2Vec(sentences, size=50)
    model.save('word2vec_model')
```

分词是一个开销比较大的工作，最开始训练的目标是搜狗公布的新闻语料库，但用我的手提电脑跑了两个小时才切完词，切出了一个十几个G的文本文件，再进行训练电脑立马卡死。

## 二.doc2vec训练

假设已经搜集好了需要训练的句子，保存在model Sentence中

```python
TaggededDocument = gensim.models.doc2vec.TaggedDocument
def train_sentence():
    sens = Sentence.objects.all()
    dic = {}
    for sen in sens:
        dic[sen.name]=sen.sen_id
    keys=dic.keys()
    x_train = []
    for key in keys:
        document = TaggededDocument(key, tags=dic[key])
        x_train.append(document)
    model_dm = Doc2Vec(x_train, min_count=1, window=3, vector_size=100, sample=1e-3, negative=5, workers=4)
    model_dm.train(x_train, total_examples=model_dm.corpus_count, epochs=70)
    model_dm.save('model/model_doc2vec')
```





