# kibana按照每小时为时间统计数据数量 - sxf_123456的博客 - CSDN博客
2018年04月26日 10:26:05[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：2186
                GET gather-apk-20180417/_search
{
  "size": 0,
  "query": {
    "range": {
      "uploadTime": {
        "gte": "2018-04-17 01:00",
        "lte":"2018-04-17 09:00",
        "format":"yyyy-MM-dd HH:mm",
        "time_zone":"+08:00"
      }
    }
  },
 "aggs": {
    "2": {
      "date_histogram": {
        "field": "uploadTime",
        "interval": "1h",
        "time_zone": "Asia/Shanghai",
        "min_doc_count": 1
      },
      "aggs": {
        "3": {
          "terms": {
            "field": "oui",
            "size": 15,
            "order": {
              "1": "desc"
            }
          },
          "aggs": {
            "1": {
              "cardinality": {
                "field": "mac"
              }
            }
          }
        }
      }
    }
  }
}            
