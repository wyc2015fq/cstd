# MySQL 5.7.22 社区版下载安装及开启允许远端连接访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月21日 11:20:39[boonya](https://me.csdn.net/boonya)阅读数：313








### 绿色版

[https://dev.mysql.com/downloads/mysql/5.7.html#downloads](https://dev.mysql.com/downloads/mysql/5.7.html#downloads)

![](https://img-blog.csdn.net/20180721111602146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 安装版

[https://dev.mysql.com/downloads/windows/installer/5.7.html](https://dev.mysql.com/downloads/windows/installer/5.7.html)

![](https://img-blog.csdn.net/20180721111513888?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 操作方法

安装注意事项：

Installer:

请选择自定义安装Custom,安装对应版本的MySQL 5.7.22 Server

========================================================

如安装后不能开启远程访问说明安装错误卸载重新安装!!!!!!!!

=========================================================

Zip:

安装步骤参考：[https://www.cnblogs.com/xiaxiaoxu/p/8977418.html](https://www.cnblogs.com/xiaxiaoxu/p/8977418.html)

### 开启远程服务

```
use mysql;

grant all privileges  on *.* to root@'%' identified by 'root';

flush privileges;
```





