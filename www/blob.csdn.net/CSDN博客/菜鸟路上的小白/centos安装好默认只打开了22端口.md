# centos安装好默认只打开了22端口 - 菜鸟路上的小白 - CSDN博客





2016年09月08日 15:32:59[somnus_小凯](https://me.csdn.net/u012486840)阅读数：2779







CentOS安装好，默认只打开了22端口，如果希望开放其他端口的话，需要打开所需端口。 
比如打开http的默认端口80 
编辑iptables 

vi /etc/sysconfig/iptables  

添加 

-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 3001 -j ACCEPT  

重新启动服务 

/sbin/service iptables restart  
查看端口是否开放 


/sbin/iptables -L -n



