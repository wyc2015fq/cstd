
# ES聚合查询，设置查询结果条数以及排序 - 追求卓越,做到专业 - CSDN博客


2019年01月02日 13:57:52[Waldenz](https://me.csdn.net/enter89)阅读数：1299


在aggs中，指定size的个数，默认为10，即返回10条聚合查询结果。
`{
  "query": {
    "bool": {
      "must": [
        {
          "range": {
            "AddDateTime": {
              "gte": "2018-12-1 00:40:39",
              "lte": "2018-12-21 23:42:59",
              "format": "yyyy-MM-dd HH:mm:ss",
              "time_zone": "+08:00"
            }
          }
        }
      ]
    }
  },
  "size": 0,
  "aggs": {
    "groups": {
      "terms": {
        "field": "ProductSubdivisionId",
        "size":20,
        "order" : {  "_count" : "desc" }
      }
    }
  }
}`按照符合条件的document个数，倒排序。增加
`"order":{"_count":"desc"}`
![](https://img-blog.csdnimg.cn/20190102134926680.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

