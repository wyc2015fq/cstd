# Elasticsearch DSL中Query与Filter的不同 - z69183787的专栏 - CSDN博客
2017年05月04日 17:11:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：470
> 
Elasticsearch支持很多查询方式，其中一种就是DSL，它是把请求写在JSON里面，然后进行相关的查询。
## 举个DSL例子
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
GET _search
{
  "query": { 
    "bool": { 
      "must": [
        { "match": { "title":   "Search"        }}, 
        { "match": { "content": "Elasticsearch" }}  
      ],
      "filter": [ 
        { "term":  { "status": "published" }}, 
        { "range": { "publish_date": { "gte": "2015-01-01" }}} 
      ]
    }
  }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
## 查询的种类
Elasticsearch中的DSL主要由两部分组成：
**Leaf query Cluase 暂且叫做叶查询子句吧**
这种查询可以单独使用，针对某一特定的字段查询特定的值，比如match、term、range等
**Compound query Cluase复合查询子句**
这种查询配合其他的叶查询或者复合查询，用于在逻辑上，组成更为复杂的查询，比如bool
**查询虽然包含这两种，但是查询在不同的执行环境下，操作还是不一样的。**
## Query与Filter
查询在Query查询上下文和Filter过滤器上下文中，执行的操作是不一样的：
**查询上下文：**
在查询上下文中，查询会回答这个问题——**“这个文档匹不匹配这个查询，它的相关度高么？”**
如何验证匹配很好理解，如何计算相关度呢？之前说过，ES中索引的数据都会存储一个_score分值，分值越高就代表越匹配。另外关于某个搜索的分值计算还是很复杂的，因此也需要一定的时间。
查询上下文 是在 使用query进行查询时的执行环境，比如使用search的时候。
**过滤器上下文：**
在过滤器上下文中，查询会回答这个问题——**“这个文档匹不匹配？”**
答案很简单，是或者不是。它不会去计算任何分值，也不会关心返回的排序问题，因此效率会高一点。
过滤上下文 是在使用filter参数时候的执行环境，比如在bool查询中使用Must_not或者filter
另外，经常使用过滤器，ES会自动的缓存过滤器的内容，这对于查询来说，会提高很多性能。
## 总结
1 查询上下文中，查询操作不仅仅会进行查询，还会计算分值，用于确定相关度；在过滤器上下文中，查询操作仅判断是否满足查询条件
2 过滤器上下文中，查询的结果可以被缓存。
## query与filter
1、区别如下：
query是要相关性评分的，filter不要；
query结果无法缓存，filter可以。
所以，选择参考：
1、全文搜索、评分排序，使用query；
2、是非过滤，精确匹配，使用filter。
## filter缓存
默认情况下，并不是所有的filter都能用缓存。常用的比如term、terms、prefix、range、bool等filter，其过滤结果明确，也容易设置缓存，ES就对这几个默认开启了filter cache工嗯呢该。而复杂一些比如：geo、script等filter，从filddata数据到过滤结果还需要进行一些列计算的，ES默认是不开启filter cache的。而想and、not、or这几个关系型filter，也是不开启的。如果想要强制开启这些默认没有的filter
 cache，需要在请求的JSON中带上“cache”：true参数。
## query_then_fetch
ESd对请求的处理过程，是有不同类型的，默认的叫query_then_fetch。在这种情况下，各数据节点处理检索请求后，返回的是只包含文档id和相关性分之的结果，这一段处理，叫做query阶段；汇聚到这份结果后，按照分值排序，得到一个全集群最终需要的文档id，再向对应节点点发送一次文档获取请求，拿到文档内容，这一段处理叫做fetch阶段。两阶段都结束后才返回响应。
DFS_query_then_fetch类型，提高小数据量时的精确度；query_then_fetch类型在明确routing时可以省略一个数据来回；count类型，在不关心文档内容只需要计数时省略query极端；sacn类型批量获取数据省略query阶段（在reindex时就是使用这种类型）。
