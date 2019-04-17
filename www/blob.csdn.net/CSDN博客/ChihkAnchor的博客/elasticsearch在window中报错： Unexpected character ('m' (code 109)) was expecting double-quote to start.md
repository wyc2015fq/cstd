# elasticsearch在window中报错："Unexpected character ('m' (code 109)): was expecting double-quote to start - ChihkAnchor的博客 - CSDN博客





2018年12月10日 22:44:33[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：545








elasticsearch从入坑到放弃

今天在配置问答系统中使用到了elasticsearch，由于在Linux下开发界面不友好，就直接在Windows下配置环境进行开发，结果一不小心入了一个坑，折腾了三个多小时才弄出来，所以以后还是要用Linux开发。。。

从官网下载ES [https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping.html)

解压直接bin目录下运行 elasticsearch.bat，

通过elasticsearch的mapping文件创建index和type时，总是报错"Failed to parse content to map，Unexpected character ('m' (code 109)): was expecting double-quote to start field name

上官网查询格式，

[https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping.html)

![](https://img-blog.csdnimg.cn/20181210223905747.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

官网生成的curl也是报错

curl -X PUT "localhost:9200/my_index" -H 'Content-Type: application/json' -d'

{

  "mappings": {

    "_doc": { 

      "properties": { 

        "title":    { "type": "text"  }, 

        "name":     { "type": "text"  }, 

        "age":      { "type": "integer" },  

        "created":  {

          "type":   "date", 

          "format": "strict_date_optional_time||epoch_millis"

        }

      }

    }

  }

}

'

最后发现在原因是Windows系统的问题：将curl后命令用双引号引用，json内容用三个双引号引用

curl -X PUT "localhost:9200/my_index" -H "Content-Type: application/json" -d"

{

  """mappings""": {

    """_doc""": { 

      """properties""": { 

        """title""":    { """type""": """text"""  }, 

        """name""":     { """type""": """text"""  }, 

        """age""":      { """type""": """integer""" },  

        """created""":  {

          """type""":   """date""", 

          """format""": """strict_date_optional_time||epoch_millis"""

        }

      }

    }

  }

}

"








