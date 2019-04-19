# airflow  cron定时详解 - sxf_123456的博客 - CSDN博客
2018年03月14日 16:03:17[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1807

![](https://img-blog.csdn.net/20180314160348893?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# Example of job definition:
# .---------------- minute (0 - 59)
# | .------------- hour (0 - 23)
# | | .---------- day of month (1 - 31)
# | | | .------- month (1 - 12) OR jan,feb,mar,apr ...
# | | | | .---- day of week (0 - 6) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat
# | | | | |
# * * * * * user-name command to be executed
![](https://images2015.cnblogs.com/blog/722724/201608/722724-20160817105840890-421233819.png)
- **01 * * * * root run-parts /etc/cron.hourly**：在 #run-parts 这一行以后的命令，我们可以发现，五个数字后面接的是 root ，这一行代表的是『执行的级别为root身份』当然，你也可以将这一行改为成其他的身份！而 run-parts代表后面接的 /etc/cron.hourly 是『一个目录内（/etc/cron.hourly）的所有可执行文件』，也就是说，每个小时的01分，系统会以root身份去/etc/cron.hourly这个目录下执行所有可执行的文件！后面三行也是类似的意思！你可以到 /etc/ 底下去看看，系统本来就预设了这4个目录！你可以将每天需要执行的命令直接写到/etc/cron.daily即可，还不需要使用到crontab -e的程式！
- 
