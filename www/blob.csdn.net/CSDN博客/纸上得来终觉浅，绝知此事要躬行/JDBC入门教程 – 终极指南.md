# JDBC入门教程 – 终极指南 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 20:57:03[boonya](https://me.csdn.net/boonya)阅读数：1178








This article is from:[http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html)






本教程主要介绍JDBC（Java数据库连接）—— 一个Oracle提供的API，它可以帮助程序员处理Java应用程序中不同数据库的访问问题：程序员可以通过它建立与数据库的连接；定义特定的客户端使之可以访问给定的数据库；提供一种能够读取、插入、更新和删除数据库中的数据项的机制；以及控制由不同SQL语句组成的事务。


在本文中我们会介绍主要的JDBC组件，包括声明（Statement）、结果集（Result Set）、存储过程（Stored Procedure）。


程序员们需要针对不同的数据库提供相应的驱动程序；在后面的章节我们会结合一些实例来详细解释。


JDBC是伴随Java的产生而产生的；它的第一个版本产生于1997年2月，也就是JDK1.1的发布日期；自那以后，JDBC已经成为Java的一个重要组成部分。JDBC相关的主要包有： [http://docs.oracle.com/javase/8/docs/api/java/sql/package-summary.html](http://docs.oracle.com/javase/8/docs/api/java/sql/package-summary.html)和[http://docs.oracle.com/javase/8/docs/api/javax/sql/package-summary.html](http://docs.oracle.com/javase/8/docs/api/javax/sql/package-summary.html).


关于JDBC的最新版本和开发、维护信息可以访问[JSR 221](https://jcp.org/en/jsr/detail?id=221)获得。


本文中的所有例子的实现环境是Java 8 update 0_25和Eclipse SDK  Luna 4.4. 你可以在本文的最后下载所有这些例子，而且还可以下载其他更多的例子！


## 目录
- [1. 组件（Components）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#components)
- [2. 数据库连接（Connections）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#connections)
- [3. 数据类型（Data types）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#dataTypes)
- [4. 驱动程序（Drivers）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#drivers)
- [5. 数据库（Databases）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#dbs)
- [6. 结果集（Result Sets）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#resultsets)
- [7. 存储过程（Stored Procedures）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#stored)
- [8. 声明（Statements）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#statements)
- [9. 批处理命令（Batch Commands）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#batch)
- [10. 事务（Transactions）](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#transactions)
- [11. CRUD 命令](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#crud)
- [12. Java 8](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#java8)
- [13. 基于JDBC的SQL类库](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#libraries)
- [14. 单元测试](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#unitTesting)
- [15. 总结](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#summary)
- [16. 源码下载](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#download)
- [17. 相关链接](http://www.javacodegeeks.com/zh-hans/2015/03/jdbc%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#links)




 由于原文内容庞大，故仅转载文章目录介绍，以备下来学习方便！



