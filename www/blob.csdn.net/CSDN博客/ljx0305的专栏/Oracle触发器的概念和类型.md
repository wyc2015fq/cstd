# Oracle触发器的概念和类型  - ljx0305的专栏 - CSDN博客
2008年08月02日 22:55:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1951
# 1.引言 <?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
# 2.触发器的概念和类型
触发器是一种特殊的存储过程，它在插入，删除或修改特定表中的数据时触发执行，它比数据库本身标准的功能有更精细和更复杂的数据控制能力。数据库触发器有以下的作用：
* 安全性。可以基于数据库的值使用户具有操作数据库的某种权利。
# 可以基于时间限制用户的操作，例如不允许下班后和节假日修改数据库数据。
# 可以基于数据库中的数据限制用户的操作，例如不允许股票的价格的升幅一次超过10%。
* 审计。可以跟踪用户对数据库的操作。
# 审计用户操作数据库的语句。
# 把用户对数据库的更新写入审计表。
* 实现复杂的数据完整性规则。
# 实现非标准的数据完整性检查和约束。触发器可产生比规则更为复杂的限制。与规则不同，触发器可以引用列或数据库对象。例如，触发器可回退任何企图吃进超过自己保证金的期货。
# 提供可变的缺省值。
* 实现复杂的非标准的数据库相关完整性规则。触发器可以对数据库中相关的表进行连环更新。例如，在auths表author_code列上的删除触发器可导致相应删除在其它表中的与之匹配的行。
# 在修改或删除时级联修改或删除其它表中的与之匹配的行。
# 在修改或删除时把其它表中的与之匹配的行设成NULL值。
# 在修改或删除时把其它表中的与之匹配的行级联设成缺省值。
# 触发器能够拒绝或回退那些破坏相关完整性的变化，取消试图进行数据更新的事务。当插入一个与其主健不匹配的外部键时，这种触发器会起作用。例如，可以在books.author_code列上生成一个插入触发器，如果新值与auths.author_code列中的某值不匹配时，插入被回退。
* 同步实时地复制表中的数据。
* 自动计算数据值，如果数据的值达到了一定的要求，则进行特定的处理。例如，如果公司的帐号上的资金低于5万元则立即给财务人员发送警告数据。
数据库触发器（dataｂａｓｅ triggers）是响应插入、更新或删除等数据库事件而执行的过程。它定义了当一些数据库相关事件发生时应采取的动作。可用于管理复杂的完整性约束，或监控对表的修改，或通知其它程序，表已发生修改。它的类型有：语句级触发器，以及行级触发器，前者可以在语句执行前或执行后被触发。后者在每个触发语句影响的行触发一次。还有before和after触发的命令。在insert,update,和delete之前或之后执行，引用新旧值进行处理。如果需通过触发器设定插入行中的某列值，则为了访问“新(new)”值，需使用一个触发器before insert，使用after insert则不行。Instead of 触发器命令，使用它告诉oracle应执行什么操作。以上四种大类合成14种小类（略）。各种触发器的执行顺序如下：
⑴如果有，最先执行语句级before触发器。
⑵每个insert，delete,update影响的行；
①如果有，最先执行行级before；
②执行行的delete或update；
③如果有,执行行级after触发器；
⑶如果有,执行语句级after触发器。
# 3.使用数据库触发器管理数据冗余性
为了数据分析和制作报表的需要,用户在数据模型中加入了冗余数据，应使用触发器,以保证数据的完整性。
冗余数据可以用规定的 for each row选项的before update触发器进行管理。update 命令可放在触发器中对所有需要更新的冗余数据进行更新,如客户表和订单表,订单表包括客户的订单和客户表的冗余信息,客户表(customer)的表结构:客户号(cu_no)、客户名(cu_name)、客户地址(cu_address)。订单表(order)的表结构:订单号(or_no),客户号(or_no)，客户名（cu_name），客户地址（cu_address）,当客户基表中的数据被更新时更新订单中的冗余列。语法如下：
create or replace trigger bj_customer 
before update on customer 
for each row 
begin 
update order set 
cu_no=:new.cu_no，
cu_name=:new.cu_name，
cu_address=:new.cu_addess，
where cu_no=:old.cu_no；
end;
# 4.用触发器完成数据复制
如果需求非常有限，可以用数据触发器从一个数据库中向另一个数据库复制数据，如果数据复制需求仅与数据的插入有关，当一条记录插入到一个数据库中的某个基表中时，用户还希望把这条记录插入到一个远程数据库中，需用create dataｂａｓｅ link语句创建一条到远程数据库的连接，一旦创建了一条数据库连接后，就可以在基表上创建一个after insert触发器，以把每一条记录插入到远程数据库中。
（1）在脚本中创建数据库连接（dataｂａｓｅ link）bj_ysd_remote基表作为数据库基表，Bj_ysd_local代表本地数据库上的源基表。
Creat dataｂａｓｅ link remote(连接名) 
Connect to bj(帐户) indentified by bj(密码) 
Using ‘:2’;
（2）复制记录
create or replace trigger trig_ysd（触发器名）
after insert on bj_ysd_local 
for each row 
begin 
insert into bj_ysd_remote@dblink remote 
value(:new.x1,:new.x2,……)/＊x1.x2代表字段名＊／
end;
（3）删除记录
create or replace trigger trig_ysd_del 
after delete on bj_ysd_local 
for each row 
begin 
delete from bj_ysd_remote@dblink remote 
where x1=:old.x1 
end;
# 5.用数据库触发器完成瀑布式删除操作
在某些情况下，当要删除一条记录时，该记录是与外键有关的另外一张基表上的记录时，这个删除操作必须在模型中进行传递，否则会出现大量的冗长数据，仍以cumstomer 和order基表为例，当从customer中删除一个客户时，order基表中所有相关记录也应当删除。
Create or replace trigger trig_cust 
Before delete oncustomer 
For each row 
Begin 
Delete from order 
Wherecu_no=old.cu_no；
End;
# 6.用触发器完成动态数据的操作
在涉及如何实现动态库存的问题时，可用触发器解决。仓库有验收、出库、调拨、报废、退料、让售等这些数据必须与以前的库存相加减，才能完成动态库存操作。本文仅以验收单触发器为例，其它的结构雷同。它们涉及到两个基表：bi_ysd(验收单)，Bj_kcb(当前库存表)，前者的表结构（rq(日期)，ysdh(验收单号)， bjbm(备件编码)，yssl(验收数量)，ysdj(验收单价)），后者的表结构为（bjbm（备件编码）,dqkcl（当前库存量）,dqkcje（当前库存金额））触发器如下：
create or replace trigger trig_ysd 
after insert or update or delete on bj_ysd 
for each row 
declare rq1 varchar2(8);rq2 varchar2(8); 
/*限于篇幅，yssl1,yssl2,ysdj1,ysdj2,bjbm1,bjbm2,ii声明略*/ 
if inserting or updating then 
rq1:=:new.rq;bjbm1:=:new.bjbm;yssl1:=:new.yssl; 
ysdj1:=:new.ysdj; 
select count(*) into ii from bj_dqkcb 
where bjbm=bjbm1; 
ifii=0then 
insertintobj_dqkcb(bjbm,dqkcl,dqkcje) 
value(bjbm1,yssl1,ysdj1); 
else 
update bj_dqkcb 
set dqkcl=dqkcl+yssl1; 
dqkcje=dqkcje+yssl1*ysdj1; 
end if 
end if 
if deleting or updating then 
rq2:=:old.rq;bjbm2:=:old.bjbm;yssl2:=:old.yssl; 
ysdj2:=:old.ysdj; 
update bj_dqkcb 
set dqkcb=dqkcl-yssl2; 
dqkcje=dqkcje-yssl2*ysdj2 
end if; 
end ;
# 7.结束语
数据库触发器在数据库开发上有很广泛的应用，但经验表明，使用过多的触发器将降低整个数据库的性能。如果数据库触发器写得不好，它会很快破坏数据库的性能，因此，在适当的时候使用恰当的触发器显得非常重要。
-------------------------------------------------------------------------------------------------
ORACLE与SYｂａｓｅ数据库的触发器有一定的区别，下面将分别讲述这两种数据库触发器的作用和写法。
# 1.ORACLE 触发器
ORACLE产生数据库触发器的语法为：
create [or replace] trigger 触发器名触发时间触发事件
on 表名
[for each row]
pl/sql 语句
其中：
**触发器名**：触发器对象的名称。由于触发器是数据库自动执行的，因此该名称只是一个名称，没有实质的用途。
**触发时间**：指明触发器何时执行，该值可取：
before---表示在数据库动作之前触发器执行；
after---表示在数据库动作之后出发器执行。
**触发事件**：指明哪些数据库动作会触发此触发器：
insert：数据库插入会触发此触发器；
update：数据库修改会触发此触发器；
delete：数据库删除会触发此触发器。
**表名**：数据库触发器所在的表。
**for each row**：对表的每一行触发器执行一次。如果没有这一选项，则只对整个表执行一次。
举例：下面的触发器在更新表auths之前触发，目的是不允许在周末修改表：
create trigger auth_secure
before insert or update or delete //对整表更新前触发
on auths
begin
if(to_char(sysdate,'DY')='SUN'
RAISE_APPLICATION_ERROR(-20600,'不能在周末修改表auths');
end if;
end
# 2.SYｂａｓｅ数据库触发器
SYｂａｓｅ数据库触发器的作用与ORACLE非常类似，仅有较小的差异。
SYｂａｓｅ产生触发器的语法为：
CREATE TRIGGER 触发器名
ON 表名
FOR INSERT，UPDATE，DELETE
AS
SQL_statement |
FOR INSERT，UPDATE
AS
IF UPDATE(column_name) [AND|OR UPDATE(column_name)]...
SQL_statements
上面FOR子句用来指定在触发器上的哪些数据更新命令可激活该触发器。IF UPDATE子句检查对指定列的操作类型，在IF UPDATE子句中可指定多个列。
与ORACLE不同，对于每条SQL语句，触发器只执行一次。触发器在数据更新语句完成以后立即执行。触发器和启动它的语句被当作一个事务处理，事务可以在触发器中回退。
下面举例说明SYｂａｓｅ触发器的写法。
create trigger forinsert_books
on books
for insert
as
if(select count(*) from auths,inserted
where auths.author_code=insert.author_code)!=@@rowcount
begin
rollback transaction
print "books 表中 author_code 列的值在auths 表中不存在。"
end
引用：[http://i.mop.com/jerrywens/blog/2008/04/16/6359650.html](http://i.mop.com/jerrywens/blog/2008/04/16/6359650.html)
