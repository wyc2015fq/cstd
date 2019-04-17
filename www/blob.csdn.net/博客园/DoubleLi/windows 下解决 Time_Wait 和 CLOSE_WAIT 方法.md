# windows 下解决 Time_Wait 和 CLOSE_WAIT 方法 - DoubleLi - 博客园






修改Time_Wait参数的方法 (在服务端修改)
Windows下在HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/Tcpip/Parameters,添加名为TcpTimedWaitDelay的
DWORD键,设置为30,以缩短TIME_WAIT的等待时间 


解决CLOSE_WAIT的方法:(在客户端修改)
1 一般原因都是TCP连接没有调用关闭方法。需要应用来处理网络链接关闭。
2 对于Web请求出现这个原因，经常是因为Response的BodyStream没有调用Close.
比如Widnows下:
使用HttpWebRequest 一定要保证GetRequestStream和GetResponse对象关闭，否则容易造成连接处于CLOSE_WAIT状态
3 TCP的KeepLive功能，可以让操作系统替我们自动清理掉CLOSE_WAIT的连接。
但是KeepLive在Windows操作系统下默认是7200秒，也就是2个小时才清理一次。往往满足不了要求。可以调小该数值。
Windows下的调整方法为
HKEY_LOCAL_MACHINE/CurrentControlSet/Services/Tcpip/Parameters下的以下三个参数： 
KeepAliveInterval,设置其值为1000   www.2cto.com  
KeepAliveTime,设置其值为300000(单位为毫秒，300000代表5分钟) 
TcpMaxDataRetransmissions,设置其值为5
Close_Wait引发的问题:
Close_Wait会占用一个连接，网络可用连接小。数量过多，可能会引起网络性能下降，并占用系统非换页内存。 尤其是在有连接池的情况下(比如HttpRequest)
会耗尽连接池的网络连接数，导致无法建立网络连接


[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters]


“KeepAliveTime”=dword:006ddd00
“KeepAliveInterval”=dword:000003e8
“MaxDataRetries”=”5″









