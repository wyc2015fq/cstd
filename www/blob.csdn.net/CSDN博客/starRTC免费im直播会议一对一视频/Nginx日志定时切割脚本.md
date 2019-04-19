# Nginx日志定时切割脚本 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月31日 16:17:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：315
nginx的日志文件如果你不处理，将变得越来越大，我们可以写一个nginx日志切割脚本来自动切割日志文件。
第一步就是重命名日志文件，不用担心重命名后nginx找不到日志文件而丢失日志。在你未重新打开原名字的日志文件前，nginx还是会向你重命名的文件写日志，linux是靠文件描述符而不是文件名定位文件。
第二步向nginx主进程发送USR1信号。
nginx主进程接到信号后会从配置文件中读取日志文件名称，重新打开日志文件(以配置文件中的日志名称命名)，并以工作进程的用户作为日志文件的所有者。
重新打开日志文件后，nginx主进程会关闭重名的日志文件并通知工作进程使用新打开的日志文件。
工作进程立刻打开新的日志文件并关闭重名名的日志文件。
然后你就可以处理旧的日志文件了。
nginx日志按日期自动切割脚本cut_nginx_log.sh如下:
*#!/bin/bash**#by www.elesos.com**#设置日志文件存放目录*logs_path="/opt/nginx/html/"*#设置pid文件*pid_path="/opt/nginx/logs/nginx.pid"*#创建日志存放目录*
**mkdir**-p${logs_path}$**(****date**-d"yesterday" +"%Y"**)****/**$**(****date**-d"yesterday" +"%m"**)****/**
*#重命名日志文件***mv **${logs_path}elesos.com.log ${logs_path}$**(****date **-d "yesterday" +"%Y"**)****/**$**(****date **-d "yesterday" +"%m"**)****/**elesos.com_$**(****date **-d "yesterday" +"%Y%m%d"**)**.log**mv**${logs_path}elesos.com.log ${logs_path}$**(****date**-d"yesterday" +"%Y"**)****/**$**(****date**-d"yesterday" +"%m"**)****/**elesos.com_$**(****date**-d"yesterday" +"%Y%m%d"**)**.log*#向nginx主进程发信号重新打开日志***kill**-USR1**`****cat**${pid_path}**`**
注意修改上面的日志文件名为你自己的。
然后通过crontab设置作业，参见[Linux计划任务cron使用指南](http://192.168.1.100/elesos_com/index.php?title=Linux%E8%AE%A1%E5%88%92%E4%BB%BB%E5%8A%A1cron%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97)
0 0 * * * bash /usr/local/nginx/nginx_log.sh
这样就每天的0点0分把nginx日志重命名为日期格式，并重新生成今天的新日志文件。
## 艺搜参考
[http://www.nginx.cn/255.html](http://www.nginx.cn/255.html)
[http://www.jb51.net/LINUXjishu/33585.html](http://www.jb51.net/LINUXjishu/33585.html)
