# Oracle10g数据库常用语句 - 纯洁的微笑 - CSDN博客
2013年03月02日 11:58:05[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：1546
**第一节**
1. 数据类型：
字符类型：char(标准通用拉丁字符),nchar(汉字等其他字符),varchar2(长度可变字符),nvarchar2,long;
数字类型：number(通用),integer,float
日期和时间：date,timestamps(分秒，时区)
行：rowid(逻辑地址),urowid（逻辑地址，内存地址）;
二进制:raw(size)(原始二进制数据),long raw,blob(二进制大型对象;最大4G字节),clob(字符大型对象),nclob,bfile;
2.oracle WEB管理页面：localhost:5560/isqlplus; localhost:5500/em
3.net设置远程测试：tnsping datebasename；
  远程连接:sqlplus name/password@datebasename;
4.创建表空间：
```
create tablespace test 
datafile 'test.dbf' size 10m autoextend on next 2m
maxsize unlimited
logging
permanent
extent management local autoallocate
blocksize 8k
segment space management manuaL;//段空间
```
5．创建用户并连接：
```
create user "TEST" identified by "TEST" 
default tablespace TEST
temporary tablespace TEMP
quota unlimited on TEST
quota unlimited on TEMP
grant "connect" to test//分配基本权限。
conn test/test;
```
6．重设用户密码：scott/tiger为默认用户，alter user scott identified by tiger;
    解锁：alter user scott account unlock;
7.sql脚本的执行：@路径/filename.sql;
8.创建表：create table t1(c1 type 约束,c2 type 约束（not null,unique,check,primary key））
9.查询：select distinct c1 from t1 where 条件 group by c1 having by 子条件order by c1;
10．连接字符串：select c1 (as可省) 列1 ||c2 from t1;
11.单行函数：select lower(c1) from t1；upper全大写，initcap第一个字母大写，length；
12．Select Sysdate from dual(系统默认的空表)显示系统时间,months_between(date,date);
13.round(数据,5位数),to_date(1997-10-11,’yyyy-mm-dd’)，to_char()函数使用要转换。
14.nvl(c1,0)把字段为空的值换为0，nvl2(c1,1,0)不空的为1，空的值为0；
15.操作符：比较：=，<>,>=,<=,>,<；逻辑：and,or,not
   其他：in/not in,between..and..,is null/is not null,like，exists/not exists；
Eg:select count(distinct c1) as 种类 from t1 where c1 like ‘%l%’(模糊查询如m_n)(c1 between 10 and 100) group by c1 order by c1 desc，c2 desc（不写就默认asc）
16．聚合函数：count(*)返回所有行的记录数,min(c1),max(c1),sum(c1),avg(c1);
Eg:select c1,count(*) from t1 group by c1 having by count(*)>1;(having不能用变量申明);等价于select c1,count(*) as cn from t1 group by c1 where cn>1;
17．声明主键和check：一.create table t1(c1 primary key) 
二.创建表的时候指定限制的名称:create table t1(c1  constraint pk_emp primary key); 
三：create table t1(emp_no number not null , constraint pk_emp primary key(emp_no))； 
为已经存在的表增加主键:alter table t1 add constraint pk_emp2 primary key (c1)； 
创建check: create table t1(budget number(7),constraint CHK_PROJ check(budget>10000 and budget<1000000))
18.创建外键: create table t1(dept_no varchar2(4) not null, constraint fk_emp foreign key (dept_no) references t2(dept_no), 
对已经存在表创建外键:alter table t1  add constraint foreign_work_emp foreign key(c1) references t2(c1)；
删除一个外键alter table t1  drop constraint foreign_work_emp；
19.删除表：drop table t1; 查看回收站:show recyclebin; 清空回收站 purge recyclebin;
从回收站中恢复数据表:flashback table t1 to before drop;
20.更新数据操作：插入记录：insert into t1(c1,c2)values(‘’,’’)；
插入一字段:insert into t1(c1,c2) select c3,c4 from t2;
更新记录：update t1   set c1=’’ where ;
删除记录：delete from t1 where;truncate table t1;drop table t1;
21.合并查询：intersect(select * intersect select *),union(去掉空),union all(包括空),minus（减集）;
22.多表查询:select * from t1,t2;(笛卡尔集c1行*c2行)；select * from t1 join t2 using(num);等价于select * from t1 inner join t2 on(t1.no=t2.no);join逻辑连接，只连接有联系的字段，full join物理机械连接，out join,left out join(右边变成空),right out join;
23．控制语句：select emp_no, case project_no
```
when 'p1' then case when enter_date<to_date('1997-10-1','yyyy-mm-dd')then '三室'
  when enter_date>to_date('1998-10-1','yyyy-mm-dd')then '等两年'//时间非字段时间型
       else '两室一厅'end
  when 'p2' then case when enter_date<to_date('1997-10-1','yyyy-mm-dd')
       then '三室两厅' when enter_date>to_date('1998-10-1','yyyy-mm-dd')
       then '看情况再分' else ' 一室一厅'end
 end as 分房情况from works_on；
if then end if,loop end loop,declare type *,while( )loop end loop,case when then end
```
24.嵌套查询select c1 from t1 where c2 in(select c2 from t2 where c3=(select c3 from t3))
25．Pl/sql语言：模块语法和流程控制：（头控制符没有标点；尾控制符都有；）
```
declare icount number;
begin
   icount :=0;
   for mycount in reverse 0..10//mycount 为自定义变量
   ---while(icount<100)
   Loop   icount :=icount+10;
   ---exit when(icount>100);
   if(mycount>5) then
     dbms_output.put_line('result='||icount);//流程输出语句
end if;
   end loop;
 end; //(set serverout on才能输出)
```
26．序列Sequence. create sequence se_1  increment by 1  start with 100
maxvalue 999999//minvalue n --表示序列可以生成的最小值(降序).  cycle;//到头自动循环
查看：select se_1.nextval from dual; select se_1.currval from dual;（必先next初始化）
使用：create table stu1(stu_no number(10) not null,stu_name varchar2(20) not null);
insert into stu1(stu_no,stu_name) values(se_1.nextval,'joi');
修改序列:alter sequence <sequencen_name>//start with 语句不能用了，否则重复定义
删除序号：drop sequence <sequence_name>；
自动序列rownum:select * from t1 where rownum<5;
select * from(select rownum as a,e.* from t1 e)where a=3//单行必先关联，e为表的别名
27．创建视图create or replace view v1 as
select c1 from t1;
使用视图：select * from v1;
28．创建函数：
```
create or replace function get_maxe_empno return varchar2   is
tmpvar varchar2(20);//局部变量声明
begin  //可执行语句
select max(emp_no) into tmpvar from employee;//把取出的值赋给一个变量
return tmpvar;//函数的重要特征就是通过return语句来传递参数
end get_maxe_empno;
```
使用：select get_maxe_empno() from dual;
数字字典select distinct(name) from user_source where type='FUNCTION'
从数字字典里查找信息：select text from all_source where name='GET_MAXE_EMPNO';
删除函数：drop function get_maxe_empno;
29.过程：
```
create or replace procedure sp_test
(fname in varchar2, update_count out int)//参数列表是过程的特征  is
             cursor emp_cursor//定义游标
            is
             select emp_no from employee where employee.emp_fname=fname;//输入in
         begin        //   可执行语句
     update_count:=0;
     for emp_rec in emp_cursor
     loop
             update employee set emp_fname=emp_lname,emp_lname=fname
     where employee.emp_no=emp_rec.emp_no;update_count:=update_count+1;
     end loop;commit;//控制语句，强制执
         end；//过程可以有in/out变量
```
查看过程：
```
select distinct(name) from user_source where type='PROCEDURE'
select text from all_source where name='SP_TEST' order by line;
```
``
使用过程：
declare fname varchar2(200);
update_count number;
begin
fname:='John';update_count:=0;
sp_test(fname,update_count);commit;dbms_output.put_line(update_count);
end;
30.触发器：
```
create or replace trigger tgr_emp_change
after insert or update or delete on employee
  referencing new as n old as o// :new为引用最新列值; :old为引用以前列值;
   for each row
 begin 
      if inserting then
      insert into emp_update_info(emp_no,oper_type) values(:n.emp_no,1);
      end if;
      if updating then  // updating为触发器的操作符；
      insert into emp_update_info(emp_no,oper_type) values(:o.emp_no,3);
      end if;
 end;
```
建表：create table emp_update_info(emp_no varchar2(20) not null,oper_type integer);
查看trigger：select distinct( name ) from user_source  where type='TRIGGER';
对employee的操作就自动出发的emp_update_info
31．声明包：
```
create or replace package pkg_test as
function get_max_empno   return  varchar2;//子程序说明，公共对象声明
procedure sp_test(fname in varchar2,update_count out int);
end pkg_test;
创建包体:create or replace package body pkg_test  as // is也行
           function get_max_empno  return varchar2   as //公共类型与对象声明
           tmpvar    varchar2(20);//公共类型和对象声明，  
          begin   // 语句体中是初始化语句，特殊用途
           select max(emp_no) into tmpvar from employee; return tmpvar;
         end get_max_empno;
         //子程序主体部分
           procedure sp_test(fname in varchar2,update_count out int) is
           cursor emp_cursor  is
           select emp_no from employee where employee.emp_fname = fname;
           begin
           update_count := 0;
           for emp_rec in emp_cursor
           loop
           update employee set emp_fname = emp_lname, emp_lname = fname
           where employee.emp_no = emp_rec.emp_no;
           update_count := update_count + 1;
           end loop;
           commit;//强制执行
   end;
end pkg_test;
```
查看定义的package：select distinct( name ) from user_source where type='PACKAGE'
使用package中的对象：select pkg_test.get_max_empno from dual;
32．数据库设计：业务规则和流程—实体和关系—ER图（一对一，一对多，出现多对多进行拆分）—SQL语句—视图和索引—储存过程和触发器—维护。
**第二节**
2、显示当前连接用户 
SQL> show user 
3、查看系统拥有哪些用户 
SQL> select * from all_users; 
4、新建用户并授权 
SQL> create user a identified by a;（默认建在SYSTEM表空间下） 
SQL> grant connect,resource to a; 
5、连接到新用户 
SQL> conn a/a 
6、查询当前用户下所有对象 
SQL> select * from tab; 
7、建立第一个表 
SQL> create table a(a number); 
8、查询表结构 
SQL> desc a 
9、插入新记录 
SQL> insert into a values(1); 
10、查询记录 
SQL> select * from a; 
11、更改记录 
SQL> update a set a=2; 
12、删除记录 
SQL> delete from a; 
13、回滚 
SQL> roll; 
SQL> rollback; 
14、提交 
SQL> commit; 
用户授权: 
GRANT ALTER ANY INDEX TO "user_id " 
GRANT "dba " TO "user_id "; 
ALTER USER "user_id " DEFAULT ROLE ALL 
创建用户: 
CREATE USER "user_id " PROFILE "DEFAULT " IDENTIFIED BY " DEFAULT TABLESPACE 
"USERS " TEMPORARY TABLESPACE "TEMP " ACCOUNT UNLOCK; 
GRANT "CONNECT " TO "user_id "; 
用户密码设定: 
ALTER USER "CMSDB " IDENTIFIED BY "pass_word " 
表空间创建: 
CREATE TABLESPACE "table_space " LOGGING DATAFILE 
'C:\ORACLE\ORADATA\dbs\table_space.ora' SIZE 5M 
------------------------------------------------------------------------ 
1、查看当前所有对象 
SQL > select * from tab; 
2、建一个和a表结构一样的空表 
SQL > create table b as select * from a where 1=2; 
SQL > create table b(b1,b2,b3) as select a1,a2,a3 from a where 1=2; 
3、察看数据库的大小，和空间使用情况 
SQL > col tablespace format a20 
SQL > select b.file_id　　文件ID, 
　　b.tablespace_name　　表空间, 
　　b.file_name　　　　　物理文件名, 
　　b.bytes　　　　　　　总字节数, 
　　(b.bytes-sum(nvl(a.bytes,0)))　　　已使用, 
　　sum(nvl(a.bytes,0))　　　　　　　　剩余, 
　　sum(nvl(a.bytes,0))/(b.bytes)*100　剩余百分比 
　　from dba_free_space a,dba_data_files b 
　　where a.file_id=b.file_id 
　　group by b.tablespace_name,b.file_name,b.file_id,b.bytes 
　　order by b.tablespace_name 
　　/ 
　　dba_free_space --表空间剩余空间状况 
　　dba_data_files --数据文件空间占用情况 
4、查看现有回滚段及其状态 
SQL > col segment format a30 
SQL > SELECT SEGMENT_NAME,OWNER,TABLESPACE_NAME,SEGMENT_ID,FILE_ID,STATUS FROM 
DBA_ROLLBACK_SEGS; 
5、查看数据文件放置的路径 
SQL > col file_name format a50 
SQL > select tablespace_name,file_id,bytes/1024/1024,file_name from 
dba_data_files order by file_id; 
6、显示当前连接用户 
SQL > show user 
7、把SQL*Plus当计算器 
SQL > select 100*20 from dual; 
8、连接字符串 
SQL > select 列1 | |列2 from 表1; 
SQL > select concat(列1,列2) from 表1; 
9、查询当前日期 
SQL > select to_char(sysdate,'yyyy-mm-dd,hh24:mi:ss') from dual; 
10、用户间复制数据 
SQL > copy from user1 to user2 create table2 using select * from table1; 
11、视图中不能使用order by，但可用group by代替来达到排序目的 
SQL > create view a as select b1,b2 from b group by b1,b2; 
12、通过授权的方式来创建用户 
SQL > grant connect,resource to test identified by test; 
SQL > conn test/test 
13、查出当前用户所有表名。 
select unique tname from col; 
----------------------------------------------------------------------- 
/* 向一个表格添加字段 */ 
alter table alist_table add address varchar2(100); 
/* 修改字段 属性 字段为空 */ 
alter table alist_table modify address varchar2(80); 
/* 修改字段名字 */ 
create table alist_table_copy as select ID,NAME,PHONE,EMAIL, 
QQ as QQ2, /*qq 改为qq2*/ 
ADDRESS from alist_table; 
drop table alist_table; 
rename alist_table_copy to alist_table 
/* 修改表名 */ 
空值处理 
有时要求列值不能为空 
create table dept (deptno number(2) not null, dname char(14), loc char(13)); 
在基表中增加一列 
alter table dept 
add (headcnt number(3)); 
修改已有列属性 
alter table dept 
modify dname char(20); 
注：只有当某列所有值都为空时，才能减小其列值宽度。 
只有当某列所有值都为空时，才能改变其列值类型。 
只有当某列所有值都为不空时，才能定义该列为not null。 
例： 
alter table dept modify (loc char(12)); 
alter table dept modify loc char(12); 
alter table dept modify (dname char(13),loc char(12)); 
查找未断连接 
select process,osuser,username,machine,logon_time ,sql_text 
from v$session a,v$sqltext b whe
**第三节**
1.以USER_开始的数据字典视图包含当前用户所拥有的信息, 查询当前用户所拥有的表信息: 
select * from user_tables; 
2.以ALL_开始的数据字典视图包含ORACLE用户所拥有的信息, 
查询用户拥有或有权访问的所有表信息: 
select * from all_tables; 
3.以DBA_开始的视图一般只有ORACLE数据库管理员可以访问: 
select * from dba_tables; 
4.查询ORACLE用户： 
conn sys/change_on_install 
select * from dba_users; 
conn system/manager; 
select * from all_users; 
5.创建数据库用户： 
CREATE USER user_name IDENTIFIED BY password; 
GRANT CONNECT TO user_name; 
GRANT RESOURCE TO user_name; 
授权的格式: grant (权限) on tablename to username; 
删除用户(或表): 
drop user(table) username(tablename) (cascade); 
6.向建好的用户导入数据表 
IMP SYSTEM/MANAGER FROMUSER = FUSER_NAME TOUSER = USER_NAME FILE = C:\EXPDAT.DMP 
COMMIT = Y 
7.索引 
create index [index_name] on [table_name]( "column_name ") 
表： 
建表： 
create table user_list
    (
     user_name varchar2(23),
      user_age int 
);
查看表结构： 
desc user_list;
添加字段： 
alter table user_list
    add (userno int );
修改字段： 
alter table user_list
   modify ( user_name varchar2(50));
删除字段： 
alter table user_list
drop column user_age;
对表添加数据： 
insert into user_list(user_name ,userno) values('gouchao' ,10001);
insert into user_list values('xuyongchao',10003);
insert into user_list values('gouchao' ,S_userno.Nextval);
对表修改数据 ： 
update user_list 
    set user_name ='gouli' where user_name ='gouchao';
对表删除数据： 
delete from user_list where userno = 10001;
删除表格: 
drop table user_list;
查询： 
查询所有记录： 
select * from user_list;
select userno from user_list;
select userno ID from user_list;
select user_list.userno from user_list;
select a.userno from user_list a ;
查询所有记录的某些字段： 
select empno ,ename from emp;
查询某些字段的不同记录： 
select distinct job from emp;
单条件 的查询： 
= 
select  * from emp where job='CLERK';
!= 
select  * from emp where job!='CLERK';
>  
select * from emp where sal > 1600;
<　 
select * from emp where sal < 1600;
>=　 
select * from emp where sal >=1600;
<= 
select * from emp where sal <= 1600;
in  
select * from emp where sal in (1600,3000);
not in 
select * from emp where sal not in(1600,3000);
between and 
select * from emp where sal between 1600 and 3000 ;
Like 
select * from emp where job like 'M%';
select * from emp where job like 'M_';
select * from  emp where job like 'MANAGE_';
not like 
elect * from emp where job not like 'M_';
select * from emp where job not like 'M%';
Is null 
select * from emp where sal is null;
select * from emp where job is null;
Is not null 
select * from emp where sal is not null;
select * from emp where job is not null;
组合条件的查询： 
and 
select * from emp where job ='CLERK' and sal <3000;
or 
elect * from emp where job ='CLERK' or sal <3000;
not 
select * from emp where not job ='CLERK';
排序查询： 
select * from emp order by job asc , sal desc;
分组查询: 
group by  having 
select empno ,ename ,job ,sal from emp group by job ,empno , ename ,sal having sal <=2000;
where group by 
select empno , ename , job , sal from emp where sal <=2000 group by job ,empno ,ename ,sal ;
字段运算查询: 
+ 
select ename ,job , mgr+sal from emp;
- 
select ename ,job , mgr-sal from emp;
*  
select ename ,job , mgr*sal from emp;
/ 
select ename ,job , mgr/sal from emp;
变换查询显示: 
 select empno 编号,job 工作,sal 薪水 from emp ;
用 SQL 进行多表查询 
无条件多表查询:产生笛卡尔积 
select a.empno , a.ename , a.sal , b.deptno , b.dname ,b.loc from emp a , dept b ;
等值多表查询 
select a.empno , a.ename , a.sal , b.deptno , b.dname ,b.loc from emp a , dept b  where a.deptno = b.deptno;
非等值多表查询 
select a.empno , a.ename , a.sal , b.deptno , b.dname ,b.loc from emp a , dept b  where a.deptno != b.deptno and a.deptno = 10;
用 SQL 进行嵌套查询 
简单嵌套查询 
select *  from emp where sal <=(select sal from emp where ename = 'SMITH');
select *  from emp where sal <(select sal from emp where ename = 'SMITH');
select *  from emp where sal >=(select sal from emp where ename = 'SMITH');
select *  from emp where sal >(select sal from emp where ename = 'SMITH');
带【in】的嵌套查询 
select *  from emp where sal IN(select sal from emp where ename = 'WARD');
select *  from emp where sal not IN(select sal from emp where ename = 'WARD');
带【any】的嵌套查询 
select  * from emp where sal > any(select sal from emp where job='MANAGER');
带【some】的嵌套查询 
select  * from emp where sal  = some (select sal from emp where job='MANAGER');
带【all】的嵌套查询 
select  * from emp where sal  > all(select sal from emp where job='MANAGER');
喜欢我的文章，请关注我的公众号
**![](https://img-blog.csdn.net/20180406125523605)**
