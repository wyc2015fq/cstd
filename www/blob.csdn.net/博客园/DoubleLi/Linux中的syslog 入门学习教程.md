# Linux中的syslog 入门学习教程 - DoubleLi - 博客园






syslog是linux系统中默认的日志守护进程，默认的syslog配置文件是/etc/syslog.conf文件。程序、守护进程和内核提供了访问系统的日志信息。任何希望生成日志信息的程序都可以向syslog接口呼叫产生该信息。

![QQ图片20150408113433.jpg](http://www.xitongbuluo.com/d/file/jiaocheng/qita/2015-04-08/bfae4c0efa77cb1faa8b0bb50ee430d5.jpg)

几乎所有的网络设备都可以通过syslog协议，将日志信息通过udp方式传送到远端的服务器，而远端的服务器通过syslogd监听udp的514端口，并且根据syslog.conf配置文件中的配置进行处理，接受访问系统的日志信息，把指定的事件写入到特定的文件中，供后台数据库管理和响应之用。它意味着我们可以让任何事件都登录到一台或者多台服务器上，以备后台数据库用off-line也就是离线的方式来分析远端设备的事件。

而/etc/syslog.conf使用facility.level action的方式，而facility.level为选择条件，它本身是两个字段，之间用一个小数点分割，前一个字段是一个服务，后一个字段是一个优先级。选择条件其实是对消息类型的一种分类，这种分类便于人们把不同类型的消息发送到不同的地方。在同一个syslog配置上允许出现一个以上的选择条件，但是必须使用分号隔开。而action字段所表示的活动具有许多灵活性，特别是我们可以使用管道来使得syslogd生成后处理信息。

通常facility指的是syslog可以检测的功能，其中kern指的是内核信息，它通过klogd来传送，而user是用户进程，而mail是邮件，而daemon是后台进程，而authpriv是授权信息，而syslog是系统日志，而lpr是打印信息，而new是新闻组信息，而uucp则是由uucp产生的信息，而cron则是计划和任务信息，而mark则是由syslog内部功能用于生成时间戳。而local0-7与自定义程序使用，比如使用local5表示ssh功能。而*表示出了mark之外的所有功能。

其中level指的是syslog的优先级，其中emerg或者panic表示系统不可用，而alert表示需要立即被修改的条件，而crit表示阻止某些工具或者子系统功能实现的错误条件，err表示阻止工具或者某些子系统部分功能实现的错误条件，而warning是警告消息，而notice则是普通消息，而info则是通知性消息。

syslog的守护程序是由/etc/rc.d/init.d/syslog脚本被调用启动的，缺省不使用选项。如果将要使用一个日志服务器，必须调用syslogd -r，缺省情况下syslog不接受来自远程系统的消息。当指定了-r选项后，syslogd会监听从514端口上进来的udp包。

如果还希望日志服务器能够传送日志消息，则可以使用-h标识。缺省时，syslogd将忽略使其从一个远程系统传送日志消息到另一个系统的/etc/syslog.conf输入项。

对配置文件的修改需要重启syslogd程序才会生效，可以使用/etc/rc.d/init.d/syslog restart即可。更多精彩内容欢迎继续访问系统部落(www.xitongbuluo.com)！









