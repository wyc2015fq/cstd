# ElasticSearch基础 - LC900730的博客 - CSDN博客
2017年11月22日 23:35:09[lc900730](https://me.csdn.net/LC900730)阅读数：91标签：[elasticsearch](https://so.csdn.net/so/search/s.do?q=elasticsearch&t=blog)
个人分类：[elasticsearch](https://blog.csdn.net/LC900730/article/category/7304581)
## 索引
索引是ES对逻辑数据的逻辑存储，所以它可以分为更小的部分。可以将索引看成是关系数据库的表
## 文档
存储在ES中的主要实体叫做文档。一个文档好比一行记录。在ES中相同字段必须相同类型，如所有title字段的文档，title类型必须一致。
## 分片和副本
ES索引是由一个或多个分片组成的，每个文档包含了文档集的一部分。
## ES查询
### 基本查询
- 查询实际数据
### 复合查询
- 如布尔查询，可以合并多个查询
