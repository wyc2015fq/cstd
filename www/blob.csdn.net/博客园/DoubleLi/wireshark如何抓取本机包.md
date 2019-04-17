# wireshark如何抓取本机包 - DoubleLi - 博客园






 在进行通信开发的过程中，我们往往会把本机既作为客户端又作为服务器端来调试代码，使得本机自己和自己通信。但是wireshark此时是无法抓取到数据包的，需要通过简单的设置才可以。 

        具体方法如下：

**方法一：**

        1.以管理员身份运行cmd

        2.route add 本机ip mask 255.255.255.255 网关ip

        如：route add 172.16.51.115 mask 255.255.255.255 172.16.1.1

        使用完毕后用route delete 172.16.51.115 mask 255.255.255.255 172.16.1.1删除，否则所有本机报文都经过网卡出去走一圈回来很耗性能。

        此时再利用wireshark进行抓包便可以抓到本机自己同自己的通信包，这样配置的原因是将发往本机的包发送到网关，而此时wireshark可以捕获到网卡驱动的报文实现抓包。

        但这样有一个缺点，那就是本地请求的URL的IP只能写本地的IP地址，不能写localhost或127.0.0.1，写localhost或127.0.0.1还是抓不到包。



**方法二：**

        windows系统没有提供本地回环网络的接口，用wireshark监控网络的话只能看到经过网卡的流量，看不到访问localhost的流量，因为wireshark在windows系统上默认使用的是WinPcap来抓包的，现在可以用Npcap来替换掉WinPcap，Npcap是基于WinPcap 4.1.3开发的，api兼容WinPcap。

1.下载安装包

[Npcap项目主页](https://github.com/nmap/npcap)，它采用的是MIT开源协议，[Npcap下载](https://github.com/nmap/npcap/releases)

2.安装

        安装时要勾选 Use DLT_NULL protocol sa Loopback ... 和 install npcap in winpcap api-compat mode，如下所示。

![](http://dl2.iteye.com/upload/attachment/0117/5929/dfccc991-ba44-3674-b01a-8a72364cf22f.png)
        如果你已经安装了wireshark, 安装前请先卸载WinPcap。

![](http://dl2.iteye.com/upload/attachment/0117/5931/ccf6f9c1-92bf-33b8-855b-3924553dd9dd.png)
        如果还提示WinPcap has been detected之类的，那就将C:\Windows\SysWOW64下的wpcap.dll修改为wpcap.dll.old，packet.dll修改为packet.dll.old，也可参考：[https://nicolask.wordpress.com/2012/09/23/solved-winpcap-4-12-install-error/](https://nicolask.wordpress.com/2012/09/23/solved-winpcap-4-12-install-error/)。

        当然，如果还没有安装wireshark安装，安装wireshark不要安装WinPcap了。

        安装完成启动wireshark, 可以看到在网络接口列表中，多了一项Npcap Loopback adapter，这个就是来抓本地回环包的网络接口了，打开后如下图：

![](http://dl2.iteye.com/upload/attachment/0117/5933/9404c547-9388-3ca3-aac5-de320fa22ae4.png)
        它不仅可以抓URL是localhost的，也可以是127.0.0.1。

![](http://dl2.iteye.com/upload/attachment/0117/5935/d8a9fa26-7e55-369c-9565-7e35084aaabd.png)
        当然，抓本机IP也是完全可以的。

![](http://dl2.iteye.com/upload/attachment/0117/5937/aec8f623-df4f-3d31-9620-0a4664f482d3.png)



文章来源：[http://www.oschina.net/question/133867_108525?fromerr=GpyhsmXr](http://www.oschina.net/question/133867_108525?fromerr=GpyhsmXr)

[http://www.cnblogs.com/wangweihan/archive/2011/01/13/1934459.html](http://www.cnblogs.com/wangweihan/archive/2011/01/13/1934459.html)

[http://blog.techbeta.me/2015/12/wireshark-Loopback/](http://blog.techbeta.me/2015/12/wireshark-Loopback/)









