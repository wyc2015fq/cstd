# Net分布式系统之三：Keepalived+LVS+Nginx负载均衡之高可用 - 文章 - 伯乐在线
原文出处： [Andon](http://www.cnblogs.com/Andon_liu/p/5380848.html)
上一篇写了[nginx负载均衡](http://blog.jobbole.com/102133/)，此篇实现高可用（HA）。系统整体设计是采用Nginx做负载均衡，若出现Nginx单机故障，则导致整个系统无法正常运行。针对系统架构设计的高可用要求，我们需要解决Nginx负载均衡出现单机故障时，系统正常运行的需求。所以系统架构引入Keepalived组件，实现系统高可用。
**一、Keepalived介绍**
Keepalived是分布式部署系统解决系统高可用的软件，结合LVS（Linux Virtual Server）使用，其功能类似于heartbeat，解决单机宕机的问题。
**二、Keepalived技术原理**
keepalived是以VRRP协议为实现基础的，VRRP全称Virtual Router Redundancy Protocol，即虚拟路由冗余协议。通过VRRP协议结合LVS，对组群服务器监控情况，若master出现宕机情况，则将VIP漂移到backup机上。实现了分布式系统高可用。可以理解为：keepalived是LVS的管理软件，根据监控情况，将宕机服务器从ipvsadm移除掉。
**三、Keepalived+LVS+Nginx实现系统高可用**
![1](http://jbcdn2.b0.upaiyun.com/2016/06/28c8edde3d61a0411511d3b1866f06365.png)
|**服务器**|**IP地址**|**说明**|
|----|----|----|
|虚拟IP|192.168.1.120:80||
|主机|192.168.1.104:80||
|备机|192.168.1.103:80||
|Web站点A|192.168.1.101:8081|不同端口|
|Web站点B|192.168.1.101:8082|不同端口|
1、安装ipvsadm，CentOS7自带安装包，通过yum进行安装。实现系统支持LVS

Shell
```
yum install ipvsadm
```
2、安装Keepalived软件，并将keepalived设置开机启动

Shell
```
yum install Keepalived
```


Shell
```
systemctl enable keepalived
```
3、进行Keepalived.conf配置，如果是MASTER机，将state BACKUP改为state MASTER。

Shell
```
vim /etc/keepalived/keepalived.conf
#配置的内容
! Configuration File for keepalived
global_defs {
   notification_email {
     xxx@126.com #收到通知的邮件地址
   }
   notification_email_from XX@126.com
   smtp_server 127.0.0.1
   smtp_connect_timeout 30
   router_id LVS_DEVEL
}
vrrp_script monitor_nginx{
   script "/usr/local/etc/keepalived/script/monitor_nginx.sh"
   interval 1
   weight -15
}
vrrp_instance VI_1 {
    state BACKUP 
    interface eno16777736 
    virtual_router_id 51
    priority 80
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        192.168.1.120
    }
    track_script {
        monitor_nginx
    }
}
virtual_server 192.168.1.120 80 {
    delay_loop 6
    lb_algo wrr 
    lb_kind DR
    persistence_timeout 50
    protocol TCP
    real_server 192.168.1.103 80 {
        weight 1
        TCP_CHECK {
            connect_timeout 10
            nb_get_retry 3
            delay_before_retry 3
            connect_port 80
        }
    }
    real_server 192.168.1.104 80 {
        weight 5
        TCP_CHECK {
            connect_timeout 10
            nb_get_retry 3
            delay_before_retry 3
            connect_port 80
        }
    }
}
```
4、配置监控shell脚本

Shell
```
（1）创建：vim /usr/local/etc/keepalived/script/monitor_nginx.sh（2）SHELL文件内容#!/bin/bash  
if [ "$(ps -ef | grep "nginx: master process"| grep -v grep )" == "" ]
then  
    systemclt start nginx.service
    sleep 5    
  if [ "$(ps -ef | grep "nginx: master process"| grep -v grep )" == "" ]  
  then   
    killall keepalived  
  fi  
fi
```
以上完成相关配置，nginx和web服务以上一篇博客内容一致。如下对功能进行验证测试。
**四、实现测试展示**
1、访问系统情况：通过VIP（192.168.1.120）访问系统页面。因为设置了轮询调度，所以刷新页面访问不同站点。
![2](http://jbcdn2.b0.upaiyun.com/2016/06/665f644e43731ff9db3d341da5c827e12.png)![3](http://jbcdn2.b0.upaiyun.com/2016/06/38026ed22fc1a91d92b5d2ef93540f202.png)
2、将 MASTER（192.168.1.104）关机前后，查看相关VLS情况：
（1）关机前：
![4](http://jbcdn2.b0.upaiyun.com/2016/06/011ecee7d295c066ae68d4396215c3d02.png)
（2）关机后：![5](http://jbcdn2.b0.upaiyun.com/2016/06/4e44f1ac85cd60e3caa56bfd4afb675e1.png)
我们看到将104服务器从 LVS移除掉。此时则将后续请求转发到103服务器。
3、关机后，BACKUP服务器 keepalived日志显示无法连接104，并移除掉
![6](http://jbcdn2.b0.upaiyun.com/2016/06/3d2f8900f2e49c02b481c2f717aa90202.png)
5、开机后，将自动检测到服务器正常，并加入LVS中。
![7](http://jbcdn2.b0.upaiyun.com/2016/06/cd7fd1517e323f26c6f1b0b6b96e3b3d1.png)
