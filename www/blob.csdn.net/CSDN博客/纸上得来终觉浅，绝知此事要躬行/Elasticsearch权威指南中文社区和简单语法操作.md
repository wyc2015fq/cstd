# Elasticsearch权威指南中文社区和简单语法操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月10日 09:30:21[boonya](https://me.csdn.net/boonya)阅读数：303








**目录**

[Elasticsearch: 权威指南](#Elasticsearch%3A%20%E6%9D%83%E5%A8%81%E6%8C%87%E5%8D%97)

[Elasticsearch：中文社区](#Elasticsearch%EF%BC%9A%E4%B8%AD%E6%96%87%E7%A4%BE%E5%8C%BA)

[Elasticsearch简单操作](#Elasticsearch%E7%AE%80%E5%8D%95%E6%93%8D%E4%BD%9C)

[Index索引](#Index%E7%B4%A2%E5%BC%95)

[Document文档](#Document%E6%96%87%E6%A1%A3)

[自定义match查询(实现任意全文搜索)](#%E8%87%AA%E5%AE%9A%E4%B9%89match%E6%9F%A5%E8%AF%A2(%E5%AE%9E%E7%8E%B0%E4%BB%BB%E6%84%8F%E5%85%A8%E6%96%87%E6%90%9C%E7%B4%A2))

[bool查询（OR AND NOT）](#bool%E6%9F%A5%E8%AF%A2%EF%BC%88OR%20AND%20NOT%EF%BC%89)

[Range查询](#Range%E6%9F%A5%E8%AF%A2)

[term 查询被用于精确值匹配](#term%20%E6%9F%A5%E8%AF%A2%E8%A2%AB%E7%94%A8%E4%BA%8E%E7%B2%BE%E7%A1%AE%E5%80%BC%E5%8C%B9%E9%85%8D)

[文档插入的JavaScript实例](#%E6%96%87%E6%A1%A3%E6%8F%92%E5%85%A5%E7%9A%84JavaScript%E5%AE%9E%E4%BE%8B)

# Elasticsearch: 权威指南

[https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/index.html](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/index.html)

Clinton Gormley     Zachary Tong

Copyright © 2014, 2015, 2016 Elasticsearch



本作品采用 [署名-非商业性使用-禁止演绎 3.0 未本地化版本](http://creativecommons.org/licenses/by-nc-nd/3.0/) 许可协议。

**摘要**

当前我们正在为这本书更新至最新的版本，有些内容可能已经过时。

一旦本书完成，你可以从 O'Reilly Media 购买纸质印刷版 : [O'Reilly Media 购买链接](http://www.oreilly.com/catalog/0636920028505)

我们欢迎提交反馈 – 如果你遇到任何错误或是修改建议，请前往 [Github](https://github.com/elasticsearch-cn/elasticsearch-definitive-guide/issues) 仓库提交一个 issue。
- [序言](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/foreword_id.html)
- [前言](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/preface.html)
- [基础入门](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/getting-started.html)
- [深入搜索](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/search-in-depth.html)
- [处理人类语言](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/languages.html)
- [聚合](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/aggregations.html)
- [地理位置](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/geoloc.html)
- [数据建模](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/modeling-your-data.html)
- [管理、监控和部署](https://elasticsearch.cn/book/elasticsearch_definitive_guide_2.x/administration.html)

 更多了解：

[https://es.xiaoleilu.com/](https://es.xiaoleilu.com/)

![](https://img-blog.csdn.net/20180910092540206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Elasticsearch：中文社区

[https://github.com/elasticsearch-cn](https://github.com/elasticsearch-cn)

![](https://img-blog.csdn.net/20180910092801738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

客户端开发包支持：Java、C#、Python、JavaScript 、PHP、Perl、Ruby、SQL...

![](https://img-blog.csdn.net/20180910093507222?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Elasticsearch简单操作

注：在Elasticsearch6.0以上版本，插入的文档类型需要添加`**-H'Content-Type: application/json'**标识，请参考：`[https://www.elastic.co/blog/strict-content-type-checking-for-elasticsearch-rest-requests](https://www.elastic.co/blog/strict-content-type-checking-for-elasticsearch-rest-requests)

下面Elasticsearch操作来自:[https://www.cnblogs.com/hello-daocaoren/p/8430582.html](https://www.cnblogs.com/hello-daocaoren/p/8430582.html)

## Index索引

创建一个索引:
`curl -XPUT localhost:9200/user`
查询一个索引:
`curl -XGET localhost:9200/user?pretty=true`
返回所有索引:
`curl -XGET http://localhost:9200/_cat/indices?v`
删除索引:
`curl -XDELETE localhost:9200/user`
## Document文档

指定ID新增文档:在user索引在新建类型为admin_user的文档，1是这个文档的id。
`curl -H "Content-Type: application/json" -XPOST localhost:9200/user/admin_user/1?pretty=true  -d '{"userName": "张三","title": "工程师","desc": "数据库管理"}'`
不指定ID新增文档:在user索引在新建类型为admin_user的文档
`curl -H "Content-Type: application/json" -XPOST localhost:9200/user/admin_user/?pretty=true -d '{"userName" : "李四","title" : "工程师","desc" : "网络管理"}'`
根据文档ID查询文档
`curl -XGET localhost:9200/user/admin_user/1?pretty=true`
查询所有文档
`curl -XGET localhost:9200/user/admin_user/_search?pretty=true`
## 自定义match查询(实现任意全文搜索)

===单字段查询:

```
curl -XGET localhost:9200/user/admin_user/_search?pretty=true -d
{
    "query":{"match":{"userName":"张三"}}
}
```



===控制返回记录数:

```
curl -XGET localhost:9200/user/admin_user/_search?pretty=true -d
{
    "query":{"match":{"userName":"张三"}},
    "size":20
}
```



===多字段查询:

```
curl -XGET localhost:9200/user/admin_user/_search?pretty=true -d
    {
    "query":{
        "multi_match":{
            "query":"张三",
            "fields": ["userName","title"]
            }
           }
    }
```

## bool查询（OR AND NOT）

单个字段匹配：

```
curl -XGET localhost:9200/user/admin_user/_search?pretty=true -d
{
    "query":{"match":{"userName":"三 四"}}
}
```

多个字段匹配：


```
curl -XGET localhost:9200/user/admin_user/_search?pretty=true -d
    {
      "query": {
        "bool": {
          "must": [
          { "match": { "userName": "三" } },
          { "match": { "title": "师" } }
        ]
    }
  }
}
```

bool查询语法字段：
-  must：所有的语句都 必须（must） 匹配，与 AND 等价。
-  must_not：所有的语句都 不能（must not） 匹配，与 NOT 等价。
- should：至少有一个语句要匹配，与 OR 等价。

```
curl -XGET localhost:9200/user/admin_user/_search?pretty=true -d
    {
      "query": {
        "bool": {
          "must":[
            { "match": { "desc": "管理" } }
          ],
           "should":[
              {"match" : {"title":"师"}} ,
              {"match": { "userName": "三" }}
          ]
        }
      }
    }
```

## Range查询

Range操作符如下：
-     gt： 大于
-     gte：大于等于
-     lt： 小于
-     lte：小于等于

设置数据：

```
curl -H "Content-Type: application/json" -XPOST localhost:9200/user/admin_user/?pretty=true -d
    {
     "userName" : "李四",
     "title" : "工程师",
     "desc" : "网络管理",
     "age" : 27
    }
```

查询age在20到30之间的记录:

```
curl -XGET localhost:9200/user/admin_user/_search?pretty=ture -d
    {
    "query":{
        "range":{
                    "age" : { "gte":20, "lt":30 }
         }
      }
    }
```

## term 查询被用于精确值匹配



在match查询中，会对输入值进行分词，然后再去匹配。term查询则没有分词这个过程，输入值是什么就去匹配什么。

```
curl -XGET localhost:9200/user/admin_user/_search?pretty=true -d
    {
    "query":{"term":{"desc":"网络,管理"}},
    "size": 12
    }
```

**注意：Elasticsearch在Windows下操作json格式要通过验证必须是如下格式**
`curl -H "Content-Type: application/json" -XPOST localhost:9200/user/test_user?pretty=true -d "{"""name""":"""boonya"""}"`
否则会报如下错误 ：

```
{
  "error" : {
    "root_cause" : [
      {
        "type" : "mapper_parsing_exception",
        "reason" : "failed to parse"
      }
    ],
    "type" : "mapper_parsing_exception",
    "reason" : "failed to parse",
    "caused_by" : {
      "type" : "not_x_content_exception",
      "reason" : "Compressor detection can only be called on some xcontent bytes or compressed xcontent bytes"
    }
  },
  "status" : 400
}
```

## 文档插入的JavaScript实例

参考：[https://www.elastic.co/blog/strict-content-type-checking-for-elasticsearch-rest-requests](https://www.elastic.co/blog/strict-content-type-checking-for-elasticsearch-rest-requests)

```
<html>
  <body>
    <script src="https://code.jquery.com/jquery-3.2.1.min.js"
            type="text/javascript"></script>
    <script type="text/javascript">
      $(function() {
        $.ajax({
          url: "http://localhost:9200/visitors/doc/",
          type:'POST',
          data: JSON.stringify({ browser: navigator.userAgent,
                                 date: new Date() }),
          contentType: 'text/plain'
        });
      });
    </script>
  </body>
</html>
```

或

```
<html>
  <body>
    <script src="https://code.jquery.com/jquery-3.2.1.min.js"
            type="text/javascript"></script>
    <script type="text/javascript">
      $(function() {
        $.ajax({
          url: "http://localhost:9200/visitors/doc/",
          type:'POST',
          data: JSON.stringify({ browser: navigator.userAgent,
                                 date: new Date() }),
          contentType: 'application/json'
        });
      });
    </script>
  </body>
</html>
```

更多阅读：

[Elasticsearch 6.1官方入门教程](https://blog.csdn.net/hololens/article/details/78932628)

[全文搜索引擎 Elasticsearch 入门教程](http://www.ruanyifeng.com/blog/2017/08/elasticsearch.html)

[eBay Elasticsearch性能优化实践](https://blog.csdn.net/dev_csdn/article/details/79044930)

[全文搜索引擎ElasticSearch新手实战教程](https://blog.csdn.net/kwame211/article/details/77374776)



