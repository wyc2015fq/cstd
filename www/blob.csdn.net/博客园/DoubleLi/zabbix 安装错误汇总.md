# zabbix 安装错误汇总 - DoubleLi - 博客园






由于公司业务需要，当前zabbixserver的压力较大，需要安装一个proxy缓解压力，开始慢慢琢磨proxy的安装。这些文档网上很多，就不在多说了。只把自己遇见的错误拿出来共享下

Zabbixproxy是一个监控代理服务器，它收集监控到的数据，先存放在缓冲区，保存的时间可以通过配置文件设定，然后再传送到Zabbixserver；监控代理需要一个单独的数据库。

zabbixproxy的好处：
远程监控
当监控的位置通信不便时
当监控上千的设备时

简化维护分布式监控，降低zabbixserver的负载

error1：zabbix编译安装好 在web配置时，php的db support却没有mysql，郁闷了

![](https://img-blog.csdn.net/20140331175622703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWNlX3Bha2Nob2k=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

solve：在编译安装php是要加上以及--with-mysqli=/usr/local/mysql/bin/mysql_config

一般在web配置时的错误都是php和mysql连接的问题。zabbix是没有问题的



error2：Unable to connect to the server [192.168.1.193]:10051 [cannot connect to [[192.168.1.193]:10051]: [4] Interrupted system call]

solve：这个就比较明显了。proxy连接不上server，查看配置文件才发现server ip被更改了，问同事才知道，他把配置文件里面的server改成了公网的ip，却没有重启服务。介个。。。

还有一个可能是 Hostname和server的web里面配置的名称不一样。这样一定要注意了无论是proxy和server  还是agent和server  Hostname都要相同的

错误提示：sending heartbeat message to server failed: error:"negative response: "failed"", info:"proxy "192.168.8.187_proxy" not found"
 11701:20140331:165755.799 Cannot obtain configuration data from server. info:"proxy "192.168.8.187_proxy" not found"
 11703:20140331:165755.802 sending host availability data to server failed: error:"negative response: "failed"", info:"proxy "192.168.8.187_proxy" not found"



error3：这个错误就比较坑了 看图

 failed to update local proxy configuration copy: database error

![](https://img-blog.csdn.net/20140331180620171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWNlX3Bha2Nob2k=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

solve：查看了众多文档之后才发现，网上关于proxy的安装有个错误，在向zabbix导入数据信息时，只需要schema.sql一个就够了，google告诉我的，baidu不知道。。。



当看到proxy和server之间跑数据，那个激动啊









