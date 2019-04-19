# kibana 两个字段的值比较查询(字段A的值小于字段B的值) - sxf_123456的博客 - CSDN博客
2018年06月22日 11:34:11[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：486
GET gather-028-20180620/_search
{
  "size": 2,
"query": {
    "bool": {
      "must": [
        {
          "exists": {
            "field": "band_width"
}
        },
{
          "range": {
            "band_width": {
              "gte": 100
}
          }
        },
{
          "script": {
            "script": "doc['max_band'].value < doc['band_width'].value"
}
        },
{
          "range":{
            "@timestamp":{
              "lt":1529521250101
}
          }
        }
      ]
    }
  },
"sort": [
    {
      "@timestamp": {
        "order": "desc"
}
    }
  ]
}
