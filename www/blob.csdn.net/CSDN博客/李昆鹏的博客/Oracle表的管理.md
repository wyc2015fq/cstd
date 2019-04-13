
# Oracle表的管理 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:35:46[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：112


-----------------------------------------------------------Oracle表的管理----------------------------------------------------
### 常用的数据类型
|No
|数据类型
|描述
|
|1
|Varchar， varchar2
|表示一个字符串
|
|2
|NUMBER
|NUMBER(n)表示一个整数，长度是n
|
|NUMBER(m,n):表示一个小数，总长度是m，小数是n，整数是m-n
|
|3
|DATA
|表示日期类型
|
|4
|CLOB
|大对象，表示大文本数据类型，可存4G
|
|5
|BLOB
|大对象，表示二进制数据，可存4G
|
### Varchar
### 和varchar2
### 的区别：
### 1.varchar2
### 把所有字符都占两字节处理(
### 一般情况下)，varchar只对汉字和全角等字符占两字节，数字，英文字符等都是一个字节；
### 2.VARCHAR2
### 把空串等同于null
### 处理，而varchar仍按照空串处理；
### 3.VARCHAR2
### 字符要用几个字节存储，要看数据库使用的字符集，
### 大部分情况下建议使用varchar2
### 类型，可以保证更好的兼容性。
### 2建表
语法：
Create table 表名（
字段1    数据类型       [default 默认值],
字段2    数据类型       [default 默认值],
...
字段n    数据类型       [default 默认值]
）;
范例：创建person表
createtableperson(
pidnumber(10),
namevarchar2(10),
gendernumber(1)default1,
birthdaydate
);
insertintoperson(pid,name, gender, birthday)
values(1,'张三',1, to_date('1999-12-22','yyyy-MM-dd'));
### 2.表删除
语法：DROP TABLE 表名
### 3.表的修改
在sql中使用alter可以修改表
添加语法：ALTER TABLE 表名称 ADD(列名1 类型 [DEFAULT默认值]，列名1 类型 [DEFAULT 默认值]...)
修改语法：ALTER TABLE 表名称 MODIFY(列名1 类型[DEFAULT 默认值]，列名1 类型 [DEFAULT 默认值]...)
注意修改时如果原有某列的数据长度为200，则不可以把该列修改成小于200的列
范例：在person表中增加列address
altertablepersonadd(addressvarchar2(10));
范例：把person表的address列的长度修改成20长度
altertablepersonmodify(addressvarchar2(20));
**删除表中的列：**
**ALTER TABLE****表名 DROP COLUMN****列名;**
**--****删除列**
**alter table person drop column address;**
### 4截断表
在person表中使用delete语句删除数据，则可以使用rollback来回滚，如果想要清空一张表的数据，同时不想回滚可以立即释放资源需要使用截断表的语法
语法：TRUNCATE TABLE 表名
范例：截断person表
truncatetableperson;
代码示例：
|--创建一个person表
|create table person(
|person_id number(4),
|pname     varchar2(10),
|age       number(3),
|birthday  date
|);
|--向person表中插入数据包
|insert into person
|(person_id, pname, age, birthday, address)
|values
|(1000, '张三', 20, to_date('1985-04-22',  'yyyy-mm-dd'), 'beijing');
|--（drop table 表名）- 删表
|drop table person;
|--对表进行添加列
|alter table person add(address  varchar2(50));
|--对列进行修改，修改的长度不能小于列表内容的长度
|alter table person modify(address  varchar2(7));
|--删除列
|alter table person drop column address;
|--删除表中的所有数据，使用delete删除需要事务提交确认
|delete person;
|--使用截断表删除，无需提交事务，直接清除数据,慎用
|truncate table person;
|select * from person;
|


