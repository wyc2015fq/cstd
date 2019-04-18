# IP报文经过路由器的转发过程及变化 - aiaiai010101的博客 - CSDN博客

2018年11月29日 14:24:53[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：309


本文转自：[https://blog.csdn.net/ever_peng/article/details/80066996](https://blog.csdn.net/ever_peng/article/details/80066996)

IP报文经过路由器的转发过程及变化

1. 防火墙收到数据包后，解封以太网帧头部，提取目的MAC地址，查看目的MAC地址是不是自己本身的MAC地址。

2. 如果不是自己的MAC地址则丢弃。

3. 如果是自己的MAC地址，上传到上层解析，解析IP层。

4. 假设是自己的MAC地址，解析IP层，提取目的IP地址，判断目的IP地址是不是指向本机，

5. 如果是指向本机，则上传到上层，有上层解析

6. 如果不是指向自己而是转发，则去查路由表，匹配出接口。

7. 假设数据包是转发，则根据路由的最长匹配原则，匹配路由表，找到出接口。

8. 如果匹配的路由是直连路由(与路由器直接连接，中间没有其他设备）则使用目的地址查ARP表。

9. 如果匹配的路由不是直连路由则使用下一跳的IP地址查ARP表。

10. 假设数据包不是直连路由，去ARP表中查下一跳的IP地址对应的MAC地址。

11. 如果查到了下一跳的IP地址对应的MAC地址，则把MAC地址封装到帧的目的MAC中，然后封装物理层发送出去

12. 如果没查到就发ARP请求，查找IP地址对应的MAC地址

13. 假设没有查到下一跳的IP地址对应的MAC地址，发送ARP请求报文获取IP对应的MAC地址。

14. 发送ARP请求后，如果没收到ARP响应则丢弃数据包。

15. 发送ARP请求后，如果收到ARP响应，提取ARP响应中的源MAC地址存放到ARP表中形成映射关系。

16. 假设收到的ARP响应，把下一跳的MAC地址放到目的MAC地址中，封装，发送。

![](https://img-blog.csdn.net/20180423181652858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2V2ZXJfcGVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

PS：数据包经过防火墙转发时，在不做任何策略的情况下，只有目的MAC地址会发生变化。

