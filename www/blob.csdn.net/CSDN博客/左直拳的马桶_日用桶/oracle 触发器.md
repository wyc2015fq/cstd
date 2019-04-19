# oracle 触发器 - 左直拳的马桶_日用桶 - CSDN博客
2017年06月24日 10:48:55[左直拳](https://me.csdn.net/leftfist)阅读数：388标签：[oracle](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[oracle](https://blog.csdn.net/leftfist/article/category/5734215)
触发器的时机，有before和after。二者使用场合肯定不一样。我想到的是：
> 
1、before：插入或修改前，一般是针对本表操作做一些补充和完善 
   2、after：插入或修改后，可以针对关联表做一些操作
举个栗子：
```
create or replace trigger trg_alert_level
  before insert on hy_data_handle_record  
  for each row
declare
  -- local variables here
begin
    select max(s_level) into :NEW.ALERT_LEVEL from(
      select :NEW.l_ph s_level from dual
             union all
      select :NEW.l_do s_level from dual
             union all
      select :NEW.l_cod s_level from dual
            union all
      select :NEW.l_andan s_level from dual
            union all
      select :NEW.l_wujidan s_level from dual
            union all
      select :NEW.l_linsuanyan s_level from dual
            union all
      select :NEW.l_zonglin s_level from dual
            union all
      select :NEW.l_vedio s_level from dual      
    );
end trg_alert_level;
/
```
上面这个触发器，就是在记录插入前，从该记录的一系列字段中找出一个最大值，付给字段alert_level，功效就是记录插入后，所有字段的值都妥妥的。 
这个：NEW，就是正在插入的记录本身。
for update的也一样，也可以使用:NEW
