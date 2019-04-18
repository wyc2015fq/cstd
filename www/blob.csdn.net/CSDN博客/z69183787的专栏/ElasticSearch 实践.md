# ElasticSearch 实践 - z69183787的专栏 - CSDN博客
2017年04月10日 10:22:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：876
版本：2.3.5
版本：5.3.0 （应为3.X，随ELK版本统一，故从5.X 开始）
Windows端
1、官网下载
https://www.elastic.co/downloads/past-releases/elasticsearch-2-3-5
2、安装head插件
进入目录E:\elasticsearch-2.3.5\bin>
执行
`plugin.bat install mobz/elasticsearch-head`
![](https://img-blog.csdn.net/20170410103455017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装完毕
3、启动
`E:\elasticsearch-2.3.5\bin>elasticsearch.bat`
![](https://img-blog.csdn.net/20170410103753520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
验证（安装curl，注意windows cmd中使用curl不能换行，所以采用cygwin + curl 解决）
可参考：[http://blog.csdn.net/carlwu/article/details/53535954](http://blog.csdn.net/carlwu/article/details/53535954)
问题：加入环境变量或放入system32目录
1、
```
C:\Users\Administrator>curl "http://localhost:9200/?pretty"
{
  "name" : "Bishop",
  "cluster_name" : "elasticsearch",
  "version" : {
    "number" : "2.3.5",
    "build_hash" : "90f439ff60a3c0f497f91663701e64ccd01edbb4",
    "build_timestamp" : "2016-07-27T10:36:52Z",
    "build_snapshot" : false,
    "lucene_version" : "5.5.0"
  },
  "tagline" : "You Know, for Search"
}
```
注意Windows 地址可以不需要引号或使用双引号；
2、访问 http://localhost:9200/_plugin/head/
![](https://img-blog.csdn.net/20170410105042735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装成功！
4、测试
自定义索引信息
索引名：zhou；索引类型：shun；字段：（userName，age，car）
创建索引：
```
C:\Users\Administrator>curl -XPUT http://localhost:9200/zhou/
{"acknowledged":true}
```
```bash
C:\Users\Administrator>curl -XGET http://localhost:9200/zhou/?pretty
{
  "zhou" : {
    "aliases" : { },
    "mappings" : { },
    "settings" : {
      "index" : {
        "creation_date" : "1491795195654",
        "uuid" : "8KtP-8HCR9er7bnsAlbbbw",
        "number_of_replicas" : "1",
        "number_of_shards" : "5",
        "version" : {
          "created" : "2030599"
        }
      }
    },
    "warmers" : { }
  }
}
"number_of_replicas" : "1",
        "number_of_shards" : "5",
        "version" : {
          "created" : "2030599"
        }
      }
    },
    "warmers" : { }
  }
}
```
标红处可看到索引默认的分片和副本分别为5和1
创建索引时 可以指定分片和副本
JSON：
```
C:\Users\Administrator>curl -XPUT http://localhost:9200/zhou/ -d {"settings":{"index":{"number_of_shards":3,"number_of_r
eplicas":2}}}
{"acknowledged":true}
C:\Users\Administrator>curl http://localhost:9200/zhou/?pretty
{
  "zhou" : {
    "aliases" : { },
    "mappings" : { },
    "settings" : {
      "index" : {
        "creation_date" : "1491807780791",
        "uuid" : "9n2r_fUVSXO53CKKgYOcnw",
        "number_of_replicas" : "2",
        "number_of_shards" : "3",
        "version" : {
          "created" : "2030599"
        }
      }
    },
    "warmers" : { }
  }
}
```
由于Windows Dos 不支持换行符，so
```
curl -XPUT http://localhost:9200/zhou/ -d 
index:
  number_of_shards:3
  number_of_replicas:2
```
索引mapping，创建了索引类型为shun 的 映射结构
```
$ curl -XPUT http://localhost:9200/zhou/_mappings/shun?pretty -d '
> {
>     "properties":{
>         "userName":{
>             "type":"string",
>             "index":"analyzed"
>         },
>         "age":{
>             "type":"integer",
>             "fields":{
>                 "age":{
>                     "type":"integer"
>                 },
>                 "raw":{
>                     "type":"string",
>                     "index":"not_analyzed"
>                 }
>             }
>         },
>         "car":{
>             "type":"string",
>             "index":"analyzed"
>         }
>     }
> } '
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   532  100    28  100   504    595  10723 --:--:-- --:--:-- --:--:-- 10723{
  "acknowledged" : true
}
```
索引插入：PUT因为幂等性，适用于指定主键插入，等同于全量更新；POST 指定主键，自动主键均可
自动主键
```
$ curl XPOST 'http://localhost:9200/zhou/shun?pretty' -d '
{
    "userName":"zhou shun",
    "age":10,
    "car":[
        "benz",
        "bmw"
    ]
}'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   287  100   193  100    94   1026    500 --:--:-- --:--:-- --:--:--  1026{
  "_index" : "zhou",
  "_type" : "shun",
  "_id" : "AVtW9_74Wik3CeNre3WI",
  "_version" : 1,
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "created" : true
}
```
指定主键：
```
$ curl -XPUT 'localhost:9200/zhou/shun/9999?pretty' -d '{
"userName":"bva","age":55,"car":"bbbbb"}'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   219  100   177  100    42   2809    666 --:--:-- --:--:-- --:--:--  2809{
  "_index" : "zhou",
  "_type" : "shun",
  "_id" : "9999",
  "_version" : 1,
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "created" : true
}
```
查询
全量：
```
$ curl 'http://localhost:9200/zhou/shun/_search?pretty'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   638  100   638    0     0    638      0  0:00:01 --:--:--  0:00:01  623k{
  "took" : 2,
  "timed_out" : false,
  "_shards" : {
    "total" : 3,
    "successful" : 3,
    "failed" : 0
  },
  "hits" : {
    "total" : 2,
    "max_score" : 1.0,
    "hits" : [ {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "AVtW96QWWik3CeNre3WH",
      "_score" : 1.0,
      "_source" : {
        "userName" : "zhou shun",
        "age" : 10,
        "car" : [ "benz", "bmw" ]
      }
    }, {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "1",
      "_score" : 1.0,
      "_source" : {
        "userName" : "zs.zhou",
        "age" : 10,
        "car" : [ "benz", "bmw" ]
      }
    } ]
  }
}
```
查询索引zhou下所有
$ curl 'http://localhost:9200/zhou/_search?pretty'、
查询所有索引库
$ curl 'http://localhost:9200/_search?pretty'
根据ID：
$ curl 'http://localhost:9200/zhou/shun/1?_source&pretty'
$ curl 'http://localhost:9200/zhou/shun/1/_source&pretty' 
```
$ curl 'http://localhost:9200/zhou/shun/1?pretty'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   190  100   190    0     0   5937      0 --:--:-- --:--:-- --:--:-- 11875{
  "_index" : "zhou",
  "_type" : "shun",
  "_id" : "1",
  "_version" : 2,
  "found" : true,
  "_source" : {
    "userName" : "zs.zhou",
    "age" : 10,
    "car" : [ "benz", "bmw" ]
  }
}
```
根据条件查询：
```
$ curl -XGET http://localhost:9200/zhou/shun/_search?q=userName:zs
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   122  100   122    0     0   7625      0 --:--:-- --:--:-- --:--:--  119k{"took":1,"timed_out":false,"_shards":{"total":3,"successful":3,"failed":0},"hits":{"total":0,"max_score":null,"hits":[]}}
```
发现查询不出数据，但有userName为 zs.zhou的，后来了解到 和数据库的like操作不一样，
es，lucene使用的分词引擎是根据拆分的词项进行匹配的。当然，zs.zhou是个完整的词。
zhou shun 由于分词后 为 zhou 和 shun ，故能查询到。
```
Administrator@PC-20150504VJBV ~
100   314  100   314    0     0    314      0  0:00:01 --:--:--  0:00:01  306k{"took":3,"timed_out":false,"_shards":{"total":3,"successful":3,"failed":0},"hits":{"total":1,"max_score":0.19178301,"hits":[{"_index":"zhou","_type":"shun","_id":"AVtW96QWWik3CeNre3WH","_score":0.19178301,"_source":
{
    "userName":"zhou shun",
    "age":10,
    "car":[
        "benz",
        "bmw"
    ]
}}]}}
[1]+  已完成               curl -XGET http://localhost:9200/zhou/shun/_search?q=userName:zhou
```
那么zs.zhou应该怎么实现和数据库一样的like查询效果呢？
使用wildcard 通配符查询：
```
Administrator@PC-20150504VJBV ~
100   279  100   279    0     0    279      0  0:00:01 --:--:--  0:00:01  272k{"took":3,"timed_out":false,"_shards":{"total":3,"successful":3,"failed":0},"hits":{"total":1,"max_score":1.0,"hits":[{"_index":"zhou","_type":"shun","_id":"1","_score":1.0,"_source":
{
    "userName":"zs.zhou",
    "age":10,
    "car":[
        "benz",
        "bmw"
    ]
}}]}}
[1]+  已完成               curl -XGET http://localhost:9200/zhou/shun/_search?q=userName:zs*
Administrator@PC-20150504VJBV ~
```
部分更新：
更新ID = 1的文档的 age字段 。 注：doc为关键字
```
Administrator@PC-20150504VJBV ~
$ curl -XPOST http://localhost:9200/zhou/shun/1/_update -d '
{"doc":{"age":20}}'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   122  100   103  100    19   1320    243 --:--:-- --:--:-- --:--:--  1320{"_index":"zhou","_type":"shun","_id":"1","_version":3,"_shards":{"total":2,"successful":1,"failed":0}}
```
或全量更新 （其余字段会被置空，需指明所有字段）
```
Administrator@PC-20150504VJBV ~
$ curl -XPUT http://localhost:9200/zhou/shun/1 -d '
{"age":30}'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   130  100   119  100    11   1919    177 --:--:-- --:--:-- --:--:--  1919{"_index":"zhou","_type":"shun","_id":"1","_version":4,"_shards":{"total":2,"successful":1,"failed":0},"created":false}
```
或根据script脚本语言
全量更新，其实就是利用PUT的幂等性 进行数据的覆盖
```
Administrator@PC-20150504VJBV ~
$ curl -XPUT http://localhost:9200/zhou/shun/1 -d '
{"userName" : "zs.zhou leo.li " ,"age":109}'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   163  100   119  100    44    691    255 --:--:-- --:--:-- --:--:--   691{"_index":"zhou","_type":"shun","_id":"1","_version":7,"_shards":{"total":2,"successful":1,"failed":0},"created":false}
```
根据_id删除：
```
$ curl -XDELETE 'http://localhost:9200/zhou/shun/AVtW9_74Wik3CeNre3WI'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   135  100   135    0     0   2177      0 --:--:-- --:--:-- --:--:--  2177{"found":true,"_index":"zhou","_type":"shun","_id":"AVtW9_74Wik3CeNre3WI","_version":2,"_shards":{"total":2,"successful":1,"failed":0}}
```
根据条件删除，需要用到DSL，使用_query替换_search达到搜索条件的目的（測試后發現不一定有作用）
注：还是需要使用_search + DSL，经测试 _query无效
```
Administrator@PC-20150504VJBV ~
$ curl -XDELETE 'localhost:9200/zhou/shun/_query?pretty' -d '
{
  "query": { "match": { "age": 555 } }
}'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   220  100   177  100    43   1609    390 --:--:-- --:--:-- --:--:--  1609{
  "found" : true,
  "_index" : "zhou",
  "_type" : "shun",
  "_id" : "_query",
  "_version" : 3,
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  }
}
```
批处理（bulk）：
batch文件，默认使用换行符作为完整一行命令的结束 ，以下二行为完整一条命令
```
{"index":{"_index":"zhou","_type":"shun"}}
{"userName":"Peter BisShop","age":101,"car":"buick"}
{"create":{"_index":"zhou","_type":"shun","_id":"1"}}
{"userName":"Amy acker","age":40,"car":["volkswagen","mini cooper"]}
{"delete":{"_index":"zhou","_type":"shun","_id":"AVtX_zfDJD8Lt5ZvIw3A"}
{"update":{"_index":"zhou","_type":"shun","_id":"AVtYAQ2RJD8Lt5ZvIw3E"}}
{"doc":{"age":9999}}
```
200：success；201：created
```
Administrator@PC-20150504VJBV /doc
$ curl -XPUT 'http://localhost:9200/_bulk?pretty' --data-binary @batch
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  1478  100  1086  100   392   3166   1142 --:--:-- --:--:-- --:--:--  3310{
  "took" : 335,
  "errors" : false,
  "items" : [ {
    "create" : {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "AVtYGcfTUuqDCgrXO2Ka",
      "_version" : 1,
      "_shards" : {
        "total" : 2,
        "successful" : 1,
        "failed" : 0
      },
      "status" : 201
    }
  }, {
    "create" : {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "1",
      "_version" : 1,
      "_shards" : {
        "total" : 2,
        "successful" : 1,
        "failed" : 0
      },
      "status" : 201
    }
  }, {
    "delete" : {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "AVtX_zfDJD8Lt5ZvIw3A",
      "_version" : 2,
      "_shards" : {
        "total" : 2,
        "successful" : 1,
        "failed" : 0
      },
      "status" : 200,
      "found" : true
    }
  }, {
    "update" : {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "AVtYAQ2RJD8Lt5ZvIw3E",
      "_version" : 2,
      "_shards" : {
        "total" : 2,
        "successful" : 1,
        "failed" : 0
      },
      "status" : 200
    }
  } ]
}
```
复制一段DSL的描述
DSL：elasticsearch 提供基于JSON的完整的Query DSL查询表达式(DSL即领域专用语言). 一般来说, 普通的查询如 term 或者 prefix. 另外还有混合查询如 bool 等. 另外查询表达式(Queries)还能够关联特定的过滤表达式，如 filtered 或者 constant_score 查询.
你可以把Query DSL当作是一系列的抽象的查询表达式树（ AST ）. 特定查询能够包含其它的查询，(如 bool ), 有些查询能够包含过滤器(如 constant_score), 还有的可以同时包含查询和过滤器 (如 filtered). 都能够从ES支持查询集合里面选择任意一个查询或者是从过滤器集合里面挑选出任意一个过滤器, 这样的话，我们就可以构造出任意复杂（maybe 非常有趣）的查询了，是不是很灵活啊.
查询和过滤都可以被用于各种不同的API接口里面. 如 search query, 或者是 facet filter 等等. 
提示. 过滤器非常有用因为他们比简单的查询更快（不进行文档评分）并且会自动缓存.
使用过滤器查询（query查询json保存在文件中）
query.json：过滤名字中有amy词项的记录
```
{
    "query":{
        "filtered":{
            "query":{
                "match":{
                    "userName":"amy"
                }
            },
            "filter":{
                "range":{
                    "age":{
                        "gte":30
                    }
                }
            }
        }
    }
}
```
```
Administrator@PC-20150504VJBV /doc
$ curl localhost:9200/zhou/shun/_search?pretty -d @query.json
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   501  100   431  100    70   4634    752 --:--:-- --:--:-- --:--:--  4634{
  "took" : 64,
  "timed_out" : false,
  "_shards" : {
    "total" : 3,
    "successful" : 3,
    "failed" : 0
  },
  "hits" : {
    "total" : 1,
    "max_score" : 1.058217,
    "hits" : [ {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "1",
      "_score" : 1.058217,
      "_source" : {
        "userName" : "Amy acker",
        "age" : 40,
        "car" : [ "volkswagen", "mini cooper" ]
      }
    } ]
  }
}
```
查询：
```
Administrator@PC-20150504VJBV /doc
$ curl -XGET localhost:9200/zhou/shun/_search?pretty -d '{
"size":3,"from":0,"sort":{"age":{"order":"asc"}},"query":
{"match":{"userName":"Peter"}
}
}'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   967  100   874  100    93    874     93  0:00:01 --:--:--  0:00:01  853k{
  "took" : 3,
  "timed_out" : false,
  "_shards" : {
    "total" : 3,
    "successful" : 3,
    "failed" : 0
  },
  "hits" : {
    "total" : 7,
    "max_score" : null,
    "hits" : [ {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "44",
      "_score" : null,
      "_source" : {
        "userName" : "Peter BisShop",
        "age" : 44
      },
      "sort" : [ 44 ]
    }, {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "AVtYAQ2RJD8Lt5ZvIw3E",
      "_score" : null,
      "_source" : {
        "userName" : "Peter",
        "age" : 51
      },
      "sort" : [ 51 ]
    }, {
      "_index" : "zhou",
      "_type" : "shun",
      "_id" : "AVtYADkNJD8Lt5ZvIw3C",
      "_score" : null,
      "_source" : {
        "userName" : "Peter BisShop",
        "age" : 101,
        "car" : "buick"
      },
      "sort" : [ 101 ]
    } ]
  }
}
```
Java端，默认client端口为 9300
```java
package elasticSearch;
import com.alibaba.fastjson.JSON;
import org.elasticsearch.action.delete.DeleteResponse;
import org.elasticsearch.action.get.GetResponse;
import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.action.update.UpdateRequest;
import org.elasticsearch.bootstrap.Elasticsearch;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.index.query.QueryBuilder;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import java.net.InetAddress;
import java.util.*;
/**
 * Created by Administrator on 2017/4/17.
 */
public class ElasticSearch {
    private static TransportClient client   = null;
    /**
     * es index
     */
    private static final String    ES_INDEX = "zhou";
    /**
     * es type
     */
    private static final String    ES_TYPE  = "shun";
    public static void main(String[] args) throws Exception {
        String aaa = new String("abc");
        String bbb = "abc";
        String ccc = "abc";
        System.out.println(aaa==bbb);
        System.out.println(ccc==bbb);
        System.out.println(aaa==ccc);
        createClient();
        String _id = createIndex();
        getDataResponse(_id);
        updateData(_id);
        getDataResponse(_id);
        QueryBuilder builder = QueryBuilders.matchAllQuery();
        // QueryBuilder builder = QueryBuilders.termQuery("username", "zhengyong");
        queryDataList(builder);
        // deleteDataResponse(_id);
        shutdown();
    }
    private static void createClient() throws Exception {
        if (client == null) {
            synchronized (ElasticSearch.class) {
//                Settings settings = Settings.settingsBuilder().put("cluster.name", "myCluster").build();
//                client = TransportClient.builder().settings(settings).build().addTransportAddress(new InetSocketTransportAddress(InetAddress.getByName("localhost"),
//                        9200));
                Settings settings = Settings.settingsBuilder().put("cluster.name", "myCluster").build();
                client = TransportClient.builder().build().addTransportAddress(new InetSocketTransportAddress(InetAddress.getByName("localhost"),
                        9300));
            }
        }
    }
    /**
     * 创建es库并导入数据
     *
     * @return _id
     */
    private static String createIndex() {
        String json = "{" + "\"userName\":\"zzs\"," + "\"age\":" + 1056 + "}";
        System.out.println(json);
        IndexResponse response = client.prepareIndex(ES_INDEX, ES_TYPE).setSource(json).get();
        if(response.isCreated()){
            System.out.println(response.getId());
        }
        return response.getId();
    }
    /**
     * 根据_id获取数据
     *
     * @param _id 唯一标识
     * @return GetResponse
     */
    private static GetResponse getDataResponse(String _id) {
        GetResponse response = client.prepareGet(ES_INDEX, ES_TYPE, _id).get();
        System.out.println(String.format("get data response: %s", JSON.toJSONString(response.getSource())));
        return response;
    }
    /**
     * 根据查询条件查询结果集
     *
     * @param queryBuilder 查询条件
     * @return List
     */
    private static List<String> queryDataList(QueryBuilder queryBuilder) {
        SearchResponse sResponse = client.prepareSearch(ES_INDEX).setTypes(ES_TYPE).setQuery(queryBuilder).setSize(1000).execute().actionGet();
        SearchHits hits = sResponse.getHits();
        List<String> list = new ArrayList<>();
        SearchHit[] hitArray = hits.hits();
        for (SearchHit hit : hitArray) {
            Map<String, Object> map = hit.getSource();
            String username = (String) map.get("userName");
            Integer age = (Integer) map.get("age");
            StringBuilder br = new StringBuilder();
            br.append(username).append("_").append(age).append("_");
            list.add(br.toString());
        }
        System.out.println(String.format("query data count=%s, list : %s", list.size(), JSON.toJSONString(list)));
        return list;
    }
    /**
     * 根据_id删除数据
     *
     * @param _id 唯一标识
     * @return DeleteResponse
     */
    private static DeleteResponse deleteDataResponse(String _id) {
        DeleteResponse response = client.prepareDelete(ES_INDEX, ES_TYPE, _id).get();
        System.out.println(String.format("delete data response: %s", JSON.toJSONString(response)));
        return response;
    }
    /**
     * 根据_id跟下数据
     *
     * @param _id 唯一标识
     * @throws Exception
     */
    private static void updateData(String _id) throws Exception {
        UpdateRequest updateRequest = new UpdateRequest();
        updateRequest.index(ES_INDEX);
        updateRequest.type(ES_TYPE);
        updateRequest.id(_id);
        String json = "{" + "\"userName\":\"zzs_update\"," + "\"age\":" + 2056 + "}";
        updateRequest.doc(json);
        client.update(updateRequest).get();
    }
    private static void shutdown() {
        if (client != null) {
            client.close();
        }
    }
}
```
