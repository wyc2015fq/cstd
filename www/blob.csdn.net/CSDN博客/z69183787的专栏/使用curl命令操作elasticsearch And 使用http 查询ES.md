# 使用curl命令操作elasticsearch And 使用http 查询ES - z69183787的专栏 - CSDN博客
2017年04月12日 13:59:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13793
第一：_cat系列 
_cat系列提供了一系列查询elasticsearch集群状态的接口。你可以通过执行 
curl -XGET localhost:9200/_cat 
获取所有_cat系列的操作 
=^.^= 
/_cat/allocation 
/_cat/shards 
/_cat/shards/{index} 
/_cat/master 
/_cat/nodes 
/_cat/indices 
/_cat/indices/{index} 
/_cat/segments 
/_cat/segments/{index} 
/_cat/count 
/_cat/count/{index} 
/_cat/recovery 
/_cat/recovery/{index} 
/_cat/health 
/_cat/pending_tasks 
/_cat/aliases 
/_cat/aliases/{alias} 
/_cat/thread_pool 
/_cat/plugins 
/_cat/fielddata 
/_cat/fielddata/{fields} 
你也可以后面加一个v，让输出内容表格显示表头，举例
name component version type url 
Prometheus analysis-mmseg NA j 
Prometheus analysis-pinyin NA j 
Prometheus analysis-ik NA j 
Prometheus analysis-ik NA j 
Prometheus analysis-smartcn 2.1.0 j 
Prometheus segmentspy NA s /_plugin/segmentspy/ 
Prometheus head NA s /_plugin/head/ 
Prometheus bigdesk NA s /_plugin/bigdesk/ 
Xandu analysis-ik NA j 
Xandu analysis-pinyin NA j 
Xandu analysis-mmseg NA j 
Xandu analysis-smartcn 2.1.0 j 
Xandu head NA s /_plugin/head/ 
Xandu bigdesk NA s /_plugin/bigdesk/ 
Onyxx analysis-ik NA j 
Onyxx analysis-mmseg NA j 
Onyxx analysis-smartcn 2.1.0 j 
Onyxx analysis-pinyin NA j 
Onyxx head NA s /_plugin/head/ 
Onyxx bigdesk NA s /_plugin/bigdesk/ 
第二：_cluster系列 
1、查询设置集群状态 
curl -XGET localhost:9200/_cluster/health?pretty=true 
pretty=true表示格式化输出 
level=indices 表示显示索引状态 
level=shards 表示显示分片信息 
2、curl -XGET localhost:9200/_cluster/stats?pretty=true 
显示集群系统信息，包括CPU JVM等等 
3、curl -XGET localhost:9200/_cluster/state?pretty=true 
集群的详细信息。包括节点、分片等。 
3、curl -XGET localhost:9200/_cluster/pending_tasks?pretty=true 
获取集群堆积的任务 
3、修改集群配置 
举例：
curl -XPUT localhost:9200/_cluster/settings -d ‘{ 
“persistent” : { 
“discovery.zen.minimum_master_nodes” : 2 
} 
}’ 
transient 表示临时的，persistent表示永久的 
4、curl -XPOST ‘localhost:9200/_cluster/reroute’ -d ‘xxxxxx’ 
对shard的手动控制，参考[http://zhaoyanblog.com/archives/687.html](http://zhaoyanblog.com/archives/687.html)
5、关闭节点 
关闭指定192.168.1.1节点 
curl -XPOST ‘[http://192.168.1.1:9200/_cluster/nodes/_local/_shutdown](http://192.168.1.1:9200/_cluster/nodes/_local/_shutdown)’ 
curl -XPOST ‘[http://localhost:9200/_cluster/nodes/192.168.1.1/_shutdown](http://localhost:9200/_cluster/nodes/192.168.1.1/_shutdown)’ 
关闭主节点 
curl -XPOST ‘[http://localhost:9200/_cluster/nodes/_master/_shutdown](http://localhost:9200/_cluster/nodes/_master/_shutdown)’ 
关闭整个集群 
$ curl -XPOST ‘[http://localhost:9200/_shutdown?delay=10s](http://localhost:9200/_shutdown?delay=10s)’ 
$ curl -XPOST ‘[http://localhost:9200/_cluster/nodes/_shutdown](http://localhost:9200/_cluster/nodes/_shutdown)’ 
$ curl -XPOST ‘[http://localhost:9200/_cluster/nodes/_all/_shutdown](http://localhost:9200/_cluster/nodes/_all/_shutdown)’ 
delay=10s表示延迟10秒关闭
第三：_nodes系列 
1、查询节点的状态 
curl -XGET ‘[http://localhost:9200/_nodes/stats?pretty=true](http://localhost:9200/_nodes/stats?pretty=true)’ 
curl -XGET ‘[http://localhost:9200/_nodes/192.168.1.2/stats?pretty=true](http://localhost:9200/_nodes/192.168.1.2/stats?pretty=true)’ 
curl -XGET ‘[http://localhost:9200/_nodes/process](http://localhost:9200/_nodes/process)’ 
curl -XGET ‘[http://localhost:9200/_nodes/_all/process](http://localhost:9200/_nodes/_all/process)’ 
curl -XGET ‘[http://localhost:9200/_nodes/192.168.1.2,192.168.1.3/jvm,process](http://localhost:9200/_nodes/192.168.1.2,192.168.1.3/jvm,process)’ 
curl -XGET ‘[http://localhost:9200/_nodes/192.168.1.2,192.168.1.3/info/jvm,process](http://localhost:9200/_nodes/192.168.1.2,192.168.1.3/info/jvm,process)’ 
curl -XGET ‘[http://localhost:9200/_nodes/192.168.1.2,192.168.1.3/_all](http://localhost:9200/_nodes/192.168.1.2,192.168.1.3/_all)
curl -XGET ‘[http://localhost:9200/_nodes/hot_threads](http://localhost:9200/_nodes/hot_threads)
第四：索引操作 
1、获取索引 
curl -XGET ‘[http://localhost:9200/](http://localhost:9200/){index}/{type}/{id}’ 
2、索引数据 
curl -XPOST ‘[http://localhost:9200/](http://localhost:9200/){index}/{type}/{id}’ -d’{“a”:”avalue”,”b”:”bvalue”}’ 
3、删除索引 
curl -XDELETE ‘[http://localhost:9200/](http://localhost:9200/){index}/{type}/{id}’ 
4、设置mapping
curl -XPUT [http://localhost:9200/](http://localhost:9200/){index}/{type}/_mapping -d ‘{ 
“{type}” : { 
“properties” : { 
“date” : { 
“type” : “long” 
}, 
“name” : { 
“type” : “string”, 
“index” : “not_analyzed” 
}, 
“status” : { 
“type” : “integer” 
}, 
“type” : { 
“type” : “integer” 
} 
} 
} 
}’ 
5、获取mapping 
curl -XGET [http://localhost:9200/](http://localhost:9200/){index}/{type}/_mapping 
6、搜索
curl -XGET ‘[http://localhost:9200/](http://localhost:9200/){index}/{type}/_search’ -d ‘{ 
“query” : { 
“term” : { “user” : “kimchy” } //查所有 “match_all”: {} 
}, 
“sort” : [{ “age” : {“order” : “asc”}},{ “name” : “desc” } ], 
“from”:0, 
“size”:100 
} 
curl -XGET ‘[http://localhost:9200/](http://localhost:9200/){index}/{type}/_search’ -d ‘{ 
“filter”: {“and”:{“filters”:[{“term”:{“age”:”123”}},{“term”:{“name”:”张三”}}]}, 
“sort” : [{ “age” : {“order” : “asc”}},{ “name” : “desc” } ], 
“from”:0, 
“size”:100 
}
http 查询方式 
```
Map<String, Object> allObMap = new HashMap<String, Object>(); 
//查ES 
StringBuffer sbUrl = new StringBuffer(); 
sbUrl.append("http://").append(Config.getInstance().getESSearch()).append("/") 
.append(PrepareForInsertES.getIndex(getlastHourTime(startTime))).append("/") 
.append(PrepareForInsertES.TYPE).append("/").append("_search"); 
// url 格式 "http://192.168.10.20:9200/acceptor-bilog-2017-01-11/bi/_search"; 
// param 格式 {\"query\": {\"term\": {\"time\": 1484096400000}}} ; 
// "{ \"query\": { \"range\": { \"time\": { \"gte\" :1484096400000, \"lte\":1484096500000 } } } }"; 
StringBuffer sbParam = new StringBuffer(); 
sbParam.append("{\"query\": {\"term\": {\"time\":").append(startTime).append("}}}");
```
```
StringBuffer result = HttpUtil.submitPost(sbUrl.toString(),sbParam.toString());
```
[ES查询term的用法](http://blog.csdn.net/lijuqi/article/details/54314138)
1、term 的用法 
term检索，如果content分词后含有中国这个token，就会检索到
curl -XPOST [http://192.168.1.101:9200/index/fulltext/_search](http://192.168.1.101:9200/index/fulltext/_search) -d’ 
{“query” : { “term” : { “content” : “中国” }}}’
2、querystring的用法 
querystring检索，它会先把”中国美国“分词成中国、美国分别去检索，然后最后默认是OR的关系
curl -XPOST [http://192.168.22.161:9200/index/fulltext/_search](http://192.168.22.161:9200/index/fulltext/_search) -d’ 
{ 
“query” : { 
“query_string” : { 
“default_field” : “content”, 
“query” : “中国美国” 
} 
} 
}’
3、 
你也可以明显的写成 “query” : “中国 AND 美国” 
或者 “query” : “中国 OR 美国” 
如果你把查询条件加上双引号 “query” : “\”中国美国\”” 便类似[MySQL](http://lib.csdn.net/base/mysql)里的like的效果 
4、 
[Java](http://lib.csdn.net/base/javase)客户端 
–总结– 
java程序都有对应的类和方法。创建索引和设置mapping,这里就不赘述了，主要是检索： 
4.1 term搜索主要是用： 
QueryBuilders.termQuery(“content”, “中国”); 
4.2 querystring搜索使用： 
QueryStringQueryBuilder queryString = new QueryStringQueryBuilder(“中国 OR 美国”); 
queryString.field(“content”);
5、query 和 filter 的区别请看： [http://www.cnblogs.com/ghj1976/p/5292740.html](http://www.cnblogs.com/ghj1976/p/5292740.html)
Filter DSL
term 过滤 
term主要用于精确匹配哪些值，比如数字，日期，布尔值或 not_analyzed 的字符串(未经分析的文本数据类型)：
{ “term”: { “age”: 26 }} 
{ “term”: { “date”: “2014-09-01” }} 
{ “term”: { “public”: true }} 
{ “term”: { “tag”: “full_text” }}
完整的例子， hostname 字段完全匹配成 saaap.wangpos.com 的数据：
{ 
“query”: { 
“term”: { 
“hostname”: “saaap.wangpos.com” 
} 
} 
}
terms 过滤 
terms 跟 term 有点类似，但 terms 允许指定多个匹配条件。 如果某个字段指定了多个值，那么文档需要一起去做匹配：
{ 
“terms”: { 
“tag”: [ “search”, “full_text”, “nosql” ] 
} 
}
完整的例子，所有http的状态是 302 、304 的， 由于ES中状态是数字类型的字段，所有这里我们可以直接这么写。：
{ 
“query”: { 
“terms”: { 
“status”: [ 
304, 
302 
] 
} 
} 
}
range 过滤 
range过滤允许我们按照指定范围查找一批数据：
{ 
“range”: { 
“age”: { 
“gte”: 20, 
“lt”: 30 
} 
} 
}
范围操作符包含：
gt :: 大于 
gte:: 大于等于 
lt :: 小于 
lte:: 小于等于 
一个完整的例子， 请求页面耗时大于1秒的数据，upstream_response_time 是 nginx 日志中的耗时，ES中是数字类型。
{ 
“query”: { 
“range”: { 
“upstream_response_time”: { 
“gt”: 1 
} 
} 
} 
}
exists 和 missing 过滤 
exists 和 missing 过滤可以用于查找文档中是否包含指定字段或没有某个字段，类似于SQL语句中的IS_NULL条件.
{ 
“exists”: { 
“field”: “title” 
} 
}
这两个过滤只是针对已经查出一批数据来，但是想区分出某个字段是否存在的时候使用。
bool 过滤 
bool 过滤可以用来合并多个过滤条件查询结果的布尔逻辑，它包含一下操作符：
must :: 多个查询条件的完全匹配,相当于 and。 
must_not :: 多个查询条件的相反匹配，相当于 not。 
should :: 至少有一个查询条件匹配, 相当于 or。 
这些参数可以分别继承一个过滤条件或者一个过滤条件的数组：
{ 
“bool”: { 
“must”: { “term”: { “folder”: “inbox” }}, 
“must_not”: { “term”: { “tag”: “spam” }}, 
“should”: [ 
{ “term”: { “starred”: true }}, 
{ “term”: { “unread”: true }} 
] 
} 
}
Query DSL
match_all 查询 
可以查询到所有文档，是没有查询条件下的默认语句。
{ 
“match_all”: {} 
}
此查询常用于合并过滤条件。 比如说你需要检索所有的邮箱,所有的文档相关性都是相同的，所以得到的_score为1.
match 查询 
match查询是一个标准查询，不管你需要全文本查询还是精确查询基本上都要用到它。
如果你使用 match 查询一个全文本字段，它会在真正查询之前用分析器先分析match一下查询字符：
{ 
“match”: { 
“tweet”: “About Search” 
} 
}
如果用match下指定了一个确切值，在遇到数字，日期，布尔值或者not_analyzed 的字符串时，它将为你搜索你给定的值：
{ “match”: { “age”: 26 }} 
{ “match”: { “date”: “2014-09-01” }} 
{ “match”: { “public”: true }} 
{ “match”: { “tag”: “full_text” }}
提示： 做精确匹配搜索时，你最好用过滤语句，因为过滤语句可以缓存数据。
match查询只能就指定某个确切字段某个确切的值进行搜索，而你要做的就是为它指定正确的字段名以避免语法错误。
multi_match 查询 
multi_match查询允许你做match查询的基础上同时搜索多个字段，在多个字段中同时查一个：
{ 
“multi_match”: { 
“query”: “full text search”, 
“fields”: [ “title”, “body” ] 
} 
}
bool 查询 
bool 查询与 bool 过滤相似，用于合并多个查询子句。不同的是，bool 过滤可以直接给出是否匹配成功， 而bool 查询要计算每一个查询子句的 _score （相关性分值）。
must:: 查询指定文档一定要被包含。 
must_not:: 查询指定文档一定不要被包含。 
should:: 查询指定文档，有则可以为文档相关性加分。 
以下查询将会找到 title 字段中包含 “how to make millions”，并且 “tag” 字段没有被标为 spam。 如果有标识为 “starred” 或者发布日期为2014年之前，那么这些匹配的文档将比同类网站等级高：
{ 
“bool”: { 
“must”: { “match”: { “title”: “how to make millions” }}, 
“must_not”: { “match”: { “tag”: “spam” }}, 
“should”: [ 
{ “match”: { “tag”: “starred” }}, 
{ “range”: { “date”: { “gte”: “2014-01-01” }}} 
] 
} 
}
提示： 如果bool 查询下没有must子句，那至少应该有一个should子句。但是 如果有must子句，那么没有should子句也可以进行查询。
上面内容来自： [http://es.xiaoleilu.com/054_Query_DSL/70_Important_clauses.html](http://es.xiaoleilu.com/054_Query_DSL/70_Important_clauses.html)
ElasticSearch 查询（match和term） 
[http://www.cnblogs.com/yjf512/p/4897294.html](http://www.cnblogs.com/yjf512/p/4897294.html)
wildcards 查询 
使用标准的shell通配符查询
参考： [https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-wildcard-query.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-wildcard-query.html)
以下查询能够匹配包含W1F 7HW和W2F 8HW的文档：
GET /my_index/address/_search 
{ 
“query”: { 
“wildcard”: { 
“postcode”: “W?F*HW” 
} 
} 
}
又比如下面查询 hostname 匹配下面shell通配符的：
{ 
“query”: { 
“wildcard”: { 
“hostname”: “wxopen*” 
} 
} 
}
regexp 查询 
假设您只想匹配以W开头，紧跟着数字的邮政编码。使用regexp查询能够让你写下更复杂的模式：
GET /my_index/address/_search 
{ 
“query”: { 
“regexp”: { 
“postcode”: “W[0-9].+” 
} 
} 
}
这个正则表达式的规定了词条需要以W开头，紧跟着一个0到9的数字，然后是一个或者多个其它字符。
下面例子是所有以 wxopen 开头的正则
{ 
“query”: { 
“regexp”: { 
“hostname”: “wxopen.*” 
} 
} 
}
参考： [https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-regexp-query.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-regexp-query.html)
prefix 查询 
以什么字符开头的，可以更简单地用 prefix，如下面的例子：
{ 
“query”: { 
“prefix”: { 
“hostname”: “wxopen” 
} 
} 
}
参考 ： [https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-prefix-query.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-prefix-query.html)
更多的查询命令，可以看： [https://www.elastic.co/guide/en/elasticsearch/reference/current/term-level-queries.html#term-level-queries](https://www.elastic.co/guide/en/elasticsearch/reference/current/term-level-queries.html#term-level-queries)
短语匹配(Phrase Matching) 
当你需要寻找邻近的几个单词时，你会使用match_phrase查询：
GET /my_index/my_type/_search 
{ 
“query”: { 
“match_phrase”: { 
“title”: “quick brown fox” 
} 
} 
} 
和match查询类似，match_phrase查询首先解析查询字符串来产生一个词条列表。然后会搜索所有的词条， 
但只保留含有了所有搜索词条的文档，并且词条的位置要邻接。一个针对短语quick fox的查询不会匹配 
我们的任何文档，因为没有文档含有邻接在一起的quick和box词条。 
match_phrase查询也可以写成类型为phrase的match查询：
“match”: { 
“title”: { 
“query”: “quick brown fox”, 
“type”: “phrase” 
} 
} 
参考： [http://blog.csdn.net/dm_vincent/article/details/41941659](http://blog.csdn.net/dm_vincent/article/details/41941659)
[es query time gte lte](http://blog.csdn.net/lijuqi/article/details/54341286)
![这里写图片描述](https://img-blog.csdn.net/20170111103341226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqdXFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
{ 
“query”: { 
“range”: { 
“time”: { 
“gte” :1484096300000, 
“lte”:1484096500000 
} 
} 
} 
}
[](http://blog.csdn.net/lijuqi/article/details/54341286#)[](http://blog.csdn.net/lijuqi/article/details/54341286#)[](http://blog.csdn.net/lijuqi/article/details/54341286#)[](http://blog.csdn.net/lijuqi/article/details/54341286#)[](http://blog.csdn.net/lijuqi/article/details/54341286#)[](http://blog.csdn.net/lijuqi/article/details/54341286#)
