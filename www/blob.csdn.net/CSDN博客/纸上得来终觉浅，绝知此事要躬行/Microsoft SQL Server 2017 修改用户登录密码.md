# Microsoft SQL Server 2017 修改用户登录密码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月19日 11:08:25[boonya](https://me.csdn.net/boonya)阅读数：4915








1、启动Microsoft  SQL Server服务，否则管理端没法连上数据库连接实例。

2、打开Microsoft SQL Server Management Studio，选择实例的"安全性">“登录名”，这里以sa为例：

![](https://img-blog.csdn.net/20180619105618982?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改输入两遍密码然后点击“确认”按钮。

3、重启服务实例使之生效






![](https://img-blog.csdn.net/20180619110653736?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：这里也可以去重启Microsoft  SQL Server实例服务，然后重连实例即可。



