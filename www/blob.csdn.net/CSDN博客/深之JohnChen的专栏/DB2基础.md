# DB2基础 - 深之JohnChen的专栏 - CSDN博客

2006年11月10日 14:30:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6439


**DB2****常用的命令**

1．启动数据库
    db2start
2．停止数据库
    db2stop
3．连接数据库
    db2 connect to oyd user db2 using pwd

(注：oyd为数据库名)
4．读数据库管理程序配置
    db2 get dbm cfg
5．写数据库管理程序配置
    db2 update dbm cfg using 参数名参数值
6．读数据库的配置
    db2 connect to o_yd user db2 using pwd
    db2 get db cfg for o_yd
7．写数据库的配置
    db2 connect to o_yd user db2 using pwd
    db2 update db cfg for o_yd using 参数名参数值
8．关闭所有应用连接
    db2 force application all
    db2 force application ID1,ID2,,,Idn MODE ASYNC
    (db2 list application for db o_yd show detail)
9．备份数据库
    db2 force application all
    db2 backup db o_yd to d:
   (db2 initialize tape on //./tape0)
   (db2 rewind tape on //./tape0)
   db2 backup db o_yd to //./tape0
10．恢复数据库
   db2 restore db o_yd from d: to d: 
   db2 restore db o_yd from //./tape0 to d:
11．绑定存储过程
  db2 connect to o_yd user db2 using pwd
  db2 bind c:/dfplus.bnd
拷贝存储过程到服务器上的C:/sqllib/function目录中
12．整理表
   db2 connect to o_yd user db2 using pwd
   db2 reorg table ydd
   db2 runstats on table ydd with distribution and indexes all 

13．导出表数据
    db2 export to c:/dftz.txt of del select * from dftz
    db2 export to c:/dftz.ixf of ixf select * from dftz
14．导入表数据
import from c:/123.txt of del  insert into ylbx.czyxx
db2 import to c:/dftz.txt of del commitcount 5000 messages  c:/dftz.msg insert into dftz
db2 import to c:/dftz.ixf of ixf commitcount 5000 messages c:/dftz.msg insert into dftz
db2 import to c:/dftz.ixf of ixf commitcount 5000 insert into dftz
db2 import to c:/dftz.ixf of ixf commitcount 5000 insert_update into dftz
db2 import to c:/dftz.ixf of ixf commitcount 5000 replace into dftz
db2 import to c:/dftz.ixf of ixf commitcount 5000 create into dftz   (仅IXF)
db2 import to c:/dftz.ixf of ixf commitcount 5000 replace_create into dftz  (仅IXF) 

15．执行一个批处理文件
      db2 –tf 批处理文件名
（文件中每一条命令用；结束）
16．自动生成批处理文件
建文本文件：temp.sql
  select 'runstats on table DB2.' || tabname || ' with distribution and  detailed indexes all;' from syscat.tables where tabschema='DB2' and type='T';
db2 –tf temp.sql>runstats.sql
17．自动生成建表（视图）语句
在服务器上：C:/sqllib/misc目录中
db2 connect to o_yd user db2 using pwd
db2look –d o_yd –u db2 –e –p –c c:/o_yd.txt
18．其他命令
grant dbadm on database to user bb 

19select * from czyxx fetch first 1 rows only
20db2look –d ylbx –u db2admin –w –asd –a –e –o a.txt21. 显示当前用户所有表

list tables 
22.列出所有的系统表

list tables for system
23.查看表结构
db2 describe select * from user.tables 

**DB2****常用的****SQL****语句**

1、组合语句执行

BEGINATOMIC

表达式1 分号空格/回车

表达式2 分号空格/回车

END

2、应该限制访问权限的表（应该撤销这些表PUBLIC SELECT访问权）

SYSCAT.DBAUTH

SYSCAT.TABAUTH

SYSCAT.PACKAGEAUTH

SYSCAT.INDEXAUTH

SYSCAT.COLAUTH

SYSCAT.PASSTHRUAUTH

SYSCAT.SCHEMAAUTH

比较有用的目录表

SYSCAT.COLUMNS：包含每一行对应于表或视图中定义的列

SYSCAT.INDEXCOLUSE：包含每一行包含的所有列

SYSCAT.INDEXES：包含每一行对应于表或视图中定义的每个索引

SYSCAT.TABLES：所创建每个表，视图，别名对应其中一行

SYSCAT.VIEWS：所创建每个视图对应其中一行或几行

通过索引保持数据唯一性：CREATE UNIQUE INDEX INDEXNAME ON TABLE (COLUMN)

消除重复行：SELECT DISTINCT COLUMN FROM TABLE

3、DB2关于时间的一些函数

得到当前时间的年份、月份、天、小时等等：

YEAR (current timestamp)
MONTH (current timestamp)
DAY (current timestamp)
HOUR (current timestamp)
MINUTE (current timestamp)
SECOND (current timestamp)
MICROSECOND (current timestamp)

分别得到当时的日期和时间

DATE (current timestamp)
TIME (current timestamp)

关于时间的一些计算：

current date + 1 YEAR
current date + 3 YEARS + 2 MONTHS + 15 DAYS
current time + 5 HOURS - 3 MINUTES + 10 SECONDS

计算两个日期之间有多少天：

days (current date) - days (date(’1999-10-22′))

得到去除毫秒的当前时间:

CURRENT TIMESTAMP - MICROSECOND (current timestamp) MICROSECONDS

将时间转换成字符串：

char(current date)
char(current time)
char(current date + 12 hours)

将字符串转换成时间：

TIMESTAMP (’2002-10-20-12.00.00.000000′)
TIMESTAMP (’2002-10-20 12:00:00′)
DATE (’2002-10-20′)
DATE (’10/20/2002′)
TIME (’12:00:00′)
TIME (’12.00.00′)

注意：在DB2的命令编辑器中可以输入SQL语句和DB2中的内部命令。要想显示当前时间的话，不能直接输入current time，这个只能在SQL语言中引用，要想显示有下面方式：

1）VALUES （current time）

2）SELECTCURRENT TIME FROM SYSIBM.SYSDUMMY1

这个与SQL SERVER2000中不一样，在SQL SERVER2000中可以输入Getdate（）得到时间，既可以显示，也可以在语句SQL中用。

4、所有返回前N条数据的表达式

在SQL SERVER2000中使用TOP N格式

比如：SELECT TOP 10 CARDNO FROM CARD

在DB2中使用fetch first N rows only格式

比如：SELECT CARDNO FROM SEALCARD fetch first 10 rows only

5、函数使用

查看系统函数：SELECT * FROMSYSibm.sysfunctions;

比如：ABS（-89）可以作为值输入到SQL中，但是要想在命令编辑器中显示函数的结果的话可以用下列方式：

1）SELECT ABS(-89) FROM SYSIBM.SYSDUMMY1;

2）VALUES ABS(-89);

6、**存储过程**

在进行DB2存储过程开发时，我们可以利用很多DB2自带的工具，例如开发中心，控制中心等。但有时使用脚本能带给开发人员更大的灵活性和更高的工作效率。

在开始开发一个新的或修改一个已存在的存储过程时，我们通常会进行下面一些准备工作：

1.查看该存储过程会用到的表的表结构、字段类型、相关索引和示例数据。

2.查看相关的存储过程或用户自定义函数（UDF）的定义。

3.查找无效的存储过程并生成绑定语句。

4.如某个表发生了改变，查看依赖于该表的所有视图、存储过程和用户自定义函数（UDF）

虽然上述信息都可以通过DB2提供的开发工具和管理工具获得,但是通过脚本可以更快速的获得所需信息,并且可以重复执行。

使用脚本完成上述任务的关键是理解和使用DB2的系统表。我们先简单回顾一下有关的DB2的系统表和视图:

1.syscat.routines：存储所有的存储过程和用户自定义函数（UDF）信息。其中routinename字段为存储过程或用户自定义函数（UDF）的名称，routinetype字段表示该记录表示的是存储过程（P）还是用户自定义函数（F），lib_id字段为存储过程编译后生成的包序列号，origin字段表示该存储过程或用户自定义函数的出处（Q表示是由SQL组成的，E表示用户定义的且是外部的），valid字段表示该存储过程或用户自定义函数是否有效，如果origin字段不是Q的话，该域为空。

2.syscat.packages：存储所有被绑定的程序包。其中pkgname表示包名称，valid字段表示该包是否合法。

3.syscat.packagedep：存储关于程序包的依赖关系。其中pkgname字段表示程序包名，btype字段表示被依赖对象的类型，bname字段表示被依赖对象的名称。

4.syscat.routinedep：存储关于程序（routine）的依赖关系。其中routinename字段表示程序名，btype字段表示被依赖对象的类型，bname字段表示被依赖对象的名称。

5.syscat.viewdep：存储了关于视图的依赖关系。其中viewname字段表示视图名，btype字段表示被依赖对象的类型，bname字段表示被依赖对象的名称。

回顾并了解了上述系统表和视图，我们就可以很容易的创建脚本以完成前面提到的开发存储过程所要做的准备工作。

1．查看该表结构、字段类型、相关索引和示例数据

虽然我们可以查询sysibm.systables表获得表结构，但有一种更简便的方法来获取表结构，即使用db2look工具。该工具可以生成创建该表和相关的索引的DDL。如我们要获取指定数据库中指定表的结构和前20条数据作为参考，可编写脚本viewtbl.cmd如下，传入参数分别为数据库名和表名。

@echo ------------------ DDL of table %2 and related index(ex) ------------------

@db2look -d %1 -t %2 -e

@echo ------------------ fisrt 20 rows in table %2 ------------------

@db2 select * from %2 fetch first 20 rows only

2．查看已存在的存储过程和用户自定义函数（UDF）的定义，将结果存入文件并自动打开结果文件。

可以从syscat.routines表中做简单查询实现脚本viewrtn.cmd。

@db2 SELECT text FROM SYSCAT.ROUTINES WHERE ROUTINENAME=upper('%1') > %1.sql

@start %1.sql

3．查看所有无效的存储过程并生成绑定语句

删除存储过程引用的表会引起该存储过程无效。无效存储过程可以采用查询syscat.routines和syscat.packages的方法获得：

SELECT 

    RTRIM(r.routineschema) || '.' || RTRIM(r.routinename) AS spname , 

    RTRIM(r.routineschema) || '.' || 'P'||SUBSTR(CHAR(r.lib_id+10000000),2) AS pkgname

FROM 

    SYSCAT.routines r

WHERE

    r.routinetype = 'P' 

    AND (

        (r.origin = 'Q' AND r.valid != 'Y') 

        OR EXISTS (

            SELECT 1 FROM syscat.packages 

            WHERE pkgschema = r.routineschema 

            AND pkgname = 'P'||SUBSTR(CHAR(r.lib_id+10000000),2) 

            AND valid !='Y'

        )

    )

ORDER BY spname

注意要同时查询syscat.routines和syscat.packages表，因为在程序包无效时syscat.routines中的valid值仍可能为Y。

如果要自动生成重新绑定语句，只需将上述SQL改写并存成invalidSP.cmd：

@echo off

db2 "SELECT '@db2 rebind package '|| RTRIM(r.routineschema) || '.' || 'P'||SUBSTR(CHAR(r.lib_id+10000000),2)||' resolve any' FROM SYSCAT.routines r WHERE r.routinetype = 'P' AND ((r.origin = 'Q' AND r.valid != 'Y') OR EXISTS (SELECT 1 FROM syscat.packages WHERE pkgschema = r.routineschema AND pkgname = 'P'||SUBSTR(CHAR(r.lib_id+10000000),2) AND valid !='Y') )" >rebindsp.bat

4．查看某个表所依赖的视图、存储过程和用户自定义函数（UDF）

使用上述系统视图，我们很容易编写出脚本：

@echo off

echo --- dependent SPs ---

db2 "select proc.procschema, proc.procname from syscat.routines r, syscat.procedures proc, syscat.packagedep pdep where pdep.bname=upper('%2') and pdep.bschema=upper('%1') and r.specificname=proc.specificname AND pdep.pkgname = 'P'||SUBSTR(CHAR(r.lib_id+10000000),2)"

echo --- dependent UDF ---

db2 select routineschema, routinename from syscat.routinedep where bschema = upper('%1') and bname = upper('%2') and btype ='T' order by bname

echo --- dependent view ---

db2 select viewschema, viewname from syscat.viewdep where bschema = upper('%1') and bname = upper('%2') and btype ='T' order by bname

**行业借鉴经验：提高****DB2****存储过程性能和健壮性的****3****个最佳实践**

**最佳实践****1****：在创建存储过程语句中提供必要的参数**

创建存储过程语句（CREATE PROCEDURE）可以包含很多参数，虽然从语法角度讲它们不是必须的，但是在创建存储过程时提供这些参数可以提高执行效率。下面是一些常用的参数:

l容许SQL（allowed－SQL）

容许SQL（allowed－SQL）子句的值指定了存储过程是否会使用SQL语句，如果使用，其类型如何。它的可能值如下所示：

NO SQL：表示存储过程不能够执行任何SQL语句。

CONTAINS SQL：表示存储过程可以执行SQL语句，但不会读取SQL数据，也不会修改SQL数据。

READS SQL DATA：表示在存储过程中包含不会修改SQL数据的SQL语句。也就是说该储存过程只从数据库中读取数据。

MODIFIES SQL DATA：表示存储过程可以执行任何SQL语句。即可以对数据库中的数据进行增加、删除和修改。

如果没有明确声明allowed-SQL，其默认值是MODIFIES SQL DATA。不同类型的存储过程执行的效率是不同的，其中NO SQL效率最好，MODIFIES SQL DATA最差。如果存储过程只是读取数据，但是因为没有声明allowed-SQL类型，它会被当作对数据进行修改的存储过程来执行，这显然会降低程序的执行效率。因此创建存储过程时，应当明确声明其allowed-SQL类型。

l返回结果集个数（DYNAMIC RESULT SETS n）

存储过程能够返回0个或者多个结果集。为了从存储过程中返回结果集，需要执行如下步骤：

在CREATE PROCEDURE 语句的DYNAMIC RESULT SETS子句中声明存储过程将要返回的结果集的数量。如果这里声明的返回结果集的数量小于存储过程中实际返回的结果集数量，在执行该存储过程的时候，DB2会返回一个警告。

使用WITH RETURN子句，在存储过程体中声明游标。

为结果集打开游标。当存储过程返回的时候，保持游标打开。

在创建存储过程时指定返回结果集的个数可以帮助程序员验证存储过程是否返回了所期待数量的结果集，提高了程序的完整性。

**最佳实践****2****：对输入参数进行必要的的检查和预处理**

无论使用哪种编程语言，对输入参数的判断都是必须的。正确的参数验证是保证程序良好运行的前提。同样的，在DB2存储过程中对输入参数的验证和处理也是很重要的。正确的验证和预处理操作包括：

如果输入参数错误，存储过程应返回一个明确的值告诉客户应用，然后客户应用可以根据返回的值进行处理，或者向存储过程提交新的参数，或者去调用其他的程序。

根据业务逻辑，对输入参数作一定的预处理，如大小写的转换，NULL与空字符串或0的转换等。

在DB2储存过程开发中，如需要遇到对空(NULL)进行初始化，我们可以使用COALESCE函数。该函数返回第一个非NULL的参数。例如，COALESCE(piName,'')，如果变量piName为NULL，那么函数会返回''，否则就会返回piName本身的值。因此，可以用下面的代码检查piName是否非NULL并且非空字符串：

SET poGenStatus = 0;                                                    

SET piName   =RTRIM(COALESCE(piName, ''));

IF (piName ='')   

THEN

SET poGenStatus = 34100;      

RETURN poGenStatus;           

ENDIF;  

同理，使用COALESCE可以对任何类型的输入参数进行初始化或验证。下面是对参数初始化规则的一个总结：

1.输入参数为字符类型，且允许为NULL的，如希望缺省值为空字符串，可以使用COALESCE(inputParameter, '')把NULL转换成空字符串;

2.输入类型为整型，且允许为NULL的，如希望缺省值为0，可以使用COALESCE(inputParameter,0)，把NULL转换成0；

3.输入参数为字符类型，且不允许是NULL或空字符串的，可以使用RTRIM(COALESCE(inputParameter, ''))把NULL转换成空字符串，然后验证函数返回值是否为空字符串；

4.输入类型为整型，且不允许是NULL的，不需要使用COALESCE函数，直接使用IS NULL进行验证。

**最佳实践****3****：异常（****condition****）处理**

在存储过程执行的过程中，经常因为数据或者其他问题产生异常（condition）。根据业务逻辑，存储过程应该对异常进行相应处理或直接返回给调用者。此处暂且将condition译为异常以方便理解。实际上有些异常（condition）并非是由于错误引起的。

当存储过程中的语句返回的SQLSTATE值超过00000的时候，就表明在存储过程中产生了一个异常（condition），它表示出现了错误、数据没有找到或者出现了警告。为了处理存储过程中出现的异常，我们必须在存储过程体中声明异常处理器（condition handler），它可以决定存储过程怎样响应一个或者多个系统定义的异常或者自定义异常。

异常处理器类型(handler-type)有以下几种：

lCONTINUE: 在处理器操作完成之后，会继续执行产生这个异常语句之后的下一条语句。

lEXIT: 在处理器操作完成之后，存储过程会终止，并将控制返回给调用者。

lUNDO: 在处理器操作执行之前，DB2会回滚存储过程中执行过的SQL操作。在处理器操作完成之后，存储过程会终止，并将控制返回给调用者。

异常处理器可以处理基于特定SQLSTATE值的自定义异常，或者处理系统预定义异常。系统预定义的3种异常如下所示：

lNOT FOUND: 标识导致SQLCODE值为＋100或者SQLSATE值为02000的异常。这个异常通常在SELECT没有返回行的时候出现。

lSQLEXCEPTIOIN: 标识导致SQLCODE值为负的异常。

lSQLWARNING: 标识导致警告异常或者导致正100以上的SQLCODE值的异常。

如果产生了NOT FOUND 或者SQLWARNING异常，并且没有为这个异常定义异常处理器，系统就会忽略这个异常，并且将控制流转向下一个语句。如果产生了SQLEXCEPTION异常，并且没有为这个异常定义异常处理器，那么存储过程就会失败，系统会将控制流返回调用者。因此如果开发人员想改变这种异常处理流程，必须自定义异常处理器。例如，希望在出现SQLWARNING异常时也终止存储过程，并将名为stmt的变量设为“ABORTED”，可以定义下面语句定义异常处理器：

DECLAREEXIT HANDLER FOR SQLEXCEPTION, SQLWARNING

SET stmt ='ABORTED';

如果预定义异常集仍不能满足需求，可以为特定的SQLSTATE值声明自定义异常，然后再为这个异常定制异常声明处理器。为特定的SQLSTATE值声明自定义异常的语法如下：

DECLARE condition-name CONDITION FOR SQLSATE ‘mysqlstate’

定义了异常和异常处理器后，在存储过程执行的任何都使用SIGNAL condition-name语句触发这种自定义类型的异常。

异常处理器可以由单独的存储过程语句定义，也可以使用复合语句定义。注意在执行复合语句的时候，SQLSATE和SQLCODE的值会被改变，如果需要保留异常前的SQLSATE和SQLCODE值，就需要在复合语句中的第一个语句把SQLSATE和SQLCODE赋予本地变量或参数。下面是一个例子：

DECLARECONTINUE HANDLER FOR SQLEXCEPTION, SQLWARNING, NOTFOUND

BEGINNOT ATOMIC                  

-- Capture SQLCODE & SQLSTATE to local variables  

SELECTSQLCODE, SQLSTATE       

INTO    hSqlcode, hSqlstate     

FROM    SYSIBM.SYSDUMMY1;       

-- your handler statements below

……

END; 

**应用难点技巧：使用****Case****让你的****SQL****语句有条件的执行**

我们在编写SQL语句时，常常遇到希望SQL能够按条件执行的情况。这里的条件不是指Where子句中的条件，而是指让DB2根据条件执行SQL的语句块。大多数情况下我们可以使用case来实现。

例如，我们希望从员工表中查出员工的工资情况，如果小于20000，则标志为low，20000到50000间为middle，否则为high。一般大家会想到先取出工资数据然后在Java代码中做判断，但我们也可以在SQL中完成上述操作。如下例：

select empno, sex, salary,

case 

 when salary < 20000 then 'low'

 when salary >=20000 and salary <50000 then 'middle'

 else 'high'

end as salaryclass

from employee

类似的，如果想在SQL语句中把性别翻译成文字，也可以用case实现，注意这两条SQL语句使用了不同的case表达式写法：

select empno, 

case sex

 when 'M' then 'male'

 when 'F' then 'female'

 else 'invalid'

end,

salary

from employee

Case不但能够在Select子句中使用，在From子句和Where子句中同样可以使用。下面是在Where子句中使用的一个例子：

select empno, sex, age

from employee

where 

case sex

 when 'M' then 55

 when 'F' then 50

end > age

在From子句中使用的例子极为少见，下面是一个极端的例子。在合同表ctrct_list、客户表customer和订单表quote中都有合同号字段，但订单表中的合同号可能为空。要求查询出订单表订单id和合同号，如订单表中合同号为空的话，查出客户表中相应的合同号。

select distinct

    q.quote_id,

    case rtrim(coalesce(q.ctrct_num, ''))

        when '' then rtrim(coalesce(c.ctrct_num, ''))

        else rtrim(coalesce(q.ctrct_num, ''))

    end ctrct_num

from

    (quote q

    left outer join customer c

        on q.sold_to_cust_num = c.cust_num)

    inner join ctrct_list cl

        on cl.cust_num = q.sold_to_cust_num

        and cl.ctrct_num = (

                case rtrim(coalesce(q.ctrct_num, ''))

                when '' then c.ctrct_num

                else q.ctrct_num

                end

            )

有时让SQL语句有条件的执行也可以不使用case。下面是一个例子：

select *

from EMPLOYEE

WHERE 

((job='MANAGER') AND vMgrFlag=1)

or

((job='DESIGNER' or job='ANALYST') AND vTechFlag =1)

or

((job='CLERK' or job='OPERATOR') AND vOfficeFlag=1)

此SQL可以要求根据标志位的不同选择出不同类型的雇员。各个标志位在执行SQL前应提前设置好。这种方法可以在某些情况下将动态SQL改写为静态SQL，因此在编写存储过程时非常实用。但要指出的是，DB2的查询优化器不可能将这种SQL也优化得非常高效，因此在数据量比较大时可能会带来性能问题。开发人员需要在编写完成后使用实际数据测试，必要的话进行性能优化。

















IBM DB2 数据库几个应用上的小技巧

1. 查看本地节点目录

命令窗口中输入：db2 list node directory 

2. 编目一个TCP/IP节点

命令窗口：db2 catalog tcpip node remote server ostype 

3. 取消节点编目

db2 uncatalog node 

4. 查看系统数据库目录

db2 list database directory 

5. 查看本地数据库目录

db2 list database directory on <盘符> 

在本地数据库目录中有而系统数据库目录中没有的数据库不能访问，可以在控制中心中选中<数据库>右键单击选择添加，然后输入需要添加的数据库名称或者点击刷新按钮选择数据库，加入数据库后即可以访问。

6. 编目数据库

db2 catalog database as at node 

7. 取消数据库编目

db2 uncatalog database 

8. 测试远程数据库的连接

db2 connect to user using 

9. 任何用户均可通过设置Current Schema专用寄存器为特定的数据库连接设置默认模式，初始默认值为当前会话用户的权限ID。

set schema = 

可以由用户交互式的使用，也可在应用程序中使用，如果用Dynamicrules Bind选项绑定包，这个语句就没有作用。此语句不在事务控制之下。

10. 代码页的设置

在创建数据库时设置字符集

create database using codeset territory 

例：

create database dbtest using codeset IBM-437 territory US 

也可以设置整个数据库的代码页，在win2000/NT/xp中，在我的电脑-->属性-->高级-->环境变量中添加变量DB2CODEPAGE = ，例：DB2CODEPAGE = 437 或 DB2CODEPAGE = 1386。或者在IBM DB2命令窗口输入 db2set DB2CODEPAGE=1386，设置后需要重新启动DB2生效。

11. DB2低版本数据到高版本的迁移

先将低版本数据备份使用恢复功能导入高版本数据库，然后在命令窗口输入 db2 migrate database 。

12. 表名或模式中含有引号时访问表

命令窗口：db2 select * from "tabschema"."tabname" 

命令行处理器：db2=> select * from "tabschema"."tabname" 

13. 导出数据库的表结构生成DDL文件

命令窗口：db2look -d -e -c -o 

14. 执行脚本文件

命令窗口：db2 -tvf 

15. 代码页的转换

16. 获取当前DB2的版本

select * from sysibm.sysversions 

17. DB2表的字段的修改限制? 

只能修改VARCHAR2类型的并且只能增加不能减少

alter table alter column set data type varchar(SIZE) 

18. 如何查看表的结构? 

describe table 

or 

describe select * from . 

19. 如何快速清除一个大表? 

ALTER TABLE TABLE_NAME ACTIVE NOT LOGGED INITALLY WITH EMPTY TABLE 

20. 如何查看数据库的存储过程? 

SELECT * FROM SYSCAT.PROCEDURES 

21. 如何查看表的约束? 

SELECT * FROM SYSCAT.CHECKS WHERE TABNAME = 

22. 如何查看表的引用完整约束? 

SELECT * FROM SYSCAT.REFERENCES WHERE TABNAME = 

23. 如何知道BUFFERPOOLS状况? 

select * from SYSCAT.BUFFERPOOLS 

24. 如何在命令行下查看修改实例和数据库配置参数? 

查看实例配置参数: db2 get dbm cfg 

修改实例配置参数: db2 update dbm cfg using 参数名新值

查看数据库配置参数: db2 get db cfg for 

修改数据库配置参数: db2 update db cfg for using 参数名新值

25. 如何修改缓冲区? 

增加缓冲区: create bufferpool size [pagesize 4096] {[not] EXTENDED STORAGE} 

修改缓冲区: alter bufferpool size {[not] EXTENDED STORAGE} 

删除缓冲区: drop bufferpool 

如果缓冲区大小设置为 -1 表示缓冲池的页面数目由数据库配置参数buffpage决定。

注意: 数据库配置参数buffpage仅对缓冲区大小设置为 -1 的缓冲池起作用。

26. 多个字段时如何不通过使用select子句使用in/not in 

select * from tabschema.tabname where (colA, colB, colC) [not] in (values (valueA1, valueB1, valueC1), (valueA2, valueB2, valueC2), ...(valueAn, valueBn, valueCn)) 

27. 查看当前连接到数据库的应用

db2 list application [show detail] 

28. 如何确认DB2数据库的一致性

db2dart /DB 

/DB表示检查整个数据库的一致性

29. 测试SQL语句的性能

db2batch -d -f [-a userid/passwd] [-r ] 

-r 选项表示将查询结果输出到一个文件中。

30. 导出某个表的数据

export to 

如：导出用户表

export to c:user.ixf of ixf select * from user 

31. 导入数据

import from 

如：导入用户表。导入时可以直接建立新表。如果有该表存在也可以用INSERT 插入，或者用UPDATE更新

import from c:user.ixf of ixf [Create/Insert into / update] tablename

32.如何知道一个用户有多少表?
SELECT * FROM SYSIBM.SYSTABLES WHERE CREATOR='USER'
33.如何知道用户下的函数?
select * from IWH.USERFUNCTION
select * from sysibm.SYSFUNCTIONS
34.如何知道用户下的VIEW数?
select * from sysibm.sysviews WHERE CREATOR='USER'
35.如何知道当前DB2的版本?
select * from sysibm.sysvERSIONS
36.如何知道用户下的TRIGGER数?
select * from sysibm.SYSTRIGGERS WHERE SCHEMA='USER'
37.如何知道TABLESPACE的状况?
select * from sysibm.SYSTABLESPACES
38.如何知道SEQUENCE的状况?
select * from sysibm.SYSSEQUENCES
39.如何知道SCHEMA的状况?
select * from sysibm.SYSSCHEMATA
40.如何知道INDEX的状况?
select * from sysibm.SYSINDEXES
41.如何知道表的字段的状况?
select * from sysibm.SYSCOLUMNSWHERETBNAME='AAAA'
42.如何知道DB2的数据类型?
select * from sysibm.SYSDATATYPES
43.如何知道BUFFERPOOLS状况?
select * from sysibm.SYSBUFFERPOOLS 
44.如何查看数据库的包?
select * fromsys CAT.PACKAGES
45.如何查看数据库的存储过程?
SELECT * FROMSYSCAT.PROCEDURES

或者SELECT * FROMSYSibm.sysprocedures
46.如何查看表的约束?
SELECT * FROM SYSCAT.CHECKSWHERETABNAME='AAAA'
47.如何查看表的引用完整约束?
SELECT * FROM SYSCAT.REFERENCES WHERETABNAME='AAAA'

**DB2****应用经验**

//建立数据库DB2_GCB 
CREATE DATABASE DB2_GCB ON G: ALIAS DB2_GCB 
USING CODESET GBK TERRITORY CN COLLATE USING SYSTEM DFT_EXTENT_SZ 32 
//连接数据库
connect to sample1 user db2admin using 8301206 
//建立别名
create alias db2admin.tables for sysstat.tables; 
CREATE ALIAS DB2ADMIN.VIEWS FOR SYSCAT.VIEWS 
create alias db2admin.columns for syscat.columns; 
create alias guest.columns for syscat.columns; 
//建立表
create table zjt_tables as 
(select * from tables) definition only; 
create table zjt_views as 
(select * from views) definition only; 
//插入记录
insert into zjt_tables select * from tables; 
insert into zjt_views select * from views; 

//建立视图
create view V_zjt_tables as select tabschema,tabname from zjt_tables; 

//建立触发器
CREATE TRIGGER zjt_tables_del 
AFTER DELETE ON zjt_tables 
REFERENCING OLD AS O 
FOR EACH ROW MODE DB2SQL 
Insert into zjt_tables1 values(substr(o.tabschema,1,8),substr(o.tabname,1,10)) 

//建立唯一性索引
CREATE UNIQUE INDEX I_ztables_tabname 
ON zjt_tables(tabname); 

//查看表
select tabname from tables 
where tabname='ZJT_TABLES'; 

//查看列
select SUBSTR(COLNAME,1,20) as 列名,TYPENAME as 类型,LENGTH as 长度
from columns 
where tabname='ZJT_TABLES'; 

//查看表结构
db2 describe table user1.department 
db2 describe select * from user.tables 

//查看表的索引
db2 describe indexes for table user1.department 

//查看视图
select viewname from views 
where viewname='V_ZJT_TABLES'; 

//查看索引
select indname from indexes 
where indname='I_ZTABLES_TABNAME'; 

//查看存贮过程
SELECT SUBSTR(PROCSCHEMA,1,15),SUBSTR(PROCNAME,1,15) 
FROM SYSCAT.PROCEDURES; 

//类型转换(cast) 
ip datatype:varchar 
select cast(ip as integer)+50 from log_comm_failed 

//重新连接
connect reset 

//中断数据库连接
disconnect db2_gcb 

//view application 
LIST APPLICATION; 

//kill application 

FORCE APPLICATION(0); 

db2 force applications all (强迫所有应用程序从数据库断开) 

//lock table 

//独占
lock table test in exclusive mode 

//共享

lock table test in share mode 

//显示当前用户所有表
list tables 

//列出所有的系统表
list tables for system 

//显示当前活动数据库
list active databases 

//查看命令选项
list command options 

//系统数据库目录
LIST DATABASE DIRECTORY 

//表空间
list tablespaces 

//表空间容器
LIST TABLESPACE CONTAINERS FOR 
Example: LIST TABLESPACE CONTAINERS FOR 1 

//显示用户数据库的存取权限
GET AUTHORIZATIONS 

//启动实例
DB2START 

//停止实例
db2stop 

//表或视图特权
grant select,delete,insert,update on tables to user 
grant all on tables to user WITH GRANT OPTION 

//程序包特权
GRANT EXECUTE 
ON PACKAGE PACKAGE-name 
TO PUBLIC 

//模式特权
GRANT CREATEIN ON SCHEMA SCHEMA-name TO USER 

//数据库特权
grant connect,createtab,dbadm on database to user 

//索引特权
grant control on index index-name to user 

//信息帮助 (? XXXnnnnn ) 
例:? SQL30081 

//SQL 帮助(说明 SQL 语句的语法) 

help statement 

例如，help SELECT 

SQLSTATE 帮助(说明 SQL 的状态和类别代码) 

? sqlstate 或 ? class-code 

//更改与“管理服务器”相关的口令

db2admin setid username password 

//创建 SAMPLE 数据库

db2sampl 

db2sampl F:(指定安装盘) 

//使用操作系统命令

! dir 

//转换数据类型 (cast) 

SELECT EMPNO, CAST(RESUME AS VARCHAR(370)) 

FROM EMP_RESUME 

WHERE RESUME_FORMAT = 'ascii' 

//要运行 DB2 Java 存储过程或 UDF，还需要更新服务器上的 DB2 数据库管理程序配置，以包括在该机器上安装 JDK 的路径

db2 update dbm cfg using JDK11_PATH d:/sqllib/java/jdk 

TERMINATE 

update dbm cfg using SPM_NAME sample 

//检查 DB2 数据库管理程序配置

db2 get dbm cfg 

//检索具有特权的所有授权名

SELECT DISTINCT GRANTEE, GRANTEETYPE, 'DATABASE' FROM SYSCAT.DBAUTH 

UNION 

SELECT DISTINCT GRANTEE, GRANTEETYPE, 'TABLE ' FROM SYSCAT.TABAUTH 

UNION 

SELECT DISTINCT GRANTEE, GRANTEETYPE, 'PACKAGE ' FROM SYSCAT.PACKAGEAUTH 

UNION 

SELECT DISTINCT GRANTEE, GRANTEETYPE, 'INDEX ' FROM SYSCAT.INDEXAUTH 

UNION 

SELECT DISTINCT GRANTEE, GRANTEETYPE, 'COLUMN ' FROM SYSCAT.COLAUTH 

UNION 

SELECT DISTINCT GRANTEE, GRANTEETYPE, 'SCHEMA ' FROM SYSCAT.SCHEMAAUTH 

UNION 

SELECT DISTINCT GRANTEE, GRANTEETYPE, 'SERVER ' FROM SYSCAT.PASSTHRUAUTH 

ORDER BY GRANTEE, GRANTEETYPE, 3 

create table yhdab 

(id varchar(10), 

password varchar(10), 

ywlx varchar(10), 

kh varchar(10)); 

create table ywlbb 

(ywlbbh varchar(8), 

ywmc varchar(60)) 

//修改表结构

alter table yhdab ALTER kh SET DATA TYPE varchar(13); 

alter table yhdab ALTER ID SET DATA TYPE varchar(13); 

alter table lst_bsi alter bsi_money set data type int; 

insert into yhdab values 

('20000300001','123456','user01','20000300001'), 

('20000300002','123456','user02','20000300002'); 

//业务类型说明

insert into ywlbb values 

('user01','业务申请'), 

('user02','业务撤消'), 

('user03','费用查询'), 

('user04','费用自缴'), 

('user05','费用预存'), 

('user06','密码修改'), 

('user07','发票打印'), 

('gl01','改用户基本信息'), 

('gl02','更改支付信息'), 

('gl03','日统计功能'), 

('gl04','冲帐功能'), 

('gl05','对帐功能'), 

('gl06','计费功能'), 

('gl07','综合统计') 

说明目录视图

检查约束 SYSCAT.CHECKS 

列 SYSCAT.COLUMNS 

检查约束引用的列 SYSCAT.COLCHECKS 

关键字中使用的列 SYSCAT.KEYCOLUSE 

数据类型SYSCAT.DATATYPES 

函数参数或函数结果 SYSCAT.FUNCPARMS 

参考约束 SYSCAT.REFERENCES 

模式 SYSCAT.SCHEMATA 

表约束 SYSCAT.TABCONST 

表 SYSCAT.TABLES 

触发器 SYSCAT.TRIGGERS 

用户定义函数 SYSCAT.FUNCTIONS 

视图 SYSCAT.VIEWS 

二进制大对象 (BLOB) 字符串。

字符大对象 (CLOB) 字符串，它的字符序列可以是单字节字符或多字节字符，或这两者的组合。

双字节字符大对象 (DBCLOB) 字符串，它的字符序列是双字节字符。

第一种规范形式

表中的每一行和每一列均有一个值，永远不会是一组值。

第二种规范形式

不在关键字中的每一列提供取决于整个关键字的事实。

第三种规范形式

每个非关键字列提供与其他非关键字列无关并只取决于该关键字的事实。

第四种规范形式

没有行包含有关一个实体的两个或更多个独立的多值事实。

数据类型类型特性示例或范围

CHAR(15) 定长字符串最大长度为 254 'Sunny day ' 

VARCHAR(15) 变长字符最大长度为 4000 'Sunny day' 

SMALLINT 数字长度为 2 字节精度为 5 位范围为-32768 至 32767 

INTEGER 数字长度为 4 字节精度为 10 位范围为-2147483648 至 2147483647 

REAL 数字单精度浮点32 位近似值范围为 -3.402E+38 至-1.175E-37或 1.175E-37 至-3.402E+38或零

DOUBLE 数字双精度浮点64 位近似值范围为-1.79769E+308 至-2.225E-307或 2.225E-307 至 1.79769E+308 

或零

DECIMAL(5,2) 数字精度为 5小数位为 2 范围为 -10**31+1 至 10**31-1 

DATE 日期时间三部分值 1991-10-27 

TIME 日期时间三部分值 13.30.05 

TIMESTAMP 日期时间七部分值 1991-10-27-13.30.05.000000 

列函数

列函数对列中的一组值进行运算以得到单个结果值。下列就是一些列函数的示例。

AVG 
返回某一组中的值除

IBM DB2 教程http://study.qqcf.com/corps/74/list_1.htm

