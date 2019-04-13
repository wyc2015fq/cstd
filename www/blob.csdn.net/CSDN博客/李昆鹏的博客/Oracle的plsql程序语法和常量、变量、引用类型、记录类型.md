
# Oracle的plsql程序语法和常量、变量、引用类型、记录类型 - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:42:40[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：528


-------------------------------------------------------Oracle的plsql程序语法和常量、变量、引用类型、记录类型-----------------------------------------------------
# pl/sql
什么是PL/SQL?
PL/SQL（Procedure Language/SQL）
PLSQL是Oracle对sql语言的过程化扩展，指在SQL命令语言中增加了过程处理语句（如分支、循环等），使SQL语言具有过程处理能力。把SQL语言的数据操纵能力与过程语言的数据处理能力结合起来，使得PLSQL面向过程但比过程语言简单、高效、灵活和实用。
范例1：为职工长工资，每人长10％的工资。
Update emp setsal=sal*1.1
范例2：例2: 按职工的职称长工资,总裁长1000元,经理长800元，其他人员长400元。
这样的需求我们就无法使用一条SQL来实现，需要借助其他程序来帮助完成，也可以使用pl/sql。
-----------------------------------------------------------------------------------------------------
### 程序语法：
declare
说明部分    （变量说明，游标申明，例外说明 〕
begin
语句序列   （DML语句〕…
exception
例外处理语句
End;
代码示例：
|declare
|begin
|dbms_output.put_line(|'hello world'|);
|end|;
|
Dbms代表数据库管理系统
--------------------------------------------------------------------------------------
### 常量和变量定义
在程序的声明阶段可以来定义常量和变量。
Ø  变量的基本类型就是oracle中的建表时字段的变量如char, varchar2, date,number, boolean, long)，
定义语法：varl  char(15);
Psal  number(9,2);
说明变量名、数据类型和长度后用分号结束说明语句。
常量定义：married  boolean:=true
Ø  引用变量
Myname emp.ename%type;
引用型变量，即my_name的类型与emp表中ename列的类型一样
在sql中使用into来赋值
declare
emprec emp.ename%type;
begin
selectt.enameintoemprecfromemp twheret.empno =7369;
dbms_output.put_line(emprec);
end;
Ø  记录型变量
Emprec  emp%rowtype
记录变量分量的引用
emp_rec.ename:='ADAMS';
declare
p emp%rowtype;
begin
select*intopfromemp twheret.empno =7369;
dbms_output.put_line(p.ename ||' '|| p.sal);
end;
代码示例：
|--基本数据类型变量和常量
|declare
|pname     varchar2(10);      --变量定义
|page      number(3) :=20;    --定义变量并且赋值
|begin
|pname := 'zhangsan';         --给变量赋值
|dbms_output.put_line(pname); --输出变量
|dbms_output.put_line(page);  --输出变量
|end;
|--引用类型变量
|declare
|pname     myemp.ename%type; --定义引用类型
|begin
|select t.ename into pname from myemp t where t.empno =7369; --把myemp表中查询的数据赋值给引用类型变量
|dbms_output.put_line(pname);
|end;
|--记录类型变量,可以对应java中对象类型的变量
|declare
|prec     myemp%rowtype; --定义记录类型变量
|begin
|select * into prec from myemp t where t.empno =7369; --把myemp表中查询的一行数据赋值给记录类型变量
|dbms_output.put_line(prec.ename || '    ' ||  prec.sal);
|end;
|


