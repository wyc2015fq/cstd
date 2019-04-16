# 转载和积累系列 - linux时间同步命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月15日 11:09:54[initphp](https://me.csdn.net/initphp)阅读数：1098








```
# yum install –y ntp
↑ 安装NTP官方的时间同步程序 (NTP:中国国家授时中心)
# /usr/sbin/ntpdate -s pool.ntp.org
↑ 以NTP官方服务器为准调整本地时间
# crontab -e
↑ 编辑计划任务列表
0 3 * * * /usr/sbin/ntpdate -s pool.ntp.org
↑ 编辑文件到此状态，表示每天凌晨3点自动同步时间
# /sbin/service crond reload
↑ 重载计划任务配置
```




