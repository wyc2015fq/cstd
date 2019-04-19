# 实时搜索引擎Elasticsearch（3）——查询API的使用 - sxf_123456的博客 - CSDN博客
2018年03月16日 15:54:08[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：49标签：[elk](https://so.csdn.net/so/search/s.do?q=elk&t=blog)
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
[上一篇](http://blog.csdn.net/xialei199023/article/details/48085125)文章介绍了ES中的Rest API，本章将重点介绍ES中的查询API的使用。由于笔者在实际项目仅仅将ES用作索引数据库，并没有深入研究过ES的搜索功能。而且鉴于笔者的搜索引擎知识有限，本文将仅仅介绍ES简单（非全文）的查询API。
> 
笔者原本打算在本文中介绍聚合API的内容，但是写着写着发现文章有点过长，不便于阅读，故将聚合API的内容移至下一篇博客中。
### 引言
单单介绍理论和API是乏味和低效率的，本文将结合一个实际的例子来介绍这些API。下表是本文数据表的表结构，表名（type）为“student”。注意，studentNo是本表的id，也就是_id字段的值与studentNo的值保持一致。
|字段名|字段含义|类型|是否能被索引|备注|
|----|----|----|----|----|
|studentNo|学号|string|是|id|
|name|姓名|string|是||
|sex|性别|string|是||
|age|年龄|integer|是||
|birthday|出生年月|date|是||
|address|家庭住址|string|是||
|classNo|班级|string|是||
|isLeader|是否为班干部|boolean|是||
上面的表结构所对应的mapping如下，将数据保存在索引名为“student”的索引中。
```java
{
  "student": {
    "properties": {
      "studentNo": {
        "type": "string",
        "index": "not_analyzed"
      },
      "name": {
        "type": "string",
        "index": "not_analyzed"
      },
      "male": {
        "type": "string",
        "index": "not_analyzed"
      },
      "age": {
        "type": "integer"
      },
      "birthday": {
        "type": "date",
        "format": "yyyy-MM-dd"
      },
      "address": {
        "type": "string",
        "index": "not_analyzed"
      },
      "classNo": {
        "type": "string",
        "index": "not_analyzed "
      },
      "isLeader": {
        "type": "boolean"
      }
    }
  }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
索引中保存的数据如下，下面介绍的所有API都将基于这个数据表。
|studentNo|name|male|age|birthday|classNo|address|isLeader|
|----|----|----|----|----|----|----|----|
|1|刘备|男|24|1985-02-03|1|湖南省长沙市|true|
|2|关羽|男|22|1987-08-23|2|四川省成都市|false|
|3|糜夫人|女|19|1990-06-12|1|上海市|false|
|4|张飞|男|20|1989-07-30|3|北京市|false|
|5|诸葛亮|男|18|1992-04-27|2|江苏省南京市|true|
|6|孙尚香|女|16|1994-05-21|3||false|
|7|马超|男|19|1991-10-20|1|黑龙江省哈尔滨市|false|
|8|赵云|男|23|1986-10-26|2|浙江省杭州市|false|
查询API
ES中的查询非常灵活，为用户提供了非常方便而强大的API。个人觉得ES的调用接口设计得非常好，所有接口合理且风格一致，值得好好研究！
#### Query和Filter
ES为用户提供两类查询API，一类是在查询阶段就进行条件过滤的query查询，另一类是在query查询出来的数据基础上再进行过滤的filter查询。这两类查询的区别是：
- query方法会计算查询条件与待查询数据之间的相关性，计算结果写入一个score字段，类似于搜索引擎。filter仅仅做字符串匹配，不会计算相关性，类似于一般的数据查询，所以filter得查询速度比query快。
- filter查询出来的数据会自动被缓存，而query不能。
query和filter可以单独使用，也可以相互嵌套使用，非常灵活。
#### Query查询
下面的情况下适合使用query查询：
- 需要进行全文搜索。
- 查询结果依赖于相关性，即需要计算查询串和数据的相关性。
（1）Match All Query
查询所有的数据，相当于不带条件查询。下面的代码是一个典型的match_all查询的调用方式。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "match_all": {}
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
查询结果如下。其他所有的查询都是返回这种格式的数据。
```java
{
  "took": 156,                  // 查询耗时（毫秒）
  "timed_out": false,           // 是否超时
  "_shards": {
    "total": 5,                 // 总共查询的分片数
    "successful": 5,            // 查询成功的分片数
    "failed": 0                 // 查询失败的分片数
  },
  "hits": {
    "total": 8,                 // 本次查询的记录数
    "max_score": 1,             // 查询所有数据中的最大score
    "hits": [                   // 数据列表
      {
        "_index": "student",    // 数据所属的索引名
        "_type": "student",     // 数据所属的type
        "_id": "4",             // 数据的id值
        "_score": 1,            // 该记录的score
        "_source": {            // ES将原始数据保存到_source字段中
          "studentNo": "4",
          "name": "张飞",
          "male": "男",
          "age": "20",
          "birthday": "1989-07-30",
          "classNo": "3",
          "isLeader": "F"
        }
      },
      {
         ……                     // 其他的数据格式相同，就不列出来了
      }
    ]
  }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
查询时，你会发现无论数据量有多大，每次最多只能查到10条数据。这是因为ES服务端默认对查询结果做了分页处理，每页默认的大小为10。如果想自己指定查询的数据，可使用from和size字段，并且按指定的字段排序。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "match_all": {}
  },
  "from": 2,        // 从2条记录开始取
  "size": 4,        // 取4条数据
  "sort": {
    "studentNo": {  // 按studentNo字段升序
      "order": "asc"// 降序为desc
    }
  } 
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
> 
注意：不要把from设得过大（超过10000），否则会导致ES服务端因频繁GC而无法正常提供服务。其实实际项目中也没有谁会翻那么多页，但是为了ES的可用性，务必要对分页查询的页码做一定的限制。
（2）term query
词语查询，如果是对未分词的字段进行查询，则表示精确查询。查找名为“诸葛亮”的学生，查询结果为学号为5的记录。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "term": {
      "name": "诸葛亮"
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
（3）Bool Query
Bool（布尔）查询是一种复合型查询，它可以结合多个其他的查询条件。主要有3类逻辑查询：
- must：查询结果必须符合该查询条件（列表）。
- should：类似于in的查询条件。如果bool查询中不包含must查询，那么should默认表示必须符合查询列表中的一个或多个查询条件。
- must_not：查询结果必须不符合查询条件（列表）。
查找2班的班干部，查询结果为学号为5的记录。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "bool": {
      "must": [
        {
          "term": {
            "classNo": "2"
          }
        },
        {
          "term": {
            "isLeader": "true"
          }
        }
      ]
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
（4）Ids Query
id字段查询。查询数据id值为1和2的同学，由于id的值与studentNo相同，故查询结果为学号为1和2的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "ids": {
      "type": "student",
      "values": [
        "1",
        "2"
      ]
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
（5）Prefix Query
前缀查询。查找姓【赵】的同学，查询结果是学号为8的赵云。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "prefix": {
      "name": "赵"
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
（6）Range Query
范围查询，针对date和number类型的数据。查找年龄到18~20岁的同学，查询结果是学号为3、4、5、7的记录。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "range": {
      "age": {
        "gte": "18",     // 表示>=
        "lte": "20"      // 表示<=
      }
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
> 
实际上，对于date类型的数据，ES中以其时间戳（长整形）的形式存放的。
（7）Terms Query
多词语查询，查找符合词语列表的数据。如果要查询的字段索引为not_analyzed类型，则terms查询非常类似于关系型数据库中的in查询。下面查找学号为1，3的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "terms": {
      "studentNo": [
        "1",
        "3"
      ]
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
（8）Wildcard Query
通配符查询，是简化的正则表达式查询，包括下面两类通配符：
- * 代表任意（包括0个）多个字符
- ? 代表任意一个字符
查找名字的最后一个字是“亮”的同学，查询结果是学号为5的诸葛亮。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "wildcard": {
      "name": "*亮"
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
（9）Regexp Query同学
正则表达式查询，这是最灵活的字符串类型字段查询方式。查找家住长沙市的学生，查询结果为学号为1的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {
    "regexp": {
      "address": ".*长沙市.*"  // 这里的.号表示任意一个字符
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
#### Filter查询
下面的情况下适合使用filter查询：
- yes/no的二元查询
- 针对精确值进行查询
filter和query的查询方式有不少是重叠的，所以本节仅仅介绍API的调用，一些通用的注意的事项就不再重复了。
（1）Term Filter
词语查询，如果是对未分词的字段进行查询，则表示精确查询。查找名为“诸葛亮”的学生，查询结果为学号为5的记录。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {               
    "term": {
      "name": "诸葛亮",
      "_cache" : true // 与query主要是这里的区别，可以设置数据缓存
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
filter查询方式都可以通过设置_cache为true来缓存数据。如果下一次恰好以相同的查询条件进行查询并且该缓存没有过期，就可以直接从缓存中读取数据，这样就大大加快的查询速度。
（2）Bool Filter
查找2班的班干部，查询结果为学号为5的记录。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
    "bool": {
      "must": [
        {
          "term": {
            "classNo": "2"
          }
        },
        {
          "term": {
            "isLeader": "true"
          }
        }
      ]
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
（3）And Filter
And逻辑连接查询，连接1个或1个以上查询条件。它与bool查询中的must查询非常相似。实际上，and查询可以转化为对应的bool查询。查找2班的班干部，查询结果为学号为5的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
      "and": [
        {
          "term": {
            "classNo": "2"
          }
        },
        {
          "term": {
            "isLeader": "true"
          }
        }
      ]
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
（4）Or Filter
Or连接查询，表示逻辑或。。查找2班或者是班干部的学生名单，查询结果为学号为1、2、5、8的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
      "or": [
        {
          "term": {
            "classNo": "2"
          }
        },
        {
          "term": {
            "isLeader": "true"
          }
        }
      ]
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
（5）Exists Filter
存在查询，查询指定字段至少包含一个非null值的数据。如果字段索引为not_analyzed类型，则查询sql中的is not null查询方式。查询地址存在学生，查询结果为除了6之外的所有学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
    "exists": {
      "field": "address"
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
（6）Missing Filter
缺失值查询，与Exists查询正好相反。查询地址不存在的学生，查询结果为学号为6的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
    "missing": {
      "field": "address"
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
（7）Prefix Filter
前缀查询。查找姓【赵】的同学，查询结果是学号为8的赵云。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
    "prefix": {
      "name": "赵"
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
（8）Range Filter
范围查询，针对date和number类型的数据。查找年龄到18~20岁的同学，查询结果是学号为3、4、5、7的记录。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
    "range": {
      "age": {
        "gte": "18",
        "lte": "20"
      }
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
（9）Terms Filter
多词语查询，查找符合词语列表的数据。如果要查询的字段索引为not_analyzed类型，则terms查询非常类似于关系型数据库中的in查询。下面查找学号为1，3的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
    "terms": {
      "studentNo": [
        "1",
        "3"
      ]
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
（10）Regexp Filter
正则表达式查询，是最灵活的字符串类型字段查询方式。查找家住长沙市的学生，查询结果为学号为1的学生。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "filter": {
    "regexp": {
      "address": ".*长沙市.*"
    }
  }
}
'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
### 总结
本文介绍了ES中的部分查询API，这些API是一些常用的简单API，如果需要使用更加复杂一点的API，请查阅[官网文档](https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl.html)。下一篇文章准备介绍ES中的聚合API的使用。
