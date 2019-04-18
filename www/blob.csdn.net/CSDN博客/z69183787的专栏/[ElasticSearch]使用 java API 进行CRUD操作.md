# [ElasticSearch]使用 java API 进行CRUD操作 - z69183787的专栏 - CSDN博客
2018年08月20日 19:08:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：82
个人分类：[搜索引擎-ElasticSearch&ELK](https://blog.csdn.net/z69183787/article/category/6850573)
[https://blog.csdn.net/lionel_fengj/article/details/78345741](https://blog.csdn.net/lionel_fengj/article/details/78345741)
本篇文章将介绍怎样使用 java 对 ElasticSearch 进行操作。 
首先需要建立一个 maven 项目，这里不再赘述。 
1.在maven 的 pom.xml文件中需要引入以下几个 jar 包
```
<dependency>
        <groupId>org.elasticsearch.client</groupId>
        <artifactId>transport</artifactId>
        <version>5.6.3</version>
</dependency>
<dependency>
        <groupId>org.apache.logging.log4j</groupId>
        <artifactId>log4j-api</artifactId>
        <version>2.9.1</version>
</dependency>
<dependency>
        <groupId>org.apache.logging.log4j</groupId>
        <artifactId>log4j-core</artifactId>
        <version>2.9.1</version>
</dependency>
```
2.在 resources 目录下添加log4j2.properties文件
```
appender.console.type = Console
appender.console.name = console
appender.console.layout.type = PatternLayout
rootLogger.level = info
rootLogger.appenderRef.console.ref = console
```
3.使用TransportClient连接 ElasticSearch。
```
Settings settings = Settings.builder().put("cluster.name", "leo").build();
TransportClient client = new PreBuiltTransportClient(settings). addTransportAddress(new InetSocketTransportAddress(InetAddress.getByName("***.***.***.***"), 9300));
//***.***.***.*** 表示 ip 地址，本地的话，可以使用 localhost，9300是默认的 api 访问接口
```
4.创建索引 
使用 json文件来创建索引，生成 json 文件的方式有多种，字符串格式、利用 Map、利用jackson 序列化 beans 到 json、使用 ElasticSearch helps生成(我使用的方式，其它方式实现可参看[官方文档](https://www.elastic.co/guide/en/elasticsearch/client/java-api/current/java-docs-index.html#java-docs-index-generate-beans))
```
IndexResponse response = client.prepareIndex("books", "book", "1")
                        .setSource(jsonBuilder().
                                startObject()
                                .field("book_name", "ElasticSearch入门")
                                .field("author", "张三")
                                .field("publish_time", "207-09-09")
                               .endObject())
                        .get();
```
5.获取索引
```
GetResponse getResponse = client.prepareGet("books", "book", "1").execute().actionGet();
        System.out.println(getResponse.getSourceAsString());
```
6.更新索引
```
UpdateRequest updateRequest = new UpdateRequest();
updateRequest.index("books");
updateRequest.type("book");
updateRequest.id("1");
updateRequest.doc(jsonBuilder().startObject().field("author", "李四").endObject());
try {
    client.update(updateRequest).get();
} catch (InterruptedException e) {
    e.printStackTrace();
} catch (ExecutionException e) {
    e.printStackTrace();
}
```
运行后发现作者从“张三”更新为“李四”了。 
7.删除索引
`DeleteResponse response = client.prepareDelete("books", "book", "1").get();`
索引被删除。
