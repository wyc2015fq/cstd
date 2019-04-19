# Oracle存在修改，不存在插入记录 - 文章 - 伯乐在线
原文出处： [Sam Xiao](http://www.cnblogs.com/xcj26/p/3452628.html)
接触编程以来，在数据存储方面一直用的MS SQL。Oracle这名字对我来说是如此的熟悉，但是对其内容却很陌生，最近公司的一个项目用起了Oracle，所以也开始高调的用起了Oracle。在没有接触Oracle之前，听很多人都说Oracle的语法与MS SQL差不多，我在朋友圈里也帮着吹嘘这个观点。告诉朋友们，Oralce与MSSQL差不多，确实，貌似一看CRUD几乎没区别，但是当你慢慢深入了解Oracle的时候，你会发现这个观点有点愚蠢。
我们先来说个很常见的开发案例，有一张Account表，有两个字段分别为AccountID, AccountName，其中AccountID为主键，往这个表中插入数据，以主键为唯一标识，表中存在这条记录则修改，不存在则添加。
**一：在MS SQL中**
首先创建一个Account表，为了简单，我们都以nvarchar(50)作为字段类型。具体代码如下：


Oracle PL/SQL
```
if object_id(N'Account',N'U') is not null
drop table Account
create table Account
(
    AccountID nvarchar(50) primary key  not null,
    AccountName nvarchar(50)
)
```
接下来我们要做的事就是往这个表中插入数据


Oracle PL/SQL
```
if not exists (select * from Account where AccountID = '1') 
    insert into Account(AccountID,AccountName) values('1','Sam Xiao')
else
    update Account set AccountName = '肖建' where AccountID = '1'
```
这种代码，我们在SQL中是写的如此自然和熟练，但是你在Oracle中，你用这种方式来写，你会遇上一些麻烦。那现在我们在Oracle中来演示如何完成这样的需求。
**二：在Oracle中**
首先是创建表有着细微的区别，判断一个表是否存在，习惯了MS SQL的OBJECT_ID(‘对象表’,’对象类型’)的童鞋们，你们是不是想到Oracle中也应该有这样的功能呢？遗憾了，Oracle中没有此类函数来判断一个表是否存在，那就只能通过委婉的方式来实现，MS SQL中有类似于 Select Name From SysObjects Where XType=’U’这样的数据库表，那对应的Oracle中就有了select  * from user_tables，通过查询系统表，判断这个表在数据库中是否存在，如果存在就删除，然后再创建。


Oracle PL/SQL
```
declare num number;   
begin
    select count(1) into num from user_tables where table_name='ACCOUNT';   
    if num > 0 then   
      dbms_output.put_line('存在!');
      execute immediate 'drop table ACCOUNT '; 
    end if;   
      execute immediate 'create table Account
                        (
                                AccountID nvarchar2(50) primary key,
                                AccountName nvarchar2(50) 
                        )';  
      dbms_output.put_line('成功创建表!');
end;
```
与MS SQL创建一个表对比，是不是还是有一些显微的差异呢？答案当然是肯定的。
这个演示是前奏，现在来开始我们今天的主题，在Oracle中，表创建成功了，现在我要往这个表中插入数据，如果新插入的数据在表中存在则修改，不存在则插入，我在网上一搜，惊奇的发现Oracle中的exists()函数是判断两个数据集合的交集是否存在，与MS SQL有一定的区别。这样的对比虽然会显的不专业，但是我还是有对比和发表自己观点自由。于是我在网上疯狂的搜索Oracle在这个问题上的解决方案，总结了以下几种方案，以供大家选择：
**1：隐式游标法 SQL%NOTFOUND   SQL%FOUND**
SQL%NOTFOUND 是SQL中的一个隐式游标，在增删查改的时候自动打开，如果有至少有一条记录受影响，都会返回false，这就就巧妙的构思出了第一种解决方案：


Oracle PL/SQL
```
begin
update account set AccountName = '修改-a' where AccountID = '5';
IF SQL%NOTFOUND THEN
   insert into account(AccountID,AccountName) values('5','添加-b');
END IF;
end;
```
先根据唯一ID到数据表中修改一条记录，如果这条记录在表中存在，则修改，并且SQL%NOTFOUND返回false。如果修改的记录不存在，SQL%NOTFOUND返回true，并且执行插入语句。
**2：异常法 DUP_VAL_ON_INDEX**
当Oracle语句执行时，发生了异常exception进行处理


Oracle PL/SQL
```
begin
insert into account(AccountID,AccountName) values('6','添加-b');
exception 
when DUP_VAL_ON_INDEX then begin 
update account set AccountName = '修改-b' where AccountID = '6';
end;
end;
```
当往表中插入一条数据，因为表中有主键约束，如果插入的数据在表中已经存在，则会抛出异常，在异常抛出后进行修改。
**3：虚拟表法  dual**
dual是一个虚拟表，用来构成select的语法规则，oracle保证dual里面永远只有一条记录。


Oracle PL/SQL
```
declare t_count number;
begin
select count(*) into t_count from dual where exists(select 1 from account where AccountID='11');
if t_count 1 then
  dbms_output.put_line('添加');
  insert into account(AccountID,AccountName) values('11','添加-11');
else
  dbms_output.put_line('修改');
  update account set AccountName = '修改-11' where AccountID = '11';
  end if;
end;
```
先声明一个变量t_count,表dual表的值赋给t_count，如果这个值小于1，表示记录不存在，进行插入操作，反之，存在就进行修改操作。
**4：no_data_found法**
先查找要插入的记录是否存在，存在则修改，不存在则插入。具体的实现如下：


Oracle PL/SQL
```
declare t_cols number;
begin
select AccountName into t_cols from account where AccountID = '8';
exception 
when no_data_found then begin 
   --dbms_output.put_line('添加');
   insert into account(AccountID,AccountName) values('8','添加-8');
end;
when others then 
  begin
    --dbms_output.put_line('修改');
    update account set AccountName = '修改-8' where AccountID = '8';
end;
end;
```
**5：merge法**
先来看一下merge的语法，


Oracle PL/SQL
```
MERGE INTO table_name alias1   
USING (table|view|sub_query) alias2  
ON (join condition)   
WHEN MATCHED THEN   
    UPDATE table_name SET col1 = col_val1
WHEN NOT MATCHED THEN   
    INSERT (column_list) VALUES (column_values);
```
看了merge的语法后，依葫芦画瓢对于我这种抄袭的人来说已经不是什么难事了。


Oracle PL/SQL
```
merge into Account t1  
using (select '3' AccountID,'肖文博' AccountName from dual) t2  
on (t1.AccountID = t2.AccountID)  
when matched then  
     update set t1.AccountName = t2.AccountName
when not matched then  
     insert values (t2.AccountID, t2.AccountName);  
commit;
```
至此介绍了五种方法来解决我提出的问题。问题是小，但是已经牵涉了Oracle的好几个知识点。最后你与MS SQL相比，在用法上还是有很大的差异。至此，仁者见仁智者见智。
