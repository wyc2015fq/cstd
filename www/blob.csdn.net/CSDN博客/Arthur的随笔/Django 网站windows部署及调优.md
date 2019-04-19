# Django 网站windows部署及调优 - Arthur的随笔 - CSDN博客
2012年12月29日 10:28:27[largetalk](https://me.csdn.net/largetalk)阅读数：11710
在过去的一周，由于客户的需要，将我以前用django写的网站部署到windows上并做了相应压力测试，该项目原来写的时候目标是linux，部署到windows修改的比较多的地方就是和文件路径相关的地方了，所有直接拼接路径的地方都改成os.path.join，然后基本就能跑起来了。
软件所需大部分包在windows下都能安装，但gunicorn和uwsgi在window下不能安装，所以只好放弃原来的部署方式。在window下一共尝试过nginx + fcgi, nginx + scgi, nginx + gevent + wsgi三种部署方式。性能测试上fcgi和gevent相差无几， scgi略高，但scgi(flup) 和python logging模块不兼容，使得程序无法写日志，这点无法接受。 而gevent启动没有一个好的管理方式，最后选定使用fcgi部署。
由于在windows上无法使用prefork启动fcgi，所以在8核cpu机器上启动了8个fcgi进程，然后用nginx做转发。每个进程启动方式如下：
python manage.py runfcgi protocol=fcgi method=threaded maxspare=40 host=127.0.0.1 port=xxxx
fcgi的一些参数：
maxspare: 在启动时就初始话这个多个 thread/process， 测试下来，能微微提高一点性能
minspare: 最小空闲 thread/process
maxrequests: 每个thread/process 在处理了这么多请求之后就会被销毁， 防止内存泄漏
maxchild: 能产生最多的 thread/子进程
好了，部署方式确定了，下面就是调优了
mysql：
使用了msyql，使用了innodb
general_log=0 #general 会影响一些性能
innodb_flush_log_at_trx_commit=2 #这条对性能影响很大， 为1时是每次commit都实时保存到硬盘，最安全， 为0时是**一秒一次**把commit添加到日志，然后写到硬盘， 最快， 为2时是每次commit都添加到日志，然后一秒一次保存到硬盘，性能和安全折中。（由于系统原因，1秒并不能保证是真正意义上的1秒）
对于安全， 值为1时最安全，mysql和系统崩溃都不会丢失数据， 为0时任一崩溃都有可能丢失最后1，2秒的数据，为2时只有在系统崩溃时才可能丢失最后1，2秒数据。
max_connect_errors=100000 #在linux下，这个值我也没设过，在window下发现这个值默认为10，即某个主机在10次链接错误之后就不允许该主机再次链接了，必须flush hosts之后才可以。这导致我测试的时候出现一旦某个用户出错之后所有测试都立马出错了， shit， 改大点。
innodb_buffer_pool_size=128M #测试的时候，我的数据集不大，这个值对我影响不大，而且，window下改的太大mysql有可能启动不起来。该值默认是为0，稍微改大点。
windows:
windows xp, 2003都有tcp链接数限制
修改注册表中下面两项：
HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\tcpip\Parameters\TcpTimedWaitDelay to 30 
and HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\tcpip\Parameters\MaxUserPort to 65534 
然后重启系统
TIME_WAIT状态：在测试会发现有很多处于TIME_WAIT状态的tcp连接， 在关闭tcp连接时，调用close的一端进入TIME_WAIT状态，然后等待对方的ACK之后才能关闭这个连接，TIME_WAIT时间长度是2*MSL（MSL:最大分段生存时间）， 在TIME_WAIT状态下，这个端口都不可被重用，通过修改住册表修改这个值，有人说这个值改为0也没有关系，只会降低一些服务器稳定性。
nginx：
nginx在window下可设置的东西不多，和fastcig的某些需要设置一下
 fastcgi_buffer_size 8k;
 fastcgi_buffers 256 8k;
这两个值可以稍微设大点，在负载大的情况下， 值小了会出现502错误。只要nginx error log没有“an upstream response is buffered to a temporary file"错误就可以了。
ps。nginx在window下现在还属于beta，存在不少问题，只能有一个工作进程，使用select模型，不能突破1024个文件描述符限制。
程序：
其实上面的调优的性能的提升加在一起都比如程序架构和本身性能的调优。由于这次测试只有一台机器，架构上的扩展性很小，只能从程序本身找一些性能提升点。
缓存：对性能提升非常厉害，一个缓存的页面和一个动态生成的页面性能差距在几百倍以上，所以有可能缓存的页面最好都缓存。另本地缓存优于memcache，因为memcahce要用到tcp连接。
orm： 我的程序使用了django model orm， 发现其对性能还是有一定影响的。我的一个主要页面，使用orm时平均响应时间是200ms，而我改写该页面，全部使用sql来处理时，响应时间能达到40ms以下。不过这也要在开发效率上做个权衡。
that all. 本次调优到此为止。
