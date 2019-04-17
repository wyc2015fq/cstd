# Zabbix监控windows的CPU利用率和其他资源 - DoubleLi - 博客园






zabbix的WEB端--配置-模板--Template OS Windows--项目--创建项目

名称：UserPerfCountercpu

键值：UserPerfCountercpu

数据类型：数字的（浮点）

单位：%

数据更新间隔(秒)：3

应用集：CPU



将此项目添加到图片（或者筛选）



windows下的配置文件zabbix_agentd.conf添加以下文字

PerfCounter=UserPerfCountercpu,"\Processor(_Total)\% Processor Time",3





监控其他的资源在CMD里面输入typeperf -qx>c:\zabbix.txt

**也可以参考http://www.2cto.com/database/201205/132326.html**




**重启**客户端的**zabbix**服务，等待1分钟左右刷新





打开C盘下的zabbix.txt文件，里面即是可监控的资源，自己再新增键值，添加windows下的配置文件zabbix_agentd.conf即可！

参考：[https://www.zabbix.com/documentation/2.2/manual/config/items/perfcounters](https://www.zabbix.com/documentation/2.2/manual/config/items/perfcounters)









