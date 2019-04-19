# es 查询含有filter过滤查询和不含有filter查询 - sxf_123456的博客 - CSDN博客
2018年03月08日 09:40:42[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1657
                GET gather-033_112000/_search
{
  "size": 0,
  "query": {
    "range": {
      "memrate": {
        "gte": 0,
        "lte": 100
      }
    }
  }
}
GET gather-033_112000/_search
{
  "size": 0,
  "query": {
    "bool": {
      "filter": {
        "range": {
          "memrate": {
            "gte": 0,
            "lte": 100
          }
        }
      }
    }
  }
}
这个两个查询结果一样，第一个是直接查询范围在0到100，第二个是将范围为0到100的过滤出来
