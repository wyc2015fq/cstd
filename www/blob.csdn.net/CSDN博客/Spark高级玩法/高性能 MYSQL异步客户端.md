# 高性能:MYSQL异步客户端 - Spark高级玩法 - CSDN博客
2018年11月07日 00:03:55[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：438
**实时必须**
实时处理领域，当需要使用外部存储数据染色的时候，需要慎重对待，不能让与外部系统之间的交互延迟对流的整个进度取决定性的影响。
同步的与数据库交互需要等待一个请求从发起到结束才能发起下次请求，等待过程是非常浪费函数时间的。
与数据库异步交互，意味着单个函数实例可以并发处理很多请求，同时并发接收响应。那么，等待时间由于发送其它请求和接收其它响应，被重复使用而节省了。至少，等待时间在多个请求上被摊销。这就使得很多使用案例具有更高的吞吐量。
浪尖在这里推荐这个jdbc异步访问客户端的主要目的是昨天发了一篇flink异步IO的文章
[Flink异步IO第一讲](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485776&idx=1&sn=bb78ed1f45c7509688526cbd29686f4f&chksm=9f38ea78a84f636e468765b2f870e5f78afe5063f6a94b96cca540c9116fc39d7e4a123b6b6e&scene=21#wechat_redirect)
这个异步IO是实现高性能维表的一种方案，而今天要说的这个jdbc客户端也是其中比较重要的一个环节。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW7W9cIicVC0bNvE3wHbYuqencwXlAE7yypOIF0rlVqeWf9P4LZibrUZvaBbUezVI2bg6y8pmibAkZRA/640)
Vert.x JDBC client客户端的github地址
https://vertx.io/docs/vertx-jdbc-client/java/
使用该客户端要加入的maven依赖配置到pom.xml文件中
<dependency>
  <groupId>io.vertx</groupId>
  <artifactId>vertx-jdbc-client</artifactId>
  <version>3.5.2</version>
</dependency>
<dependency>
  <groupId>io.vertx</groupId>
  <artifactId>vertx-core</artifactId>
  <version>3.5.2</version>
</dependency>
大多数情况下可能是希望在多个客户端实例中共享数据源。
例如，通过创建多个实例来扩展应用程序，并且希望每个实例共享相同的数据源，避免创建多个链接池。
**Client**
实现起来很简单，只需调用：
SQLClient client =JDBCClient.createShared(vertx, config);
第一次调用该方法确实会使用指定的配置创建一个数据源。
后续的调用会直接返回一个客户端实例，配置不会再次使用。
可以为创建的数据源指定一个名称，方式如下：
SQLClient client =JDBCClient.createShared(vertx, config, "MyDataSource");
如果使用相同的数据源名称和相同的Vert.x实例创建不同的客户端，实际上客户端会共享相同的数据源。
这种创建方式是在你希望有不同的客户端组分别去跟不同的数据库交互的时候使用。
当然了，有时候也可能是希望每个客户端实例独享一个数据源，这种客户端的创建方式也很简单。
SQLClient client =JDBCClient.createNonShared(vertx, config);
这种方式等价于每次调用客户端给一个唯一的数据源名称。
如果已经存在了一个数据源，那么也可以直接指定该数据源
SQLClient client = JDBCClient.create(vertx,dataSource);
在使用完了数据库客户端之后应该释放掉资源。共享数据源的客户端会存在一个引用计数的概念。相同数据源的最后一个引用关闭的时候，数据源才会被释放掉。
**Connection**
使用客户端对象的getConnection方法可以获取一个数据库链接，具体使用方式如下：
client.getConnection(res -> {
if(res.succeeded()) {
   SQLConnection connection = res.result();
   connection.query("SELECT * FROM some_table", res2 -> {
if (res2.succeeded()) {
       ResultSet rs = res2.result();
// Do something with results
     }
   });
  }else {
// Failed to get connection - deal with it
  }
});
SQLConnection是一个通用的接口，不仅仅适用于Vert.x客户端。
接口的介绍如下：
https://vertx.io/docs/vertx-sql-common/java/
**配置**
主要是介绍一下常用的配置。
**provider_class**：该类主要是用来管理数据库连接的。默认是
io.vertx.ext.jdbc.spi.impl.C3P0DataSourceProvider。可以覆盖该参数配置，指定自己的实现。已实现的方式多种，可以参考
https://github.com/vert-x3/vertx-jdbc-client/tree/master/src/test/java/io/vertx/ext/jdbc/spi/impl
**row_stream_fetch_size** : SQLRowStream的内部cache的大小，默认是128.
假设使用了C3P0的实现，也即是默认，配置参数如下：
**url** : 数据库连接用的URL。
**driver_class** : jdbc driver的类名称。
**user**:数据库的用户名。
**password**:数据库密码。
**max_pool_size**:连接池中最大连接数量，默认是15.
**initial_pool_size**:连接池中最大初始连接数量，默认是3.
**min_pool_size**:连接池中最小的连接数量。
**max_statements**:缓存的prepared statementszui 大数量，默认是0.
**max_statements_per_connection**:每个连接缓存的prepared statements最大的数量，默认是0。
**max_idle_time**: 一个连接的空闲时间超过该值，就会被关闭，默认是0，禁止超时关闭机制.
**完整的案例**
package bigdata.spark.DaoUtil;
import com.google.common.collect.Lists;
import io.vertx.core.Vertx;
import io.vertx.core.VertxOptions;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.jdbc.JDBCClient;
import io.vertx.ext.sql.ResultSet;
import io.vertx.ext.sql.SQLClient;
import io.vertx.ext.sql.SQLConnection;
import org.apache.flink.streaming.api.functions.async.ResultFuture;
import org.apache.flink.types.Row;
import java.util.Collections;
import java.util.List;
publicclassVertxMysql{
privatetransient SQLClient mySQLClient;
publicvoidopen()throws Exception {
        JsonObject mySQLClientConfig = new JsonObject();
        mySQLClientConfig.put("url", "jdbc:mysql://localhost:3306/")
                .put("driver_class", "com.mysql.jdbc.Driver")
                .put("max_pool_size", 20)
                .put("user", "root")
                .put("password", "password");
        VertxOptions vo = new VertxOptions();
        vo.setEventLoopPoolSize(10);
        vo.setWorkerPoolSize(20);
        Vertx vertx = Vertx.vertx(vo);
        mySQLClient = JDBCClient.createNonShared(vertx, mySQLClientConfig);
    }
publicvoidclose()throws Exception {
        mySQLClient.close();
    }
publicvoidasyncInvoke()throws Exception {
//        JsonArray inputParams = new JsonArray();
        mySQLClient.getConnection(conn -> {
if (conn.failed()) {
//Treatment failures
return;
            }
final SQLConnection connection = conn.result();
            connection.query("SELECT * FROM some_table", res2 -> {
if (res2.succeeded()) {
                    ResultSet rs = res2.result();
                    List<JsonObject> rows = rs.getRows();
for(JsonObject json : rows){
                        String id = json.getString("id");
                        String name = json.getString("name");
                        System.out.println("name : "+name + "ID : "+ id);
                    }
// Do something with results
                }
            });
        });
    }
}
github地址:
https://github.com/vert-x3/vertx-jdbc-client
推荐阅读：
[小白专属mysql入门](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484889&idx=1&sn=1736df817988e7c9d31ddc09263258d6&chksm=9f38e6f1a84f6fe745309d09e3771c189a51a3ed8d5e8e36e8868d62d7e22b9c1ef094ca3b9a&scene=21#wechat_redirect)
[MySQL索引优化分析](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484978&idx=1&sn=d5ffc0f6000d6c13344ecbc617fbc3da&chksm=9f38e51aa84f6c0cf1cb364d0afd36e341a0ea5695e8b315fe7800bd6ea77e952e956ea283aa&scene=21#wechat_redirect)
[老司机也该掌握的MySQL优化指南](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485654&idx=1&sn=fdaaee44fecec1acc44f03e664913cb8&chksm=9f38ebfea84f62e83cf4b3466e634808238e2315e4dd5423f2cd2a00a7855e2f14198c3df5b1&scene=21#wechat_redirect)
[kafka源码系列之mysql数据增量同步到kafka](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484962&idx=1&sn=e92a2521395d862c1e16efc266837152&chksm=9f38e50aa84f6c1cd72d24a86938a3b320a5e41ab8bdb6939de7381192c023b066da9c0ce305&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
