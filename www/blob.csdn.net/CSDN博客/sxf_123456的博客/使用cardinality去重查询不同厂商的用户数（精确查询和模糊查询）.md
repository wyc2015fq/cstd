# 使用cardinality去重查询不同厂商的用户数（精确查询和模糊查询） - sxf_123456的博客 - CSDN博客
2018年03月07日 13:31:59[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：175
先查看所有的用户数（去重查询），注意使用precision_threshold，当precision_threshold大于等于100时为模糊查询，小于100时，查询的数据有正确数据。
GET gather-034test/_search
{
  "size": 1,
  "aggs": {
    "IptvAccount_type": {
      "cardinality": {
        "field": "IptvAccount.keyword",
        "precision_threshold":90
      }
    }
  }
}
使用聚合查询所有厂商
GET gather-034test/_search
{
  "size": 1,
  "aggs": {
    "OUI": {
      "terms": {
        "field": "OUI.keyword",
        "size": 400
      }
    }
  }
}
结果：
  "aggregations": {
    "OUI": {
      "doc_count_error_upper_bound": 0,
      "sum_other_doc_count": 0,
      "buckets": [
        {
          "key": "SKYWORTH",
          "doc_count": 1515218
        },
        {
          "key": "ZTE",
          "doc_count": 1234089
        },
        {
          "key": "huawei",
          "doc_count": 716292
        },
        {
          "key": "Hisense",
          "doc_count": 105701
        },
        {
          "key": "Jiuzhou",
          "doc_count": 83134
        },
        {
          "key": "990104",
          "doc_count": 49
        },
        {
          "key": "Tianyi",
          "doc_count": 12
        },
        {
          "key": "changhong",
          "doc_count": 3
        }
查询每个厂商的用户数，注意用户去重
GET gather-034test/_search
{
  "size": 1,
  "query": {
    "term": {
      "OUI.keyword": {
        "value": "Jiuzhou"
      }
    }
  },
  "aggs": {
    "IptvAccount_type": {
      "cardinality": {
        "field": "IptvAccount.keyword",
        "precision_threshold":90
      }
    }
  }
}

