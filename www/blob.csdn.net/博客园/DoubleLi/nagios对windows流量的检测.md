# nagios对windows流量的检测 - DoubleLi - 博客园






windows下用于和 nagios 整合监控的方式主要有三种：nsclient++ 、nrpe_nt、SNMP。三者各自的特点主要如下：

1、nsclient++比较成熟稳定，文档也丰富，内置很多了指标，能够提供很多snmp无法提供的信息。安装，配置，使用也很快捷方便。
      2、nrpe_nt是linux下nrpe载windows上的实现，灵活性不必说了，pluings在nagiosexchange上也非常丰富。稳定性上稍差于nsclient++。
      3、SNMP协议经历了1、2、3三个版本的发展，其在监控上的应用也比较成熟，nagios也有相应的插件包nagios-plugins-snmp对其支持，不过其在nagios上应用上，不比前两者广，当然，如果只是基本信息的监控，推荐snmp方式。

当然，三者之间也可以混用，我在生产环境上就利用SNMP只对网卡流量进行监控，其他使用nsclient++进行监控。

**一、在windows主机上安装并SNMP服务**

1、安装

开始-控制面板-添加删除程序-添加/删除windows组件-选管理和监视工具-选简单网络管理协议（snmp），如图：



![](http://www.361way.com/wp-content/uploads/2012/10/snmpinstall.png)



不过，在点击添加/删除windows组件时，可能会遇到错误“无法加载安装程序库 wbemupgd.dll，或是找不到函数 0cEntry。请与您的系统管理员联系。特定错误码是0x7e”。该问题的解决方法如下：

a、在windows环境变量的系统变量中增加一项，变量名填“path”，变量值填“%SystemRoot%system32;%SystemRoot%;%SystemRoot%System32Wbem” 。

b、打开注册表，找到HKEY_LOCAL_MACHINESYSTEMCurrentControlSetControlSession ManagerEnvironment，在其下增加如下键值：

名称：path
类型：REG_EXPAND_SZ
值：%SystemRoot%system32;%SystemRoot%;%SystemRoot%System32Wbem





2、配置

运行-services.msc-SNMP service-右键-属性-陷阱-在团体名称中填写如"nagios"-添加到列表，切换至"安全"选项卡，在接受团体名称点击添加-输入团体名称如“nagios”，在“接受来自这些主机的snmp数据包” 中添加远程nagios主机的IP。

![](http://www.361way.com/wp-content/uploads/2012/10/snmpsetup.png)



3、开启防火墙端口

在windows防火墙例外端口中增加udp端口，端口号为161 。

**二、nagios监控端的配置**

1、安装

该检测项用的插件为check_traffic.sh ，可以在[exchang.nagios上的找到该插件的用法](http://exchange.nagios.org/directory/Plugins/Network-Connections%2C-Stats-and-Bandwidth/check_traffic-2Esh/details)，其下载页面托管[在github上](https://github.com/cloved/check_traffic/downloads)。下载好后，放到nagios libexec目录下，不过该脚本依赖Net::SNMP  perl模块。同时需要系统安装好net-snmp的相关包（因为其要用到snmpwalk命令）。

2、检测网卡
[root@web3 libexec]# /App/nagios/libexec/check_traffic.sh -V 2c -C nagios -H 192.168.1.6 -LListInterfacefor host 192.168.1.6.Interface index 1 orresponding to  MS TCP Loopback interface
Interface index 65539 orresponding to  Broadcom BCM5709C NetXtreme II GigE(NDIS VBD Client)#2Interface index 65540 orresponding to  Broadcom BCM5709C NetXtreme II GigE(NDIS VBD Client)#3Interface index 65541 orresponding to  Broadcom BCM5709C NetXtreme II GigE(NDIS VBD Client)#4Interface index 65542 orresponding to  Broadcom BCM5709C NetXtreme II GigE(NDIS VBD Client)
从上面不难看到，我有四块网卡（共五卡，index 1是回环网卡），注意上面的index后面的数字，后面我们要用到。

3、增加配置

打开192.168.1.6主机所对应的配置文件，加入以下监控项：
define service{use                     windows-service,srv-pnp
        host_name               192.168.1.6
        service_description     Network
        check_command           check_traffic_nv!65540!40000,2000!50000,3000!192.168.1.6
        notifications_enabled   1
        normal_check_interval   5
        retry_interval          3}
该配置check_traffic_nv!后面的数字就是上面通过检测所对应的index值，其对应的是网卡3 。以上配置项完成后，reload nagios服务，使新增监控项生效。

注意事项：

1. check_traffic.sh脚本需要net-snmp支持。(snmpwalk在net-snmp-utils软件包中，因此需要安装此软件包)
      2. 不同于http://nagios.sourceforge.net/download/contrib/misc/下的那个perl的check_traffic，请注意区别！！！
      3.如果以非nagios用户身份，手动测试执行过该脚本，请在正式使用该脚本前，删除/var/tmp下对应测试生成的/var/tmp/check_traffic_${Host}_${Interface}.hist_dat文件，否则会造成nagios用户无法读写该文件的错误。













