# DB2数据库 - fanyun的博客 - CSDN博客
2016年11月02日 14:02:40[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：346
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
IBM DB2 是[美国](http://baike.baidu.com/view/2398.htm)[IBM](http://baike.baidu.com/view/1937.htm)公司开发的一套[关系型数据库管理系统](http://baike.baidu.com/view/1450387.htm)，它主要的运行环境为[UNIX](http://baike.baidu.com/view/8095.htm)包括IBM自家的[AIX](http://baike.baidu.com/view/349664.htm)）、[Linux](http://baike.baidu.com/view/1634.htm)、IBM
 i（旧称OS/400）、[z/OS](http://baike.baidu.com/view/1245216.htm)，以及[Windows](http://baike.baidu.com/view/4821.htm)服务器版本。
DB2主要应用于大型应用系统，具有较好的可伸缩性，可支持从大型机到单用户环境，应用于所有常见的服务器操作系统平台下。
 DB2提供了高层次的数据利用性、完整性、安全性、可恢复性，以及小规模到大规模应用程序的执行能力，具有与平台无关的基本功能和SQL命令。DB2采用了数据分级技术，能够使大型机数据很方便地下载到LAN数据库服务器，使得客户机/服务器用户和基于LAN的应用程序可以访问大型机数据，并使数据库本地化及远程连接透明化。
 DB2以拥有一个非常完备的查询优化器而著称，其外部连接改善了查询性能，并支持多任务并行查询。 DB2具有很好的网络支持能力，每个子系统可以连接十几万个分布式用户，可同时激活上千个活动线程，对大型分布式应用系统尤为适用。
 DB2除了可以提供主流的[OS/390](http://baike.baidu.com/view/744300.htm)和VM[操作系统](http://baike.baidu.com/view/880.htm)，以及中等规模的AS/400系统之外，IBM还提供了跨平台（包括基于[UNIX](http://baike.baidu.com/view/8095.htm)的[LINUX](http://baike.baidu.com/view/1634.htm)，[HP-UX](http://baike.baidu.com/view/58963.htm)，[Sun](http://baike.baidu.com/view/24856.htm)[Solaris](http://baike.baidu.com/view/329359.htm)，以及[SCO](http://baike.baidu.com/view/276950.htm)UnixWare；还有用于个人电脑的[OS/2](http://baike.baidu.com/view/108542.htm)操作系统，以及微软的[Windows
 2000](http://baike.baidu.com/view/153378.htm)和其早期的系统）的DB2产品。DB2[数据库](http://baike.baidu.com/view/1088.htm)可以通过使用微软的开放数据库连接（[ODBC](http://baike.baidu.com/view/41321.htm)）接口，Java数据库连接（[JDBC](http://baike.baidu.com/view/25611.htm)）接口，或者[CORBA](http://baike.baidu.com/view/153815.htm)接口代理被任何的[应用程序](http://baike.baidu.com/view/330120.htm)访问。
       DB2有众多的版本，或者是许可证。为了弱化“版本”的概念增强可选择性，IBM允许客户不购买他们不需要的特性。示例版本包括Express、Workgroup和Enterprise版本。基于Linux/UNIX/Windows的最复杂的版本是DB2
 Data Warehouse Enterprise Edition，缩写为DB2 DWE。这个版本的偏重于混合工作负荷（线上交易处理和[数据仓库](http://baike.baidu.com/view/19711.htm)）和[商业智能](http://baike.baidu.com/view/21020.htm)的实现。DB2
 DWE包括一些商务智能的特性例如[ETL](http://baike.baidu.com/view/69207.htm)、数据发掘、[OLAP](http://baike.baidu.com/view/57810.htm)加速以及in-line
 analytics。
        DB2有众多的版本，或者是许可证。为了弱化“版本”的概念增强可选择性，IBM允许客户不购买他们不需要的特性。示例版本包括Express、Workgroup和Enterprise版本。基于Linux/UNIX/Windows的最复杂的版本是DB2
 Data Warehouse Enterprise Edition，缩写为DB2 DWE。这个版本的偏重于混合工作负荷（线上交易处理和[数据仓库](http://baike.baidu.com/view/19711.htm)）和[商业智能](http://baike.baidu.com/view/21020.htm)的实现。DB2
 DWE包括一些商务智能的特性例如[ETL](http://baike.baidu.com/view/69207.htm)、数据发掘、[OLAP](http://baike.baidu.com/view/57810.htm)加速以及in-line
 analytics。
**DB2 Everyplace**
主要用于移动计算。[](http://baike.baidu.com/pic/DB2/7034285/1410906/b3508d136d41bee76538db8c?fr=lemma&ct=cover)
![](https://img-blog.csdn.net/20161102112219273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       移动计算的真正力量并不在于移动设备本身，而是在于能够利用来自其他来源的数据。
DB2 Everyplace不仅仅是一种移动计算基础设施。它是一个完整的环境，包含了构建、部署和支持强大的电子商务[应用程序](http://baike.baidu.com/view/330120.htm)所需的工具。DB2
 Everyplace 提供一个“指纹”引擎（大约200
 KB），其中包含所有的安全特性，比如表加密和提供高性能的高级索引技术。
DB2 Engine：
DB2 Engine是整个数据库系统的核心，提供了 DB2的基本功能。
 DB2 引擎类似汽车的发动机，负责管理和控制对数据的存取；负责生成程序包(存储存取计划的数据库对象)；提供事务的管理；保障数据的完整性和数据保护；提供应用程序并发控制。数据库引擎(DB2Engine)设计的完善与否，决定了数据库系统是否稳定和高效。
 DB2 Engine 是所有数据库中最强大的数据库引擎。
