# 用Supervisor 守护Golang 应用进程的配置示例 - 三少GG - CSDN博客
2015年12月22日 00:55:27[三少GG](https://me.csdn.net/scut1135)阅读数：3801

Supervisor 可以监控服务器的进程，并在出现问题是自动重启。用Supervisor 来守护Golang 应用进程是不错的选择。
下面是一份用Supervisor 守护Golang 应用进程的配置：
```bash
[program:yourapp]
command=/home/yourappuser/bin/yourapp # the location of your app
autostart=true
autorestart=true
startretries=10
user=yourappuser # the user your app should run as (i.e. *not* root!)
directory=/srv/www/yourapp.com/ # where your application runs from
environment=APP_SETTINGS="/srv/www/yourapp.com/prod.toml" # environmental variables
redirect_stderr=true
stdout_logfile=/var/log/supervisor/yourapp.log # the name of the log file.
stdout_logfile_maxbytes=50MB
stdout_logfile_backups=10
```

###
1. 启动下观察效果,出现了一个错误。
**[python]**[view
 plain](http://blog.csdn.net/orangleliu/article/details/41317887#)[copy](http://blog.csdn.net/orangleliu/article/details/41317887#)[print](http://blog.csdn.net/orangleliu/article/details/41317887#)[?](http://blog.csdn.net/orangleliu/article/details/41317887#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/526883/fork)
- [root@localhost ~]# supervisord -c ./supervisord.conf 
- Error: .ini file does not include supervisord section  
- For help, use /usr/bin/supervisord -h  
意思是少了 [supervisrod] 配置项，可以参考 [supervisord-section-settings](http://supervisord.org/configuration.html#supervisord-section-settings), 修改配置文件如下，还有log文件最好先建立好，supervisord不会自己创建。
2. 
# [解决ubuntu-supervisor-unix:var-run-supervisor.sock-no-such-file.的方法](http://tuzii.me/diary/522dc528848eea683d7724f2/%E8%A7%A3%E5%86%B3ubuntu-supervisor-unix:var-run-supervisor.sock-no-such-file.%E7%9A%84%E6%96%B9%E6%B3%95)
` sudo easy_install supervisor==3.2.0`
**注意** 这里就是重点:
**通过apt-get安装的supervisor 版本号是3.0b2, 而当你第一次安装这个版本之后 再用easy_install的话 就也是这个版本 而不是最新版的3.2.0**
然后生成配置文件(root权限):
`  echo_supervisord_conf > /etc/supervisord.conf`
再然后:
`  sudo supervisord`
再然后:
`   sudo supervisorctl`
启动成功.
最后说明:
使用easy_install安装的supervisor的默认sock文件 会被定义在/tmp下面 而apt-get会在/var/run下面. 鉴于apt-get会出现各种问题 这里建议使用easy_install或是pip安装 supervisor.

golang:   https://astaxie.gitbooks.io/build-web-application-with-golang/content/zh/12.3.html
