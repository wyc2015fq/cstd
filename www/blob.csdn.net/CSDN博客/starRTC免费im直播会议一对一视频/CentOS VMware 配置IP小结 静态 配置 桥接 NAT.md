# CentOS VMware 配置IP小结 静态 配置 桥接 NAT - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 14:42:41[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：227
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

系统启动后可先ping下外网或局域网内其它机器。
如果配置虚拟机时选择的NAT上网方式，后面需要配置固定IP，请先参见[VMware
 NAT方式下设置静态IP](http://192.168.222.250/wiki_elesos_com/index.php?title=VMware_NAT%E6%96%B9%E5%BC%8F%E4%B8%8B%E8%AE%BE%E7%BD%AE%E9%9D%99%E6%80%81IP&action=edit&redlink=1)获得可用的IP范围和网关等信息。
先将ifcfg-eth0备份到home目录下，不要放在与它同一目录下，然后
**vim ****/**etc**/**sysconfig**/**network-scripts**/**ifcfg-eth0
 
IPADDR=192.168.88.201 *#指定ip地址*NETMASK=255.255.255.0 *#子网掩码*GATEWAY=192.168.88.2*#默认网关*DNS1=192.168.88.2 *#DNS1服务器的地址，一般跟网关一样*
ONBOOT="yes"
然后备份/etc/sysconfig/network文件并vim修改网关。参见： 查看与修改网关，DNS
重启网络服务
service network restart
## 相关命令ifconfig
**ifconfig**
显示当前激活的网络接口信息。
**ifconfig**-a
显示所有网络接口的信息，无论是否激活。
网卡接口关闭与激活
**[**root**@**localhost ~**]***# ifdown eth0   #关闭网络***[**root**@**localhost ~**]***# ifup eth0     #启动网络*
Vmware桥接方式下设置静态IP，可以参考下宿主计算机的IP设置。,然后ping一下，看下设置的ip是否被其它人占用。
