# java操作elasticsearch 5.6.0查询、插入 - z69183787的专栏 - CSDN博客
2018年09月17日 12:00:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：280
[https://blog.csdn.net/lixin2151408/article/details/78276513](https://blog.csdn.net/lixin2151408/article/details/78276513)
**需注意：**
1、ES5*以上版本需要jdk1.8
2、1.8JDK需tomcat 8
# **一、新建maven项目，添加依赖**
```
<dependencies>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<version>4.9</version>
</dependency>
<dependency>
<groupId>jdk.tools</groupId>
<artifactId>jdk.tools</artifactId>
<version>1.8</version>
<scope>system</scope>
<systemPath>C:/Program Files/Java/jdk1.8.0_144/lib/tools.jar</systemPath>
</dependency>
<dependency>
<groupId>org.elasticsearch</groupId>
<artifactId>elasticsearch</artifactId>
<version>5.6.0</version>
</dependency>
<dependency>
<groupId>org.elasticsearch.client</groupId>
<artifactId>transport</artifactId>
<version>5.6.0</version>
</dependency>
<dependency>
<groupId>org.apache.logging.log4j</groupId>
<artifactId>log4j-api</artifactId>
<version>2.7</version>
</dependency>
<dependency>
<groupId>org.apache.logging.log4j</groupId>
<artifactId>log4j-core</artifactId>
<version>2.7</version>
</dependency>
</dependencies>
```
```
二、新建log4j相关文件
```
**在main文件夹下新建resource文件夹，新建log4j.properties和log4j2.xml**
**log4j.properties**
```
appender.console.type = Console
appender.console.name = console
appender.console.layout.type = PatternLayout
rootLogger.level = info
rootLogger.appenderRef.console.ref = console
```
log4j2.xml
```
<?xml version="1.0" encoding="UTF-8"?>
 <Configuration status="WARN">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
        </Console>
        </Appenders>
    <Loggers>
        <Root level="error">
            <AppenderRef ref="Console"/>
        </Root>
    </Loggers>
</Configuration>
```
# **三、查询**
**在es中新建索引/blog/article ，插入几条数据；（注意：这里IP地址写自己在elasticsearch.yml中写的network.host，port就写9300，虽然自己设置的es集群port是9200）**
```java
public static void main(String[] args) {
try {
//设置集群名称
Settings settings = Settings.builder().put("cluster.name", "elasticsearch").build();
//创建client
@SuppressWarnings("resource")
TransportClient client = new PreBuiltTransportClient(settings)
.addTransportAddress(new InetSocketTransportAddress(InetAddress.getByName("0.0.0.0"), 9300));
//写入数据
// createDate(client);
//搜索数据
GetResponse response = client.prepareGet("blog", "article", "1").execute().actionGet();
//输出结果
System.out.println(response.getSource());
//关闭client
client.close();
} catch (Exception e) {
e.printStackTrace();
}
}
```
```
四、插入单条数据
```
**(因为在查询时已经创建client，因此这里直接引入了，单独插入需单独创建)**
```java
/**
* 插入单条数据
* @param client
*/
public static void createDate(Client client){
Map<String,Object> map = new HashMap<String, Object>();
map.put("title", "红楼梦");
map.put("content", "重生之我是贾宝玉");
map.put("newcontent", new String[]{"first","我是贾宝玉"}) ;
map.put("about", "i'd like to play");
try {
IndexResponse response = client.prepareIndex("blog", "article",UUID.randomUUID().toString())
.setSource(map).execute().actionGet();
System.out.println("写入数据结果=" + response.status().getStatus() + "！id=" + response.getId());
} catch (Exception e) {
// TODO: handle exception
e.printStackTrace();
}
```
![](https://img-blog.csdn.net/20171018200431421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl4aW4yMTUxNDA4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
