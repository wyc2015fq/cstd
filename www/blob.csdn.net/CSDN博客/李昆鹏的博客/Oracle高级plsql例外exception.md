
# Oracle高级plsql例外exception - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:46:49[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：495


------------------------------------------------Oracle高级plsql例外exception---------------------------------------------
例外是程序设计语言提供的一种功能，用来增强程序的健壮性和容错性。
系统定义例外
no_data_found    (没有找到数据)
too_many_rows          (select …into语句匹配多个行)
zero_divide   ( 被零除)
value_error     (算术或转换错误)
timeout_on_resource      (在等待资源时发生超时)
范例1：写出被0除的例外的plsql程序
declare
pnumnumber;
begin
pnum :=1/0;
exception
whenzero_dividethen
dbms_output.put_line('被0除');
whenvalue_errorthen
dbms_output.put_line('数值转换错误');
whenothersthen
dbms_output.put_line('其他错误');
end;
用户也可以自定义例外，在声明中来定义例外
DECLARE
My_job   char(10);
v_sal   emp.sal%type;
No_data   exception;
cursor c1 is select distinct job from emp    order by job;
如果遇到异常我们要抛出raise no_data;
范例：查询部门编号是50的员工
declare
no_emp_foundexception;
cursorpempis
selectt.enamefromemp twheret.deptno =50;
pename emp.ename%type;
begin
openpemp;
fetchpemp
intopename;
ifpemp%notfoundthen
raiseno_emp_found;
endif;
closepemp;
exception
whenno_emp_foundthen
dbms_output.put_line('没有找到员工');
whenothersthen
dbms_output.put_line('其他错误');
end;
代码示例：
|declare
|pnum    number(4) :=5;
|begin
|pnum := pnum/0; --发生异常
|exception         --处理异常
|when zero_divide then
|dbms_output.put_line('被0除异常');
|when value_error then
|dbms_output.put_line('数值转换异常');
|when others then
|dbms_output.put_line('其他异常');
|end;
|--查询部门编号为50的员工
|declare
|prec emp%rowtype;
|cursor c1 is
|select * from emp t where t.empno = 50;
|no_data exception; --自定义异常
|begin
|open c1;
|loop
|fetch c1
|into prec;
|if c1%notfound then
|raise no_data; --抛出异常
|end if;
|end loop;
|close c1;
|exception
|when no_data then
|--处理异常
|dbms_output.put_line('没有员工');
|when others then
|dbms_output.put_line('其他异常');
|end;
|

