# ES  查询一个字段的值小于另一个字段的值 - sxf_123456的博客 - CSDN博客
2019年03月15日 15:07:35[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：35标签：[ES 一个字段的值小于另一个字段的值](https://so.csdn.net/so/search/s.do?q=ES 一个字段的值小于另一个字段的值&t=blog)
个人分类：[Elasticsearch](https://blog.csdn.net/sxf_123456/article/category/7660594)
查询语句:
  "query": {
        "bool": {
            "must": [
                {
                    "script": {
                        "script": {
                            "inline": "doc['fieldA'].value < doc['fieldB'].value",
                            "lang": "painless"
                        }
                    }
                }
            ]
        }
    }
