# 未在本地计算机上注册Microsoft.ACE.OLEDB.12.0提供程序 - xqhrs232的专栏 - CSDN博客
2018年05月26日 12:25:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：168
原文地址::[https://blog.csdn.net/ldl22847/article/details/7728208](https://blog.csdn.net/ldl22847/article/details/7728208)
解决访问Excel数据源时出现 未在本地计算机上注册Microsoft.ACE.OLEDB.12.0提供程序
1、确保安装了Microsoft.ACE.OLEDB.12.0驱动
[http://download.microsoft.com/download/7/0/3/703ffbcb-dc0c-4e19-b0da-1463960fdcdb/AccessDatabaseEngine.exe](http://download.microsoft.com/download/7/0/3/703ffbcb-dc0c-4e19-b0da-1463960fdcdb/AccessDatabaseEngine.exe)
2、在vs中右击项目--》属性--》生成 下的 目标平台 改为x86
如果以上两个方法还是不行的话，用第三个方法
3、在对应的 IIS 应用程序池中，“设置应用程序池默认属性”右击/“高级设置”/"启用32位应用程序"，设置为 true。
问题：未在本地计算机上注册Microsoft.ACE.OLEDB.12.0提供程序
