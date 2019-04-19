# Linux 日志定时轮询流程详解 - 文章 - 伯乐在线
原文出处： [cooffeelis](http://www.jianshu.com/p/ea7c2363639c)
### logrotate介绍
对于Linux系统安全来说，日志文件是极其重要的工具。日志文件包含了关于系统中发生的事件的有用信息，在排障过程中或者系统性能分析时经常被用到。当日志文件不断增长的时候，就需要定时切割，否则，写日志的速度和性能也会下降，更不便于我们归档，查询。
所以便有了使用logrotate的时候 ，logrotate是个十分有用的工具，它可以自动对日志进行截断（或轮循）、压缩以及删除旧的日志文件。例如，你可以设置logrotate，让/var/log/foo日志文件每30天轮循，并删除超过6个月的日志。配置完后，logrotate的运作完全自动化，不必进行任何进一步的人为干预。
### logrotate配置文件位置
Linux系统默认安装logrotate工具，它默认的配置文件在：


```
/etc/logrotate.conf
/etc/logrotate.d/
```
logrotate.conf 才主要的配置文件，logrotate.d 是一个目录，该目录里的所有文件都会被主动的读入/etc/logrotate.conf中执行。
另外，如果 /etc/logrotate.d/ 里面的文件中没有设定一些细节，则会以/etc/logrotate.conf这个文件的设定来作为默认值。
实际运行时，Logrotate会调用配置文件/etc/logrotate.conf。
可以在/etc/logrotate.d目录里放置自定义好的配置文件，用来覆盖Logrotate的缺省值。
### 定时轮循机制
**Logrotate是基于CRON来运行的，其脚本是/etc/cron.daily/logrotate，日志轮转是系统自动完成的。**
logrotate这个任务默认放在cron的每日定时任务cron.daily下面 /etc/cron.daily/logrotate/etc/目录下面还有cron.weekly/, cron.hourly/, cron.monthly/ 的目录都是可以放定时任务的


```
[/etc]$ cat /etc/cron.daily/logrotate
#!/bin/sh
# Clean non existent log file entries from status file
cd /var/lib/logrotate
test -e status || touch status
head -1 status > status.clean
sed 's/"//g' status | while read logfile date
do
    [ -e "$logfile" ] && echo "\"$logfile\" $date"
done >> status.clean
mv status.clean status
test -x /usr/sbin/logrotate || exit 0
/usr/sbin/logrotate /etc/logrotate.conf
```
这里实际操作轮询的命令最后一行
`/usr/sbin/logrotate /etc/logrotate.conf`
定义好了每日执行任务的脚本cron.daily/logrotate ，再查看crontab的内容，里面设置好了对应的cron.xxly
执行时间


```
[/etc]$ vim /etc/crontab
SHELL=/bin/sh
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
# m h dom mon dow user    command
17 *    * * *    root    cd / && run-parts --report /etc/cron.hourly
25 6    * * *    root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.daily )
47 6    * * 7    root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.weekly )
52 6    1 * *    root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.monthly )
```
可以看出来了只要是在
- /etc/cron.daily/ 下面的任务都是每天6：25 执行
- /etc/cron.weekly/ 下面的任务都是每周日 6：47 执行
- /etc/cron.monthly/ 下面的任务都是每月1号 6：52 执行
如果等不及cron自动执行日志轮转，想手动强制切割日志，需要加-f参数；
不过正式执行前最好通过Debug选项来验证一下（-d参数），这对调试也很重要


```
# /usr/sbin/logrotate -f /etc/logrotate.d/nginx  // 未到时间或者未到切割条件，强制切割
# /usr/sbin/logrotate -d -f /etc/logrotate.d/nginx // 输出切割debug信息
```
那么至此，我们就知道logrotate是如何实现自动切割日志的
### logrotate配置案例
nginx 常用日志切割配置


```
/data/log/nginx/*.log /data/log/nginx/*/*.log { # 对匹配上的日志文件进行切割
    weekly # 每周切割
    missingok     # 在日志轮循期间，任何错误将被忽略，例如“文件无法找到”之类的错误。
    rotate 6      # 保留 6 个备份
    compress     # 压缩
    delaycompress    # delaycompress 和 compress 一起使用时，转储的日志文件到下一次转储时才压缩
    notifempty     # 如果是空文件的话，不转储
    create 0644 www-data ymserver     # mode owner group 转储文件，使用指定的文件模式创建新的日志文件
    sharedscripts # 下面详细说
    prerotate # 在logrotate转储之前需要执行的指令，例如修改文件的属性等动作；必须独立成行
        if [ -d /etc/logrotate.d/httpd-prerotate ]; then \
            run-parts /etc/logrotate.d/httpd-prerotate; \
        fi \
    endscript
    postrotate  # 在logrotate转储之后需要执行的指令，例如重新启动 (kill -HUP) 某个服务！必须独立成行
        [ -s /run/nginx.pid ] && kill -USR1 `cat /run/nginx.pid`
    endscript
    su root ymserver # 轮训日志时切换设置的用户/用户组来执行（默认是root），如果设置的user/group 没有权限去让文件容用 create 选项指定的拥有者 ，会触发错误。
}
```
如果要配置一个每日0点执行切割任务，怎么做到？我们的logrotate默认每天执行时间已经写到了/etc/cron.daily/目录下面，而这个目录下面的任务执行时间上面也说了，在/etc/crontab里面定义了时6：25。我之前就有个这样的需求，看看下面的配置


```
/data/log/owan_web/chn_download_stat/chn_app_rec.log {
    copytruncate
    # weekly 注释了 但是会继承/etc/logrorate.conf的全局变量，也是weekly
    missingok
    rotate 10
    compress
    delaycompress
    size=1000M # 大小到达size开始转存
    notifempty
    create 664 www-data ymserver
    su root
    dateext       //这个参数很重要！就是切割后的日志文件以当前日期为格式结尾，如xxx.log-20131216这样,如果注释掉,切割出来是按数字递增,即前面说的 xxx.log-1这种格式
    compress      //是否通过gzip压缩转储以后的日志文件，如xxx.log-20131216.gz ；如果不需要压缩，注释掉就行
}
```
然后去root的crontab配置一个0点执行的任务


```
wwwadm@host:/etc/logrotate.d$ sudo crontab -l -u root
0 0 * * * /usr/sbin/logrotate /etc/logrotate.d/web_roteate -fv  >/tmp/logro.log 2>&1
```
因为logrotate的切割周期是weekly，每次切割都是根据上一个切割的时间来进行，如果距离上一次有一周时间，就会切割，但是我们设置了crontab的每天切割，既不会进入/etc/cron.daily/的每日切割，也不会每周切割。这样就能完美定制自己想要的切割日志时间
## logrotate参数说明


```
compress 通过gzip 压缩转储以后的日志
nocompress 不做gzip压缩处理
create mode owner group 轮转时指定创建新文件的属性，如create 0777 nobody nobody
nocreate 不建立新的日志文件
delaycompress 和compress 一起使用时，转储的日志文件到下一次转储时才压缩
nodelaycompress 覆盖 delaycompress 选项，转储同时压缩。
missingok 如果日志丢失，不报错继续滚动下一个日志
ifempty 即使日志文件为空文件也做轮转，这个是logrotate的缺省选项。
notifempty 当日志文件为空时，不进行轮转
mail address 把转储的日志文件发送到指定的E-mail 地址
olddir directory 转储后的日志文件放入指定的目录，必须和当前日志文件在同一个文件系统
noolddir 转储后的日志文件和当前日志文件放在同一个目录下
sharedscripts 运行postrotate脚本，作用是在所有日志都轮转后统一执行一次脚本。如果没有配置这个，那么每个日志轮转后都会执行一次脚本
prerotate 在logrotate转储之前需要执行的指令，例如修改文件的属性等动作；必须独立成行
postrotate 在logrotate转储之后需要执行的指令，例如重新启动 (kill -HUP) 某个服务！必须独立成行
daily 指定转储周期为每天
weekly 指定转储周期为每周
monthly 指定转储周期为每月
rotate count 指定日志文件删除之前转储的次数，0 指没有备份，5 指保留5 个备份
dateext 使用当期日期作为命名格式
dateformat .%s 配合dateext使用，紧跟在下一行出现，定义文件切割后的文件名，必须配合dateext使用，只支持 %Y %m %d %s 这四个参数
size(或minsize) log-size 当日志文件到达指定的大小时才转储，log-size能指定bytes(缺省)及KB (sizek)或MB(sizem).
当日志文件 >= log-size 的时候就转储。 以下为合法格式：（其他格式的单位大小写没有试过）
size = 5 或 size 5 （>= 5 个字节就转储）
size = 100k 或 size 100k
size = 100M 或 size 100M
```
**值得注意的一个配置是：copytruncate**
`copytruncate`*如果没有这个选项的话，操作方式：是将原log日志文件，移动成类似log.1的旧文件， 然后创建一个新的文件。 如果设置了，操作方式：拷贝原日志文件，并且将其变成大小为0的文件。*
区别是如果进程,比如nginx 使用了一个文件写日志，没有copytruncate的话，切割日志时， 把旧日志log->log.1 ，然后创建新日志log。这时候nginx 打开的文件描述符依然时log.1，由没有信号通知nginx 要换日志描述符，所以它会继续向log.1写日志，这样就不符合我们的要求了。 因为我们想切割日志后，nginx 自动会向新的log 文件写日志，而不是旧的log.1文件
解决方法有两个：
1.向上面的nginx 切割日志配置，再postrotate里面写个脚本


```
postrotate  # 在logrotate转储之后需要执行的指令，例如重新启动 (kill -HUP) 某个服务！必须独立成行
    [ -s /run/nginx.pid ] && kill -USR1 `cat /run/nginx.pid`
endscript
```
这样就是发信号给nginx ,让nginx 关闭旧日志文件描述符，重新打开新的日志文件描述，并写入日志
2.使用`copytruncate`参数，向上面说的，配置了它以后，操作方式是把log 复制一份 成为log.1，然后清空log的内容，使大小为0，那此时log依然时原来的旧log，对进程（nginx）来说，依然打开的是原来的文件描述符，可以继续往里面写日志，而不用发送信号给nginx
copytruncate这种方式操作的时候， 拷贝和清空之间有一个时间差，可能会丢失部分日志数据。
`nocopytruncate` 备份日志文件不过不截断
### 参考
《[Linux下logrotate日志轮询操作梳理](http://blog.jobbole.com/111385/)》
