# elasticsearch min_hash 应用分析 - ggabcda的博客 - CSDN博客





2017年08月28日 15:37:28[ggabcda](https://me.csdn.net/ggabcda)阅读数：731








需求作相似文本查询



爬虫作页面去重,会用到simhash,第一个想到的是用simhash算法



但在现有数据集(elasticsearch集群)上用simhash,成本高,simhash值还好计算,不论是外部api还是实现一套es token filter都很容易实现.最大的难点在于查询,及相似度计算.需要根据simhash的距离,重写elasticsearch的评分逻辑.



如果不考虑关键字权重的话,minhash和simhash的效果类似.



目前新版的elasticsearch(5.5) 原生支持minhash,但官方的文档很粗略



https://www.elastic.co/guide/en/elasticsearch/reference/5.5/analysis-minhash-tokenfilter.html



遂参照minhash及es源码 调研测试了minhash 为图简单中文分词采用的ik



四个参数
|Setting|Description|
|----|----|
|`hash_count`|The number of hashes to hash the token stream with. Defaults to `1`.|
|`bucket_count`|The number of buckets to divide the minhashes into. Defaults to `512`.|
|`hash_set_size`|The number of minhashes to keep per bucket. Defaults to `1`.|
|`with_rotation`|Whether or not to fill empty buckets with the value of the first non-empty bucket to its circular right. Only takes effect if hash_set_size is equal to one. Defaults to `true`if bucket_count is greater than one, else `false`.|





代码是是三层结构(懒得画图)

```
[ hashcount_0:[
   bucket_count_0:[hash_set_size_0,hash_set_size_1...],
   bucket_count_1:[hash_set_size_0,hash_set_size_1...],
   ...
   bucket_count_511:[hash_set_size_0,hash_set_size_1...]
 ],
 hashcount_1:[
   bucket_count_0:[hash_set_size_0,hash_set_size_1...],
   bucket_count_1:[hash_set_size_0,hash_set_size_1...],
   ...
   bucket_count_511:[hash_set_size_0,hash_set_size_1...]
 ]
]
```



```
第一种：使用多个hash函数

        为了计算集合A、B具有最小哈希值的概率，我们可以选择一定数量的hash函数，比如K个。然后用这K个hash函数分别对集合A、B求哈希值，对
每个集合都得到K个最小值。比如Min(A)k={a1,a2,...,ak}，Min(B)k={b1,b2,...,bk}。
        那么，集合A、B的相似度为|Min(A)k ∩ Min(B)k| / |Min(A)k  ∪  Min(B)k|，及Min(A)k和Min(B)k中相同元素个数与总的元素个数的比例。

       第二种：使用单个hash函数

       第一种方法有一个很明显的缺陷，那就是计算复杂度高。使用单个hash函数是怎么解决这个问题的呢？请看：
       前面我们定义过 hmin(S)为集合S中具有最小哈希值的一个元素，那么我们也可以定义hmink(S)为集合S中具有最小哈希值的K个元素。这样一来，
我们就只需要对每个集合求一次哈希，然后取最小的K个元素。计算两个集合A、B的相似度，就是集合A中最小的K个元素与集合B中最小的K个元素
的交集个数与并集个数的比例。
```
for (int i = 0; i < hashCount; i++) {
byte[] bytes = current.getBytes("UTF-16LE");
LongPair hash = new LongPair();
murmurhash3_x64_128(bytes, 0, bytes.length, 0, hash);
LongPair rehashed = combineOrdered(hash, getIntHash(i));
minHashSets.get(i).get((int) ((rehashed.val2 >>> 32) / bucketSize)).add(rehashed);
}
es综合采用了这两种方法

hashcount 是hash函数的个数 getIntHash根据i计算出不同偏移值,类似于不同hash函数计出算不同的hash值

bucket_count 是每个hash函数对集合所有元素计算出的bucket_count 便是hmink(S) 中的k值



最终最小哈希值个数为

hashcount* bucket_count



至于with_rotation目前只觉得是只是一个调节的优化



看代码
while (hashPosition < hashCount) {
if (hashPosition == -1) {
hashPosition++;
} else {
while (bucketPosition < bucketCount) {
if (bucketPosition == -1) {
bucketPosition++;
} else {
        LongPair hash = minHashSets.get(hashPosition).get(bucketPosition).pollFirst();
        if (hash != null) {
termAttribute.setEmpty();
          if (hashCount > 1) {
termAttribute.append(int0(hashPosition));
termAttribute.append(int1(hashPosition));
}
long high = hash.val2;
termAttribute.append(long0(high));
termAttribute.append(long1(high));
termAttribute.append(long2(high));
termAttribute.append(long3(high));
          long low = hash.val1;
termAttribute.append(long0(low));
termAttribute.append(long1(low));
          if (hashCount == 1) {
termAttribute.append(long2(low));
termAttribute.append(long3(low));
}
posIncAttribute.setPositionIncrement(positionIncrement);
offsetAttribute.setOffset(0, endOffset);
typeAttribute.setType(MIN_HASH_TYPE);
posLenAttribute.setPositionLength(1);
          return true;
} else {
bucketPosition++;
}
      }

    }
bucketPosition = -1;
hashPosition++;
}
}
主要在这里
bucketPosition = -1
因为在返回内容小于  hashcount*bucket_count之前 会不断轮询所有bucket_count ,从中取值



假设一条句子只有三个字, hashcount为1 bucket_count为512, hash_set_size
 为默认的1

则这三个字,最多只会填入3个bucket(最少一个),假如是两个,前一项为0,后一项为2,中间大量空白,若不填充则最后为,1,2,1,2,1,2.....直到512条,每个值的占比相等.

但如果选择了填充则各值的占比会更随机
// fix up any wrap around bucket values. ...
if (withRotation && (hashSetSize == 1)) {
for (int hashLoop = 0; hashLoop < hashCount; hashLoop++) {
for (int bucketLoop = 0; bucketLoop < bucketCount; bucketLoop++) {
if (minHashSets.get(hashLoop).get(bucketLoop).size() == 0) {
for (int bucketOffset = 1; bucketOffset < bucketCount; bucketOffset++) {
if (minHashSets.get(hashLoop).get((bucketLoop + bucketOffset) % bucketCount).size() > 0) {
            LongPair replacementHash = minHashSets.get(hashLoop).get((bucketLoop + bucketOffset) % bucketCount)
                .first();
minHashSets.get(hashLoop).get(bucketLoop).add(replacementHash);
            break;
}
        }
      }
    }
  }
}




该filter初始化时,构造三层结构
while (input.incrementToken()) {
  found = true;
String current = new String(termAttribute.buffer(), 0, termAttribute.length());

  for (int i = 0; i < hashCount; i++) {
byte[] bytes = current.getBytes("UTF-16LE");
LongPair hash = new LongPair();
murmurhash3_x64_128(bytes, 0, bytes.length, 0, hash);
LongPair rehashed = combineOrdered(hash, getIntHash(i));
minHashSets.get(i).get((int) ((rehashed.val2 >>> 32) / bucketSize)).add(rehashed);
}
endOffset = offsetAttribute.endOffset();
}
exhausted = true;
input.end();
// We need the end state so an underlying shingle filter can have its state restored correctly.
endState = captureState();
if (!found) {
return false;
}
遍历上一filter来的所有数据,作hash填充至三层结构


while (hashPosition < hashCount) {
if (hashPosition == -1) {
hashPosition++;
} else {
while (bucketPosition < bucketCount) {
if (bucketPosition == -1) {
bucketPosition++;
} else {
        LongPair hash = minHashSets.get(hashPosition).get(bucketPosition).pollFirst();
        if (hash != null) {
termAttribute.setEmpty();
          if (hashCount > 1) {
termAttribute.append(int0(hashPosition));
termAttribute.append(int1(hashPosition));
}
long high = hash.val2;
termAttribute.append(long0(high));
termAttribute.append(long1(high));
termAttribute.append(long2(high));
termAttribute.append(long3(high));
          long low = hash.val1;
termAttribute.append(long0(low));
termAttribute.append(long1(low));
          if (hashCount == 1) {
termAttribute.append(long2(low));
termAttribute.append(long3(low));
}
posIncAttribute.setPositionIncrement(positionIncrement);
offsetAttribute.setOffset(0, endOffset);
typeAttribute.setType(MIN_HASH_TYPE);
posLenAttribute.setPositionLength(1);
          return true;
} else {
bucketPosition++;
}
      }

    }
bucketPosition = -1;
hashPosition++;
}
}
无限循环从三层结构里拿minhash值,将这些值构造为terms传入下一个filter.



elasticsearch (确切的说是Lucene)的min_hash filter代码分析就这些,下一次开始应用测试
|Setting|Description|
|----|----|
|`hash_count`|The number of hashes to hash the token stream with. Defaults to `1`.|
|`bucket_count`|The number of buckets to divide the minhashes into. Defaults to `512`.|
|`hash_set_size`|The number of minhashes to keep per bucket. Defaults to `1`.|
|`with_rotation`|Whether or not to fill empty buckets with the value of the first non-empty bucket to its circular right. Only takes effect if hash_set_size is equal to one. Defaults to `true`if bucket_count is greater than one, else `false`.|



