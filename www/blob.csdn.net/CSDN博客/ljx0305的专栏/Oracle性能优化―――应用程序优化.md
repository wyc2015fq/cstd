# Oracle性能优化―――应用程序优化 - ljx0305的专栏 - CSDN博客
2008年10月02日 13:22:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：810
**一、****与DBA有关的问题 **
原因：（1）80％的oracle性能问题实际都与oracle的数据库配置无关。
（2）源于本人的一次不成功优化经历。
SQL语句处理过程
1、句法分析
最费时间、代价最高。包括语法检查、运行方案制定等方面。
2、执行
3、读取数据
**引入两个概念：**
oracle的优化策略有两个：基于规则的优化RBO和基于代价（基于成本）的优化CBO。
***CBO—cost based optimizer ***
***RBO—rule based optimizer ***
**1、****什么是RBO **
oracle 7.0版本之前，优化程序中没有关于SQL语句优化的灵活选择，用一组固定的内在规则来优化SQL语句，即RBO。举例：
规则1：用rowid选行。针对WHERE字句使用rowid的程序。（表重构或移植要有待代价）
规则8：用于组合索引访问。
规则9：用于单列索引访问。
规则15：全表扫描。
具体执行：从最底层开始上溯，直到有合适的规则。
专家认为：rbo的优化程序优先权列表极像C语言的运算符优先顺序列表。所有很死板，有时会有问题。
缺点：不灵活，要有目的的更改应用程序。
**2、****什么是CBO **
基于代价的优化对影响SQL语句的执行因素综合分析，得到代价最小方案。代价：I/O 和CPU消耗、完成时间等。（统计什么数据，如何统计，下面会讲到）
oracle7.0版开始出现，但是很不成熟。Oracle7.3版开始逐渐成熟起来。RBO要求不断优化程序，CBO会根据当前的统计信息自动优化、决定运行方案。所以CBO优于RBO
**3、****Oracle****优化程序的初始化参数设置**
参数:optimizer_mode(initSID.ora中设置) 缺省：CHOOSE 这是基于成本的优化所需的设置。
参数值：
ØCHOOSE ：混合使用基于规则和代价的优化器。取决于是否有统计信息。有基于代价，否则基于规则。
例如：***select /*+ CHOOSE */bkdh,bkmc from bkmlb where nian=2005; ***
ØRULE ：表示对所有的sql语句使用基于规则的优化器。
例如：***select /*+ CHOOSE*/bkdh,bkmc from bkmlb where nian=2005; ***
ØFIRST_ROW：表示使用基于代价的优化器，减少立即显示时间，减少资源消耗，以最短时间返回前n行数据。（n＝1，10，100，1000）。
例如：***select /*+ FIRST_ROWS(10)*/bkdh,bkmc from bkmlb where nian=2005; ***
ØALL_ROWS：表示对所有的SQL语句使用基于代价的优化器，减少总的相应时间。
例如：***select /*+ ALL_ROWS*/bkdh,bkmc from bkmlb where nian=2005; ***
后几种不建议使用。如果要用，在会话级设置
4、**CBO****使用控制**
l通过实例：INITSID.ORA
l通过会话 alter session
会话级更改的方式
***alter session set OPTIMIZER_MODE=FIRST_ROW ***
l通过语句 /* ＋choose */――――――――见上。
**5、****什么是提示(hint****技术) **
隐含技术（hints）：影响对基于代价的方法的选择和数据的不同访问路径的试验。Orac le执行自身的例程，将新的优化技术提供给用户，并通过使用隐含影响ORACLE的选择。
SQL中嵌入/*Hint*/是oracle优化程序要作的工作。
必须放在SQL语句右边，理想：放在SQL语句的第一列引用之前。
/*+hint */即要求，如果要求不合理，会被当成注视忽略。
**6、****统计信息介绍**
统计具体信息：每个表中数据行数目、主关键字列表中关键字分布、分配的和被表的数据行占用的数据块数等。
关于统计信息的几个表：***USER_TABLESUSER_INDEXES USER TAB COLUMNS***
**统计的方法：**
l**SQLPLUS****中收集**
***ANALYZE TABLE tablenameCOMPUTE STATISTICS for all indexes; ***
***ANALYZE TABLE table_nameCOMPUTE STATISTICS; ***
***ANALYZE index indexnameCOMPUTE STATISTICS; ***
***ANALYZE TABLE tablename estimate statistics sample 20 percent;(******样本估算) ***
自动生成统计脚本：
***select ‘analyze table’||’owner||’.’||’estimate statistics sample 20%;’ from all_tables where owner=’bkfx’; ***
SPOOL ANALYTAB.SQL 
Set pagesize 0
SELECT 'ANALYZE TABLE BKFX.'||TABLE_NAME||' COMPUTE STATISTICS;' FROM USER_TABLES; 
SPOOL OFF 
SPOOL ANALYZE.LOG 
@ANALYTAB.SQL 
SPOOL OFF 
EXIT 
使用crontab做成定时执行进程即可。
l**使用PL/SQL****过程收集**
(1)dbms_utility.analyze_schema过程
用法：execute dbms_utility.analyze_schema (参数，参数，参数，参数)
(2)dbms_ddl.analyze_object过程
用法：同上。
**7、****多长时间进行一次统计**
根据数据变化。变化的大，要经常统计。
**8、****如何确定当前运行的是哪个优化程序**
一般：OPTIMIZER_MODE为choose.
字典USER_TABLES中统计数据的存在决定是否使用CBO。
特别注意：在缺乏某个sql语句中的所有对象的统计信息时，采用RBO。如果生成了统计信息，执行CBO。生成了统计信息，除非重设OPTIMIZER_MODE,或被/*+hint*/覆盖，否则执行CBO
!!!!要对所有应用程序中的所有对象生成统计信息。
――――――――――――――――――――――――――――――――――（1次）
**9、****什么是AUTOTRACE **
AUTOTRACE自动为指定的用户的一个或多个语句生成EXPLAIN PLAN，set autotrace提供了在SQL使用explain plan的更自动化的方法。
用法：set autotrace【参数】
DBMS_UTILTITY程序包中的analyze_schema过程允许对整个模式进行统计计算。可以避免一个一个表的统计；
***SQL>execute dbms_utility.analyze_schema(******参数，参数)***
具体参数有用户名，统计方式等。
**10、****使用explain plan****和autotrace ****分析oracle****选择访问数据的访问路径**
  在SQLPLUS中给指定用户进行 set autotrace on/off
    以SCOTT用户为例： 
    SQL>CONNECT scott/tiger 
    connected.
    SQL>@$ORACLE_HOME/RDBMS/ADMIN/UTLXPLAN.SQL 
    Table created.
    SQL>CONNECT / AS SYSDBA 
    connected.
    SQL>@$ORACLE_HOME/SQLPLUS/ADMIN/PLUSTRCE.SQL 
    drop role plustrace;
    Role dropped.
    create role plustrace;
    Role created.
    .
    grant plustrace to dba with admin option;
    Grant succeeded.
    SQL>GRANT PLUSTRACE TO SCOTT; 
    Grant succeeded.
    SQL>CONNECT SCOTT/TIGER
    connected.        
    SQL>set autotrace on
    SQL>
    SQL>CONNECT scott/tiger 
    connected.
    SQL>@$ORACLE_HOME/RDBMS/ADMIN/UTLXPLAN.SQL 
    Table created.
    SQL>CONNECT / AS SYSDBA 
    connected.
    SQL>@$ORACLE_HOME/SQLPLUS/ADMIN/PLUSTRCE.SQL 
    drop role plustrace;
    Role dropped.
    create role plustrace;
    Role created.
    .
    grant plustrace to dba with admin option;
    Grant succeeded.
    SQL>GRANT PLUSTRACE TO SCOTT; 
    Grant succeeded.
    SQL>CONNECT SCOTT/TIGER
    connected.        
    SQL>set autotrace on
    SQL>
4、使用方法：
举例：
***select bkdh,bkmc from bkfx.bkmlb where nian=2005 and bkdh='1-1'; ***
BKDHBKMC
------- ------------------------------------------------------------
1-1人民日报
Elapsed: 00:00:08.88
Execution Plan
----------------------------------------------------------
0SELECT STATEMENT Optimizer=CHOOSE
10TABLE ACCESS (FULL) OF 'BKMLB'
SQL> select bkdh,bkmc from bkfx.bkmlb where nian='2005' and bkdh='1-1';
BKDHBKMC
------- ------------------------------------------------------------
1-1人民日报
Elapsed: 00:00:00.03
Execution Plan
----------------------------------------------------------
0SELECT STATEMENT Optimizer=CHOOSE
10TABLE ACCESS (BY INDEX ROWID) OF 'BKMLB'
21INDEX (UNIQUE SCAN) OF 'SYS_C0011781' (UNIQUE)
**11、****计划的阅读方法：**
以树状格式进行读取，通过递归进入最底层，然后再返回该树的父（第一）层。
举上例分析。

二、SQL使用技巧以及优化的方法
1、条件中使用的列的值类型变化，导致索引无效。
2、索引列前加函数，如upper等，导致索引无效。
3、被索引的列不能使用null字符。
4、除了where字句的硬编码值外，不要建立包含相同sql语句的应用程序。
错误：
***selectfirstname,lastname from emp where empno=’1234’ ***
***selectfirstname,lastname from emp where empno=’5678’ ***
***正确：***
***selectfirstname,lastname from emp where empno=:b1 ***
5、批量的insertupdatedelete 比循环一个一个的处理要快。
6、尽量不要在应用程序中编制相关子查询，有可能对系统性能产生负面影响，消耗大量的资源。
错误：
select * from empa where salary>(select avg(salary) from empb wherea.deptid=b.deptid)
正确：
***select * from emp a,(select b.deptid,avg(salary) as avg_salfrom emp b group by deptid) c ***
***where a.deptid=c.deptid and a.salary>c.avg_sal; ***
7、where 字句的条件中使用not exists 代替not in;
8、使用like运算符代替substr;like 使得索引有效，substr使得索引失效。
9、如果使用基于规则的优化，构造from字句时最小的表成为在表的列表定义的最后一个表。
10、不要为在FROM字句中没有所有表的所有连接条件的select语句建立where字句的条件。
11、避免使用select x from DUAL(会很快耗尽系统性能)
12、可以在适当的地方使用nvl函数。Nvl函数不需要知道该函数所作用的列的类型，所以可以大大减少不匹配的可能性。
13、对于带有如多or的非常复杂的查询考虑用union all 重写。
引用:http://blog.csdn.net/sunyx/archive/2005/11/10/526908.aspx
