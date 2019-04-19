# es 基础操作文档 - sxf_123456的博客 - CSDN博客
2018年05月13日 11:12:11[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：550
PUT 和POST 
PUT 添加数据,需要id,POST 修改数据，不需要id
curl -XPUT'localhost:9200/customer/external/1?pretty' -d '
{
  "name": "John Doe"
}'
`curl -XPOST 'localhost:9200/customer/external?pretty' -d '``{``  "name": "Jane Doe"``}'`
es 更新先删除旧文档，再更新新文档
`curl -XPOST 'localhost:9200/customer/external/1/_update?pretty' -d '``{``  "doc": { "name": "Jane Doe" }``}'`
将ID为1的文档的name字段改成“Jane Doe”的同时，给它加上age字段：
`curl -XPOST 'localhost:9200/customer/external/1/_update?pretty' -d '``{``  "doc": { "name": "Jane Doe", "age": 20 }``}'`
也可以通过使用简单的脚本来进行。这个例子使用一个脚本将age加5：
`curl -XPOST 'localhost:9200/customer/external/1/_update?pretty' -d '``{``  "script" : "ctx._source.age += 5"``}'`
删除单个文档
`curl -XDELETE 'localhost:9200/customer/external/2?pretty'`
一次删除符合某个查询条件的多个文档
删除名字中包含“John” 的所有的客户：
`curl -XDELETE 'localhost:9200/customer/external/_query?pretty' -d '``{``  "query": { "match": { "name": "John" } }``}'`
Elasticsearch也提供了操作的批量处理功能，它通过使用_bulk   API实现
作为一个快速的例子，以下调用在一次bulk操作中索引了两个文档（ID 1 - JohnDoe and ID 2 - Jane Doe） :
`curl -XPOST 'localhost:9200/customer/external/_bulk?pretty' -d '``{"index":{"_id":"1"}}``{"name": "John Doe" }``{"index":{"_id":"2"}}``{"name": "Jane Doe" }``'`
在一个bulk操作中，首先更新第一个文档（ID为1），然后删除第二个文档（ID为2）
`curl -XPOST 'localhost:9200/customer/external/_bulk?pretty' -d '``{"update":{"_id":"1"}}``{"doc": { "name": "John Doe becomes Jane Doe" } }``{"delete":{"_id":"2"}}``'`
bulk API按顺序执行这些动作。如果其中一个动作因为某些原因失败了，它将会继续处理后面的动作。当bulk API返回时，它将提供每个动作的状态（按照同样的顺序），所以你能够看到某个动作成功与否。
加载json格式数据到es集群中
`curl -XPOST 'localhost:9200/bank/account/_bulk?pretty' --data-binary @accounts.json``curl 'localhost:9200/_cat/indices?v'`
搜索数据
`curl 'localhost:9200/bank/_search?q=*&pretty'`
bank索引中搜索（`_search`终点），并且`q=*`参数指示Elasticsearch去匹配这个索引中所有的文档。pretty参数仅仅是告诉Elasticsearch返回美观的JSON结果。
做了一次`match_all`查询并且返回第11到第20个文档：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": { "match_all": {} },``  "from": 10,``  "size": 10``}'`
做了一次`match_all`查询并且以账户余额降序排序，最后返前十个文档：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": { "match_all": {} },``  "sort": { "balance": { "order": "desc" } }``}'`
es查询默认返回的是所有数据，使用_source可以指定返回指定字段的数据
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": { "match_all": {} },``  "_source": ["account_number", "balance"]``}'`
返回账户编号为 20 的文档：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": { "match": { "account_number": 20 } }``}'`
`match_phrase`，它会去匹配短语“mill lane”：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": { "match_phrase": { "address": "mill lane" } }``}'`
使用bool查询
使用must组合了两个`match`查询，这个组合查询返回包含“mill” 和“lane” 的所有的账户
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": {``    "bool": {``      "must": [``        { "match": { "address": "mill" } },``        { "match": { "address": "lane" } }``      ]``    }``  }``}'`
使用should组合了两个`match`查询，它返回的是地址中包含“mill” 或者“lane”的所有的账户:
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": {``    "bool": {``      "should": [``        { "match": { "address": "mill" } },``        { "match": { "address": "lane" } }``      ]``    }``  }``}'`
使用must_not组合了两个查询，它返回地址中既不包含“mill”，同时也不包含“lane”的所有的账户信息：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "query": {``    "bool": {``      "must_not": [``        { "match": { "address": "mill" } },``        { "match": { "address": "lane" } }``      ]``    }``  }``}'`
使用过滤查询，找到存款大于等于20000并且小于等于30000的账户
`curl -`XPOST'localhost`:`9200`/bank/_search?pretty' -d '``{``  `"query"`: {``    `"filtered"`: {``      `"query"`: { `"match_all"`: {} },``      `"filter"`: {``        `"range"`: {``          `"balance"`: {``            `"gte"`: `20000`,``            `"lte"`: `30000`          }``        }``      }``    }``  }``}'`
聚合提供了分组并统计数据的能力。理解聚合的最简单的方式是将其粗略地等同为SQL的GROUP BY和SQL聚合函数。在Elasticsearch中，你可以在一个响应中同时返回命中的数据和聚合结果。你可以使用简单的API同时运行查询和多个聚合并一次返回，这避免了来回的网络通信，是非常强大和高效的。
按照state分组，并按照州名的计数倒序排序(默认)：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "size": 0,``  "aggs": {``    "group_by_state": {``      "terms": {``        "field": "state"``      }``    }``  }``}'`
在SQL中，上面的聚合在概念上类似于：
SELECTCOUNT(*) from bank GROUPBY state ORDERBYCOUNT(*) DESC
计算了每个州的账户的平均存款（还是按照账户数量倒序排序的前10个州）：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "size": 0,``  "aggs": {``    "group_by_state": {``      "terms": {``        "field": "state"``      },``      "aggs": {``        "average_balance": {``          "avg": {``            "field": "balance"``          }``        }``      }``    }``  }``}'`
在前面的聚合的基础上，现在让我们按照平均余额进行排序：
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "size": 0,``  "aggs": {``    "group_by_state": {``      "terms": {``        "field": "state",``        "order": {``          "average_balance": "desc"``        }``      },``      "aggs": {``        "average_balance": {``          "avg": {``            "field": "balance"``          }``        }``      }``    }``  }``}'`
下面的例子显示了如何使用年龄段（20-29，30-39，40-49）分组，然后再用性别分组，最后为每一个年龄段的每组性别计算平均账户余额。
`curl -XPOST 'localhost:9200/bank/_search?pretty' -d '``{``  "size": 0,``  "aggs": {``    "group_by_age": {``      "range": {``        "field": "age",``        "ranges": [``          {``            "from": 20,``            "to": 30``          },``          {``            "from": 30,``            "to": 40``          },``          {``            "from": 40,``            "to": 50``          }``        ]``      },``      "aggs": {``        "group_by_gender": {``          "terms": {``            "field": "gender"``          },``          "aggs": {``            "average_balance": {``              "avg": {``                "field": "balance"``              }``            }``          }``        }``      }``    }``  }``}'`
使用多索引多类型搜索
所有的搜索API都可以跨多个类型使用，也可以通过多索引语法跨索引使用。例如，我们可以搜索twitter索引的跨类型的所有文档。
`$ curl -XGET 'http://localhost:9200/twitter/_search?q=user:kimchy'`
我们也可以带上特定的类型:
`$ curl -XGET 'http://localhost:9200/twitter/tweet,user/_search?q=user:kimchy'`
我们也可以搜索跨多个索引的所有文档
`$ curl -XGET 'http://localhost:9200/kimchy,elasticsearch/tweet/_search?q=tag:wow'`
或者我们也可以用`_all`占位符表示搜索所有可用的索引的所有推特。
`$ curl -XGET 'http://localhost:9200/_all/tweet/_search?q=tag:wow'`
或者搜索跨所有可用索引和所有可用类型的推特
`$ curl -XGET 'http://localhost:9200/_search?q=tag:wow'`
采用uri搜索
一个搜索可以用纯粹的uri来执行查询。在这种模式下使用搜索，并不是所有的选项都是暴露的。它可以方便快速进行curl 测试。
$ curl-XGET 'http://localhost:9200/twitter/tweet/_search?q=user:kimchy'
**参数**
|**Name**|**Description**|
|----|----|
|q|表示查询字符串|
|df|在查询中，当没有定义字段的前缀的情况下的默认字段前缀|
|analyzer|当分析查询字符串时，分析器的名字|
|default_operator|被用到的默认操作，有AND和OR两种，默认是OR|
|explain|对于每一个命中(hit)，对怎样得到命中得分的计算给出一个解释|
|_source|将其设置为false，查询就会放弃检索_source字段。你也可以通过设置_source_include和_source_exclude检索部分文档|
|fields|命中的文档返回的字段|
|sort|排序执行。可以以fieldName、fieldName:asc或者fieldName:desc的格式设置。fieldName既可以是存在的字段，也可以是_score字段。可以有多个sort参数|
|track_scores|当排序的时候，将其设置为true，可以返回相关度得分|
|timeout|默认没有timeout|
|from|默认是0|
|size|默认是10|
|search_type|搜索操作执行的类型，有dfs_query_then_fetch, dfs_query_and_fetch, query_then_fetch, query_and_fetch, count, scan几种，默认是query_then_fetch|
|lowercase_expanded_terms|terms是否自动小写，默认是true|
|analyze_wildcard|是否分配通配符和前缀查询，默认是false|
有搜索DSL的搜索请求可以被执行。这些DSL包含在请求的请求体中。
$ curl-XGET 'http://localhost:9200/twitter/tweet/_search' -d '{
    "query" : {
        "term" : { "user" :"kimchy" }
    }
}
'
**1 参数**
|**Name**|**Description**|
|----|----|
|timeout|默认没有timeout|
|from|默认是0|
|size|默认是10|
|search_type|搜索操作执行的类型，有dfs_query_then_fetch, dfs_query_and_fetch, query_then_fetch, query_and_fetch, count, scan几种，默认是query_then_fetch|
|query_cache|当?search_type=count时，查询结果是否缓存|
`search_type`和`query_cache`必须通过查询参数字符串传递。
`HTTPGET`和`HTTP POST`都可以用来执行带有请求体的搜索
可以用from和size参数对结果进行分页。from表示你想获得的第一个结果的偏移量，size表示你想获得的结果的个数。from默认是0，size默认是10.
`{``    "from" : 0, "size" : 10,``    "query" : {``        "term" : { "user" : "kimchy" }``    }``}`
一个特定的字段运行添加一个或者多个排序。排序定义在字段级别，特定的字段名`_score`是通过得分排序。
`{``    "sort" : [``        { "post_date" : {"order" : "asc"}},``        "user",``        { "name" : "desc" },``        { "age" : "desc" },``        "_score"``    ],``    "query" : {``        "term" : { "user" : "kimchy" }``    }``}`
elasticsearch支持通过数组或者多值字段排序。mode选项控制获取排序文档的什么数组值来进行排序。mode选项有如下几种
- min：挑选最低的值
- max: 挑选最高的值
- sum：挑选所有值的和作为排序的值，仅用于数字
- avg：挑选所有值得平均作为排序的值，仅用于数字
按照文档的平均价格的升序进行排列
`·         ``curl -XPOST 'localhost:9200/_search' -d '{``·         ``   "query" : {``·         ``    ...``·         ``   },``·         ``   "sort" : [``·         ``      {"price" : {"order" : "asc", "mode" : "avg"}}``·         ``   ]`·         `}'`
