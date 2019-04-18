# 大数据学习笔记(十三) -- 搭建lvs  DR模型 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月29日 09:34:43[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：39


lvs服务器下 ， 在eth0网卡上在配置一个ip   /24 代表 netmask 为 255.255.255.0

所有基于内核的配置修改不能使用vi ，只能使用重定向

```bash
[root@node001 ~]# ifconfig eth0:8 192.168.46.100/24
[root@node001 ~]# echo "1" >/proc/sys/net/ipv4/ip_forward
```

两台 RS服务器下 更改接口访问权限  -- 安装 httpd --- 设置首页

```bash
[root@node003 ~]# echo 1 >/proc/sys/net/ipv4/conf/eth0/arp_ignore
[root@node003 ~]# echo 2 >/proc/sys/net/ipv4/conf/eth0/arp_announce 
[root@node003 ~]# echo 1 >/proc/sys/net/ipv4/conf/all/arp_ignore
[root@node003 ~]# echo 2 >/proc/sys/net/ipv4/conf/all/arp_announce
[root@node003 ~]# ifconfig lo:3 192.168.46.100 netmask 255.255.255.255
```

```bash
[root@node003 ~]# yum install httpd -y
[root@node003 ~]# cd /var/www/html/
[root@node003 html]# vi index.html     # 内容输入的是 from 192.168.46.23
[root@node003 html]# service httpd start
```

测试

![](https://img-blog.csdn.net/20180928221439253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装ipvsadm

```bash
[root@node001 ~]# yum install ipvsadm -y
```

配置ipvsadm的负载模式，rr代表轮询

```bash
[root@node001 ~]# ipvsadm -A -t 192.168.46.100:80 -s rr
```

指定为哪些RS做负载

```bash
[root@node001 ~]# ipvsadm -a -t 192.168.46.100:80 -r 192.168.46.22 -g
[root@node001 ~]# ipvsadm -a -t 192.168.46.100:80 -r 192.168.46.23 -g
```

 测试

![](https://img-blog.csdn.net/20180928222449216?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180928222502729?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



