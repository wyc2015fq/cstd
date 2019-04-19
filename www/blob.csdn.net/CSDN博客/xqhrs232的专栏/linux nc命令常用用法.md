# linux nc命令常用用法 - xqhrs232的专栏 - CSDN博客
2016年06月13日 19:25:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：645
原文地址::[http://my.oschina.net/davehe/blog/261845](http://my.oschina.net/davehe/blog/261845)

 NetCat，在网络工具中有“瑞士军刀”美誉，其有Windows和Linux的版本。因为它短小精悍,功能实用，被设计为一个简单、可靠的网络工具，可通过TCP或UDP协议传输读写数据。同时，它还是一个网络应用Debug分析器，因为它可以根据需要创建各种不同类型的网络连接。
先来看下它的基本用法:
```shell; toolbar
```
root@10.1.1.43:~# nc -h
[v1.10-38]
connect to somewhere:	nc [-options] hostname port[s] [ports] ... 
listen for inbound:	nc -l -p port [-options] [hostname] [port]
options:
    -c shell commands	as `-e'; use /bin/sh to exec [dangerous!!]
    -e filename		program to exec after connect [dangerous!!]
    -b			allow broadcasts
    -g gateway		source-routing hop point[s], up to 8                          设置路由器跃程通信网关，最高可设置8个。
    -G num			source-routing pointer: 4, 8, 12, ...                         设置来源路由指向器，其数值为4的倍数。
    -h			this cruft 
    -i secs			delay interval for lines sent, ports sca                      延时的间隔
        -k                      set keepalive option on socket
    -l			listen mode, for inbound connects                             监听模式,入站连接
    -n			numeric-only IP addresses, no DNS                             直接使用ip地址,而不用域名服务器
    -o file			hex dump of traffic                                           指定文件名称，把往来传输的数据以16进制字码倾倒成该文件保存。
    -p port			local port number                                             本地端口
    -r			randomize local and remote ports                              随机本地和远程端口 
    -q secs			quit after EOF on stdin and delay of secs
    -s addr			local source address  
    -T tos			set Type Of Service
    -t			answer TELNET negotiation   
    -u			UDP mode                                                      udp  模式
    -v			verbose [use twice to be more verbose]                        显示过程,vv 更多
    -w secs			timeout for connects and final net reads                      等待连接超时
    -z			zero-I/O mode [used for scanning]                             使用输入/输出模式，只在扫描通信端口时使用。
```
```
1.监听本地端口
```shell; toolbar
```
root@10.1.1.43:~# nc -l -p 1234 
root@10.1.1.43:~# netstat -tunlp | grep 1234
tcp        0      0 0.0.0.0:1234            0.0.0.0:*               LISTEN      15543/nc
```
```
2.端口扫描
```python
```
root@10.1.1.43:~# nc -v -w 10 10.1.1.180 80
(UNKNOWN) [10.1.1.180] 80 (www) open
root@10.1.1.43:~# nc -v -w 10 10.1.1.180 -z 80-30000
(UNKNOWN) [10.1.1.180] 22000 (?) open
(UNKNOWN) [10.1.1.180] 80 (www) open
```
```
3.文件传出
源10.1.1.43 text.txt 
目的 10.1.1.180
```shell; toolbar
```
root@10:~# nc -l -p 1234 > test.txt                 #开10.1.1.180:1234端口监听，并将socket传输过来的数据重定向到test.txt文件 test 43 nc
root@10.1.1.43:~#cat test.txt    
root@10.1.1.43:~# nc  10.1.1.180 1234 < test.txt    #连接远程的10.1.1.180，从test.txt的路径从定向到socket，从而将文件传输到远方。
root@10:~# cat test.txt
test 43 nc
```
```
4.目录传输
源10.1.1.43 python_program
目的 10.1.1.180
```shell; toolbar
```
root@10:~# nc -l -p 1234 | tar xzvf -
root@10.1.1.43:~# tar czvf -  python_program | nc 10.1.1.180 1234
python_program/
python_program/1.py
python_program/4.py
python_program/3.py
```
```
5.测试UDP端口
```python
```
root@172.16.211.34:web# netstat -tunlp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address               Foreign Address             State       PID/Program name   
tcp        0      0 0.0.0.0:22                  0.0.0.0:*                   LISTEN      1111/sshd           
udp        0      0 0.0.0.0:68                  0.0.0.0:*                               887/dhclient 
root@172.16.211.35:~# nc -vuz  172.16.211.34 68
Connection to 172.16.211.34 68 port [udp/bootpc] succeeded!
```
```
参考:
NetCat 官方地址：[http://netcat.sourceforge.net/](http://www.linuxso.com/%22http://netcat.sourceforge.net/%22)
