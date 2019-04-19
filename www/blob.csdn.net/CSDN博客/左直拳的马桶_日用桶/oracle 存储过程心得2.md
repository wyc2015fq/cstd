# oracle 存储过程心得2 - 左直拳的马桶_日用桶 - CSDN博客
2017年07月09日 12:39:15[左直拳](https://me.csdn.net/leftfist)阅读数：418标签：[oracle																[procedure																[存储过程](https://so.csdn.net/so/search/s.do?q=存储过程&t=blog)](https://so.csdn.net/so/search/s.do?q=procedure&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[oracle](https://blog.csdn.net/leftfist/article/category/5734215)
1、退出存储过程 
return
```
if old_save_time = new_save_time then--没有最新数据，退出
    insert into hy_data_handle_mark(id,save_time,mark_start,mark_finish) values(seq_hy_data_handle_mark.nextval,new_save_time,sysdate,sysdate);
    commit;
    return;
  end if;
```
2、退出循环 
exit
```
begin
    open c_id;
    LOOP
      FETCH c_id INTO v_id;
      EXIT WHEN c_id%NOTFOUND;
      insert into tmp_cursor(id) values(v_id);
    END LOOP;  
    CLOSE c_id;
  end;
```
3、退出当前循环 
continue
```
LOOP
if min_time is null then continue;end if;--指定时间段内已处理
END LOOP
```
4、返回多个值 
返回一个值用函数，超出一个，一般用存储过程
```
create or replace procedure PRO_OFFSET_TIME(v_station_code in nmemc.hy_station.station_code%TYPE,
v_min_time date,
v_max_time date,
min_offset out number,
max_offset out number)
is
begin
  ……
  if min_offset is null then
    min_offset:= 30/1440;--30分钟
  end if;
  if max_offset is null then
    max_offset:= 240/1440;--4小时
  end if;
end PRO_OFFSET_TIME;
/
```
调用：
```
reate or replace procedure pro_hy_data_handle_record
as
……
  min_offset number:= 30/1440;--30分钟
  max_offset number:= 240/1440;--4小时
begin
……
pro_offset_time(v_station_code,k_min_time,k_max_time,min_offset,max_offset);
end;
--然后就可以直接使用min_offset,max_offset了
```
