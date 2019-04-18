# Oracle\Sqlserver\Mysql\Sybase\DB2五类数据库通信协议 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年10月13日 16:05:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6807








                
 1．Oracle-tns-tcp1521（默认端口）协议

tns通信协议参考地址：

[http://wenku.baidu.com/view/0ba5df6925c52cc58bd6bedc.html](http://wenku.baidu.com/view/0ba5df6925c52cc58bd6bedc.html)

[http://wenku.baidu.com/view/2864c568011ca300a6c39052.html](http://wenku.baidu.com/view/2864c568011ca300a6c39052.html)



2．Mysql-mysql-tcp3306（默认端口）协议

mysql通信协议参考地址：

[http://forge.mysql.com/wiki/MySQL_Internals_ClientServer_Protocol](http://forge.mysql.com/wiki/MySQL_Internals_ClientServer_Protocol)



 3．Sqlserver-tds-tcp1433/udp1434（默认端口）协议

tds通信协议参考地址：

[http://freetds.cvs.sourceforge.net/viewvc/freetds/freetds/doc/tds.html](http://freetds.cvs.sourceforge.net/viewvc/freetds/freetds/doc/tds.html)

[http://www.docin.com/p-98157348.html](http://www.docin.com/p-98157348.html)  //tds协议分析



4．Sybase-tds-tcp5000（默认端口）协议

1）tds通信协议参考地址：

[http://www.eol.cn/20030826/3089816.shtml](http://www.eol.cn/20030826/3089816.shtml)

[http://www.cnblogs.com/yylqinghao/archive/2010/03/16/1687551.html](http://www.cnblogs.com/yylqinghao/archive/2010/03/16/1687551.html)

Sybase和Sqlserver都采用tds通信协议，不同的是不同数据库版本采用不同的协议版本。

2）tds文档地址：[http://download.csdn.net/detail/fjssharpsword/3680852](http://download.csdn.net/detail/fjssharpsword/3680852) ；



5．DB2-drda-tcp50000协议

DRDA（Distributed Relational Database Architecture ）分布式关系数据库体系结构，定义了客户机和后台数据库之间的接口协议。提供了IBM的DB2、DBM、SQL/DS和SQL/400数据库系统的互连框架。

1）主要参考地址：

[http://en.wikipedia.org/wiki/DRDA](http://en.wikipedia.org/wiki/DRDA)

[http://www.opengroup.org/dbiop/](http://www.opengroup.org/dbiop/)  //the DRDA v5 technical standard，需要注册才能下载

[http://www-01.ibm.com/common/ssi/cgi-bin/ssialias?infotype=an&subtype=ca&htmlfid=897/ENUS207-041](http://www-01.ibm.com/common/ssi/cgi-bin/ssialias?infotype=an&subtype=ca&htmlfid=897/ENUS207-041)  //ibm官方资料

其他开发者研究的资料：

[http://blog.donews.com/sunjita/archive/2007/07/09/1184729.aspx](http://blog.donews.com/sunjita/archive/2007/07/09/1184729.aspx)

[http://blog.csdn.net/moreorless/article/details/5988155](http://blog.csdn.net/moreorless/article/details/5988155)

[http://www.redbooks.ibm.com/abstracts/sg244249.html](http://www.redbooks.ibm.com/abstracts/sg244249.html)

2）文档地址： [http://download.csdn.net/detail/fjssharpsword/3680856](http://download.csdn.net/detail/fjssharpsword/3680856)；



 6．各类数据库协议可以部署数据产品后进行客户端和服务端通信，然后用wireshark解析验证协议结构。Internet包头结构解析可下载Internet中各种协议数据包格式：[http://download.csdn.net/detail/fjssharpsword/3680782](http://download.csdn.net/detail/fjssharpsword/3680782) 参考。





