# Oracle 触发器 - z69183787的专栏 - CSDN博客
2013年12月13日 11:26:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：569
```
create or replace trigger trg_ins_shortmessage
  before insert on t_short_msg
  for each row
-- Date:2011-11-22 by zs
-- 重复短信不插入
declare
  -- local variables here
  v_content t_short_msg.content%type;
  v_mobile t_short_msg.mobile%type;
  v_insertdate t_short_msg.insertdate%type;
  v_count number(5);
  v_date t_short_msg.insertdate%type;
begin
  v_content := :new.content;
  v_mobile := :new.mobile;
  v_insertdate := :new.insertdate;
  v_date := to_char(sysdate,'yyyy-mm-dd hh24:mi:ss');
  if instr(:new.content,'业务待办提醒')>0
    then
     select count(id) into v_count from t_short_msg  where  content like '%'||v_content||'%' and mobile = v_mobile
     and to_date(v_insertdate,'yyyy-mm-dd hh24:mi:ss') < to_date(insertdate,'yyyy-mm-dd hh24:mi:ss')+interval '6' hour;
       if (v_count>0) then
          :new.insertdate := v_date;
          :new.senddate := v_date;
          :new.status := 1;
       end if;
  end if;
end trg_ins_shortmessage;
```
:new. 为 当前插入的这条记录  :new.content 为 当前这条记录的属性。可在插入前更改。
            
