
# win7 防火墙开启ping - 阳光岛主 - CSDN博客

2013年10月01日 00:00:29[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：14605


Windows7出于安全考虑，默认情况下是不允许外部主机对其进行Ping测试的。但在一个安全的局域网环境中，Ping测试又是管理员进行网络测试所必须的，如何允许 Windows 7的ping测试回显呢?
**(1)、查看、开启或禁用系统防火墙**
打开命令提示符输入输入命令“netsh firewall show state”，然后回车可查看防火墙的状态
![](https://img-blog.csdn.net/20130930122453296)
从显示结果中，可看到防火墙各功能模块的禁用及启用情况。
命令“netsh firewall set opmode disable”用来禁用系统防火墙
命令“netsh firewall set opmode enable”可启用防火墙。

**(2)、允许文件和打印共享**
文件和打印共享在局域网中常用的，如果要允许客户端访问本机的共享文件或者打印机，可分别输入并执行如下命令：
netsh firewall add portopening UDP 137 Netbios-ns (允许客户端访问服务器UDP协议的137端口)
netsh firewall add portopening UDP 138 Netbios-dgm (允许访问UDP协议的138端口)
netsh firewall add portopening TCP 139 Netbios-ssn (允许访问TCP协议的139端口)
netsh firewall add portopening TCP 445 Netbios-ds (允许访问TCP协议的445端口)
命令执行完毕后，文件及打印共享所须的端口都被防火墙放行了。

**(3)、允许ICMP回显**
默认情况下，Windows7出于安全考虑是不允许外部主机对其进行Ping测试的。但在一个安全的局域网环境中，Ping测试又是管理员进行网络测试所必须的，如何允许 Windows 7的ping测试回显呢?
当然，通过系统防火墙控制台可在“入站规则”中将“文件和打印共享(回显请求– ICMPv4-In)”规则设置为允许即可，如果网络使用了 IPv6，则同时要允许 ICMPv6-In 的规则。
具体步骤：
控制面板 ——》Windows 防火墙 ——》 高级设置 ——》 入站规则(Inbound Rules) ——》 文件和打印共享（File and Printer Sharing），如下图：
![](https://img-blog.csdn.net/20130930123811921)

在上图，选中“File and Printer Sharing(Echo Request - ICMPv4-In”，右键属性（双击也行），勾选“Eanbled”，如下图：
![](https://img-blog.csdn.net/20130930124815015)

主机开启防护墙的情况下，在虚拟机Ubuntu的终端中，ping主机（Host），ping通有回显了：
![](https://img-blog.csdn.net/20130930125349640)

我们在命令行下通过netsh命令可快速实现。执行命令“netsh firewall set icmpsetting 8”可开启ICMP回显，反之执行“netsh firewall set icmpsetting 8 disable”可关闭回显。


