# 嵌入式linux中使用 DHCP功能描述 - DoubleLi - 博客园






嵌入式linux中使用 DHCP功能描述



busybox中udhcpc的默认script脚本地址为
#define DEFAULT_SCRIPT   CONFIG_UDHCPC_DEFAULT_SCRIPT
#define CONFIG_UDHCPC_DEFAULT_SCRIPT "/usr/share/udhcpc/default.script"
我想之所以以它作为默认地址,肯定有他的道理,所以在这个地址上创建它[luther.gliethttp]

[luther@gliethttp:/vobs/initramfs$](mailto:luther@gliethttp:/vobs/initramfs%24) mkdir usr/share/udhcpc/ -p
[luther@gliethttp:/vobs/initramfs$](mailto:luther@gliethttp:/vobs/initramfs%24) cp ~/busybox-1.14.3/examples/udhcp/simple.script usr/share/udhcpc/default.script
[luther@gliethttp:/vobs/initramfs$](mailto:luther@gliethttp:/vobs/initramfs%24) chmod +x usr/share/udhcpc/default.script
在/etc/inittab中加入下面一行语句:
::once:"/sbin/udhcpc >/dev/null 2>&1" # udhcpc会退出,所以不能使用respawn,必须为once,1次,否则将一直出现udhcpc重启log信息
就这样简单的几行ip和dns就能自动获取了[luther.gliethttp]
其实有些东西就可以放在rcS等启动脚本中,有些可以到init脚本中去完成.
# /sbin/udhcpc





　由于要使用网络通讯，所以不可避免的要用到dhcp。理想的网络通讯方式是下面3种都要支持:

　　1,接入已有网络。这便要求可以作为dhcp客户端。

　　2,作为DHCP服务器，动态分配IP。

　　3,指定固定IP

　　第3种情况没有什么好说的，简单说下前2种情况。

　　使用步骤：

　　(1)在内核的网络项里面把DHCP配置上;

　　(2)在busybox里面把[*]udhcp server(udhcpd)

　　[*]udhcp client(udhcpc)都选上。

　　udhcpd就是终端设备作为DHCP服务器

　　udhcpc就是终端设备作为DHCP客户端

　　busybox里面对dhcp都已经给出例子了，

　　[zhh@localhost busybox-1.14.1]$ ls ./examples/udhcp/

　　sample.bound sample.deconfig sample.nak sample.renew sample.script simple.script udhcpd.conf

　　比如使用udhcpc时

　　就可以直接把simple.script拿来使用，改不改名字都可以，busybox里面默认的目录文件是/usr/share/udhcpc/default.script

　　可以查看下帮助

　　# udhcpc --help

　　BusyBox v1.14.1 (2010-01-22 10:35:16 CST) multi-call binary

　　Usage: udhcpc [-Cfbnqtvo] [-c CID] [-V VCLS] [-H HOSTNAME] [-i INTERFACE]

　　[-p pidfile] [-r IP] [-s script] [-O dhcp-option]... [-P N]

　　-V CLASSID Vendor class identifier

　　-i INTERFACE Interface to use (default: eth0)

　　-H,-h HOSTNAME Client hostname

　　-c CLIENTID Client identifier

　　-C Suppress default client identifier

　　-p file Create pidfile

　　-r IP IP address to request

　　-s file Run file at DHCP events (default /usr/share/udhcpc/default.script)

　　-t N Send up to N request packets

　　-T N Try to get a lease for N seconds (default 3)

　　-A N Wait N seconds (default 20) after failure

　　-O OPT Request DHCP option OPT (cumulative)

　　-o Do not request any options (unless -O is also given)

　　-f Run in foreground

　　-b Background if lease is not immediately obtained

　　-S Log to syslog too

　　-n Exit with failure if lease is not immediately obtained

　　-q Quit after obtaining lease

　　-R Release IP on quit

　　-P N Use port N instead of default 68

　　-a Use arping to validate offered address

　　#

　　这样就很好理解了。

　　下面就说说使用udhcpd，同样可以直接把busybox自带的udhcpd.conf作为参考，怎么设置该文件就不详述了，去参考相关文档即可。看看udhcpd怎么使用

　　# udhcpd --help

　　BusyBox v1.14.1 (2010-01-22 10:35:16 CST) multi-call binary

　　Usage: udhcpd [-fS] [-P N] [configfile]

　　DHCP server

　　-f Run in foreground

　　-S Log to syslog too

　　-P N Use port N instead of default 67

　　帮助一目了然。

　　我使用的时候直接

　　#udhcpd /etc/udhcpd.conf

　　即可。

　　还有一个租约文件要简要说说，使用udhcpd时会要求建立一个租约文件，按照提示创建一个就行了，它起什么作用呢?就是为了记录客户端所获得的IP，如果没有租约文件，则会重新给客户端分配一个新IP，如果有，则使用原先分配的IP。

[http://blog.sina.com.cn/s/blog_6d7e4eba0100r64r.html](http://blog.sina.com.cn/s/blog_6d7e4eba0100r64r.html)









