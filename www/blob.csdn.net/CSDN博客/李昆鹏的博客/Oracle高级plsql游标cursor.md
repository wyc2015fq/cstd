
# Oracle高级plsql游标cursor - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:45:37[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：145


-----------------------------------------------Oracle高级plsql游标cursor---------------------------------------------
### 游标Cursor
在写java程序中有集合的概念，那么在pl/sql中也会用到多条记录，这时候我们就要用到游标，游标可以存储查询返回的多条数据。
语法：
CURSOR  游标名  [ (参数名  数据类型,参数名 数据类型,...)]  IS  SELECT  语句;
例如：cursorc1isselectenamefromemp;
游标的使用步骤：
Ø  打开游标：open c1;(打开游标执行查询)
Ø  取一行游标的值：fetch c1 into pjob;(取一行到变量中)
Ø  关闭游标：close  c1;(关闭游标释放资源)
Ø  游标的结束方式   exit when c1%notfound
Ø注意： 上面的pjob必须与emp表中的job列类型一致：
定义：pjob emp.empjob%type;
范例1：使用游标方式输出emp表中的员工编号和姓名
declare
cursorpcis
select*fromemp;
pemp emp%rowtype;
begin
openpc;
loop
fetchpc
intopemp;
exitwhenpc%notfound;
dbms_output.put_line(pemp.empno||' '|| pemp.ename);
endloop;
closepc;
end;
范例2：按员工的工种长工资,总裁1000元，经理长800元其，他人员长400元。
declare
cursorpcis
select*frommyemp;
addsal myemp.sal%type;
pemp   myemp%rowtype;
begin
openpc;
loop
fetchpc
intopemp;
exitwhenpc%notfound;
ifpemp.job ='PRESIDENT'then
addsal :=1000;
elsifpemp.job ='MANAGER'then
addsal :=800;
else
addsal :=400;
endif;
updatemyemp tsett.sal = t.sal + addsalwheret.empno =pemp.empno;
endloop;
closepc;
end;
范例3：写一段PL/SQL程序，为部门号为10的员工涨工资。
declare
cursorpc(dno myemp.deptno%type)is
selectempnofrommyempwheredeptno = dno;
pno myemp.empno%type;
begin
openpc(20);
loop
fetchpc
intopno;
exitwhenpc%notfound;
updatemyemp tsett.sal = t.sal +1000wheret.empno = pno;
endloop;
closepc;
end;
代码示例：
|--使用游标方式输出emp表中的员工编号和姓名
|declare
|prec    emp%rowtype;  --定义记录型变量
|cursor  c1 is select * from emp;  --定义游标
|begin
|open c1; --打开游标
|loop
|fetch c1 into prec; --从游标中取值，取值后游标会自动向下移一步
|exit when c1%notfound; --判断循环结束条件
|dbms_output.put_line(prec.empno || '    ' || prec.ename);
|end loop; --结束循环
|close         c1; --关闭游标
|end;
|--按员工的工种长工资,总裁1000元，经理长800元其，其他人员长400元。
|declare
|prec myemp%rowtype;
|cursor c1 is
|select * from myemp;
|addsal number(4);
|begin
|open c1; --打开游标
|loop
|fetch c1
|into prec;
|exit when c1%notfound;
|if prec.job = 'PRESIDENT' then
|addsal := 1000;
|elsif prec.job = 'MANAGER' then
|addsal := 800;
|else
|addsal := 400;
|end if;
|update myemp t set t.sal = t.sal + addsal where t.empno = prec.empno;
|end loop;
|close c1; --关闭游标
|commit; --提交事务
|end;
|select * from myemp;
|--写一段PL/SQL程序，为部门号为10的员工涨工资。
|declare
|--定义带有参数的游标
|cursor c1(dno myemp.deptno%type) is
|select * from myemp t where t.deptno = dno;
|prec myemp%rowtype;
|begin
|open c1(10); --打开带有参数的游标，除此处需要声明带参数，其他地方都不用
|loop
|fetch c1
|into prec;
|exit when c1%notfound;
|update myemp t set t.sal = t.sal + 1000 where t.empno = prec.empno;
|end loop;
|close c1; --关闭游标
|commit;
|end;
|


