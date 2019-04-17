# LSH︱python实现MinHash-LSH及MinHash LSH Forest——datasketch（四） - 素质云笔记/Recorder... - CSDN博客





2017年04月21日 19:17:57[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5027








关于局部敏感哈希算法，之前用R语言实现过，但是由于在R中效能太低，于是放弃用LSH来做相似性检索。学了Python发现很多模块都能实现，而且通过随机投影森林让查询数据更快，觉得可以试试大规模应用在数据相似性检索+去重的场景。

> 
私认为，文本的相似性可以分为两类：一类是机械相似性；一类是语义相似性。  

  机械相似性代表着，两个文本内容上的相关程度，比如“你好吗”和“你好”的相似性，纯粹代表着内容上字符是否完全共现，应用场景在：文章去重；  

  语义相似性代表着，两个文本语义上的相似程度，比如“苹果”和“公司”的相似性，本篇不做这一讨论
之前写关于R语言实现的博客： 
[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（一，基本原理）](http://blog.csdn.net/sinat_26917383/article/details/52451028)
[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（二，textreuse介绍）](http://blog.csdn.net/sinat_26917383/article/details/52457252)

. 

机械相似性python版的四部曲： 
[LSH︱python实现局部敏感随机投影森林——LSHForest/sklearn（一）](http://blog.csdn.net/sinat_26917383/article/details/70243066)
[LSH︱python实现局部敏感哈希——LSHash（二）](http://blog.csdn.net/sinat_26917383/article/details/70285678)
[相似性︱python+opencv实现pHash算法+hamming距离（simhash）（三）](http://blog.csdn.net/sinat_26917383/article/details/70287521)
[LSH︱python实现MinHash-LSH及MinHash LSH Forest——datasketch（四）](http://blog.csdn.net/sinat_26917383/article/details/70332325)
datasketch这个模块有非常多的功能，主要是：
- HyperLogLog
- HyperLogLog++
- MinHash LSH
- MinHash LSH Ensemble
- MinHash LSH Forest
- MinHash
- Weighted MinHash

其中MinHash 与simHash不同，其主要采用的是Jaccard距离，LSHForest/sklearn是常规的Hash函数，所以可以用cosine距离。 

其中，Jaccard距离较多地可以应用在BOW模型之上，图片和文字在用词袋模型表征特征的时候，较适合应用。 

.
# 一、MinHash

在检索场景应用比较多，每当有新的搜索,需要创建一个新的MinHash，同时与候选集中求Jaccard相似性，然后根据一些阈值筛选符合的样例。

## 1、MinHash 主函数
`class datasketch.MinHash(num_perm=128, seed=1, hashobj=<built-in function openssl_sha1>, hashvalues=None, permutations=None)`
MinHash 哈希化专属的距离是 Jaccard距离。
- num_perm (int, optional)

哈希置换函数设定个数，如果hashvalues有值，那么该参数将被忽略。
- 
seed (int, optional)

— MinHash中随机种子。

- 
hashobj (optional)

— MinHash的哈希方程式。

- 
hashvalues (numpy.array or list, optional)

— 哈希内部状态。如果使用另外已经存在状态的MinHash，哈希初始化会更快

- 
permutations (optional)

— 哈希置换函数的参数。如果有已经存在状态的MinHash，会更快


当然，如果要节约内存可以使用： datasketch.LeanMinHash 

MinHash 

## 2、MinHash案例

```
from datasketch import MinHash

data1 = ['minhash', 'is', 'a', 'probabilistic', 'data', 'structure', 'for',
        'estimating', 'the', 'similarity', 'between', 'datasets']
data2 = ['minhash', 'is', 'a', 'probability', 'data', 'structure', 'for',
        'estimating', 'the', 'similarity', 'between', 'documents']

m1, m2 = MinHash(), MinHash()
for d in data1:
    m1.update(d.encode('utf8'))
for d in data2:
    m2.update(d.encode('utf8'))
print("Estimated Jaccard for data1 and data2 is", m1.jaccard(m2))

s1 = set(data1)
s2 = set(data2)
actual_jaccard = float(len(s1.intersection(s2)))/float(len(s1.union(s2)))
print("Actual Jaccard for data1 and data2 is", actual_jaccard)
```

案例的大致步骤为：
- MinHash初始化状态，需要预先设定MinHash()初始化
- 内容哈希化，内容m1.update哈希化
- jaccard距离，float(len(s1.intersection(s2)))/float(len(s1.union(s2)))用集合的方式求距离

**另外：**
- 1、提高精度

```bash
m = MinHash(num_perm=256)
```

通过调整num_perm数量，来提高精度，代价是更多CPU消耗 + 更多内存
- 2、哈希合并
`m1.merge(m2)`
联合两个minhash，这样就可以更快的进行并行parallel MapReduce
- 3、求cardinality count

```python
def count(self):
        k = len(self)
        return np.float(k) / np.sum(self.hashvalues / np.float(_max_hash)) - 1.0

m.count()
```

# 二、MinHash LSH

LSH能够将相似的条例远大于非相似的，详细详细可见[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（一，基本原理）](http://blog.csdn.net/sinat_26917383/article/details/52451028)

## 1、主函数MinHash LSH
`MinHashLSH(threshold=0.9, num_perm=128, weights=(0.5, 0.5), params=None)`- 
threshold (float)

– Jaccard 距离阈值设定，默认为0.9

- 
num_perm (int, optional)

– 哈希置换函数设定个数，在weighted-MinHash中为样本规模大小。

- 
weights (tuple, optional)

– 优化Jaccard 阈值，能够弹性选择。

- 
params (tuple, optional)

– bands 的数量与规模大小。


## 2、案例

```bash
from datasketch import MinHash, MinHashLSH

set1 = set(['minhash', 'is', 'a', 'probabilistic', 'data', 'structure', 'for',
            'estimating', 'the', 'similarity', 'between', 'datasets'])
set2 = set(['minhash', 'is', 'a', 'probability', 'data', 'structure', 'for',
            'estimating', 'the', 'similarity', 'between', 'documents'])
set3 = set(['minhash', 'is', 'probability', 'data', 'structure', 'for',
            'estimating', 'the', 'similarity', 'between', 'documents'])

m1 = MinHash(num_perm=128)
m2 = MinHash(num_perm=128)
m3 = MinHash(num_perm=128)
for d in set1:
    m1.update(d.encode('utf8'))
for d in set2:
    m2.update(d.encode('utf8'))
for d in set3:
    m3.update(d.encode('utf8'))

# Create LSH index
lsh = MinHashLSH(threshold=0.5, num_perm=128)
lsh.insert("m2", m2)
lsh.insert("m3", m3)
result = lsh.query(m1)
print("Approximate neighbours with Jaccard similarity > 0.5", result)
```

案例的大致步骤为：
- MinHash初始化状态，需要预先设定MinHash(num_perm=128)初始化状态
- 内容哈希化，内容m1.update哈希化
- MinHashLSH初始化， MinHashLSH(threshold=0.5, num_perm=128)
- 内容载入LSH系统，lsh.insert(“m3”, m3)，其中insert（Hash名称，minHash值）
- 查询，lsh.query，其中查询的内容也必须minHash化。

同时，MinHashLSH不能采用多项内容 Top-K 查询。可以使用之后的 MinHash LSH Forest，同时Jaccard 距离可能不是最好的选择，也可以选择其他的距离，可见[MinHash LSH Ensemble](https://ekzhu.github.io/datasketch/lshensemble.html#minhash-lsh-ensemble).

**额外的其他内容：**
- 移除相关哈希值：
`remove(key)`
与lsh.insert(“m2”, m2)，对应。
- 是否为空
`is_empty()`
返回的是布尔值，检查hash是否为空 

.

# 三、MinHash LSH Forest——局部敏感随机投影森林

与文章[LSH︱python实现局部敏感随机投影森林——LSHForest/sklearn（一）](http://blog.csdn.net/sinat_26917383/article/details/70243066)类似，都是用来做随机投影森林的，这里专门使用minhash。 

MinHash LSH可以使用radius 或阈值查询。同时，MinHash LSH Forest支持指定top-K查询内容。forest更少地占用空间。

## 1、主函数

```bash
MinHashLSHForest(num_perm=128, l=8)
```

与原论文使用prefix trees不同，作者这里把哈希值存储在每个哈希列表中。 

num_perm就是随机置换函数的个数，l代表prefix trees的数量。

其中每个前缀树的最大深度K取决于num_perm、l
`k = int(num_perm / l)`
.

## 2、案例

```bash
from datasketch import MinHashLSHForest, MinHash

data1 = ['minhash', 'is', 'a', 'probabilistic', 'data', 'structure', 'for',
        'estimating', 'the', 'similarity', 'between', 'datasets']
data2 = ['minhash', 'is', 'a', 'probability', 'data', 'structure', 'for',
        'estimating', 'the', 'similarity', 'between', 'documents']
data3 = ['minhash', 'is', 'probability', 'data', 'structure', 'for',
        'estimating', 'the', 'similarity', 'between', 'documents']

# Create MinHash objects
m1 = MinHash(num_perm=128)
m2 = MinHash(num_perm=128)
m3 = MinHash(num_perm=128)
for d in data1:
    m1.update(d.encode('utf8'))
for d in data2:
    m2.update(d.encode('utf8'))
for d in data3:
    m3.update(d.encode('utf8'))

# Create a MinHash LSH Forest with the same num_perm parameter
forest = MinHashLSHForest(num_perm=128)

# Add m2 and m3 into the index
forest.add("m2", m2)
forest.add("m3", m3)

# IMPORTANT: must call index() otherwise the keys won't be searchable
forest.index()

# Check for membership using the key
print("m2" in forest)
print("m3" in forest)

# Using m1 as the query, retrieve top 2 keys that have the higest Jaccard
result = forest.query(m1, 2)
print("Top 2 candidates", result)
```

案例的大致步骤为：
- MinHash初始化状态，需要预先设定MinHash(num_perm=128)初始化状态
- 内容哈希化，内容m1.update哈希化
- MinHashLSHForest初始化， MinHashLSHForest(num_perm=128)
- 内容载入投影森林之中，forest.add(“m2”, m2)
- forest.index()，相当于update一下，更新一下
- 查询，lsh.query，其中查询的内容也必须minHash化。 

.

# 四、[MinHash LSH Ensemble](https://ekzhu.github.io/datasketch/lshensemble.html)

新距离：Containment，简单介绍一下。 
![这里写图片描述](https://img-blog.csdn.net/20170421191228646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170421191241693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**案例一则：**

```bash
from datasketch import MinHashLSHEnsemble, MinHash

set1 = set(["cat", "dog", "fish", "cow"])
set2 = set(["cat", "dog", "fish", "cow", "pig", "elephant", "lion", "tiger",
             "wolf", "bird", "human"])
set3 = set(["cat", "dog", "car", "van", "train", "plane", "ship", "submarine",
             "rocket", "bike", "scooter", "motorcyle", "SUV", "jet", "horse"])

# Create MinHash objects
m1 = MinHash(num_perm=128)
m2 = MinHash(num_perm=128)
m3 = MinHash(num_perm=128)
for d in set1:
    m1.update(d.encode('utf8'))
for d in set2:
    m2.update(d.encode('utf8'))
for d in set3:
    m3.update(d.encode('utf8'))

# Create an LSH Ensemble index with a threshold
lshensemble = MinHashLSHEnsemble(threshold=0.8, num_perm=128)

# Index takes an iterable of (key, minhash, size)
lshensemble.index([("m2", m2, len(set2)), ("m3", m3, len(set3))])

# Check for membership using the key
print("m2" in lshensemble)
print("m3" in lshensemble)

# Using m1 as the query, get an result iterator
print("Sets with containment > 0.8:")
for key in lshensemble.query(m1, len(set1)):
    print(key)
```

.

# 五、[Weighted MinHash](https://ekzhu.github.io/datasketch/weightedminhash.html)

Jaccard距离加权

```
import numpy as np
from datasketch import WeightedMinHashGenerator
from datasketch import MinHashLSH

v1 = np.random.uniform(1, 10, 10)
v2 = np.random.uniform(1, 10, 10)
v3 = np.random.uniform(1, 10, 10)
mg = WeightedMinHashGenerator(10, 5)
m1 = mg.minhash(v1)
m2 = mg.minhash(v2)
m3 = mg.minhash(v3)

# Create weighted MinHash LSH index
lsh = MinHashLSH(threshold=0.1, sample_size=5)
lsh.insert("m2", m2)
lsh.insert("m3", m3)
result = lsh.query(m1)
print("Approximate neighbours with weighted Jaccard similarity > 0.1", result)
```








