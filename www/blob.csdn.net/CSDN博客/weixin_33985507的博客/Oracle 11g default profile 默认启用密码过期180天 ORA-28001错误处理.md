# Oracle 11g default profile 默认启用密码过期180天 ORA-28001错误处理 - weixin_33985507的博客 - CSDN博客
2013年06月19日 09:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
问题描述：客户反映客户端不能登录应用程序时不能连接。
环境：AIX 5.3 + Oracle 11gR2
解决：远程Telnet到数据库主机，sqlplus / as sysdba 连接后操作正常，表明数据库服务器应该正常，然后在客户端用sqlplus连接数据库提示密码过期错误：ORA-28001: the password has expired，原来是密码过期了。
分析：原来Oracle 11g之前的版本默认profile是没有密码过期的限制的，在Oracle 11g中默认的profile启用了密码过期时间是180天，去除这个限制便可。
查询默认的profile的密码过期的设置
select * from dba_profiles
where profile='DEFAULT' and resource_name='PASSWORD_LIFE_TIME';
设置default profile密码永不过期，立即生效，不需重启
alter profile default limit password_life_time unlimited;
已经过期的密码就需要改了，可以该成一样
可以sqlplus 登录会提示你改密码
也可以alter user username identified by password;
