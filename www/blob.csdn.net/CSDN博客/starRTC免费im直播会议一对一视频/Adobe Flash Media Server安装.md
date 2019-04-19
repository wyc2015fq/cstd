# Adobe Flash Media Server安装 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年11月18日 14:18:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1592
Flash Media Server（FMS)是一个流媒体服务器
使用 实时消息传送协议（RTMP），RTMP是一种未加密的TCP/IP协议，专门设计用来高速传送音频、视频和数据信息。
3.5版32位下载(内含windows和linux版)：链接：[http://pan.baidu.com/s/1miTce1M](http://pan.baidu.com/s/1miTce1M)密码：2rzb
4.5版64位linux版：链接：[http://pan.baidu.com/s/1eS2PUts](http://pan.baidu.com/s/1eS2PUts)密码：850t
序列号生成器（可能会被杀毒软件拦截，建议在虚拟机VMware Workstation里运行）：[https://yunpan.cn/cqMLuTV2sZIWA](https://yunpan.cn/cqMLuTV2sZIWA)访问密码 5282
测试序列号：1462-5345-0626-5376-6187-3552
## CentOS 6.3安装4.5版注意事项：
fms4.5支持的CentOS版本为5.5 64 bit
在6.3下安装
**yum install**  libcap compat-libcap1 libcap-devel libcap.i686   -y附其它可能需要安装的工具：**yum install   ****autoconf  ****automake  ****gcc** gcc-c++ **git** libtool **make  ****nasm** pkgconfig zlib-devel  -yldconfig
## 安装fms
FlashMediaServer4.5_x64.tar.gz
解压后，进入目录
tar -zxvf FlashMediaServer4.5_x64.tar.gz 
**cd** FMS_4_5_6_r5012**/****ln **-s  **/**lib64**/**libcap.so.2 .**/**libcap.so.1
**vim** installFMS
 
找到
cd_check "Red Hat Enterprise Linux Client release 5"**/**etc**/**redhat-release redhat-RHEL5-i686
后面加上
cd_check "Red Hat Enterprise Linux Client release 6" **/**etc**/**redhat-release redhat-RHEL6-i686
cd_check "Red Hat Enterprise Linux Server release 6" /etc/redhat-release redhat-RHEL6-i686 
找到
cd_check "CentOS release 5"**/**etc**/**redhat-release centos-5-i686  
后面加上  
cd_check "CentOS release 6" **/**etc**/**redhat-release centos-6-i686
 
 
**if****[**-f**/**lib**/**libcap.so.2 **]**; **then    ****//这一行中**将**/**lib改为**/**lib64
安装
.**/**installFMS
### 输入序列号
1462-5345-0626-5376-6187-3552（3.5的正版序列号1373-5711-0151-4137-9283-0503）
序列号生成器（可能会被杀毒软件拦截，建议在虚拟机[VMware Workstation](http://172.30.24.252/elesos_com/index.php?title=VMware_Workstation&action=edit&redlink=1)里运行）：[艺搜下载](http://dl.vmall.com/c05szulpl4)
输入用户名：elesos
选择默认用户
我们不安装apache，而用Nginx代替。参见[CentOS下安装Nginx+MySQL+PHP+phpMyAdmin](http://172.30.24.252/elesos_com/index.php?title=CentOS%E4%B8%8B%E5%AE%89%E8%A3%85Nginx%2BMySQL%2BPHP%2BphpMyAdmin)
选择安装后不启动！
### 安装信息确认
## 开放端口
**/**sbin**/**iptables -I INPUT -p tcp --dport80-j ACCEPT
**/**sbin**/**iptables -I INPUT -p tcp --dport1935-j ACCEPT
**/**sbin**/**iptables -I INPUT -p tcp --dport1111-j ACCEPT
**/**etc**/**init.d**/**iptables save
service iptables restart
## 配置Nginx
**vi ****/**opt**/**nginx**/**conf**/**nginx.conf
将网站目录从html修改为fms的webroot目录：/opt/adobe/fms/webroot
重新加载
**cd ****/**opt**/**nginx**/**sbin
.**/**nginx -s reload
打开[http://192.168.220.137/fms_adminConsole.htm即可登录](http://192.168.220.137/fms_adminConsole.htm%E5%8D%B3%E5%8F%AF%E7%99%BB%E5%BD%95)
## 其它
忘记密码后如何修改密码
**cd****/**opt**/**adobe**/**fms
.**/**fmsadmin -console-user**<**username**>**
## 艺搜参考
[http://help.adobe.com/en_US/adobemediaserver/install/index.html](http://help.adobe.com/en_US/adobemediaserver/install/index.html)
[https://en.wikipedia.org/wiki/Adobe_Flash_Media_Server](https://en.wikipedia.org/wiki/Adobe_Flash_Media_Server)
