# Weblogic11G，日志功能按天分割记录 - z69183787的专栏 - CSDN博客
2013年04月03日 14:11:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14401
Weblogic 主要有三种日志，放在目录
%BEA_HOME%\wlserver6.1\config\mydomain\logs下面：
1.域日志
2.应用日志
3.HTTP访问日志
进入控制台后，可以分别配置。
![](https://img-my.csdn.net/uploads/201304/03/1364974094_7942.jpg)
portal.shmetro.com 为域名 点击后，可以配置域日志；
点击环境下的服务器后，在日志记录中 可配置应用日志及http访问日志
配置方式类似
如下图所示：
![](https://img-my.csdn.net/uploads/201304/03/1364970246_8234.jpg)
将滚动类型修改为 按时间，日志文件名为图中所示，则weblogic日志文件会按天记录，当天记录的文件名为 YYYY_MM_DD
![](https://img-my.csdn.net/uploads/201304/03/1364969610_7221.jpg)
还有一个问题，发现系统中 system.out 的 内容在默认配置下都不会再控制台及log中输出，需要修改，如下图：
![](https://img-my.csdn.net/uploads/201304/03/1364969711_5486.jpg)
![](https://img-my.csdn.net/uploads/201304/03/1364969742_2420.jpg)
包括 勾选 重定向标准输出日志记录 及 严重性级别的修改。
