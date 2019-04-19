# Centos下daemontools监控logstash进程 - BlueSky - CSDN博客
2016年06月04日 17:14:18[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1683
Logstash安装参考http://udn.yyuap.com/doc/logstash-best-practice-cn/get_start/daemon.html
首先用root用户，
```
$ yum install python-setuptools
$ easy_install supervisor
```
然后可以查看具体的安装supervisord的版本：
```
$ supervisord --version
3.3.0
```
利用easy install安装完后并不会创建一个Supervisor的配置文件，但幸运的是Supervisor包括一个通用的配置文件，然后可以
`$ echo_supervisord_conf > /etc/supervisord.conf`
在supervisord.conf添加要监测的文件：
```
[program:logstash_hy]
command=/home/hadoop1/bms/logstash-1.5.4/bin/logstash -f /home/hadoop1/bms/logstash-1.5.4/conf/haiyun.conf
logfile=/home/hadoop1/bms/logstash-1.5.4/logs/test.log
```
然后就可以启动supervisor，
`/etc/init.d/supervisord -c /etc/supervisord.conf`
使配置更改生效
每次修改主配置文件或增改子配置文件都需要执行supervisorctl update使新配置生效。或者用supervisorctl reload重新重新启动supervisor程序。
开启基于http的UI控制台
修改主配置文件 
vim /etc/supervisor/supervisord.conf，添加如下内容：
[inet_http_server]
port = 192.168.1.60:9001 ＃IP和绑定端口
username = admin ＃管理员名称
password = 123456 #管理员密码
这样，在浏览器中输入配置中写的地址和端口号，输入配置好的用户名和密码，便可以看到基于网页的控制台界面。从中，可以察看supervisor监控的>信息，可以察看进程的日志等。
这样就可以检测logstash的实时运行了
同时可以通过：supervisorctl查看管理监控的进程情况：
```
supervisor> help
 
Documented commands (type help <topic>):
========================================
EOF    exit  maintail  quit    restart   start   stop
clear  help  open      reload  shutdown  status  tail
 
supervisor> help stop
stop <processname>            Stop a process.
stop <processname> <processname>    Stop multiple processes
stop all                Stop all processes
  When all processes are stopped, they are stopped in
  reverse priority order (see config file)
supervisor> help status
status          Get all process status info.
status <name>     Get status on a single process by name.
status <name> <name>    Get status on multiple named processes.
 
#停止某个进程
supervisor> stop publisher_for_summary
publisher_for_summary: stopped
 
#查看此时此刻的状态
supervisor> status
publisher_for_detail RUNNING    pid 27557, uptime 0:05:41
publisher_for_detail_nt RUNNING    pid 27567, uptime 0:05:41
publisher_for_summary STOPPED    Feb 27 02:48 PM
publisher_for_summary_nt RUNNING    pid 27568, uptime 0:05:41
worker_for_detail_all RUNNING    pid 27581, uptime 0:05:41
worker_for_detail_recent RUNNING    pid 27582, uptime 0:05:41
worker_for_summary RUNNING    pid 27559, uptime 0:05:41
#发现被supervisorctl停掉的进程不会被自动重启
 
#开启刚才停掉的进程
supervisor> start publisher_for_summary
publisher_for_summary: started
supervisor> status
publisher_for_detail RUNNING    pid 27557, uptime 0:08:02
publisher_for_detail_nt RUNNING    pid 27567, uptime 0:08:02
publisher_for_summary RUNNING    pid 3035, uptime 0:00:04
publisher_for_summary_nt RUNNING    pid 27568, uptime 0:08:02
worker_for_detail_all RUNNING    pid 27581, uptime 0:08:02
worker_for_detail_recent RUNNING    pid 27582, uptime 0:08:02
worker_for_summary RUNNING    pid 27559, uptime 0:08:02
 
#停掉所有进程
supervisor> stop all
worker_for_detail_recent: stopped
worker_for_detail_all: stopped
publisher_for_summary_nt: stopped
publisher_for_detail_nt: stopped
publisher_for_summary: stopped
worker_for_summary: stopped
publisher_for_detail: stopped
supervisor> status
publisher_for_detail STOPPED    Feb 27 02:51 PM
publisher_for_detail_nt STOPPED    Feb 27 02:51 PM
publisher_for_summary STOPPED    Feb 27 02:51 PM
publisher_for_summary_nt STOPPED    Feb 27 02:51 PM
worker_for_detail_all STOPPED    Feb 27 02:51 PM
worker_for_detail_recent STOPPED    Feb 27 02:51 PM
worker_for_summary STOPPED    Feb 27 02:51 PM
 
#开启所有进程
supervisor> start all
publisher_for_detail: started
worker_for_summary: started
publisher_for_summary: started
publisher_for_detail_nt: started
publisher_for_summary_nt: started
worker_for_detail_all: started
worker_for_detail_recent: started
supervisor> status
publisher_for_detail RUNNING    pid 5111, uptime 0:00:15
publisher_for_detail_nt RUNNING    pid 5141, uptime 0:00:15
publisher_for_summary RUNNING    pid 5135, uptime 0:00:15
publisher_for_summary_nt RUNNING    pid 5147, uptime 0:00:15
worker_for_detail_all RUNNING    pid 5153, uptime 0:00:15
worker_for_detail_recent RUNNING    pid 5159, uptime 0:00:14
worker_for_summary RUNNING    pid 5112, uptime 0:00:15
```
参考资料：
http://liyangliang.me/posts/2015/06/using-supervisor/
http://www.alphadevx.com/a/455-Installing-Supervisor-and-Superlance-on-CentOS
http://blog.csdn.net/xia7139/article/details/9033483
