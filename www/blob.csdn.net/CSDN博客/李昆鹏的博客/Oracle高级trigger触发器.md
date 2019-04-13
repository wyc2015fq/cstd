
# Oracle高级trigger触发器 - 李昆鹏的博客 - CSDN博客


2018年06月07日 00:00:34[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：135


-------------------------------------------Oracle高级trigger触发器--------------------------------------------------
## 触发器
数据库触发器是一个与表相关联的、存储的PL/SQL程序。每当一个特定的数据操作语句(Insert,update,delete)在指定的表上发出时，Oracle自动地执行触发器中定义的语句序列。
**触发器可用于**
数据确认
实施复杂的安全性检查
做审计，跟踪表上所做的数据操作等
数据的备份和同步
**触发器的类型**
语句级触发器：在指定的操作语句操作之前或之后执行一次，不管这条语句影响                                                                            了多少行。
行级触发器（FOR EACH ROW） ：触发语句作用的每一条记录都被触发。在行级触                                                 发器中使用old和new伪记录变量, 识别值的状态。
语法：
**CREATE  [or REPLACE] TRIGGER  触发器名**
**{BEFORE | AFTER}**
**{DELETE | INSERT | UPDATE [OF 列名]}**
**ON  表名**
**[FOR EACH ROW [WHEN(条件) ] ]**
**begin**
**PLSQL 块**
**End 触发器名**
范例：插入员工后打印一句话“一个新员工插入成功”
createorreplacetriggertestTrigger
afterinsertonperson
declare
*-- local variables here*
begin
dbms_output.put_line('一个员工被插入');
endtestTrigger;
范例：不能在休息时间插入员工
createorreplacetriggervalidInsertPerson
beforeinsertonperson
declare
weekendvarchar2(10);
begin
selectto_char(sysdate,'day')intoweekendfromdual;
ifweekendin('星期一')then
raise_application_error(-20001,'不能在非法时间插入员工');
endif;
endvalidInsertPerson;
当执行插入时会报错
![](https://img-blog.csdn.net/20180606234730304?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在触发器中触发语句与伪记录变量的值
|触发语句
|:old
|:new
|
|Insert
|所有字段都是空(null)
|将要插入的数据
|
|Update
|更新以前该行的值
|更新后的值
|
|delete
|删除以前该行的值
|所有字段都是空(null)
|
范例：判断员工涨工资之后的工资的值一定要大于涨工资之前的工资
createorreplacetriggeraddsal4p
beforeupdateofsalonmyemp
foreachrow
begin
if:old.sal >= :new.salthen
raise_application_error(-20002,'涨前的工资不能大于涨后的工资');
endif;
end;
调用
updatemyemp tsett.sal = t.sal -1;
![](https://img-blog.csdn.net/20180606234750411?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
create tablespace my0108
logging
datafile'D:\oracle\product\10.2.0\oradata\orcl\my0108.dbf'
size 32m
autoextend on
next 32m maxsize 2048m
extent management local;
代码示例：
|--创建触发器
|create or replace trigger insertptrg
|before insert on person
|begin
|dbms_output.put_line('一条数据被插入');
|end insertptrg;
|insert into person  values(seqpersonid.nextval,'王五',1,sysdate);
|--不能再休息时间插入数据
|create or replace trigger valid_insert_p
|before insert on person
|declare
|cruday varchar2(10); --定义字符串的时候要指定长度
|begin
|select to_char(sysdate, 'day') into cruday from dual;
|if cruday = '星期三' then
|raise_application_error(-20001, '星期三不能插入数据');
|end if;
|end valid_insert_p;
|--给员工涨工资，涨之后的工资不能低于涨之前的工资
|create or replace trigger valid_addsal
|before update of sal on myemp
|for each row
|begin
|if :new.sal <= :old.sal then
|raise_application_error(-20002, '涨之后的工资不能低于涨之前的工资');
|end if;
|end valid_addsal;
|update myemp t set t.sal = t.sal - 100  where t.empno = 7369;
|


