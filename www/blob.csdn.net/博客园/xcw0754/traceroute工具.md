# traceroute工具 - xcw0754 - 博客园
# [traceroute工具](https://www.cnblogs.com/xcw0754/p/8449132.html)
**traceroute**是类Unix的命令工具，用于测试到网络上某台主机**host**的跳数以及延时情况。其运行原理是，通过发送指定TTL的**UDP包**给目的主机host，通过将TTL逐渐增大，就能收到一些**ICMP包**，这些包是从本机到host路径上的网关发出的，用于告知本机TTL已为0，即目的主机host不可达，从而得知路径上网关的跳数和延时情况。由于traceroute发出的是UDP数据包，每个包的端口号是逐渐上升的，可能是为了防止恰好碰到某个网关上的UDP服务，要是碰到了就可能回的不是ICMP包了，**这一点有待验证**！
具体的实现细节是这样的，先一次性发出3个TTL=1探测包，等待回复的ICMP包到达或者超时，将信息打印成一行，接着发出TTL=2的探测包，以此类推，直到TTL达到规定的上限(默认30跳)后终止运行。从这样的实现细节可以看出，它不能用于确定网络出现的情况，网络是千变万化的，经过的网段也复杂多样，两次同样指令的运行结果也可能非常不同，它一般只用来辅助判断网络状况。
### 使用方法
打印出traceroute的用法如下：
```
Usage：
    traceroute [ -46dFITnreAUD ] 
        [ -f first_ttl ] [ -g gate,... ] 
        [ -i device ] [ -m max_ttl ] 
        [ -N squeries ] [ -p port ] 
        [ -t tos ] [ -l flow_label ] 
        [ -w waittime ] [ -q nqueries ] 
        [ -s src_addr ] [ -z sendwait ] 
        [ --fwmark=num ] 
        host 
        [ packetlen ]
```
使用方法比较直观，看第一部分的`[-46dFITnreAUDV]`，因为这部分选项不需要带参数，所以可以连着写。其他的都是有参数的，不能连着写。倒数第二行的`host`就是要追踪的主机，比如可以是`www.baidu.com`。最后一个参数`[packetlen]`是可选的，表示包的长度，我的机子默认是60B，一般不用太关心。
不带参数选项如下：
`-4` ipv4。
`-6` ipv6。
`-d` 打开socket层的debug。
`-F` 禁止分片。
`-I` 使用ICMP ECHO。
`-T` 使用TCP，默认端口80。
`-n` 不打印domaim。
`-r`
`-e` 显示ICMP扩展。
`-A` 在每个地址后面加个AS(autonomous system)标识，如果没有就显示[*]。
`-U` 使用UDP请求，默认端口53。
`-D` 使用DCCP(数据报拥塞控制协议)请求，默认端口33434。
带参数选项如下：
`-f first_ttl` 指定首个报文的TTL值，默认是1。
`-m max_ttl` 指定末个报文TTL值，默认是30。
`-g gate,...` 指定网关，可以多个。
`-i device` 指定网卡，默认应该是eth0。
`-w waittime` 指定等待探测响应时间，默认3秒。
`-q nqueries` 指定每个TTL发几个包，默认3个。
`-p port` 指定host的端口号。
`-N squeries`
`-t tos`
`-l flow_label`
`-s src_addr`
`-z sendwait`
`--fwmark=num`
上面常用的选项其实不多，大多都是使用默认的选项。输出的每一行可能是多种情况，对于发出的具有相同TTL值的包(默认3个)，若回复的都是都是同一个网关，那么显示的结果就是一个IP后面带3个时间，像这样：
`113.96.0.18  66.389 ms  85.115 ms  92.113 ms`
如果回复的都是不同的网关(路径不是唯一的)，那显示的结果就是每个IP后面各带一个时间，像这样：
`14.29.121.186  114.796 ms 14.29.121.190  94.882 ms 14.29.121.186  119.937 ms`
比较常见的显示结果还是三个`*`，那就是发出去的包在3秒内(默认)没有收到回复，这可能是丢包，也可能是网关在收到不想要的包会丢弃，直接把我们发的包给忽略掉。
还有显示得比较奇怪的，像这样`5 * 113.96.251.61  37.077 ms *`，表示第5跳的3个包中只有第2个包得到了回复，往返耗时37ms左右。也肯能是这样`10 14.215.177.38  37.779 ms *  42.592 ms`，表示第10跳的第2个包得不到回复，而第1和第3个包的回复来自相同站点。
TODO：**具体发出的包是什么样的，待抓包分析。**以上只是个人理解，等后面会熟练使用tcpdump了再来更新。
### 帮助
- [AIX命令参考之traceroute 命令](http://www.kuqin.com/aixcmds/aixcmds5/traceroute.htm)
- [源码](https://files.cnblogs.com/files/xcw0754/traceroute-2.1.0.tar.gz)

