# VMware虚拟机中设置端口映射----本人备注！！！ - xqhrs232的专栏 - CSDN博客
2017年07月18日 17:53:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1387
原文地址::[https://www.server110.com/vmware/201309/1703.html](https://www.server110.com/vmware/201309/1703.html)
VMware Workstation提供了两种虚拟机上网方式，一种bridge,一种NAT，bridge可以获得公网地址，而NAT只能是内网地址了。
NAT相当于把主机当成了一个NAT转换器，我们可以添加端口映射，使得外网可以访问利用NAT上网的虚拟机。
下面举两个例子
使用的主机环境为
主机：win7 + vmware workstation 8  公网IP: 118.229.240.94
虚拟机:： Fedora 8  内网IP: 192.168.126.141
例1：在虚拟机内搭建http服务器，使用公网地址访问
1.配置httpd
虚拟机内部安装httpd,没安的话yum install httpd
启动 /etc/init.d/httpd start
关掉防火墙 /etc/init.d/iptables stop
在虚拟机内部用127.0.0.1测试，在主机用192.168.126.141测试，均能正常访问
2.添加端口映射规则
在主机浏览器内输入http://118.229.240.94 ，不可访问，因为主机没安http服务器。
打开VMware Network Editor,找到这个虚拟机使用的网卡，我的是VMnet8
![](http://upload.server110.com/image/20130924/120J16031-0.png)
打开NAT setting
![](http://upload.server110.com/image/20130924/120J11557-1.png)
添加一个端口映射，我们将主机10000端口映射成虚拟机80端口
![](http://upload.server110.com/image/20130924/120J12P4-2.png)
OK，在主机内访问 http://118.229.240.94:10000，可以成功。
但是在一台远程主机上不能成功，为什么呢？因为虚拟机关了防火墙，主机开了防火墙，手动打开主机的10000端口即可，方法参考如下链接：
http://windows.microsoft.com/zh-CN/windows7/Open-a-port-in-Windows-Firewall
例2： ssh端口映射
按上述方法将主机的10001端口映射成虚拟机的22端口（ssh使用22端口），就可以通过putty指定端口号10001访问虚拟机
![](http://upload.server110.com/image/20130924/120J13F9-3.png)
//===================================================================================================================================
//备注::
1>要确保虚拟机中的windows/linux系统能够ping通外网
2>虚拟机的主机上端可以放路由器，路由器可以做一层端口转发
3>虚拟机的NAT虚拟网络编辑器可以做一层端口转发
4>最简单的测试端口转发能不能成功的方法就是通过远程控制机去连虚拟机中的windows/linux系统中的sshd服务
5>远程控制机通过ip地址加端口访问本地虚拟机中的windows/linux系统


