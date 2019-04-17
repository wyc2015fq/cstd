# 阿里Redis Java客户端Tedis - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 12:55:13[boonya](https://me.csdn.net/boonya)阅读数：2384








Github地址：[https://github.com/justified/tedis](https://github.com/justified/tedis)


# Tedis



Tedis是另一个redis的java客户端


Tedis的目标是打造一个可在生产环境直接使用的高可用Redis解决方案。参见更多资料

Feature


    高可用，Tedis使用多写随机读做HA确保redis的高可用

    高性能，使用特殊的线程模型，使redis的性能不限制在客户端

    多种使用方式，如果你只有一个redis实例，并不需要tedis的HA功能，可以直接使用tedis-atomic；使用tedis的高可用功能需要部署多个redis实例使用tedis-group

    两种API，包括针对byte的底层api和面向object的高层api

    多种方便使用redis的工具集合，包括mysql数据同步到redis工具，利用redis做搜索工具等



# QuickStart



引入Maven依赖：



```
<dependency>
  <groupId>com.taobao.common</groupId>
  <artifactId>tedis-group</artifactId>
  <version>1.1.0</version>
</dependency>
```


Java 使用示例：



```java
Group tedisGroup = new TedisGroup(appName, version);
tedisGroup.init();
ValueCommands valueCommands = new DefaultValueCommands(tedisGroup.getTedis());
// 写入一条数据
valueCommands.set(1, "test", "test value object");
// 读取一条数据
valueCommands.get(1, "test");
```



Tedis在GPL version 2协议下开源，目前还尚不完善，如果你有什么好的想法或者发现了任何bug，请不要吝啬你的意见:)






