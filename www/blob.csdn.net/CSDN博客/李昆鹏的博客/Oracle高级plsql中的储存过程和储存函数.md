
# Oracle高级plsql中的储存过程和储存函数 - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:52:05[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：131标签：[Oracle																](https://so.csdn.net/so/search/s.do?q=Oracle&t=blog)个人分类：[Oracle																](https://blog.csdn.net/weixin_41547486/article/category/7679485)


----------------------------------------Oracle高级plsql中的储存过程和储存函数------------------------------------------------
## 存储过程
存储过程（Stored Procedure）是在大型数据库系统中，一组为了完成特定功能的SQL 语句集，经编译后存储在数据库中，用户通过指定存储过程的名字并给出参数（如果该存储过程带有参数）来执行它。存储过程是数据库中的一个重要对象，任何一个设计良好的数据库应用程序都应该用到存储过程。
创建存储过程语法：
**create[or replace] PROCEDURE 过程名[(参数名in/out 数据类型)]**
**AS**
**begin**
**PLSQL子程序体；**
**End;**
**或者**
**create[or replace] PROCEDURE 过程名[(参数名in/out 数据类型)]**
**is**
**begin**
**PLSQL子程序体；**
**End  过程名;**
范例：创建一个输出helloword的存储过程
createorreplaceprocedurehelloworldis
begin
dbms_output.put_line('helloworld');
endhelloworld;
调用存储过程
在plsql中调用存储过程
begin
*-- Call the procedure*
helloworld;
end;
范例2：给指定的员工涨100工资，并打印出涨前和涨后的工资
分析：我们需要使用带有参数的存储过程
createorreplaceprocedureaddSal1(enoinnumber)is
pemp myemp%rowtype;
begin
select*intopempfrommyempwhereempno = eno;
updatemyempsetsal = sal +100whereempno = eno;
dbms_output.put_line('涨工资前'|| pemp.sal ||'涨工资后'|| (pemp.sal +100));
endaddSal1;
调用
begin
*-- Call the procedure*
addsal1(eno =>7902);
commit;
end;
代码示例：
|--|使用储存过程输出|hellowrold
|create or replace  procedure hellowrold
|as
|begin
|dbms_output.put_line('hellowrold');
|end;
|--|给指定的员工涨|100|工资，并打印出涨前和涨后的工资
|create or replace  procedure addsal(pno in myemp.empno%type) as
|--|在此处可以声明变量
|prec myemp%rowtype;
|begin
|select * into prec from myemp t where  t.empno = pno;
|--|事务的提交要在调用端来做
|update myemp t set t.sal = t.sal + 100  where t.empno = pno;
|dbms_output.put_line(prec.ename || '|涨之前的工资是：|' || prec.sal ||
|'|涨之后的工资是|' || (prec.sal + 100));
|end;
|--|计算指定员工的年薪
|create or replace  procedure countYSal(pno     in  emp.empno%type,
|yearSal  out number) as
|--psal   emp.sal%type;
|--pcomm emp.comm%type;
|begin
|--|多个值赋值是按照先后顺序来赋值的
|--select t.sal,t.comm into psal,pcomm from  emp t where t.empno = pno;
|--yearSal := psal * 12 + nvl(pcomm, 0);
|select t.sal * 12 + nvl(t.comm, 0)
|into yearSal
|from emp t
|where t.empno = pno;
|end;
|--|通过|pl/sql|调用存储过程
|declare
|ysal number;
|begin
|countysal(7369, ySal);
|dbms_output.put_line(ysal);
|end;
|
---------------------------------------------------------------------------------------------------
## 存储函数
**create or replace function 函数名(Name in type,Name in type, ...) return 数据类型 is**
**结果变量 数据类型;**
**begin**
**return(结果变量);**
**end函数名;**
存储过程和存储函数的区别
一般来讲，过程和函数的区别在于函数可以有一个返回值；而过程没有返回值。
但过程和函数都可以通过out指定一个或多个输出参数。我们可以利用out参数，在过程和函数中实现返回多个值。
范例：使用存储函数来查询指定员工的年薪
createorreplacefunctionempincome(enoinemp.empno%type)returnnumberis
psal  emp.sal%type;
pcomm emp.comm%type;
begin
selectt.salintopsalfromemp twheret.empno = eno;
returnpsal *12+ nvl(pcomm,0);
end;
使用存储过程来替换上面的例子
createorreplaceprocedureempincomep(enoinemp.empno%type, incomeoutnumber)is
psal emp.sal%type;
pcomm emp.comm%type;
begin
selectt.sal, t.commintopsal, pcommfromemp twheret.empno = eno;
income :=psal*12+nvl(pcomm,0);
endempincomep;
调用：
declare
incomenumber;
begin
empincomep(7369, income);
dbms_output.put_line(income);
代码示例：
|--|计算指定员工的年薪
|create or replace  function countysalf(pno in emp.empno%type) return number is
|result number;
|begin
|select t.sal * 12 + nvl(t.comm, 0)
|into result
|from emp t
|where t.empno = pno;
|return(result);
|end countysalf;
|
end;
使用java代码调用存储过程和函数
|/*Class.forName("oracle.jdbc.OracleDriver");
|Connection|conn|= null;
|conn|=  DriverManager.getConnection("jdbc:oracle:thin:@127.0.0.1:1521:|orcl|",
|"|scott|",  "tiger");
|CallableStatement call =  conn.prepareCall("{call|countyearsal|(?,?)}");
|call.setInt(1, 7369);
|call.registerOutParameter(2,  OracleTypes.NUMBER);
|call.execute();
|int|sum = call.getInt(2);
|System.out.println(sum);*/
|Class.|forName|(|"oracle.jdbc.OracleDriver"|);
|Connection conn =|null|;
|conn = DriverManager.|getConnection|(|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|,
|"scott"|,|"tiger"|);
|CallableStatement call =  conn.prepareCall(|"{?= call  countyearsal1(?)}"|);
|call.registerOutParameter(1,  OracleTypes.|NUMBER|);
|call.setInt(2, 7369);
|call.execute();
|int|sum = call.getInt(1);
|System.|out|.println(sum);
|
代码示例：
|package|cn.itcast.procedure;
|import|java.sql.CallableStatement;
|import|java.sql.Connection;
|import|java.sql.DriverManager;
|import|java.sql.SQLException;
|import|oracle.jdbc.internal.OracleTypes;
|public|class|TestProFun {
|public|static|void|main(String[]|args|) {
|testPro|();
|testFun|();
|}
|/**
|*|存储过程的调用
|*/
|public|static|void|testPro() {
|String|driver|=|"oracle.jdbc.OracleDriver"|;
|String|connStr|=|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|;
|Connection|conn|=|null|;
|CallableStatement|cs|=|null|;
|try|{
|Class.|forName|(|driver|);
|conn|= DriverManager.|getConnection|(|connStr|,|"scott"|,|"tiger"|);
|//|获得|CallableStatement|对象
|cs|=|conn|.prepareCall(|"{call countysal(?,?)}"|);
|cs|.setInt(1, 7369);
|//|注册|Oracle|的输出参数类型
|cs|.registerOutParameter(2, OracleTypes.|NUMBER|);
|//|执行存储过程
|cs|.execute();
|//|获得返回值
|int|ysal|=|cs|.getInt(2);
|System.|out|.println(|ysal|);
|}|catch|(Exception|e|) {
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|cs|!=|null|)
|cs|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|/**
|*|存储函数的调用
|*/
|public|static|void|testFun() {
|String|driver|=|"oracle.jdbc.OracleDriver"|;
|String|connStr|=|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|;
|Connection|conn|=|null|;
|CallableStatement|cs|=|null|;
|try|{
|Class.|forName|(|driver|);
|conn|= DriverManager.|getConnection|(|connStr|,|"scott"|,|"tiger"|);
|//|获得|CallableStatement|对象
|cs|=|conn|.prepareCall(|"{? = call countysalf(?)}"|);
|cs|.setInt(2, 7369);
|//|注册|Oracle|的输出参数类型
|cs|.registerOutParameter(1, OracleTypes.|NUMBER|);
|//|执行存储过程
|cs|.execute();
|//|获得返回值
|int|y sal|=|cs|.getInt(1);
|System.|out|.println(|ysal|);
|}|catch|(Exception|e|) {
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|cs|!=|null|)
|cs|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|}
|


