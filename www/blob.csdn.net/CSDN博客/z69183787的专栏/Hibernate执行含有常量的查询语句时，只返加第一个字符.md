# Hibernate执行含有常量的查询语句时，只返加第一个字符 - z69183787的专栏 - CSDN博客
2013年12月23日 22:26:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1152
如sql语句select 'users' as nodetype from dual;当hibernate执行时，会只返回一个字符'u'。
查找资料，常量在数据库中被认为是CHAR类型的，Hibernate取值时将其保存为Character类型，而Character只能保存一个字符，所以造成返回值中只存在一个字符。
解决方法有两个：
1，sql语句改为select decode(1,1,'users') as nodetype from dual;
2，sql语句改为select cast('users' as varchar2(5)) as nodetype fom dul;
