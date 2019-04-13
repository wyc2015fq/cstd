
# Java 使用 Redis -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:31:25[seven-soft](https://me.csdn.net/softn)阅读数：512



## 安装
开始在 Java 中使用 Redis 前， 我们需要确保已经安装了 redis 服务及 Java redis 驱动，且你的机器上能正常使用 Java。 Java的安装配置可以参考我们的[Java开发环境配置 ](http://www.runoob.com/redis/java-environment-setup.html)接下来让我们安装 Java redis 驱动：
首先你需要下载驱动包，下载 jedis.jar，确保下载最新驱动包。
在你的classpath中包含该驱动包。
---

## 连接到 redis 服务
importredis.clients.jedis.Jedis;publicclassRedisJava{publicstaticvoidmain(String[]args){//连接本地的 Redis 服务Jedisjedis=newJedis("localhost");System.out.println("Connection to server sucessfully");//查看服务是否运行System.out.println("Server is running: "+jedis.ping());}}编译以上 Java 程序，确保驱动包的路径是正确的。
$javacRedisJava.java
$javaRedisJavaConnectionto server sucessfullyServerisrunning:PONGRedisJavaStringExample
---

## Redis Java String(字符串) 实例
importredis.clients.jedis.Jedis;publicclassRedisStringJava{publicstaticvoidmain(String[]args){//连接本地的 Redis 服务Jedisjedis=newJedis("localhost");System.out.println("Connection to server sucessfully");//设置 redis 字符串数据jedis.set("w3ckey","Redis tutorial");// 获取存储的数据并输出System.out.println("Stored string in redis:: "+jedis.get("w3ckey"));}}编译以上程序。
$javacRedisStringJava.java
$javaRedisStringJavaConnectionto server sucessfullyStoredstringinredis::Redistutorial
---

## Redis Java List(列表) 实例
importredis.clients.jedis.Jedis;publicclassRedisListJava{publicstaticvoidmain(String[]args){//连接本地的 Redis 服务Jedisjedis=newJedis("localhost");System.out.println("Connection to server sucessfully");//存储数据到列表中jedis.lpush("tutorial-list","Redis");jedis.lpush("tutorial-list","Mongodb");jedis.lpush("tutorial-list","Mysql");// 获取存储的数据并输出List<String>list=jedis.lrange("tutorial-list",0,5);for(inti=0;i<list.size();i++){System.out.println("Stored string in redis:: "+list.get(i));}}}编译以上程序。
$javacRedisListJava.java
$javaRedisListJavaConnectionto server sucessfullyStoredstringinredis::RedisStoredstringinredis::MongodbStoredstringinredis::Mysql
---

## Redis Java Keys 实例
importredis.clients.jedis.Jedis;publicclassRedisKeyJava{publicstaticvoidmain(String[]args){//连接本地的 Redis 服务Jedisjedis=newJedis("localhost");System.out.println("Connection to server sucessfully");// 获取数据并输出List<String>list=jedis.keys("*");for(inti=0;i<list.size();i++){System.out.println("List of stored keys:: "+list.get(i));}}}编译以上程序。
$javacRedisKeyJava.java
$javaRedisKeyJavaConnectionto server sucessfullyListof stored keys::tutorial-nameListof stored keys::tutorial-list

