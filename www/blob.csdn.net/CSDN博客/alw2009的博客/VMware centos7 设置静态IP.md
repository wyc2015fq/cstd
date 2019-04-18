# VMware centos7 设置静态IP - alw2009的博客 - CSDN博客





2019年01月01日 01:25:04[traveler_zero](https://me.csdn.net/alw2009)阅读数：13








一、修改虚拟网卡的连接模式

![](https://img-blog.csdnimg.cn/20190101002604991.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)

 编辑->虚拟网络编辑器

![](https://img-blog.csdnimg.cn/20190101012116848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)



二、修改IP配置文件

用root用户登录

![](https://img-blog.csdnimg.cn/20190101002746396.png)

 vi /etc/sysconfig/network-scripts/ifcfg-ens33

默认的配置是

![](https://img-blog.csdnimg.cn/20190101002936755.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)

修改为

![](https://img-blog.csdnimg.cn/20190101003411349.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)

 三、重启网络

```
systemctl restart network
ip addr
```

四、配置DSN

在/etc/resolv.conf里加入阿里的DNS

```
nameserver 223.5.5.5
nameserver 223.5.5.6
```

五、检查网络

到这里已经配置完成， 可以ping 一下百度。’ 





