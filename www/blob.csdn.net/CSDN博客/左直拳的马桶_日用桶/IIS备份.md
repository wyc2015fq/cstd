# IIS备份 - 左直拳的马桶_日用桶 - CSDN博客
2012年08月01日 16:31:13[左直拳](https://me.csdn.net/leftfist)阅读数：5482
WEB服务器上的设置通常很多，不备份不行。
其实，IIS自己是有备份的，每当我们做了什么修改之后，它就会将修改前的设置做一个备份。
![](https://img-my.csdn.net/uploads/201208/01/1343809794_1480.png)
![](https://img-my.csdn.net/uploads/201208/01/1343809820_9488.png)
自动备份的文件存放在
C:\WINDOWS\system32\inetsrv\History\
也可以手动备份
![](https://img-my.csdn.net/uploads/201208/01/1343810337_3816.png)
手动备份的话，据说一定要使用密码，否则还原的时候，会报无效。
手动备份存放在
C:\WINDOWS\system32\inetsrv\MetaBack\
所以，我们只要搞一个批处理文件，然后再用任务计划定期运行这个批文件，就可以达到备份的效果了。
批处理文件可以这样写：
xcopy C:\WINDOWS\system32\inetsrv\History\*.* E:\backup\iis\history /e/d/c /y
xcopy C:\WINDOWS\system32\inetsrv\MetaBack\*.* E:\backup\iis\metaback /e/d/c /y
