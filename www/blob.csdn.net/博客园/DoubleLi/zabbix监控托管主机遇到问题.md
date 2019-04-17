# zabbix监控托管主机遇到问题 - DoubleLi - 博客园






昨天监控公司的托管主机时发现监控不上，回想起来其实就是个小问题，分分钟能解决的事，排错的过程才是真正耗心费神的。

### **监控环境：**



|```bashA   zabbix server: 192.168.17.110    server端设在公司内部``````bashB   zabbix agent: 10.200.66.90      （被动模式）这台服务器为托管机 A（192.168.17.110）通过vpn与托管机B（10.200.66.90）通信，由于防火墙策略，A能``````bashping``````bash通B，B不能``````bashping``````bash通A，即发起tcp连接的机器必须为A（A--->B），从B方发起的连接(B--->A)是被拒绝的。```|
|----|




### **zabbix agent端配置文件：**



|```bashegrep``````bash-``````bashv``````bash"^#|^$"``````bash/usr/local/zabbix/conf/zabbix_agentd``````bash.conf``````bashLogFile=``````bash/tmp/zabbix_agentd``````bash.log``````bashDebugLevel=3``````bashServer=192.168.17.120``````bashListenPort=10050``````bash#ServerActive=192.168.17.120``````bashHostname=10.200.66.92```|
|----|




### 在zabbix server端运行zabbix_get获取数据报错：

Get value from agent failed: ZBX_TCP_READ() failed: [104] Connection reset by peer

zabbix_get [30318]: Check access restrictions in Zabbix agent configuration

### **是由于AB只能单向发起连接请求导致的吗？**

在过去的几小时笔者也在纠结是否是这个原因，是不是agent端因为不能主动发起连接所以server获取不到数据呢？

### ssh端口转发能解决吗？

既然A到B能通，B到A不通，那可以将B端的10050端口转发到A主机吗？

ssh端口转发的三种模式：本地端口转发，远程端口转发，动态端口转发

A到B能通，B到A不通，照理说应该在A机器上进行ssh远程端口转发，打通B端10050口到A端的通道，但被动模式下A并不是用确定的端口与B的10050通信，所以ssh端口转发并不能解决笔者的问题

### 问题所在

前面一系列猜想都是因为未对zabbix被动模式有深入理解而导致的，被动模式并不要求AB能互相发起连接，只要A（server）能向B（agentd）发起连接足矣。A通过B的10050端口向B发起TCP连接，三次握手完成后即可进行数据传输。因此问题不应该在此。应该还是配置出问题了！！

### 查看agent端日志



|```bashtail``````bash/tmp/zasbbix_agentd``````bash.log``````bash<br>......<br>......``````bash104276:20170829:095704.310 failed to accept an incoming connection: connection from``````bash"10.200.58.10"``````bashrejected, allowed hosts:``````bash"192.168.17.110"```|
|----|




日志说明了一切，都是vpn惹的祸，server（192.168.17.110）通过vpn后进行了地址转换，最终与agent通信的已经不是在内网中的真实IP了，而是变成了10.200.58.10。

解决该问题只需更改server名或server IP即可：将agent配置文件中Server=192.168.17.120改为Server=10.200.58.10









