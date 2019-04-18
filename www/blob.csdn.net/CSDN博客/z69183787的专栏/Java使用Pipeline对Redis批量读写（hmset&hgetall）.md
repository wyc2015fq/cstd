# Java使用Pipeline对Redis批量读写（hmset&hgetall） - z69183787的专栏 - CSDN博客
2016年08月30日 18:04:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3411
redis的管道(Pipelining)操作是一种异步的访问模式，一次发送多个指令，不同步等待其返回结果。这样可以取得非常好的执行效率。这就是管道，调用方法如下：
```java
@Test
public void test3Pipelined() {
    Jedis jedis = new Jedis("localhost");
    Pipeline pipeline = jedis.pipelined();
    long start = System.currentTimeMillis();
    for (int i = 0; i < 100000; i++) {
        pipeline.set("p" + i, "p" + i);
    }
    List<Object> results = pipeline.syncAndReturnAll();
    long end = System.currentTimeMillis();
    System.out.println("Pipelined SET: " + ((end - start)/1000.0) + " seconds");
    jedis.disconnect();
}
```
摘要
Java使用Pipeline对Redis批量读写（hmset&hgetall）
一般情况下，Redis Client端发出一个请求后，通常会阻塞并等待Redis服务端处理，Redis服务端处理完后请求命令后会将结果通过响应报文返回给Client。
这有点类似于HBase的Scan，通常是Client端获取每一条记录都是一次RPC调用服务端。
在Redis中，有没有类似HBase Scanner Caching的东西呢，一次请求，返回多条记录呢？
有，这就是Pipline。官方介绍 [http://redis.io/topics/pipelining](http://redis.io/topics/pipelining)
通过pipeline方式当有大批量的操作时候，我们可以节省很多原来浪费在网络延迟的时间，需要注意到是用pipeline方式打包命令发送，redis必须在处理完所有命令前先缓存起所有命令的处理结果。打包的命令越多，缓存消耗内存也越多。所以并不是打包的命令越多越好。
使用Pipeline在对Redis批量读写的时候，性能上有非常大的提升。
使用Java测试了一下：
```java
```java
```
```
- 
- package com.lxw1234.redis;
- 
- import java.util.HashMap;
- import java.util.Map;
- import java.util.Set;
- 
- import redis.clients.jedis.Jedis;
- import redis.clients.jedis.Pipeline;
- import redis.clients.jedis.Response;
- 
- 
- publicclassTest{
- 
- publicstaticvoid main(String[] args)throwsException{
- Jedis redis =newJedis("127.0.0.1",6379,400000);
- Map<String,String> data =newHashMap<String,String>();
-  redis.select(8);
-  redis.flushDB();
- //hmset 
- long start =System.currentTimeMillis();
- //直接hmset
- for(int i=0;i<10000;i++){
-  data.clear();
-  data.put("k_"+ i,"v_"+ i);
-  redis.hmset("key_"+ i, data);
- }
- longend=System.currentTimeMillis();
- System.out.println("dbsize:["+ redis.dbSize()+"] .. ");
- System.out.println("hmset without pipeline used ["+(end- start)/1000+"] seconds ..");
-  redis.select(8);
-  redis.flushDB();
- //使用pipeline hmset
- Pipeline p = redis.pipelined();
-  start =System.currentTimeMillis();
- for(int i=0;i<10000;i++){
-  data.clear();
-  data.put("k_"+ i,"v_"+ i);
-  p.hmset("key_"+ i, data);
- }
-  p.sync();
- end=System.currentTimeMillis();
- System.out.println("dbsize:["+ redis.dbSize()+"] .. ");
- System.out.println("hmset with pipeline used ["+(end- start)/1000+"] seconds ..");
- 
- //hmget 
- Set keys = redis.keys("*");
- //直接使用Jedis hgetall
-  start =System.currentTimeMillis();
- Map<String,Map<String,String>> result =newHashMap<String,Map<String,String>>();
- for(String key : keys){
-  result.put(key, redis.hgetAll(key));
- }
- end=System.currentTimeMillis();
- System.out.println("result size:["+ result.size()+"] ..");
- System.out.println("hgetAll without pipeline used ["+(end- start)/1000+"] seconds ..");
- 
- //使用pipeline hgetall
- Map<String,Response<Map<String,String>>> responses =newHashMap<String,Response<Map<String,String>>>(keys.size());
-  result.clear();
-  start =System.currentTimeMillis();
- for(String key : keys){
-  responses.put(key, p.hgetAll(key));
- }
-  p.sync();
- for(String k : responses.keySet()){
-  result.put(k, responses.get(k).get());
- }
- end=System.currentTimeMillis();
- System.out.println("result size:["+ result.size()+"] ..");
- System.out.println("hgetAll with pipeline used ["+(end- start)/1000+"] seconds ..");
- 
-  redis.disconnect();
- 
- }
- 
- 
- }
- 
测试结果如下：
```
```
```
```
- 
- dbsize:[10000]..
- hmsetwithoutpipelineused[243]seconds..
- dbsize:[10000]..
- hmsetwithpipelineused[0]seconds..
- resultsize:[10000]..
- hgetAllwithoutpipelineused[243]seconds..
- resultsize:[10000]..
- hgetAllwithpipelineused[0]seconds..
使用pipeline来批量读写10000条记录，就是小菜一碟，秒完。
原文地址：[http://lxw的大数据田地
 » Java使用Pipeline对Redis批量读写（hmset&hgetall）](/)
