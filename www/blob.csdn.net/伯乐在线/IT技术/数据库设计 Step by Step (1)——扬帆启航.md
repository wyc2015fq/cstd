# 数据库设计 Step by Step (1)——扬帆启航 - 文章 - 伯乐在线
原文出处： [知性思新](http://www.cnblogs.com/DBFocus/archive/2011/03/27/1996655.html)
引言：一直在从事数据库开发和设计工作，也看了一些书籍，算是略有心得。很久之前就想针对关系数据库设计进行整理、总结，但因为种种原因迟迟没有动手，主要还是惰性使然。今天也算是痛下决心开始这项卓绝又令我兴奋的工作。这将是一个系列的文章，我将以讲座式的口吻展开讨论（个人偷懒，这里的总结直接拿去公司培训新人用）。
系列的第一讲我们先来回答下面几个问题
![image](http://jbcdn2.b0.upaiyun.com/2016/03/20b525d32e9c6ea02e947da009964a1c.png)
**数据库是大楼的根基**
大多数程序员都很急切，在了解基本需求之后希望很快的进入到编码阶段（可能只有产出代码才能反映工作量），对于数据库设计思考得比较少。
这给系统留下了许多隐患。许多软件系统的问题，如：输出错误的数据，性能差或后期维护繁杂等，都与前期数据库设计有着密切的关系。到了这个时候再想修改数据库设计或进行优化等同于推翻重来。
我经常把软件开发比作汽车制造。汽车制造会经过图纸设计，模型制作，样车制造，小批量试生产，最后是批量生产等步骤。整个过程环环相扣，后一过程是建立在前一过程正确的前提基础之上的。如果在图纸设计阶段发现了一个纰漏，我们可以重新进行图纸设计，如果到了样车制造阶段发现这个错误，那么我们就要把从图纸设计到样车制造的阶段重来，越到后面发现设计上的问题，所付出的代价越大，修改的难度也越大。
数据库是整个应用的根基，没有坚实的根基，整个应用也就岌岌可危了。
**强大的数据库面对不良设计也无能为力**
现代数据库管理系统（DBMS）提供了方便的图形化界面工具，通过这些工具可以很方便的创建表、定义列，但我们设计出的结构好吗？
关系数据库有许多非常好的特性，但设计不当会使这些特性部分或完全的丧失。
我们来看看以下几个数据库不良设计造成的场景：
1. 数据一致性的丧失
一个订单管理系统，维护着客户和客户下的订单信息。使用该系统的用户在接到客户修改收货地址的电话后，在系统的客户信息页面把该客户的收货地址进行了修改，但原先该客户的订单还是送错了地址。
2. 数据完整性的丧失
公司战略转移，准备撤出某地区。系统操作人员顺手把该地区的配置信息在系统中进行删除，系统提示删除成功。随后问题就来了，客服人员发现该地区的历史订单页面一打开就出错。
3. 性能的丧失
一个库存管理系统，仓库管理员使用该系统记录每一笔进出货情况，并能查看当前各货物的库存情况。在系统运行几个月后，仓库管理员发现打开当前库存页面变得非常慢，而且整个趋势是越来越慢。
上面这些场景都是由于数据库设计不当造成的，根源包括：设计时引入了冗余字段，没有设计合理的约束，对性能没有进行充足设计等，上面的例子也只是沧海一粟。
![image](http://jbcdn2.b0.upaiyun.com/2016/03/86f8e5bf4d26be1e2b2d989c793a2dc4.png)
**数据库平台无关性**
我在这个系列博客里讨论的数据库设计不针对任何一个关系数据库产品。无论你使用的是Oracle，SQL Server，Sybase，亦或是开源数据库如：MySQL，SQLite等，都可以用来实践我们这里讨论的设计方法和设计理念，设计是这个系列博文的核心和灵魂。
注：在文中我会选用一个数据库产品来进行演示，大家可以选用自己熟悉的数据库产品来实验。本文最后会给出一些免费数据库产品的链接，大家可以下载学习。
**一起学习共同进步**
无论你是数据库设计师，应用架构师，软件工程师，数据库管理员（DBA），软件项目经理，软件测试工程师等项目组成员，都能从该系列博文中有所收获。大家一起讨论，共同进步。
**内容涉及领域**
我对这一系列博文现在的设想是涉及数据库设计的整个过程。从需求分析开始，到数据库建模（概念数据建模），进行范式化，直至转化为SQL语句。
![image](http://jbcdn2.b0.upaiyun.com/2016/03/3c776c52c9455cf5ae9852ec93b04ace.png)
在我们一头扎进数据库设计之前，我们先了解一下除了关系型数据库之外的数据存储方式。
**平面文件（Flat File）**
包括以.txt和.ini结尾的文件。
eg: 一个.ini文件的内容：
————————————————————
[WebSites]
MyBlog=http://www.cnblogs.com/DBFocus
[Directorys]
Image=E:DBFocus ProjectImg
Text=E:DBFocus ProjectDocuments
Data=E:DBFocus ProjectDB
————————————————————
优点：
> 
文件的存储形式非常简单，普通的编辑器都能对其进行打开、修改
缺点：
> 
无法支持复杂的查询
没有任何验证功能
对平面文件中间的内容进行插入、删除操作其实是重新生成了一个新文件
适用场景：
> 
存放小量，修改不频繁的数据，如应用配置信息
**Windows注册表**
错误的修改Windows注册表会引起系统的紊乱，故不建议把很多数据存放在注册表中。
Windows注册表为树形结构，存放着一些系统配置信息和应用配置信息。
通过把不同的配置存放在注册表的不同分支上，使得应用程序公共配置信息与用户个人配置信息分离。
eg：某文档版本管理系统，能通过配置与本主机上安装的文件比较器建立关联进行文档比较。这是一个公共配置信息，文件比较器路径可以存放在注册表的HKEY_LOCAL_MACHINESOFTWARE分支下。
同时该文档版本管理系统能记录用户最近打开的10个文档路径。这是用户个人配置信息，对于不同的Windows用户最近打开的10个文档可以不同，这些配置信息可存放在注册表的HKEY_CURRENT_USERSoftware分支下。
**Excel表单（Spreadsheets）**
优点：
> 
Excel 非常普及，用户对于Spreadsheet的表现形式非常熟悉
可以进行简单统计，方便出各种图表
缺点：
> 
不适用于许多Spreadsheet之间关系复杂的情况
无法应对复杂查询
数据验证功能弱
适用场景：
> 
数据量不是非常大的办公自动化环境
**XML**
XML是一种半结构化的数据。相比于超文本标记语言（HTML），其标签是可以自行定义的，即可扩展的。
eg：一个XML文件内容
—————————————————–


```
<?xml version=”1.0” encoding=”UTF-8” ?>
<ClassSchedule>
     <Class Name=“Psychology” Room=”Field 3”>
          <Instructor>Richard Storm</Instructor>
          <Students>
               <Student>
                     <FirstName>Ben</FirstName>
                     <LastName>Breaker</LastName>
               </Student>
               <Student>
                     <FirstName>Carol</FirstName>
                     <LastName>Enflame</LastName>
                     <NickName>Candy</NickName>
               </Student>
          </Students>
     </Class>
</ClassSchedule>
```
—————————————————–
XML文件有几个特点。
首先，XML标签要求严格对应，且不能出现交错的现象。
其次，XML文件必须有一个根节点，该节点包含所有其他元素。
第三，同级别的不同节点内不必包含相同的元素，如上例中第二个学生Carol有一个特别的节点NickName。这个特性使得在某些场景中XML比关系数据库更能应对变化。
优点：
> 
自然的层次型结构
文本内容通过标签是自解释的
通过XSD（XML Schema语言）可以验证XML的结构
有许多辅助型技术如：XPath, XQuery, XSL, XSLT等
一些商业数据库（如Oracle，SQL Server）已支持XML数据的存储与操作
缺点：
> 
数据的冗余信息较多
无法支持复杂的查询
验证功能有限
对XML中间的内容进行插入、删除操作其实是重新生成一个新文件
适用场景：
> 
适合存放数据量不大，具有层次型结构的数据，如树形配置信息
**NoSQL数据库**
非关系型数据库我接触的不是很多，除了给出一些产品名称之外不做很多展开。园子里已有一些文章，本文最后也给出了链接供大家学习、研究。
1. Key-Value数据库
> 
Redis, Tokyo Cabinet, Flare
2. 面向文档的数据库
> 
MongoDB, CouchDB
3. 面向分布式计算的数据库
> 
Cassandra, Voldemort
这几年NoSQL非常热。我认为NoSQL并不是“银弹”，在某些SNS应用场景中NoSQL显示了其优越性，但在如金融行业等对数据的一致性、完整性、可用性、事务性高要求的场景下，现在的NoSQL就未必适用。我们应充分分析应用的需求，非常谨慎地选择技术和产品。
![image](http://jbcdn2.b0.upaiyun.com/2016/03/de89a8f95d41f440581b667fc62833ee.png)
**主要内容回顾**
1.数据库设计对于软件项目成功的关键作用
2.本课程与数据库产品无关，核心是设计的理念和方法
3.各种数据存储所适用的场景
**参考资料**
1. [*Oracle* Database 10g *Express* Edition](http://www.google.com.hk/url?sa=t&source=web&cd=1&ved=0CCMQFjAA&url=http%3A%2F%2Fwww.oracle.com%2Ftechnetwork%2Fdatabase%2Fexpress-edition%2Foverview%2Findex.html&ei=NxeOTcCPKYKavAOptpCoDQ&usg=AFQjCNFOJiAhUUQvPBsNyORhipe7zzPZpA)
2. [*SQL Server* 2008 R2 *Express* – Overview](http://www.google.com.hk/url?sa=t&source=web&cd=1&ved=0CBgQFjAA&url=http%3A%2F%2Fwww.microsoft.com%2Fexpress%2FDatabase%2F&ei=fBeOTZ70GobKvQPQ6aG6DQ&usg=AFQjCNHjeaYABXz3Vpzmoqyz_kqbTWCfuA)
3. [*SQLite* Home Page](http://www.google.com.hk/url?sa=t&source=web&cd=1&ved=0CCQQFjAA&url=http%3A%2F%2Fwww.sqlite.org%2F&ei=pxeOTbXNC4HIvQPB2NmcDQ&usg=AFQjCNEakQjCFFXx7pu-fq8qk9wUOlC1Bg)
4. [*NoSQL*数据库笔谈](http://www.google.com.hk/url?sa=t&source=web&cd=4&ved=0CEAQFjAD&url=http%3A%2F%2Fsebug.net%2Fpaper%2Fdatabases%2Fnosql%2FNosql.html&ei=yheOTeTqGoOOuQOSm8S5DQ&usg=AFQjCNHv9kYt5Rh5DRDu7s-8R37kfO1bAw)
