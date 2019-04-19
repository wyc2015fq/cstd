# WINDOWS命令行关闭本地占用的端口 - Ms.Fox的专栏 - CSDN博客
2019年04月02日 16:29:36[lulu-lu](https://me.csdn.net/smbluesky)阅读数：5
本地运行jboss服务，因eclipse意外关闭，而服务未断导致的端口被占用，解决办法如下：
1.查找对应的端口占用的进程：netstat  -aon|findstr  "8080"    ，找到占用8080端口对应的程序的PID号；
![](https://img-blog.csdn.net/20180620185254451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poeF8wMzIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.根据PID号找到对应的程序：tasklist|findstr "PID号"    ，找到对应的程序名；
![](https://img-blog.csdn.net/20180620185521781?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poeF8wMzIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.结束该进程：taskkill /f /t /im javaw.exe；或者taskkill /pid 3672 /F。
