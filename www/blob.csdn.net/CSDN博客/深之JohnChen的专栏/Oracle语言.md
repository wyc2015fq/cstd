# Oracle语言 - 深之JohnChen的专栏 - CSDN博客

2008年07月29日 21:13:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4335标签：[oracle																[constraints																[table																[语言																[数据库																[insert](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=constraints&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[数据库编程](https://blog.csdn.net/byxdaz/article/category/184468)


 一、Oracle语言

Oracle 数据库系统使用经验
**1.having 子句的用法**

 having 子句对 group by 子句所确定的行组进行控制 ,having 子句条件中只允许涉及常量 , 聚组函数或 group by 子句中的列 . 

**2. 外部联接 "+" 的用法**

外部联接 "+" 按其在 "=" 的左边或右边分左联接和右联接 . 若不带 "+" 运算符的表中的一个行不直接匹配于带 "+" 预算符的表中的任何行 , 则前者的行与后者中的一个空行相匹配并被返回 . 若二者均不带 '+', 则二者中无法匹配的均被返回 . 利用外部联接 "+", 可以替代效率十分低下的 not in 运算 , 大大提高运行速度 . 例如 , 下面这条命令执行起来很慢 

select a.empno from emp a where a.empno not in 

(select empno from emp1 where job='SALE'); 

倘若利用外部联接 , 改写命令如下 : 

select a.empno from emp a ,emp1 b 

where a.empno=b.empno(+) 

and b.empno is null 

and b.job='SALE'; 

可以发现 , 运行速度明显提高 . 

**3. 删除表内重复记录的方法**

可以利用这样的命令来删除表内重复记录 : 

delete from table_name a 

where rowid< (select max(rowid) from table_name 

where column1=a.column1 and column2=a.column2 

and colum3=a.colum3 and ...); 

**4.set transaction 命令的用法**

在执行大事务时 , 有时 oracle 会报出如下的错误 : 

ORA-01555:snapshot too old (rollback segment too small) 

这说明 oracle 给此事务随机分配的回滚段太小了 , 这时可以为它指定一个足够大的回滚段 , 以确保这个事务的成功执行 . 例如 

set transaction use rollback segment roll_abc; 

delete from table_name where ... 

commit; 

回滚段 roll_abc 被指定给这个 delete 事务 ,commit 命令则在事务结束之后取消了回滚段的指定 . 

**5. 使用索引的注意事项**

select,update,delete 语句中的子查询应当有规律地查找少于 20% 的表行 . 如果一个语句查找的行数超过总行数的 20%, 它将不能通过使用索引获得性能上的提高 . 

索引可能产生碎片 , 因为记录从表中删除时 , 相应也从表的索引中删除 . 表释放的空间可以再用 , 而索引释放的空间却不能再用 . 频繁进行删除操作的被索引的表 , 应当阶段性地重建索引 , 以避免在索引中造成空间碎片 , 影响性能 . 在许可的条件下 , 也可以阶段性地 truncate 表 ,truncate 命令删除表中所有记录 , 也删除索引碎片 . 

** 6. 数据库重建应注意的问题**

在利用 import 进行数据库重建过程中 , 有些视图可能会带来问题 , 因为结构输入的顺序可能造成视图的输入先于它低层次表的输入 , 这样建立视图就会失败 . 要解决这一问题 , 可采取分两步走的方法 : 首先输入结构 , 然后输入数据 . 命令举例如下 (uesrname:jfcl,password:hfjf,host sting:ora1, 数据文件 :expdata.dmp): 

imp jfcl/hfjf@ora1 file=empdata.dmp rows=N 

imp jfcl/hfjf@ora1 file=empdata.dmp full=Y buffer=64000 

commit=Y ignore=Y 

第一条命令输入所有数据库结构 , 但无记录 . 第二次输入结构和数据 ,64000 字节提交一次 .ignore=Y 选项保证第二次输入既使对象存在的情况下也能成功 . 

Oracle数据库的参数绑定性能实践

从Oracle的SGA的构成来看，它是推崇使用参数绑定的。使用参数绑定可以有效的使用Share Pool，对已经缓存的SQL不用再硬解析，能明显的提高性能。 

具体实践如下： 

SQL>create table test (a number(10));< 
再创建一个存储过程： 

create or replace procedure p_test is
  i number(10);
begin
  i := 0;
   while i <= 100000 loop
    execute immediate ' insert into test values (' || to_char(i) || ')';
    i := i + 1;
  end loop;

  commit;

end p_test; 
先测试没有使用参数绑定的： 

运行 p_test 后，用时91.111秒。 

再创建一个使用参数绑定的： 

create or replace procedure p_test is
  i number(10);
begin
  i := 0;
  while i <= 100000 loop
    execute immediate ' insert into test values (:a)'
      using i;
    i := i + 1;
  end loop;
  commit;

end p_test; 
运行 p_test 后，用时55.099秒。 

从上面的运行时间可以看出，两者性相差 39.525％，可见，用不用参数绑定在性能上相差是比较大的。 

Oracle PL/SQL语言入门

**一、背景介绍**
结构化查询语言(Structured Query Language，简称SQL)是用来访问关系型数据库一种通用语言，属于第四代语言（4GL），其执行特点是非过程化，即不用指明执行的具体方法和途径，而是简单地调用相应语句来直接取得结果即可。显然，这种不关注任何实现细节的语言对于开发者来说有着极大的便利。然而，有些复杂的业务流程要求相应的程序来描述，这种情况下4GL就有些无能为力了。PL/SQL的出现正是为了解决这一问题，PL/SQL是一种过程化语言，属于第三代语言，它与C、 C++、Java等语言一样关注于处理细节，可以用来实现比较复杂的业务逻辑。本文主要介绍PL/SQL的编程基础，以使入门者对PL/SQL语言有一个总体认识和基本把握。 
**二、编程基础知识**
1. 程序结构 
PL/SQL程序都是以块（block）为基本单位，整个PL/SQL块分三部分：声明部分（用declare开头）、执行部分（以 begin开头）和异常处理部分（以exception开头）。其中执行部分是必须的，其他两个部分可选。无论PL/SQL程序段的代码量有多大，其基本结构就是由这三部分组成。如下所示为一段完整的PL/SQL块： 

/*声明部分，以declare开头*/
declare v_id integer;
v_name varchar(20);
cursor c_emp is select * from employee where emp_id=3; 
/*执行部分，以begin开头*/
begin 　open c_emp;　//打开游标　
loop
fetch c_emp into v_id,v_name;　    　//从游标取数据
exit when c_emp%notfound ;　
end loop ;
close c_emp;　            　 //关闭游标
dbms_output.PUT_LINE(v_name); 
/*异常处理部分，以exception开始*/
exception
when no_data_found then
dbms_output.PUT_LINE('没有数据');
end ;
2. 控制结构 
PL/SQL程序段中有三种程序结构：条件结构、循环结构和顺序结构。 
1) 条件结构 
与其它语言完全类似，语法结构如下： 
if condition then
statement1
else
statement2
end if ;
2) 循环结构 
这一结构与其他语言不太一样，在PL/SQL程序中有三种循环结构： 
a. loop … end loop;
b. while condition loop … end loop;
c. for variable in low_bound . . upper_bound loop … end loop;
其中的“…”代表循环体。 
3) 顺序结构 
实际就是goto的运用，不过从程序控制的角度来看，尽量少用goto可以使得程序结构更加的清晰。 
3. 变量声明与赋值 
PL/SQL主要用于数据库编程，所以其所有的数据类型跟Oracle数据库里的字段类型是一一对应的，大体分为数字型、布尔型、字符型和日期型。这里简单介绍两种常用数据类型：number、varchar2。 
number 
用来存储整数和浮点数。范围为１e130～10e125，其使用语法为： 
number[(precision, scale)] 
其中(precision, scale)是可选的，precision表示所有数字的个数，scale表示小数点右边数字的个数。 
varchar2 
用来存储变长的字符串，其使用语法为： 
varchar2[(size)] 
其中size为可选，表示该字符串所能存储的最大长度。 
在PL/SQL中声明变量与其他语言不太一样，它采用从右往左的方式声明，比如声明一个number类型的变量v_id，那其形式应为： 
v_id number; 
如果给上面的v_id变量赋值，不能用“=”，应该用“:=”,即形式为： 
v_id :=5; 
4. SQL基本命令 
PL/SQL使用的数据库操作语言还是基于SQL的，所以熟悉SQL是进行PL/SQL编程的基础。SQL语言的分类情况大致如下： 
1) 数据定义语言(DDL)：Create，Drop，Grant，Revoke，… 
2) 数据操纵语言(DML)：Update，Insert，Delete，… 
3) 数据控制语言(DCL)：Commit，Rollback，Savapoint，… 
4) 其他：Alter System，Connect，Allocate， … 
具体的语法结构可以参阅其他关于SQL语言的资料，这里不再赘述。 
**三、过程与函数**
PL/SQL中的过程和函数与其他语言的过程和函数一样，都是为了执行一定的任务而组合在一起的语句。过程无返回值，函数有返回值。其语法结构为： 
过程：Create or replace procedure procname(参数列表) as PL/SQL语句块 
函数：Create or replace function funcname(参数列表) return 返回值 as PL/SQL语句块 
为便于理解，举例如下： 
问题：假设有一张表t1，有f1和f2两个字段，f1为number类型，f2为varchar2类型，要往t1里写两条记录，内容自定。 
Create or replace procedure test_procedure as 
V_f11 number :=1; /*声明变量并赋初值*/
V_f12 number :=2;
V_f21 varchar2(20) :='first';
V_f22 varchar2(20) :='second';
Begin 
Insert into t1 values (V_f11, V_f21);
Insert into t1 values (V_f12, V_f22);
End test_procedure; /*test_procedure可以省略*/
至此，test_procedure存储过程已经完成，经过编译后就可以在其他PL/SQL块或者过程中调用了。函数与过程具有很大的相似性，此处不再详述。 
**四、游标**
游标的定义为：用游标来指代一个DML SQL操作返回的结果集。即当一个对数据库的查询操作返回一组结果集时，用游标来标注这组结果集，以后通过对游标的操作来获取结果集中的数据信息。这里特别提出游标的概念，是因为它在PL/SQL的编程中非常的重要。定义游标的语法结构如下： 
cursor cursor_name is SQL语句; 
在本文第一段代码中有一句话如下： 
cursor c_emp is select * from employee where emp_id=3; 
其含义是定义一个游标c_emp，代表employee表中所有emp_id字段为３的结果集。当需要操作该结果集时，必须完成三步：打开游标、使用fetch语句将游标里的数据取出、关闭游标。请参照本文第一段代码的注释理解游标操作的三步骤。 
**五、其他概念**
PL/SQL中包的概念很重要，主要是对一组功能相近的过程和函数进行封装，类似于面向对象中的名字空间的概念。 
触发器是一种特殊的存储过程，其调用者比较特殊，是当发生特定的事件才被调用，主要用于多表之间的消息通知。 

Oracle系统表查询

数据字典dict总是属于Oracle用户sys的。 
1、用户： 
　select username from dba_users; 
改口令 
　alter user spgroup identified by spgtest; 
2、表空间： 
　select * from dba_data_files; 
　select * from dba_tablespaces;//表空间 
　select tablespace_name,sum(bytes), sum(blocks) 
from dba_free_space group by tablespace_name;//空闲表空间 
　select * from dba_data_files 
where tablespace_name='RBS';//表空间对应的数据文件 
　select * from dba_segments 
where tablespace_name='INDEXS'; 
3、数据库对象： 
　select * from dba_objects; 
　CLUSTER、DATABASE LINK、FUNCTION、INDEX、LIBRARY、PACKAGE、PACKAGE BODY、 
　PROCEDURE、SEQUENCE、SYNONYM、TABLE、TRIGGER、TYPE、UNDEFINED、VIEW。 
4、表： 
　select * from dba_tables; 
　analyze my_table compute statistics;->dba_tables后6列 
　select extent_id,bytes from dba_extents 
　where segment_name='CUSTOMERS' and segment_type='TABLE' 
　order by extent_id;//表使用的extent的信息。segment_type='ROLLBACK'查看回滚段的空间分配信息 
　列信息： 
select distinct table_name 
from user_tab_columns 
where column_name='SO_TYPE_ID'; 
5、索引：　 
　select * from dba_indexes;//索引，包括主键索引 
　select * from dba_ind_columns;//索引列 
　select i.index_name,i.uniqueness,c.column_name 
from user_indexes i,user_ind_columns c 
　where i.index_name=c.index_name 
　and i.table_name ='ACC_NBR';//联接使用 
6、序列： 
　select * from dba_sequences; 
7、视图： 
　select * from dba_views; 
　select * from all_views; 
text 可用于查询视图生成的脚本 
8、聚簇： 
　select * from dba_clusters; 
9、快照： 
　select * from dba_snapshots; 
快照、分区应存在相应的表空间。 
10、同义词： 
　select * from dba_synonyms 
where table_owner='SPGROUP'; 
//if owner is PUBLIC,then the synonyms is a public synonym. 
　if owner is one of users,then the synonyms is a private synonym. 
11、数据库链： 
　select * from dba_db_links; 
在spbase下建数据库链 
　create database link dbl_spnew 
　connect to spnew identified by spnew using 'jhhx'; 
　insert into acc_nbr@dbl_spnew 
　select * from acc_nbr where nxx_nbr='237' and line_nbr='8888'; 
12、触发器： 
　select * from dba_trigers; 
存储过程，函数从dba_objects查找。 
其文本：select text from user_source where name='BOOK_SP_EXAMPLE'; 
建立出错：select * from user_errors; 
oracle总是将存储过程，函数等软件放在SYSTEM表空间。 
13、约束： 
（1）约束是和表关联的，可在create table或alter table table_name add/drop/modify来建立、修改、删除约束。 
可以临时禁止约束，如： 
　alter table book_example 
　disable constraint book_example_1; 
　alter table book_example 
　enable constraint book_example_1; 
（2）主键和外键被称为表约束，而not null和unique之类的约束被称为列约束。通常将主键和外键作为单独的命名约束放在字段列表下面，而列约束可放在列定义的同一行，这样更具有可读性。 
（3）列约束可从表定义看出，即describe;表约束即主键和外键，可从dba_constraints和dba_cons_columns 查。 
　select * from user_constraints 
　where table_name='BOOK_EXAMPLE'; 
　select owner,CONSTRAINT_NAME,TABLE_NAME 
from user_constraints 
where constraint_type='R' 
order by table_name; 
（4）定义约束可以无名（系统自动生成约束名）和自己定义约束名（特别是主键、外键） 
如：create table book_example 
(identifier number not null); 
create table book_example 
(identifier number constranit book_example_1 not null); 
14、回滚段： 
在所有的修改结果存入磁盘前，回滚段中保持恢复该事务所需的全部信息，必须以数据库发生的事务来相应确定其大小（DML语句才可回滚，create,drop,truncate等DDL不能回滚）。 
回滚段数量=并发事务/4，但不能超过50；使每个回滚段大小足够处理一个完整的事务； 
　create rollback segment r05 
　tablespace rbs; 
　create rollback segment rbs_cvt 
　tablespace rbs 
　storage(initial 1M next 500k); 
使回滚段在线 
　alter rollback segment r04 online; 
用dba_extents,v$rollback_segs监测回滚段的大小和动态增长。 
回滚段的区间信息 
　select * from dba_extents 
　where segment_type='ROLLBACK' and segment_name='RB1'; 
回滚段的段信息,其中bytes显示目前回滚段的字节数 
　select * from dba_segments 
where segment_type='ROLLBACK' and segment_name='RB1'; 
为事物指定回归段 
　set transaction use rollback segment rbs_cvt 
针对bytes可以使用回滚段回缩。 
　alter rollback segment rbs_cvt shrink; 
　select bytes,extents,max_extents from dba_segments 
where segment_type='ROLLBACK' and segment_name='RBS_CVT'; 
回滚段的当前状态信息： 
　select * from dba_rollback_segs 
where segment_name='RB1'; 
比多回滚段状态status，回滚段所属实例instance_num 
查优化值optimal 
　select n.name,s.optsize 
from v$rollname n,v$rollstat s 
　where n.usn=s.usn; 
回滚段中的数据 
　set transaction use rollback segment rb1;/*回滚段名*/ 
　select n.name,s.writes 
from v$rollname n,v$rollstat s 
　where n.usn=s.usn; 
当事务处理完毕，再次查询$rollstat，比较writes(回滚段条目字节数)差值，可确定事务的大小。 
查询回滚段中的事务 
　column rr heading 'RB Segment' format a18 
　column us heading 'Username' format a15 
　column os heading 'Os User' format a10 
　column te heading 'Terminal' format a10 
　select r.name rr,nvl(s.username,'no transaction') us,s.osuser os,s.terminal te 
from v$lock l,v$session s,v$rollname r 
　where l.sid=s.sid(+) 
　and trunc(l.id1/65536)=R.USN 
　and l.type='TX' 
　and l.lmode=6 
　order by r.name; 
15、作业 
查询作业信息 
　select job,broken,next_date,interval,what from user_jobs; 
　select job,broken,next_date,interval,what from dba_jobs; 
查询正在运行的作业 
　select * from dba_jobs_running; 
使用包exec dbms_job.submit(:v_num,'a;',sysdate,'sysdate + (10/(24*60*60))')加入作业。间隔10秒钟 
exec dbms_job.submit(:v_num,'a;',sysdate,'sysdate + (11/(24*60))')加入作业。间隔11分钟使用包exec dbms_job.remove(21)删除21号作业。 

Oracle 9i取得建表和索引DDL语句

我们都知道在9i之前，要想获得建表和索引的语句是一件很麻烦的事。我们通常的做法都是通过export with rows=no来得到，但它的输出因为格式的问题并不能直接拿来用。而另一种方法就是写复杂的脚本来查询数据字典，但这对于一稍微复杂的对象，如IOT和嵌套表等，还是无法查到。 

从数据字典中获得DDL语句是经常要用的，特别是在系统升级/重建的时候。在Oracle 9i中，我们可以直接通过执行dbms_metadata从数据字典中查处DDL语句。使用这个功能强大的工具，我们可以获得单个对象或整个SCHEMA的DDL语句。最好不过的是因为它使用起来很简单。 

**1、获得单个表和索引DDL语句的方法：**

set heading off;　
set echo off;
Set pages 999;　
set long 90000;　
spool get_single.sql　
select dbms_metadata.get_ddl("TABLE","SZT_PQSO2","SHQSYS") from dual;
select dbms_metadata.get_ddl("INDEX","INDXX_PQZJYW","SHQSYS") from dual;
spool off;

下面是输出。我们只要把建表/索引语句取出来在后面加个分号就可以直接运行了。

SQL> select dbms_metadata.get_ddl("TABLE","SZT_PQSO2","SHQSYS") from dual;

CREATE TABLE "SHQSYS"."SZT_PQSO2"　
( "PQBH" VARCHAR2(32) NOT NULL ENABLE,
"ZJYW" NUMBER(10,0),
"CGSO" NUMBER(10,0) NOT NULL ENABLE,
"SOLS" VARCHAR2(17),
"SORQ" VARCHAR2(8),
"SOWR" VARCHAR2(8),
"SOCL" VARCHAR2(6),
"YWHM" VARCHAR2(10),
"YWLX" VARCHAR2(6)
)
PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
STORAGE(INITIAL 1048576 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
TABLESPACE "DATA1"

SQL> select dbms_metadata.get_ddl("INDEX","INDXX_PQZJYW","SHQSYS") from dual;

CREATE INDEX "SHQSYS"."INDXX_PQZJYW" ON "SHQSYS"."SZT_PQSO2" ("ZJYW")
PCTFREE 10 INITRANS 2 MAXTRANS 255
STORAGE(INITIAL 1048576 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645　
PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)　
TABLESPACE "DATA1"

SQL>　
SQL> spool off;

**2、获得整个SCHEMA DDL语句的方法：**

set pagesize 0
set long 90000
set feedback off
set echo off
spool get_schema.sql
connect shqsys/shqsys@hawk1;
SELECT DBMS_METADATA.GET_DDL("TABLE",u.table_name)
FROM USER_TABLES u;　
SELECT DBMS_METADATA.GET_DDL("INDEX",u.index_name)
FROM USER_INDEXES u;　
spool off;

需要注意的是，当我们的表中有外健（参照约束）时，我们需要判别参照表之间的顺序，确保重建时按照合理的顺序进行。你可以通过查询dba_constraints and dba_cons_columns来确定各表之间的顺序，不再详述。 

二、PL/SQL工具的使用
       PL/SQL Developer是一种集成的开发环境，专门用于开发、测试、调试和优化Oracle PL/SQL存储程序单元，比如触发器等。PL/SQL Developer功能十分全面，大大缩短了程序员的开发周期。强大的PL/SQL编辑器,完善的Debugger调试器（需要Oracle 7.3.4或以上版本）询问创建/SQL视窗/命令视窗/报告视窗/项目/浏览器/过程优化/HTML手册/Non-PL/SQL目标/模板目录/比较用户目标/输出用户目标/工具/lug-In扩展/Multi-threaded IDE/简单的安装(本软件由allroundautomations中国区总代理www.softwarecn.com 授权发布。)。

       下载地址：[http://www.onlinedown.net/soft/4902.htm](http://www.onlinedown.net/soft/4902.htm)


