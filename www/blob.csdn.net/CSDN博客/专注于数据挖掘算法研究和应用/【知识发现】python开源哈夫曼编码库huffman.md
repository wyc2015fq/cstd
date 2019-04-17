# 【知识发现】python开源哈夫曼编码库huffman - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月27日 09:27:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1328
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、哈夫曼树：




   安装：pip install huffman




   Github地址： https://github.com/nicktimko/huffman

   pypi地址：https://pypi.python.org/pypi/huffman

   源码很值得参考。



2、案例：



```python
# -*- coding: utf-8 -*-
'''
Created on 2017年9月26日

@author: Administrator
'''
import huffman
import collections

t1=huffman.codebook([('A', 2), ('B', 4), ('C', 1), ('D', 1)])
print (t1)
t2=huffman.codebook(collections.Counter('man the stand banana man').items())
print (t2)
```


说明：rovided an iterable of 2-tuples in (symbol, weight) format, generate a Huffman codebook, returned as a dictionary in {symbol: code, ...} format.



3、构造哈夫曼树参考：



```python
#构造哈夫曼树
import heapq
trees=huff_df.values.T.tolist()  #dataframe转化成list
heapq.heapify(trees)
while len(trees)>1:
    rightChild,leftChild=heapq.heappop(trees),heapq.heappop(trees)
    parentNode=(leftChild[0]+rightChild[0],leftChild,rightChild)
    heapq.heappush(trees,parentNode)
print (trees)
```


huff_df是一个dataframe，转化成list，里面是结点及其频率。
            


