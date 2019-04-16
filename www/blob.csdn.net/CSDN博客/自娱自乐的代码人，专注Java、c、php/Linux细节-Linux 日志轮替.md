# Linux细节 - Linux 日志轮替 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月20日 15:49:19[initphp](https://me.csdn.net/initphp)阅读数：1289








```
#日志轮替默认子Cron里面进行
/etc/cron.daily/logrotate
#日志文件依赖配置文件
/etc/logrotate.conf

#配置文件详细内容解读
weekly  #默认每周进行一次日志清理
rotate 10 #保留的日志文件
# create new (empty) log files after rotating old ones
create  #创建一个新的来存储
# uncomment this if you want your log files compressed
#compress  #是否需要压缩
# RPM packages drop log rotation information into this directory
include /etc/logrotate.d
# no packages own wtmp -- we'll rotate them here
/var/log/wtmp {  #针对单个wtmp日志操作
    monthly    #每个月一次
    minsize 1M    #超过1M整理
    create 0664 root utmp  #新加文件权限和用户组
    rotate 1  #保留一个文件
}
```





