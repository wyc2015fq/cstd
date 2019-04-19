# iptables常用命令及应用 - xqhrs232的专栏 - CSDN博客
2016年10月27日 18:14:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：306
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.chinaunix.net/uid-26963748-id-3267167.html](http://blog.chinaunix.net/uid-26963748-id-3267167.html)
相关文章
1、关于Linux下的iptables 的浅析命令和了解----[http://www.linuxidc.com/Linux/2013-03/81739.htm](http://www.linuxidc.com/Linux/2013-03/81739.htm)
2、[利用iptables开放被动模式下的FTP服务](http://blog.chinaunix.net/uid-26963748-id-3267168.html) ----[http://blog.chinaunix.net/uid-26963748-id-3267168.html](http://blog.chinaunix.net/uid-26963748-id-3267168.html)
一、命令格式
iptables [-t TABLE] COMMAND CHAIN [creteria] -j(jump) ACTION{ACCEPT,DROP,REJECT,SNAT,DNAT}
 -t {raw|mangle|nat|filter},默认filter
二、命令各选项及解释
 COMMAND:
 规则管理类
  -A，(append的意思)
  -I # (I表示insert的意思，#表示数字，以下相同)
  -D # (delete的意思)
  -R # (replace的意思)
 链管理类：
  -F （flush）（清空链）
  -N （new）（新建链）
  -X （delete）（删除自定义的空链）
  -E （rename）（对链重命名）
 默认策略：
  -P policy
 清空计数器：
  -Z,zero
  每条规则（包括默认策略）都有两个计数器：
   被此规则匹配到的所有数据包的个数；
   被此规则匹配到的所有数据包的大小之和；
 查看类：
  -L（list）
   -n,numeric（以纯数字的方式来显示，不做解析）
   -v,verbose(详细信息)
   -vv -vvv
   -x,exactly（显示精确值，不做单位换算）
   --line-numbers  
 匹配条件（creterial规则）
  基本匹配：
   -s （SOURCE:可以是IP,!NETWORK）
   -d （目标地址）
   -p{tcp|udp|icmp}
   -i INTERFACE（数据包的流入接口）
   -o OUTERFACE（数据包的流出接口）
 扩展匹配：（调用iptables的模块，以便扩展iptables匹配功能）
  隐含扩展
   -p tcp
    --sport PORT
    --dport PORT
     可以指定连续的端口；例如：--sport 22:80(表示从22到80的端口，一般不用)
    --tcp-flags(ACK,SYN,RST,FIN | SYN,ACK只检查前四个标志位，后组出现的为1)= --syn
     例如：--tcp-flags ACK,SYN,RST,FIN SYN,ACK,RST,FIN （不可能，丢弃）
   -p UDP
    --sport PORT
    --dport PORT
   -p icmp
    --icmp-type
     8:echo-request（ping请求）
     0:echo-reply（ping回应）
     3：（表示ping不通）  
  显式扩展
   -m（指定调用的模块）
    state --state {NEW|ESTABLISHD|INVALIDE|RELATED}(指定iptables的状态)，
    iptables各state解释如下：
     NEW（相当于tcp三次握手的第一次请求，是syn=1的包）
     ESTABLISHED
     INVALID
     RELATED(主要用于做ftp命令跟踪的)
     例如：对ssh连接的控制：
     (1)对客户端的请求进行限制：
     iptables -A INPUT -d 172.16.100.1 -p tcp -dport 22 -m state --state NEW,ESTABLISHE -j ACCEPT
     同理，限制web服务只需将端口号改为80即可。
     (2)对服务器的回应进行限制:
     #iptables -A OUTPUT -s 172.16.100.1 -p tcp -sport 22 -m state --state ESTABLISHD -j ACCEKPT
     (3)修改iptables的默认策略：
     iptables -P INPUT DROP
     iptables -P OUTPUT DROP
     (4)让自己可以ping通自己：
     #iptables -I INPUT 1 -s 127.0.0.1 -d 127.0.0.1 -j ACCEPT
     #iptablses -I OUTPUT 1 -s 127.0.0.1 -d 127.0.0.1 -j ACCEPT 
    multiport(指定端口)
     --source-ports 22,53,80
     ！--destination-ports
     --ports
     例如：可以将以下两个相同特征的规则合并为一个规则：
     ACCEPT    tcp  --  0.0.0.0/0   172.16.58.1   tcp dpt:22 state NEW,ESTABLISHED 
     ACCEPT    tcp  --  0.0.0.0/0   172.16.58.1   tcp dpt:80 state NEW,ESTABLISHED 
     #iptables -I INPUT 2 -d 172.16.58.1 -p tcp -m state --state NEW,ESTABLISHED -m
     multiport --destination-port 22,80 -j ACCEPT
     #iptables -D INPUT #(删除合并过的行)
     #iptables -I OUTPUT 1 -m state --state ESTABLISHED -j ACCEPT(这条命令表示不管是什么协议，什么端口，只要状态是ESTABLISHED都允许)
    iprange 
     --src-range 172.16.100.190-172.16.100.210 
     --dst-range
     例如：允许地址为172.16.100.190-172.16.100.210网段的地址访问web服务：
     #iptables -A INPUT -p tcp --dport 80 -m iprange --src-range 172.16.100.190-172.16.100.210 -m state --state NEW -j ACCEPT
    connlimit（表示限定每个主机的tcp连接数）
     --connlimit-above
     例如：同一台主机只允许建立两次ssh连接：
     #iptables -A INPUT -d 172.16.58.1 -p tcp --dport 22 -m state --state NEW -m connlimit ! --connlimit-above 2 -j ACCEPT (表示一台主机最多只允许两次新连接)
    limit (限定速率)
     --limit 1/second（限定多长时间能可以请求几次）
     --limit-burst（限定并发请求的总个数）
     例如：
     #iptables -A INPUT -d 172.16.58.1 -p tcp --deport 80 -m state --state NEW -m limit --limit 1/minite --limit-burst 3 -j ACCEPT
     为了使显示效果明显，可以将keepalive关掉。步骤如下（源码安装）：
     可以修改 /etc/httpd/extra/httpd-default.conf文件中的：KeepAlive On改为KeepAlive Off 
    string
     --algo bm|kmp （指定算法）
     --string "STRING"（功能是拒绝网页中含有指定字符串的网页显示）
     例如：拒绝网页中含有“***功”字样的网页显示：
     #iptables -A OUTPUT -s 172.16.58.1 -m string --algo kmp --string "***功" -j REJECT
     (注意一定要在链OUTPUT中拒绝才生效)  
    time 
     --timestart 
     --timestop -j 
     -days
     例如：限定只能在12点到下午2:30之间请求主机172.16.100.1的web服务：
     #iptables -A INPUT -d 172.16.100.1 -p tcp 80 -m state --state NEW,ESTABLISTHED -m time --timestart 12:00 --timestop 14:00 -j ACCEPT  
