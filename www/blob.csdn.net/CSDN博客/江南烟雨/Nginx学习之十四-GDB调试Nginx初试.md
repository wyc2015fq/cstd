# Nginx学习之十四-GDB调试Nginx初试 - 江南烟雨 - CSDN博客
2013年07月19日 18:11:41[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：12649
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)

本文的测试环境：
Win7+虚拟机VMWareVMware-workstation-full-7.1.4-385536+Ubuntu12.04
Nginx-1.4.0
要想有效的研究Nginx源码，必须要动手调试，GDB就是有用的利器。下面简要介绍用GDB调试Nginx的一个小例子。
本文完成了这样的工作：跟踪worker子进程，观察worker子进程阻塞在何处。然后利用wget向nginx发送一个消息，观察整个消息处理过程。
关于GDB调试Nginx，有一篇博文讲的很不错，可以参考下：[利用GDB调试Nginx](http://lenky.info/2011/09/10/%E5%88%A9%E7%94%A8gdb%E8%B0%83%E8%AF%95nginx/)。这篇博客的作者Lenky即《深入剖析Nginx》的作者。这本书讲的也不错，正在研究中。
我们以默认的Nginx配置来进行调试，即master模式。
首先我们切换工作目录至：.../nginx-1.4.-/objs/,然后在终端中启动GDB调试器：sudo gdb -q -tui（q选项是以安静模式启动，不显示GDB版本等信息。tui选项可以显示代码界面）
![](https://img-blog.csdn.net/20130719172701734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在GDB中启动nginx：shell ./nginx
启动之后，可以查看当前nginx中的进程号：shell pidof nginx
![](https://img-blog.csdn.net/20130719173058000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
也可以在另一个终端中通过下列命令来查看：
![](https://img-blog.csdn.net/20130719173218765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们可以用attach命令来跟踪子进程：
![](https://img-blog.csdn.net/20130719173302062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们知道，子进程即worker进程在运行后会停留在epoll_wait处等待相应的事件发生，而这个函数调用被封装在ngx_process_events_and_timers 中。于是我们在这个函数出设置一个断点：b ngx_process_events_and_timers 
![](https://img-blog.csdn.net/20130719175134031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后采用命令c，使得nginx一直运行，直到遇到第一个断点：
![](https://img-blog.csdn.net/20130719175257562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们知道处理事件的方法是ngx_process_events，于是我们跟踪进去这个函数：
![](https://img-blog.csdn.net/20130719175415906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
s命令跟踪进函数里面：
![](https://img-blog.csdn.net/20130719175503187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们用n命令继续向下执行，当执行到epoll_wait函数的时候，发现进程停留在这里，不能在向下执行。这就验证了：worker子进程阻塞在epoll_wait函数调用处。
这时，我们在另一个终端执行下列命令，以向nginx发送消息：wget www.web_test2.com（预先在etc/hosts文件中加入下列行：192.168.2.129 www.web_test2.com.其中192.168.2.129 是本机IP）
![](https://img-blog.csdn.net/20130719175914343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
终端显示，请求已经发送，正在等待回应。
在gdb中用n命令继续执行，直到跳出ngx_process_events函数接口。
其实我们可以通过查看栈帧来看函数调用的整个过程：bt
![](https://img-blog.csdn.net/20130719180456812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
继续执行，最后wget命令的那个终端会收到服务器端的响应：
![](https://img-blog.csdn.net/20130719180830375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考资料：
[http://lenky.info/2011/09/10/%E5%88%A9%E7%94%A8gdb%E8%B0%83%E8%AF%95nginx/](http://lenky.info/2011/09/10/%E5%88%A9%E7%94%A8gdb%E8%B0%83%E8%AF%95nginx/)
