# oracle的max()、full join和declare - 左直拳的马桶_日用桶 - CSDN博客
2017年08月28日 20:15:51[左直拳](https://me.csdn.net/leftfist)阅读数：528
这是近期的一些小心得，它们之间没啥关系，统一做个记录而已。
**一、max()**
`select * from table where ...`
假如过滤条件不满足的话，返回的记录数为0. 
但是，如果是
`select max(...) from table where ...`
过滤条件不满足，照样返回一条记录，max(…) is null 
   所以，如果是
`insert into table1(...) select max(...) from table2 where ... `
必有记录插入， `sql%rowcount` 必定大于0，有时会导致一些逻辑上的错误。
解决办法是再select 多一次，将max(…)作为过滤条件，is null则过滤掉。
**二、full join**
左连接，left outer join,简写为left join 
右连接，right join 
全连接，full join，呵呵
**三、declare**
直接给代码，注意end后一定要带分号`；`
```
declare
  new_time date := null;
begin
while new_time is null loop  
      pro_hy_data_handle_record;
      select max(create_time) into new_time from hy_data_handle_record
             where create_time>to_date('2017/8/28 13:48:33','yyyy/MM/dd hh24:mi:ss');
end loop;
end;
```
