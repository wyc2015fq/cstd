# Zabbix 命令：zabbix_get - DoubleLi - 博客园






zabbix_get 作用
总有人在群里提问，为什么 zabbix 获取不到数据，为什么 zabbix 提示 Not Support，怎么办？别老问，用 zabbix_get 试着获取数据即可。在 zabbix server 上执行 zabbix_get 命令来试着获取 item 值


zabbix_get 命令详解
命令在 zabbix 安装目录 bin 下，如果是 window 命令自然是 zabbix_get.exe，使用方法都是一样的。端口和源（绑定）IP 是可选的，其他参数不能漏掉
# /usr/local/zabbix-2.2.1/bin/zabbix_get -h
Zabbix get v2.2.1 (revision 40808) (09 December 2013)
usage: zabbix_get [-hV] -s <host name or IP> [-p <port>] [-I <IP address>] -k <key>
Options:
-s --host <host name or IP> Specify host name or IP address of a host
-p --port <port number> Specify port number of agent running on the host. Def
ault is 10050
-I --source-address <IP address> Specify source IP address
-k --key <key of metric> Specify key of item to retrieve value for
-h --help Give this help
-V --version Display version number
Example: zabbix_get -s 127.0.0.1 -p 10050 -k "system.cpu.load[all,avg1]"
参数说明：
-s –host： 指定客户端主机名或者 IP
-p –port：客户端端口，默认 10050
-I –source-address：指定源 IP，写上 zabbix server 的 ip 地址即可，一般留空，服务器如果有多 ip 的时候，
你指定一个。
-k –key：你想获取的 key
至于使用长参数还是短的，自己选，我经常使用-s 而不是-host，太长了。来个例子咯
作者：凉白开 版本：v1.0 网址：http://www.ttlsa.com（运维生存时间）
254 / 266
zabbix_get 获取数据
获取负载
./zabbix_get -s 127.0.0.1 -p 10050 -k "system.cpu.load[all,avg15]"
0.270000
获取主机名
# ./zabbix_get -s 127.0.0.1 -p 10050 -I 127.0.0.1 -k "system.hostname"
10-9-4-20









