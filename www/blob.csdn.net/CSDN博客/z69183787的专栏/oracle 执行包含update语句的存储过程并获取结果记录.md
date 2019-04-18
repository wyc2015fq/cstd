# oracle 执行包含update语句的存储过程并获取结果记录 - z69183787的专栏 - CSDN博客
2014年08月05日 10:19:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11498
斜杠 就是让服务器执行前面所写的 SQL 脚本。
因为你普通的 select 语句， 一个分号，就可以执行了。
但是如果你的是存储过程， 那么遇到分号，就不能马上执行了。
这个时候，就需要通过 斜杠 来执行了。
存储过程：
```
create or replace procedure updateJobContact (s in varchar,
                                      num out varchar
)
AS
s_sql varchar(5000) := '';
num1 number;
begin
s_sql := 'update t_dept_contact_tree t set t.status=0 where status=0 '; //insert 也可
execute immediate s_sql;
num1:=sql%rowcount;
num := num1 || '';
dbms_output.put_line(num);
commit;
end updateJobContact;
```
调用方式：
comman中；
```
set serveroutput on;
	declare
    so varchar(10);
   	begin
    updatejobcontact('1',so);
    end;
    /
```
