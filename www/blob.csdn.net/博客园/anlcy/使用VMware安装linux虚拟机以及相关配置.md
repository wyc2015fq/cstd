
# 使用VMware安装linux虚拟机以及相关配置 - anlcy - 博客园






# [使用VMware安装linux虚拟机以及相关配置](https://www.cnblogs.com/camilla/p/8984400.html)
版权声明:
作者：虚无境
博客园出处：[http://www.cnblogs.com/xuwujing](http://www.cnblogs.com/xuwujing)
CSDN出处：[http://blog.csdn.net/qazwsxpcm](http://blog.csdn.net/qazwsxpcm)
个人博客出处：[http://www.panchengming.com](http://www.panchengming.com/)
原创不易，转载请标明出处，谢谢！
## 前言
使用VMware安装虚拟机这个一般都知道，操作简单。而本文主要讲使用虚拟机的后续相关配置。并记录使用过程中遇到的问题以及一些技巧。本篇文章以后回持续更新的。。。
## 安装包准备
VM:12
Linux:CentOS 7.0
百度云盘:
链接：[https://pan.baidu.com/s/1geEAtpx](https://pan.baidu.com/s/1geEAtpx)密码：r503
## 一、CentOS 使用
### 1，安装CentOS
下载VMware Workstation Pro 和CentOS 系统。
成功安装之虚拟机软件后，点击新建虚拟机，安装已经下载好的CentOS。
![这里写图片描述](https://img-blog.csdn.net/20171215191040484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171215191047752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171215191053824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171215191102021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后一直点击下一步就可以了(虚拟机的配置参数可以根据自己的电脑配置来更改)
注:如果出现Intel VT-x 处于禁用状态，那么重启电脑，进入BIOS中，移动到system选项中，将 Intel Virtualization Technology 这个打开就行了，然后F10保存。
再次打开虚拟机，设置好用户名和密码，就可以进入了。
进入之后，可以先设置时区和时间
在CentOS桌面右上角，点击时间，然后进行设置
![这里写图片描述](https://img-blog.csdn.net/20171215192204685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 2，设置静态IP
虚拟机默认是DHCP 自动分配IP地址，如果不想自动分配的话，设置为静态IP就可以了。
步骤:
输入:
```python
vim  /etc/sysconfig/network
```
```python
-scripts/ifcfg
```
```python
-eno
```
```python
*
```
1
注:后面的*根据实际的名称来更改
然后编辑该文件
成功保存退出之后
重启网络，输入
```python
service network restart
```
1
完整配置
```python
TYPE=Ethernet
```
```python
# 设置为静态IP
```
```python
BOOTPROTO=static
```
```python
DEFROUTE=yes
```
```python
IPV4_FAILURE_FATAL=no
```
```python
IPV6INIT=yes
```
```python
IPV6_AUTOCONF=yes
```
```python
IPV6_DEFROUTE=yes
```
```python
IPV6_FAILURE_FATAL=no
```
```python
NAME=eno16777736
```
```python
UUID=47fdc855-346c-4987-b0e2-8f2326864b1c
```
```python
DEVICE=eno16777736
```
```python
ONBOOT=yes
```
```python
PEERDNS=yes
```
```python
PEERROUTES=yes
```
```python
IPV6_PEERDNS=yes
```
```python
IPV6_PEERROUTES=yes
```
```python
#开启自动启用网络连接
```
```python
ONBOOT=yes
```
```python
#设置IP地址
```
```python
IPADDR0=192.168.238.128
```
```python
#设置子网掩码
```
```python
PREFIXO0=24
```
```python
#设置网关
```
```python
GATEWAY0=192.168.238.255
```
```python
#设置主DNS
```
```python
DNS1=8.8.8.8
```
```python
#设置备DNS
```
```python
DNS2=8.8.4.4
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
![这里写图片描述](https://img-blog.csdn.net/20171215192501243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 3，添加用户使用root权限
安装虚拟机之后，登录的是自己设置的用户名，但是没有使用root权限。
本来可以在命令前加上 sudo，但是安装的CentOS7缺没有配置，所以我们需要手动配置。
首先切换到root 用户
输入:
```python
su root
```
1
然后输入
```python
visudo
```
1
找到root ALL=（ALL） ALL位置，
在下面添加一行，将root改成你的用户名，然后退出保存就可以了。
![这里写图片描述](https://img-blog.csdn.net/20171215193043550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 4，更改主机名称
进入linux之后，用户名旁边的就是主机名，默认是使用localhost。平常使用的话倒也不影响，但是如果想做一些学习的话，例如集群的配置，这个主机名就相当不方便，于是便更改这个主机名称。
步骤如下:
CentOS 6
输入
```python
vim /etc/sysconfig/network
```
1
将localhost.localdomain修改为‘你更改的名称‘
![这里写图片描述](https://img-blog.csdn.net/20171215193439375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
CentOS 7
输入:
hostnamectl set-hostname master
![这里写图片描述](https://img-blog.csdn.net/20171215193450577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注:更改主机名称之后输入reboot重启才生效
### 5，IP与主机名映射
平常使用的话，也无须做映射。但是搭建某些环境的时候，做映射使用起来就相当方便。
步骤如下:
修改hosts文件，做关系映射
输入
```python
vim /etc/hosts
```
1
添加
主机的ip 和 主机名称
例如:
```python
192.168.238.128 master
```
1
![这里写图片描述](https://img-blog.csdn.net/20171215193750053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 5，SSH安装
安装新的虚拟机是默认是没有安装SSH的，如果我们想从外部使用xshell等工具连接虚拟机的话，那么SSH就必须安装了
步骤如下:
说明:为了方便，直接使用root账号
首先输入:
```python
rpm
```
```python
-qa
```
```python
|grep
```
```python
-E
```
```python
"openssh"
```
1
查看是否已经安装，如果为安装或缺少依赖
那么久重新安装
输入:
```python
yum
```
```python
install openssh*
```
1
安装，后面就直接输入y就行了
![这里写图片描述](https://img-blog.csdn.net/20171215194006660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
安装成功后，输入ifconfig查看ip，然后在电脑上使用xshell连接，输入ip和端口，然后输入用户名和密码，查看是否连接成功。
![这里写图片描述](https://img-blog.csdn.net/20171215194022119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171215194037293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171215194052408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWF6d3N4cGNt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







