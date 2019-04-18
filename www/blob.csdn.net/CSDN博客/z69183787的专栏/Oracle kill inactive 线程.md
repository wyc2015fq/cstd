# Oracle kill inactive 线程 - z69183787的专栏 - CSDN博客
2015年04月20日 09:46:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：754
```
CREATE OR REPLACE PROCEDURE "KILL_SESSION" AS
        v_sid number;
        v_serial number;
                killer varchar2(1000);
        CURSOR cursor_session_info is select sid,serial# from v$session where type!='BACKGROUND' and status='INACTIVE' and last_call_et>2700 and username='STPT' and machine='wonders_svr2';
BEGIN
        open cursor_session_info;
        loop
                fetch cursor_session_info into v_sid,v_serial;
                exit when cursor_session_info%notfound;
                                killer:='alter system disconnect session '''||v_sid||','||v_serial||''' post_transaction immediate';
                                                                execute immediate killer;
                                        end loop;
                dbms_output.PUT_LINE(cursor_session_info%rowcount||' users with idle_time>2700s have been killed!');
                close cursor_session_info;
END;
```
```
begin
  -- Call the procedure
  kill_session;
end;
```
使用DBA用户 创建存储过程 并 执行，否则无法在procedure中访问 v$session
select machine,username,count(*) from gv$session group by machine,username
