# Linux命令netstat解读 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月06日 15:26:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1044








                
Netstat命令用于显示与IP、TCP、UDP和ICMP协议相关的统计数据，一般用于检验本机各端口的网络连接情况。

 在Internet RFC标准中，Netstat的定义是： Netstat是在内核中访问网络及相关信息的程序，它能提供TCP连接，TCP和UDP监听，进程内存管理的相关报告。



```
# netstat --help
usage: netstat [-veenNcCF] [<Af>] -r         netstat {-V|--version|-h|--help}
       netstat [-vnNcaeol] [<Socket> ...]
       netstat { [-veenNac] -I[<Iface>] | [-veenNac] -i | [-cnNe] -M | -s } [delay]

        -r, --route                display routing table
        -I, --interfaces=<Iface>   display interface table for <Iface>
        -i, --interfaces           display interface table
        -g, --groups               display multicast group memberships
        -s, --statistics           display networking statistics (like SNMP)
        -M, --masquerade           display masqueraded connections

        -v, --verbose              be verbose
        -n, --numeric              don't resolve names
        --numeric-hosts            don't resolve host names
        --numeric-ports            don't resolve port names
        --numeric-users            don't resolve user names
        -N, --symbolic             resolve hardware names
        -e, --extend               display other/more information
        -p, --programs             display PID/Program name for sockets
        -c, --continuous           continuous listing

        -l, --listening            display listening server sockets
        -a, --all, --listening     display all sockets (default: connected)
        -o, --timers               display timers
        -F, --fib                  display Forwarding Information Base (default)
        -C, --cache                display routing cache instead of FIB
        -T, --notrim               stop trimming long addresses
        -Z, --context              display SELinux security context for sockets

  <Iface>: Name of interface to monitor/list.
  <Socket>={-t|--tcp} {-u|--udp} {-S|--sctp} {-w|--raw} {-x|--unix} --ax25 --ipx --netrom
  <AF>=Use '-A <af>' or '--<af>'; default: inet
  List of possible address families (which support routing):
    inet (DARPA Internet) inet6 (IPv6) ax25 (AMPR AX.25) 
    netrom (AMPR NET/ROM) ipx (Novell IPX) ddp (Appletalk DDP) 
    x25 (CCITT X.25)
```


常用命令：

# netstat -an |grep 8080  ：查找8080端口


#netstat -s ：按照各个协议分别显示其统计数据。

# netstat -r ：显示关于路由表的信息。

#netstat -a ：显示一个所有的有效连接信息列表。

#netstat -n ： 显示所有已建立的有效连接。


#netstat -at：显示tcp或udp协议的链接

#netstat -tnl：列出监听端口

#netstat -ant：禁用反向域名解析

#netstat -nlpt：获取进程名、进程号以及用户 ID



