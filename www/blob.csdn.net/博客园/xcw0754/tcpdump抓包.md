# tcpdump抓包 - xcw0754 - 博客园
# [tcpdump抓包](https://www.cnblogs.com/xcw0754/p/8447362.html)
`tcpdump`是个Linux抓包工具，由于它是命令行的，打印出来的东西也是纯文本的，可以输出为pcap类型后用wireshark解析，其本身不支持GUI。
### tcpdump基本功能
使用`$tcpdump --help`打印出这么堆东西：
```
tcpdump [ -AbdDefhHIJKlLnNOpqStuUvxX# ] [ -B buffer_size ] 
         [ -c count ] [ -C file_size ] 
         [ -E spi@ipaddr algo:secret,... ] 
         [ -F file ] [ -G rotate_seconds ] [ -i interface ] 
         [ --immediate-mode ] [ -j tstamp_type ] [ -m module ] 
         [ -M secret ] [ --number ] [ --print ] [ -Q in|out|inout ] 
         [ -r file ] [ -s snaplen ] [ -T type ] [ --version ] 
         [ -V file ] [ -w file ] [ -W filecount ] [ -y datalinktype ] 
         [ -z postrotate-command ] [ -Z user ] 
         [ --time-stamp-precision=tstamp_precision ] 
         [ expression ]
```
简单看一下，除了最后的`[expression]`之外都只是一些参数，参数在最下面，表达式比较重要，如果一个数据包根据expression得出`true`结果，那就是应该打印的，否则就不打印。先来看看基本的使用：
`-i`：**指定接口**，比如eth0、lan0等。`$ifconfig`可以查看，注意127.0.0.1是属于lo接口。比如这样指定：
`$tcpdump -i eth0`
`port`：**指定端口**。如果只关心某个端口那就好了，打印的内容必定少了许多。比如这样指定：
`$tcpdump -i eth0 port 80`
`dst`、`src`、`host`这三个关键字分别用于增加限制**"目的端"、"源端"、"地址"**。用起来像这样：
`$tcpdump -i eth0     src host 192.168.1.103`
src同样可以用在port上面，就是表示源端口的意思。**注意**，那些前面带有`-`的才是参数，比如`-i`，而那些没有的，就是个关键字，它们是不能一块写的，比如这样就不对：
`$tcpdump -i eth0 src host 192.168.1.103 port 80`
这`port`和`host`是**起限制作用**的关键字，不能直接这样拼接的，正确用法是用`and`关键字，可以换成下面这样就合法了：
`$tcpdump -i wlan0     src host 192.168.1.103     and     port 80`
如果关心的是某个网段而不是具体到某个ip，可以指定一个范围就像这样：
`$tcpdump -i wlan0    net 192.168`
相当于
`$tcpdump -i wlan0    net 192.168.0.1/16`
现在范围已经比较小了，但是我们往往只关心某个协议，毕竟多个协议放一块，太不直观了。可以这样指定协议：
`$tcpdump -i wlan0    ip src host 192.168.1.103     and     port 80`
其实`and`是个逻辑操作符，同样类型的还有`or`和`not`两个操作符，如果嫌不直观就用符号吧，分别对应`&&`、`||`、`!`。它还允许复杂式子用括号改变优先级，比如这样：
`$tcpdump -i wlan0 ip src host 192.168.1.103 && ! \(port 80 or 23\)`
注意转义两个括号符号，它们是在bash shell里面的。也别写冲突了，比如`src host xxx && src host yyy`就不可能有符合的情况，tcpdump就会提示你 **expression rejects all packets**。表达式写得不好就会显示**syntax error**或者**illegal xxx**什么的。
综上，expression就是由一个至多个子表达式`[proto] [dir] [type]`搭配逻辑符号构成，其中
- `proto` 表示协议，可以是`ether`、`fddi`、`tr`、`wlan`、`ip`、`ip6`、`arp`、`rarp`、`decnet`、`tcp`、`udp`，默认协议是所有。
- `dir` 表示方向，可以是`dst`、`src` 、`dst && src`、`dst || src`(默认)、`ra`、`ta`、`addr1`、`addr2`、`addr3`、`addr4`，后六个仅可用于802.11 Wireless LAN link layers。
- `type` 表示类型，可以是`net`、`port`、`portrange`、`host`(默认)。
示例：
`$tcpdump -n "dst host 192.168.1.1 && (dst port 80 or dst port 443)"`
更多细节请参考[expression手册](http://www.tcpdump.org/manpages/pcap-filter.7.html)
现在开始挖一挖tcpdump提供的选项效果，首先是不带参数的选项：
```
-A    以ASCII编码输出。
-b    BGP协议相关的选项，不解释了。
-d    
-D    列出能监听的网卡interface，比如eth0，wlan0等等，-i选项所带的参数。
-e    显示数据链路层的报头。
-f    
-h
-H
-I
-J
-K
-l
-L
-n    显示地址而不是hostname。
-nn  显示端口号和地址，而不是hostname和服务，比如默认会将端口号80显示成"http"等，加上这个选项后就不会这样了。这个命令很重要：有一次在远程机器上抓包没有加此选项，tcpdump需要30秒以上才输出结果，而且ctrl+c还杀不掉tcpdump，然后加上这个选项就正常了。
-N
-O
-p
-q    精简输出。
-S    显示绝对TCP序号，默认是相对的，即从1开始。
-u
-U
-t    不输出时间戳。下面四个是指定时间戳格式的。
-tt
-ttt
-tttt
-ttttt
-v    让输出详细些。
-vv    让输出更详细些。
-vvv    让输出最详细。
-x    这两个都是十六进制输出，格式比较易看。
-X    比上面多打印出ASCII码。
-xx    这两个也是十六进制输出，但是比上面两个多一个数据链路层报头。
-XX
```
接下来是带参数值的选项：
`[ -B buffer_size ]` 指定缓冲区大小，单位是千字节。
`[ -c count ]` 指定捕获包的数量
`[ -C file_size ]`
`[ -E spi@ipaddr algo:secret,... ]`
`[ -F file ]`
`[ -G rotate_seconds ]`
`[ -i interface ]` 指定网卡，默认是其中一个，用-D看哪些可用，**any**指代所有，**lo**指代本机通讯包。
`[ --immediate-mode ]`
`[ -j tstamp_type ]`
`[ -m module ]`
`[ -M secret ]`
`[ --number ]`
`[ --print ]`
`[ -Q in|out|inout ]`
`[ -r file ]` 从文件中解析出包的信息并输出，file就是-w所保存的那种文件。可以用`-`从标准输入读取。
`[ -s snaplen ]` 指定所捕获包的长度，默认是68B，若为0则是整个包。
`[ -T type ]`
`[ --version ]`
`[ -V file ]` 从file中读取多个文件名，应该同-r的性质差不多。
`[ -w file ]` 将输出到终端改成以pcap格式输出到文件。
`[ -W filecount ]`
`[ -y datalinktype ]`
`[ -z postrotate-command ]`
`[ -Z user ]`
`[ --time-stamp-precision=tstamp_precision ]`
比较常用的格式是先列出要监听的网卡：
`$tcpdump -D`
从中挑选一个(比如eth0)后开始监听ICMP包：
`$sudo tcpdump -i eth0 -x -vvv -n -s 0 icmp`
监听所有包一般要保存到文件中：
`$sudo tcpdump -i eth0 -x -nnv -s0 -w test.pcap`
保存的pcap文件解析后打印出来：
`$sudo tcpdump -x -vvv -r test.pcap`
### 高级筛选
expression还可以详细到指定报文中的某个field的值，不过还是建议都抓下来后用wireshark分析，待续～
### 编辑pcap文件
pcap文件中的包可以进行裁剪，删除，过滤。如果能通过wireshark的过滤器进行筛选出必要的包，那么久可以点击【选择特定分组】进行导出这部分包。
如果想要筛选的包是没有规则可言的，或者不好过滤，那就通过`editcap`工具来编辑它，下面是几个常用命令。
删除指定编号2的报文：
`editcap  in.pcap out.pcap 2`
留下指定编号范围1~5的报文：
`editcap -r  in.pcap out.pcap 1-5`
如果是要合并几个pcap文件，可以适用`mergecap`工具，下面是几个常用命令。
将3个in文件合成out.pcap输出：
`mergecap -w out.pcap in1.pcap in2.pcap in3.pcap`
### 相关文档
- [expression手册](http://www.tcpdump.org/manpages/pcap-filter.7.html)
- [tcpdump手册(中文)](https://www.ibm.com/support/knowledgecenter/zh/ssw_aix_71/com.ibm.aix.cmds5/tcpdump.htm)
- [A tcpdump Tutorial and Primer with Examples](https://danielmiessler.com/study/tcpdump/)

