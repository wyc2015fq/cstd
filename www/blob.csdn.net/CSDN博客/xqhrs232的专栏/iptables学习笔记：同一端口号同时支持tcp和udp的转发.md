# iptables学习笔记：同一端口号同时支持tcp和udp的转发 - xqhrs232的专栏 - CSDN博客
2018年07月30日 09:49:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：122
原文地址::[https://blog.csdn.net/subfate/article/details/50510375](https://blog.csdn.net/subfate/article/details/50510375)
相关文章
1、iptables下udp端口转发----[http://www.361way.com/iptables-udp-nat/2947.html](http://www.361way.com/iptables-udp-nat/2947.html)
2、linux 下有能转发 udp 端口数据的软件么----[https://www.v2ex.com/t/213227](https://www.v2ex.com/t/213227)
3、Linux iptables 端口转发----[https://www.linuxidc.com/Linux/2017-07/145378.htm](https://www.linuxidc.com/Linux/2017-07/145378.htm)
前段时间，某项目中遇到一个端口转发问题，虽然我无缘参与项目，但由于项目使用到的一个平台恰好是我前不久搞过的，所以最终还是找我，于是中断了正在进行的任务进行协助。他们定位到只有udp无法转发成功，而tcp是成功的。我看了代码，里面只针对tcp进行转发的。因此，需要对该端口同时支持tcp和udp转发。下面记录一下转发命令。
原先的端口转发的命令如下(172.18.18.18的端口100，转发到100.10.10.168的80端口)：
` `- 
`iptables -t nat -A PREROUTING -d 172.18.18.188 -p tcp --dport 100 -j DNAT --to-destination 100.100.100.168:80 `
- 
`iptables -t nat -A POSTROUTING -d 100.100.100.168 -p tcp --dport 80 -j SNAT --to 172.18.18.188 `
- 
`iptables -A FORWARD -o eth0 -d 100.100.100.168 -p tcp --dport 80 -j ACCEPT `
- 
`iptables -A FORWARD -i eth0 -s 100.100.100.168 -p tcp --sport 80 -j ACCEPT `
上述命令只是针对tcp协议进行转发，下面是修改后的命令，同时支持tcp和udp：
` `- 
`iptables -t nat -A PREROUTING -d 172.18.18.18 -p udp --dport 100 -j DNAT --to-destination 100.10.10.168:80`
- 
`iptables -t nat -A POSTROUTING -s 100.10.10.168 -p udp --dport 80 -j SNAT --to-source 172.18.18.18:100`
- 
`iptables -A FORWARD -o eth0 -d 100.10.10.168 -p udp --dport 80 -j ACCEPT`
- 
`iptables -A FORWARD -i eth0 -s 100.10.10.168 -p udp --sport 80 -j ACCEPT`
- 
`iptables -t nat -A PREROUTING -d 172.18.18.18 -p tcp --dport 100 -j DNAT --to-destination 100.10.10.168:80`
- 
`iptables -t nat -A POSTROUTING -s 100.10.10.168 -p tcp --dport 80 -j SNAT --to-source 172.18.18.18:100`
- 
`iptables -A FORWARD -o eth0 -d 100.10.10.168 -p tcp --dport 80 -j ACCEPT`
- 
`iptables -A FORWARD -i eth0 -s 100.10.10.168 -p tcp --sport 80 -j ACCEPT`
对比发现，只是改了POSTROUTING链，将“-d”改为“-s”，然后将“-to”改为“--to-source”，其它不变，但却能同时进行tcp和udp转发。原理方面还有待继续学习。
如果要删除转发命令，使用下面的命令：
` `- 
`iptables -t nat -D PREROUTING -d 172.18.18.18 -p udp --dport 100 -j DNAT --to-destination 100.10.10.168:80`
- 
`iptables -t nat -D POSTROUTING -s 100.10.10.168 -p udp --dport 80 -j SNAT --to-source 172.18.18.18:100`
- 
`iptables -D FORWARD -o eth0 -d 100.10.10.168 -p udp --dport 80 -j ACCEPT`
- 
`iptables -D FORWARD -i eth0 -s 100.10.10.168 -p udp --sport 80 -j ACCEPT`
- 
`iptables -t nat -D PREROUTING -d 172.18.18.18 -p tcp --dport 100 -j DNAT --to-destination 100.10.10.168:80`
- 
`iptables -t nat -D POSTROUTING -s 100.10.10.168 -p tcp --dport 80 -j SNAT --to-source 172.18.18.18:100`
- 
`iptables -D FORWARD -o eth0 -d 100.10.10.168 -p tcp --dport 80 -j ACCEPT`
- 
`iptables -D FORWARD -i eth0 -s 100.10.10.168 -p tcp --sport 80 -j ACCEPT`
注：
1、参考原来的命令，将tcp换成udp，无法转发，失败。
2、指定端口时，必须要用-p指定传输协议，如果不指定会有语法错误。-p只能同时指定一种协议，不能使用“-p udp,tcp”或“-p all”，这些格式同样有语法错误。
