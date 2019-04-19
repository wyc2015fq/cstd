# 利用SQL SERVER建立登录WINDOWS帐号 - 左直拳的马桶_日用桶 - CSDN博客
2008年11月06日 15:21:00[左直拳](https://me.csdn.net/leftfist)阅读数：1474标签：[sql server																[windows																[数据库服务器																[远程登录																[数据库																[raid5](https://so.csdn.net/so/search/s.do?q=raid5&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=远程登录&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
利用SQL SERVER建立登录WINDOWS帐号
左直拳
<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
那一刻，我的心跳到了嗓子眼。
运行建立帐号命令，OK!
远程登录，输入帐号，密码，OK！终于又进去了！
×××××××××××××××××××
我忽然发现，数据库服务器登录不了了。
提示帐号密码过期。按提示修改，提交，又说我没权限修改密码！
CNM，这样来害我。这个帐号属于administrators，原先那个默认的administrator被我停掉了。因此这个帐号是进入系统的唯一通道。
接着发现后果比我想象的要严重更多：
1、硬盘做了RAID5，这样即使拿到硬盘，可能也读不出数据库文件了。
2、我们数据库的备份文件一周才备份一次，而且备份文件也放在同一台服务器
3、众所周知，MS SQL备份只能备份到本机，除非有映射其他机器的盘。当然拿个U盘挂到上面，然后再备份也是可以的，可是机房离我们这里几百公里。
我的心情真是跌到了冰点。
……
幸亏，现在服务器还没出什么问题，并且我有数据库超级管理员SA的帐号密码，用同一局域网的SQL还可以连过去。
解决方案如下：
用SA进入SQL SERVER2005，运行以下命令：
先开启xp_cmdshell:
-- To allow advanced options to be changed.
EXECsp_configure'show advanced options', 1
GO
-- To update the currently configured value for advanced options.
RECONFIGURE
GO
-- To enable the feature.
EXECsp_configure'xp_cmdshell', 1
GO
-- To update the currently configured value for this feature.
RECONFIGURE
GO
然后建一个windows账户：
execxp_cmdshell'netusermm123/add'
execxp_cmdshell'netlocalgroupadministratorsmm/add'

