# Keepalived 集群软件高级使用(工作原理和状态通知) - 文章 - 伯乐在线
原文出处： [李振良](http://lizhenliang.blog.51cto.com/7876557/1653523)
**1、介绍**
Keeaplived 主要有两种应用场景，一个是通过配置keepalived结合ipvs做到负载均衡（LVS+Keepalived）。另一个是通过自身健康检查、资源接管功能做高可用（双机热备），实现故障转移。
以下内容主要针对Keepalived+MySQL双主实现双机热备为根据，主要讲解keepalived的状态转换通知功能，利用此功能可有效加强对MySQL数据库监控。此文不再讲述Keepalived+MySQL双主部署过程，有需求者可参考以往博文：[http://blog.jobbole.com/94643/](http://blog.jobbole.com/94643/)
### 2、keepalived主要作用
keepalived采用VRRP（virtual router redundancy protocol），虚拟路由冗余协议，以软件的形式实现服务器热备功能。通常情况下是将两台linux服务器组成一个热备组（master-backup），同一时间热备组内只有一台主服务器（master）提供服务，同时master会虚拟出一个共用IP地址（VIP），这个VIP只存在master上并对外提供服务。如果keepalived检测到master宕机或服务故障，备服务器（backup）会自动接管VIP成为master，keepalived并将master从热备组移除，当master恢复后，会自动加入到热备组，默认再抢占成为master，起到故障转移功能。
### 3、工作在三层、四层和七层原理
**Layer3：**工作在三层时，keepalived会定期向热备组中的服务器发送一个ICMP数据包，来判断某台服务器是否故障，如果故障则将这台服务器从热备组移除。
**Layer4：**工作在四层时，keepalived以TCP端口的状态判断服务器是否故障，比如检测mysql 3306端口，如果故障则将这台服务器从热备组移除。


```
示例：
! Configuration File for keepalived
global_defs {
   notification_email {
     example@163.com
   }
   notification_email_from  example@example.com
   smtp_server 127.0.0.1
   smtp_connect_timeout 30
   router_id MYSQL_HA
}
vrrp_instance VI_1 {
    state BACKUP
    interface eth1
    virtual_router_id 50
    nopreempt                   #当主down时，备接管，主恢复，不自动接管
    priority 100
    advert_int 1
    authentication {
        auth_type PASS
        ahth_pass 123
    }
    virtual_ipaddress {
        192.168.1.200          #虚拟IP地址
    }
}
virtual_server 192.168.1.200 3306 {        
    delay_loop 6
#    lb_algo rr 
#    lb_kind NAT
    persistence_timeout 50
    protocol TCP
    real_server 192.168.1.201 3306 {       #监控本机3306端口
        weight 1
        notify_down /etc/keepalived/kill_keepalived.sh   #检测3306端口为down状态就执行此脚本（只有keepalived关闭，VIP才漂移 ） 
        TCP_CHECK {         #健康状态检测方式，可针对业务需求调整（TTP_GET|SSL_GET|TCP_CHECK|SMTP_CHECK|MISC_CHECK）
            connect_timeout 3
            nb_get_retry 3
            delay_before_retry 3
        }
    }
}
```
**Layer7：**工作在七层时，keepalived根据用户设定的策略判断服务器上的程序是否正常运行，如果故障则将这台服务器从热备组移除。


```
示例：
! Configuration File for keepalived
global_defs {
   notification_email {
     example@163.com
   }
   notification_email_from  example@example.com
   smtp_server 127.0.0.1
   smtp_connect_timeout 30
   router_id MYSQL_HA
}
vrrp_script check_nginx {
    script /etc/keepalived/check_nginx.sh    #检测脚本
    interval 2   #执行间隔时间
}
vrrp_instance VI_1 {
    state BACKUP
    interface eth1
    virtual_router_id 50
    nopreempt                   #当主down时，备接管，主恢复，不自动接管
    priority 100
    advert_int 1
    authentication {
        auth_type PASS
        ahth_pass 123
    }
    virtual_ipaddress {
        192.168.1.200          #虚拟IP地址
    }
    track_script {          #在实例中引用脚本
        check_nginx
    }
}
```


```
脚本内容如下：
# cat /etc/keepalived/check_nginx.sh
Count1=`netstat -antp |grep -v grep |grep nginx |wc -l`
if [ $Count1 -eq 0 ]; then
    /usr/local/nginx/sbin/nginx
    sleep 2
    Count2=`netstat -antp |grep -v grep |grep nginx |wc -l`
    if [ $Count2 -eq 0 ]; then
        service keepalived stop
    else
        exit 0
    fi 
else
    exit 0
fi
```
###  4、健康状态检测方式
4.1 HTTP服务状态检测


```
HTTP_GET或SSL_GET {    
      url {
          path /index.html        #检测url，可写多个
          digest  24326582a86bee478bac72d5af25089e    #检测效验码
          #digest效验码获取方法：genhash -s IP -p 80 -u http://IP/index.html 
          status_code 200         #检测返回http状态码
      }
      connect_port 80 #连接端口
      connect_timeout 3  #连接超时时间
      nb_get_retry 3  #重试次数
      delay_before_retry 2 #连接间隔时间
   }
```
4.2 TCP端口状态检测（使用TCP端口服务基本上都可以使用）


```
TCP_CHECK {    
      connect_port 80     #健康检测端口，默认为real_server后跟端口
      connect_timeout 5
      nb_get_retry 3
      delay_before_retry 3
  }
```
4.3 邮件服务器SMTP检测


```
SMTP_CHECK {            #健康检测邮件服务器smtp    
      host {
          connect_ip
          connect_port
      }
      connect_timeout 5
      retry 2
      delay_before_retry 3
      hello_name "mail.domain.com"
  }
```
4.4 用户自定义脚本检测real_server服务状态


```
MISC_CHECK {    
      misc_path /script.sh    #指定外部程序或脚本位置
      misc_timeout 3      #执行脚本超时时间
      !misc_dynamic       #不动态调整服务器权重（weight），如果启用将通过退出状态码动态调整real_server权重值
  }
```
### 5、状态转换通知功能
keepalived主配置邮件通知功能，默认当real_server宕机或者恢复时才会发出邮件。有时我们更想知道keepalived的主服务器故障切换后，VIP是否顺利漂移到备服务器，MySQL服务器是否正常？那写个监控脚本吧，可以，但没必要，因为keepalived具备状态检测功能，所以我们直接使用就行了。


```
主配置默认邮件通知配置模板如下：
global_defs           # Block id
    {
    notification_email    # To:
        {
        admin@example1.com
        ...
         }
    # From: from address that will be in header
    notification_email_from admin@example.com
    smtp_server 127.0.0.1   # IP
    smtp_connect_timeout 30 # integer, seconds
    router_id my_hostname   # string identifying the machine,
                            # (doesn't have to be hostname).
    enable_traps            # enable SNMP traps
        }
```
** 5.1 实例状态通知**
a) notify_master ：节点变为master时执行
b) notify_backup ： 节点变为backup时执行
c) notify_fault  ： 节点变为故障时执行
**5.2 虚拟服务器检测通知**
a) notify_up   ： 虚拟服务器up时执行
b) notify_down  ： 虚拟服务器down时执行


```
示例：
    ! Configuration File for keepalived
    global_defs {
       notification_email {
         example@163.com
       }
       notification_email_from example@example.com 
       smtp_server 127.0.0.1
       smtp_connect_timeout 30
       router_id MYSQL_HA
    }
    vrrp_instance VI_1 {
        state BACKUP
        interface eth1
        virtual_router_id 50
        nopreempt           #当主down时，备接管，主恢复，不自动接管
        priority 100
        advert_int 1
        authentication {
            auth_type PASS
            ahth_pass 123
        }
        virtual_ipaddress {
            192.168.1.200
        }
            notify_master /etc/keepalived/to_master.sh
            notify_backup /etc/keepalived/to_backup.sh
            notify_fault /etc/keepalived/to_fault.sh
    }
    virtual_server 192.168.1.200 3306 {
        delay_loop 6
        persistence_timeout 50
        protocol TCP
        real_server 192.168.1.201 3306 {
            weight 1
            notify_up /etc/keepalived/mysql_up.sh
            notify_down /etc/keepalived/mysql_down.sh    
            TCP_CHECK {
                connect_timeout 3
                nb_get_retry 3
                delay_before_retry 3
            }
        }
    }
```
**状态参数后可以是bash命令，也可以是shell脚本，内容根据自己需求定义，以上示例中所涉及状态脚本如下：**
1) 当服务器改变为主时执行此脚本


```
# cat to_master.sh 
#!/bin/bash
Date=$(date +%F" "%T)
IP=$(ifconfig eth0 |grep "inet addr" |cut -d":" -f2 |awk '{print $1}')
Mail="baojingtongzhi@163.com"
echo "$Date $IP change to master." |mail -s "Master-Backup Change Status" $Mail
```
2) 当服务器改变为备时执行此脚本


```
# cat to_backup.sh
#!/bin/bash
Date=$(date +%F" "%T)
IP=$(ifconfig eth0 |grep "inet addr" |cut -d":" -f2 |awk '{print $1}')
Mail="baojingtongzhi@163.com"
echo "$Date $IP change to backup." |mail -s "Master-Backup Change Status" $Mail
```
3) 当服务器改变为故障时执行此脚本


```
# cat to_fault.sh
#!/bin/bash
Date=$(date +%F" "%T)
IP=$(ifconfig eth0 |grep "inet addr" |cut -d":" -f2 |awk '{print $1}')
Mail="baojingtongzhi@163.com"
echo "$Date $IP change to fault." |mail -s "Master-Backup Change Status" $Mail
```
4) 当检测TCP端口3306为不可用时，执行此脚本，杀死keepalived，实现切换


```
# cat mysql_down.sh
#!/bin/bash
Date=$(date +%F" "%T)
IP=$(ifconfig eth0 |grep "inet addr" |cut -d":" -f2 |awk '{print $1}')
Mail="baojingtongzhi@163.com"
pkill keepalived
echo "$Date $IP The mysql service failure,kill keepalived." |mail -s "Master-Backup MySQL Monitor" $Mail
```
5) 当检测TCP端口3306可用时，执行此脚本


```
# cat mysql_up.sh
#!/bin/bash
Date=$(date +%F" "%T)
IP=$(ifconfig eth0 |grep "inet addr" |cut -d":" -f2 |awk '{print $1}')
Mail="baojingtongzhi@163.com"
echo "$Date $IP The mysql service is recovery." |mail -s "Master-Backup MySQL Monitor" $Mail
```
