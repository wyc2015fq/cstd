# SQL Server 附加数据库提示5120错误 - xqhrs232的专栏 - CSDN博客
2018年07月25日 14:39:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：93
原文地址::[https://www.cnblogs.com/yeminglong/p/5792553.html](https://www.cnblogs.com/yeminglong/p/5792553.html)
相关文章
1、[SQL Server附加数据库出现错误5123的正确解决方法](https://www.cnblogs.com/xuruiming/articles/2964507.html)----[https://www.cnblogs.com/xuruiming/articles/2964507.html](https://www.cnblogs.com/xuruiming/articles/2964507.html)
怎么样是不是跟你的错误是一样的，心里是不是有点小激动？ T_T 终于有办法了！！！！
**![](https://images2015.cnblogs.com/blog/341068/201608/341068-20160822141253214-594435272.png)**
**第一步先关掉你的SQLserver**
**然后在菜单上找找到SQLSERVER右键选择“以管理员运行”**
**![](https://images2015.cnblogs.com/blog/341068/201701/341068-20170113144718994-2122737311.png)**
**第二步给你的数据库文件所在文件夹给上“读取，写入”的权限。**
**在附加数据库时候遇见 5120错误，建议把放置放置数据库的文件夹的权限更改为完全控制。**
**![](https://images2015.cnblogs.com/blog/341068/201608/341068-20160821130319683-1535423130.png)**
**假如你的操作系统是windows8 或者windows10 那么你发现操作了这一步还是不行。**
**我告诉你 关掉你的sqlserver 然后再开始菜单点击右键“以管理员身份运行 然后再次附件一次数据库 ”**
**好了整个世界都清静了**
**　　　　收工。**
