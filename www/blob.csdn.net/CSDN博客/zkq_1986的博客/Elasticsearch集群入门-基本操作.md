# Elasticsearch集群入门-基本操作 - zkq_1986的博客 - CSDN博客





2016年07月21日 08:35:10[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：303








## 1 启动集群

转到bin目录，运行如下命令：

./elasticsearch

## 关掉Elasticsearch

```
如果节点是连接到控制台，按下Ctrl+C。
第二种选择是通过发送TERM信号杀掉服务器进程（参考Linux上的kill命令和Windows上的任务管理器）。
第三种方法是使用REST API。
```

对于第三种方法可以执行以下命令来关掉整个集群：
`curl -XPOST http://localhost:9200/_cluster/nodes/_shutdown`
为关闭单一节点，假如节点标识符是BlrmMvBdSKiCeYGsiHijdg，可以执行下面的命令：
`curl –XPOST http://localhost:9200/_cluster/nodes/BlrmMvBdSKiCeYGsiHijdg/_shutdown`
## 2 全文搜索

### 2.1  创建文档

```
curl -XPOST 'localhost:9200/books/es/1' -d '{"title":"Elasticsearch Server cas", "published": 2013}' 
curl -XPOST 'localhost:9200/books/es/2' -d '{"title":"Mastering Elasticsearch", "published": 2013}' 
curl -XPOST 'localhost:9200/books/es/3' -d '{"title":"Mastering Elasticsearch asb", "published": 2013}' 
curl -XPOST 'localhost:9200/books/es/4' -d '{"title":"Mastering Elasticsearch cda", "published": 2013}' 
curl -XPOST 'localhost:9200/books/solr/1' -d '{"title":"Apache Solr 4 Cookbook", "published": 2012}'
```

### 2.2  检索（查询）

2.1 查询所有的 index, type:
`$ curl localhost:9200/_search?pretty=true`
2.2 查询某个index下所有的type:
`$ curl localhost:9200/films/_search`
2.3 查询某个index 下， 某个 type下所有的记录：
`$ curl localhost:9200/films/md/_search?pretty=true`
2.4 带有参数的查询： 
`$ curl localhost:9200/films/md/_search?q=tag:good`
{“took”:7,”timed_out”:false,”_shards”:{“total”:5,”successful”:5,”failed”:0},”hits”:{“total”:2,”max_score”:1.0,”hits”:[{“_index”:”film”,”_type”:”md”,”_id”:”2”,”_score”:1.0, “_source” : 

{ “name”:”hei yi ren”, “tag”: “good”}},{“_index”:”film”,”_type”:”md”,”_id”:”1”,”_score”:0.30685282, “_source” : 

{ “name”:”ma da jia si jia”, “tag”: “good”}}]}}
**精确检索**

假设想找到books索引中title字段包含elasticsearch asb一词的所有文档，可以运行以下查询：
`curl -XGET 'localhost:9200/books/_search?pretty&q=title:elasticsearch asb`
**模糊检索**

使用JSON参数的查询： （注意 query 和 term 关键字）

```
$ curl -XGET localhost:9200/books/_search -d '
{"query": {"match": {"title": "elasticsearch a" }}}'
```

### 2.3  更新update
`$ curl -XPUT localhost:9200/films/md/1 -d { ...(data)... }`
### 2.4  删除
`$ curl -XDELETE localhost:9200/films `
# 3 ui 查看

[http://ip:9200/_plugin/head/](http://ip:9200/_plugin/head/)

注：360浏览器不支持，火狐可以

## 3.1 查询

前缀查询 
![这里写图片描述](https://img-blog.csdn.net/20160816191721854)

{“query”:{“prefix”:{“name”:”r”}}} 

结果：
```
{
    "took": 2,
    "timed_out": false,
    "_shards": {
        "total": 5,
        "successful": 5,
        "failed": 0
    },
    "hits": {
        "total": 1,
        "max_score": 1,
        "hits": [
            {
                "_index": "clients",
                "_type": "client",
                "_id": "4",
                "_score": 1,
                "_source": {
                    "id": "4",
                    "name": "Rob"
                }
            }
        ]
    }
}
```

下面就通过match方式查询特定字段的特定内容，比如查询余额为20的账户信息：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": { "match": { "account_number": 20 } }
}'
```

查询地址为mill的信息：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": { "match": { "address": "mill" } }
}'
```

查询地址为mill或者lane的信息：

curl -XPOST ‘localhost:9200/bank/_search?pretty’ -d ’ 

{ 

  “query”: { “match”: { “address”: “mill lane” } } 

}’ 

如果我们想要返回同时包含mill和lane的，可以通过match_phrase查询：
```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": { "match_phrase": { "address": "mill lane" } }
}'
```

ES提供了bool查询，可以把很多小的查询组成一个更为复杂的查询，比如查询同时包含mill和lane的文档：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": {
    "bool": {
      "must": [
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}'
```

修改bool参数，可以改为查询包含mill或者lane的文档：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": {
    "bool": {
      "should": [
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}'
```

也可以改写为must_not，排除包含mill和lane的文档：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": {
    "bool": {
      "must_not": [
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}'
```

bool查询可以同时使用must, should, must_not组成一个复杂的查询：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": {
    "bool": {
      "must": [
        { "match": { "age": "40" } }
      ],
      "must_not": [
        { "match": { "state": "ID" } }
      ]
    }
  }
}'
```

### 3.2 过滤查询

之前说过score字段指定了文档的分数，使用查询会计算文档的分数，最后通过分数确定哪些文档更相关，返回哪些文档。

有的时候我们可能对分数不感兴趣，就可以使用filter进行过滤，它不会去计算分值，因此效率也就更高一些。

filter过滤可以嵌套在bool查询内部使用，比如想要查询在2000-3000范围内的所有文档，可以执行下面的命令：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "query": {
    "bool": {
      "must": { "match_all": {} },
      "filter": {
        "range": {
          "balance": {
            "gte": 20000,
            "lte": 30000
          }
        }
      }
    }
  }
}'
```

ES除了上面介绍过的范围查询range、match_all、match、bool、filter还有很多其他的查询方式，这里就先不一一说明了。

## 4 聚合

聚合提供了用户进行分组和数理统计的能力，可以把聚合理解成SQL中的GROUP BY和分组函数。在ES中，你可以在一次搜索查询的时间内，即完成搜索操作也完成聚合操作，这样就降低了多次使用REST API造成的网络开销。

下面就是通过terms聚合的简单样例：

```
curl -XPOST 'localhost:9200/bank/_search?pretty' -d '
{
  "size": 0,
  "aggs": {
    "group_by_state": {
      "terms": {
        "field": "state"
      }
    }
  }
}'
```

它类似于SQL中的下面的语句：

SELECT state, COUNT(*) FROM bank GROUP BY state ORDER BY COUNT(*) DESC

## 查询全局索引

curl localhost:9200/_cat/indices/real_index_*?v

## 更新局部数据

curl -XPOST localhost:9200/index/type/id/_update -d’ 

{“doc”:{“name”:”12”} 

}’






