# LSH︱python实现局部敏感哈希——LSHash（二） - 素质云笔记/Recorder... - CSDN博客





2017年04月20日 18:43:09[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6548








关于局部敏感哈希算法，之前用R语言实现过，但是由于在R中效能太低，于是放弃用LSH来做相似性检索。学了Python发现很多模块都能实现，而且通过随机投影森林让查询数据更快，觉得可以试试大规模应用在数据相似性检索+去重的场景。

> 
私认为，文本的相似性可以分为两类：一类是机械相似性；一类是语义相似性。  

  机械相似性代表着，两个文本内容上的相关程度，比如“你好吗”和“你好”的相似性，纯粹代表着内容上字符是否完全共现，应用场景在：文章去重；  

  语义相似性代表着，两个文本语义上的相似程度，比如“苹果”和“公司”的相似性，本篇不做这一讨论
之前写关于R语言实现的博客： 
[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（一，基本原理）](http://blog.csdn.net/sinat_26917383/article/details/52451028)
[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（二，textreuse介绍）](http://blog.csdn.net/sinat_26917383/article/details/52457252)
机械相似性python版的四部曲： 
[LSH︱python实现局部敏感随机投影森林——LSHForest/sklearn（一）](http://blog.csdn.net/sinat_26917383/article/details/70243066)
[LSH︱python实现局部敏感哈希——LSHash（二）](http://blog.csdn.net/sinat_26917383/article/details/70285678)
[相似性︱python+opencv实现pHash算法+hamming距离（simhash）（三）](http://blog.csdn.net/sinat_26917383/article/details/70287521)
[LSH︱python实现MinHash-LSH及MinHash LSH Forest——datasketch（四）](http://blog.csdn.net/sinat_26917383/article/details/70332325)

.
# 一、LSHash介绍

官方文档：[https://github.com/kayzhu/LSHash](https://github.com/kayzhu/LSHash)

这个模块是python里面专门来实现LSH，比较特别的有：
- 通过使用NumPy数组的计算将高维数据快速哈希。
- 支持多哈希索引
- 内置通常的距离函数/排名输出

安装
`pip install lshash`
.

## 1、主函数
`LSHash(hash_size, input_dim, num_of_hashtables=1, storage=None, matrices_filename=None, overwrite=False)`- hash_size:

二进制散列的长度。
- input_dim:

输入变量的维度
- num_hashtables = 1:

(optional)多索引查询时哈希表数量。
- 
storage = None: 

Specify the name of the storage to be used for the index storage. Options include “redis”. 

存储- 
matrices_filename = None:


文档存储，以.npz方式
- overwrite = False:

是否覆盖原文件 

.

## 2、函数：lsh.index(input_point, extra_data=None)
- input_point:

输入
- extra_data = None:

(optional) Extra data to be added along with the input_point. 

.

## 3、查询函数：lsh.query(query_point, num_results=None, distance_func=”euclidean”):
- query_point:

The query data point is an array or tuple of numbers of input_dim.
- num_results = None:

(optional) The number of query results to return in ranked order. By default all results will be returned.
- distance_func = “euclidean”:

强调：这里的距离函数有，euclidean，true_euclidean，centred_euclidean，cosine，l1norm，hamming等多种。默认是euclidean 距离。

.

# 二、简单案例

```python
>>> from lshash import LSHash

>>> lsh = LSHash(6, 8)
>>> lsh.index([1,2,3,4,5,6,7,8])
>>> lsh.index([2,3,4,5,6,7,8,9])
>>> lsh.index([10,12,99,1,5,31,2,3])
>>> lsh.query([1,2,3,4,5,6,7,7])
[((1, 2, 3, 4, 5, 6, 7, 8), 1.0),
 ((2, 3, 4, 5, 6, 7, 8, 9), 11)]
```

LSHash(6, 8)，创建一个8维度，6-bits的哈希，初始化一下； 

 lsh.index,数据载入的过程； 

 lsh.query，查询。 

这里可以选择自己喜欢的距离。









