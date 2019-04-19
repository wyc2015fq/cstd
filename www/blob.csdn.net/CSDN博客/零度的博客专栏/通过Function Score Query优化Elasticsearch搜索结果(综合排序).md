# 通过Function Score Query优化Elasticsearch搜索结果(综合排序) - 零度的博客专栏 - CSDN博客
2018年09月11日 19:18:37[零度anngle](https://me.csdn.net/zmx729618)阅读数：280
版权声明：本文为博主原创文章，未经博主允许不得转载。 https://blog.csdn.net/wwd0501/article/details/78652850
在使用 Elasticsearch 进行全文搜索时，搜索结果默认会以文档的相关度进行排序，如果想要改变默认的排序规则，也可以通过`sort`指定一个或多个排序字段。
但是使用`sort`排序过于绝对，它会直接忽略掉文档本身的相关度（根本不会去计算）。在很多时候这样做的效果并不好，这时候就需要对多个字段进行综合评估，得出一个最终的排序。
###  function\_score
在 Elasticsearch 中`function_score`是用于处理文档分值的 DSL，它会在查询结束后对每一个匹配的文档进行一系列的重打分操作，最后以生成的最终分数进行排序。它提供了几种默认的计算分值的函数：
- `weight`：设置权重
- `field_value_factor`：将某个字段的值进行计算得出分数。
- `random_score`：随机得到 0 到 1 分数
- 衰减函数：同样以某个字段的值为标准，距离某个值越近得分越高
- 
`script_score`：通过自定义脚本计算分值
它还有一个属性`boost_mode`可以指定计算后的分数与原始的`_score`如何合并，有以下选项：
- 
`multiply`：将结果乘以`_score`
- `sum`：将结果加上`_score`
- `min`：取结果与`_score`的较小值
- `max`：取结果与`_score`的较大值
- 
`replace`：使结果替换掉`_score`
接下来本文将详细介绍这些函数的用法，以及它们的使用场景。
###  weight
weight 的用法最为简单，只需要设置一个数字作为权重，文档的分数就会乘以该权重。
他最大的用途应该就是和过滤器一起使用了，因为过滤器只会筛选出符合标准的文档，而不会去详细的计算每个文档的具体得分，所以只要满足条件的文档的分数都是 1，而 weight 可以将其更换为你想要的数值。
###  field\_value\_factor
field\_value\_factor 的目的是通过文档中某个字段的值计算出一个分数，它有以下属性：
- 
`field`：指定字段名
`factor`：对字段值进行预处理，乘以指定的数值（默认为 1）
- 
`modifier`将字段值进行加工，有以下的几个选项：
- `none`：不处理
- `log`：计算对数
- `log1p`：先将字段值 +1，再计算对数
- `log2p`：先将字段值 +2，再计算对数
- `ln`：计算自然对数
- `ln1p`：先将字段值 +1，再计算自然对数
- `ln2p`：先将字段值 +2，再计算自然对数
- `square`：计算平方
- `sqrt`：计算平方根
- `reciprocal`：计算倒数
举一个简单的例子，假设有一个商品索引，搜索时希望在相关度排序的基础上，销量（`sales`）更高的商品能排在靠前的位置，那么这条查询 DSL 可以是这样的：
|1234567891011121314151617|{  "query": {    "function_score": {      "query": {        "match": {          "title": "雨伞"        }      },      "field_value_factor": {        "field": "sales",        "modifier": "log1p",        "factor": 0.1      },      "boost_mode": "sum"    }  }}|
这条查询会将标题中带有雨伞的商品检索出来，然后对这些文档计算一个与库存相关的分数，并与之前相关度的分数相加，对应的公式为：
|1|_score = _score + log (1 + 0.1 * sales)|
Java实现：
- 
```java
String index = "wareic";
```
- 
```java
String type = "product";
```
- 
```java
SearchRequestBuilder searchRequestBuilder = client.prepareSearch(index).setTypes(type);
```
- 
```java
searchRequestBuilder.setSearchType(SearchType.DFS_QUERY_THEN_FETCH);
```
- 
```java
//分页
```
- 
```java
searchRequestBuilder.setFrom(0).setSize(10);
```
- 
```java
//explain为true表示根据数据相关度排序，和关键字匹配最高的排在前面
```
- 
```java
searchRequestBuilder.setExplain(true);
```
- 
- 
```java
BoolQueryBuilder queryBuilder = QueryBuilders.boolQuery();
```
- 
```java
queryBuilder.must(QueryBuilders.matchQuery("title", "雨伞"));
```
- 
- 
```java
ScoreFunctionBuilder<?> scoreFunctionBuilder = ScoreFunctionBuilders.fieldValueFactorFunction("sales").modifier(Modifier.LN1P).factor(0.1f);
```
- 
```java
FunctionScoreQueryBuilder query = QueryBuilders.functionScoreQuery(queryBuilder,scoreFunctionBuilder).boostMode(CombineFunction.SUM);
```
- 
- 
```java
searchRequestBuilder.setQuery(query);
```
- 
- 
```java
SearchResponse searchResponse = searchRequestBuilder.execute().get();
```
- 
```java
System.out.println(searchResponse.toString());
```
- 
- 
```java
long totalCount = searchResponse.getHits().getTotalHits();
```
- 
```java
System.out.println("总条数 totalCount:" + totalCount);
```
- 
- 
```java
//遍历结果数据
```
- 
```java
SearchHit[] hitList = searchResponse.getHits().getHits();
```
- 
```java
for (SearchHit hit : hitList) {
```
- 
```java
System.out.println("SearchHit hit string:" + hit.getSourceAsString());
```
- 
```java
}
```
###  random\_score
这个函数的使用相当简单，只需要调用一下就可以返回一个 0 到 1 的分数。
它有一个非常有用的特性是可以通过`seed`属性设置一个随机种子，该函数保证在随机种子相同时返回值也相同，这点使得它可以轻松地实现对于用户的个性化推荐。
###  衰减函数
衰减函数（Decay Function）提供了一个更为复杂的公式，它描述了这样一种情况：对于一个字段，它有一个理想的值，而字段实际的值越偏离这个理想值（无论是增大还是减小），就越不符合期望。这个函数可以很好的应用于数值、日期和地理位置类型，由以下属性组成：
- 原点（`origin`）：该字段最理想的值，这个值可以得到满分（1.0）
- 偏移量（`offset`）：与原点相差在偏移量之内的值也可以得到满分
- 衰减规模（`scale`）：当值超出了原点到偏移量这段范围，它所得的分数就开始进行衰减了，衰减规模决定了这个分数衰减速度的快慢
- 
衰减值（`decay`）：该字段可以被接受的值（默认为 0.5），相当于一个分界点，具体的效果与衰减的模式有关
例如我们想要买一样东西：
- 
它的理想价格是 50 元，这个值为原点
- 但是我们不可能非 50 元就不买，而是会划定一个可接受的价格范围，例如 45-55 元，±5 就为偏移量
- 
当价格超出了可接受的范围，就会让人觉得越来越不值。如果价格是 70 元，评价可能是不太想买，而如果价格是 200 元，评价则会是不可能会买，这就是由衰减规模和衰减值所组成的一条衰减曲线
或者如果我们想租一套房：
- 
它的理想位置是公司附近
- 如果离公司在 5km 以内，是我们可以接受的范围，在这个范围内我们不去考虑距离，而是更偏向于其他信息
- 
当距离超过 5km 时，我们对这套房的评价就越来越低了，直到超出了某个范围就再也不会考虑了
衰减函数还可以指定三种不同的模式：线性函数（linear）、以 e 为底的指数函数（Exp）和高斯函数（gauss），它们拥有不同的衰减曲线：
![衰减曲线](http://www.scienjus.com/uploads/2016/04/decay-function.png)
将上面提到的租房用 DSL 表示就是：
|12345678910111213141516171819|{  "query": {    "function_score": {      "query": {        "match": {          "title": "公寓"        }      },      "gauss": {        "location": {          "origin": { "lat": 40, "lon": 116 },          "offset": "5km",          "scale": "10km"           }         },         "boost_mode": "sum"    }  }}|
我们希望租房的位置在`40, 116`坐标附近，`5km`以内是满意的距离，`15km`以内是可以接受的距离。
衰减函数Java实现：
- 
```java
String index = "wareic";
```
- 
```java
String type = "product";
```
- 
```java
SearchRequestBuilder searchRequestBuilder = client.prepareSearch(index).setTypes(type);
```
- 
```java
searchRequestBuilder.setSearchType(SearchType.DFS_QUERY_THEN_FETCH);
```
- 
```java
//分页
```
- 
```java
searchRequestBuilder.setFrom(0).setSize(50);
```
- 
```java
//explain为每个匹配到的文档产生一大堆额外内容,设为 true就可以得到更详细的信息；
```
- 
```java
//输出 explain 结果代价是十分昂贵的，它只能用作调试工具 。千万不要用于生产环境
```
- 
```java
searchRequestBuilder.setExplain(false);
```
- 
- 
```java
BoolQueryBuilder queryBuilder = QueryBuilders.boolQuery();
```
- 
```java
queryBuilder.must(QueryBuilders.matchQuery("nicknames.nickname", "菜"));
```
- 
- 
```java
//原点（origin）：该字段最理想的值，这个值可以得到满分（1.0）
```
- 
```java
double origin = 200;
```
- 
```java
//偏移量（offset）：与原点相差在偏移量之内的值也可以得到满分
```
- 
```java
double offset = 30;
```
- 
```java
//衰减规模（scale）：当值超出了原点到偏移量这段范围，它所得的分数就开始进行衰减了，衰减规模决定了这个分数衰减速度的快慢
```
- 
```java
double scale = 40;
```
- 
```java
//衰减值（decay）：该字段可以被接受的值（默认为 0.5），相当于一个分界点，具体的效果与衰减的模式有关
```
- 
```java
double decay = 0.5;
```
- 
- 
```java
//高斯函数
```
- 
```java
// GaussDecayFunctionBuilder functionBuilder = ScoreFunctionBuilders.gaussDecayFunction("productID", origin, scale, offset, decay);
```
- 
```java
//以 e 为底的指数函数
```
- 
```java
ExponentialDecayFunctionBuilder functionBuilder = ScoreFunctionBuilders.exponentialDecayFunction("productID", origin, scale, offset, decay);
```
- 
```java
//线性函数
```
- 
```java
// LinearDecayFunctionBuilder functionBuilder = ScoreFunctionBuilders.linearDecayFunction("productID", origin, scale, offset, decay);
```
- 
```java
FunctionScoreQueryBuilder query = QueryBuilders.functionScoreQuery(queryBuilder,functionBuilder).boostMode(CombineFunction.SUM);
```
- 
- 
```java
searchRequestBuilder.setQuery(query);
```
- 
- 
```java
SearchResponse searchResponse = searchRequestBuilder.execute().get();
```
- 
```java
System.out.println(searchResponse.toString());
```
- 
- 
```java
long totalCount = searchResponse.getHits().getTotalHits();
```
- 
```java
System.out.println("总条数 totalCount:" + totalCount);
```
- 
- 
```java
//遍历结果数据
```
- 
```java
SearchHit[] hitList = searchResponse.getHits().getHits();
```
- 
```java
for (SearchHit hit : hitList) {
```
- 
```java
System.out.println("SearchHit hit string:" + hit.getSourceAsString());
```
- 
```java
}
```
###  script\_score
虽然强大的 field\_value\_factor 和衰减函数已经可以解决大部分问题了，但是也可以看出它们还有一定的局限性：
- 这两种方式都只能针对一个字段计算分值
- 
这两种方式应用的字段类型有限，field\_value\_factor 一般只用于数字类型，而衰减函数一般只用于数字、位置和时间类型
这时候就需要 script\_score 了，它支持我们自己编写一个脚本运行，在该脚本中我们可以拿到当前文档的所有字段信息，并且只需要将计算的分数作为返回值传回 Elasticsearch 即可。
注：使用脚本需要首先在配置文件中打开相关功能：
|12345|script.groovy.sandbox.enabled: truescript.inline: onscript.indexed: onscript.search: onscript.engine.groovy.inline.aggs: on|
举一个之前做不到的例子，假如我们有一个位置索引，它有一个分类（`category`）属性，该属性是字符串枚举类型，例如商场、电影院或者餐厅等。现在由于我们有一个电影相关的活动，所以需要将电影院在搜索列表中的排位相对靠前。
之前的两种方式都无法给字符串打分，但是如果我们自己写脚本的话却很简单，使用 Groovy（Elasticsearch 的默认脚本语言）也就是一行的事：
|1|return doc ['category'].value == '电影院' ? 1.1 : 1.0|
接下来只要将这个脚本配置到查询语句中就可以了：
|1234567891011121314|{  "query": {    "function_score": {      "query": {        "match": {          "name": "天安门"        }      },      "script_score": {        "script": "return doc ['category'].value == '电影院' ? 1.1 : 1.0"      }    }  }}|
或是将脚本放在`elasticsearch/config/scripts`下，然后在查询语句中引用它：
category-score.groovy：
|1|return doc ['category'].value == '电影院' ? 1.1 : 1.0|
|12345678910111213141516|{  "query": {    "function_score": {      "query": {        "match": {          "name": "天安门"        }      },      "script_score": {        "script": {         "file": "category-score"        }      }    }  }}|
在`script`中还可以通过`params`属性向脚本传值，所以为了解除耦合，上面的 DSL 还能接着改写为：
category-score.groovy：
|1|return doc ['category'].value == recommend_category ? 1.1 : 1.0|
|12345678910111213141516171819|{  "query": {    "function_score": {      "query": {        "match": {          "name": "天安门"        }      },      "script_score": {        "script": {         "file": "category-score",         "params": {            "recommend_category": "电影院"         }        }      }    }  }}|
这样就可以在不更改大部分查询语句和脚本的基础上动态修改推荐的位置类别了。
###  同时使用多个函数
上面的例子都只是调用某一个函数并与查询得到的`_score`进行合并处理，而在实际应用中肯定会出现在多个点上计算分值并合并，虽然脚本也许可以解决这个问题，但是应该没人愿意维护一个复杂的脚本吧。这时候通过多个函数将每个分值都计算出在合并才是更好的选择。
在 function\_score 中可以使用`functions`属性指定多个函数。它是一个数组，所以原有函数不需要发生改动。同时还可以通过`score_mode`指定各个函数分值之间的合并处理，值跟最开始提到的`boost_mode`相同。下面举两个例子介绍一些多个函数混用的场景。
第一个例子是类似于大众点评的餐厅应用。该应用希望向用户推荐一些不错的餐馆，特征是：范围要在当前位置的 5km 以内，有停车位是最重要的，有 Wi-Fi 更好，餐厅的评分（1 分到 5 分）越高越好，并且对不同用户最好展示不同的结果以增加随机性。
那么它的查询语句应该是这样的：
|12345678910111213141516171819202122232425262728293031323334353637383940414243444546|{  "query": {    "function_score": {      "filter": {        "geo_distance": {          "distance": "5km",          "location": {            "lat": $lat,            "lon": $lng          }        }      },      "functions": [        {          "filter": {            "term": {              "features": "wifi"            }          },          "weight": 1        },        {          "filter": {            "term": {              "features": "停车位"            }          },          "weight": 2        },        {            "field_value_factor": {               "field": "score",               "factor": 1.2             }        },        {          "random_score": {            "seed": "$id"          }        }      ],      "score_mode": "sum",      "boost_mode": "multiply"    }  }}|
注：其中所有以`$`开头的都是变量。
这样一个饭馆的最高得分应该是 2 分（有停车位）+ 1 分（有 wifi）+ 6 分（评分 5 分 \* 1.2）+ 1 分（随机评分）。
另一个例子是类似于新浪微博的社交网站。现在要优化搜索功能，使其以文本相关度排序为主，但是越新的微博会排在相对靠前的位置，点赞（忽略相同计算方式的转发和评论）数较高的微博也会排在较前面。如果这篇微博购买了推广并且是创建不到 24 小时（同时满足），它的位置会非常靠前。
|1234567891011121314151617181920212223242526272829303132333435363738|{  "query": {    "function_score": {      "query": {        "match": {          "content": "$text"        }      },      "functions": [        {          "gauss": {            "createDate": {              "origin": "$now",              "scale": "6d",              "offset": "1d"            }          }        },        {          "field_value_factor": {            "field": "like_count",            "modifier": "log1p",            "factor": 0.1          }        },        {          "script_score": {            "script": "return doc ['is_recommend'].value && doc ['create_date'] > time ? 1.5 : 1.0",            params: {                "time": $time            }          }        }      ],      "boost_mode": "multiply"    }  }}|
它的公式为：
|1|_score * gauss (create_date, $now, "1d", "6d") * log (1 + 0.1 * like_count) * is_recommend ? 1.5 : 1.0|
文章来源：[http://www.scienjus.com/elasticsearch-function-score-query/](http://www.scienjus.com/elasticsearch-function-score-query/)
