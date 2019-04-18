# Openstack 网络服务 Neutron介绍和控制节点部署 （九） - weixin_33985507的博客 - CSDN博客
2017年02月02日 18:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
## Neutron介绍
neutron是openstack重要组件之一，在以前是时候没有neutron项目。
早期的时候是没有neutron，早期所使用的网络的nova-network，经过版本改变才有个neutron。
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202170044964-882172946.png)
**OpenStack Networking**
- 网络：在实际的物理环境下，我们使用交换机或者集线器把多个计算机连接起来形成了网络。在Neutron的世界里，网络也是将多个不同的云主机连接起来。
- 子网：在实际的物理环境下，在一个网络中。我们可以将网络划分成多为逻辑子网。在Neutron的世界里，子网也是隶属于网络下的。
- 端口：在实际的物理环境下，每个子网或者网络，都有很多的端口，比如交换机端口来供计算机连接。在Neutron的世界端口也是隶属于子网下，云主机的网卡会对应到一个端口上。
- 路由器：在实际的网络环境下，不同网络或者不同逻辑子网之间如果需要进行通信，需要通过路由器进行路由。在Neutron的实际里路由也是这个作用。用来连接不同的网络或者子网。
## Neutron 架构介绍
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202170537651-1734567227.png)
Neutron分为控制节点和计算节点。
openstack默认的网络是单一扁平网络（虚拟机跟宿主机在同一个网段）在官方文档上称为提供者网络。
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202171158620-1101854426.png)
## 网络介绍
配置网络选项，分公共网络和私有网络
部署网络服务使用公共网络和私有网络两种架构中的一种来部署网络服务。
- 公共网络采用尽可能简单的架构进行部署，只支持实例连接到公有网络（外部网络）。没有私有网络（个人网络），路由器以及浮动IP地址。只有admin或者其他特权用户才可以管理公有网络。
- 私有网络在公共网络的基础上多了layer－3服务，支持实例连接到私有网络。demo或者其他没有特权的用户可以管理自己的私有网络，包含连接公网和私网的路由器。另外，浮动IP地址可以让实例使用私有网络连接到外部网络，例如互联网。典型的私有网络一般使用覆盖网络。覆盖网络，例如VXLAN包含了额外的数据头，这些数据头增加了开销，减少了有效内容和用户数据的可用空间。在不了解虚拟网络架构的情况下，实例尝试用以太网 *最大传输单元 (MTU)* 1500字节发送数据包。网络服务会自动给实例提供正确的MTU的值通过DHCP的方式。但是，一些云镜像并没有使用DHCP或者忽视了DHCP MTU选项，要求使用元数据或者脚本来进行配置。
**本章使用公共网络进行配置**
## Neutron控制节点部署
1、软件包安装
yum install -y openstack-neutron openstack-neutron-ml2 \
openstack-neutron-linuxbridge ebtables
2、数据库配置
[root@linux-node1 ~]# vim /etc/neutron/neutron.conf
[database]
connection = mysql+pymysql://neutron:neutron@192.168.137.11/neutron
3、配置keystone
[root@linux-node1 ~]# vim /etc/neutron/neutron.conf
[keystone_authtoken]
auth_uri = http://192.168.137.11:5000
auth_url = http://192.168.137.11:35357
memcached_servers = 192.168.137.11:11211
auth_type = password
project_domain_name = default
user_domain_name = default
project_name = service
username = neutron
password = neutron
[DEFAULT]
auth_strategy = keystone
4、rabbitmq（消息队列）配置
[root@linux-node1 ~]# vim /etc/neutron/neutron.conf
rpc_backend = rabbit
rabbit_host = 192.168.137.11
rabbit_port = 5672
rabbit_userid = openstack
rabbit_password = openstack
5、neutron核心配置，启用ML2插件并禁用其他插件
[root@linux-node1 ~]# vim /etc/neutron/neutron.conf
[DEFAULT]
core_plugin = m12
service_plugins =
备注：service_plugins等号后面什么都不写就是禁用其他插件
6、配置网络服务来通知计算节点的网络拓扑变化
[DEFAULT]
notify_nova_on_port_status_changes = True
notify_nova_on_port_data_changes = True
[nova]
auth_url = http://192.168.137.11:35357
auth_type = password
project_domain_name = default
user_domain_name = default
region_name = RegionOne
project_name = service
username = nova
password = nova
7、配置锁路径
[oslo_concurrency]
lock_path = /var/lib/neutron/tmp
8、配置Modular Layer 2 （ML2）
ML2插件使用Linuxbridge机制来为实例创建layer－2虚拟网络基础设施
编辑配置文件/etc/neutron/plugins/ml2/ml2_conf.ini
- 驱动的选择
[root@linux-node1 ~]# vim /etc/neutron/plugins/ml2/ml2_conf.ini
[ml2]
type_drivers = flat,vlan,gre,vxlan,geneve
- 设置使用什么插件来创建网络
[ml2]
mechanism_drivers = linuxbridge,openvswitch
- 禁用私有网络（租户的网络类型）
[ml2]
tenant_network_types =
- 启动端口安全扩展驱动
[ml2]
extension_drivers = port_security
- 在[ml2_type_flat]部分，配置公共的网络
[ml2_type_flat]
flat_networks = public
- 在 [securitygroup]部分，启用 ipset增加安全组规则的高效性
[securitygroup]
enable_ipset = true
9、配置Linuxbridge代理
Linuxbridge代理为实例建立layer－2虚拟网络并且处理安全组规则。
编辑/etc/neutron/plugins/ml2/linuxbridge_agent.ini文件并且完成以下操作：
- 在[linux_bridge]部分，将公共虚拟网络和公共物理网络接口对应起来
[linux_bridge]
physical_interface_mappings = public:eth0
- 在[vxlan]部分，禁止VXLAN覆盖网络
[vxlan]
enable_vxlan = false
- 在[securitygroup]部分，启用安全组并配置防火墙驱动
[securitygroup]
enable_security_group = True
firewall_driver = neutron.agent.linux.iptables_firewall.IptablesFirewallDriver
10、配置DHCP代理
编辑/etc/neutron/dhcp_agent.ini文件并完成下面的操作：
- 在[DEFAULT]部分，配置Linuxbridge驱动接口，DHCP驱动并启用隔离元数据，这样在公共网络上的实例就可以通过网络来访问元数据
[DEFAULT]
interface_driver = neutron.agent.linux.interface.BridgeInterfaceDriver  # 虚拟接口驱动，使用Linuxbridge
dhcp_driver = neutron.agent.linux.dhcp.Dnsmasq  # dhcp驱动，默认使用Dnsmasq(是一个小的开源项目)来提供dhcp服务
enable_isolated_metadata = true  # 刷新路由使用
11、配置元数据代理
<Metadata agent>负责提供配置信息，例如：访问实例的凭证
编辑/etc/neutron/metadata_agent.ini文件并完成以下操作：
- 在[DEFAULT]部分，配置元数据主机以及共享密码
nova_metadata_ip = 192.168.137.11        # 元数据主机
metadata_proxy_shared_secret = shhnwangjian  # 共享密钥
12、为nova-api配置网络服务
编辑/etc/nova/nova.conf文件并完成以下操作：
- 在[neutron]部分，配置访问参数
[neutron]
url = http://192.168.137.11:9696
auth_url = http://192.168.137.11:35357
auth_type = password
project_domain_name = default
user_domain_name = default
region_name = RegionOne
project_name = service
username = neutron
password = neutron
- 在[neutron]部分，启用元数据代理并设置密码
[neutron]
service_metadata_proxy=true
metadata_proxy_shared_secret = shhnwangjian
13、网络服务初始化脚本需要一个超链接 /etc/neutron/plugin.ini指向ML2插件配置文件/etc/neutron/plugins/ml2/ml2_conf.ini
ln -s /etc/neutron/plugins/ml2/ml2_conf.ini /etc/neutron/plugin.ini
14、同步数据库
su -s /bin/sh -c "neutron-db-manage --config-file /etc/neutron/neutron.conf \
--config-file /etc/neutron/plugins/ml2/ml2_conf.ini upgrade head" neutron
15、重启Nova API 服务
systemctl restart openstack-nova-api.service
16、启动Neutron服务并配置开机自启动
systemctl enable neutron-server.service \
neutron-linuxbridge-agent.service neutron-dhcp-agent.service \
neutron-metadata-agent.service
systemctl start neutron-server.service \
neutron-linuxbridge-agent.service neutron-dhcp-agent.service \
neutron-metadata-agent.service
17、keystone上注册Neutron服务
- 创建neutron服务实体
source /root/admin-openstack.sh
openstack service create --name neutron \
--description "OpenStack Networking" network
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202202745245-1157122743.png)
- 创建neutron网络服务API端点（公有、私有、admin）
openstack endpoint create --region RegionOne \
network public http://192.168.137.11:9696
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202202838604-1898967109.png)
openstack endpoint create --region RegionOne \
network internal http://192.168.137.11:9696
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202202923354-1641474779.png)
openstack endpoint create --region RegionOne \
network admin http://192.168.137.11:9696
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202203007917-631135045.png)
18、检查neutron是否安装成功
neutron agent-list
![](https://images2015.cnblogs.com/blog/830561/201702/830561-20170202203553183-1949188324.png)
