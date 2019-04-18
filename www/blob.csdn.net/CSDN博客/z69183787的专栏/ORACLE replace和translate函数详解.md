# ORACLE replace和translate函数详解 - z69183787的专栏 - CSDN博客
2013年08月15日 10:30:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1739
题记：老了，很多东西看了之后就忘了，所谓过目即忘，把他记下来，有时间再翻翻看，
内容来自网上，向原作者致敬
简要比较：
replace：字符串级别的代替  
如：SELECTREPLACE('acdd','cd','ef') FROM dual;
 →aefd
translate：字符级别的代替
如：SELECTTRANSLATE('acdd','cd','ef') FROM dual;
 →aeff
分别详解：
replace:
语法：REPLACE
 ( char , search_string [, replacement_string] )
REPLACE returns char with every occurrence of search_string replaced
 with replacement_string. If replacement_string is omitted or null, then all occurrences of search_string are removed. If search_string is null, then char is returned.
解释：repalce中，每个search_string都被replacement_string所代替。
selectreplace('acdd','cd','ef') from dual;→ aefd
如果replacement_string为空或为NULL,那么所有的search_string都被移除。
selectreplace('acdd','cd','') from dual;→ad
如果search_string为null,那么就返回原来的char。
selectreplace('acdd','','ef') from dual;→acdd
selectreplace('acdd','','') from dual;→acdd(也是两者都为空的情况) 
Both search_string and replacement_string, as well as char, can be any of the datatypes CHAR, VARCHAR2, NCHAR, NVARCHAR2, CLOB, or NCLOB.
 The string returned is of VARCHAR2 datatype and is in the same character set as char.
解释：这段指明了可以用的数据类型.
This function provides functionality related to that provided by the
 TRANSLATE function. TRANSLATE provides single-character, one-to-one substitution. REPLACE lets
 you substitute one string for another as well as to remove character strings.
解释：红色部分也是replace和translate的区别。
translate:
语法：TRANSLATE ( 'char' , 'from_string' , 'to_string' ) 
TRANSLATE returns char with all occurrences of each character in from_string replaced by its corresponding character in to_string. Characters in char that are not in from_string are not replaced. The
 argument from_string can contain more characters than to_string. In this case, the extra characters at the end of from_string have no corresponding characters in to_string. If these extra characters appear in char, then they are removed from the return value.You
 cannot use an empty string for to_string to remove all characters in from_string from the return value. Oracle interprets the empty string as null, and if this function has a null argument, then it returns null.
解释：Translate中，每个from_string中的字符被to_string中相应的字符所代替。
select translate('acdd','cd','ef') from dual;→aeff
如果from_string比to_string长，那么from_string中多余的字符将被移除。
select translate('acdd','acd','ef') from dual;→ef (a由e代替，c由f代替,d就被移除)
select translate('acdd','cda','ef') from dual;→eff(c由e代替，d由f代替，a就被移除)
如果to_string为空，或者两者都为空，那么返回char也为空。所以to_string不能为空。
select translate('acdd','cd','') from dual;→ (空)
select translate('acdd','','') from dual;→(空)
实战：
如何判断一个字符串是否是数字？
解：先转换：由于to_string不能为空，我们巧用#号代替
select translate('abc123','#1234567890.','#') from dual;→abc
from_string中的#被to_string中的#代替，但char中又没有#字符，所以通过这一步躲开了to_string必须不为空的规则。然后后面的数字以及小数点都转换为空，于是原来的字符串中只留下abc三个字符。
转换好后，用nvl2判断即可：
select nvl2(translate('abc123','#1234567890.','#'),'字符串','数字') from dual;→字符串
nvl2的作用就是，NVL2 (expr1, expr2, expr3) ->expr1不为NULL，返回expr2；为NULL，返回expr3。这样我们就可以判断一个字符串是否是数字了!解毕!
