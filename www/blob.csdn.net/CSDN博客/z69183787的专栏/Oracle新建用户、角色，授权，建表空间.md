# Oracle新建用户、角色，授权，建表空间 - z69183787的专栏 - CSDN博客
2013年12月27日 17:12:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1485
oracle[数据库](http://www.2cto.com/database/)的权限[系统](http://www.2cto.com/os/)分为系统权限与对象权限。系统权限(
 database system privilege )可以让用户执行特定的命令集。例如，create table权限允许用户创建表，grant any privilege 权限允许用户授予任何系统权限。对象权限( database object privilege )可以让用户能够对各个对象进行某些操作。例如delete权限允许用户删除表或视图的行，select权限允许用户通过select从表、视图、序列(sequences)或快照(snapshots)中查询信息。
　　每个oracle用户都有一个名字和口令,并拥有一些由其创建的表、视图和其他资源。oracle角色(role)就是一组权限(privilege)(或者是每个用户根据其状态和条件所需的访问类型)。用户可以给角色授予或赋予指定的权限，然后将角色赋给相应的用户。一个用户也可以直接给其他用户授权。
　　一、创建用户
　　oracle内部有两个建好的用户：system和sys。用户可直接登录到system用户以创建其他用户，因为system具有创建别 的用户的 权限。 在安装oracle时，用户或系统管理员首先可以为自己建立一个用户。例 如 ：
Sql代码 
create user user01 identified by u01;  
  　　该命令还可以用来设置其他权限，详细情况参见自学资料。要改变一个口令，可以使用alter user命令：
　<span style="white-space: pre;">　alter user user01 identified by usr01;</span>
<span style="white-space: pre;"><span style="white-space: normal;"> </span> </span>
　　现在user01的口令已由“u01”改为“usr01”。
　　除了alter user命令以外，用户还可以使用password命令。如果使用password命令，用户输入的新口令将不在屏幕上显示。有dba特权的用户可以通过password命令改变任何其他用户的口令;其他用户只能改变自己的口令。
　　当用户输入password命令时，系统将提示用户输入旧口令和新口令，如下所示：
　　password
　　changing password for user01
　　old password:
　　new password:
　　retype new password:
　　当成功地修改了口令时，用户会得到如下的反馈：
　　password changed
　　二 、删除用户
　　删除用户，可以使用drop user命令，如下所示：
　　drop user user01;
　　如果用户拥有对象，则不能直接删除，否则将返回一个错误值。指定关键字cascade，可删除用户所有的对象，然后再删除用户。下面的例子用来删除用户与其对象：
　　drop user user01 cascade;
　　三 、3种标准角色
　　qracle为了兼容以前的版本，提供了三种标准的角色(role)：connect、resource和dba。
　　1. connect role(连接角色)
　　临时用户，特别是那些不需要建表的用户，通常只赋予他们connectrole。connect是使用oracle的简单权限，这种权限只有在对其他用户的表有访问权时，包括select、insert、update和delete等，才会变得有意义。拥有connect role的用户还能够创建表、视图、序列(sequence)、簇(cluster)、同义词(synonym )、会话(session)和与其他数据库的链(link)。
　　2. resource role(资源角色)
　　更可靠和正式的数据库用户可以授予resource role。resource提供给用户另外的权限以创建他们自己的表、序列、过程(procedure)、触发器(trigger)、索引(index)和簇(cluster)。
　　3. dba role(数据库管理员角色)
　　dba role拥有所有的系统权限----包括无限制的空间限额和给其他用户授予各种权限的能力。system由dba用户拥有。下面介绍一些dba经常使用的典型权限。
　　(1)grant(授权)命令
　　下面对刚才创建的用户user01授权，命令如下：
　　grant connect, resource to user01;
　　(2)revoke(撤消)权限
　　已授予的权限可以撤消。例如撤消(1)中的授权，命令如下：
　　revoke connect, resource from user01;
　　一个具有dba角色的用户可以撤消任何别的用户甚至别的dba的connect、resource 和dba的其他权限。当然，这样是很危险的，因此，除非真正需要，dba权限不应随便授予那些不是很重要的一般用户。 撤消一个用户的所有权限，并不意味着从oracle中删除了这个用户， 也不会破坏用户创建的任何表;只是简单禁止其对这些表的访问。其他要访问这些表的用户可以象以前那样地访问这些表。
　　四、创建角色
　　除了前面讲到的三种系统角色----connect、resource和dba，用户还可以在oracle创建自己的role。用户创建的role可以由表或系统权限或两者的组合构成。为了创建role，用户必须具有create role系统权限。下面给出一个create role命令的实例：
　　create role student;
　　这条命令创建了一个名为student的role。
　　一旦创建了一个role，用户就可以给他授权。给role授权的grant命令的语法与对对用户的语法相同。在给role授权时，在grant命令的to子句中要使用role的名称，如下所示：
　　grant select on class to student;
　　现在，拥有student 角色的所有用户都具有对class 表的select权限。
　　五、删除角色
　　要删除角色，可以使用drop role命令，如下所示：
　　drop role student;
　　指定的role连同与之相关的权限将从数据库中全部删除。
    六、<span style="font-family: helvetica, tahoma, arial, sans-serif; font-size: 14px; line-height: 25px;">删除表的注意事项</span>
<span style="font-family: helvetica, tahoma, arial, sans-serif; font-size: 14px; line-height: 25px;">在删除一个表中的全部数据时，须使用</span>
Sql代码 
truncate table 表名  
因为用drop table，delete * from 表名时，tablespace表空间该表的占用空间并未释放，反复几次drop，delete操作后，该tablespace上百兆的空间就被耗光了。  
