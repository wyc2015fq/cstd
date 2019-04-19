# 开源图形数据库Neo4j介绍与安装 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月26日 14:59:54[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：126
图形数据库是以图形结构形式存储数据的数据库。
[https://neo4j.com/](https://neo4j.com/)
 Java 编写
保存为节点以及节点之间的关系
Neo4j 的数据由下面几部分构成：
- 节点
- 边
- 属性
无论是顶点还是边，都可以有任意多的属性。属性的存放类似于一个 HashMap，Key 为一个字符串，而 Value 必须是基本类型或者是基本类型数组。
关系总是从一个节点指向另一个节点
支持两种 Java API：Cypher API 和 Native Java API 来开发 Java 应用程序
Neo4j分为企业版和社区版，企业版是需要付费的。社区版是免费的。在基本功能上两个版本没有区别，企业版相比社区版拥有一些高级特性，如集群等。具体可参考官网[https://neo4j.com/editions/](https://neo4j.com/editions/)关于两个版本的区别。对于一般中小型应用社区版就足够用了，因此，这里选择社区版。
(2) 网络访问：默认Neo4j只支持本地访问，如果要从网络中的其他主机访问Neo4j，需要修改如下两项配置。
dbms.connector.http.listen_address=0.0.0.0:7474
dbms.connector.bolt.listen_address=0.0.0.0:7687
安装
1，解压
2，
To run Neo4j as a console application
bin/neo4j console
- To run Neo4j in a background process, use:
bin/neo4j start
Visit [http://localhost:7474](http://localhost:7474/)  Neo4j Browser
默认用户和密码都是neo4j
The Neo4j Browser is a tool for developers to explore their Neo4j database, execute Cypher queries and see results in graph form
