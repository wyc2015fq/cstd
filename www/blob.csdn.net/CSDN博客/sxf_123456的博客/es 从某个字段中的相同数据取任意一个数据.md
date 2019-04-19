# es 从某个字段中的相同数据取任意一个数据 - sxf_123456的博客 - CSDN博客
2017年08月16日 11:33:35[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：187
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
So，什么是字段折叠，可以理解就是按特定字段进行合并去重，比如我们有一个菜谱搜索，我希望按菜谱的“菜系”字段进行折叠，即返回结果每个菜系都返回一个结果，也就是按菜系去重，我搜索关键字“鱼”，要去返回的结果里面各种菜系都有，有湘菜，有粤菜，有中餐，有西餐，别全是湘菜，就是这个意思，通过按特定字段折叠之后，来丰富搜索结果的多样性。
说到这里，有人肯定会想到，使用 term agg+ top hits agg 来实现啊，这种组合两种聚和的方式可以实现上面的功能，不过也有一些局限性，比如，不能分页，[#4915](https://github.com/elastic/elasticsearch/issues/4915)；结果不够精确（top
 term+top hits，es 的聚合实现选择了牺牲精度来提高速度）；数据量大的情况下，聚合比较慢，影响搜索体验。
而新的的字段折叠的方式是怎么实现的的呢，有这些要点：
- 折叠+取 inner_hits 分两阶段执行（组合聚合的方式只有一个阶段），所以 top hits 永远是精确的。
- 字段折叠只在 top hits 层执行，不需要每次都在完整的结果集上对为每个折叠主键计算实际的 doc values 值，只对 top hits 这小部分数据操作就可以，和 term agg 相比要节省很多内存。
- 因为只在 top hits 上进行折叠，所以相比组合聚合的方式，速度要快很多。
- 折叠 top docs 不需要使用全局序列（global ordinals）来转换 string，相比 agg 这也节省了很多内存。
- 分页成为可能，和常规搜索一样，具有相同的局限，先获取 from+size 的内容，再合并。
- search_after 和 scroll 暂未实现，不过具备可行性。
-  折叠只影响搜索结果，不影响聚合，搜索结果的 total 是所有的命中纪录数，去重的结果数未知（无法计算）。
下面来看看具体的例子，就知道怎么回事了，使用起来很简单。
- 先准备索引和数据，这里以菜谱为例，name：菜谱名，type 为菜系，rating 为用户的累积平均评分
```
DELETE recipes
PUT recipes
POST recipes/type/_mapping
{
  "properties": {
    "name":{
      "type": "text"
    },
    "rating":{
      "type": "float"
    },"type":{
      "type": "keyword"
    }
  }
}
POST recipes/type/
{
  "name":"清蒸鱼头","rating":1,"type":"湘菜"
}
POST recipes/type/
{
  "name":"剁椒鱼头","rating":2,"type":"湘菜"
}
POST recipes/type/
{
  "name":"红烧鲫鱼","rating":3,"type":"湘菜"
}
POST recipes/type/
{
  "name":"鲫鱼汤（辣）","rating":3,"type":"湘菜"
}
POST recipes/type/
{
  "name":"鲫鱼汤（微辣）","rating":4,"type":"湘菜"
}
POST recipes/type/
{
  "name":"鲫鱼汤（变态辣）","rating":5,"type":"湘菜"
}
POST recipes/type/
{
  "name":"广式鲫鱼汤","rating":5,"type":"粤菜"
}
POST recipes/type/
{
  "name":"鱼香肉丝","rating":2,"type":"川菜"
}
POST recipes/type/
{
  "name":"奶油鲍鱼汤","rating":2,"type":"西菜"
}
```
- 现在我们看看普通的查询效果是怎么样的，搜索关键字带“鱼”的菜，返回3条数据
```
POST recipes/type/_search
{
  "query": {"match": {
    "name": "鱼"
  }},"size": 3
}
```
全是湘菜，我的天，最近上火不想吃辣，这个第一页的结果对我来说就是垃圾，如下：
```
{
  "took": 2,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 9,
    "max_score": 0.26742277,
    "hits": [
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHYF_OA-dG63Txsd",
        "_score": 0.26742277,
        "_source": {
          "name": "鲫鱼汤（变态辣）",
          "rating": 5,
          "type": "湘菜"
        }
      },
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHXO_OA-dG63Txsa",
        "_score": 0.19100356,
        "_source": {
          "name": "红烧鲫鱼",
          "rating": 3,
          "type": "湘菜"
        }
      },
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHWy_OA-dG63TxsZ",
        "_score": 0.19100356,
        "_source": {
          "name": "剁椒鱼头",
          "rating": 2,
          "type": "湘菜"
        }
      }
    ]
  }
}
```
我们再看看，这次我想加个评分排序，大家都喜欢的是那些，看看有没有喜欢吃的，执行查询：
```
POST recipes/type/_search
{
  "query": {"match": {
    "name": "鱼"
  }},"sort": [
    {
      "rating": {
        "order": "desc"
      }
    }
  ],"size": 3
}
```
结果稍微好点了，不过3个里面2个是湘菜，还是有点不合适，结果如下：
```
{
  "took": 1,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 9,
    "max_score": null,
    "hits": [
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHYF_OA-dG63Txsd",
        "_score": null,
        "_source": {
          "name": "鲫鱼汤（变态辣）",
          "rating": 5,
          "type": "湘菜"
        },
        "sort": [
          5
        ]
      },
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHYW_OA-dG63Txse",
        "_score": null,
        "_source": {
          "name": "广式鲫鱼汤",
          "rating": 5,
          "type": "粤菜"
        },
        "sort": [
          5
        ]
      },
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHX7_OA-dG63Txsc",
        "_score": null,
        "_source": {
          "name": "鲫鱼汤（微辣）",
          "rating": 4,
          "type": "湘菜"
        },
        "sort": [
          4
        ]
      }
    ]
  }
}
```
现在我知道了，我要看看其他菜系，这家不是还有西餐、广东菜等各种菜系的么，来来，帮我每个菜系来一个菜看看，换 terms agg 先得到唯一的 term 的 bucket，再组合 top_hits agg，返回按评分排序的第一个 top
 hits，有点复杂，没关系，看下面的查询就知道了：
```
GET recipes/type/_search
{
  "query": {
    "match": {
      "name": "鱼"
    }
  },
  "sort": [
    {
      "rating": {
        "order": "desc"
      }
    }
  ],"aggs": {
    "type": {
      "terms": {
        "field": "type",
        "size": 10
      },"aggs": {
        "rated": {
          "top_hits": {
            "sort": [{
              "rating": {"order": "desc"}
            }], 
            "size": 1
          }
        }
      }
    }
  }, 
  "size": 0,
  "from": 0
}
```
看下面的结果，虽然 json 结构有点复杂，不过总算是我们想要的结果了，湘菜、粤菜、川菜、西菜都出来了，每样一个，不重样：
```
{
  "took": 4,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 9,
    "max_score": 0,
    "hits": []
  },
  "aggregations": {
    "type": {
      "doc_count_error_upper_bound": 0,
      "sum_other_doc_count": 0,
      "buckets": [
        {
          "key": "湘菜",
          "doc_count": 6,
          "rated": {
            "hits": {
              "total": 6,
              "max_score": null,
              "hits": [
                {
                  "_index": "recipes",
                  "_type": "type",
                  "_id": "AVoESHYF_OA-dG63Txsd",
                  "_score": null,
                  "_source": {
                    "name": "鲫鱼汤（变态辣）",
                    "rating": 5,
                    "type": "湘菜"
                  },
                  "sort": [
                    5
                  ]
                }
              ]
            }
          }
        },
        {
          "key": "川菜",
          "doc_count": 1,
          "rated": {
            "hits": {
              "total": 1,
              "max_score": null,
              "hits": [
                {
                  "_index": "recipes",
                  "_type": "type",
                  "_id": "AVoESHYr_OA-dG63Txsf",
                  "_score": null,
                  "_source": {
                    "name": "鱼香肉丝",
                    "rating": 2,
                    "type": "川菜"
                  },
                  "sort": [
                    2
                  ]
                }
              ]
            }
          }
        },
        {
          "key": "粤菜",
          "doc_count": 1,
          "rated": {
            "hits": {
              "total": 1,
              "max_score": null,
              "hits": [
                {
                  "_index": "recipes",
                  "_type": "type",
                  "_id": "AVoESHYW_OA-dG63Txse",
                  "_score": null,
                  "_source": {
                    "name": "广式鲫鱼汤",
                    "rating": 5,
                    "type": "粤菜"
                  },
                  "sort": [
                    5
                  ]
                }
              ]
            }
          }
        },
        {
          "key": "西菜",
          "doc_count": 1,
          "rated": {
            "hits": {
              "total": 1,
              "max_score": null,
              "hits": [
                {
                  "_index": "recipes",
                  "_type": "type",
                  "_id": "AVoESHY3_OA-dG63Txsg",
                  "_score": null,
                  "_source": {
                    "name": "奶油鲍鱼汤",
                    "rating": 2,
                    "type": "西菜"
                  },
                  "sort": [
                    2
                  ]
                }
              ]
            }
          }
        }
      ]
    }
  }
}
```
上面的实现方法，前面已经说了，可以做，有局限性，那看看新的字段折叠法如何做到呢，查询如下，加一个 collapse 参数，指定对那个字段去重就行了，这里当然对菜系“type”字段进行去重了：
```
GET recipes/type/_search
{
  "query": {
    "match": {
      "name": "鱼"
    }
  },
  "collapse": {
    "field": "type"
  },
  "size": 3,
  "from": 0
}
```
结果很理想嘛，命中结果还是熟悉的那个味道（和查询结果长的一样嘛），如下：
```
{
  "took": 1,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 9,
    "max_score": null,
    "hits": [
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoDNlRJ_OA-dG63TxpW",
        "_score": 0.018980097,
        "_source": {
          "name": "鲫鱼汤（微辣）",
          "rating": 4,
          "type": "湘菜"
        },
        "fields": {
          "type": [
            "湘菜"
          ]
        }
      },
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoDNlRk_OA-dG63TxpZ",
        "_score": 0.013813315,
        "_source": {
          "name": "鱼香肉丝",
          "rating": 2,
          "type": "川菜"
        },
        "fields": {
          "type": [
            "川菜"
          ]
        }
      },
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoDNlRb_OA-dG63TxpY",
        "_score": 0.0125863515,
        "_source": {
          "name": "广式鲫鱼汤",
          "rating": 5,
          "type": "粤菜"
        },
        "fields": {
          "type": [
            "粤菜"
          ]
        }
      }
    ]
  }
}
```
我再试试翻页，把 from 改一下，现在返回了3条数据，from 改成3，新的查询如下：
```
{
  "took": 1,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 9,
    "max_score": null,
    "hits": [
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoDNlRw_OA-dG63Txpa",
        "_score": 0.012546891,
        "_source": {
          "name": "奶油鲍鱼汤",
          "rating": 2,
          "type": "西菜"
        },
        "fields": {
          "type": [
            "西菜"
          ]
        }
      }
    ]
  }
}
```
上面的结果只有一条了，去重之后本来就只有4条数据，上面的工作正常，每个菜系只有一个菜啊，那我不乐意了，帮我每个菜系里面多返回几条，我好选菜啊，加上参数 inner_hits 来控制返回的条数，这里返回2条，按 rating 也排个序，新的查询构造如下：
```
GET recipes/type/_search
{
  "query": {
    "match": {
      "name": "鱼"
    }
  },
  "collapse": {
    "field": "type",
    "inner_hits": {
      "name": "top_rated",
      "size": 2,
      "sort": [
        {
          "rating": "desc"
        }
      ]
    }
  },
  "sort": [
    {
      "rating": {
        "order": "desc"
      }
    }
  ],
  "size": 2,
  "from": 0
}
```
查询结果如下，完美：
```
{
  "took": 1,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 9,
    "max_score": null,
    "hits": [
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHYF_OA-dG63Txsd",
        "_score": null,
        "_source": {
          "name": "鲫鱼汤（变态辣）",
          "rating": 5,
          "type": "湘菜"
        },
        "fields": {
          "type": [
            "湘菜"
          ]
        },
        "sort": [
          5
        ],
        "inner_hits": {
          "top_rated": {
            "hits": {
              "total": 6,
              "max_score": null,
              "hits": [
                {
                  "_index": "recipes",
                  "_type": "type",
                  "_id": "AVoESHYF_OA-dG63Txsd",
                  "_score": null,
                  "_source": {
                    "name": "鲫鱼汤（变态辣）",
                    "rating": 5,
                    "type": "湘菜"
                  },
                  "sort": [
                    5
                  ]
                },
                {
                  "_index": "recipes",
                  "_type": "type",
                  "_id": "AVoESHX7_OA-dG63Txsc",
                  "_score": null,
                  "_source": {
                    "name": "鲫鱼汤（微辣）",
                    "rating": 4,
                    "type": "湘菜"
                  },
                  "sort": [
                    4
                  ]
                }
              ]
            }
          }
        }
      },
      {
        "_index": "recipes",
        "_type": "type",
        "_id": "AVoESHYW_OA-dG63Txse",
        "_score": null,
        "_source": {
          "name": "广式鲫鱼汤",
          "rating": 5,
          "type": "粤菜"
        },
        "fields": {
          "type": [
            "粤菜"
          ]
        },
        "sort": [
          5
        ],
        "inner_hits": {
          "top_rated": {
            "hits": {
              "total": 1,
              "max_score": null,
              "hits": [
                {
                  "_index": "recipes",
                  "_type": "type",
                  "_id": "AVoESHYW_OA-dG63Txse",
                  "_score": null,
                  "_source": {
                    "name": "广式鲫鱼汤",
                    "rating": 5,
                    "type": "粤菜"
                  },
                  "sort": [
                    5
                  ]
                }
              ]
            }
          }
        }
      }
    ]
  }
}
```
好了，字段折叠介绍就到这里。
https://elasticsearch.cn/question/263
