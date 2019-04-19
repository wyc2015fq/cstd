# Ubuntu网卡IP设置 - 建建的博客 - CSDN博客
2017年03月01日 10:25:46[纪建](https://me.csdn.net/u013898698)阅读数：620
Ubuntu里网卡IP设置的步骤：
1.单网卡配单IP（普遍）
- vi /etc/network/interface  
- auto eth0  
- iface eth0 inet static  
- address IP  
- netmask 子网掩码  
- gateway 网关  
2.单网卡双[IP](http://28653.blog.51cto.com/18653/92119)（情况较少，多用于区分服务或者安全隐藏）
- vi /etc/network/interface  
- auto eth0：0  
- iface eth0：0 inet static  
- address IP  
- netmask 子网掩码  
- gateway 网关  
- auto eth0：1  
- iface eth0：1 inet static  
- address IP  
- netmask 子网掩码  
- gateway 网关 
3.双网卡双IP（应用情况较多，一般为一个[内网](http://tql.blog.51cto.com/39187/5431)一个为外网）
- auto eth0  
- iface eth0 inet static  
- address IP  
- netmask 子网掩码  
- gateway 网关  
- auto eth1  
- iface eth1 inet static  
- address IP  
- netmask 子网掩码  
- gateway 网关 
综上，配置后保存推出，一般需要重启下网卡才能设置生效，方法很多，比如ifdown eth0 ，ifup eth0，最直接的方法直接在终端键入/etc/init.d/networking restart www.kissgz.com
至于[DNS](http://dgcnn.blog.51cto.com/124312/33176/)，都是一样的，如下
- vi /etc/resolv.conf（实际系统默认的并不存在resolv.conf该文件，属于自己创建）  
- nameserver 主DNS  
- nameserver 后备DNS 
