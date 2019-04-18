# linux 下 配置虚拟ip 搭建主备 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月06日 21:40:52[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：309


本文借鉴于：[lvs+keepalived+nginx 搭建主备](https://blog.csdn.net/feng_qi_1984/article/details/54808815#reply)

![](http://on-img.com/chart_image/5ac6f75ce4b0cf24e96ca7e0.png)

防止 nginx 服务器 挂了，需要配置 一个备用nginx。

一、把之前配置好的nginx ，commit 成镜像

docker commit 0dce1173d7c0 nginx1:v1

二、给linux主机加个 虚拟网卡 

ifconfig eth0:1 192.168.46.228 netmask 255.255.255.0 up   

三、运行刚弄好的 nginx镜像 ，创建容器、同时给 使用 service 命令的 权限（-itd代表后台运行、-p代表映射）

docker run --privileged=true -itd -p 192.168.46.228:80:80 556b04ca3b57 /bin/bash

四、进入刚 创建的容器

 docker exec -it 19543a6b3af9 /bin/bash

五、到 init.d 文件夹下  

cd /etc/init.d

vi realserver

输入以下内容

SNS_VIP=172.17.0.50     #配置一个虚拟ip，之后输入这个ip地址可以访问到该nginx
. /etc/rc.d/init.d/functions
case "$1" in
start)
ifconfig lo:0 $SNS_VIP netmask 255.255.255.255 broadcast $SNS_VIP
/sbin/route add -host $SNS_VIP dev lo:0
echo "1" >/proc/sys/net/ipv4/conf/lo/arp_ignore
echo "2" >/proc/sys/net/ipv4/conf/lo/arp_announce
echo "1" >/proc/sys/net/ipv4/conf/all/arp_ignore
echo "2" >/proc/sys/net/ipv4/conf/all/arp_announce
sysctl -p >/dev/null 2>&1
echo "RealServer Start OK"
;;
stop)
ifconfig lo:0 down
route del $SNS_VIP >/dev/null 2>&1
echo "0" >/proc/sys/net/ipv4/conf/lo/arp_ignore
echo "0" >/proc/sys/net/ipv4/conf/lo/arp_announce
echo "0" >/proc/sys/net/ipv4/conf/all/arp_ignore
echo "0" >/proc/sys/net/ipv4/conf/all/arp_announce
echo "RealServer Stoped"
;;
*)
echo "Usage: $0 {start|stop}"
exit 1
esac

exit 0

保存退出

六、更改权限、变为 可 执行

还是在cd /etc/init.d 下

chmod 755 realserver

七 、执行 

 service realserver start

八、安装 keepalived

yum -y install keepalived

九、更改配置文件

cd /etc/keepalived/

vi keepalived.conf 

将以前的内容全部删除  ： ESC     :1,$d

如果是主nginx配置以下内容：

global_defs {                       
#   notification_email {             
#       admin@toxingwang.com
#   }
#   notification_email_from master@toxingwang.com
#   smtp_server smtp.exmail.qq.com
#   smtp_connect_timeout 30
router_id LVS_DEVEL             
}
vrrp_instance VI_1 {            
state MASTER           
interface eth0            
virtual_router_id 51        
priority 100                  
advert_int 1           
authentication {        
auth_type PASS
auth_pass 1111
}
virtual_ipaddress {         
172.17.0.50                #之前配置的虚拟ip
}
}
virtual_server 172.17.0.50 80 {     #之前配置的虚拟ip
delay_loop 6           
lb_algo wrr            
lb_kind DR                          
nat_mask 255.255.255.0   
persistence_timeout 0    
protocol TCP                          
real_server 192.168.46.228 80 {       #映射该docker的linux 的ip
weight 1                             
TCP_CHECK {                     
connect_timeout 10   
nb_get_retry 3
delay_before_retry 3
connect_port 80
}
}
}

备：

global_defs {                       
#   notification_email {             
#       admin@toxingwang.com
#   }
#   notification_email_from master@toxingwang.com
#   smtp_server smtp.exmail.qq.com
#   smtp_connect_timeout 30
router_id LVS_DEVEL             
}
vrrp_instance VI_1 {            
state BACKUP             
interface eth0            
virtual_router_id 51        
priority 100                  
advert_int 1           
authentication {        
auth_type PASS
auth_pass 1111
}
virtual_ipaddress {         
172.17.0.50  
}
}
virtual_server  172.17.0.50   80 {
delay_loop 6           
lb_algo wrr            
lb_kind DR                          
nat_mask 255.255.255.0   
persistence_timeout 0    
protocol TCP                          
real_server 192.168.46.227 80 {     
weight 1                             
TCP_CHECK {                     
connect_timeout 10   
nb_get_retry 3
delay_before_retry 3
connect_port 80
}
}

}

十、启动服务

service keepalived start

配置 第二个 一样，从第二步开始，注意 虚拟ip 要和 第一个一样，就是 172.17.0.50 不要变。

real_server 是  docker run --privileged=true -itd -p 192.168.46.228:80:80 556b04ca3b57 /bin/bash  映射的那个ip

