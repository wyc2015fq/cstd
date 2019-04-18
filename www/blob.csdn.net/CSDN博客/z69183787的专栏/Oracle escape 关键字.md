# Oracle escape 关键字 - z69183787的专栏 - CSDN博客
2013年10月16日 17:02:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：989
英文解释：
It is necessary to use an "escape" character to locate the characters '_' and '%' in a column. The keyword ESCAPE followed by the character used as the delimitor of choice is coded after the string search. For example, '+' is used as the escape character. For
 example:
SELECT NAME        
FROM XYZ_TABLE            
WHERE NAME LIKE 'XY+_Z+%BC%'ESCAPE '+'   
Result: XY_Z%BCAA
            ...
            XY_Z%BC99
The plus sign '+' can still be used as part of the search string as long as a '+' precedes it. For example:
SELECT NAME         
FROM XYZ_TABLE         
WHERE NAME LIKE 'XY++Z%' ESCAPE '+'    
Result: XY+ZAAAAA
            ...
            XY+Z99999
汉语解释：
定义：escape关键字经常用于使某些特殊字符，如通配符：'%','_'转义为它们原
来的字符的意义，被定义的转义字符通常使用'\',但是也可以使用其他的符号。
实例：
SQL> select * from t11 where name like '%_%';
NAME
----------
aa_a
aaa
SQL> select * from t11 where name like '%\_%' escape '\';
NAME
----------
aa_a
注意：如果是 '/' 作为检索字符, 必须 用 '/' 作为转义符, 正斜扛也一样.
select * from wan_test where psid like '%//%' escape '/'
