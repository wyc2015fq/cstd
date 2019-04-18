# ElasticSearch搜索时match和term大小写问题 - z69183787的专栏 - CSDN博客
2018年08月22日 11:45:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：837
个人分类：[搜索引擎-ElasticSearch&ELK](https://blog.csdn.net/z69183787/article/category/6850573)
[https://blog.csdn.net/Java_Beginner27/article/details/81127102](https://blog.csdn.net/Java_Beginner27/article/details/81127102)
ES的建立索引过程：分词->语法处理（还原时态等等）->排序->创建索引。
其他暂不讨论，本文只讨论大小写问题。
如果创建index时mapping没有指定某个filed的标准化配置normalizer，那么如果写入ES的是大写，搜索出来看到的结果也是大写，但是创建的索引却是小写（可以用分词验证），以至于搜索的时候使用term会失败。
比如我的mapping为（内部使用，非完整mapping，重要是看字段）：
```bash
"mappings": {
          "rack_crm_community_type": {
            "dynamic": "false",
            "_all": {
              "enabled": false
            },
            "properties": {
              "claimBdPin": {
                "type": "string"
              },
              "createBdPin": {
                "type": "string"
              },
              "city": {
                "type": "integer"
              },
              "communityStatusCode": {
                "type": "integer"
              },
              "communityName": {
                "analyzer": "ik_max_word",
                "type": "string"
              },
              "location": {
                "type": "geo_point"
              },
              "communityId": {
                "type": "long"
              },
              "communityPersonNum": {
                "type": "integer"
              },
              "claimTime": {
                "format": "yyyy-MM-dd HH:mm:ss||yyyy-MM-dd||epoch_millis",
                "type": "date"
              }
            }
          }
        }
```
 以"claimBdPin"字段为例，只是指定了String类型。存储结果为：
![](https://img-blog.csdn.net/20180720103048599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0phdmFfQmVnaW5uZXIyNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当搜索条件为：
```bash
{
  "query":{
    "bool":{
      "filter":[
        {"term":  {"claimBdPin":"KA_liulin"}}
      ]
    }
  }
}
```
```
结果为空。
```
当搜索条件为：
```bash
{
  "query":{
    "bool":{
      "must":[
        {"match":  {"claimBdPin":"KA_liulin"}}
      ]
    }
  }
}
```
结果为：
![](https://img-blog.csdn.net/20180720103503187?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0phdmFfQmVnaW5uZXIyNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
能查询到，为什么呢？测试分词(ik_max_word)发现：KA_liulin分词为ka_liulin、ka、liulin三个分词。虽然存储的是KA_liulin，但是因为建立索引的时候会自动进行处理为ka_liulin（因为mapping未做分词），再创建索引。match查询的时候，会自动对参数进行处理，所以match搜索的时候使用KA_liulin（实际搜索为ka_liulin）能查询到，而term则查询不到。
因为mapping时未设置分词，所以一般使用term(过滤)来查询，所以要么在代码里面使用String.toLowerCase，但是太麻烦，那么要怎么一次性解决问题呢？我们可以mapping的时候设置normalizer（normalizer用于解析前的标准化配置，比如把所有的字符转化为小写等）
[https://www.elastic.co/guide/en/elasticsearch/reference/6.1/normalizer.html](https://www.elastic.co/guide/en/elasticsearch/reference/6.1/normalizer.html)
```bash
PUT index
{
  "settings": {
    "analysis": {
      "normalizer": {
        "my_normalizer": {
          "type": "custom",
          "char_filter": [],
          "filter": ["lowercase", "asciifolding"]
        }
      }
    }
  },
  "mappings": {
    "type": {
      "properties": {
        "foo": {
          "type": "keyword",
          "normalizer": "my_normalizer"
        }
      }
    }
  }
}
```
