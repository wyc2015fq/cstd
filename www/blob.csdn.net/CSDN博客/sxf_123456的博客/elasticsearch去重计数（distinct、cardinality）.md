# elasticsearch去重计数（distinct、cardinality） - sxf_123456的博客 - CSDN博客
2017年08月16日 10:13:59[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：7356
如果需要针对ES索引统计某个字段上出现的不同值的个数时，可以使用cardinality聚合查询完成:
GET /urlAttributes/_search?search_type=count
{
  "aggs": {
    "uniq_attr": {
      "cardinality": {
        "field": "domain.keyword"
      }
    }
  }
}
注释：如上，是查询索引urlAttributes中domain字段上的不同值个数， "uniq_attr"是用户自己取的bucket名字。
得到的响应如下所示：
{
   "took": 28,
   "timed_out": false,
   "_shards": {
      "total": 6,
      "successful": 6,
      "failed": 0
   },
   "hits": {
      "total": 68000,
      "max_score": 0,
      "hits": []
   },
   "aggregations": {
      "uniq_attr": {
         "value": 1216
      }
   }
}
返回结果表示该字段出现过121618个不同的domain名字。
GET  gather-000/_search
{
 "size": 0, 
 "aggs": {
   "distinct_oltopvarname": {
     "cardinality": {
       "field": "oltopvarname.keyword"
     }
   }
 },
 "_source":["oltopvarname"]
}
{
  "took": 216,
  "timed_out": false,
  "_shards": {
    "total": 3,
    "successful": 3,
    "failed": 0
  },
  "hits": {
    "total": 14404611,
    "max_score": 0,
    "hits": []
  },
  "aggregations": {
    "distinct_oltopvarname": {
      "value": 495
    }
  }
}

