
# Python Supervisord 进程管理工具 - 阳光岛主 - CSDN博客

2014年03月08日 20:59:34[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：17717


**Supervisord**，用Python实现的一款非常实用的进程管理工具，在批量服务化管理时特别有效。
环境
centos
python2.6.6
安装
wget http://pypi.python.org/packages/source/s/supervisor/supervisor-3.0a10.tar.gz
tar zxvf supervisor-3.0a10.tar.gz
cd supervisor-3.0a10
python setup.py install
如果上述遇到问题，请按如下操作。
依赖setuptools安装
wgethttps://pypi.python.org/packages/2.6/s/setuptools/setuptools-0.6c11-py2.6.egg
sh setuptools-0.6c11-py2.6.egg
更多[setuptools](https://pythonhosted.org/setuptools/)
产生基础配置文件
echo_supervisord_conf > /etc/supervisord.conf
配置
[inet_http_server]前面的分号去掉; 如下：
![](https://img-blog.csdn.net/20140212163201531)

启动
/usr/local/bin/supervisord -c /etc/supervisord.conf

**关闭**
username=user
passwd=123
/usr/local/bin/supervisorctl -u{$username}-p{$passwd} stop all
/usr/local/bin/supervisorctl -u{$username} -p{$passwd} shutdown

查看
浏览器打开[http://127.0.0.1:9001](http://192.168.1.1:8888/)
如下图：
![](https://img-blog.csdn.net/20140212163441781)

深入配置
[program:service]
command=sh /opt/soft/service/run.sh  ; the program (relative uses PATH, can take args)
process_name=%(program_name)s ; process_name expr (default %(program_name)s)
numprocs=1                    ; number of processes copies to start (def 1)
directory=/opt/soft/service/                ; directory to cwd to before exec (def no cwd)
autostart=true                ; start at supervisord start (default: true)
autorestart=true              ; retstart at unexpected quit (default: true)
stopsignal=QUIT               ; signal used to kill process (default TERM)
stopwaitsecs=10               ; max num secs to wait b4 SIGKILL (default 10)
以上配置了一个shell脚本的启动和stop过程。
一般java程序都喜欢在一个shell脚本内初始化classpath，再执行java main。
直接在shell中run java会出现stop失败的情况，在启动前添加 exec即可。
> \#!/bin/sh

> export CLASSPATH=.

> for jarpath in `ls library/*.jar`

> do

> CLASSPATH=
> $CLASSPATH:$jarpath

> done

> export CLASSPATH=$CLASSPATH

> exec java com.chen.main;

帮助
supervisord，初始启动Supervisord，启动、管理配置中设置的进程。
supervisorctl stop programxxx，停止某一个进程(programxxx)，programxxx为[program:chatdemon]里配置的值，这个示例就是chatdemon。
supervisorctl start programxxx，启动某个进程
supervisorctl restart programxxx，重启某个进程
supervisorctl stop all，停止全部进程，注：start、restart、stop都不会载入最新的配置文件。
supervisorctl reload，载入最新的配置文件，并按新的配置启动、管理所有进程。
http://smarden.org/runit/runscripts.html 一些可参考的启动配置

注意
不带参数运行supervisord是以daemon方式运行。
把supervisord加入到开机启动项里就可以完成监控进程的功能了。
当supervisord以非daemon方式运行时，杀掉supervisord后，被监控的进程也退出了。
而以daemon方式运行，杀掉supervisord对被监控进程无影响。


**参考推荐：**
[Supervisord管理Python进程](http://feilong.me/2011/03/monitor-processes-with-supervisord)
[Supervisord管理杂乱的服务](http://bbs.chinaunix.net/thread-3598458-1-1.html)
[Linux /proc/pid记录了什么](http://www.linuxidc.com/Linux/2012-09/70975.htm)



