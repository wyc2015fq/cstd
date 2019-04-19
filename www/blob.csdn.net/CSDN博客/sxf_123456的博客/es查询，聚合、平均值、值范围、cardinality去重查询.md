# es查询，聚合、平均值、值范围、cardinality去重查询 - sxf_123456的博客 - CSDN博客
2017年10月10日 18:27:44[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：4575
                
GET ana-apk/_search
{
  "query": {
    "match_all": {}
  }
}
GET ana-apk/_search #查询不重复的mac地址
{
  "size": 10,
  "aggs": {
    "MAC": {
      "cardinality": {
        "field": "MAC.keyword"
      }
    }
  },
  "_source":{
    "include":["MAC","ProbeOUI","RTPLossRate"]
  }
}
GET ana-apk/_search #计算RTPLossRate的平均值
{
  "size": 0,
  "aggs": {
    "avg_RTPLossRate": {
      "avg": {
        "field": "RTPLossRate"
      }
    }
  }
}
GET ana-apk/_search #查询RTPLossRate小于0的值
{
  "size": 10,
  "query": {
    "range": {
      "RTPLossRate": {
        "lt": 0
      }
    }
  },
  "_source":{
    "include":["MAC","ProbeOUI","RTPLossRate","message"]
  }
}
GET ana-apk/_search #查询RTPLossRate大于等于0小于等于1000的值
{
  "size": 10,
  "query": {
    "range": {
      "RTPLossRate": {
        "gte": 0,
        "lte": 1000
      }
    }
  },
  "aggs": {
    "avg_RTPLossRate": {
      "avg": {
        "field": "RTPLossRate"
      }
    }
  }
}
GET ana-apk/_search  #查询RTPLossRate在0到1000之间的数据，并输出字段为OUI的10个值
{
  "size": 0,
  "query": {
    "range": {
      "RTPLossRate": {
        "gte": 0,
        "lte": 1000
      }
    }
  },
  "aggs": {
    "NAME": {
      "terms": {
        "field": "OUI",
        "size": 10
      }
    }
  }
}
GET ana-apk/_search #查询字段OUI包含SKYWORTH的值，并且RTPLossRate的范围在0到1000之间，计算RTPLossRate的平均值
{
  "size": 0,
  "query": {
    "bool": {
      "must": [
        {"term": {
          "OUI": {
            "value": "SKYWORTH"
          }
        }},
        {"range": {
          "RTPLossRate": {
            "gte": 0,
            "lte": 1000
          }
        }}
      ]
    }},
    "aggs": {
      "avg_RTPLossRate": {
        "avg": {
          "field": "RTPLossRate"
        }
      }
    }
}
GET ana-apk/_search
{
  "size": 0,
  "query": {
    "bool": {
      "must": [
        {"term": {
          "OUI": {
            "value": "ZTE"
          }
        }},
        {"range": {
          "RTPLossRate": {
            "gte": 0,
            "lte": 1000
          }
        }}
      ]
    }},
    "aggs": {
      "avg_RTPLossRate": {
        "avg": {
          "field": "RTPLossRate"
        }
      }
    }
}
GET ana-apk/_search
{
  "size": 0,
  "query": {
    "bool": {
      "must": [
        {"term": {
          "OUI": {
            "value": "huawei"
          }
        }},
        {"range": {
          "RTPLossRate": {
            "gte": 0,
            "lte": 1000
          }
        }}
      ]
    }},
    "aggs": {
      "avg_RTPLossRate": {
        "avg": {
          "field": "RTPLossRate"
        }
      }
    }
}
GET ana-apk/_search
{
  "size": 0,
  "query": {
    "bool": {
      "must": [
        {"term": {
          "OUI": {
            "value": "Hisense"
          }
        }},
        {"range": {
          "RTPLossRate": {
            "gte": 0,
            "lte": 1000
          }
        }}
      ]
    }},
    "aggs": {
      "avg_RTPLossRate": {
        "avg": {
          "field": "RTPLossRate"
        }
      }
    }
}

宜宾
获取manufacturer的左右类型，如ZTE、SCTY、Star-Net、FiberHome、Huawei Technologies Co., Ltd、
changhong、ALCL、CHANGHONG、Hisense
GET gather-033/_search  #查询宜宾地区，所有的光猫型号类型
{
  "size": 10,
  "query": {
  "bool": {
    "must": [
      {"term":{"area_code":"112000"}}
    ]
  }
  },
  "aggs": {
    "manufacturer": {
      "terms": {
        "field": "manufacturer.keyword",
        "size": 100
      }
    }
  },  
  "_source": ["cpurate","memrate","temp","manufacturer","area_code"]
}
计算ZTE的cpu平均值
GET gather-033_112000/_search
{
  "size":5,
  "query": {
  "match": {
    "manufacturer": "ZTE"
  }
  },
  "aggs": {
    "avg_cpurate": {
      "avg": {
        "field": "cpurate"
      }
    }
  }, 
 "_source": ["cpurate","memrate","temp","manufacturer","area_code"]
}
计算ZTE的mem平均值
GET gather-033_112000/_search
{
  "size":5,
  "query": {
  "match": {
    "manufacturer": "ZTE"
  }
  },
  "aggs": {
    "avg_memrate": {
      "avg": {
        "field": "memrate"
      }
    }
  }, 
 "_source": ["cpurate","memrate","temp","manufacturer","area_code"]
}
计算ZTE的温度平均值
GET gather-033_112000/_search
{
  "size":5,
  "query": {
  "match": {
    "manufacturer": "ZTE"
  }
  },
  "aggs": {
    "avg_temp": {
      "avg": {
        "field": "temp"
      }
    }
  }, 
 "_source": ["cpurate","memrate","temp","manufacturer","area_code"]
}
以cpu使用率降序，取top 10数据
GET gather-033_112000/_search
{
  "size": 10,
  "sort": [
    {
      "cpurate": {
        "order": "desc"
      }
    }
  ],
  "_source": ["cpurate","memrate","temp","manufacturer","area_code","loid"]
}
以内存使用率降序，取top 10数据
GET gather-033_112000/_search
{
  "size": 10,
  "sort": [
    {
      "memrate": {
        "order": "desc"
      }
    }
  ],
  "_source": ["cpurate","memrate","temp","manufacturer","area_code","loid"]
}
以温度降序，取top 10数据(asc升序，desc降序)
GET gather-033_112000/_search
{
  "size": 10,
  "query": {
    "range": {
      "temp": {
        "gte": 0,
        "lte": 100
      }
    }
  }, 
  "sort": [
    {
      "temp": {
        "order": "desc"
      }
    }
  ],
  "_source": ["cpurate","memrate","temp","manufacturer","area_code","loid"]
}
当温度中存在不规则数据时，使用filter过滤
查找光猫型号为Huawei Technologies Co., Ltd,且温度范围在0到100之间，计算温度的平均值
GET gather-033_112000/_search
{
  "size": 10,
  "query": {
    "bool": {
      "must": {
        "match": {
          "manufacturer": "Huawei Technologies Co., Ltd"
        }
      },
      "filter": {
        "range": {
          "temp": {
            "gte": 0,
            "lte": 100
          }
        }
      }
    }
  },
  "aggs": {
    "avg_temp":{
      "avg": {
        "field": "temp"
      }
    }
  },
  "_source": ["cpurate","memrate","temp","manufacturer","area_code"]
}
