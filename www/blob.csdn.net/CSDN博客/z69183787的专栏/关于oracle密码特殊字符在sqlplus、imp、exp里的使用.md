# 关于oracle密码特殊字符在sqlplus、imp、exp里的使用 - z69183787的专栏 - CSDN博客
2013年12月27日 17:14:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3011
假如数据库网络连接串是db_wending, 密码是 dba/123,@lk.com
SQLPLUS下(密码用一对双引号括住): 
SQL> alter user u_test identified by "dba/123,@lk.com";
SQL> conn u_test/"dba/123,@lk.com"@db_wending
Linux下(密码用一对双引号, 整体userid用对单引号括住): 
$ exp 'u_test/"dba/123,@lk.com"@db_wending' file=u_test.dmp buffer=4096000
windows下(密码前后分别用三个双引号括住): 
D:\> exp u_test/"""dba/123,@lk.com"""@db_wending file=u_test.dmp buffer=4096000
--End--
