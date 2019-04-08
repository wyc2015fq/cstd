# 文本相似度（tf-idf 和 bm25的算法讲解）

2018年05月20日 18:33:01 [诶呀吗_Bug](https://me.csdn.net/weixin_40411446) 阅读数：2779



## 1.关于tf-idf：

(使用tf-idf和向量空间模型)

TF： 文档 j 中的关键词 i 的归一化词频值

描述某一词在一篇文档中出现的频繁程度。

（为了阻止更长的文档得到更高的相关度权值，必须进行文档长度的某种**归一化**）



- TF=freq(i,j) / maxOthers(i,j)      ###(maxxOthers = max(freq(z,j)) 

IDF : 逆文档频率。

降低所有文档中几乎都会出现的关键词的权重。（例如的，了等）



- IDF = log(N /n(i))   ###N 为所有可能推荐文档的数量，n(i)为N中关键词 i 出现过得文档的数量。

TF-IDF权值 = TF*IDF





## 2.关于BM25：（可插拔的相似度算法）

BM25源于概率相关模型，而非向量空间模型

BM25同样使用词频，逆文档频率以及字段长度归一化，但是每个因子的定义都有细微差别

（###TF-IDF没有考虑词频上限的问题，因为高频停用词已经被移除了）

（###BM25 有一个上限，文档里出现5-10次的词会比那些只出现一两次的对相关度有显著影响），参见词频饱和度图：

![img](https://img-blog.csdn.net/20180520180628686?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDQxMTQ0Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

字段长度的归一化：

字段某个词的频率所带来的重要性会被这个字段长度抵消，但是在实际的评分函数中会将所有字段以同等方式对待，认为所有较短的title 比较长的body 字段更重要。

BM25 当然也认为较短字段应该有更多的权重，但是它会分别考虑每个字段内容的平均长度，这样就能区分短 `title` 字段和 `长` title 字段。





在 [查询时权重提升](https://www.elastic.co/guide/cn/elasticsearch/guide/current/query-time-boosting.html) 中，已经说过 `title` 字段因为其长度比 `body` 字段 *自然* 有更高的权重提升值。由于字段长度的差异只能应用于单字段，这种自然的权重提升会在使用 BM25 时消失。





不像 TF/IDF ，BM25 有一个比较好的特性就是它提供了两个可调参数：

- `k1`

  这个参数控制着词频结果在词频饱和度中的上升速度。默认值为 `1.2` 。值越小饱和度变化越快，值越大饱和度变化越慢。

- `b`

  这个参数控制着字段长归一值所起的作用， `0.0` 会禁用归一化， `1.0` 会启用完全归一化。默认值为 `0.75` 。

在实践中，调试 BM25 是另外一回事， `k1` 和 `b` 的默认值适用于绝大多数文档集合，但最优值还是会因为文档集不同而有所区别，为了找到文档集合的最优值，就必须对参数进行反复修改验证。

数学公式：

​        idf  : ![img](https://img-blog.csdn.net/20180520182019171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDQxMTQ0Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​    

​        tf:![img](https://img-blog.csdn.net/20180520182805915?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDQxMTQ0Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



​    sim = tf*idf

BM25算法代码：

```python
class BM25(object):
 
    def __init__(self, docs):
        self.D = len(docs)
        self.avgdl = sum([len(doc)+0.0 for doc in docs]) / self.D
        self.docs = docs
        self.f = []  # 列表的每一个元素是一个dict，dict存储着一个文档中每个词的出现次数
        self.df = {} # 存储每个词及出现了该词的文档数量
        self.idf = {} # 存储每个词的idf值
        self.k1 = 1.5   #参数1，默认值是1.2  #####调参就是调这里的啦
        self.b = 0.75   #参数1，默认值为0.75
    
        self.init()
 
    def init(self):
        for doc in self.docs:
            tmp = {}
            for word in doc:
                tmp[word] = tmp.get(word, 0) + 1  # 存储每个文档中每个词的出现次数
            self.f.append(tmp)
            for k in tmp.keys():
                self.df[k] = self.df.get(k, 0) + 1        #分母+1，平滑处理，避免出现log(0)
        for k, v in self.df.items():
            self.idf[k] = math.log(self.D-v+0.5)-math.log(v+0.5)       #IDF的计算
 
    def sim(self, doc, index):
        score = 0
        for word in doc:
            if word not in self.f[index]:
                continue
            d = len(self.docs[index])  #k1
            score += (self.idf[word]*self.f[index][word]*(self.k1+1)
                      / (self.f[index][word]+self.k1*(1-self.b+self.b*d
                                                      / self.avgdl)))
        return score
 
    def simall(self, doc):
        scores = []
        for index in range(self.D):
            score = self.sim(doc, index)
            scores.append(score)
        return scores
```

25 公式中包含 3 个自由调节参数 ，除了调节因子 b 外 ，还有针对词频的调节因子 k1和 k2。（我们这里不考虑k2,一般只调节k1）

 k1的作用是对查询词在文档中的词频进行调节，如果将 k1设定为 0，则第二部分计算因子成了整数 1，即不考虑词频的因素，退化成了二元独立模型。 

如果将 k1设定为较大值， 则第二部分计算因子基本和词频 fi保持线性增长，即放大了词频的权值，根据经验，一般将 k1设定为 1.2。调节因子 k2和 k1的作用类似，不同点在于其是针对查询词中的词频进行调节，一般将这个值设定在 0 到 1000 较大的范围内。之所以如此，是因为查询往往很短，所以不同查询词的词频都很小，词频之间差异不大，较大的调节参数数值设定范围允许对这种差异进行放大。



参考文献： 《Elasticsearch权威指南》中文版-[可插拔的相似度算法权威讲解](https://www.elastic.co/guide/cn/elasticsearch/guide/current/pluggable-similarites.html#bm25)

​                    BM25算法详解

具体的公式

在 [查询时权重提升](https://www.elastic.co/guide/cn/elasticsearch/guide/current/query-time-boosting.html) 中，已经说过 `title` 字段因为其长度比 `body` 字段 *自然* 有更高的权重提升值。由于字段长度的差异只能应用于单字段，这种自然的权重提升会在使用 BM25 时消失。