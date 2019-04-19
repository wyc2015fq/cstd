# 简单几步实现用Web客户端来管理SqlServer2008数据库 - xqhrs232的专栏 - CSDN博客
2017年10月17日 14:03:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1332
原文地址::[http://blog.csdn.net/binyao02123202/article/details/6969565](http://blog.csdn.net/binyao02123202/article/details/6969565)
相关文章
1、SQLSERVER绿色版企业管理器----[http://download.csdn.net/download/comjn/1049225](http://download.csdn.net/download/comjn/1049225)
2、SQL Server 企业管理器 免安装版----[http://download.csdn.net/download/wqh891105/2969256](http://download.csdn.net/download/wqh891105/2969256)
3、WEB版的SQL Server 企业管理器----[http://download.csdn.net/download/ebonet/4015163](http://download.csdn.net/download/ebonet/4015163)
SQL Server 企业管理器 免安装版
作为一个.NET开发者每次安装Visual Studio的时候我总是选择安装附带的Sql Express数据库,这货比较小巧简单用来做开发最合适了，不过默认安装的Sql Express没有管理客户端（即我们所说的Management Studio）也没有开启SA用户登录，这不是很方便，所以以前在安装完Sql Express后我都是接着再安装一个Sql Server Management Studio Express作为管理器并先用集成认证方式登录进数据库再启用混合验证，以便在程序开发时可以在连接字符串中以"User=user1;
 password=xxx"的形式书写。
上面说的是以前的做法，但自从换到Mac下用虚拟机来开发后对虚拟机的文件使用就比较敏感了，不想再安装超过百M的Sql Server Management Studio Express了，想使用Web版本的管理器了，毕竟Web版本的管理器才几M，也能完成我平常所做的操作。下面是我安装Web版本的管理器步骤，有类似需求的朋友参考一下吧。
- 在CMD窗口中运行“sqlcmd -S .\SqlExpress” 登录到数据库，登录成功后启用sa并设置sa用户的密码，命令如下图
![](http://pic002.cnblogs.com/images/2011/18819/2011102923243652.png)
- 重启SqlExpress服务
- 下载 [SqlDmo](http://db.tt/BnTae6Kr)[SqlServerWebAdmin](http://db.tt/OCQ0D35S)
- 将SqlDmo解压并Copy到C:\Program Files\Microsoft SQL Server\80\Tools\Binn目录
- 在命令行中注册SqlDmo，命令为 regsvr32 "C:\Program Files\Microsoft SQL Server\80\Tools\Binn\sqldmo.dll"
- 运行regedit命令打开注册表，将HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10.SqlExpress\MSSQLServer\LoginMode的值由1改为2，即由Windows集成验证发为混合验证。
- 运行iismgr，将下载的SqlServerWebAdmin在IIS中新建一个虚拟目录。
- 所有操作完成，访问新建的虚拟目录就可以在Web中管理数据库了。
![](http://pic002.cnblogs.com/images/2011/18819/2011102923265352.png)
本文中所介绍的方法比直接安装Sql Server Management Studio Express要麻烦一下，不过所需要下载的文件体积小，占用的资源也少，我个人还是比较喜欢这种方式。
PS:上面给出的文件下载地址是DropBox的，所以有可能有的园友不能下载，不过可以从[这里](http://files.cnblogs.com/zhangronghua/sqldmo.rar)下载SqlDMO文件。另外本文中提到SqlServerWebAdmin来源于[WXWinter的博客](http://www.cnblogs.com/foundation/archive/2008/10/07/1305297.html)，各位可以从那里直接下载SqlServerWebAdmin。
SQL Server 企业管理器 免安装版
