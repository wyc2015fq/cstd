# 连接MySQL 提示错误"未能加载文件或程序集“MySql.Data, Version=5.1.4.0, Culture=neutral,........” - 三少GG - CSDN博客
2011年01月04日 14:41:00[三少GG](https://me.csdn.net/scut1135)阅读数：23636

[http://hi.baidu.com/culion/blog/item/535c81325a4c59f41a4cff37.html](http://hi.baidu.com/culion/blog/item/535c81325a4c59f41a4cff37.html)
不能连接MySQL的解决方法
2010-05-13 15:08
CodeSmith4.1.3版本连接MySQL 提示错误"未能加载文件或程序集“MySql.Data, Version=5.1.4.0, Culture=neutral,........”或它的某一个依赖项。找到的程序集清单定义与程序集引用不匹配。"
解决方法：
下载MySQL.Data.dll(5.1.4)程序集 ，地址是 [ftp.ntu.edu.tw/pub/MySQL/Downloads/Connector-Net/mysql-connector-net-5.1.4-noinstall.zip](http://ftp.ntu.edu.tw/pub/MySQL/Downloads/Connector-Net/mysql-connector-net-4.1.3-noinstall.zip)
将MySQL.Data.dll放在CodeSmith目录中Bin目录下，重新运行，问题解决！
注意：下载MySQL.Data.dll版本必须和的连接错误提示中的版本号对应，下载地址"5.1.4"改为你所需版本号即可
　提示"未能加载文件或程序集“MySql.Data, Version=5.0.7.0, Culture=neutral,PublicKeyToken=c5687fc88969c44d”或它的某一个依赖项。找到的程序集清单定义与程序集引用不匹配。 (异常来自HRESULT:0x80131040)" 
　　把MySQL.Data.dll,复制到CodeSmith下的Bin目录,再调用还是错误,百思不得其解, 
　　后来看MySQL.Data.dll文件版本是5.2.1.0,比错误提示的大,看来要下一个5.0.7.0的试试, 
　　下载地址是 http://ftp.ntu.edu.tw/pub/MySQL/Downloads/Connecto r-Net/mysql-connector-net-5.0.7-noinstall.zip , 
　　将其中的MySQL.Data.Dll文件重新复制到CodeSmith下的Bin目录,再配置测试,OK了. 
　　看来CodeSmith将数据库程序调用写死了,应该设置成配置文件,让用户自己定义版本,这样才更灵活. 
[http://www.cnblogs.com/68681395/archive/2010/09/26/1835971.html](http://www.cnblogs.com/68681395/archive/2010/09/26/1835971.html)
## [未能加载文件或程序集“Oracle.DataAccess, Version=2.111.7.0, Culture=neutral, PublicKeyToken=89b483f429c47342”或它的某一个依赖项。试图加载格式不正确的程序](http://www.cnblogs.com/68681395/archive/2010/09/26/1835971.html)
对于64位的操作系统：Oracle.DataAccess.dll iis的应用程序池的高级选项
![](http://pic002.cnblogs.com/images/2010/17860/2010092614091856.jpg)
这里要设置为true，上面的是32位的数据库,上次的出错，就是这个解决。但是当数据库为64位的时候：Oracle.DataAccess.dll，要换成当前数据库的这个文件，同时把那个地方设置为false
![](http://www.mekery.cn/wp-content/uploads/2010/07/oracle.png)最近在做ASP.Net开发中使用Oracle，其中用到了Oracle提供的数据库提供程序ODP.NET，具体用到了Oracle.DataAccess.dll这个组件。在实践中，碰到过几次都出现“未能加载文件或程序集”Oracle.DataAccess,
 Version=2.112.1.0 …””，如上图所示。在此，问题和解决方法总结如下：
1. 问题出现的可能原因
（1）引用出错：无法加载时，可能是Oracle.DataAccess.dll引用的路径不正确；
（2）版本不匹配：在开发中引用的Oracle.DataAccess.dll与Oracle中的版本不匹配。所谓版本不匹配，一方面是指版本号不匹配；另一方面是64位和32位不匹配，使用64位的Oracle时一定要使用相应64位的Oracle.DataAccess.dll。
2. 解决办法
（1）检查引用是否出错时很简单，就看引用路径中是否存在Oracle.DataAccess.dll这个文件；
（2）Oracle.DataAccess.dll 一般位于Oracle安装目录 “../product/11.2.0/dbhome_1/ODP.NET/bin/2.x/”中，在开发时使用与程序中连接Oracle对应的Dll即可。
