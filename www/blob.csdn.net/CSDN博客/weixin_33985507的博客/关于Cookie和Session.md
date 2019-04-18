# 关于Cookie和Session - weixin_33985507的博客 - CSDN博客
2017年06月28日 14:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
以前很少写后台，老是搞不明白cookie和session的工作流程。今天总结一下他们是怎么创建和相互联系的。
#### 1.诞生
说起这哥俩的诞生，应该怪罪于HTTP协议，为啥呢？众所周知，HTTP协议是一种无状态的协议，所以如果服务器要记录终端用户的状态的时候，单单依赖HTTP协议是做不到的。所以cookie和session就被搞出来了。session存放在服务器端，用于标示特定的一个用户并跟踪用户状态。但是，服务器怎么识别来自遥远的客户端的用户呢？终于，小二cookie要出场了，cookie存放在客户端，它最主要的功能之一就是保存session id，cookie会跟着http请求一起造访服务器，这样cookie和session就有机会搞到一起了(呵呵呵...😊)；
#### 2.session和cookie交互及创建的流程。
当客户端http请求到达服务器的时候，服务器会去尝试到跟http请求一起到达的cookie里面去获取session id，如果获取到了，就根据这个session id到数据库或者其他地方获取对应的session。如果cookie中没有session id，就会新建一个session, 然后服务器会在HTTP协议中告诉客户端，需要在cookie里设置一个session id，这样下次小cookie再来的时候，我就问他要session id去获取对应的session就好了。
session中一般存储登录用户的信息（比如数据库表User中的某一个用户的整条记录）。所以找到对应的session就能知道是谁在客户端了。
#### 3.redis+SpringBoot
如果用redis来集中式管理session，只需要添加以下依赖
```
<dependency>
                        <groupId>org.springframework.session</groupId>
            <artifactId>spring-session-data-redis</artifactId>
            <version>1.2.1.RELEASE</version>
</dependency>
```
然后，开启`@EnableRedisHttpSession`注解即可。
```
@Configuration
@EnableRedisHttpSession(maxInactiveIntervalInSeconds = 86400*30)//设置session过期时间
public class SessionConfig {
    
}
```
`@EnableRedisHttpSession`注解会创建一个`SessionRepositoryFilter`，`SessionRepositoryFilter`是一个优先级最高的Filter，这个Filter负责用Spring Session来替换原先的默认Http Session的创建和管理工作。
