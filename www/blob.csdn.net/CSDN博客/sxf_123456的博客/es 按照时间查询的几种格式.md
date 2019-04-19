# es  按照时间查询的几种格式 - sxf_123456的博客 - CSDN博客
2018年03月16日 11:21:43[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：9306
                GET m-2fg-day/_search
{
  "size": 0,
  "query": {
    "bool": {
      "filter":[
        {"term":{"2fg_year":"2017"}},
        {"term":{"2fg_month":"09"}},
        {"term":{"2fg_day":"25"}}
        ]
    }
  },
  "aggs": {
    "uncertain_reason": {
      "terms": {
        "field": "uncertain_reason.keyword",
        "size": 10
      }
    }
  }
}
GET gather-034-20180302/_search
{
  "query": {
    "bool": {
      "must": [
        {"range": {
          "report_time": {
            "gte": "2018-03-02T09:39:12.000Z",
            "lte": "2018-03-02T13:00:00.000Z"
          }
        }},
        {
          "term": {
            "itv_account": {
              "value": "AD87218665@ITV4"
            }
          }
        }
      ]
    }
  }
}
GET gather-000/_search
{
  "size": 0,
  "query": {
    "range": {
      "@timestamp": {
        "gte": "2017-08-30T00:00:00",
        "lte": "2017-08-30T23:59:59",
        "time_zone":"+08:00"
      }
    }
  }
}
GET gather-034-20180316/_search
{
    "_source": {
    "include": [
      "report_time",
      "flr"
    ]
  },
  "query": {
    "bool": {
      "must": [
        {
          "range": {
            "flr": {
              "gte": 30
            }
          }
        },
        {"term": {
          "area": {
            "value": "SC_CD"
          }
        }},
        {
          "range": {
            "report_time": {
              "gte": "now-30m"
            }
          }
        }
      ]
    }
  },
  "aggs": {
    "1": {
      "terms": {
        "field": "itv_account",
        "size": 10
      },
      "aggs": {
        "2": {
          "avg": {
            "field": "flr"
          }
        }
      }
    }
  },"sort": [
    {
      "report_time": {
        "order": "desc"
      }
    }
  ]
}
