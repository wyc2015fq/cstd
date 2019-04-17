# Mysql 创建用户并授权 - 在思索中前行！ - CSDN博客





2014年06月14日 17:06:33[_Tham](https://me.csdn.net/txl16211)阅读数：898







设置MySQL用户账号，指定哪个用户可以连接服务器，从哪里连接，连接后能做什么。
MySQL管理员应该知道如何设置MySQL用户账号，指出哪个用户可以连接服务器，从哪里连接，连接后能做什么？




GRANT语句创建MySQL用户并指定其权限，而REVOKE语句删除权限
当你对一个用户发出一条GRANT语句时，在user表中为该用户创建一条记录。如果语句指定任何全局权限（管理权限或适用于所有数据库的权限），这些也记录在user表中。如果你指定数据库、表和列级权限，他们被分别记录在db、tables_priv和columns_priv表中。


用GRANT和REVOKE比直接修改授权表更容易些，然而，建议你阅读一下《MySQL安全性指南》。这些表异常重要，而且作为一名管理员，你应该理解它们如何超越GRANT和REVOKE语句的功能水平。





1创建用户并授权
GRANT语句的语法看上去像这样


GRANT privileges (columns)
ON what
TO user IDENTIFIED BY "password"
WITH GRANT OPTION
要使用该语句，你需要填写下列部分：

privileges
授予用户的权限，下表列出可用于GRANT语句的权限指定符：
    ALL意味着“所有权限”，UASGE意味着无权限，即创建用户，但不授予权限。

columns
权限运用的列，它是可选的，并且你只能设置列特定的权限。如果命令有多于一个列，应该用逗号分开它们。

what
权限运用的级别。权限可以是全局的（适用于所有数据库和所有表）、特定数据库（适用于一个数据库中的所有表）或特定表的。可以通过指定一个columns字句是权限是列特定的。

user
权限授予的用户，它由一个用户名和主机名组成。在MySQL中，你不仅指定谁能连接，还有从哪里连接。这允许你让两个同名用户从不同地方连接。MySQL让你区分他们，并彼此独立地赋予权限。

MySQL中的一个用户名就是你连接服务器时指定的用户名，该名字不必与你的Unix登录名或Windows名联系起来。缺省地，如果你不明确指定一个名字，客户程序将使用你的登录名作为MySQL用户名。这只是一个约定。你可以在授权表中将该名字改为nobody，然后以nobody连接执行需要超级用户权限的操作。

password
赋予用户的口令，它是可选的。如果你对新用户没有指定IDENTIFIEDBY子句，该用户不赋给口令（不安全）。对现有用户，任何你指定的口令将代替老口令。如果你不指定口令，老口令保持不变，当你用IDENTIFIEDBY时，口令字符串用改用口令的字面含义，GRANT将为你编码口令，不要象你用SET PASSWORD 那样使用password()函数。

WITH GRANT OPTION子句是可选的。如果你包含它，用户可以授予权限通过GRANT语句授权给其它用户。你可以用该子句给与其它用户授权的能力。

用户名、口令、数据库和表名在授权表记录中是大小写敏感的，主机名和列名不是。








一般地，你可以通过询问几个简单的问题来识别GRANT语句的种类：
谁能连接，从那儿连接？

用户应该有什么级别的权限，他们适用于什么？

用户应该允许管理权限吗？

下面就讨论一些例子。

1.1 谁能连接，从那儿连接？
你可以允许一个用户从特定的或一系列主机连接。有一个极端，如果你知道降职从一个主机连接，你可以将权限局限于单个主机：

GRANT ALL ON samp_db.* TO boris@localhost IDENTIFIED BY "ruby"
GRANT ALL ON samp_db.* TO fred@res.mars.com IDENTIFIED BY "quartz"
(samp_db.*意思是“samp_db数据库的所有表)另一个极端是，你可能有一个经常旅行并需要能从世界各地的主机连接的用户max。在这种情况下，你可以允许他无论从哪里连接：
**　　GRANT ALL ON samp_db.* TO max@%   IDENTIFIED  BY "diamond"**
“%”字符起通配符作用，与LIKE模式匹配的含义相同。在上述语句中，它意味着“任何主机”。所以max和max@%等价。这是建立用户最简单的方法，但也是最不安全的。



取其中，你可以允许一个用户从一个受限的主机集合访问。例如，要允许mary从snake.net域的任何主机连接，用一个%.snake.net主机指定符：
GRANT ALL ON samp_db.* TO mary@.snake.net IDENTIFIED BY "quartz";
如果你喜欢，用户标识符的主机部分可以用IP地址而不是一个主机名来给定。你可以指定一个IP地址或一个包含模式字符的地址，而且，从MySQL 3.23，你还可以指定具有指出用于网络号的位数的网络掩码的IP号：
GRANT ALL ON samp_db.* TO boris@192.168.128.3 IDENTIFIED BY "ruby"
GRANT ALL ON samp_db.* TO fred@192.168.128.% IDENTIFIED BY "quartz"
GRANT ALL ON samp_db.* TO rex@192.168.128.0/17 IDENTIFIED BY "ruby"
第一个例子指出用户能从其连接的特定主机，第二个指定对于C类子网192.168.128的IP模式，而第三条语句中，192.168.128.0/17指定一个17位网络号并匹配具有192.168.128头17位的IP地址

如果MySQL抱怨你指定的用户值，你可能需要使用引号（只将用户名和主机名部分分开加引号）。
GRANT ALL ON samp_db.president TO "my friend"@"boa.snake.net"

1.2 用户应该有什么级别的权限和它们应该适用于什么？
你可以授权不同级别的权限，全局权限是最强大的，因为它们适用于任何数据库。要使ethel成为可做任何事情的超级用户，包括能授权给其它用户，发出下列语句：

GRANT ALL  ON  *.*  TO  ethel@localhost   IDENTIFIED   BY   "coffee" WITH GRANT OPTION
ON子句中的*.*意味着“所有数据库、所有表”。从安全考虑，我们指定ethel只能从本地连接。限制一个超级用户可以连接的主机通常是明智的，因为它限制了试图破解口令的主机。
有些权限（FILE、PROCESS、RELOAD和SHUTDOWN）是管理权限并且只能用"ON*.*"全局权限指定符授权。如果你愿意，你可以授权这些权限，而不授权数据库权限。例如，下列语句设置一个flush用户，他只能发出flush语句。这可能在你需要执行诸如清空日志等的管理脚本中会有用：

GRANT  RELOAD  ON  *.*   TO flushl@localhost  IDENTIFIED  BY "flushpass"
一般地，你想授权管理权限，吝啬点，因为拥有它们的用户可以影响你的服务器的操作。
数据库级权限适用于一个特定数据库中的所有表，它们可通过使用ON db_name.*子句授予：
GRANT  ALL   ON samp_db   TO   bill@racer.snake.net   INDETIFIED BY "rock"
GRANT  SELECT ON  samp_db TO ro_user@% INDETIFIED BY "rock"
第一条语句向bill授权samp_db数据库中所有表的权限，第二条创建一个严格限制访问的用户ro_user（只读用户），只能访问samp_db数据库中的所有表，但只有读取，即用户只能发出SELECT语句。

你可以列出一系列同时授予的各个权限。例如，如果你想让用户能读取并能修改现有数据库的内容，但不能创建新表或删除表，如下授予这些权限：
GRANT SELECT,INSERT,DELETE,UPDATE ON samp_db TO bill@snake.net INDETIFIED BY "rock"
对于更精致的访问控制，你可以在各个表上授权，或甚至在表的每个列上。当你想向用户隐藏一个表的部分时，或你想让一个用户只能修改特定的列时，列特定权限非常有用。如：

GRANT SELECT ON samp_db.member TO bill@localhost INDETIFIED BY "rock"
GRANT UPDATE (expiration) ON samp_db. member TO bill@localhost
第一条语句授予对整个member表的读权限并设置了一个口令，第二条语句增加了UPDATE权限，当只对expiration列。没必要再指定口令，因为第一条语句已经指定了。
如果你想对多个列授予权限，指定一个用逗号分开的列表。例如，对assistant用户增加member表的地址字段的UPDATE权限，使用如下语句，新权限将加到用户已有的权限中：
GRANT UPDATE (street,city,state,zip) ON samp_db TO assistant@localhost

通常，你不想授予任何比用户确实需要的权限宽的权限。然而，当你想让用户能创建一个临时表以保存中间结果，但你又不想让他们在一个包含他们不应修改内容的数据库中这样做时，发生了要授予在一个数据库上的相对宽松的权限。你可以通过建立一个分开的数据库（如tmp）并授予开数据库上的所有权限来进行。例如，如果你想让来自mars.net域中主机的任何用户使用tmp数据库，你可以发出这样的GRANT语句：
**　GRANT ALL  ON  tmp.*   TO  ""@mars.net**

在你做完之后，用户可以创建并用tmp.tbl_name形式引用tmp中的表（在用户指定符中的""创建一个匿名用户，任何用户均匹配空白用户名）。

1.3 用户应该被允许管理权限吗？
你可以允许一个数据库的拥有者通过授予数据库上的所有拥有者权限来控制数据库的访问，在授权时，指定WITH GRANTOPTION。例如：如果你想让alicia能从big.corp.com域的任何主机连接并具有sales数据库中所有表的管理员权限，你可以用如下GRANT语句：
GRANT ALL ON sales.* TO alicia@%.big.corp.com INDETIFIED BY "applejuice" WITH GRANT OPTION

在效果上WITH GRANTOPTION子句允许你把访问授权的权利授予另一个用户。要注意，拥有GRANT权限的两个用户可以彼此授权。如果你只给予了第一个用户SELECT权限，而另一个用户有GRANT加上SELECT权限，那么第二个用户可以是第一个用户更“强大”。



2 撤权并删除用户
要取消一个用户的权限，使用REVOKE语句。REVOKE的语法非常类似于GRANT语句，除了TO用FROM取代并且没有INDETIFED BY和WITH GRANT OPTION子句：
**REVOKE privileges (columns) ON what FROM user**

user部分必须匹配原来GRANT语句的你想撤权的用户的user部分。privileges部分不需匹配，你可以用GRANT语句授权，然后用REVOKE语句只撤销部分权限。

REVOKE语句只删除权限，而不删除用户。即使你撤销了所有权限，在user表中的用户记录依然保留，这意味着用户仍然可以连接服务器。要完全删除一个用户，你必须用一条DELETE语句明确从user表中删除用户记录：
%mysql -u root mysql
mysql>DELETE FROM user
->WHERE User="user_name" and Host="host_name";
mysql>FLUSH PRIVILEGES;
**DELETE语句删除用户记录，而FLUSH语句告诉服务器重载授权表。（**当你使用GRANT和REVOKE语句时，表自动重载，而你直接修改授权表时不是










