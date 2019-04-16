# 用Apache CouchDB存储医疗可视化数据 - 沧海一粟 —— 技术随手记 - CSDN博客





2015年07月29日 22:26:59[慢游](https://me.csdn.net/eengel)阅读数：3473








# 介绍

之前花了些时间探讨用D3.js做医疗数据的可视化，所用的数据来自国家卫生和计划生育委员会统计信息中心网站。简便起见，相关的数据做成JSON格式，并存在了本地JSON文件中。对于短期试验性开发，对数据的存储并没有太多要求。但随着时间流逝，我也想能用比较“正式”的方法来存储相应的数据，也就是采用数据库。

当前的数据库技术可谓流派众多，经典的Relational Database依旧占据了大半江山，而NoSQLDatabase也不逞多让，各种新技术层出不穷。孰优孰劣，得看具体的项目需求。在本系列试验中，对数据存储的要求主要有三点：

1.     能从客户端直接访问数据。为了让工作重心在数据及其可视化上，需要尽量弱化甚至取消服务器端的业务逻辑。比如现在的可视化实现通过直接在浏览器中访问HTML网页得到。而预期数据的获取也直接在HTML+JavaScript(D3.js)中完成。

2.    数据格式尽量接近客户端做可视化时需要的格式。这样就可以尽量减少花在格式转换上的时间精力。

3.    简单易学。常用功能上手快。毕竟是一个试验性的项目，尽量提高效率吧。

综合考虑后，我选择了ApacheCouchDB：直接提供RESTful API来访问和操作数据库，直接存储JSON格式，安装简单、容易上手。



# 安装和运行CouchDB

Apache CouchDB可以直接从[官网](http://couchdb.apache.org/)上下载安装包安装。安装完后直接运行，就可以打开它自带的控制台Futon。在上面可以很容易地创建Databases、Documents以及Fields。也可以通过cURL用命令行的方式操作，看个人习惯。

在这儿我创建了一个Database叫healthdata，在里面创建了一个Document，然后把原来JSON文件中的内容都放到了这个Document中。要注意的是CouchDB会为这个Document自动生成两个Field：_id,*rev。*在后续处理中要注意。完成后数据就可以通过http://127.0.0.1:5984/healthdata/{_id}访问了。

其实数据库的数据存储任务已经完成了。但为了后续让D3能直接从网页中访问到数据URL，还需要做一些配置。

1.     通过Futon界面打开Configuration，在Section cors中增加一个Option origins，Value设成*；

2.     在Section httpd中，将Option enable_cors的Value设成true。



# 在D3.js中调用数据库连接

在《[用D3.js进行医疗数据可视化 （四） 堆积区图 (Stacked Area Chart)](http://blog.csdn.net/eengel/article/details/47029301) 》的基础上，修改一行代码就能达到同样的效果：



```java
d3.text("http://127.0.0.1:5984/healthdata/698aed33444a75cddb94bd147f000ea4", function(rawdatastr){…});
```



# 结论

Apache CouchDB + D3.js的组合，现在看来还比较合适。很轻量级，但包含了必要的功能。后面有机会可以继续挖掘这个组合的潜力。



