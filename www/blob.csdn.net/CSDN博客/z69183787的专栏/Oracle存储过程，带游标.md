# Oracle存储过程，带游标 - z69183787的专栏 - CSDN博客
2012年12月10日 16:34:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：913
```
create or replace procedure aa is
 bb number;
cursor cur is  
select t.id,v.description from t_user t ,v_userdep v where t.removed=0 and t.login_name = substr(v.login_name,1,12);
begin
for c in cur loop
      bb := 0;
      select count(*) into bb from t_user_temp s where s.id = c.id;
      dbms_output.put_line(bb);
      if(bb<=0) then
          insert into t_user_temp s (s.id,s.name) values(c.id,c.description);
          commit;
       else 
          update t_user_temp s set s.name = s.name || ',' || c.description where s.id=c.id;
          end if;
        end loop;
end aa;
```
```
CREATE OR REPLACE PROCEDURE F_holiday is
  cursor mycursor is select t.hol_days,t.hol_days_left,t.hol_days_wait,t.hol_id,t.remark from t_hol_holiday t where t.removed=0 ;
   t_mycursor mycursor%ROWTYPE;
begin
 -- strSql:='select t.hol_days,t.hol_days_left,t.hol_days_wait,t.hol_id,t.remark from t_hol_holiday t where t.removed=0 ';
  open mycursor;
     --FETCH v_mysursor INTO holDays,holDaysLeft,holDaysWait,holId,remark;
  Loop
     FETCH mycursor INTO t_mycursor;
     exit when mycursor%NOTFOUND;
    -- WHILE v_mysursor%FOUND LOOP
        insert into t_hol_holiday (ID,hol_year,hol_days,hol_days_left,hol_days_wait,removed,operator,operate_time,hol_id,hol_state,remark) values (sys_guid(),'2011',t_mycursor.hol_days,t_mycursor.hol_days,t_mycursor.hol_days,'0','1156',to_char(sysdate,'yyyy-mm-dd hh24:mi:ss'),t_mycursor.hol_id,'725',t_mycursor.remark);
  End LOOP;
  close mycursor; --关闭游标
   EXCEPTION
     WHEN NO_DATA_FOUND THEN
      null;
     WHEN OTHERS THEN
       -- Consider logging the error and then re-raise
       RAISE;
end F_holiday;
```
```
CREATE OR REPLACE PROCEDURE rrrr is
  cursor mycursor is select t.mc_dx from t_func_xt_ywdxsm t ;
   t_mycursor mycursor%ROWTYPE;
begin
 -- strSql:='select t.hol_days,t.hol_days_left,t.hol_days_wait,t.hol_id,t.remark from t_hol_holiday t where t.removed=0 ';
  open mycursor;
     --FETCH v_mysursor INTO holDays,holDaysLeft,holDaysWait,holId,remark;
  Loop
     FETCH mycursor INTO t_mycursor;
     exit when mycursor%NOTFOUND;
    -- WHILE v_mysursor%FOUND LOOP
        insert into t_rectification_user_conf (reserve1,removed) values(t_mycursor.mc_dx,0);
  End LOOP;
  close mycursor; --关闭游标
   EXCEPTION
     WHEN NO_DATA_FOUND THEN
      null;
     WHEN OTHERS THEN
       -- Consider logging the error and then re-raise
       RAISE;
end rrrr;
```
test 窗口 在下方输入 测试数据 执行后即可
command 窗口中 如下执行
```
set serveroutput on
SQL> declare
  2  i number;
  3  begin
  4  pro_test(1,i);
  5  dbms_output.put_line(i);
  6  end;
  7  /
```
```
create or replace procedure testt is
cursor cur is  
  select *  from c_contract c where c.removed = 0 and c.contract_price is not null;
   n number;
begin
 begin
 n:=1;
   dbms_output.put_line (n);
       for c in cur loop
          dbms_output.put_line (c.id);
           n := to_number (c.contract_price);
          
          end loop;
  end;
end testt;
```
