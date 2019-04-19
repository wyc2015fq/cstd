# 实时搜索引擎Elasticsearch（4）——Aggregations （聚合）API的使用 - sxf_123456的博客 - CSDN博客
2018年03月16日 14:27:17[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：78

转载：http://blog.csdn.net/xialei199023/article/details/48298635 
# [实时搜索引擎Elasticsearch（4）——Aggregations （聚合）API的使用](http://blog.csdn.net/xialei199023/article/details/48298635)
[上一篇](http://blog.csdn.net/xialei199023/article/details/48227247)博客介绍了ES中的简单查询API的使用，本篇将介绍ES提供的聚合API的使用。ES提供的聚合功能可以用来进行简单的数据分析。本文仍然以上一篇提供的数据为例来讲解。数据如下：
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
本文的主要内容有：- metric API的使用
- bucketing API的使用
- 两类API的嵌套使用
### 1. 聚合API
ES中的Aggregations API是从[Facets](https://www.elastic.co/guide/en/elasticsearch/reference/current/search-facets.html)功能基础上发展而来，官网正在进行替换计划，建议用户使用Aggregations API，而不是Facets API。ES中的聚合上可以分为下面两类：
- metric（度量）聚合：度量类型聚合主要针对的number类型的数据，需要ES做比较多的计算工作
- bucketing（桶）聚合：划分不同的“桶”，将数据分配到不同的“桶”里。非常类似sql中的group语句的含义。
metric既可以作用在整个数据集上，也可以作为bucketing的子聚合作用在每一个“桶”中的数据集上。当然，我们可以把整个数据集合看做一个大“桶”，所有的数据都分配到这个大“桶”中。
ES中的聚合API的调用格式如下：
```
"aggregations" : {                  // 表示聚合操作，可以使用aggs替代
    "<aggregation_name>" : {        // 聚合名，可以是任意的字符串。用做响应的key，便于快速取得正确的响应数据。
        "<aggregation_type>" : {    // 聚合类别，就是各种类型的聚合，如min等
            <aggregation_body>      // 聚合体，不同的聚合有不同的body
        }
        [,"aggregations" : { [<sub_aggregation>]+ } ]? // 嵌套的子聚合，可以有0或多个
    }
    [,"<aggregation_name_2>" : { ... } ]* // 另外的聚合，可以有0或多个
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
#### 1.1 度量类型（metric）聚合
（1）Min Aggregation
最小值查询，作用于number类型字段上。查询2班最小的年龄值。
```
curl -XPOST "192.168.1.101:9200/student/student/_search" -d 
'
{
  "query": {         // 可以先使用query查询得到需要的数据集
    "term": {
      "classNo": "2"
    }
  },
  "aggs": {
    "min_age": {
      "min": {
        "field": "age"
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
- 14
- 15
- 16
- 17
查询结果为：
```java
{
  "took": 19,                     // 前面部分数据与普通的查询数据相同
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 3,
    "max_score": 1.4054651,
    "hits": [
      {
        "_index": "student",
        "_type": "student",
        "_id": "2",
        "_score": 1.4054651,
        "_source": {
          "studentNo": "2",
          "name": "关羽",
          "male": "男",
          "age": "22",
          "birthday": "1987-08-23",
          "classNo": "2",
          "isLeader": "false"
        }
      },
      {
        "_index": "student",
        "_type": "student",
        "_id": "8",
        "_score": 1,
        "_source": {
          "studentNo": "8",
          "name": "赵云",
          "male": "男",
          "age": "23",
          "birthday": "1986-10-26",
          "classNo": "2",
          "isLeader": "false"
        }
      },
      {
        "_index": "student",
        "_type": "student",
        "_id": "5",
        "_score": 0.30685282,
        "_source": {
          "studentNo": "5",
          "name": "诸葛亮",
          "male": "男",
          "age": "18",
          "birthday": "1992-04-27",
          "classNo": "2",
          "isLeader": "true"
        }
      }
    ]
  },
  "aggregations": {                    // 聚合结果
    "min_age": {                       // 前面输入的聚合名
      "value": 18,                     // 聚合后的数据
      "value_as_string": "18.0"
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
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
上面的聚合查询有两个要注意的点：
- 可以通过query先过滤数据
- 返回的结果会包含聚合操作所作用的数据全集
有时候我们对作用的数据全集并不太敢兴趣，我们仅仅需要最终的聚合结果。可以通过查询类型（search_type）参数来实现这个需求。下面查询出来的数据量会大大减少，ES内部也会在查询时减少一些耗时的步骤，所以查询效率会提高。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d     // 注意这里的search_type=count
'
{
  "query": {             // 可以先使用query查询得到需要的数据集
    "term": {
      "classNo": "2"
    }
  },
  "aggs": {
    "min_age": {
      "min": {
        "field": "age"
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
- 14
- 15
- 16
- 17
本次的查询结果为：
```
{
...
"aggregations": {                    // 聚合结果
    "min_age": {                       // 前面输入的聚合名
      "value": 18,                     // 聚合后的数据
      "value_as_string": "18.0"
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
（2）Max Aggregation
最大值查询。下面查询2班最大的年龄值，查询结果为23。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "query": {
    "term": {
      "classNo": "2"
    }
  },
  "aggs": {
    "max_age": {
      "max": {
        "field": "age"
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
- 14
- 15
- 16
- 17
（3）Sum Aggregation
数值求和。下面统计查询2班的年龄总和，查询结果为63。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "query": {
    "term": {
      "classNo": "2"
    }
  },
  "aggs": {
    "sum_age": {
      "sum": {
        "field": "age"
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
- 14
- 15
- 16
- 17
（4）Avg Aggregation
计算平均值。下面计算查询2班的年龄平均值，结果为21。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "query": {
    "term": {
      "classNo": "2"
    }
  },
  "aggs": {
    "avg_age": {
      "avg": {
        "field": "age"
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
- 14
- 15
- 16
- 17
（5）Stats Aggregation
统计查询，一次性统计出某个字段上的常用统计值。下面对整个学校的学生进行简单地统计。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "aggs": {
    "stats_age": {
      "stats": {
        "field": "age"
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
查询结果为：
```java
{
  ...                     // 次要数据省略
  "aggregations": {
    "stats_age": {
      "count": 8,        // 含有年龄数据的学生计数
      "min": 16,         // 年龄最小值
      "max": 24,         // 年龄最大值
      "avg": 20.125,     // 年龄平均值
      "sum": 161,        // 年龄总和
      "min_as_string": "16.0",
      "max_as_string": "24.0",
      "avg_as_string": "20.125",
      "sum_as_string": "161.0"
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
（6）Top hits Aggregation
取符合条件的前n条数据记录。下面查询全校年龄排在前2位的学生，仅需返回学生姓名和年龄。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
{
  "aggs": {
    "top_age": {
      "top_hits": {
        "sort": [               // 排序
          {
            "age": {            // 按年龄降序
              "order": "desc"
            }
          }
        ],
        "_source": {
          "include": [           // 指定返回字段
            "name",
            "age"
          ]
        },
        "size": 2                 // 取前2条数据
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
返回结果为：
```java
{
  ...
  "aggregations": {
    "top_age": {
      "hits": {
        "total": 9,
        "max_score": null,
        "hits": [
          {
            "_index": "student",
            "_type": "student",
            "_id": "1",
            "_score": null,
            "_source": {
              "name": "刘备",
              "age": "24"
            },
            "sort": [
              24
            ]
          },
          {
            "_index": "student",
            "_type": "student",
            "_id": "8",
            "_score": null,
            "_source": {
              "name": "赵云",
              "age": "23"
            },
            "sort": [
              23
            ]
          }
        ]
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
- 37
- 38
- 39
- 40
#### 1.2 桶类型（bucketing）聚合
（1）Terms Aggregation
按照指定的1或多个字段将数据划分成若干个小的区间，计算落在每一个区间上记录数量，并按指定顺序进行排序。下面统计每个班的学生数，并按学生数从大到小排序，取学生数靠前的2个班级。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "aggs": {
    "terms_classNo": {
      "terms": {
        "field": "classNo",            // 按照班号进行分组
        "order": {                     // 按学生数从大到小排序
          "_count": "desc"
        },
        "size": 2                      // 取前两名
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
- 14
- 15
- 16
值得注意的，取得的前2名的学生数实际上是一个近似值，ES的实现方式参见[这里](https://www.elastic.co/guide/en/elasticsearch/reference/current/search-aggregations-bucket-terms-aggregation.html)。如果想要取得精确值，可以不指定size值，使其进行一次全排序，然后在程序中自行去取前2条记录。当然，这样做会使得ES做大量的排序运算工作，效率比较差。
（2）Range Aggregation
自定义区间范围的聚合，我们可以自己手动地划分区间，ES会根据划分出来的区间将数据分配不同的区间上去。下面将全校学生按照年龄划分为5个区间段：16岁以下、16~18、19~21、22~24、24岁以上，要求统计每一个年龄段内的学生数。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "aggs": {
    "range_age": {
      "range": {
        "field": "age",
        "ranges": [
          {
            "to": 15
          },
          {
            "from": "16",
            "to": "18"
          },
          {
            "from": "19",
            "to": "21"
          },
          {
            "from": "22",
            "to": "24"
          },
          {
            "from": "25"
          }
        ]
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
（3）Date Range Aggregation
时间区间聚合专门针对date类型的字段，它与Range Aggregation的主要区别是其可以使用时间运算表达式。主要包括+（加法）运算、-（减法）运算和/（四舍五入）运算，每种运算都可以作用在不同的时间域上面，下面是一些时间运算表达式示例。
- now+10y：表示从现在开始的第10年。
- now+10M：表示从现在开始的第10个月。
- 1990-01-10||+20y：表示从1990-01-01开始后的第20年，即2010-01-01。
- now/y：表示在年位上做舍入运算。今天是2015-09-06，则这个表达式计算结果为：2015-01-01。说好的rounding运算呢？结果是做的flooring运算，不知道为啥，估计是我理解错了-_-!!
下面查询25年前及更早出生的学生数。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "aggs": {
    "range_age": {
      "date_range": {
        "field": "birthday",
        "ranges": [
          {
            "to": "now-25y"
          }
        ]
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
- 14
- 15
- 16
- 17
（4）Histogram Aggregation
直方图聚合，它将某个number类型字段等分成n份，统计落在每一个区间内的记录数。它与前面介绍的Range聚合非常像，只不过Range可以任意划分区间，而Histogram做等间距划分。既然是等间距划分，那么参数里面必然有距离参数，就是interval参数。下面按学生年龄统计各个年龄段内的学生数量，分隔距离为2岁。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "aggs": {
    "histogram_age": {
      "histogram": {
        "field": "age",
        "interval": 2,               // 距离为2
        "min_doc_count": 1           // 只返回记录数量大于等于1的区间
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
- 14
（5）Date Histogram Aggregation
时间直方图聚合，专门对时间类型的字段做直方图聚合。这种需求是比较常用见得的，我们在统计时，通常就会按照固定的时间断（1个月或1年等）来做统计。下面统计学校中同一年出生的学生数。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "aggs": {
    "data_histogram_birthday": {
      "date_histogram": {
        "field": "birthday",
        "interval": "year",              // 按年统计
        "format": "yyyy"                 // 返回结果的key的格式
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
- 14
返回结果如下，可以看到由于上面的”format”: “yyyy”，所以返回的key_as_string只返回年的信息。
```java
{
  "buckets": [
    {
      "key_as_string": "1985",
      "key": 473385600000,
      "doc_count": 1
    },
    {
      "key_as_string": "1986",
      "key": 504921600000,
      "doc_count": 1
    },
    {
      "key_as_string": "1987",
      "key": 536457600000,
      "doc_count": 1
    },
    {
      "key_as_string": "1989",
      "key": 599616000000,
      "doc_count": 1
    },
    {
      "key_as_string": "1990",
      "key": 631152000000,
      "doc_count": 1
    },
    {
      "key_as_string": "1991",
      "key": 662688000000,
      "doc_count": 1
    },
    {
      "key_as_string": "1992",
      "key": 694224000000,
      "doc_count": 1
    },
    {
      "key_as_string": "1994",
      "key": 757382400000,
      "doc_count": 1
    }
  ]
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
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
（6）Missing Aggregation
值缺损聚合，它是一类单桶聚合，也就是最终只会产生一个“桶”。下面统计学生信息中地址栏缺损的记录数量。由于只有学号为6的孙尚香的地址缺损，所以统计值为1。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d 
'
{
  "aggs": {
    "missing_address": {
      "missing": {
        "field": "address"
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
#### 1.3 嵌套使用
前面已经说过，聚合操作是可以嵌套使用的。通过嵌套，可以使得metric类型的聚合操作作用在每一“桶”上。我们可以使用ES的嵌套聚合操作来完成稍微复杂一点的统计功能。下面统计每一个班里最大的年龄值。
```
curl -XPOST "192.168.1.101:9200/student/student/_search?search_type=count" -d
'
{
  "aggs": {
    "missing_address": {
      "terms": {
        "field": "classNo"
      },
      "aggs": {                 // 在这里嵌套新的子聚合
        "max_age": {
          "max": {              // 使用max聚合
            "field": "age"
          }
        }
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
- 14
- 15
- 16
- 17
- 18
- 19
返回结果如下：
```java
{
  "buckets": [
    {
      "key": "1",               // key是班级号
      "doc_count": 3,           // 每个班级内的人数
      "max_age": {              // 这里是我们指定的子聚合名
        "value": 24,            // 每班的年龄值
        "value_as_string": "24.0"
      }
    },
    {
      "key": "2",
      "doc_count": 3,
      "max_age": {
        "value": 23,
        "value_as_string": "23.0"
      }
    },
    {
      "key": "3",
      "doc_count": 1,
      "max_age": {
        "value": 20,
        "value_as_string": "20.0"
      }
    },
    {
      "key": "4",
      "doc_count": 1,
      "max_age": {
        "value": 16,
        "value_as_string": "16.0"
      }
    }
  ]
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
### 2. 总结
本文介绍了ES中的一些常用的聚合API的使用，包括metric、bucketing以及它们的嵌套使用方法。掌握了这些API就可以完成简单的数据统计功能，更多的API详见[官方文档](https://www.elastic.co/guide/en/elasticsearch/reference/current/search-aggregations.html)。前面的博客中都是介绍了ES的Rest API，接下来的文章中将会介绍Java API的使用，使用Java API可以实现前面介绍的所有API的功能。
