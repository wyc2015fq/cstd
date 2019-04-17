# leveldb 是什么东东？ - 家家的专栏 - CSDN博客





2014年05月25日 13:27:51[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1478
个人分类：[图像处理算法																[c++常见知识](https://blog.csdn.net/yihaizhiyan/article/category/715110)](https://blog.csdn.net/yihaizhiyan/article/category/715109)








**leveldb**

A fast and lightweight Key/Value **database library** by Google. (Leveldb是一个google实现的非常高效的kv数据库)




特点：

1、Keys and values are arbitrary byte arrays.

2、Data is stored sorted by key.

3、Callers can provide a custom comparison function to override the sort order.

4、The basic opteration are Put (key and value), Get (key), Delete (key). 

5 、Multiple changes can be made in one atomic batch.

6、Users can create a transient snapshot to get a consistent view of data

7、Forward and backward iteration is supported over the data

8、Data is automatically compressed using the Snappy compression library.

9、External activity is relayed through a virtual interface so users can customize the operatting system interations.

10、Detailed documentation about how to use the library is included with the source code.




局限性：

不是SQL数据库，没有关联数据模型，不支持索引和查询

单进程




参考网址：[http://code.google.com/p/leveldb/](http://code.google.com/p/leveldb/)







