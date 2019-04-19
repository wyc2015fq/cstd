# elasticsearch 中term与match区别 - sxf_123456的博客 - CSDN博客
2017年12月19日 17:50:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3508
elasticsearch 中term与match区别
term是精确查询
match是模糊查询
#### term查询
term是代表完全匹配，也就是精确查询，搜索前不会再对搜索词进行分词，所以我们的搜索词必须是文档分词集合中的一个。比如说我们要找标题为北京奥运的所有文档
```
$curl -XGET http://localhost:9200/index/doc/_search?pretty -d 
'{
  "query":{
    "term":{
        "title":"北京奥运"
    }
  }
}'
```
将会得到如下结果
```
{
    "took": 1,
    "timed_out": false,
    "_shards": {
        "total": 5,
        "successful": 5,
        "failed": 0
    },
    "hits": {
    "total": 1,
    "max_score": 0.92055845,
    "hits": [
     {
        "_index": "index",
        "_type": "doc",
        "_id": "3",
        "_score": 0.92055845,
        "_source": {
           "content": "同一个世界同一个梦想",
           "title": "北京奥运",
           "tags": [
               "和平"
            ]
        }
      }
    ]
  }
}
```
``
#### match类查询
match查询会先对搜索词进行分词,分词完毕后再逐个对分词结果进行匹配，因此相比于term的精确搜索，match是分词匹配搜索,match搜索还有两个相似功能的变种，一个是match_phrase，一个是multi_match，接下来详细介绍一下
###### match
前面提到match搜索会先对搜索词进行分词，对于最基本的match搜索来说，只要搜索词的分词集合中的一个或多个存在于文档中即可，例如，当我们搜索`中国杭州`，搜索词会先分词为`中国`和`杭州`,只要文档中包含`搜索`和`杭州`任意一个词，都会被搜索到
```
$curl -XGET http://localhost:9200/index/doc/_search?pretty -d 
'{
    "query": {
        "match": {
            "content": "中国杭州"
        }
    }
}'
```
文档3正文中有杭州，文档2中有中国，因此搜索结果有两个，文档3中杭州出现两次，所以排在前面，结果如下：
```
{
  "took" : 1,
  "timed_out" : false,
  "_shards" : {
    "total" : 5,
    "successful" : 5,
    "failed" : 0
  },
  "hits" : {
      "total" : 2,
      "max_score" : 0.99999994,
      "hits" : [ {
            "_index" : "index",
            "_type" : "doc",
            "_id" : "4",
            "_score" : 0.99999994,
            "_source" : {
                 "content" : "杭州是一个美丽的城市,欢迎来到杭州",
                "title" : "宣传",
                "tags" : [ "旅游", "城市" ]
            }
       }, {
            "_index" : "index",
            "_type" : "doc",
            "_id" : "2",
            "_score" : 0.8838835,
            "_source" : {
                  "content" : "中国是世界上人口最多的国家",
                  "title" : "中国",
                  "tags" : [ "中国", "人口" ]
            }
       } ]
    }
}
```
