# 从零讲Java，给你一条清晰地学习道路！该学什么就学什么！ - Spark高级玩法 - CSDN博客
2018年06月09日 00:12:46[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：139
**1. 计算机基础：**
### 1.1 数据结构基础：
主要学习：
1.向量，链表，栈，队列和堆，词典。熟悉
2.树，二叉搜索树。熟悉
3.图，有向图，无向图，基本概念
4.二叉搜索A，B，C类熟练，9大排序熟悉。
5.树的前中后，层次，之字，最短路。
6.KMP等字符串算法。
### 1.2 操作系统：
主要学习：
1.进程，线程，进程线程区别。进程间通信
2.进程调度算法理解
3.存储，虚拟内存，分页分段，内存调度算法
4.文件系统，链式，索引
5.死锁：原因，避免，解除k
6.Linux基本操作命令，会一门脚本编程：shell，Python
### 1.3 计算机网络：
主要学习：
1.网络分层：五层，七层，四层等
2.网络层：IP分类，子网划分，超网。ARP协议，ICMP协议。理解内部网关和外部网关，vpn等
3.运输层：端口，UDP协议理解，TCP协议理解，三次握手，四次断开。
4.应用层：了解常用协议使用的端口和底层协议。
5.更加深入理解：TCP/IP 对拥塞控制，滑动窗口，如何实现可靠等。
### 1.4 基础算法：
主要学习：
1.理解大O等记号
2.动态规划如：几类背包问题最好有现成的代码用于笔试。最长公共子序列
3.贪心算法：理解赫夫曼编码（笔试常考概念）
4.B树B+树用于理解数据库索引
5.常用的图算法：广度优先，深度优先，最短路径算法（最好有现成的代码），最小生成树。
## **2. JAVA知识基础：**
### 2.1 JAVA基础
推荐书籍：《编程思想》
1.掌握java常用技术，io、多线程、反射、常用集合框架
2.对处理输入输出的IO进行熟悉，用于笔试
3.对Iterator，Iterable和Comparable,Comparator了解，以及常用集合的遍历和排序。
4.对NIO一定要熟悉，以及了解JMS和RPC
5.对JAVA基础概念要熟悉，比如抽象类和接口区别等，这些熟记。
### 2.2 JAVA源码
1.hashmap，hashtable，concurrentHashMap源码以及区别
2.其他集合框架：linkedHashMap，LinkedHashSet，Copy-On-Write等
3.对Lock，Object，String等类的源码了解。
**2.3 JAVA并发API**
推荐书籍：应付笔试面试：《JAVA多线程编程与核心技术》；真正神书：《JAVA并发编程实战》。
1.Synchronize 和 Lock 的区别与用法
2.wait()和await() notify()和signal()，wait()和sleep()区别与用法
3.其他的API用法：yield，join等
4.voliate深入理解，以及CAS理解
5.JAVA锁底层实现：自旋锁，阻塞锁，可重入锁，偏向锁，等。
6.线程池和阻塞队列
7.以及一些常用的并发实现案例如生产者和消费者
**2.4 理解JVM**
推荐书籍：《深入理解JAVA虚拟机》
1.JAVA内存模型，哪个区域存放哪些内容
2.引用技术法和可达性分析法
3.对象的强、软、弱和虚引用区别与作用
4.jvm垃圾回收机制，以及回收算法
5.常见的OOM和SOF的原因和解决方案，以及通过什么工具进行分析。
6.常见的JVM输入参数熟悉
7.了解GC停顿，逃逸分析，TLAB区，堆外内存等
8.常用垃圾收集器：G1，CMS等
9.了解常用性能监控工具：如visualvm jconsole等
10.必须熟悉虚拟机类加载机制，包括原理步骤，以及双亲委派方式的原理。
**3. 常规储备知识学习**
**3.1 数据库：**
推荐书籍：sql入门：《Mysql必知必会》；深入理解：《高性能Mysql》
1.常用关联查询语法，自己所动手写几个
2.数据库索引深入认识和理解
3.JDBC学习
4.常用数据库调优手段：比如通过索引调优
5.InnoDB和MyISAM 区别
6.了解主键，外键，索引，唯一索引
7.了解索引、视图、触发器、游标和存储过程
8.常用关系型数据库和非关系型数据库：关系型了解：Mysql，Oracle 非关系型了解：redis，Hbase，monogodb等
**3.2 设计模式：**
推荐书籍：《设计模式之禅》
1.单例模式的几种写法一定要会
2.JDK中对应的设计模式：如IO中的装饰模式和设配器模式等
3.框架中常用的设计模式：如Struts中的责任链模式，Spring中的工厂模式，动态代理模式等
4.了解6大设计原则
**3.3 HTTP协议：**
推荐书籍：《HTTP权威指南》
1.什么是HTTP，协议版本等
2.学会使用浏览器查看HTTP请求与响应
3.熟悉HTTP请求头和请求体
4.熟悉HTTP响应头和响应体
5.了解Rest风格
**3.4神一般的Spring：**
推荐书籍：《Spring实战》和《Spring技术内幕》
1.了解IOC原理和作用
2.了解AOP原理和作用
3.常用的XML配置
4.Spring事务原理和使用
5.SpringMVC
6.了解Spring的webflow，远程服务，rest，消息，安全等
7.新功能：Spring boot和cloud
**4. 后端学习之路**
**4.1 WEB后端学习之路**
推荐书籍：可以选择一些SSH整合开发之类的
1.JSP和Servlet学习
2.XML和JSON解析学习
3.前端知识：HTML、css，js
4.JDBC学习，文件上传下载，JAVAMail等
5.可以利用jsp和Servlet开发简单项目
6.接着框架学习：Struts2，Hibernate，Spring以及相互整合开发
7.接着可以了解：SpringMvc，Mybatis，Lucene，shiro等
8.利用框架实现简单项目比如CMS。
**4.2 服务架构学习之路（此部分笔试面试不一定学习）**
推荐书籍：《分布式服务框架原理与实践和微服务设计》
1.了解SOA和微服务
2.负载均衡：LVS，NGINX等
3.RPC框架学习：RMI，Thrift等
4.消息框架学习：ActiveMq等
5.Netty通信框架学习
6.服务框架：dubbo
**4.3 Hadoop学习之路（此部分笔试面试不一定学习）**
推荐书籍：《Hadoop权威指南》
1.熟悉mapReduce和HDfS
2.Hadoop集群搭建
3.了解：zk，hive，hbase，pig，mahout等
4.zk几个常用作用如做注册中心，放配置信息等。
- 
**原文：cnblogs.com/nbalive2001/p/6418069.html**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
