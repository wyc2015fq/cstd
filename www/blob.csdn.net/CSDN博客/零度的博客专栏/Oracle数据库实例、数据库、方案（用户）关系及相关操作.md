# Oracle数据库实例、数据库、方案（用户）关系及相关操作 - 零度的博客专栏 - CSDN博客
2019年04月01日 10:44:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：44
[Oracle - 数据库的实例、表空间、用户、表之间关系](https://www.cnblogs.com/adforce/p/3312252.html)
完整的Oracle数据库通常由两部分组成：Oracle数据库和数据库实例。
1) 数据库是一系列物理文件的集合（数据文件，控制文件，联机日志，参数文件等）；
2) Oracle数据库实例则是一组Oracle后台进程/线程以及在服务器分配的共享内存区。
在启动Oracle数据库服务器时，实际上是在服务器的内存中创建一个Oracle实例（即在服务器内存中分配共享内存并创建相关的后台内存），然后由这个Oracle数据库实例来访问和控制磁盘中的数据文件。Oracle有一个很大的内存快，成为全局区（SGA）。
**一、数据库、表空间、数据文件**
**1、数据库**
数据库是数据集合。Oracle是一种数据库管理系统，是一种关系型的数据库管理系统。
通常情况了我们称的“数据库”，并不仅指物理的数据集合，他包含物理数据、数据库管理系统。也即物理数据、内存、操作系统进程的组合体。
我们在安装Oracle数据库时，会让我们选择安装启动数据库（即默认的全局数据库）如下图：![](https://images0.cnblogs.com/blog/244227/201309/10151004-8e46cbf52a3549e785b99b8e6f9d542b.jpg)
全局数据库名：就是一个数据库的标识，在安装时就要想好，以后一般不修改，修改起来也麻烦，因为数据库一旦安装，数据库名就写进了控制文件，数据库表，很多地方都会用到这个数据库名。
启动数据库：也叫全局数据库，是数据库系统的入口，它会内置一些高级权限的用户如SYS，SYSTEM等。我们用这些高级权限账号登陆就可以在数据库实例中创建表空间，用户，表了。
查询当前数据库名：
select name from v$database;
**2、数据库实例**
用Oracle官方描述：实例是访问Oracle数据库所需的一部分计算机内存和辅助处理后台进程，是由进程和这些进程所使用的内存(SGA)所构成一个集合。
其实就是用来访问和使用数据库的一块进程，它只存在于内存中。就像Java中new出来的实例对象一样。
我们访问Oracle都是访问一个实例，但这个实例如果关联了数据库文件，就是可以访问的，如果没有，就会得到实例不可用的错误。
实例名指的是用于响应某个数据库操作的数据库管理系统的名称。她同时也叫SID。实例名是由参数instance_name决定的。
查询当前数据库实例名：
select instance_name from v$instance;
数据库实例名(instance_name)用于对外部连接。在操作系统中要取得与数据库的联系，必须使用数据库实例名。比如我们作开发，要连接数据库，就得连接数据库实例名：
jdbc:oracle:thin:@localhost:**1521**:orcl（orcl就为数据库实例名）
一个数据库可以有多个实例，在作数据库服务集群的时候可以用到。
**3、表空间**
Oracle数据库是通过表空间来存储物理表的，一个数据库实例可以有N个表空间，一个表空间下可以有N张表。
有了数据库，就可以创建表空间。
表空间(tablespace)是数据库的逻辑划分，每个数据库至少有一个表空间（称作SYSTEM表空间）。为了便于管理和提高运行效率，可以使用一些附加表空间来划分用户和应用程序。例如：USER表空间供一般用户使用，RBS表空间供回滚段使用。一个表空间只能属于一个数据库。
创建表空间语法：
Create TableSpace 表空间名称  
DataFile          表空间数据文件路径  
Size              表空间初始大小  
Autoextend on
如1：创建临时表空间
CREATE TEMPORARY TABLESPACE DB_TEMP TEMPFILE '/opt/oracle/oradata/orcl/DB_TEMP.DBF'
SIZE 64M
AUTOEXTEND ON
NEXT 64M MAXSIZE UNLIMITED
EXTENT MANAGEMENT LOCAL;
如2：创建数据表空间
CREATE TABLESPACE DB_DATA
LOGGING
DATAFILE '/opt/oracle/oradata/orcl/DB_DATA.DBF'
SIZE 64M
AUTOEXTEND ON
NEXT 64M MAXSIZE UNLIMITED
EXTENT MANAGEMENT LOCAL;
查看已经创建好的表空间：
select default_tablespace, temporary_tablespace, d.username  from dba_users d
**4、用户**
Oracle数据库建好后，要想在数据库里建表，必须先为数据库建立用户，并为用户指定表空间。
上面我们建好了数据库和表空间，接下来建用户：
创建新用户：
CREATEUSER          用户名  
IDENTIFIED BY        密码  
DEFAULT TABLESPACE   表空间(默认USERS)  
TEMPORARY TABLESPACE 临时表空间(默认TEMP) 
如：
CREATEUSER utest  
IDENTIFIED BY utestpwd  
DEFAULT TABLESPACE db_data  
TEMPORARY TABLESPACE db_temp; 
有了用户，要想使用用户账号管理自己的表空间，还得给它分权限：
GRANT CONNECT TO utest;  
GRANT RESOURCE TO utest;  
GRANT dba TO utest;--dba为最高级权限，可以创建数据库，表等。
查看数据库用户：
select*from dba_users;
**5、表**
有了数据库，表空间和用户，就可以用自定义的用户在自己的表空间创建表了。有了表，我们可以开发了。
转载：http://yuwenlin.iteye.com/blog/1882988
