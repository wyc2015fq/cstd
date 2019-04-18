# Elasticsearch----Query DSL - weixin_33985507的博客 - CSDN博客
2017年04月08日 17:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
# 基本语法
```
{
    QUERY_NAME:{
        ARGUMENT:VALUE,
        ARGUMENT:VALUE,
        ...
    }
}
{
    QUERY_NAME:{
        FIELD_NAME:{
            ARGUMENT:VALUE,
            ARGUMENT:VALUE,
            ...
        }
    }
}
```
# 示例
```
GET /_search    //查询所有的，默认只会显示10条document
{
  "query":{
    "match_all": {}
  }
}
GET /test_index/test_type/_search    //查询字段“test_field”，包含“test”
{
  "query": {
    "match": {
      "test_field": "test"
    }
  }
}
```
**组合搜索多个条件**
```
//模拟数据
PUT /website/article/1
{
  "title":"my elasticsearch article",
  "content":"es is very good",
  "author_id":110
}
PUT /website/article/2
{
  "title":"my hadoop article",
  "content":"hadoop is very bad",
  "author_id":111
}
PUT /website/article/3
{
  "title":"my elasticsearch article",
  "content":"es is very bad",
  "author_id":112
}
```
```
//查询 title必须包含elasticsearch，content可以不包含elasticsearch，author_id必须不为110
GET /website/article/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "match": {
            "title": "elasticsearch"
          }
        }
      ],
      "should": [
        {
         "match": {
           "content":"elasticsearch"
         }
        }
      ],
      "must_not": [
        {
          "match": {
            "author_id": 110
          }
        }
      ]
    }
  }
}
//查询title,content包含elasticsearch
GET /website/article/_search
{
  "query": {
    "multi_match": {
      "query": "elasticsearch",
      "fields": ["title","content"]
    }
  }
}
//查询title,content必须包含elasticsearch，并且author_id必须不为110的
GET /website/article/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "multi_match": {
            "query": "elasticsearch",
            "fields": ["title","content"]
          }
        }
      ],
      "must_not": [
        {
          "match": {
            "author_id": 110
          }
        }
      ]
    }
  }
}
```
# 定位不合法的搜索以及原因
```
//以这样的语法，来判断一个查询json串是否合法
GET /test_index/test_type/_validate/query?explain
{
  "query":{
    "math":{
      "test_field":"test"
    }
  }
}
//出错返回值
{
  "valid": false,
  "error": "org.elasticsearch.common.ParsingException: no [query] registered for [math]"
}
//正确返回值
{
  "valid": true,
  "_shards": {
    "total": 1,
    "successful": 1,
    "failed": 0
  },
  "explanations": [
    {
      "index": "test_index",
      "valid": true,
      "explanation": "+test_field:test #(#_type:test_type)"
    }
  ]
}
```
# filter与query的对比
```
//模拟数据
PUT /company/employee/1
{
  "address":{
    "country":"china",
    "provice":"beijing",
    "city":"beijing"
  },
  "name":"jack",
  "age":28,
  "join_date":"2017-01-01"
}
PUT /company/employee/2
{
  "address":{
    "country":"china",
    "provice":"jiangsu",
    "city":"nanjing"
  },
  "name":"tom",
  "age":30,
  "join_date":"2016-01-01"
}
PUT /company/employee/3
{
  "address":{
    "country":"china",
    "provice":"shanxi",
    "city":"xian"
  },
  "name":"marry",
  "age":35,
  "join_date":"2015-01-01"
}
```
```
//查询年龄大于等于30，同时join_date必须是2016-01-01
GET /company/employee/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "match": {
            "join_date": "2016-01-01"
          }
        }
      ],
      "filter": {
        "range": {
          "age": {
            "gte": 30
          }
        }
      }
    }
  }
}
```
***filter与query的对比***
> - filter，仅仅只是按照搜索条件过滤出需要的数据而已，不计算任何相关度分数，对相关度没有任何影响
- query，会去计算每个document相对于搜索条件的相关度，并按照相关度进行排序
- 一般来说，如果是在进行搜素，需要将最匹配搜索条件的数据先返回，那么用query；如果只是要根据一些条件筛选出一部分数据，不关注其排序，那么用filter
- 如果你希望越符合这些搜索条件的document越排在前面返回，那么这些搜索条件就要放在query中；如果不希望一些搜索条件来影响你的document排序，那么就放在filter中即可
- filter不需要计算相关度分数，不需要按照相关度分数进行排序，同时还有内置的自动cache最常使用filter的数据；query则相反，要计算相关度分数，还需要按照分数排序，而且无法cache结果。所以，filter的速度会更快些
