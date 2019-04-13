
# InfluxDB 的卸载与重装 - leofionn的博客 - CSDN博客


2018年05月01日 08:33:00[leofionn](https://me.csdn.net/qq_36142114)阅读数：789


我是通过下面方式安装的，所以卸载也是用的 rpm 的卸载命令
wget[http://s3.amazonaws.com/influxdb/influxdb-latest-1.x86_64.rpm](http://s3.amazonaws.com/influxdb/influxdb-latest-1.x86_64.rpm)
sudo rpm -ivh influxdb-latest-1.x86_64.rpm
安装参考：[http://www.cnblogs.com/ghj1976/p/4087649.html](http://www.cnblogs.com/ghj1976/p/4087649.html)
卸载命令：
[root@localhost shared]\# rpm -q influxdb
influxdb-0.8.7-1.x86_64
[root@localhost shared]\# rpm -e influxdb
[root@localhost shared]\# rpm -q influxdb
package influxdb is not installed
参数说明：
首先通过  rpm -q <关键字> 可以查询到rpm包的名字
然后 调用 rpm -e <包的名字> 删除特定rpm包
如果遇到依赖，无法删除，使用 rpm -e --nodeps <包的名字> 不检查依赖，直接删除rpm包
如果恰好有多个包叫同样的名字，使用 rpm -e --allmatches --nodeps <包的名字> 删除所有相同名字的包， 并忽略依赖
删除完后，清除已有文件：
[root@localhost opt]\# ls
influxdb
[root@localhost opt]\# rm -rf influxdb
[root@localhost opt]\# ls
【来自@若泽大数据】


