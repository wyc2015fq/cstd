# ubuntu16.04 server 无法联网 - Keith - CSDN博客





2016年12月29日 18:32:25[ke1th](https://me.csdn.net/u012436149)阅读数：6231








# ubuntu16.04 server 无法联网

由于服务器上有四个网卡，不确定网线应该插哪个，所以导致了无法联网。

## 解决方法
- `ifconfig -a` 打印出所有网卡的信息
- `ethtool -p 网口名` 然后看哪个接口亮
- 把网线插到对应的接口
- `sudo dhclient 网口名` 自动获取`ip`地址
- `ping` 一下，看看是否成功 

然后：

```python
sudo vi /etc/network/interfaces
#在里面添加   
auto 网口名 #auto 网口名 开机自动连接网络
iface 网口名 inet dhcp   #manual表示使用固定ip，dhcp表述使用动态ip
```

```
# demo of static ip
auto eth1   
iface eth1 inet static   
address 192.168.0.101   
netmask 255.255.255.0
gateway 192.168.0.2
```

```bash
# set dns
$ sudo vim /etc/resolv.conf
nameserver 202.112.14.21
nameserver 202.112.14.11
```

```bash
# 重启网卡
sudo /etc/init.d/networking restart
```

**执行`ethtool -p eth0`时，eth0对应的网口的灯就会闪烁，你可以分别调用ethtool判断eth1,eth2对应的网口；注意要在不插网线的时候进行，否则看不出来是否因ethtool引起的闪烁；**

**参考文献**
[http://www.cnblogs.com/yudar/p/4446975.html](http://www.cnblogs.com/yudar/p/4446975.html)
[https://www.douban.com/group/topic/4385501/](https://www.douban.com/group/topic/4385501/)
[how to get gateway info in ubuntu](http://blog.csdn.net/duyiwuer2009/article/details/26263855)



