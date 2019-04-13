
# Oracle中删除大数据量表 - 追求卓越,做到专业 - CSDN博客


2016年01月21日 16:09:10[Waldenz](https://me.csdn.net/enter89)阅读数：1526


首先创建存储过程

```python
create or replace procedure delBigTab(p_TableName in varchar2,
                                      P_Condition in varchar2,
                                      p_Count     in varchar2) as
  pragma autonomous_transaction;
  n_delete number := 0;
begin
  while 1 = 1 loop
    EXECUTE IMMEDIATE 'delete from ' || p_TableName || ' where ' || P_Condition || ' and  rownum <= :rn'
      USING p_Count;
    if SQL%NOTFOUND then
      exit;
    else
      n_delete := n_delete + SQL%ROWCOUNT;
    end if;
    commit;
  end loop;
  commit;
  DBMS_OUTPUT.PUT_LINE('Finished!');
  DBMS_OUTPUT.PUT_LINE('Totally ' || to_char(n_delete) ||
                       ' records deleted!');
end;
```
在PL/SQL中新建Command Window，执行以下语句即可。
```python
exec  delBigTab('table','1=1','100');
```


