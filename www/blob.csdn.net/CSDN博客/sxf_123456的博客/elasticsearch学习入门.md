# elasticsearch学习入门 - sxf_123456的博客 - CSDN博客
2017年07月19日 13:11:05[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：99
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
[](http://blog.csdn.net/lgnlgn/article/details/8053626)
**（由于ES更新很快，本文这类快餐式的记录仅供参考）**
这几年，搜索的开发门槛越来越低，每个语言都有开源的检索工具包，而且功能越来越全，完整的解决方案也越来越多、越来越好用，比如lucene上就有solr, elasticsearch, sensei等。它们对于绝大部分的需求应该说都覆盖了，解放了直接在检索工具包上的开发工作量，让人可以更多关注业务上的开发。个人比较看好elasticsearch（简称ES），ES的使用非常简单，让人感觉更多地在使用一个Nosql，而且允许很多插件功能可以自己开发。我们可以很容易通过rest客户端去[测试](http://lib.csdn.net/base/softwaretest)ES，因此学习起来很容易。
ES的官网有比较全面的API，但我看过以后感觉API的层次还是有点乱，至少没有[MongoDB](http://lib.csdn.net/base/mongodb)的文档那么简单易读。从简单的应用开始慢慢认识ES的。比如要搭建个中文新闻信息的[搜索引擎](http://lib.csdn.net/base/searchengine)，新闻有"标题"、"内容"、"作者"、"类型"、"发布时间"这五个字段；我们要提供"标题和内容的检索"、"排序"、"高亮"、"统计"、"过滤"等一些基本功能。ES提供了smartcn的中文分词插件，测试的话建议使用[IK分词插件](https://github.com/medcl/elasticsearch-analysis-ik)，也可以看看插件作者给的例子。
下载安装好插件、启动ES，之后就可以开始ES的体验了。
1. 创建一个名叫test的索引
PUT http://localhost:9200/test
2. 创建mapping
POST http://localhost:9200/test/news/_mapping
内容为：
{
    "news": {
        "properties": {
            "content": {
                "type": "string",
                "store": "no",
                "term_vector": "with_positions_offsets",
                "index_analyzer": "ik",
                "search_analyzer": "ik"
            }
,
            "title": {
                "type": "string",
                "store": "no",
                "term_vector": "with_positions_offsets",
                "index_analyzer": "ik",
                "search_analyzer": "ik",
                "boost": 5
            }
,
            "author": {
                "type": "string","index":"not_analyzed"
            }
,
            "publish_date": {
                "type": "date", "format":"yyyy/MM/dd","index":"not_analyzed"
            }
,
   "category": {
                "type": "string","index":"not_analyzed"
            }
        }
    }
}
URL的test、news在ES中是索引/类型，感觉是对应[数据库](http://lib.csdn.net/base/mysql)像库名/表名的关系，POST内容中properties对应mapping里的内容，里面5个字段。type指出字段类型、内容、标题字段要进行分词和高亮因此要设置分词器和开启term_vector。具体类型的API可以看[这里](http://www.elasticsearch.org/guide/reference/mapping/core-types.html)。
3. 制造并提交一些数据：
POST http://localhost:9200/test/news/
内容随便造若干条：
{
"content":"中国国务院总理访问欧洲",
"title":"美国今年经济形势良好",
"publish_date":"2010/07/01",
"author":"张三",
"category":"财经"
}
4. 检索 
POST http://localhost:9200/test/news/_search
内容包括几个部分：
分页:[from/size](http://www.elasticsearch.org/guide/reference/api/search/from-size.html)、字段:[fields](http://www.elasticsearch.org/guide/reference/api/search/fields.html)、排序[sort](http://www.elasticsearch.org/guide/reference/api/search/sort.html)、查询:[query](http://www.elasticsearch.org/guide/reference/query-dsl/)、过滤:[filter](http://www.elasticsearch.org/guide/reference/api/search/fields.html)、高亮:[highlight](http://www.elasticsearch.org/guide/reference/api/search/highlighting.html)、统计:[facet](http://www.elasticsearch.org/guide/reference/api/search/facets/)
{
"from":0,
"size":10,
"fields":["title","content","publish_date","category", "author"],
"sort":[
{ "publish_date" : {"order" : "asc"} },
"_score"
],
        "query":{
"bool":{
"should" : [
{
"term" : { "title" : "中国" }
},
{
"term" : { "content" : "中国" }
}
]}
},
"filter":{
"range" : {
"publish_date" : { 
"from" : "2010/07/01", 
"to" : "2010/07/21", 
"include_lower" : true, 
"include_upper" : false
}
}
},
"highlight" : {
"pre_tags" : ["<tag1>", "<tag2>"],
"post_tags" : ["</tag1>", "</tag2>"],
"fields" : {
"title" : {}, 
"content" : {}
}
},
"facets" : {
 "cate" : { "terms" : {"field" : "category"} }
}
}
这里query我选择构造了个标题或内容中含”中国“， 
结果如下：
{
  "took" : 18,
  "timed_out" : false,
  "_shards" : {
    "total" : 5,
    "successful" : 5,
    "failed" : 0
  },
  "hits" : {
    "total" : 6,
    "max_score" : null,
    "hits" : [ {
      "_index" : "test",
      "_type" : "news",
      "_id" : "_FM13zCCSnWaTPOuzIok_A",
      "_score" : 0.024621923,
      "fields" : {
        "content" : "路透社报道，美国昨天在伊拉克如何如何，中国将如何面对形势。",
        "author" : "张三",
        "title" : "美国留给伊拉克的是个烂摊子",
        "category" : "政治",
        "publish_date" : "2010/07/10"
      },
      "highlight" : {
        "content" : [ "路透社报道，美国昨天在伊拉克如何如何，<tag2>中国</tag2>将如何面对形势。" ]
      },
      "sort" : [ 1278720000000, 0.024621923 ]
    }, {
      "_index" : "test",
      "_type" : "news",
      "_id" : "4FEY1T6-RMOmOJYts4FoAQ",
      "_score" : 0.024621923,
      "fields" : {
        "content" : "美联社报道，中国将于今天访问俄罗斯的北部地区。",
        "author" : "张三",
        "title" : "美国访问俄罗斯",
        "category" : "政治",
        "publish_date" : "2010/07/11"
      },
      "highlight" : {
        "content" : [ "美联社报道，<tag2>中国</tag2>将于今天访问俄罗斯的北部地区。" ]
      },
      "sort" : [ 1278806400000, 0.024621923 ]
    }, {
      "_index" : "test",
      "_type" : "news",
      "_id" : "lL-8BzCNTT2yEkqs1Owc_A",
      "_score" : 0.61871845,
      "fields" : {
        "content" : "美国国务卿希拉里对记者说，韩国首尔江南区的大部分富豪都有移居朝鲜的意愿。",
        "author" : "李四",
        "title" : "中国经济将面临下滑危险",
        "category" : "经济",
        "publish_date" : "2010/07/12"
      },
      "highlight" : {
        "title" : [ "<tag2>中国</tag2>经济将面临下滑危险" ]
      },
      "sort" : [ 1278892800000, 0.61871845 ]
    }, {
      "_index" : "test",
      "_type" : "news",
      "_id" : "DNB6gTpSRaOexC1axCpZ0Q",
      "_score" : 0.048311904,
      "fields" : {
        "content" : "中国国务院总理谈到国内体育，金牌无疑是最重要的，美国",
        "author" : "张三",
        "title" : "美国体育的举国体制",
        "category" : "体育",
        "publish_date" : "2010/07/14"
      },
      "highlight" : {
        "content" : [ "<tag2>中国</tag2>国务院总理谈到国内体育，金牌无疑是最重要的，美国" ]
      },
      "sort" : [ 1279065600000, 0.048311904 ]
    }, {
      "_index" : "test",
      "_type" : "news",
      "_id" : "4lH55yoAQVe7cARIYVfYnQ",
      "_score" : 0.048311904,
      "fields" : {
        "content" : "中国对东南亚海域的军事威胁将持续，俄罗斯严密关注事态发展",
        "author" : "张三",
        "title" : "各国大力发展教育事业",
        "category" : "政治",
        "publish_date" : "2010/07/15"
      },
      "highlight" : {
        "content" : [ "<tag2>中国</tag2>对东南亚海域的军事威胁将持续，俄罗斯严密关注事态发展" ]
      },
      "sort" : [ 1279152000000, 0.048311904 ]
    }, {
      "_index" : "test",
      "_type" : "news",
      "_id" : "hFovoBdCTI-2ErYAQV-AKg",
      "_score" : 0.12422675,
      "fields" : {
        "content" : "中国经济反弹乏力，美国经济持续低迷，其他经济体也靠不住",
        "author" : "李四",
        "title" : "欧洲债务危机席卷全球",
        "category" : "经济",
        "publish_date" : "2010/07/19"
      },
      "highlight" : {
        "content" : [ "<tag2>中国</tag2>经济反弹乏力，美国经济持续低迷，其他经济体也靠不住" ]
      },
      "sort" : [ 1279497600000, 0.12422675 ]
    } ]
  },
  "facets" : {
    "cate" : {
      "_type" : "terms",
      "missing" : 0,
      "total" : 10,
      "other" : 0,
      "terms" : [ {
        "term" : "政治",
        "count" : 4
      }, {
        "term" : "经济",
        "count" : 3
      }, {
        "term" : "体育",
        "count" : 3
      } ]
    }
  }
}
结果包含了需要的几个部分。值得注意的是，facet的统计是命中的结果进行统计，filter是对结果进行过滤，filter不会影响facet，如果要统计filter掉的的就要使用filter facet。
ES提供了非常多的功能，JSON的组合比一般的SQL这样的语法要凌乱，但是通过组合能够满足各种复杂的应用，很多影响性能的设置也值得好好去看。对开发者而已，利用好内置API接口进行二次开发也非常必要，比如开发自己的分词，和其他库的同步等。ES现在还在0.19.9
 但是已经出现了很多官方的培训，所以前途应该不可限量，未来可能会是检索系统解决方案的首选。
[](http://blog.csdn.net/lgnlgn/article/details/8053626#)[](http://blog.csdn.net/lgnlgn/article/details/8053626#)[](http://blog.csdn.net/lgnlgn/article/details/8053626#)[](http://blog.csdn.net/lgnlgn/article/details/8053626#)[](http://blog.csdn.net/lgnlgn/article/details/8053626#)[](http://blog.csdn.net/lgnlgn/article/details/8053626#)
- 
顶
0- 
踩
