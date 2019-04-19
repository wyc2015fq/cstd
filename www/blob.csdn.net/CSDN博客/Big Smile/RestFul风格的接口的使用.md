# RestFul风格的接口的使用 - Big Smile - CSDN博客
2017年06月18日 23:46:32[王啸tr1912](https://me.csdn.net/tr1912)阅读数：9611
        最近小编一直在使用Restful风格的接口来做后台编程，有一些想法，在此分享给大家。
# 一、起源
        因为现代互联网发展，随着互联网的发展，我们日常使用的软件越来越偏向于网络应用程序来发展，甚至随着我们的访问终端的多样化，网络应用变得越来越方便，也越来越流行，与之相对应的就是前端和后端的开发和构成的分离，对于我们的应用独立解耦和的开发十分的重要，由此出现了API编程这个概念。
        在API编程中甚至出现了API First的概念，而Restful风格的API是比较成熟的一套API设计理论，所以现在有很多的人都在使用这个标准来开发。并且作为一种基于HTTP协议的接口类型服务，Rrestful也有他自己的一套合理的，简单的规范可供咱们使用。
二、Restful风格
        说道Restful风格，其实它分为几个规范。
**1、应用的部署设计规范**
            一般我们写网络应用程序的时候都会把它放在一个应用服务器上面，然后通过应用服务器的运行，发布，部署，然后形成了一个容器，可以让你的应用程序在里面跑起来，通过发布的接口和电脑的IP地址可以访问到你的应用服务器，然后通过一个应用发布的标识可以访问到你的应用程序这就是一个定位访问的例子，在我们部署的时候也是如此，比如在Tomcat中发布了一个应用名为shop的应用程序，我们就可以用localhost:8080/shop  就可以访问到我们的shop应用程序的默认页面。
**  2、方法的访问**
            但是Restful风格接口从本质上来说是一种接口，他也包括了很多接口的访问规范，在我看来就是把整个接口的访问通过一个标准给转化成了Http请求，我们通过在地址栏输入URL就可以访问到方法并且可以传输数据和接收数据。这里我们把这个URL的访问约定写在了程序的controller层，以@Path注解的方式存在：
![](https://img-blog.csdn.net/20170618231944075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们可以看到在一个类上有一个@Path注解，这个代表通过这个path可以定位到这个类里面来，然后我们在类的方法上面还要有一个@Path注解，然后我们可以发现这个里面还有一个标识，是一个请求方式的标识，主要有POST ,GET,PUT,DELETE。
![](https://img-blog.csdn.net/20170618232615754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        那么这个整体的流程是什么呢？
首先我们需要先引入Restful的jax-rs协议的jar包，有：
```java
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
```
         我们可以看到在这些Jar包中，包含了我们用到的注解的一些引用，包括了Path ，还有请求的传输方法的包如post，get等，那么我们为什么要分不同的请求类型呢，请求类型的这几种都有什么作用呢？
# 三、请求类型（HTTP动词）
        请求分这么多的类型呢，主要是因为我们要实现的目的不同，所以需要引入不同的请求，并且返回的数据处理的方式在不同的请求中也是不同的。
        常用的请求类型有如下5中，括号中是对应的请求的sql语句类型
- GET（SELECT）：从服务器取出资源（一项或多项）。
- POST（CREATE）：在服务器新建一个资源。
- PUT（UPDATE）：在服务器更新资源（客户端提供改变后的完整资源）。
- PATCH（UPDATE）：在服务器更新资源（客户端提供改变的属性）。
- DELETE（DELETE）：从服务器删除资源。
还有两个不常用的请求
- HEAD：获取资源的元数据。
- OPTIONS：获取信息，关于资源的哪些属性是客户端可以改变的。
所以我们在写不同的查询方法的时候需要用到不同的请求类型（动词）来进行注解。
# 四、总结
        总而言之，我们想要用Restful风格的接口，需要遵循jax-rs的协议，同时也要引用动词和其他Restful的注解，方可进行接口的构建，在这里要强调一下，一个类的path即是这个类的访问URL路径，比如有个类为student，他的Path注解为“/api/student” 那么我们在访问的时候就可以写成“http://loaclhost:8080/应用名/api/student” 然后我们在里面写方法的时候可以不用path标记，但是必须用类型标记，如@GET，那么我们在发送请求的时候只要发送一个get类型的请求，访问地址还是“http://loaclhost:8080/应用名/api/student”，我们就可以直接访问到我们写的这个方法了。
        对于Restful风格的接口如何进行测试，我们下篇文章接着谈。
