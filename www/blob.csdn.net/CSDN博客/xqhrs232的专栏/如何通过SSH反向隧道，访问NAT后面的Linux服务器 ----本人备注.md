# 如何通过SSH反向隧道，访问NAT后面的Linux服务器?----本人备注 - xqhrs232的专栏 - CSDN博客
2017年07月17日 22:40:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：235
原文地址::[http://network.51cto.com/art/201505/477144.htm](http://network.51cto.com/art/201505/477144.htm)
相关文章
1、
# [sshd_config配置详解](http://blog.csdn.net/zhu_xun/article/details/18304441)----[http://blog.csdn.net/zhu_xun/article/details/18304441](http://blog.csdn.net/zhu_xun/article/details/18304441)
# 2、[sshd服务取消DNS反向解析及修改超时时间](http://blog.csdn.net/chinalinuxzend/article/details/2916799)----[http://blog.csdn.net/chinalinuxzend/article/details/2916799](http://blog.csdn.net/chinalinuxzend/article/details/2916799)

假设你在家里运行一台Linux服务器，该服务器位于NAT路由器或限制性防火墙后面。现在，你想在不在家的时候，可以通过SSH连接到家用服务器。你该如何做到这一点?SSH端口转发无疑是一个办法。然而，如果你面对的是多重嵌套NAT环境，端口转发会变得很棘手。
- 作者：布加迪/编译来源：51CTO.com|2015-05-22 08:59
[移动端](http://network.51cto.com/art/201505/477144.htm#%23%23)
 收藏
  分享
## [【技术沙龙】AI开发者实战营-7分钟打造1个定制技能。7月22号，我们等你一起！](http://developer.51cto.com/act/sibichi/201707)
假设你在家里运行一台Linux服务器，该服务器位于NAT路由器或限制性防火墙后面。现在，你想在不在家的时候，可以通过SSH连接到家用服务器。你该如何做到这一点?SSH端口转发无疑是一个办法。然而，如果你面对的是多重嵌套NAT环境，端口转发会变得很棘手。此外，由于每家ISP的情况不一样，比如限制性ISP防火墙阻止转发端口，或者运营商级NAT在诸多用户之间共享IPv4端口，端口转发也会受到干扰。
![如何通过SSH反向隧道，访问NAT后面的Linux服务器?](http://s9.51cto.com/wyfs02/M00/6D/31/wKioL1VehGbgMI0bAAGTlfwePbs680.jpg-wh_651x-s_453241748.jpg)
**何谓SSH反向隧道?**
除了SSH端口转发外，另一个办法就是SSH反向隧道。SSH反向隧道这个概念其实很简单。为此，你需要在限制性家用网络外面有另一个主机，即所谓的“中继主机”(relay host)，你可以从所在地方通过SSH连接到该主机。你可以使用带公共IP地址的虚拟专用服务器(VPS)实例来建立中继主机。然后要做的就是建立一条持久性SSH隧道，从你家用网络的服务器通向公共中继主机。有了这条隧道，你就可以从中继主机“连回”到家用服务器(这就是为什么它叫“反向”隧道)。无论你人在什么地方，或者你家用网络中的NAT或防火墙限制多严格，只要你可以连接到中继主机，就可以连接到家用服务器。
**在Linux上建立SSH反向隧道**
不妨看看我们如何可以建立并使用一条SSH反向隧道。我们假设下列设置。我们将建立一条从家用服务器(homeserver)到中继服务器(relayserver)的SSH反向隧道，那样我们就可以从另一台名为clientcomputer的计算机，通过中继服务器以SSH的方式连接到家用服务器。中继服务器的公共IP地址是1.1.1.1。
在家用服务器上，打开通向中继服务器的SSH连接，如下所示。
homeserver~$ ssh -fN -R 10022:localhost:22 relayserver_user@1.1.1.1
这里的端口10022是你可以选择的任何随意的端口号。只要确保该端口没有被中继服务器上的其他程序所使用就行。
“-R 10022:localhost:22”选项定义了反向隧道。它通过中继服务器的端口1022，将流量转发到家用服务器的端口22。
若使用“-fN”选项，一旦你成功验证了身份、登录到SSH服务器，SSH就会径直进入后台。如果你不想在远程SSH服务器上执行任何命令，只想转发端口，就像在本文的示例中，这个选项很有用。
运行上述命令后，你将直接回到家用服务器的命令提示符。
登录进入到中继服务器，核实127.0.0.1:10022绑定到sshd。如果是这样，那意味着反向隧道已正确建立起来。
relayserver~$ sudo netstat -nap | grep 10022
tcp 0 0 127.0.0.1:10022 0.0.0.0:* LISTEN 8493/sshd
现在可以从其他任何计算机(比如clientcomputer)，登录进入到中继服务器。然后访问家用服务器，如下所示。
relayserver~$ ssh -p 10022 homeserver_user@localhost
需要注意的一个地方就是，你为localhost输入的SSH登录信息/密码应该适用于家用服务器，而不是适用于中继服务器，因为你是通过隧道的本地端点登录进入到家用服务器。所以别为中继服务器输入登录信息/密码。成功登录后，你就接入到了家用服务器。
**通过SSH反向隧道，直接连接到NAT后面的服务器**
虽然上述方法让你可以连接到NAT后面的家用服务器，但是你需要登录两次，先登录到中继服务器，然后登录到家用服务器。这是由于中继服务器上SSH隧道的端点绑定到回送地址(127.0.0.1)。
但实际上，只要单次登录到中继服务器，就可以直接连接到NAT后面的家用服务器。为此，你需要让中继服务器上的sshd不仅可以从回送地址转发端口，还可以从外部主机转发端口。这可以通过在中继服务器上运行的sshd里面指定GatewayPorts选项来实现。
打开中继服务器的/etc/ssh/sshd_conf，添加下面这一行。
relayserver~$ vi /etc/ssh/sshd_conf
GatewayPorts clientspecified
重启sshd。
基于Debian的系统：
relayserver~$ sudo /etc/init.d/ssh restart
基于红帽的系统：
relayserver~$ sudo systemctl restart sshd
现在不妨从家用服务器开始建立SSH反向隧道，如下所示。
homeserver~$ ssh -fN -R 1.1.1.1:10022:localhost:22 relayserver_user@1.1.1.1
登录进入到中继服务器，用netstat命令核实SSH反向隧道已成功建立起来。
relayserver~$ sudo netstat -nap | grep 10022
tcp 0 0 1.1.1.1:10022 0.0.0.0:* LISTEN 1538/sshd: dev
不像之前的情况，隧道的端点现在是1.1.1.1:10022(中继服务器的公共IP地址)，而不是127.0.0.1:10022。这意味着，可以从外部主机连接到隧道端点。
现在可以从其他任何计算机(比如clientcomputer)，输入下列命令，访问NAT后面的家用服务器。
clientcomputer~$ ssh -p 10022 homeserver_user@1.1.1.1
在上述命令中，虽然1.1.1.1是中继服务器的公共IP地址，但homeserver_user必须是与家用服务器关联的用户帐户。这是由于，你实际登录进入的主机是家用服务器，而不是中继服务器。后者只是将你的SSH流量中继转发到家用服务器而已。
**在Linux上建立持久性SSH反向隧道**
想必你已明白了如何建立一条SSH反向隧道，现在不妨让隧道具有“持久性”，那样隧道随时建立并运行起来(无论面对什么样的情况：暂时网络拥塞、SSH超时还是中继主机重启等)。毕竟，要是隧道没有始终建立起来，你也就无法可靠地连接到家用服务器。
为了建立持久性隧道，我要使用一款名为autossh的工具。顾名思义，万一SSH会话由于任何原因而断开，这个程序让你可以自动重启SSH会话。所以，让SSH反向隧道保持持久连接很有用。
第一步，不妨建立无需密码的SSH登录机制，从家用服务器登录到中继服务器。那样一来，autossh就能重启断开的SSH反向隧道，不需要用户干预。
下一步，将autossh安装到发起隧道的家用服务器上。
从家用服务器运行带下列变量的autossh，从而建立一条通向中继服务器的持久性SSH隧道。
homeserver~$ autossh -M 10900 -fN -o "PubkeyAuthentication=yes" -o "StrictHostKeyChecking=false" -o "PasswordAuthentication=no" -o "ServerAliveInterval 60" -o "ServerAliveCountMax 3" -R 1.1.1.1:10022:localhost:22 relayserver_user@1.1.1.1
“-M 10900”选项指定了中继服务器上的一个监控端口，将用来交换测试数据，以监控SSH会话。该端口不应该被中继服务器上的任何程序所使用。
“-fN”选项传递给ssh命令，让SSH隧道可以在后台运行。
“-o XXXX”选项指令ssh执行下列操作：
•使用密钥验证，而不是密码验证。
•自动接受(未知的)SSH主机密钥。
•每60秒就交换持久连接(keep-alive)消息。
•最多发送3个持久连接消息，而不接受任何响应。
与SSH反向隧道有关的其余选项仍与之前一样。
如果你希望SSH隧道一启动就自动建立起来，可以在/etc/rc.local中添加上述的autossh命令。
**结束语**
我在本文中介绍了如何可以使用SSH反向隧道，从外面访问限制性防火墙或NAT网关后面的Linux服务器。我演示的使用场合是通过公共VPS访问家用网络，可是你在用于企业网络时需要小心。可以认为这类隧道违反了企业政策，因为它避开了企业防火墙，因而将企业网络暴露在外部攻击者面前。它被误用或滥用的可能性很大。所以，在建立SSH反向隧道之前总是要牢记因此带来的影响或后果。
英文：How to access a Linux server behind NAT via reverse SSH tunnel
//=============================================================================================================================
备注::
1>按这个文章确实把反向ssh走通了，关键点是把sshd_config的配置修改GatewayPorts clientspecified
2>让22的端口反向映射到对应ssh服务器的ip地址，而不是对应ssh服务器的回环网卡127.0.0.1
3>做好对应的端口转发
4>ssh服务器要能上外网
