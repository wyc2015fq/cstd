# tcpdump一些参数意义 - Arthur的随笔 - CSDN博客
2012年01月03日 23:44:42[largetalk](https://me.csdn.net/largetalk)阅读数：3371
                
-w file: save dump log to file
-r file: read saved data from file, not capture from network interface
-c count: 如果没有这个参数就一直抓包直到被kill或中断，如有则抓取count个包后停止
-A      : 包内容以ascii输出
-B buff_size: 设置抓包得buffer size
-C file_size: 设置抓包文件（-w设置)得文件大小
-d      : 以可读方式dump出
-dd     : 以C片段方式dump出
-ddd    : 以数字方式dump出
-D      : 列出可被tcpdump抓包得网络设备和代号
-e      : dump中包含link-level（链接层) header, 看起来只是多了一些mac地址信息
-E spi@addr alog:secret : 对某个地址选用特定得包解密算法
-f      : 显示ip而不是别名
-F file : 过滤表达式从文件中读取， 命令行中得被忽略
-G secs : 如果可以，每secs秒rotate一下dump出来得文件(-w设置)
-H      : 试图检测802.11得draft mesh header
-i interface : 指定监听哪个网络设备， 如不指定，监听number最小的
-I      : 使设备进入"monitor"模式， 只对802.11 wifi有用
-K      : 不验证TCP， IP， UDP得校验
-l      : 输出带有行号(不过在我得arch中好像没有什么效果)
-L      : 列出设备所有可知data link type
-m      : load SMI MIB module
-M secret : 以secret给tcp加密?
-n      ：不将address转为name
-N      ：不输出domain name
-O      : Do not run the packet-matching code optimizer
-p      : Don't put the interface into promiscuous mode
-q      : quick (quite) output, 输出内容比较简单
-S      : 输出tcp seq num 得绝对值，而不是相对值
-s snaplen : truncate packet with len snaplen, 而不是65535
-T type : 强制被filter得包以type类型被解析
-t      : dump输出行中不包括时间, tt, ttt, tttt, ttttt都和时间有关
-tt     :
-ttt    :
-u      : Print undecoded NFS handles
-U      : 使输出立即被输出到文件，而不是留在缓冲区
-v      : verbose output
-vv     : more verbose output
-vvv    :
-x      : 输出包里面得内容, 在链路层， 以十六进制形式
-xx     : 包括link layer header
-X      ：同-x， 数据更有ascii格式
-XX     ：同-xx， 更有ascii格式数据
-y type : 设置data link type
-z      : 压缩dump文件
-Z      : 改变运行时用户
experssion : 过滤的表达式
experssion syntac:
    type  : host port net portrange
    dir   : src, dst, src or  dst,  src  and  dst, addr1, addr2, addr3, and addr4
    proto : ether, fddi, tr, wlan, ip,ip6, arp, rarp, decnet, tcp and udp
    there are some special `primitive' keywords  that  don't  follow  the  pattern:  gateway, broadcast, less, greater and arithmetic expressions.
    EX. :
        dst host xxxx
        src host xxxx
        host xxxx
        ether host yyyy
        ether src yyyy
        ether dst yyyy
        gateway zzzz
        dst net aaaa
        src net aaaa
        net aaaa
        net aaaa netmask bbbb = net aaaa/bbbb
        src port xx
        dst port xx
        port xx
        src portrange xx-yy
        dst portrange xx-yy
        portrange xx-yy
        less ll #packet length less than ll
        greater ll
        ip proto xxx  #xxx can be icmp, icmp6, igmp, igrp, pim, ah, esp, vrrp, udp, or tcp
        ip6 proto xxx
        ip6 protochain xxx
        ip protochain xxx
        ether broadcast
        ip broadcast
        ether multicast
        ip6 multicast
        ether proto xxx #xxx can be  ip,  ip6,  arp, rarp,  atalk,  aarp,  decnet, sca, lat, mopdl, moprc, iso, stp, ipx, or netbeui
            
