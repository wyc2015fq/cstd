# Linux负载均衡软件LVS之四 - 建建的博客 - CSDN博客
2017年03月20日 15:26:24[纪建](https://me.csdn.net/u013898698)阅读数：224
一、启动LVS集群服务
LVS负载均衡管理和使用有两种方式，一种是以ipvsadm命令行脚步与ldirectord监控方式，一种是以Piranha工具进行管理和使用。下面分别介绍。
1、利用ipvsadm命令行脚步与ldirectord监控方式管理LVS
在一切配置完毕之后，依次执行下面的操作启动LVS集群服务。
首先，启动每个real server节点的服务：
[root@localhost ~]# /etc/init.d/lvsrs  start
 start LVS  of  REALServer
然后，在Director Server启动ldirectord服务：
[root@localhost ~]# /etc/init.d/ldirectord  start
Starting ldirectord                                        [  OK  ]
此时查看ldirectord 输出日志信息如下：
ldirectord|2561] Invoking ldirectord invoked as: /usr/sbin/ldirectord start 
ldirectord|2561] Starting Linux Director v1.186 as daemon
ldirectord|2563] Added virtual server: 192.168.60.200:80
ldirectord|2563] Added fallback server: 127.0.0.1:80 (192.168.60.200:80) (Weight set to 1)
ldirectord|2563] Added real server: 192.168.60.132:80 (192.168.60.200:80) (Weight set to 1)
ldirectord|2563] Deleted fallback server: 127.0.0.1:80 (192.168.60.200:80)
ldirectord|2563] Added real server: 192.168.60.144:80 (192.168.60.200:80) (Weight set to 1)
从日志可以看出，ldirectord首先加载了一个虚拟IP的80端口，然后加载了两个real server节点的80端口以及Director Server本机的80端口，同时将real server两个节点的权值设置为1，由于两个节点均可用，最后将Director Server本机的80端口从LVS路由表中删除。
接着，在Director Server启动lvsDR主服务：
[root@xa_web1 ~]# /etc/init.d/lvsDR  start 
 start LVS  of DirectorServer
IP Virtual Server version 1.2.0 (size=4096)
Prot LocalAddress:Port Scheduler Flags
  -> RemoteAddress:Port          Forward  Weight  ActiveConn InActConn
TCP  192.168.60.200:http rr persistent 600
  -> 192.168.60.132:http          Route    1         3          22        
  -> 192.168.60.144:http          Route    1         9          22
到此为止，LVS负载均衡集群系统已经开始运行。
2、利用Piranha工具管理LVS
在这种方式下，启动LVS集群很简单，只需执行如下命令即可：
/etc/init.d/pulse  start
此时查看pulse服务的系统日志信息如下：
[root@localhost ~]# tail -f /var/log/messages
Nov 22 15:52:55 lvs lvs[7031]: starting virtual service [www.gaojf.com](http://www.gaojf.com/) active: 80
Nov 22 15:52:55 lvs lvs[7031]: create_monitor for [www.gaojf.com/RS1](http://www.gaojf.com/RS1) running as pid 7039
Nov 22 15:52:55 lvs nanny[7039]: starting LVS client monitor for 192.168.60.200:80
Nov 22 15:52:55 lvs lvs[7031]: create_monitor for [www.gaojf.com/RS2](http://www.gaojf.com/RS2) running as pid 7040
Nov 22 15:52:55 lvs nanny[7039]: making 192.168.60.132:80 available
Nov 22 15:52:55 lvs nanny[7040]: starting LVS client monitor for 192.168.60.200:80
Nov 22 15:52:55 lvs nanny[7040]: making 192.168.60.144:80 available
从日志可以看出，pulse服务启动了一个[www.gaojf.com](http://www.gaojf.com/)的虚拟服务，同时加载了RS1和RS2两个节点，然后检测到RS1和RS2两个节点对应的80端口可用，最后启动了虚拟IP的80端口，对外提供服务。
日志中“www.gaojf.com”、“RS1”、“RS2”等标识均在Piranha的配置文件/etc/sysconfig/ha/lvs.cf中定义。
Pulse服务启动完毕，通过“ps –ef”命令可以看到有如下进程在运行：
[root@localhost ~]#  ps -ef|grep nanny
/usr/sbin/nanny -c -h 192.168.60.132 -p 80 -s GET / HTTP/1.0\r\n\r\n -x HTTP -a 15 -I /
/usr/sbin/nanny -c -h 192.168.60.144 -p 80 -s GET / HTTP/1.0\r\n\r\n -x HTTP -a 15 -I /
其中，nanny就是Pulse服务的守护进程，用于监控LVS服务节点的运行状态，类似与ldirectord的节点监控功能。
二、 测试负载均衡
这里假定两个real server节点配置www服务的网页文件根目录均为/webdata/www目录，然后分别执行如下操作：
在real server1 执行：
echo "real server1 ok" >/webdata/www/index.html
 在real server2 执行： 
echo "real server2 ok" >/webdata/www/index.html
接着打开浏览器，访问[http://192.168.60.200](http://192.168.60.200/)这个地址，然后不断刷新此页面，如果能分别看到real
 server1 ok;和real server2 ok就表明LVS正常工作了。
三、测试故障切换功能
故障切换是测试当某个节点出现故障后，监控模块是否能及时发现，然后屏蔽故障节点，同时将服务转移到正常节点来执行，下面分别讲述用ldirectord监控时的故障切换过程和用nanny监控时的故障切换情形。
1、利用ldirectord监控LVS的故障切换
这里假定停掉real server1节点的www服务，然后观察ldirectord的日志变化，应该能看到类似如下的信息：
ldirectord|2614] Deleted real server: 192.168.60.132:80 (192.168.60.200:80)
由此可知，ldirectord监控到了192.168.60.132这个节点的服务出现故障，然后将此节点从LVS路由表中删除。
此时继续通过浏览器访问[http://192.168.60.200](http://192.168.60.200/)，访问不受任何影响，但是仅仅显示real server2
 ok信息，这是由于real server1出现问题，被ldirectord屏蔽，整个LVS集群只有real server2在提供服务。
上面的日志输出信息是由于在ldirectord.cf中设置了quiescent=no，如果设置为quiescent=yes;日志输出信息变为如下：
ldirectord|32454] Quiescent real server:192.168.60.132:80 (192.168.60.200:80) (Weight set to 0)
这段日志输出是将故障节点192.168.60.132的权值设置为0，而不从LVS路由表中删除这个主机，此时已经连接的客户端将出现无法访问现象，但是新的连接将不会分配到此节点。
如果重新启动Real server1的服务，ldirectord将能自动检测到节点已经激活，重新将此节点加入LVS路由表中，ldirectord会在日志中输出这些信息：
ldirectord|2614] Added real server: 192.168.60.132:80 (192.168.60.200:80) (Weight set to 1)
2、利用nanny监控LVS的故障切换
 同样，这里也假定停掉real server1节点的www服务，然后观察pluse服务的日志信息，日志输出如下：
Nov 22 15:54:01 lvs nanny[7039]: shutting down 192.168.60.132:80 due to connection failure
可以看出，当停掉real server1的服务后，nanny守护进程监控到此节点故障，然后关闭了此节点的连接服务。
现在重启启动real server1节点的服务，然后观察pluse服务的日志输出：
Nov 22 16:49:41 lvs nanny[7158]: making 192.168.60.132:80 available
nanny守护进程在设定的检测时间内自动检测到real server1服务已经激活，重新使此节点变得可以连接。
