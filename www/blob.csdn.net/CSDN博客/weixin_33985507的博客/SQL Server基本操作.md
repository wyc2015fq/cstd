# SQL Server基本操作 - weixin_33985507的博客 - CSDN博客
2017年04月08日 10:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
#### 启动SQL Server数据库服务
启动SQL Server数据库服务有以下三种方式。
**①后台启动服务**
  “我的电脑”，右键单击——>管理——>服务和应用程序——>双击“服务”，这里会显示系统中所有服务。找到SQL Server (MSSOLSERVER)这个服务，此时我们就可以选择启动此服务：
![4444657-b86697b1fe09d887.png](https://upload-images.jianshu.io/upload_images/4444657-b86697b1fe09d887.png)
**②SQL Server配置管理器启动服务**
  开始——>全部程序——>Microsoft SQL Server 2008——>SQL Server配置管理器。
![4444657-7916ac1232e990df.png](https://upload-images.jianshu.io/upload_images/4444657-7916ac1232e990df.png)
SQL Server服务——>右击SQL Server (MSSOLSERVER)启动服务
![4444657-3ace8af70c86f965.png](https://upload-images.jianshu.io/upload_images/4444657-3ace8af70c86f965.png)
**③在运行窗口中使用命令启动和停止服务**
  通过`net start 服务器名称`命令启动SQL Server数据库服务器。
```
C:\>net start MSSOLSERVER
```
#### 数据库的服务器机器名与实例名称
数据库实例就是后台进程和数据库文件的集合。**一个SQL Server服务器就是一个实例**。 Microsoft SQL Server支持在**同一台计算机**上同时运行多个 SQL Server 数据库引擎实例，每个 SQL Server 数据库引擎实例可以配置IP地址，相关的访问协议，端口等，并且各有一套不为其它实例共享的系统及用户数据库。
**实例的可访问性需要启动该实例对应的相关服务。**此处需要注意的是实例名和实例的服务名并不是相同的。缺省的实例的服务名为MSSQLSERVER，而命名实例的服务名为MSSQL$INSTANCE_NAME。
> 
在SQL Server服务器实例上，可以创建多个数据库，启动实例服务后，这些数据库就都可以使用了，这是与Oracle数据库的不同之处。
  一个Oracle Server由一个Oracle实例和一个Oracle数据库组成，也即是Oracle的每个数据库都对应操作系统上一个单独的服务，所以每个Oracle数据库在操作之前都需要单独启动。
SQL Server 实例有两种类型：
- (1)默认实例：默认实例**仅由运行该实例的计算机的名称唯一标识**，它没有单独的实例名。如果应用程序在请求连接 SQL Server 时只需要指定计算机名，则 SQL Server 客户端组件将尝试连接这台计算机上的数据库引擎默认实例。
  一台计算机上最多只能有一个默认实例，也可以没有默认实例。如果要访问本机上的默认SQL服务器实例，使用计算机名、(local)、localhost、127.0.0.1、本机IP地址，都可以达到相同的目的。如下图所示：
![4444657-377f895fef5f9283.png](https://upload-images.jianshu.io/upload_images/4444657-377f895fef5f9283.png)
  默认实例的服务名一般为MSSQLSERVER，可以到SQL Server配置管理器的服务中查看，如下图所示：
![4444657-5f23751e5016bc4f.png](https://upload-images.jianshu.io/upload_images/4444657-5f23751e5016bc4f.png)
- （2）命名实例：除默认实例外的所有实例即为命名实例。对于不同实例下的数据库的访问，使用<计算机名或ip地址\实例名称>即可实现访问，缺省实例使用<计算机名或ip地址>进行访问。
```
#查看数据库服务器的机器名称
1> select @@SERVERNAME
2> go
                                                                                
---------------------------------------------
DELL-PC
#查看数据库的实例名称，如果是默认实例名上述查询结果就会返回null
1>select SERVERPROPERTY('InstanceName')
```
#### 启动SQL Server客户端工具
SQL Server的客户端工具包括：
- sqlcmd，SQL Server的字符界面管理工具。
- SQL Server Management Studio(SSMS)，是SQL Server图形界面工具。
**Windows身份验证**
①启动SQL Server的sqlcmd并连接到服务器
  如果连接本地服务器的默认SQL Server实例，直接输入以下命令即可：
```
C:\>sqlcmd -E
或
C:\>sqlcmd
```
能够使用操作系统验证方式连接服务器的前提是当前登录Windows的账号已经被映射为SQL Server登录账号，这种方式与SQL*Plus中以sys用户的操作系统验证方式连接数据库相同，即sqlplus / as sysdba。
②SSMS连接到服务器
![4444657-377f895fef5f9283.png](https://upload-images.jianshu.io/upload_images/4444657-377f895fef5f9283.png)
**SQL Server身份验证**
①启动SQL Server的sqlcmd并连接到服务器
  如果要使用SQL Server验证方式连接服务器，可以使用-U参数指定服务器**登录账号**名称，使用-P参数指定口令，如服务器登陆账号名称为login3，口令为login3login3，则可以使用下面命令里连接到服务器。
```
C:\>sqlcmd -U login3 -P login3login3
```
如果要连接远程服务器，或本地服务器上的非默认实例，则要使用-S参数：
```
C:\>sqlcmd -S law\instance1 -U login3 -P login3login3
```
其中law是服务器的机器名称，instance1是非默认实例的实例名称。
  若省略-S参数，则默认连接本地服务器的默认实例，如果省略-E 或-U参数，则默认使用Windows验证方式连接服务器。
②SSMS连接到服务器
![4444657-ac708c13a16578ed.png](https://upload-images.jianshu.io/upload_images/4444657-ac708c13a16578ed.png)
  在安全性——>登录名中可以看到所有的登陆账号。
![4444657-e50adb8874e713fd.png](https://upload-images.jianshu.io/upload_images/4444657-e50adb8874e713fd.png)
  我们在这些登录名上单击右键，就可以查看并修改它们的属性。
![4444657-27eb249bc71ffcd5.png](https://upload-images.jianshu.io/upload_images/4444657-27eb249bc71ffcd5.png)
#### 数据库的切换和查看
SQL Server服务器实例上可以创建多个数据库，连接到服务器后，可以随时使用use命令切换数据库，**前提是当前服务器登陆账号已经映射为这多个数据库中的用户**，如下面命令所示：
```
C:\>sqlcmd
1> use DBForTest
2> go
已将数据库上下文更改为 'DBForTest'。
```
查看当前连接的数据库名称：
```
print db_name()
或
select db_name()
```
#### SQL执行与显示
1.设置客户端界面每行容纳的字符数
```
C:\>sqlcmd -w 300
```
2.修改执行过的SQL命令以重新执行
  可以使用上下方向键吧执行过的命令回显回来，然后使用左右方向键移动光标到合适位置来修改特定的内容。除此方法外，在sqlcmd中可以使用ed命令在编辑器中打开并修改执行过的上一条SQL命令。前提是要设置sqlcmdeditor环境变量的值为notepad。
```
C:\>set sqlcmdeditor = notepad
```
这个方法设置环境变量只能临时生效，如果要永久生效，则可以使用下面方法：右击“我的电脑”，在弹出菜单中选择“属性”。在属性对话框中选择“高级”，然后单击“环境变量”按钮，在“系统变量”部分单击“新建”按钮，在出现的对话框中的“变量名”中填入“sqlcmdeditoe”，在“变量值”部分填入“notepad”。
  完成以上设置后，重启cmd工具即可生效。
3.执行SQL脚本文件
  sqlcmd连接服务器和数据库后，使用:r执行脚本文件：
```
:r c:\test.sql
```
4.查询结果保存为文件
  可以在sqlcmd中使用:out命令指定输出结果保存的文件名称，从而避免把输出结果显示在当前窗口中。然后使用type命令查看输出文件中的结果。
```
1>:out c:\out.txt
1>select * from t
2>go
1>exit
C:\>type c:\out.txt
a      b
---- -----
1      1
```
#### 切换用户
与SQL*Plus相似，在sqlcmd中也可以使用connect命令切换用户，但是这里的connect关键字不能简写为conn，另外要使用-U及-P参数分别指定服务器**登陆账号**及其口令：
```
:connect DELL-PC -U login3 -P login2login2
```
其中DELL-PC是连接的数据库服务器的机器名称。
  如果使用操作系统验证，则不必附加账号名称及口令。
```
:connect DELL-PC
```
如果为默认实例，则可以如同示例只输入机器名称DELL-PC，如果是非默认实例，可以在机器名称后面附加实例名称，如DELL-PC\instance1。切换完成后，SQL Server自动关闭之前的用户连接。
  要查询当前登陆账号及数据库用户名称，则可以分别使用下面命令：
```
#查询当前登陆账号
print system_user
#查询当前数据库用户名称
print user
```
