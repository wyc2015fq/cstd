# Log日志 - weixin_33985507的博客 - CSDN博客
2018年11月12日 15:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
#### 1.关于MDC
为什么要用log的MDC，它是用来解决什么问题？
多线程，高并发下，多个线程打印，无法形成连贯的思路。
MDC 可以看成是一个与当前线程绑定的哈希表，可以往其中添加键值对。MDC 中包含的内容可以被同一线程中执行的代码所访问。当前线程的子线程会继承其父线程中的 MDC 的内容。当需要记录日志时，只需要从 MDC 中获取所需的信息即可。MDC 的内容则由程序在适当的时候保存进去。对于一个 Web 应用来说，通常是在请求被处理的最开始保存这些数据。
MDC中的put方法其实就是讲键值对放入一个Hashtable对象中，然后赋值给当前线程的ThreadLocal.ThreadLocalMap对象，即threadLocals，这保证了各个线程的在MDC键值对的独立性。
```
public static void put(String key, Object o)
  {
    mdc.put0(key, o);
  }
  private void put0(String key, Object o)
  {
    if (this.java1) {
      return;
    }
    Hashtable ht = (Hashtable)((ThreadLocalMap)this.tlm).get();
    if (ht == null) {
      ht = new Hashtable(7);
      ((ThreadLocalMap)this.tlm).set(ht);
    }
    ht.put(key, o);
  }
```
在 Web 应用中增加用户跟踪功能
参考资料：[https://www.ibm.com/developerworks/cn/web/wa-lo-usertrack/index.html#fig1](https://www.ibm.com/developerworks/cn/web/wa-lo-usertrack/index.html#fig1)
#### 2.log4j
log4j2 相比于1，配置等改动较大。生产比较实用的功能有：
- 异步打印，提升程序性能
- 动态配置，这一点对于某些debug日志平时，关闭，关键时刻打开非常方便
实际中，还有一个，我们一般将root设为一个级别，而在排查问题等时，将一些其他的可以针对性配置，如mybatis，你只需要
```
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN">
  <Appenders>
    <Console name="Console" target="SYSTEM_OUT">
      <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
    </Console>
  </Appenders>
  <Loggers>
    <Logger name="xxx.xxx.db.mapper" level="debug" addivity="true">
    </Logger>
    <Root level="info">
      <AppenderRef ref="Console"/>
    </Root>
  </Loggers>
</Configuration>
```
利用addictivity 多余性，可以直接在root的输出中添加进去,结合动态配置的功能，在生产排查问题时，就可以做到不停机的情况下，针对性排查了
