# 使用Oracle显式游标及for循环 - z69183787的专栏 - CSDN博客
2014年07月11日 17:09:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：841
使用隐式游标和显式游标：
1.查询返回单行记录时→隐式游标；
2.查询返回多行记录并逐行进行处理时→显式游标
--显示游标属性
declare
CURSOR cur_emp IS SELECT * FROM emp;
row_emp cur_emp%ROWTYPE;
BEGIN
OPEN cur_emp;
FETCH cur_emp INTO row_emp;
WHILE cur_emp%FOUND
LOOP
dbms_output.put_line(row_emp.empno||'----'||row_emp.ename);
FETCH cur_emp INTO row_emp;
END LOOP;
close cur_emp;
END;
--使用显式游标修改数据(给所有的部门经理加薪1000)
DECLARE
CURSOR emp_cur IS
SELECT empno,ename,sal FROM emp WHERE job='MANAGER' FOR UPDATE;
emp_row emp_cur%ROWTYPE;
BEGIN
OPEN emp_cur;
LOOP
FETCH emp_cur INTO emp_row;
IF emp_cur%NOTFOUND THEN
EXIT;
ELSE
UPDATE emp SET sal=sal+1000 WHERE CURRENT OF emp_cur;
END IF;
END LOOP;
COMMIT;
CLOSE emp_cur;
END;
·注意：1、如果游标打开之前或关闭之后，使用游标属性，Oracle会抛出一个INVALID_CURSOR错误（ORA-01001）；
        2、如果在第一次fetch后结果集是空的，%found=false，%NotFound=true，%ROWCOUNT=0；
        3、如果使用了BULK COLLECT，那么%ROWCOUNT的值可能不是0或1，实际上他返回的是提取到相关集合的行数。
--游标for循环（给所有的部门经理减薪1000）
DECLARE
CURSOR emp_cur IS
SELECT empno,ename,sal FROM emp WHERE job='MANAGER' FOR UPDATE;
BEGIN
FOR emp_row IN emp_cur
LOOP
UPDATE emp SET sal=sal-1000 WHERE CURRENT OF emp_cur;
END LOOP;
COMMIT;
END;
--我们可以看到游标FOR循环确实很好的简化了游标的开发，我们不在需要open、fetch和close语句，不在需要用%FOUND属性检测是否到最后一条记录，这一切Oracle隐式的帮我们完成了。
--给经理加薪5000，其他加薪1000
DECLARE
CURSOR emp_cur IS
SELECT * FROM emp FOR UPDATE;
BEGIN
 FOR emp_row IN emp_cur
 LOOP
 IF emp_row.job='MANAGER' THEN
 UPDATE emp SET sal=sal+5000 WHERE CURRENT OF emp_cur;
 ELSE
 UPDATE emp SET sal=sal+1000 WHERE CURRENT OF emp_cur;
 END IF;
 END LOOP;
 END;
