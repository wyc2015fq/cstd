# 主题模型︱几款新主题模型——SentenceLDA、CopulaLDA、TWE简析与实现 - 素质云笔记/Recorder... - CSDN博客





2017年07月15日 20:51:57[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：10128








> 
百度最近开源了一个新的关于主题模型的项目。文档主题推断工具、语义匹配计算工具以及基于工业级语料训练的三种主题模型：Latent 

  Dirichlet Allocation(LDA)、SentenceLDA 和Topical Word Embedding(TWE)。 

  .
# 一、Familia简介

帮Familia，打个小广告~  Familia的[github](https://github.com/baidu/Familia)
**主题模型在工业界的应用范式可以抽象为两大类: 语义表示和语义匹配。**
- 
语义表示 (Semantic Representation) 

对文档进行主题降维，获得文档的语义表示，这些语义表示可以应用于文本分类、文本内容分析、CTR预估等下游应用。

- 
语义匹配 (Semantic Matching)


计算文本间的语义匹配度，我们提供两种文本类型的相似度计算方式:

```
- 短文本-长文本相似度计算，使用场景包括文档关键词抽取、计算搜索引擎查询和网页的相似度等等。
- 长文本-长文本相似度计算，使用场景包括计算两篇文档的相似度、计算用户画像和新闻的相似度等等。
```

### **Familia自带的Demo包含以下功能：**
- 语义表示计算

利用主题模型对输入文档进行主题推断，以得到文档的主题降维表示。
- 
语义匹配计算

计算文本之间的相似度，包括短文本-长文本、长文本-长文本间的相似度计算。

- 
模型内容展现 

对模型的主题词，近邻词进行展现，方便用户对模型的主题有直观的理解。


.

# 二、Topical Word Embedding(TWE)

Zhiyuan Liu老师的文章，[paper下载](http://www.aaai.org/ocs/index.php/AAAI/AAAI15/paper/download/9314/9535/)以及[github](https://github.com/largelymfs/topical_word_embeddings)

 In this way, contextual word embeddings can be flexibly obtained to measure contextual word similarity. We can also build document representations.

### 且有三款：TWE-1，TWE-2，TWE-3，来看看和传统的skip-gram的结构区别：

![这里写图片描述](https://img-blog.csdn.net/20170715203145804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 在多标签文本分类的精确度：

![这里写图片描述](https://img-blog.csdn.net/20170715203259973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 百度开源项目 Familia中TWE模型的内容展现：

```
请输入主题编号(0-10000):    105
Embedding Result              Multinomial Result
------------------------------------------------
对话                                    对话
磋商                                    合作
合作                                    中国
非方                                    磋商
探讨                                    交流
对话会议                                联合
议题                                    国家
中方                                    讨论
对话会                                  支持
交流                                    包括
```

第一列为基于embedding的结果，第二列为基于多项分布的结果，均按照在主题中的重要程度从大到小的顺序排序。

### 来简单看一下train文件：

```python
import gensim #modified gensim version
import pre_process # read the wordmap and the tassgin file and create the sentence
import sys
if __name__=="__main__":
    if len(sys.argv)!=4:
        print "Usage : python train.py wordmap tassign topic_number"
        sys.exit(1) 
    reload(sys)
    sys.setdefaultencoding('utf-8')
    wordmapfile = sys.argv[1]
    tassignfile = sys.argv[2]
    topic_number = int(sys.argv[3])
    id2word = pre_process.load_id2word(wordmapfile)
    pre_process.load_sentences(tassignfile, id2word)
    sentence_word = gensim.models.word2vec.LineSentence("tmp/word.file")
    print "Training the word vector..."
    w = gensim.models.Word2Vec(sentence_word,size=400, workers=20)
    sentence = gensim.models.word2vec.CombinedSentence("tmp/word.file","tmp/topic.file")
    print "Training the topic vector..."
    w.train_topic(topic_number, sentence)
    print "Saving the topic vectors..."
    w.save_topic("output/topic_vector.txt")
    print "Saving the word vectors..."
    w.save_wordvector("output/word_vector.txt")
```

.

# 三、SentenceLDA

[paper链接](https://pdfs.semanticscholar.org/c311/778adb9484c86250e915aecd9714f4206050.pdf) +  [github：balikasg/topicModelling](https://github.com/balikasg/topicModelling/)

### **SentenceLDA是什么？**

an extension of LDA whose goal is to overcome this limitation by incorporating the structure of 

the text in the generative and inference processes.

### **SentenceLDA和LDA区别？**

LDA and senLDA differ in that the second assumes a very strong dependence of the latent topics between the words of sentences, whereas the first ssumes independence between the words of documents in general

### **SentenceLDA和LDA两者对比实验：**

We illustrate the advantages of sentenceLDA by comparing it with LDA using both intrinsic (perplexity) and extrinsic (text classification) evaluation tasks on different text collections 
![这里写图片描述](https://img-blog.csdn.net/20170715204602644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170715204224627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **原作者的github的结果：**

[https://github.com/balikasg/topicModelling/tree/master/senLDA](https://github.com/balikasg/topicModelling/tree/master/senLDA)

截取一部分code：

```python
import numpy as np, vocabulary_sentenceLayer, string, nltk.data, sys, codecs, json, time
from nltk.tokenize import sent_tokenize
from lda_sentenceLayer import lda_gibbs_sampling1
from sklearn.cross_validation import train_test_split, StratifiedKFold
from nltk.stem import WordNetLemmatizer
from sklearn.utils import shuffle
from functions import *

path2training = sys.argv[1]
training = codecs.open(path2training, 'r', encoding='utf8').read().splitlines()

topics = int(sys.argv[2])
alpha, beta = 0.5 / float(topics), 0.5 / float(topics)

voca_en = vocabulary_sentenceLayer.VocabularySentenceLayer(set(nltk.corpus.stopwords.words('english')), WordNetLemmatizer(), excluds_stopwords=True)

ldaTrainingData = change_raw_2_lda_input(training, voca_en, True)
ldaTrainingData = voca_en.cut_low_freq(ldaTrainingData, 1)
iterations = 201


classificationData, y = load_classification_data(sys.argv[3], sys.argv[4])
classificationData = change_raw_2_lda_input(classificationData, voca_en, False)
classificationData = voca_en.cut_low_freq(classificationData, 1)

final_acc, final_mif, final_perpl, final_ar, final_nmi, final_p, final_r, final_f = [], [], [], [], [], [], [], []
start = time.time()
for j in range(5):
    perpl, cnt, acc, mif, ar, nmi, p, r, f = [], 0, [], [], [], [], [], [], []
    lda = lda_gibbs_sampling1(K=topics, alpha=alpha, beta=beta, docs= ldaTrainingData, V=voca_en.size())
    for i in range(iterations):
        lda.inference()
        if i % 5 == 0:
            print "Iteration:", i, "Perplexity:", lda.perplexity()
            features = lda.heldOutPerplexity(classificationData, 3)
            print "Held-out:", features[0]
            scores = perform_class(features[1], y)
            acc.append(scores[0][0])
            mif.append(scores[1][0])
            perpl.append(features[0])
    final_acc.append(acc)
    final_mif.append(mif)
    final_perpl.append(perpl)
```

### **来看看百度开源项目的最终效果，LDA和SentenceLDA的内容展现：**

LDA结果：

```
请输入主题编号(0-1999): 105
--------------------------------------------
对话    0.189676
合作    0.0805558
中国    0.0276284
磋商    0.0269797
交流    0.021069
联合    0.0208559
国家    0.0183163
讨论    0.0154165
支持    0.0146714
包括    0.014198
```

第二列的数值表示词在该主题下的重要程度。 

SentenceLDA结果：

```
请输入主题编号(0-1999): 105
--------------------------------------------
浙江    0.0300595
浙江省  0.0290975
宁波    0.0195277
记者    0.0174735
宁波市  0.0132504
长春市  0.0123353
街道    0.0107271
吉林省  0.00954326
金华    0.00772971
公安局  0.00678163
```

.

# 四、CopulaLDA

SentenceLDA和CopulaLDA同一作者，可见github：[balikasg/topicModelling](https://github.com/balikasg/topicModelling/)

没细看，来贴效果好了： 
![这里写图片描述](https://img-blog.csdn.net/20170715204821023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170715204846448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170715204855818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
## 参考文献：

[Familia一个中文主题建模工具包](https://www.ctolib.com/baidu-Familia.html#familia)







