# Oracle 存储过程 - z69183787的专栏 - CSDN博客
2012年10月25日 14:22:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：872
个人分类：[Oracle																[数据库-存储过程](https://blog.csdn.net/z69183787/article/category/2175485)](https://blog.csdn.net/z69183787/article/category/2175483)

简要记录存储过程语法与Java程序的调用方式
　　一 存储过程
　　　　首先，我们建立一个简单的表进行存储过程的测试
```
create table 
xuesheng(id integer, xing_ming varchar2(**25**), yu_wen number, shu_xue number);
insert into xuesheng values(**1**,'zhangsan',**80**,**90**)
insert into xuesheng values(**2**,'lisi',**85**,**87**)
```
1）无返回值的存储过程
```
create or replace procedure xs_proc_no is
begin
  insert into xuesheng values (**3**, 'wangwu', **90**, **90**);
  commit;
end xs_proc_no;
```
2）有单个数据值返回的存储过程
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
create or replace procedure xs_proc(temp_name in varchar2,
                                    temp_num  out number) is
  num_1 number;
  num_2 number;
begin
  select yu_wen, shu_xue
    into num_1, num_2
    from xuesheng
   where xing_ming = temp_name;
  --dbms_output.put_line(num_1 + num_2);
  temp_num := num_1 + num_2;
end;
![复制代码](http://common.cnblogs.com/images/copycode.gif)
其中，以上两种与sql server基本类似，而对于返回数据集时，上述方法则不能满足我们的要求。在Oracle中，一般使用ref cursor来返回数据集。示例代码如下：
3）有返回值的存储过程(列表返回)
首先，建立我们自己的包。并定义包中的一个自定义ref cursor
```
create or replace package mypackage as
  type my_cursor is ref cursor;
end mypackage;
```
在定义了ref cursor后，可以书写我们的程序代码
```
create or replace procedure xs_proc_list(shuxue   in number,
                                         p_cursor out mypackage.my_cursor) is
begin
  open p_cursor for
    select * from xuesheng where shu_xue > shuxue;
end xs_proc_list;
```
 二、程序调用
在本节中，我们使用java语言调用存储过程。其中，关键是使用CallableStatement这个对象，代码如下：
[?](http://www.cnblogs.com/liliu/archive/2011/06/22/2087546.html#)
```java
String
 oracleDriverName =
```
```java
"oracle.jdbc.driver.OracleDriver"
```
```java
;
```
```java
```
```java
//
 以下使用的Test就是Oracle里的表空间
```
```java
```
```java
String
 oracleUrlToConnect =
```
```java
"jdbc:oracle:thin:@127.0.0.1:1521:orcl"
```
```java
;
```
```java
```
```java
Connection
 myConnection =
```
```java
null
```
```java
;
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
Class.forName(oracleDriverName);
```
```java
```
```java
}
```
```java
catch
```
```java
(ClassNotFoundException
 ex) {
```
```java
```
```java
ex.printStackTrace();
```
```java
```
```java
}
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
myConnection
 = DriverManager.getConnection(oracleUrlToConnect,
```
```java
```
```java
"xxxx"
```
```java
,
```
```java
"xxxx"
```
```java
);
```
```java
//此处为数据库用户名与密码
```
```java
```
```java
}
```
```java
catch
```
```java
(Exception
 ex) {
```
```java
```
```java
ex.printStackTrace();
```
```java
```
```java
}
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
```
```java
CallableStatement
 proc=
```
```java
null
```
```java
;
```
```java
```
```java
proc=myConnection.prepareCall(
```
```java
"{call
 xs_proc(?,?)}"
```
```java
);
```
```java
```
```java
proc.setString(
```
```java
1
```
```java
,
```
```java
"zhangsan"
```
```java
);
```
```java
```
```java
proc.registerOutParameter(
```
```java
2
```
```java
,
 Types.NUMERIC);
```
```java
```
```java
proc.execute();
```
```java
```
```java
String
 teststring=proc.getString(
```
```java
2
```
```java
);
```
```java
```
```java
System.out.println(teststring);
```
```java
```
```java
}
```
```java
catch
```
```java
(Exception
 ex) {
```
```java
```
```java
ex.printStackTrace();
```
```java
```
```java
}
```
对于列表返回值的存储过程，在上述代码中做简单修改。如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
CallableStatement proc=null;
            proc=myConnection.prepareCall("{call getdcsj(?,?,?,?,?)}");
            proc.setString(1, strDate);
            proc.setString(2, jzbh);
            proc.registerOutParameter(3, Types.NUMERIC);
            proc.registerOutParameter(4, OracleTypes.CURSOR);
            proc.registerOutParameter(5, OracleTypes.CURSOR);
            proc.execute();
            ResultSet rs=null;
            int total_number=proc.getInt(3);
            rs=(ResultSet)proc.getObject(4);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
上述存储过程修改完毕。另外，一个复杂的工程项目中的例子：查询一段数据中间隔不超过十分钟且连续超过100条的数据。即上述代码所调用的getdcsj存储过程
[?](http://www.cnblogs.com/liliu/archive/2011/06/22/2087546.html#)
`create``or``replace``procedure``getDcsj(var_flag    ``in``varchar2,`
`                                    ``var_jzbh    ``in``varchar2,`
`                                    ``number_total``out``number,`
`                                    ``var_cursor_a``out``mypackage.my_cursor,`
`                                    ``var_cursor_b``out``mypackage.my_cursor)``is`
`  `
```
total
 number;
```
`  ``cursor``cur``is`
`    ``select`
```
sj,
 flag
```
`      ``from``d_dcsj`
`     ``where`
```
jzbh
 = var_jzbh
```
`     ``order``by``sj``desc`
`       ``for``update``;`
`  ``last_time``date``;`
`begin`
`  ``for``cur1``in`
```
cur
 loop
```
`    `
```
if
 last_time
```
`is``null``or`
```
cur1.sj
 >= last_time - 10 / 60 / 24
```
`then`
`      ``update``d_dcsj``set`
```
flag
 = var_flag
```
`where``current``of``cur;`
`      `
```
last_time
 := cur1.sj;
```
`    ``else`
`      ``select``count``(*)``into``total``from``d_dcsj``where`
```
flag
 = var_flag;
```
`      ``dbms_output.put_line(total);`
`      `
```
if
 total < 100
```
`then`
`        ``update``d_dcsj``set`
```
flag
 =
```
`null``where`
```
flag
 = var_flag;
```
`        `
```
last_time
 :=
```
`null``;`
`        ``update``d_dcsj``set`
```
flag
 = var_flag
```
`where``current``of``cur;`
`      ``else`
`        ``open``var_cursor_a``for`
`          ``select``*`
`            ``from``d_dcsj`
`           ``where`
```
flag
 = var_flag
```
`             ``and`
```
jzbh
 = var_jzbh
```
`             ``and`
```
zh
 =
```
`'A'`
`           ``order``by``sj``desc``;`
`        `
```
number_total
 := total;
```
`        ``open``var_cursor_b``for`
`          ``select``*`
`            ``from``d_dcsj`
`           ``where`
```
flag
 = var_flag
```
`             ``and`
```
jzbh
 = var_jzbh
```
`             ``and`
```
zh
 =
```
`'B'`
`           ``order``by``sj``desc``;`
`        `
```
number_total
 := total;
```
`        ``exit;`
`      ``end``if;`
`    ``end``if;`
`  ``end``loop;`
`  ``select``count``(*)``into``total``from``d_dcsj``where`
```
flag
 = var_flag;
```
`  ``dbms_output.put_line(total);`
`  `
```
if
 total < 100
```
`then`
`    ``open``var_cursor_a``for`
`      ``select``*``from``d_dcsj``where`
```
zh
 =
```
`'C'``;`
`    ``open``var_cursor_b``for`
`      ``select``*``from``d_dcsj``where`
```
zh
 =
```
`'C'``;`
`  ``else`
`    ``open``var_cursor_a``for`
`      ``select``*`
`        ``from``d_dcsj`
`       ``where`
```
flag
 = var_flag
```
`         ``and`
```
jzbh
 = var_jzbh
```
`         ``and`
```
zh
 =
```
`'A'`
`       ``order``by``sj``desc``;`
`    `
```
number_total
 := total;
```
`    ``open``var_cursor_b``for`
`      ``select``*`
`        ``from``d_dcsj`
`       ``where`
```
flag
 = var_flag
```
`         ``and`
```
jzbh
 = var_jzbh
```
`         ``and`
```
zh
 =
```
`'B'`
`       ``order``by``sj``desc``;`
`    `
```
number_total
 := total;
```
`  ``end``if;`
`  ``commit``;`
`end``;`
```
```
