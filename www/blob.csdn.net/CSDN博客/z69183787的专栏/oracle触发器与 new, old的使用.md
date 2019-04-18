# oracle触发器与:new,:old的使用 - z69183787的专栏 - CSDN博客
2013年12月13日 11:26:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：954
:new --为一个引用最新的列值;
:old --为一个引用以前的列值; 这两个变量只有在使用了关键字 "FOR EACH ROW"时才存在.且update语句两个都有,而insert只有:new ,delect 只有:old;
系统中的触发器实例：
create or replace trigger JBPM.TIB_DEPLOYBYMOVEPAPER before //before表示在操作完成前触发，                                                                                                             after表示在完成后触发
insert //发生插入数据操作触发
on JBPM.DEPLOYBYMOVEPAPER for each row //指定触发器每行触发一次
declare
    integrity_error exception; //用户自定义错误
    errno            integer;
    errmsg           char(200);
    dummy            integer;
    found            boolean;
    row_count number;
    max_num deploybymovepaper.num%type; //根据表的字段定义变量类型
    zero_today deploybymovepaper.num%type;
begin
-- Column "ID" uses sequence SEQUENCE_38
case
when inserting then //SQL语句只能使用:new特殊变量
select SEQUENCE_38.NEXTVAL INTO :new.ID from dual;//主键自增
zero_today :='0391'||to_char(sysdate,'yymmdd')||lpad(to_char(0),4,'0');//lpad函数定义字符串长度                                                                                                               为4不足时用0补全
    select count(*) into row_count from deploybymovepaper;
    if row_count = 0 then
        max_num:=zero_today;//:=给变量赋值
    else
      select max(num) into max_num from deploybymovepaper;
    end if;
    if max_num<zero_today then
        max_num:=zero_today;
    end if;
    if :new.num is null then
    :new.num:=lpad(to_char(to_number(max_num)+1),14,'0'); //递增编号，有14为数字组成
    end if;
when updating('num') then//:new、:old都可以用
if :new.num!=:old.num then
      :new.num:=:old.num;
end if;
end case;
-- Errors handling
exception
    when integrity_error then
       raise_application_error(errno, errmsg); //抛出异常语句
end;
/
