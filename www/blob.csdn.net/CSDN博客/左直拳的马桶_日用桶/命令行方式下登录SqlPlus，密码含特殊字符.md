# 命令行方式下登录SqlPlus，密码含特殊字符 - 左直拳的马桶_日用桶 - CSDN博客
2015年09月15日 19:44:40[左直拳](https://me.csdn.net/leftfist)阅读数：4125
全撞上了！ 
真难侍候！oracle 12c，想登录sql plus，结果没有图形界面，直接出来个命令行。这下好了，我这个数据库，多实例，意味着登录要指定实例；密码中含有特殊字符“@”，哇塞！因为以前我试过酱紫登录：
sqlplus  用户名/密码@实例名 as sysdba
那现在咋整？
sqlplus  sys/!Q@W3e4r@sso as sysdba，失败！其中 !Q@W3e4r 是密码。
最后，经过艰苦的网上寻觅和自我搜索，是这样，分成2步：
C:\Users\Administrator>sqlplus “as sysdba”
C:\Users\Administrator>Enter user-name: sys/”!Q@W3e4r”@sso
搞定！
