# oracle的clob转换varchar2 - z69183787的专栏 - CSDN博客
2013年10月30日 17:26:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2102
time: 2008/02/29
author: skate
oracle的clob转换varchar2
今天在做一个表的数据转移的时候,发现要他通过比较clob字段,但大家都知道clob字段是无法比较的,所以我就用下面的函数来转换
to_char(substr(mw.context_,1,1000))
--end-- 
