# Spring整合JAX-WS - z69183787的专栏 - CSDN博客
2015年07月14日 21:50:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4739
　Jax-ws在使用上很方便，也很轻量级。重点是他是jvnet(dev.java.net)的项目，是基于java标准的(JSR181)。
　　不过它与Spring的整合相对麻烦，于此，我将自己的一些研究结果贴出来以供备忘和后来者参考。
　　首先我们要有组件支持，包括三部分(我们需要他们的jar包)：
[Spring](http://www.springframework.org/)
[jax-ws](https://jax-ws.java.net/)
[jaxws commons spring](https://jax-ws-commons.java.net/spring/)
　　Spring就不用说了，如果大家使用MyEclipse的话就直接添加支持。
　　jax-ws的jar包可以再网站上下载，或者搜索下载“jax-ws 2.2”。
　　jaxws commons spring的jar包是中间件，这个可能不好下载，如果不适用maven的话可能就只能在网络上搜索下载“jaxws-spring-1.8”(包含不只一个jar包哦)。
　　然后我们导入jar包并创建web项目。
![](http://images.cnitblog.com/i/372875/201407/041735272776038.gif)![](http://images.cnitblog.com/i/372875/201407/041735436054593.gif)
　　然后编写一个测试WebService类：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) HelloWorldService
```
　　然后修改applicationContext.xml，加上webservice的绑定，这里面有几个问题，对于wss和ws的schema必须加上：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) applicationContext
```
　　最后修改web.xml，修改几个地方：
　　　　加上Spring的listener，并配置正确；
　　　　加上WSSpringServlet的拦截。
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) web
```
　　最后就启动，搞定。
　　访问localhost:8080/ExJaxwsSpring/service/hello?wsdl就是接口；访问localhost:8080/ExJaxwsSpring就是你的网站。
![](http://images.cnitblog.com/i/372875/201407/041757174657341.gif)
