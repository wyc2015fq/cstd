# Elasticsearch教程 ，Elasticsearch count 查询，Elasticsearch 查询是否存在 - z69183787的专栏 - CSDN博客
2018年08月22日 13:01:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：620
[https://www.sojson.com/blog/206.html](https://www.sojson.com/blog/206.html)
# 一、Elasticsearch Count查询
当我们使用  [Elasticsearch](http://www.sojson.com/tag_elasticsearch.html)  的时候，如果只想知道符合条件的结果集，应该怎么查询？
更多教程点击：  [Elasticsearch教程](http://www.sojson.com/tag_elasticsearchjiaocheng.html)  。
## 1.1 Elasticsearch count Java API 查询
- 
```
Client client = ESTools.client;
	SearchResponse response = client.prepareSearch(MappingManager.ASK)
	.setTypes(MappingManager.ASK)
	.setQuery(new TermQueryBuilder("id", id))//设置查询类型
	.setSearchType(SearchType.COUNT)//设置查询类型，有的版本可能过期
	.setSize(0)//设置返回结果集为0
	.get();
	long length = response.getHits().totalHits();
```
最后返回了符合结果集的`Count` 数量，但是不返回结果集，不反回结果集靠`size = 0`  来决定，当然我觉得  [Elasticsearch](http://www.sojson.com/tag_elasticsearch.html)  在一些版本里应该会对数据级别的`Count` 查询应该有更好的优化，自己对应想当前版本的  [API](http://www.sojson.com/tag_api.html)  。我的`Version：2.0.2` 。
## 1.2 Elasticsearch count Http API 查询
- `POST - http://192.168.0.1:9200/index/type/_search/`
- `{`
- `"size" : 0,`
- `"query" : {`
- `"term" : {`
- `"id" : "adf183208e9a4116353e9d9cd78f2b6a"`
- `}`
- `}`
- `}`
![](https://cdn.www.sojson.com/file/17-01-03-22-31-07/doc/5488119794)
## 1.3 Elasticsearch Index Count查询
- `CountResponse response = client.prepareCount("index1","index2").get();`
- `long count = response.getCount();//返回当前index Count数量`
## 1.4 Elasticsearch Type Count查询
- `CountResponse response = client.prepareCount("index1","index2").setTypes("type1","type2").get();`
- `long count = response.getCount();//返回符合条件的数据`
# 二、Elasticsearch 查询数据是否存在
我也是认为  [Elasticsearch](http://www.sojson.com/tag_elasticsearch.html)  一些版本会有这个方法。下面看看官方的介绍：[https://www.elastic.co/guide/en/elasticsearch/reference/2.3/search-exists.html](https://www.elastic.co/guide/en/elasticsearch/reference/2.3/search-exists.html)
## 2.1 curl 方式查询数据是否存在：
查询：
- `$ curl -XGET 'http://localhost:9200/twitter/tweet/_search/exists?q=user:kimchy'`
- 
- `$ curl -XGET 'http://localhost:9200/twitter/tweet/_search/exists' -d '`
- `{`
- `"query" : {`
- `"term" : { "user" : "kimchy" }`
- `}`
- `}'`
返回结果：
- `{`
- `"exists" : true`
- `}`
[Java](http://www.sojson.com/tag_java.html)  API 我这个版本我没找到，其他版本有一些应该有 `Java API` 。
## 2.2 Elasticsearch Java API 数据Exists判断。
- `/**`
- `* 判断数据是否存在`
- `* @param id`
- `* @return`
- `*/`
- `public static boolean existsById(String id){`
- `Client client = ESTools.client;`
- `SearchRequestBuilder searchBuilder = client.prepareSearch(MappingManager.ASK)`
- `.setTypes(MappingManager.ASK)`
- `.setQuery(new TermQueryBuilder("id", id))//设置查询类型`
- `.setSearchType(SearchType.COUNT)//设置查询类型，有的版本可能过期`
- `.setSize(0);//设置返回结果集为0`
- `SearchResponse response = searchBuilder.get();`
- `long length = response.getHits().totalHits();`
- `return length > 0;`
- `}`
## 2.3 Elasticsearch Java API 判断 Index 是否存在。
- `//Index 可以多个`
- `ExistsRequest request = new ExistsRequest("index1","index2"); `
- `ExistsResponse response = client.exists(request).get();`
- `//返回是否存在`
- `boolean exists = response.exists();`
## 2.4 Elasticsearch Java API 判断 Type 是否存在。
- `//Index 可以多个`
- `ExistsRequest request = new ExistsRequest("index1","index2").types("type1","type2"); `
- `ExistsResponse response = client.exists(request).get();`
- `//返回是否存在`
- `boolean exists = response.exists();`
