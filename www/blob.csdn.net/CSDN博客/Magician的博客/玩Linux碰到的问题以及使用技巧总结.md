
# 玩Linux碰到的问题以及使用技巧总结 - Magician的博客 - CSDN博客


2018年08月30日 15:23:19[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：125



### 文章目录
[1、问题](#1_3)
[问题一：](#font_colorFF0000JDKgzipstdinnot_in_gzip_format_font_6)解压JDK报错：gzip:stdin:not in gzip format。
[问题二：](#font_colorFF0000Linuxping_font_45)在Linux下ping不通外网
[问题三：](#font_colorFF0000eth0_font_121)解决虚拟机克隆后网卡eth0不见的问题
[问题四：](#font_colorFF0000syntax_error_unexpected_end_of_filesyntax_error_near_unexpected_token_fibash_bad_interpreter_No_such_file_or_directoryfont_137)执行脚本报错：syntax error: unexpected end of file或syntax error near unexpected token `fi'或-bash bad interpreter: No such file or directory
[问题五：Authentication refused:bad ownership or modes for dircetory /root，ssh免密错误](#Authentication_refusedbad_ownership_or_modes_for_dircetory_rootssh_155)
[问题六：centos下完全卸载mysql](#centosmysql_162)
[2、使用技巧：](#2_167)
[一、minimal版本的linux系统需要安装以下常用的命令：](#minimallinux_170)

**文章总体分为两部分：问题和使用技巧。**
# 1、问题
## 问题一：
## 解压JDK报错：gzip:stdin:not in gzip format。
**情景：**
我先是通过命令:
`wget http://download.oracle.com/otn-pub/java/jdk/8u144-b01/090f390dda5b47b9b721c7dfaa008135/jdk-8u144-linux-x64.tar.gz`从oracle官网下载jdk。
然后执行解压命令：tar -zxvfjdk-8u144-linux-x64.tar.gz，却报错：
`gzip: stdin: not in gzip format 
tar: Child returned status 1 
tar: Error is not recoverable: exiting now`**解决方法：**
执行以下命令（将下载地址改为自己需要的版本）
`wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" http://download.oracle.com/otn-pub/java/jdk/8u111-b14/jdk-8u111-linux-x64.tar.gz`**报错原因：**
通过file命令辨识一下压缩文件的类型：
`file jdk-8u144-linux-x64.tar.gz`结果却是jdk-8u144-linux-x64.tar.gz:HTML document text…
|原来这个压缩文件是html|
这个文件之所以是html，是因为是：文件的下载链接不是直接指向文件，而是先指向一个页面，在这个页面里才是真的下载链接，所以我们通过wget下载的其实是一个页面。通过以上的命令就OK了。
**其他解决方法：**
有些是因为解压缩方式不正确，更改对应的解压缩指令即可。
## 问题二：
## 在Linux下ping不通外网
在linux中ping[www.baidu.com](http://www.baidu.com)无法ping通，可能原因是DNS没配置好
**方法一：修改vi /etc/resolv.conf**
`增加如下内容:
nameserver 114.114.114.114 (电信的DNS)
nameserver 8.8.8.8（googel的DNS）`就可以实现ping通外网了
**方法二：**
ip a 或者 ifconfig 查看使用的网卡然后进入/etc/sysconfig/network-scripts找到对应的网卡进行修改
`vi /etc/sysconfig/network-scripts/ifcfg-ethx``DEVICE=ethx
TYPE=Ethernet
ONBOOT=yes #网卡eth0设置为自启用
BOOTPROTO=static
IPADDR=192.168.1.101
NETMASK=255.255.255.0
DNS1=114.114.114.114
重启网卡 service network restart`|方法三:（配置了前两步依旧无法ping通外网，那么很大一部分原因就是没有添加网关）|
如果添加了DNS还是无法ping外网，那就要查看路由中的网关设置`netstat -rn`
`Kernel IP routing table
Destination           Gateway           Genmask            Flags   MSS Window    irtt Iface
192.168.129.0       0.0.0.0            255.255.255.0       U             0 0                 0 eth0`这就表示网关没有设置，添加路由网关
`route add  default gw 192.168.129.2（我的路由网关是这个）`再次查看`netstat -rn`
`Kernel IP routing table
Destination         Gateway           Genmask              Flags   MSS Window  irtt Iface
192.168.129.0     0.0.0.0            255.255.255.0        U              0 0               0 eth0
 0.0.0.0           192.168.129.2         0.0.0.0                  UG          0 0          0 eth0`添加成功，但是这种方法不是永久的，重启服务器或者重启网卡后悔失效。
静态路由加到/etc/sysconfig/static-routes 文件中就行了，没有这个文件就新建一个
如`route add default gw 192.168.129.2`
则文件中加入
any net default gw 192.168.129.2
保存退出，重启网卡验证
## 问题三：
## 解决虚拟机克隆后网卡eth0不见的问题
**解决方法：**
直接修改`/etc/sysconfig/network-script/ifcfg-eth0`
删掉UUID  HWADDR
配置静态IP地址
然后：
`rm -rf 　/etc/udev/rules.d/70-persistent-net.rules`然后 reboot
**另一种方法：**
编辑/etc/udev/rules.d/70-persistent-net.rules,找到与ifconfig -a得出的MAC相同的一行（NAME='eth1’这一行），把它改为"NAME=eth0 "，然后把上面一行(这一行是对应的老网卡)（NAME=‘eth0’）删除掉。
## 问题四：
## 执行脚本报错：syntax error: unexpected end of file或syntax error near unexpected token `fi’或-bash bad interpreter: No such file or directory
**问题情境：**我们在用NodePad++编辑完脚本文件并上传到服务器后，直接运行脚本会经常报出上面两个错误。
**解决方法：**
`1、vi nginx_check.sh
2、直接输入":"，然后在":"之后输入"set ff"，回车即可看到脚本格式，可以看到当前脚本格式，若为dos则需要接着以下步骤改为unix格式。
3、我们需要把格式改为unix，方法是输入":set ff=unix"，也可以输入":set fileformat=unix"。输入完之后，回车即可完成切换格式。
4、然后我们再输入":set ff"来查看格式，可以看到当前脚本格式变成了我们想要的"unix"了。
5、这时我们再执行nginx_check.sh应该就没问题了`**另一种解决方法：在notepad中直接修改文件格式为unix即可。**
**问题原因：**
上面两个问题都是由于.sh文件的格式为dos格式。而linux只能执行格式为unix格式的脚本。因为在dos/window下按一次回车键实际上输入的是“回车（CR)”和“换行（LF）”，而Linux/unix下按一次回车键只输入“换行（LF）”，所以修改的sh文件在每行都会多了一个CR，所以Linux下运行时就会报错找不到命令。
## 问题五：Authentication refused:bad ownership or modes for dircetory /root，ssh免密错误
服务器配置了免密，但是ssh过去时报错，错误如下：
`Authentication refused:bad ownership or modes for dircetory /root`因为之前有过经验id_ras的权限非600，导致免密失败，检查发现id_ras权限正常600，然后检查发现**root目录的权限配置成了777，改成700后，免密正常登陆**
## 问题六：centos下完全卸载mysql
参考：[https://www.cnblogs.com/wanghuaijun/p/6398240.html](https://www.cnblogs.com/wanghuaijun/p/6398240.html)
# 2、使用技巧：
## 一、minimal版本的linux系统需要安装以下常用的命令：
`yum install net-tools   安装ifconfig命令
yum install lrzsz   这是一个用于上传下载文件的小插件
yum -y install tree   目录以树结构显示
yum -y install wget`

