# DB2数据库使用存储过程详解 - fanyun的博客 - CSDN博客
2016年11月30日 09:16:50[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：18360
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
**       存储过程**（Stored Procedure）是在大型[数据库系统](http://baike.so.com/doc/3054063-3219400.html)中，一组为了完成特定功能的SQL语句集，经编译后存储在数据库中，用户通过指定存储过程的名字并给出参数（如果该存储过程带有参数）来执行它。
**  主要特点：**
类语言主要提供以下功能，让用户可以设计出符合引用需求的程序：
1)、[变量](http://baike.so.com/doc/2977483-3140702.html)说明
2)、ANSI（美国国家标准化组织）兼容的SQL命令(如Select,Update….)
3)、一般流程控制命令(if…else…、while….)
4)、内部函数
**    存储过程的优点：**
(1) .存储过程的能力大大增强了SQL语言的功能和灵活性。存储过程可以用流控制语句编写，有很强的灵活性，可以完成复杂的判断和较复杂的运算。
(2) .可保证数据的安全性和完整性。
(3) .通过存储过程可以使没有权限的用户在控制之下间接地存取数据库，从而保证数据的安全。
(4) .通过存储过程可以使相关的动作在一起发生，从而可以维护数据库的完整性。
(5) .在运行存储过程前，数据库已对其进行了语法和句法分析，并给出了优化执行方案。这种已经编译好的过程可极大地改善SQL语句的性能。由于执行SQL语句的大部分工作已经完成，所以存储过程能以极快的速度执行。
(6) .可以降低网络的通信量。
(7) .使体现企业规则的运算程序放入[数据库服务器](http://baike.so.com/doc/5571843-5787017.html)中，以便：
(8) .集中控制。
(9) .当企业规则发生变化时在[服务器](http://baike.so.com/doc/4487696-4696885.html)中改变存储过程即可，无须修改任何[应用程序](http://baike.so.com/doc/3417785-3597266.html)。企业规则的特点是要经常变化，如果把体现企业规则的运算程序放入应用程序中，则当企业规则发生变化时，就需要修改应用程序工作量非常之大（修改、发行和安装应用程序）。如果把体现企业规则的运算放入存储过程中，则当企业规则发生变化时，只要修改存储过程就可以了，应用程序无须任何变化。
简单讲：
1.存储过程只在创造时进行编译，以后每次执行存储过程都不需再重新编译，而一般SQL语句每执行一次就编译一次,所以使用存储过程可提高数据库执行速度。
2.当对数据库进行复杂操作时(如对多个表进行Update,Insert,Query,Delete时)，可将此复杂操作用存储过程封装起来与数据库提供的事务处理结合一起使用。
3.存储过程可以重复使用,可减少数据库开发人员的工作量
4.安全性高,可设定只有某些用户才具有对指定存储过程的使用权
有一点需要注意的是，一些网上盛传的所谓的存储过程要比sql语句执行更快的说法，实际上是个误解，并没有根据，包括微软内部的人也不认可这一点，所以不能作为正式的优点，希望大家能够认识到这一点。
用户创建的存储过程是由用户创建并完成某一特定功能的存储过程，事实上一般所说的存储过程就是指本地存储过程。
### 临时存储过程
分为两种存储过程：
一是本地临时存储过程，以井字号(#)作为其名称的第一个字符，则该存储过程将成为一个存放在tempdb数据库中的本地临时存储过程，且只有创建它的用户才能执行它;
二是全局临时存储过程，以两个井字号(##)号开始，则该存储过程将成为一个存储在tempdb数据库中的全局临时存储过程，全局临时存储过程一旦创建，以后连接到服务器的任意用户都可以执行它，而且不需要特定的权限。
### 远程存储过程
在SQLServer2005中，远程存储过程(RemoteStored Procedures)是位于远程服务器上的存储过程，通常可以使用分布式查询和EXECUTE命令执行一个远程存储过程。
### 扩展存储过程
扩展存储过程(ExtendedStored Procedures)是用户可以使用外部程序语言编写的存储过程，而且扩展存储过程的名称通常以xp_开头。
**一.创建存储过程**
createprocedure sp_name
@[参数名] [类型],@[参数名]
 [类型]
as
begin
.........
end
以上格式还可以简写成：
createproc sp_name
@[参数名] [类型],@[参数名]
 [类型]
as
begin
.........
end
/*注：“sp_name”为需要创建的存储过程的名字，该名字不可以以阿拉伯数字开头*/
**二.调用存储过程**
1.基本语法：execsp_name [参数名]
**三.删除存储过程**
1.基本语法：
dropprocedure sp_name
2.注意事项
(1)不能在一个存储过程中删除另一个存储过程，只能调用另一个存储过程
**四.其他常用命令**
1.showprocedure status
显示数据库中所有存储的存储过程基本信息，包括所属数据库，存储过程名称，创建时间等
2.showcreate procedure sp_name
显示某一个mysql存储过程的详细信息
3、execsp_helptext sp_name
显示你这个sp_name这个对象创建文本
**DB2存储过程实例：**
        客户在进行短信服务这个业务申请时，需要填写一些基本信息，然后根据这些信息判断这个用户是否已经存在于业务系统中。因为网上服务和业务系统两个项目物理隔离，而且网上数据库保存的客户信息不全，所以判断需要把数据交换到业务系统，在业务系统中判断。
**解决方式是通过存储过程：**
**       功能说明：**
**       调用存储过程时会传入一些值（IN输入参数），然后根据传入的值查询数据库（select语句），根据查询结果执行操作（添加、删除、更新）****有两种方式执行写好的存储过程：**
```cpp
DROP PROCEDURE "PLName"
@
CREATE PROCEDURE "PLName"(--存储过程名字
IN IN_ID BIGINT ,                    --以下全是输入参数
IN IN_ENTNAME VARCHAR(200) ,
IN IN_REGNO VARCHAR(50),
IN IN_PASSWORD VARCHAR(20),
IN IN_LEREP VARCHAR(300),
IN IN_CERTYPE CHARACTER(1),
IN IN_CERNO VARCHAR(50),
IN IN_LINKMAN VARCHAR(50),
IN IN_SEX CHARACTER(1),
IN IN_MOBTEL VARCHAR(30),
IN IN_REQDATE TIMESTAMP,
IN IN_REMITEM VARCHAR(300),
IN IN_STATE CHARACTER(1),
IN IN_TIMESTAMP TIMESTAMP
)
BEGIN
    
   declare V_RESULT  BIGINT;     --声明变量
   DELETE FROM  TableNameA WHERE ID = IN_ID;
   SET V_RESULT = NULL;          --为变量赋值
  --检查用户输入的信息是否合法
  select b.id INTO V_RESULT  from TableNameB b,TableNameC c where 正常的判断条件  
  if(V_RESULT IS NOT NULL)  then ---如果合法，执行下面的insert语句
   INSERT INTO TableNameA(ID,ENTNAME,REGNO,PASSWORD,LEREP,CERTYPE,CERNO,LINKMAN,SEX,MOBTEL,REQDATE,REMITEM,STATE,TIMESTAMP)
   VALUES(IN_ID,IN_ENTNAME,IN_REGNO,IN_PASSWORD,IN_LEREP,IN_CERTYPE,IN_CERNO,IN_LINKMAN,IN_SEX,IN_MOBTEL,IN_REQDATE,IN_REMITEM,IN_STATE,IN_TIMESTAMP);
  end if;
    commit;
END
@
```
   1.拷贝到DB2客户端工具中直接执行
**特别注意**：执行时将改成@，之前很多错误都和它有关，比如：“该命令被当作 SQL语句来处理，因为它不是有效的命令行处理器命令”.
   2.将上面的语句保存为test.db2文件放到任意目录下（比如D盘根目录），然后在cmd输入db2cmd 然后输入db2 -td@ -vf D：\test.db2即可执行后就可以测试存储过程写的是否正确.
**直接写sql：**
```cpp
call PLName(存储过程名字) （IN_ID,IN_ENTNAME,IN_REGNO,IN_PASSWORD,IN_LEREP,IN_CERTYPE,IN_CERNO,IN_LINKMAN,IN_SEX,IN_MOBTEL,IN_REQDATE,IN_REMITEM,IN_STATE,IN_TIMESTAMP对应的值）
```
