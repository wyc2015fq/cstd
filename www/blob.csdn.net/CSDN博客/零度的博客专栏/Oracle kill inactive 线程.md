# Oracle kill inactive 线程 - 零度的博客专栏 - CSDN博客
2016年04月27日 15:25:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：555标签：[oracle](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[数据库](https://blog.csdn.net/zmx729618/article/category/3108441)

**[sql]**[view plain](http://blog.csdn.net/z69183787/article/details/45146497#)[copy](http://blog.csdn.net/z69183787/article/details/45146497#)[print](http://blog.csdn.net/z69183787/article/details/45146497#)[?](http://blog.csdn.net/z69183787/article/details/45146497#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/648109/fork)
- CREATEORREPLACEPROCEDURE"KILL_SESSION"AS
-         v_sid number;  
-         v_serial number;  
-                 killer varchar2(1000);  
- CURSOR cursor_session_info isselect sid,serial# from v$session where type!='BACKGROUND'and status='INACTIVE'and last_call_et>2700 and username='STPT'and machine='wonders_svr2';  
- BEGIN
- open cursor_session_info;  
-         loop  
- fetch cursor_session_info into v_sid,v_serial;  
-                 exit when cursor_session_info%notfound;  
- 
-                                 killer:='alter system disconnect session '''||v_sid||','||v_serial||''' post_transaction immediate';  
- execute immediate killer;  
- end loop;  
-                 dbms_output.PUT_LINE(cursor_session_info%rowcount||' users with idle_time>2700s have been killed!');  
- close cursor_session_info;  
- END;  
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
**[sql]**[view plain](http://blog.csdn.net/z69183787/article/details/45146497#)[copy](http://blog.csdn.net/z69183787/article/details/45146497#)[print](http://blog.csdn.net/z69183787/article/details/45146497#)[?](http://blog.csdn.net/z69183787/article/details/45146497#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/648109/fork)
- begin
- -- Call the procedure
-   kill_session;  
- end;  
```
begin
  -- Call the procedure
  kill_session;
end;
```
使用DBA用户 创建存储过程 并 执行，否则无法在procedure中访问 v$session
select machine,username,count(*) from gv$session group by machine,username
