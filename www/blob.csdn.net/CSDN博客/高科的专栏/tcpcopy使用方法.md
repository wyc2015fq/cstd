
# tcpcopy使用方法 - 高科的专栏 - CSDN博客

2015年03月18日 16:14:40[高科](https://me.csdn.net/pbymw8iwm)阅读数：6551


1、下载tcpcopy
http://code.google.com/p/tcpcopy/downloads/list
2、配置、编译、安装
依此使用如下命令：
配置：
./configure
编译：
make
**安装：**
make install
**3、使用方法**
下面以mosquitto为例，说明tcpcopy的用法，tcpcopy分为客户端（称为tcpcopy）和服务器端（称为intercept），tcpcopy截获并复制实际运营的程序的数据，并将其发送给待测的服务器程序，如下图所示：
![[原]tcpcopy使用方法 - 逍遥子 - 逍遥子 曰：](http://img1.ph.126.net/16aD78uOaKqlemST1M3eCg==/2970968379281020888.jpg)
上述配置文件为：
1.原始版本mosquitto程序，在实际运行的服务器C上，ip地址为192.168.4.221，端口为1883；
2.优化之后的mosquitto程序，在待测试的服务器T，ip为：192.168.4.223，端口为：1888；
3.在上述C、T两天测试机上编译并安装tcpcopy程序。
4.在实际运营服务器C上启动tcpcopy的客户端tcpcopy，使用命令：
tcpcopy -x 1883-192.168.4.223:1888
这句命令的意思是，让tcpcopy抓取本机的1883端口的数据，并将其发送到192.168.4.223的1888端口。
5.在待测试的服务器T上设置路由表，使用命令：
iptables -I OUTPUT -p tcp --sport 1888 -j QUEUE
6.在待测试服务器T上启动tcpcopy的server程序intercept，使用命令：
intercept
7.测试tcpcopy的客户端和服务器都已经起来了，实际运营的客户端发送给原始mosquitto的数据都被复制发送给了测试服务器T的1888端口。
8.启动服务器T上的优化之后的mosquitto程序，开始测试。

**4、其他使用说明：**
https://github.com/wangbin579/tcpcopy

