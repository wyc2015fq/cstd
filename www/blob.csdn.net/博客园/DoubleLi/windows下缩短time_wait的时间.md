# windows下缩短time_wait的时间 - DoubleLi - 博客园






最近线上遇到windows机器访问其他机器的时候失败的情况。实际就是本地的端口不够用造成的。


D:\>netsh interface ipv4 show dynamicportrange protocol=tcp


Protocol tcp Dynamic Port Range
---------------------------------
Start Port : 49152
Number of Ports : 16384


D:\>netsh interface ipv4 show tcpstats


MIB-II TCP Statistics
------------------------------------------------------
Timeout Algorithm: Van Jacobson's Algorithm
Minimum Timeout: 10
Maximum Timeout: 4294967295
Maximum Connections: Dynamic
Active Opens: 402947
Passive Opens: 989620
Attempts Failed: 122047
Established Resets: 49384
Currently Established: 135
In Segments: 47651175
Out Segments: 45510477
Retransmitted Segments: 465369
In Errors: 0
Out Resets: 65865
这个能用的端口实际就16384个,可以直接 netstat -atn查看一下每个状态的数量。如果对外做大量的请求会造成本地端口不够用。


可以修改一下起始端口:
netsh int ipv4 set dynamicport tcp start=2000 num=63000
netsh int ipv4 set dynamicport udp start=2000 num=63000
netsh int ipv6 set dynamicport tcp start=2000 num=63000
netsh int ipv6 set dynamicport udp start=2000 num=63000


然后修改一下注册表缩短time-wait的时间。
regedit打开注册表，添加
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters
“MaxUserPort”=dword:0000fffe
“TcpTimeWaitDelay”=dword:0000005 (缩短为5秒）









