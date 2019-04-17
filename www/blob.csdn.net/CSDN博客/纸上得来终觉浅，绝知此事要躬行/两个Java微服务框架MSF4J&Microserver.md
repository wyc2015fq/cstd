# 两个Java微服务框架MSF4J&Microserver - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月24日 15:11:30[boonya](https://me.csdn.net/boonya)阅读数：1515











## MSF4J

Github:[https://github.com/wso2/msf4j](https://github.com/wso2/msf4j)





﻿MSF4J 是 Java 轻量级高性能的 WSO2 微服务框架。

示例代码：

```java
//Application.java
public class Application {
    public static void main(String[] args) {
        new MicroservicesRunner()
                .deploy(new HelloService())
                .start();
    }
}


//HelloService.java
package org.example.service; 

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;

@Path("/hello")
public class HelloService {

    @GET
    @Path("/{name}")
    public String hello(@PathParam("name") String name) {
        return "Hello " + name;
    }

}
```

性能比较：



![Throughput](http://static.oschina.net/uploads/img/201602/18080148_vMOC.png)

内存占用比较：

![Memory](http://static.oschina.net/uploads/img/201602/18080149_Nft4.png)








## Microserver

Github:[https://github.com/aol/micro-server](https://github.com/aol/micro-server)





Microserver 是一个零配置、基于标准的身经百战的库，用来运行 Java REST 微服务，通过 Java 标准 main 类执行。从 2014 年开始就一直在 AOL 生产环境中使用。

框架结构：

![high level architecture](http://static.oschina.net/uploads/img/201506/14064805_1XWZ.png)

main 类：

```java
public class AppRunnerTest {
    public static void main(String[] args) throws InterruptedException {
        new MicroserverApp(() -> "test-app").run();
    }
}
```

服务类：

```java
@Rest 
@Path("/status")
public class StatusResource {
  @GET 
  @Produces("text/plain")
  @Path("/ping")
  public String ping() {
     return "ok";
  }
}
```










