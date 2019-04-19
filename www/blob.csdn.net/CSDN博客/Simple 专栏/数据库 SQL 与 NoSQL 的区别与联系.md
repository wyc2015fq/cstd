# 数据库 SQL 与 NoSQL 的区别与联系 - Simple 专栏 - CSDN博客
2018年11月16日 15:48:10[Simple_Zz](https://me.csdn.net/love284969214)阅读数：54标签：[SQL																[NoSQL																[大数据](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=NoSQL&t=blog)](https://so.csdn.net/so/search/s.do?q=SQL&t=blog)
个人分类：[大数据](https://blog.csdn.net/love284969214/article/category/8000571)
### NoSQL也称为Not only SQL
可理解为，除关系型数据库以外的其他数据库系统的统称。
特点，菲关系型、分布式、分布式、不提供ACID的数据库设计模式等特征。
### RDBMS
RDBMS即关系数据库管理系统(Relational Database Management System)，是将数据组织为相关的行和列的系统，而管理关系数据库的计算机软件就是关系数据库管理系统。
### **NoSQL与RDBMS的九点区别联系 **
    1 理解ACID与BASE的区别（ACID是关系型数据库强一致性的四个要求，而BASE是NoSQL数据库通常对可用性及一致性的弱要求原则，它们的意思分别是，ACID：atomicity, consistency, isolation, durability;BASE：Basically Available, Soft-state, Eventually Consistent。同时有意思的是ACID在英语里意为酸，BASE意思为碱）
    2 理解持久化与非持久化的区别。这么说是因为有的NoSQL系统是纯内存存储的。
    3 你必须意识到传统有关系型数据库与NoSQL系统在数据结构上的本质区别。传统关系型数据库通常是基于行的表格型存储，而NoSQL系统包括了列式存储（Cassandra）、key/value存储（Memcached）、文档型存储（CouchDB）以及图结构存储（Neo4j）
    4与传统关系数据库有统一的SQL语言操作接口不同，NoSQL系统通常有自己特有的API接口。
    5 在架构上，你必须搞清楚，NoSQL系统是被设计用于成百上千台机器的集群中的，而非共享型数据库系统的架构。
    6在NoSQL系统中，可能你得习惯一下不知道你的数据具体存在何处的情况。
    7 在NoSQL系统中，你最好习惯它的弱一致性。”eventually consistent”(最终一致性)正是BASE原则中的重要一项。比如在Twitter，你在Followers列表中经常会感受到数据的延迟。
    8 在NoSQL系统中，你要理解，很多时候数据并不总是可用的。
    9 你得理解，有的方案是拥有分区容忍性的，有的方案不一定有。
