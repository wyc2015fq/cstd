# 查询光猫厂家为ZTE,且内存范围在0到100之间，统计去掉重复的loid之后的数量，并计算出内存的avg，max，min，sum - sxf_123456的博客 - CSDN博客
2017年10月11日 16:25:25[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：202
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
                
GET gather-033_112000/_search #查询光猫厂家为ZTE,且内存范围在0到100之间，统计去掉重复的loid之后的数量，并计算出内存的avg，max，min，sum
{
  "size": 0,
  "query": {
    "bool": {
      "must": {
        "match": {
          "manufacturer": "ZTE"
        }
      },
      "filter": {
        "range": {
          "memrate": {
            "gte": 0,
            "lte": 100
          }
        }
      }
    }
  },
  "aggs":{
    "loid":{
      "cardinality": {
        "field": "loid.keyword"
      }
    },
   "avg_temp": {  #stat能够查询到avg、max、min、sum
    "stats": {
        "field":"memrate"
      }
    }
  }
}v
            
