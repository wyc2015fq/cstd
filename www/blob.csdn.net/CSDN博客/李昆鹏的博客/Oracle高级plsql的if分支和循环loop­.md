
# Oracle高级plsql的if分支和循环loop­ - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:44:08[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：235


------------------------------------------------Oracle高级plsql的if分支和循环loop­----------------------------------------------------
**If语句**
语法1：
**IF   条件  THEN 语句1;**
**语句2;**
**END IF;**
语法2：
**IF  条件  THEN  语句序列1；**
**ELSE  语句序列 2；**
**END  IF；**
语法3：
**IF   条件  THEN 语句;**
**ELSIF  语句  THEN  语句;**
**ELSE  语句;**
**END  IF;**
范例1：如果从控制台输入1则输出我是1
declare
pnumnumber:= &num;
begin
ifpnum =1then
dbms_output.put_line('我是1');
endif;
end;
范例2：如果从控制台输入1则输出我是1否则输出我不是1
declare
mynumnumber:= &num;
begin
ifmynum =1then
dbms_output.put_line('我是1');
else
dbms_output.put_line('我不是1');
endif;
end;
范例3:判断人的不同年龄段18岁以下是未成年人，18岁以上40以下是成年人，40以上是老年人
declare
mynumnumber:= &num;
begin
ifmynum <18then
dbms_output.put_line('未成年人');
elsifmynum >=18andmynum <40then
dbms_output.put_line('中年人');
elsifmynum >=40then
dbms_output.put_line('老年人');
endif;
end;
代码示例：
|declare
|pno number(4) := &num; --定义手动赋值的变量
|begin
|if pno < 5 then
|dbms_output.put_line('pno小于5');
|end if;
|end;
|declare
|pno number(4) := &num; --定义手动赋值的变量
|begin
|if pno = 5 then
|dbms_output.put_line('pno等于1');
|else
|dbms_output.put_line('我不是1');
|end  if;
|end;
|declare
|pno number(4) :=&num; --定义手动赋值的变量
|begin
|if pno = 1 then
|dbms_output.put_line('pno等于1');
|elsif pno = 2 then
|dbms_output.put_line('pno等于2');
|else
|dbms_output.put_line('其他');
|end if;
|end;
|
-------------------------------------------------------------------------------------------------------------
### .LOOP循环语句
其中语法2比较常用
语法1：
**WHILE  total  <= 25000 LOOP**
**.. .**
**total : = total + salary;**
**END  LOOP;**
语法2：
**Loop**
**EXIT[when****条件];**
**……**
**End loop**
语法3：
**FOR   I  IN   1 . . 3    LOOP**
**语句序列 ;**
**END    LOOP ;**
范例:使用语法1输出1到10的数字
declare
stepnumber:=1;
begin
whilestep <=10loop
dbms_output.put_line(step);
step := step +1;
endloop;
end;
范例:使用语法2输出1到10的数字
declare
stepnumber:=1;
begin
loop
exitwhenstep >10;
dbms_output.put_line(step);
step := step +1;
endloop;
end;
范例:使用语法3输出1到10的数字
declare
stepnumber:=1;
begin
forstepin1..10loop
dbms_output.put_line(step);
endloop;
end;
代码示例：
|declare
|total   number(4) :=0;
|begin
|while total <= 10 loop
|total := total +1;
|dbms_output.put_line(total);
|end loop;
|end;
|--最常用循环的方法
|declare
|total number(4) := 0;
|begin
|loop
|exit when total = 100;
|total := total + 1;
|dbms_output.put_line(total);
|end loop;
|end;
|--适合于连续的数值遍历
|declare
|total number(4) := 0;
|begin
|for total in 1 .. 100 loop
|dbms_output.put_line(total);
|end loop;
|end;
|


