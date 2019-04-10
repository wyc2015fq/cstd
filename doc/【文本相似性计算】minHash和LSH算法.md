# 【文本相似性计算】minHash和LSH算法

 																				2018年09月25日 14:27:06 					[夜风之南](https://me.csdn.net/weixin_43098787) 						阅读数：450 										

 									

# minHash和LSH算法

------

## 原理

原理部分皆转载于http://www.cnblogs.com/bourneli/archive/2013/04/04/2999767.html

### Jaccard相似度

判断两个集合是否相等，一般使用称之为Jaccard相似度的算法（后面用Jac(S1,S2)来表示集合S1和S2的Jaccard相似度）。举个列子，集合X  = {a,b,c}，Y = {b,c,d}。那么Jac(X,Y) = 2 / 4 =  0.50。也就是说，结合X和Y有50%的元素相同。下面是形式的表述Jaccard相似度公式：

Jac(X,Y) = |X∩Y| / |X∪Y|

也就是两个结合交集的个数比上两个集合并集的个数。范围在[0,1]之间。

### minHash

举个例子，S1 = {a,d,e}，S2 = {c, e}，设全集U = {a,b,c,d,e}。集合可以如下表示：![在这里插入图片描述](https://img-blog.csdn.net/20180925140625653?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzA5ODc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上表中，列表示集合，行表示元素，值1表示某个集合具有某个值，0则相反。Minhash算法大体思路是：采用一种hash函数，将元素的位置均匀打乱，然后将新顺序下每个集合第一个元素作为该集合的特征值。比如哈希函数h1(i)  = (i + 1) % 5，其中i为行号。作用于集合S1和S2，得到如下结果：
 ![在这里插入图片描述](https://img-blog.csdn.net/20180925140731133?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzA5ODc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这时，Minhash(S1) = e，Minhash(S2) = e。也就是说用元素e表示S1，用元素e表示集合S2。

### LSH – 局部敏感哈希

现在有了原始集合的摘要，但是还是没有解决最初的问题，仍然需要遍历所有的集合对,，才能所有相似的集合对，复杂度仍然是O(n2)。所以，接下来描述解决这个问题的核心思想LSH。其基本思路是将相似的集合聚集到一起，减小查找范围，避免比较不相似的集合。仍然是从例子开始，现在有5个集合，计算出对应的Minhash摘要，如下：![在这里插入图片描述](https://img-blog.csdn.net/20180925140859304?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzA5ODc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面的集合摘要采用了12个不同的hash函数计算出来，然后分成了B =  4个区间。前面已经分析过，任意两个集合（S1，S2）对应的Minhash值相等的概率r =  Jac(S1，S2)。先分析区间1，在这个区间内，P(集合S1等于集合S2) =  r3。所以只要S­1和S2的Jaccard相似度越高，在区间1内越有可能完成全一致，反过来也一样。那么P(集合S1不等于集合S2) = 1 -  r3。现在有4个区间，其他区间与第一个相同，所以P(4个区间上，集合S1都不等于集合S2) = (1 –  r3)4。P(4个区间上，至少有一个区间，集合S1等于集合S2) = 1 - (1 –  r3)4。这里的概率是一个r的函数，形状犹如一个S型，如下：
 ![在这里插入图片描述](https://img-blog.csdn.net/20180925140949383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzA5ODc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果令区间个数为B，每个区间内的行数为C，那么上面的公式可以形式的表示为：

P(B个区间中至少有一个区间中两个结合相等) = 1 - (1 - rC)B

令r = 0.4，C=3，B =  100。上述公式计算的概率为0.9986585。这表明两个Jaccard相似度为0.4的集合在至少一个区间内冲撞的概率达到了99.9%。根据这一事实，我们只需要选取合适的B和C，和一个冲撞率很低的hash函数，就可以将相似的集合至少在一个区间内冲撞，这样也就达成了本节最开始的目的：将相似的集合放到一起。具体的方法是为B个区间，准备B个hash表，和区间编号一一对应，然后用hash函数将每个区间的部分集合映射到对应hash表里。最后遍历所有的hash表，将冲撞的集合作为候选对象进行比较，找出相识的集合对。整个过程是采用O(n)的时间复杂度，因为B和C均是常量。由于聚到一起的集合相比于整体比较少，所以在这小范围内互相比较的时间开销也可以计算为常量，那么总体的计算时间也是O(n)。

## 代码

### 方法一：引用python包datasketch

安装：

```
pip install datasketch
```

使用示例如下：

#### MinHash

```python
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

12345678910111213141516171819
```

#### MinHash LSH

```python
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

1234567891011121314151617181920212223242526
```

#### MinHash LSH Forest——局部敏感随机投影森林

```python
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

1234567891011121314151617181920212223242526272829303132333435363738
```

### 方法二

minHash源码实现如下：

```python
from random import randint, seed, choice, random
import string
import sys
import itertools

def generate_random_docs(n_docs, max_doc_length, n_similar_docs):
	for i in range(n_docs):
		if n_similar_docs > 0 and i % 10 == 0 and i > 0:
			permuted_doc = list(lastDoc)
			permuted_doc[randint(0,len(permuted_doc))] = choice('1234567890')
			n_similar_docs -= 1
			yield ''.join(permuted_doc)
		else:
			lastDoc = ''.join(choice('aaeioutgrb ') for _ in range(randint(int(max_doc_length*.75), max_doc_length)))
			yield lastDoc

def generate_shingles(doc, shingle_size):
	shingles = set([])
	for i in range(len(doc)-shingle_size+1):
		shingles.add(doc[i:i+shingle_size])
	return shingles

def get_minhash(shingles, n_hashes, random_strings):
	minhash_row = []
	for i in range(n_hashes):
		minhash = sys.maxsize
		for shingle in shingles:
			hash_candidate = abs(hash(shingle + random_strings[i]))
			if hash_candidate < minhash:
				minhash = hash_candidate
		minhash_row.append(minhash)
	return minhash_row

def get_band_hashes(minhash_row, band_size):
	band_hashes = []
	for i in range(len(minhash_row)):
		if i % band_size == 0:						
			if i > 0:
				band_hashes.append(band_hash)
			band_hash = 0
		band_hash += hash(minhash_row[i])		
	return band_hashes

def get_similar_docs(docs, n_hashes=400, band_size=7, shingle_size=3, collectIndexes=True):
	hash_bands = {}
	random_strings = [str(random()) for _ in range(n_hashes)]
	docNum = 0
	for doc in docs:
		shingles = generate_shingles(doc, shingle_size)
		minhash_row = get_minhash(shingles, n_hashes, random_strings)
		band_hashes = get_band_hashes(minhash_row, band_size)
		
		docMember = docNum if collectIndexes else doc
		for i in range(len(band_hashes)):
			if i not in hash_bands:
				hash_bands[i] = {}
			if band_hashes[i] not in hash_bands[i]:
				hash_bands[i][band_hashes[i]] = [docMember]
			else:
				hash_bands[i][band_hashes[i]].append(docMember)
		docNum += 1

	similar_docs = set()
	for i in hash_bands:
		for hash_num in hash_bands[i]:
			if len(hash_bands[i][hash_num]) > 1:
				for pair in itertools.combinations(hash_bands[i][hash_num], r=2):
					similar_docs.add(pair) 

	return similar_docs
		
if __name__ == '__main__':
	n_hashes = 200
	band_size = 7
	shingle_size = 3
	n_docs = 1000
	max_doc_length = 40
	n_similar_docs = 10
	seed(42)
	docs = generate_random_docs(n_docs, max_doc_length, n_similar_docs)

	similar_docs = get_similar_docs(docs, n_hashes, band_size, shingle_size, collectIndexes=False)

	print(similar_docs)
	r = float(n_hashes/band_size)
	similarity = (1/r)**(1/float(band_size))
	print("similarity: %f" % similarity)
	print("# Similar Pairs: %d" % len(similar_docs))

	if len(similar_docs) == n_similar_docs:
		print("Test Passed: All similar pairs found.")
	else:
		print("Test Failed.")

12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970717273747576777879808182838485868788899091929394
```

------

## 参考资料

[1] 利用Minhash和LSH寻找相似的集合
 <http://www.cnblogs.com/bourneli/archive/2013/04/04/2999767.html>
 [2] datasketch
 <https://ekzhu.github.io/datasketch/>