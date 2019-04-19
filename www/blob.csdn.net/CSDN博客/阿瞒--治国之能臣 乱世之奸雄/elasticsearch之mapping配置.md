# elasticsearch之mapping配置 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2017年11月28日 17:11:24[baofenny](https://me.csdn.net/jiaminbao)阅读数：762
                
本文主要记录es的schema mapping的一些配置项
mapping定义
{
  "mappings": {
    "post": {
      "properties": {                
        "id": {"type":"long", "store":"yes", "precision_step":"8" },
        "name": {"type":"string", "store":"yes", "index":"analyzed" },
        "published": {"type":"date", "store":"yes", "precision_step":"8" },
        "contents": {"type":"string", "store":"no", "index":"analyzed" }             
      }
    }
  }
}
或者
{
  "book" : {
    "_index" : { 
      "enabled" : true 
    },
    "_id" : {
      "index": "not_analyzed", 
      "store" : "yes"
    },
    "properties" : {
      "author" : {
        "type" : "string"
      },
      "characters" : {
        "type" : "string"
      },
      "copies" : {
        "type" : "long",
        "ignore_malformed" : false
      },
      "otitle" : {
        "type" : "string"
      },
      "tags" : {
        "type" : "string"
      },
      "title" : {
        "type" : "string",
        "fields":{
          "sort":{"type":"string","index":"not_analyzed"}
        }
      },
      "year" : {
        "type" : "long",
        "ignore_malformed" : false,
        "index" : "analyzed"
      },
      "available" : {
        "type" : "boolean"
      }
    }
  }
}
属性解说
index
可选值为analyzed(默认)和no，如果是字段是字符串类型的，则可以是not_analyzed.
store
可选值为yes或no，指定该字段的原始值是否被写入索引中，默认为no，即结果中不能返回该字段。
boost
默认为1，定义了文档中该字段的重要性，越高越重要
null_value
如果一个字段为null值(空数组或者数组都是null值)的话不会被索引及搜索到，null_value参数可以显示替代null values为指定值，这样使得字段可以被搜索到。
include_in_all
指定该字段是否应该包括在_all字段里头，默认情况下都会包含。
mapping操作
新建mapping
curl -s -XPOST '192.168.99.100:9200/library' --data-binary @mapping.json
更新mapping
curl -XPOST '192.168.99.100:9200/library/book/_mapping' -d'
{
    "book": {
        "properties": {
            "description": {
                "type": "string", 
                "store": "yes", 
                "index": "analyzed"
            }
        }
    }
}
'
查看mapping
curl -XGET '192.168.99.100:9200/library/book/_mapping?pretty'
返回
{
  "library" : {
    "mappings" : {
      "book" : {
        "properties" : {
          "author" : {
            "type" : "string"
          },
          "available" : {
            "type" : "boolean"
          },
          "characters" : {
            "type" : "string"
          },
          "copies" : {
            "type" : "long"
          },
          "description" : {
            "type" : "string",
            "store" : true
          },
          "otitle" : {
            "type" : "string"
          },
          "section" : {
            "type" : "long"
          },
          "tags" : {
            "type" : "string"
          },
          "title" : {
            "type" : "string"
          },
          "year" : {
            "type" : "long"
          }
        }
      }
    }
  }
}
可以修改的项：
增加新的类型定义
增加新的字段
增加新的分析器
不允许修改的项：
更改字段类型(比如文本改为数字)
更改存储为不存储，反之亦然
更改索引属性的值
更改已索引文档的分析器
注意的是新增字段或更改分析器之后，需要再次对所有文档进行索引重建
字段的数据类型
简单类型
string(指定分词器)
date(默认使用UTC保持,也可以使用format指定格式)
数值类型(byte,short,integer,long,float,double)
boolean
binary(存储在索引中的二进制数据的base64表示，比如图像，只存储不索引)
ip(以数字形式简化IPV4地址的使用，可以被索引、排序并使用IP值做范围查询).
有层级结构的类型
比如object 或者 nested.
特殊类型
比如geo_point, geo_shape, or completion.
            
