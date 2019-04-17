# udhcpc 参数使用说明 - DoubleLi - 博客园






当没有网络的时候，板子一直发送dhcp请求，导致程序不往下执行，解决的办法是把它切换到后台运行，可是如何切换到后台呢，有办法，它自带参数可以实现该功能。如下：

   udhcpc -b -i eth0 -p /var/run/udhcpc.pid -R

解释一下，-b就是切换到后台指令，-i是指定使用哪个网络接口，双网卡的时候一定要使用它来指定eth0  or  eth1。

【最近新添加 -R】

对于 -R参数，其实也很重要，在关闭udhcpc时，可以向dhcpserver发送release取消租约。

下面引用网友的一篇文章：

udhcpc 的配置记录


udhcpc scripts

Here's some debian scripts I wrote for udhcpc 0.9.6. I wanted to make it easy to run local scripts for DHCP changes so I created /etc/udhcpc.d. All the /usr/share/udhcpc/default.* scripts have been merged into /etc/udhcpc.d/S50default. S99local is an example script that patches resolv.conf and calls a DNS update script.

/etc/init.d/udhcpc
/etc/udhcpc.script
/etc/udhcpc.d/S50default
/etc/udhcpc.d/S99local

Last modified: Mon Feb 4 10:23:49 PST 2002


单调的分割线


http://udhcp.busybox.net/README.udhcpc

udhcp client (udhcpc)
--------------------

The udhcp client negotiates a lease with the DHCP server and notifies
a set of scripts when a leases is obtained or lost.


command line options
-------------------

The command line options for the udhcp client are:

-c, --clientid=CLIENTID         Client identifier
-H, --hostname=HOSTNAME         Client hostname
-h,    Alias for -H
-f, --foreground                Do not fork after getting lease
-b, --background                Fork to background if lease cannot be
                                immediately negotiated.
-i, --interface=INTERFACE       Interface to use (default: eth0)
-n, --now                       Exit with failure if lease cannot be
                                immediately negotiated.
-p, --pidfile=file              Store process ID of daemon in file
-q, --quit                      Quit after obtaining lease
-r, --request=IP                IP address to request (default: none)
-s, --script=file               Run file at dhcp events (default:
                                /usr/share/udhcpc/default.script)
-v, --version                   Display version


If the requested IP address cannot be obtained, the client accepts the
address that the server offers.


udhcp client scripts
-------------------

When an event occurs, udhcpc calls the action script. The script by
default is /usr/share/udhcpc/default.script but this can be changed via 
the command line arguments. The three possible arguments to the script 
are:

 deconfig: This argument is used when udhcpc starts, and
 when a leases is lost. The script should put the interface in an
 up, but deconfigured state, ie: ifconfig $interface 0.0.0.0.

 bound: This argument is used when udhcpc moves from an
 unbound, to a bound state. All of the paramaters are set in
 enviromental variables, The script should configure the interface,
 and set any other relavent parameters (default gateway, dns server, 
 etc).

 renew: This argument is used when a DHCP lease is renewed. All of
 the paramaters are set in enviromental variables. This argument is
 used when the interface is already configured, so the IP address,
 will not change, however, the other DHCP paramaters, such as the
 default gateway, subnet mask, and dns server may change.

 nak: This argument is used with udhcpc receives a NAK message.
 The script with the deconfig argument will be called directly
 afterwards, so no changes to the network interface are neccessary.
 This hook is provided for purely informational purposes (the
 message option may contain a reason for the NAK).

The paramaters for enviromental variables are as follows:

 $HOME  - The set $HOME env or "/"
 $PATH  - the set $PATH env or "/bin:/usr/bin:/sbin:/usr/sbin"
 $1  - What action the script should perform
 interface - The interface this was obtained on
 ip  - The obtained IP
 siaddr  - The bootp next server option
 sname  - The bootp server name option
 boot_file - The bootp boot file option
 subnet  - The assigend subnet mask
 timezone - Offset in seconds from UTC
 router  - A list of routers
 timesvr  - A list of time servers
 namesvr  - A list of IEN 116 name servers
 dns  - A list of DNS server
 logsvr  - A list of MIT-LCS UDP log servers
 cookiesvr - A list of RFC 865 cookie servers
 lprsvr  - A list of LPR servers
 hostname - The assigned hostname
 bootsize - The length in 512 octect blocks of the bootfile
 domain  - The domain name of the network
 swapsvr  - The IP address of the client's swap server
 rootpath - The path name of the client's root disk
 ipttl  - The TTL to use for this network
 mtu  - The MTU to use for this network
 broadcast - The broadcast address for this network
 ntpsrv  - A list of NTP servers
 wins  - A list of WINS servers
 lease  - The lease time, in seconds
 dhcptype - DHCP message type (safely ignored)
 serverid - The IP of the server
 message  - Reason for a DHCPNAK
 tftp  - The TFTP server name
 bootfile - The bootfile name

additional options are easily added in options.c.


note on udhcpc's random seed
---------------------------

udhcpc will seed its random number generator (used for generating xid's)
by reading /dev/urandom. If you have a lot of embedded systems on the same
network, with no entropy, you can either seed /dev/urandom by a method of
your own, or doing the following on startup:

ifconfig eth0 > /dev/urandom

in order to seed /dev/urandom with some data (mac address) unique to your
system. If reading /dev/urandom fails, udhcpc will fall back to its old
behavior of seeding with time(0).


signals accepted by udhcpc
-------------------------

udhcpc also responds to SIGUSR1 and SIGUSR2. SIGUSR1 will force a renew state,
and SIGUSR2 will force a release of the current lease, and cause udhcpc to
go into an inactive state (until it is killed, or receives a SIGUSR1). You do
not need to sleep between sending signals, as signals received are processed
sequencially in the order they are received.


compile time options
-------------------

options.c contains a set of dhcp options for the client:

 name[10]: The name of the option as it will appear in scripts

 flags: The type of option, as well as if it will be requested
 by the client (OPTION_REQ)

 code: The DHCP code for this option









