# SQL Server与MySQL中的MD5与SHA1 - z69183787的专栏 - CSDN博客
2017年01月12日 12:05:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：744
SQL Server 2008：
select UPPER(replace(sys.fn_varbintohexstr(hashbytes('SHA1', '12345')), '0x', ''));
select UPPER(replace(sys.fn_varbintohexstr(hashbytes('MD5', '12345')), '0x', ''));
[MySQL](http://lib.csdn.net/base/mysql)：
SELECT md5('12345');
SELECT sha1('12345');
