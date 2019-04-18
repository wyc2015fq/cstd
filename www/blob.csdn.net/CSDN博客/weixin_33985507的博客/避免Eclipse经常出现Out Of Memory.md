# 避免Eclipse经常出现Out Of Memory - weixin_33985507的博客 - CSDN博客
2012年05月14日 13:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# 修改eclipse.ini
-showsplash 
org.eclipse.platform 
--launcher.XXMaxPermSize 
512m 
-vmargs 
-Xms256m 
-Xmx512m 
-XX:PermSize=128M 
-XX:MaxPermSize=128M
