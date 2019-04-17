# Github Mybatis深入学习之OSCache配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月13日 09:37:41[boonya](https://me.csdn.net/boonya)阅读数：1971









     Mybatis应用为对象数据添加缓存：OSCache就是其选择之一，另外它还支持cache和EHCache。原文地址：[http://mybatis.github.io/oscache-cache/](http://mybatis.github.io/oscache-cache/),相关Jar包下载地址：[https://code.google.com/p/mybatis/](https://code.google.com/p/mybatis/)

不幸的是，OpenSymphony在它最后的日子里，OSCache的将无法再维持。[OSCache的](http://www.opensymphony.com/oscache/)是一个高性能的缓存解决方案，开发和维护的  [Open
 Symphony](http://www.opensymphony.com/)，并轻松整合以来的iBATIS的版本2.x的MyBatis中自3 MyBatis的没有它，我们强烈地使用它，我们决定，为恢复其使用。



用户要使用到他们的应用OSCache的，有下载的 [zip包](http://code.google.com/p/mybatis/downloads/list?q=Product-Cache)，解压缩，并在classpath中添加的罐子; Apache
 Maven用户的，而不是可以简单地添加在pom.xml中下面的依赖：



```
<dependencies>
  ...
  <dependency>
    <groupId>org.mybatis.caches</groupId>
    <artifactId>mybatis-oscache</artifactId>
    <version>1.0.2-SNAPSHOT</version>
  </dependency>
  ...
</dependencies>
```




然后，仅仅配置XML映射



```
<mapper namespace="org.acme.FooMapper">

  <cache type="org.mybatis.caches.oscache.OSCache"/>

  ...

</mapper>
```




如果用户需要登录缓存操作，它们可以堵塞缓存日志记录的版本：



```
<mapper namespace="org.acme.FooMapper">

  <cache type="org.mybatis.caches.oscache.LoggingOSCache"/>

  ...

</mapper>
```



适当的OSCache的配置，请阅读官方 [参考](http://wiki.opensymphony.com/display/CACHE/Configuration)



