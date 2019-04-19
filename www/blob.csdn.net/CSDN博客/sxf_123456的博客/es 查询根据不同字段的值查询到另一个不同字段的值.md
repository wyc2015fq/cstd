# es 查询根据不同字段的值查询到另一个不同字段的值 - sxf_123456的博客 - CSDN博客
2018年03月13日 14:58:01[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：947
1、根据不同的硬件版本，找到对应的软件版本
GET gather-012-20180109/_search
{
  "size": 0, 
  "aggs": {
    "1": {
      "terms": {
        "field": "dev_hard_ver",
        "size": 200
      },
      "aggs": {
        "2": {
          "terms": {
            "field": "soft_ver",
            "size": 1
          }
        }
      }
    }
  }
}
结果：
"aggregations": {
    "1": {
      "doc_count_error_upper_bound": 0,
      "sum_other_doc_count": 39,
      "buckets": [
        {
          "2": {
            "doc_count_error_upper_bound": 5,
            "sum_other_doc_count": 595451,
            "buckets": [
              {
                "key": "HWV218012P0000",
                "doc_count": 2109171
              }
            ]
          },
          "key": "EC6108V9U_ca_sccdx",
          "doc_count": 2704622
        },
2、根据不同厂商，看出不同厂商对应的用户数量
GET gather-034-test-20180312/_search
{
  "size": 0, 
  "aggs": {
    "1": {
      "terms": {
        "field": "oui",
        "size": 20
      },
      "aggs": {
        "2": {
          "cardinality": {
            "field": "iptvAccount"
          }
        }
      }
    }
  }
}
结果：
"aggregations": {
    "1": {
      "doc_count_error_upper_bound": 0,
      "sum_other_doc_count": 0,
      "buckets": [
        {
          "2": {
            "value": 443051
          },
          "key": "SKYWORTH",
          "doc_count": 32039736
        },
        {
          "2": {
            "value": 209894
          },
          "key": "ZTE",
          "doc_count": 21264268
        },
