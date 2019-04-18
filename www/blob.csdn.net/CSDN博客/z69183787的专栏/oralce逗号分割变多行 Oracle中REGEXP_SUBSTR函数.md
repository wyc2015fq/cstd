# oralce逗号分割变多行 Oracle中REGEXP_SUBSTR函数 - z69183787的专栏 - CSDN博客
2014年11月19日 15:08:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：25846
[Oracle](http://www.2cto.com/database/Oracle/)中REGEXP_SUBSTR函数
Oracle中REGEXP_SUBSTR函数的使用说明：
题目如下:
在oracle中，使用一条语句实现将'17,20,23'拆分成'17'，'20'，'23'的集合。
REGEXP_SUBSTR函数格式如下：
function REGEXP_SUBSTR(String, pattern, position, occurrence, modifier)
__srcstr     ：需要进行正则处理的字符串
__pattern    ：进行匹配的正则表达式
__position   ：起始位置，从第几个字符开始正则表达式匹配（默认为1）
__occurrence ：标识第几个匹配组，默认为1
__modifier   ：模式（'i'不区分大小写进行检索；'c'区分大小写进行检索。默认为'c'。）
1、查询使用正则分割后的第一个值，也就是17
[sql] 
SELECT REGEXP_SUBSTR('17,20,23','[^,]+',1,1,'i') AS STR FROM DUAL;  
结果:  www.2cto.com  
STR
-----
17
2、查询使用正则分割后的最后一个值，也就是23
[sql] 
SELECT REGEXP_SUBSTR('17,20,23','[^,]+',1,3,'i') AS STR FROM DUAL;  
结果:
STR
----
23
3、获取一个多个数值的列，从而能够让结果以多行的形式展示出来
[sql] 
SELECT LEVEL FROM DUAL CONNECT BY LEVEL <=7;  
结果:  www.2cto.com  
LEVEL
----
1
2
3
4
5
6
7
4、将上面REGEXP_SUBSTR的occurrence关联
[sql] 
SELECT NVL(REGEXP_SUBSTR('17,20,23', '[^,]+', 1, LEVEL, 'i'), 'NULLL') AS STR  
  FROM DUAL  
CONNECT BY LEVEL <= 7;  
STR  www.2cto.com  
----
17
20
23
NULL
NULL
NULL
NULL
5、优化上面的SQL语句，让生成的行的数量符合实际情况
[sql] 
SELECT REGEXP_SUBSTR('17,20,23', '[^,]+', 1, LEVEL, 'i') AS STR  
  FROM DUAL  
CONNECT BY LEVEL <=  
           LENGTH('17,20,23') - LENGTH(REGEXP_REPLACE('17,20,23', ',', ''))+1;  
STR
----
17
20
23
oralce逗号分割变多行
方法一
select  a.*  , 
REGEXP_SUBSTR(a.rolecode ,'[^,]+',1,l) AS rolecode 
from p_user a,(SELECT LEVEL l FROM DUAL CONNECT BY LEVEL<=100) b
WHERE l <=LENGTH(a.rolecode) - LENGTH(REPLACE(rolecode,','))+1
   www.2cto.com  
使用[函数](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2Fdb%2FOracle%2F201303%2F29270.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=%BA%AF%CA%FD&k0=sum&kdi0=8&k1=%BA%AF%CA%FD&kdi1=8&k2=open&kdi2=1&k3=type&kdi3=1&k4=%B1%EA%CA%B6&kdi4=1&sid=bf266d7d90d60111&ch=0&tu=u1682280&jk=0ece881761e169d5&cf=29&fv=11&stid=9&urlid=0&luki=2&seller_id=1&di=128)REGEXP_SUBSTR拆分字符串：
5个参数
第一个是输入的字符串
第二个是正则表达式
第三个是标识从第几个字符开始正则表达式匹配。（默认为1）
第四个是[标识](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2Fdb%2FOracle%2F201303%2F29270.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=%B1%EA%CA%B6&k0=sum&kdi0=8&k1=%BA%AF%CA%FD&kdi1=8&k2=open&kdi2=1&k3=type&kdi3=1&k4=%B1%EA%CA%B6&kdi4=1&sid=bf266d7d90d60111&ch=0&tu=u1682280&jk=0ece881761e169d5&cf=29&fv=11&stid=9&urlid=0&luki=5&seller_id=1&di=128)第几个匹配组。（默认为1）
第五个是是取值范围：
i：大小写不敏感；
c：大小写敏感；
n：点号 . 不匹配换行符号；
m：多行模式；
x：扩展模式，忽略正则表达式中的空白字符。
SELECT a.*,REGEXP_SUBSTR(servicereqid ,'[^;]+',1,l) AS servicereq
  FROM sum_portal_satisfaction a,(SELECT LEVEL l FROM DUAL CONNECT BY LEVEL<=100) b
WHERE l <=LENGTH(servicereqid) - LENGTH(REPLACE(servicereqid,';'))+1
ORDER BY 1,2;
----SELECT LEVEL l FROM DUAL CONNECT BY LEVEL<=100; 生成1到100的数据行。
----l <=LENGTH(servicereqid) - LENGTH(REPLACE(servicereqid,';'))+1，注意此处是‘L’并非‘1’，上面的REGEXP_SUBSTR的第四个参数也一样。
---下面为拆分字符串，再进行的行转列
create or replace view v_[sum](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2Fdb%2FOracle%2F201303%2F29270.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=sum&k0=sum&kdi0=8&k1=%BA%AF%CA%FD&kdi1=8&k2=open&kdi2=1&k3=type&kdi3=1&k4=%B1%EA%CA%B6&kdi4=1&sid=bf266d7d90d60111&ch=0&tu=u1682280&jk=0ece881761e169d5&cf=29&fv=11&stid=9&urlid=0&luki=1&seller_id=1&di=128)_portal_satisfaction_sr
 as
select 
survey_[type](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2Fdb%2FOracle%2F201303%2F29270.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=type&k0=sum&kdi0=8&k1=%BA%AF%CA%FD&kdi1=8&k2=open&kdi2=1&k3=type&kdi3=1&k4=%B1%EA%CA%B6&kdi4=1&sid=bf266d7d90d60111&ch=0&tu=u1682280&jk=0ece881761e169d5&cf=29&fv=11&stid=9&urlid=0&luki=4&seller_id=1&di=128),
survey_time,
center_code,
center_name,
city_id,
city_name,
REGEXP_SUBSTR(servicereqid ,'[^;]+',1,l) AS servicereqid,
REGEXP_SUBSTR(servicereqname ,'[^;]+',1,l) AS servicereqname,
sum(decode(survey_value,0, sur_times,null)) giveup_times,--调查值 -1:未处理 0:用户放弃 1:很满意 2.满意 3.对csr不满意 4.对其它不满意
sum(decode(survey_value,1, sur_times,null))vsatis_times,
sum(decode(survey_value,2, sur_times,null))satis_times,
sum(decode(survey_value,3, sur_times,null))ncsr_times,
sum(decode(survey_value,4, sur_times,null))nelse_times,
sum(sur_times) sur_times
  FROM [sum](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2Fdb%2FOracle%2F201303%2F29270.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=sum&k0=sum&kdi0=8&k1=%BA%AF%CA%FD&kdi1=8&k2=open&kdi2=1&k3=type&kdi3=1&k4=%B1%EA%CA%B6&kdi4=1&sid=bf266d7d90d60111&ch=0&tu=u1682280&jk=0ece881761e169d5&cf=29&fv=11&stid=9&urlid=0&luki=1&seller_id=1&di=128)_portal_satisfaction
 a,(SELECT LEVEL l FROM DUAL CONNECT BY LEVEL<=100) b
WHERE l <=LENGTH(servicereqid) - LENGTH(REPLACE(servicereqid,';'))+1
group by
subslevelid,
center_code,
center_name,
city_id,
city_name,
survey_[type](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2Fdb%2FOracle%2F201303%2F29270.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=type&k0=sum&kdi0=8&k1=%BA%AF%CA%FD&kdi1=8&k2=open&kdi2=1&k3=type&kdi3=1&k4=%B1%EA%CA%B6&kdi4=1&sid=bf266d7d90d60111&ch=0&tu=u1682280&jk=0ece881761e169d5&cf=29&fv=11&stid=9&urlid=0&luki=4&seller_id=1&di=128),
survey_time,
servicereqid,
servicereqname,l
方法二:
create table  testTable (
       id  nvarchar2(200) primary key not null ,
       content  nvarchar2(200) not null 
)
insert into  testTable values ('4','馆内idx_10馆外idx_11总体idx_12');
select *  from table ( CAST (fn_split(('馆内idx_1$馆外idx_2$总体idx_3$') ,'$') as ty_str_split  )  )
select * from testtable b left join  table (fn_split((content), '$')   ) a   on 1=1;
--实现split函数
CREATE OR REPLACE TYPE ty_str_split IS TABLE OF VARCHAR2 (4000);
CREATE OR REPLACE FUNCTION fn_split (p_str IN VARCHAR2, p_delimiter IN VARCHAR2)
    RETURN ty_str_split
IS
    j INT := 0;
    i INT := 1;
    len INT := 0;
    len1 INT := 0;
    str VARCHAR2 (4000);
    str_split ty_str_split := ty_str_split ();
BEGIN
    len := LENGTH (p_str);
    len1 := LENGTH (p_delimiter);
    WHILE j < len
    LOOP
        j := INSTR (p_str, p_delimiter, i);
        IF j = 0
        THEN
            j := len;
            str := SUBSTR (p_str, i);
            str_split.EXTEND;
            str_split (str_split.COUNT) := str;
            IF i >= len
            THEN
                EXIT;
            END IF;
        ELSE
            str := SUBSTR (p_str, i, j - i);
            i := j + len1;
            str_split.EXTEND;
            str_split (str_split.COUNT) := str;
        END IF;
    END LOOP;
    RETURN str_split;
END fn_split;
DECLARE
    CURSOR c
    IS
select *  from table ( CAST (fn_split(('馆内idx_1$馆外idx_2$总体idx_3$') ,'$') as ty_str_split  )  );
    r c%ROWTYPE;
BEGIN
    OPEN c;
    LOOP
        FETCH c INTO r;
        EXIT WHEN c%NOTFOUND;
        DBMS_OUTPUT.put_line (r.column_value);
    END LOOP;
    CLOSE c;
END;
