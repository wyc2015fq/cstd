# sqlserver 2005的安装 - 战斗蜗牛的专栏 - CSDN博客





2011年05月04日 22:29:00[vbskj](https://me.csdn.net/vbskj)阅读数：1322标签：[sqlserver																[sql server																[server																[sql																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[SqlServer & MySql](https://blog.csdn.net/vbskj/article/category/817727)





1.先安装vs2005 ，再安装sql server

2.sql安装时，出现Com+目录的警告，网上很多解决方案，保证兼容性全部通过。

3.第一次安装失败，卸载sql，按照网上攻略删除对应的目录，以及相应的注册表键值

4.Develop版的sql 2005可以随意安装，Enterprise的只能装windows server 2003

5.将dev_cd1,拷贝到D盘新建一个server目录中，再新建tool目录，将cd2的内容拷贝至此。

6.安装cd1：

      进入setup目录



      SqlRun_SQL.msi  ->  SqlRun_RS.msi  -> SqlRun_NS.msi  ->  SqlRun_DTS.msi  ->  SqlRun_AS.msi



->  Sqlncli.msi  ->  msxml6.msi  ->SQLServer2005_BC.msi  ->  SqlSupport.msi  -> SqlWriter.msi

->  sqlxml4.msi





7.安装cd2：

       进入setup目录



       SqlRum_Tools.msi  ->  SqlServer2K5_BOL.msi](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlserver&t=blog)




