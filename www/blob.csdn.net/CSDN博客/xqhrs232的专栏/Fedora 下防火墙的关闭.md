# Fedora 下防火墙的关闭 - xqhrs232的专栏 - CSDN博客
2016年04月19日 17:32:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：305
原文地址::[http://blog.csdn.net/ashuai81/article/details/8062168](http://blog.csdn.net/ashuai81/article/details/8062168)

相关文章
1、
## fedora 关闭防火墙的方法
 ----[http://blog.sina.com.cn/s/blog_6cc850810101086l.html](http://blog.sina.com.cn/s/blog_6cc850810101086l.html)

http://blog.csdn.net/sws9999/article/details/2673271

-------------------------------Fedora 下防火墙的关闭-----------------
从配置菜单关闭防火墙是不起作用的，索性在安装的时候就不要装防火墙
查看防火墙状态：
/etc/init.d/iptables status
暂时关闭防火墙：
/etc/init.d/iptables stop
禁止防火墙在系统启动时启动
/sbin/chkconfig --level 2345 iptables off
重启iptables:
/etc/init.d/iptables restart
题外话：
BT或者骡子速度慢用不着关防火墙，只要把相应端口开放就可以了
在文件
/etc/sysconfig/iptables
在系统原始配置的:RH-Firewall-1-INPUT规则链增加类似这样的行：
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 39764 -j ACCEPT
-A RH-Firewall-1-INPUT -m state --state NEW -m udp -p udp --dport 39764 -j ACCEPT
如果发现原有-j REJECT一类的语句，上面两句要放在它的前面
