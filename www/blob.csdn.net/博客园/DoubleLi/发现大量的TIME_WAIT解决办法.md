# 发现大量的TIME_WAIT解决办法 - DoubleLi - 博客园






今天早上一上班，有同事就反映公司好几个网站都打不开，登陆数据库

服务器(windows)，发现很卡,于是重启了下服务器，进入系统后，没过一会问题依旧,查看了下系统进程，发现mysql占用率达到99%，可以肯定的是mysql连接出现问题:

netstat -an

192.168.12.13:3306      192.168.12.12:30443      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30444      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30445      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30446      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30447      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30448      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30449      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30450      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30451      TIME_WAIT

192.168.12.13:3306      192.168.12.12:30452      TIME_WAIT

... ...

      根据TCP协议定义的3次握手断开连接规定,发起socket主动关闭的一方 socket将进入TIME_WAIT状态,TIME_WAIT状态将持续2个MSL(Max Segment Lifetime),在Windows下默认为4分钟,即240秒,TIME_WAIT状态下的socket不能被回收使用. 具体现象是对于一个处理大量短连接的服务器,如果是由服务器主动关闭客户端的连接,将导致服务器端存在大量的处于TIME_WAIT状态的socket, 甚至比处于Established状态下的socket多的多,严重影响服务器的处理能力,甚至耗尽可用的socket,停止服务. TIME_WAIT是TCP协议用以保证被重新分配的socket不会受到之前残留的延迟重发报文影响的机制,是必要的逻辑保证.
      在HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters,添加名为TcpTimedWaitDelay的
DWORD键,设置为60,以缩短TIME_WAIT的等待时间



登陆到web服务器(linux)：



     netstat -ae |grep mysql

     tcp        0      0 aaaa:53045               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53044               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53051               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53050               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53049               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53048               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53055               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53054               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53053               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53052               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53059               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53058               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53057               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53056               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53063               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53062               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53061               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53060               192.168.12.3:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53067               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53066               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53065               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53064               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa53071               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53070               192.168.12.13:mysql           TIME_WAIT   root       0
tcp        0      0 aaaa:53069               192.168.12.13:mysql           TIME_WAIT   root       0
发现系统存在大量TIME_WAIT状态的连接，通过调整内核参数解决，

vi /etc/sysctl.conf


编辑文件，加入以下内容：

```
net.ipv4.tcp_syncookies = 1
net.ipv4.tcp_tw_reuse = 1
net.ipv4.tcp_tw_recycle = 1
net.ipv4.tcp_fin_timeout = 30
```


``

``然后执行 `/sbin/sysctl -p` 让参数生效。



**net.ipv4.tcp_syncookies = 1** 表示开启SYN Cookies。当出现SYN等待队列溢出时，启用cookies来处理，可防范少量SYN攻击，默认为0，表示关闭；


**net.ipv4.tcp_tw_reuse = 1** 表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；


**net.ipv4.tcp_tw_recycle = 1** 表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。


**net.ipv4.tcp_fin_timeout** 修改系統默认的 TIMEOUT 时间



修改之后，再用

netstat -ae|grep mysql

tcp        0      0 aaaa:50408               192.168.12.13:mysql           ESTABLISHED nobody     3224651
tcp        0      0 aaaa:50417               192.168.12.13:mysql           ESTABLISHED nobody     3224673
tcp        0      0 aaaa:50419               192.168.12.13:mysql           ESTABLISHED nobody     3224675


       发现大量的TIME_WAIT 已不存在，mysql进程的占用率很快就降下来的，各网站访问正常！！

       以上只是暂时的解决方法，最后仔细巡查发现是前天新上线的一个系统，程序代码中没有使用mysql.colse()，才导致大量的mysql  TIME_WAIT 









