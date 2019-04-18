# ElasticSearch的 Query DSL 和 Filter DSL - z69183787的专栏 - CSDN博客
2017年04月12日 10:41:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3170
Elasticsearch支持很多查询方式，其中一种就是DSL，它是把请求写在JSON里面，然后进行相关的查询。 
# Query DSL 与 Filter DSL
DSL查询语言中存在两种：查询DSL（query DSL）和过滤DSL（filter DSL）。
它们两个的区别如下图：
![image](http://images2015.cnblogs.com/blog/120296/201603/120296-20160318164439303-146810557.png)
## query DSL
在查询上下文中，查询会回答这个问题——“这个文档匹不匹配这个查询，它的相关度高么？”
如何验证匹配很好理解，如何计算相关度呢？ES中索引的数据都会存储一个_score分值，分值越高就代表越匹配。另外关于某个搜索的分值计算还是很复杂的，因此也需要一定的时间。
查询上下文 是在 使用query进行查询时的执行环境，比如使用search的时候。
一些query的场景：
- 与full text search的匹配度最高
- 包含run单词，如果包含这些单词：runs、running、jog、sprint，也被视为包含run单词
- 包含quick、brown、fox。这些词越接近，这份文档的相关性就越高
## filter DSL
在过滤器上下文中，查询会回答这个问题——“这个文档匹不匹配？”
答案很简单，是或者不是。它不会去计算任何分值，也不会关心返回的排序问题，因此效率会高一点。
过滤上下文 是在使用filter参数时候的执行环境，比如在bool查询中使用Must_not或者filter
另外，经常使用过滤器，ES会自动的缓存过滤器的内容，这对于查询来说，会提高很多性能。
一些过滤的情况：
- 创建日期是否在2013-2014年间？
- status字段是否为published？
- lat_lon字段是否在某个坐标的10公里范围内？
我们这业务上关于elasticsearch的查询语法基本都是用query filtered方式进行的，我也推荐大家直接用这样的方法。should ，must_not, must 都是列表，列表里面可以写多个条件。 这里再啰嗦一句，如果你的查询是范围和类型比较粗大的，用filter ！ 如果是那种精准的，就用query来查询。 
{
   "bool":{
     "should":[],   #相当于OR条件
     "must_not":[],  #必须匹配的条件，这里的条件都会被反义
     "must":[]        #必须要有的
  }
}
参考： [http://www.cnblogs.com/xing901022/p/4975931.html](http://www.cnblogs.com/xing901022/p/4975931.html)
下图的查询就是一个组合查询， 既有 filter 也有 query：
![image](http://images2015.cnblogs.com/blog/120296/201603/120296-20160318164440834-1630357453.png)
[http://xiaorui.cc/2015/11/09/elasticsearch%E7%9A%84%E6%9F%A5%E8%AF%A2%E5%99%A8query%E4%B8%8E%E8%BF%87%E6%BB%A4%E5%99%A8filter%E7%9A%84%E5%8C%BA%E5%88%AB/](http://xiaorui.cc/2015/11/09/elasticsearch%E7%9A%84%E6%9F%A5%E8%AF%A2%E5%99%A8query%E4%B8%8E%E8%BF%87%E6%BB%A4%E5%99%A8filter%E7%9A%84%E5%8C%BA%E5%88%AB/)
上面文章提供了一个测试例子。
- query语句查询结果,第一次查询用了300ms,第二次用了280ms.
- filter查询出来的结果,第一次查询时间是280ms,第二次130ms。
具体如何写 查询和 过滤并存的请看下面这篇文章：
查询与过滤条件的合并 
[http://es.xiaoleilu.com/054_Query_DSL/75_Queries_with_filters.html](http://es.xiaoleilu.com/054_Query_DSL/75_Queries_with_filters.html)
比如说我们有这样一条查询语句，获取右键内容中带“`business opportunity`”  的：
{ 
    "match": { 
        "email": "business opportunity" 
    } 
}
然后我们想要让这条语句加入 term 过滤，只在收信箱中匹配邮件：
{ 
    "term": { 
        "folder": "inbox" 
    } 
}
search API中只能包含 query 语句，所以我们需要用 filtered 来同时包含 "query" 和 "filter" 子句：
{ 
    "filtered": { 
        "query":  { "match": { "email": "business opportunity" }}, 
        "filter": { "term":  { "folder": "inbox" }} 
    } 
}
我们在外层再加入 query 的上下文关系： 
GET /_search 
{ 
    "query": { 
        "filtered": { 
            "query":  { "match": { "email": "business opportunity" }}, 
            "filter": { "term": { "folder": "inbox" }} 
        } 
    } 
} 
更多参考： [http://es.xiaoleilu.com/054_Query_DSL/75_Queries_with_filters.html](http://es.xiaoleilu.com/054_Query_DSL/75_Queries_with_filters.html)
参考：
[http://www.fanli7.net/a/bianchengyuyan/C__/20150526/501179.html](http://www.fanli7.net/a/bianchengyuyan/C__/20150526/501179.html)
2、
Query查询器 与 Filter 过滤器
尽管我们之前已经涉及了查询DSL，然而实际上存在两种DSL：查询DSL（query DSL）和过滤DSL（filter DSL）。
过滤器（filter）通常用于过滤文档的范围，比如某个字段是否属于某个类型，或者是属于哪个时间区间
* 创建日期是否在2014-2015年间？
* status字段是否为success？
* lat_lon字段是否在某个坐标的10公里范围内？
查询器（query）的使用方法像极了filter，但query更倾向于更准确的查找。
* 与full text search的匹配度最高
* 正则匹配
* 包含run单词，如果包含这些单词：runs、running、jog、sprint，也被视为包含run单词
* 包含quick、brown、fox。这些词越接近，这份文档的相关性就越高
查询器会计算出每份文档对于某次查询有多相关（relevant），然后分配文档一个相关性分数：_score。而这个分数会被用来对匹配了的文档进行相关性排序。相关性概念十分适合全文搜索（full-text search），这个很难能给出完整、“正确”答案的领域。
query filter在性能上对比：filter是不计算相关性的，同时可以cache。因此，filter速度要快于query。
下面是使用query语句查询的结果,第一次查询用了300ms,第二次用了280ms.
```python
{
    "size": 1,
    "query": {
        "bool": {
            "must": [
                {
                    "terms": {
                        "keyword": [
                            "手机",
                            "iphone"
                        ]
                    }
                },
                {
                    "range": {
                        "cdate": {
                            "gt": "2015-11-09T11:00:00"
                        }
                    }
                }
            ]
        }
    }
}
 
{
    "took": 51,
    "timed_out": false,
    "_shards": {
        "total": 30,
        "successful": 30,
        "failed": 0
    },
    "hits": {
        "total": 6818,
        "max_score": 0,
        "hits": []
    }
}
```
下面是使用filter查询出来的结果,第一次查询时间是280ms,第二次130ms…. 速度确实快了不少，也证明filter走了cache缓存。 但是如果我们对比下命中的数目，query要比filter要多一点，换句话说，更加的精准。 
```python
{
    "size": 0,
    "filter": {
        "bool": {
            "must": [
                {
                    "terms": {
                        "keyword": [
                            "手机",
                            "iphone"
                        ]
                    }
                },
                {
                    "range": {
                        "cdate": {
                            "gt": "2015-11-09T11:00:00"
                        }
                    }
                }
            ]
        }
    }
}
{
    "took": 145,
    "timed_out": false,
    "_shards": {
        "total": 30,
        "successful": 30,
        "failed": 0
    },
    "hits": {
        "total": 6804,
        "max_score": 0,
        "hits": []
    }
}
```
如果你想同时使用query和filter查询的话，需要使用 {query:{filtered：{}}} 来包含这两个查询语法。他们的好处是，借助于filter的速度可以快速过滤出文档，然后再由query根据条件来匹配。
```python
"query": {
        "filtered": {
            "query":  { "match": { "email": "business opportunity" }},
            "filter": { "term": { "folder": "inbox" }}
        }
    }
}
{   "size":0,    
    "query": {
        "filtered": {
            "query": {
                "bool": {
                    "should": [],
                    "must_not": [
                       
                    ],
                    "must": [
                        {
                         "term": {
                             
                                "channel_name":"微信自媒体微信"
                            }
                        }
                   
                    ]
                }
            }
        }，
        "filter":{
            "range": {
                "idate": {
                    "gte": "2015-09-01T00:00:00",
                    "lte": "2015-09-10T00:00:00"
                    
                    }
                }
        }
    }
}
```
我们这业务上关于elasticsearch的查询语法基本都是用query filtered方式进行的，我也推荐大家直接用这样的方法。should ，must_not, must 都是列表，列表里面可以写多个条件。 这里再啰嗦一句，如果你的查询是范围和类型比较粗大的，用filter ！ 如果是那种精准的，就用query来查询。 
{
   ”bool”:{
     ”should”:[],   #相当于OR条件
     ”must_not”:[],  #必须匹配的条件，这里的条件都会被反义
     ”must”:[]        #必须要有的
  }
}
END..
