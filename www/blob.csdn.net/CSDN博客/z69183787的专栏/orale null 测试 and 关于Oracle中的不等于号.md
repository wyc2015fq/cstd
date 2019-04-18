# orale null 测试 and 关于Oracle中的不等于号 - z69183787的专栏 - CSDN博客
2013年03月29日 22:38:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3097
create table testequal (id number,name varchar2(100));
（1）当数据类型为 varchar2时， 插入‘’，相当于null
 （2）当数据类型为 varchar2时，
select * from testequal where name<>'2323'; 查不出。
select * from testequal where name<>'2323' or name is null;   --可行。
 select * from testequal where instr(concat(name,'5555'),'2323')=0;---也可行
        ID NAME
---------- --------------------------------------------------------------------------------
         2 
         5
（3）||， concat 可以连接字符串 (包括null 与别的连接）
 select null||'a' from dual;
NULL||'A'
---------
a
SQL> select concat(null,'aaa') from dual;
CONCAT(NULL,'AAA')
------------------
aaa
关于Oracle中的不等于号
关于Oracle中的不等于号：
在Oracle中，
<> 
!= 
~= 
^=
都是不等于号的意思。都可以使用。
但是奇怪是的， 我想拿出providerid 不是180000的商品时：（providerid 是Number类型的）
SELECT id, name FROM product where providerid <> 180000;
执行这个语句时，providerid is null 的记录不出来。也就是拿不到providerid 是null的商品。必须使用：
SELECT id, name FROM product where providerid <> 180000 or providerid is null;才行。
字符串的字段存在同样的问题。
记住：null只能通过is null或者is not null来判断，其它操作符与null操作都是false。
======================================================================================================
测试：select * from test where name<>'xn'。只能查出name非空的记录。去掉name<>'xn'就可以了。这种写法有问题。
然后用了instr(name,'xn')=0 来判断，如果name非空的话，判断还是有效的。如果name为空，这个判断又出问题了。不得已只得采取instr(concat(name,'xx'),'xn') = 0来判断，因为就算name为空，当和'xx'连接后，也会不为空的。
所以最后的sql语句为：
select * from test where instr(concat(name,'xx'),'xn') = 0 来查询name字段不等于'xn'的记录。
或者可以用 select * from test where nvl(name,'xx')<>'xn' 来查询name字段不等于'xn'的记录。
