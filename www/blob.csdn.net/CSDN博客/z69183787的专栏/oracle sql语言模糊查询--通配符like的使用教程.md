# oracle sql语言模糊查询--通配符like的使用教程 - z69183787的专栏 - CSDN博客
2014年02月11日 15:09:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2433
oracle在Where子句中，可以对datetime、char、varchar字段类型的列用Like子句配合通配符选取那些“很像...”的数据记录，以下是可使用的通配符：
%   零或者多个字符
_    单一任何字符（下划线）
\     特殊字符
oracle10g以上支持正则表达式的函数主要有下面四个：
1，REGEXP_LIKE ：与LIKE的功能相似
2，REGEXP_INSTR ：与INSTR的功能相似
3，REGEXP_SUBSTR ：与SUBSTR的功能相似
4，REGEXP_REPLACE ：与REPLACE的功能相似
POSIX 正则表达式由标准的元字符（metacharacters）所构成：
'^' 匹配输入字符串的开始位置，在方括号表达式中使用，此时它表示不接受该字符集合。
'$' 匹配输入字符串的结尾位置。如果设置了 RegExp 对象的 Multiline 属性，则 $ 也匹
配 '\n' 或 '\r'。
'.' 匹配除换行符之外的任何单字符。
'?' 匹配前面的子表达式零次或一次。
'+' 匹配前面的子表达式一次或多次。
'*' 匹配前面的子表达式零次或多次。
'|' 指明两项之间的一个选择。例子'^([a-z]+|[0-9]+)$'表示所有小写字母或数字组合成的
字符串。
'( )' 标记一个子表达式的开始和结束位置。
'[]' 标记一个中括号表达式。
'{m,n}' 一个精确地出现次数范围，m=<出现次数<=n，'{m}'表示出现m次，'{m,}'表示至少
出现m次。
\num 匹配 num，其中 num 是一个正整数。对所获取的匹配的引用。
字符簇： 
[[:alpha:]] 任何字母。
[[:digit:]] 任何数字。
[[:alnum:]] 任何字母和数字。
[[:space:]] 任何白字符。
[[:upper:]] 任何大写字母。
[[:lower:]] 任何小写字母。
[[:punct:]] 任何标点符号。
[[:xdigit:]] 任何16进制的数字，相当于[0-9a-fA-F]。
各种操作符的运算优先级
\转义符
(), (?:), (?=), [] 圆括号和方括号
*, +, ?, {n}, {n,}, {n,m} 限定符
^, $, anymetacharacter 位置和顺序
| 
*/
其中关于条件，SQL提供了四种匹配模式：
1，%：表示任意0个或多个字符。可匹配任意类型和长度的字符，有些情况下若是中文，请使用两个百分号（%%）表示。
比如 SELECT * FROM [user] WHERE u_name LIKE '%三%'
将会把u_name为“张三”，“张猫三”、“三脚猫”，“唐三藏”等等有“三”的记录全找出来。
另外，如果需要找出u_name中既有“三”又有“猫”的记录，请使用and条件
SELECT * FROM [user] WHERE u_name LIKE '%三%' AND u_name LIKE '%猫%'
若使用 SELECT * FROM [user] WHERE u_name LIKE '%三%猫%'
虽然能搜索出“三脚猫”，但不能搜索出符合条件的“张猫三”。
2，_： 表示任意单个字符。匹配单个任意字符，它常用来限制表达式的字符长度语句：
比如 SELECT * FROM [user] WHERE u_name LIKE '_三_'
只找出“唐三藏”这样u_name为三个字且中间一个字是“三”的；
再比如 SELECT * FROM [user] WHERE u_name LIKE '三__';
只找出“三脚猫”这样name为三个字且第一个字是“三”的；
3. regexp_like 正则表达式函数查询
--查询FieldName中以1开头60结束的记录并且长度是7位
select * from fzq where FieldName like '1____60';
select * from fzq where regexp_like(FieldName,'1....60');
--查询FieldName中以1开头60结束的记录并且长度是7位并且全部是数字的记录。
--使用like就不是很好实现了。
select * from fzq where regexp_like(FieldName,'1[0-9]{4}60');
-- 也可以这样实现，使用字符集。
select * from fzq where regexp_like(FieldName,'1[[:digit:]]{4}60');
-- 查询FieldName中不是纯数字的记录
select * from fzq where not regexp_like(FieldName,'^[[:digit:]]+$');
-- 查询FieldName中不包含任何数字的记录。
select * from fzq where regexp_like(FieldName,'^[^[:digit:]]+$');
--查询以12或者1b开头的记录.不区分大小写。
select * from fzq where regexp_like(FieldName,'^1[2b]','i');
--查询以12或者1b开头的记录.区分大小写。
select * from fzq where regexp_like(FieldName,'^1[2B]');
-- 查询数据中包含空白的记录。
select * from fzq where regexp_like(FieldName,'[[:space:]]');
--查询所有包含小写字母或者数字的记录。
select * from fzq where regexp_like(FieldName,'^([a-z]+|[0-9]+)$');
--查询任何包含标点符号的记录。
select * from fzq where regexp_like(FieldName,'[[:punct:]]');
