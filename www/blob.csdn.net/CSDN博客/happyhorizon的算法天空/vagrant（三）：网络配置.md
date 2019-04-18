# vagrant（三）：网络配置 - happyhorizon的算法天空 - CSDN博客
2019年01月01日 12:29:00[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：81
网络配置
所有的网络设置都可以通过配置Vagrantfile来实现。具体来说，就是在Vagrantfile中调用config.vm.network进行相关的设置。
vagrant支持以下三种网络配置：
- Forwarded port(端口映射)
- Private network(私有网络)
- Public network(公有网络)
# 一、端口映射
是指将宿主计算机的端口映射到虚拟机上的某个端口上，访问宿主计算机的该端口时，请求实际会被转发到虚拟机上指定的端口。例如虚拟机上在运行一个web服务器监听80端口，可以通过端口映射映射到宿主机上的8080端口上。这样就可以在宿主机上用localhost:8080访问网站了。配置文件设置语法为：
config.vm.network “forwarded_port”, guest: 80, host: 8080
上面的设置可以允许通过宿主机的8080端口访问虚拟机的80端口。
端口转发的方式简单，很容易实现外网访问虚拟机。需映射很多端口时较麻烦、不支持在宿主机器上使用小于1024的端口来转发(如:不能使用SSL的443端口来进行https连接)。
端口转发的方式也意味着任何一台在宿主机网络上的设备都可以直接访问虚拟机。如果需要限制访问，可以设置guest_ip和host_ip.
## 参数设置
1）auto_correct: boolean类型。默认是false，当设置true的时候，宿主机端口如果被占用，会自动
2）guest: int型。虚拟机向宿主机开放的端口号，可以是任意一个端口。
3）guest_ip：string型。虚拟机开放端口绑定特定的ip，如果这一项没有设置，那么虚拟机的端口会面向所有的ip开放，默认为空。
4）host：int型。宿主机开放给虚拟机通讯的端口，端口号必须大于1024.
5）host_ip：string型。宿主机上端口绑定的ip地址。
6）protocol：string型。网络协议，默认为tcp。
7）id号：string 型。
## 端口转发协议
默认情况下，任何定义的端口都将只转发TCP协议。如果需要支持其他的协议，例如UDP，需要在Vagrantfile中进行设置，如果虚拟机和宿主机的protocol不一致，需要在设置中“显式”地定义两种protocol，例如：
```
Vagrant.configure("2") do |config|
  config.vm.network "forwarded_port", guest: 2003, host: 12003, protocol: "tcp"
  config.vm.network "forwarded_port", guest: 2003, host: 12003, protocol: "udp"
end
```
# 二、私有网络
vagrant的私有网络可以允许用户通过特定ip地址访问虚拟机，但是这个特定的ip是外界网络（global internet）无法访问的，也就是说，宿主机和虚拟机的通讯是在一个完全私有的网络地址空间进行的。
在这个私有空间中的多台虚拟机之间可以互相通讯，但是前提条件是它们的provider必须是一样的，也就是vagrant基于的虚拟机软件是一样的。vagrant支持的虚拟机软件有VirtualBox, Hyper-V, and Docker等等。
## 动态ip
设置私有网络最简单的办法就是利用DHCP。DHCP(Dynamic Host Configuration Protocol)是动态主机配置协议，是一个应用层协议。当我们将客户主机ip地址设置为动态获取方式时，DHCP服务器就会根据DHCP协议给客户端分配IP，使得客户机能够利用这个IP上网。
```
Vagrant.configure("2") do |config|
  config.vm.network "private_network", type: "dhcp"
end
```
基于DHCP，可以实现私有局域网络ip的动态分配。ip地址可以用vagrant ssh访问，同时也可以用ficonfig命令查询。
##静态ip
私有网络也可以设置固定的静态ip，宿主机通过该ip访问虚拟机，例如
Vagrant.configure(“2”) do |config|
config.vm.network “private_network”, ip: “192.168.50.4”
end
私有网络静态ip地址可以任意设置，只要不和网络中任何一台机器的ip冲突就可以。但是ip字段的选择，最好还是要满足私有网络ip的要求。
具体见：[https://en.wikipedia.org/wiki/Private_network#Private_IPv4_address_spaces](https://en.wikipedia.org/wiki/Private_network#Private_IPv4_address_spaces)
这些ip可以保证永远不被公开的路由，而且绝大多数路由器会阻止外部设备对私有ip的访问。
## ipv6
私有网络的ip也可以设置为ipv6的，ipv6不能设置成DHCP，所以如果要用ipv6的方式访问，必须要给定ip地址：
```
Vagrant.configure("2") do |config|
  config.vm.network "private_network", ip: "fde4:8dba:82e1::c4"
end
```
还可以设置子网掩码，例如：
```
Vagrant.configure("2") do |config|
  config.vm.network "private_network",
    ip: "fde4:8dba:82e1::c4",
    netmask: "96"
end
```
## 取消自动配置
如果你想要自己配置网络接口，需要设置auto_config参数为false来关闭Vagrant的自动配置，例如：
Vagrant.configure(“2”) do |config|
config.vm.network “private_network”, ip: “192.168.50.4”,
auto_config: false
end
如果在vagrant环境初始化的时候设置auto_config为true，那么后面修改auto_config设置后，还要手动删去虚拟机上的相关文件（具体位置根据系统不同而不同，通常在/etc/network/interfaces文件夹中），或者更简单地，重建虚拟机。
# 三、公有网络
vagrant的公有网络不如私有网络那么私密，而公有网络的开放特性也很provider直接相关，基本上公有网络可以允许外界直接访问虚拟机，但是私有网络是不允许的。
vagrant的公有网络在未来的版本中可能会更改为bridged，因为公有网络的实现实际上就是采用了桥接bridged这种方式。
在设置公有网络之前一定要注意做好相关的网络安全性准备，review所有的默认box configureation，确认没有任何潜在安全风险。
## DHCP
首先还是可以用HDCP的方式动态分配网址，只需要设置network为public_network就可以实现：
```
Vagrant.configure("2") do |config|
  config.vm.network "public_network"
end
```
也可以设置DHCP的默认路由，例如：
```
Vagrant.configure("2") do |config|
  config.vm.network "public_network",
    use_dhcp_assigned_default_route: true
end
```
## 静态IP
config.vm.network “public_network”, ip: “192.168.0.17”
## 默认网络接口
如果宿主机上有多个网络接口，就需要给vagrant指定虚拟机访问外部网络的特定接口，默认是采用桥接的方式：
config.vm.network “public_network”, bridge: “en1: Wi-Fi (AirPort)”
有些provider可能还需要指定桥接的网络适配器adapter：
```
config.vm.network "public_network", bridge: [
  "en1: Wi-Fi (AirPort)",
  "en6: Broadcom NetXtreme Gigabit Ethernet Controller",
]
```
## 自定义配置
如果需要自定义网络接口，首先应该将auto_config设置为false，然后用shell provisioner实现网口配置，例如:
```
Vagrant.configure("2") do |config|
  config.vm.network "public_network", auto_config: false
  # manual ip
  config.vm.provision "shell",
    run: "always",
    inline: "ifconfig eth1 192.168.0.17 netmask 255.255.255.0 up"
  # manual ipv6
  config.vm.provision "shell",
    run: "always",
    inline: "ifconfig eth1 inet6 add fc00::17/7"
end
```
## 自定义路由
如果需要通过公用网络从其他网络访问vagrant的box，还可以用自定义的路由设置代替默认的路由，例如：
```
Vagrant.configure("2") do |config|
  config.vm.network "public_network", ip: "192.168.0.17"
  # default router
  config.vm.provision "shell",
    run: "always",
    inline: "route add default gw 192.168.0.1"
  # default router ipv6
  config.vm.provision "shell",
    run: "always",
    inline: "route -A inet6 add default gw fc00::1 eth1"
  # delete default gw on eth0
  config.vm.provision "shell",
    run: "always",
    inline: "eval `route -n | awk '{ if ($8 ==\"eth0\" && $2 != \"0.0.0.0\") print \"route del default gw \" $2; }'`"
end
```
参考：
[https://www.vagrantup.com/docs/index.html](https://www.vagrantup.com/docs/index.html)
[https://www.cnblogs.com/hafiz/p/9175484.html](https://www.cnblogs.com/hafiz/p/9175484.html)
