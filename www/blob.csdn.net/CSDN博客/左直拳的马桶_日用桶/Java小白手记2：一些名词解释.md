# Java小白手记2：一些名词解释 - 左直拳的马桶_日用桶 - CSDN博客
2016年03月10日 19:58:38[左直拳](https://me.csdn.net/leftfist)阅读数：2053标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
看到[《Java 征途：行者的地图》](http://www.cnblogs.com/mindwind/p/5251430.html) ，这是一篇有关java学习路径文章。对我等Java小白有指引作用。里面提到了一些基本的名词术语，有些我知道，有些不知道，再补上一些自己曾觉得模糊的，记录如下： 
此前已经总结了一些，见拙作：
《 [Java小白手记：WEB项目等](http://blog.csdn.net/leftfist/article/details/46336037)》
0、.java文件 
java源代码。
1、.class文件 
.java文件编译后就是.class文件，二进制，能被JVM所识别。类似windows的dll。
2、.jar文件 
也叫jar包。Java归档文件（Java ARchive），可以用WinZip打开。通常用于聚合大量的Java类文件、相关的元数据和资源（文本、图片等）文件到一个文件，以便分发Java平台应用软件或库。
3、JMS 
Java消息服务（Java Message Service，JMS）应用程序接口，是一个Java平台中关于面向消息中间件（MOM）的API，用于在两个应用程序之间，或分布式系统中发送消息，进行异步通信。Java消息服务是一个与具体平台无关的API，绝大多数MOM提供商都对JMS提供支持。
4、JNDI 
Java 命名与目录接口（Java Naming and Directory Interface），是一个比较重要的J2EE规范。其作用，就像我们代码中的变量，一个短短的变量，其对应的值内容可以很多很多，很大很大。 
有篇极好的介绍文章：《[JNDI 是什么](http://blog.csdn.net/zhaosg198312/article/details/3979435)》
5、JTA 
JTA，即Java Transaction API，Java事务API。 
JTA事务比JDBC事务更强大。一个JTA事务可以有多个参与者，而一个JDBC事务则被限定在一个单一的数据库连接。换言之，JTA支持分布式事务。
6、JAT 
JAT（Java Agent Template ）是一种灵活的Java基础框架，可以很容易的扩展它来改善工程的启动时间。（又JTA，又JAT，很容易搞混吧？）
7、JTS 
Java事务服务（JTS：Java Transaction Service）。JTA 和 JTS 为J2EE平台提供了分布式事务服务（distributed transaction）。
8、EJB 
EJB (Enterprise JavaBean)是J2EE(javaEE)的一部分，定义了一个用于开发基于组件的企业多重应用程序的标准。设计目标与核心应用是部署分布式应用程序。简单来说就是把已经编写好的程序（即：类）打包放在服务器上执行。
在J2EE里，EJB称为Java 企业Bean，是Java的核心代码，分别是会话Bean（Session Bean），实体Bean（Entity Bean）和消息驱动Bean（MessageDriven Bean）。
但听说，早期的太复杂，没能提高开发效率，所以被各种框架（比如SSH）所取代。不知道EJB3.0如何。
9、POJO 
简单的Java对象，实际就是普通JavaBeans，是为了避免和EJB混淆所创造的简称。 
简单理解，POJO就是个实体类，里面只有属性，没啥业务逻辑。
