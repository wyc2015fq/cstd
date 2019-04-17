# zabbix监控进程的CPU和内存占用量，进程数量 - DoubleLi - 博客园








由于需要对公司特定服务进行监控，于是，通过编写脚本获取各个进程占用系统资源的信息，从而使用zabbix采集到这些数据进行特定进程的基础监控。

我这主要需要监控的程序如下：

**nginx redis mysql tomcat sentinel mongodb openfire kafka zookeeper twemproxy mycat memcached php httpd**

首先，在agent端编写监控脚本，脚本内容如下：



```
```bash
[root@monitor sbin]$
```

```bash
cat
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh
```



```bash
#!/bin/bash
```



```bash
#date:2015.06.15
```



```bash
nginxmem(){
```



```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"nginx"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
nginxcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"nginx"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
nginxnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"nginx"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```







```bash
redismemmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"redismem"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
redismemcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"redismem"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
redismemnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"redismem"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```







```bash
mysqlmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mysql"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
mysqlcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mysql"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
mysqlnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mysql"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```











```bash
tomcatmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"tomcat"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
tomcatcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"tomcat"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
tomcatnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"tomcat"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```









```bash
sentinelmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"sentinel"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
sentinelcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"sentinel"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
sentinenum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"sentinel"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```











```bash
mongodbmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mongod"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
mongodbcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mongod"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
mongodbnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mongod"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```











```bash
openfiremem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"openfire"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
openfirecpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"openfire"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
openfirenum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"openfire"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```









```bash
kafkamem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"kafka"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
kafkacpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"kafka"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
kafkanum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"kafka"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```









```bash
zookeepermem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"zookeeper"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
zookeepercpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"zookeeper"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
zookeepernum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"zookeeper"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```









```bash
twemproxymem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"twemproxy"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
twemproxycpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"twemproxy"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
twemproxynum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"twemproxy"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```









```bash
mycatmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mycat"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
mycatcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mycat"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
mycatnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"mycat"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```







```bash
httpdmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"httpd"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
httpdcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"httpd"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
httpdnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"httpd"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```







```bash
memcachedmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"memcached"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
memcachedcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"memcached"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
memcachednum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"memcached"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```









```bash
phpmem(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"php"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$6}; END{print sum}'
```



```bash
}
```



```bash
phpcpu(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"php"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
awk
```

```bash
'{sum+=$3}; END{print sum}'
```



```bash
}
```



```bash
phpnum(){
```



```bash

```

```bash
ps
```

```bash
aux|
```

```bash
grep
```

```bash
"php"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"grep"
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
"processstatus.sh"
```

```bash
|
```

```bash
wc
```

```bash
-l
```



```bash
}
```





```bash
case
```

```bash
"$1"
```

```bash
in
```



```bash
nginxmem)
```



```bash
nginxmem
```



```bash
;;
```



```bash
nginxcpu)
```



```bash
nginxcpu
```



```bash
;;
```



```bash
nginxnum)
```



```bash
nginxnum
```



```bash
;;
```



```bash
redismem)
```



```bash
redismem
```



```bash
;;
```



```bash
rediscpu)
```



```bash
rediscpu
```



```bash
;;
```



```bash
redisnum)
```



```bash
redisnum
```



```bash
;;
```



```bash
mysqlmem)
```



```bash
mysqlmem
```



```bash
;;
```



```bash
mysqlcpu)
```



```bash
mysqlcpu
```



```bash
;;
```



```bash
mysqlnum)
```



```bash
mysqlnum
```



```bash
;;
```



```bash
tomcatmem)
```



```bash
tomcatmem
```



```bash
;;
```



```bash
tomcatcpu)
```



```bash
tomcatcpu
```



```bash
;;
```



```bash
tomcatnum)
```



```bash
tomcatnum
```



```bash
;;
```



```bash
sentinelmem)
```



```bash
sentinelmem
```



```bash
;;
```



```bash
sentinelcpu)
```



```bash
sentinelcpu
```



```bash
;;
```



```bash
sentinelnum)
```



```bash
sentinelnum
```



```bash
;;
```



```bash
mongodbmem)
```



```bash
mongodbmem
```



```bash
;;
```



```bash
mongodbcpu)
```



```bash
mongodbcpu
```



```bash
;;
```



```bash
mongodbnum)
```



```bash
mongodbnum
```



```bash
;;
```



```bash
openfiremem)
```



```bash
openfiremem
```



```bash
;;
```



```bash
openfirecpu)
```



```bash
openfirecpu
```



```bash
;;
```



```bash
openfirenum)
```



```bash
openfirenum
```



```bash
;;
```



```bash
kafkamem)
```



```bash
kafkamem
```



```bash
;;
```



```bash
kafkacpu)
```



```bash
kafkacpu
```



```bash
;;
```



```bash
kafkanum)
```



```bash
kafkanum
```



```bash
;;
```



```bash
zookeepermem)
```



```bash
zookeepermem
```



```bash
;;
```



```bash
zookeepercpu)
```



```bash
zookeepercpu
```



```bash
;;
```



```bash
zookeepernum)
```



```bash
zookeepernum
```



```bash
;;
```



```bash
twemproxymem)
```



```bash
twemproxymem
```



```bash
;;
```



```bash
twemproxycpu)
```



```bash
twemproxycpu
```



```bash
;;
```



```bash
twemproxynum)
```



```bash
twemproxynum
```



```bash
;;
```



```bash
mycatmem)
```



```bash
mycatmem
```



```bash
;;
```



```bash
mycatcpu)
```



```bash
mycatcpu
```



```bash
;;
```



```bash
mycatnum)
```



```bash
mycatnum
```



```bash
;;
```



```bash
httpdmem)
```



```bash
httpdmem
```



```bash
;;
```



```bash
httpdcpu)
```



```bash
httpdcpu
```



```bash
;;
```



```bash
httpdnum)
```



```bash
httpdnum
```



```bash
;;
```



```bash
memcachedmem)
```



```bash
memcachedmem
```



```bash
;;
```



```bash
memcachedcpu)
```



```bash
memcachedcpu
```



```bash
;;
```



```bash
memcachednum)
```



```bash
memcachednum
```



```bash
;;
```



```bash
phpmem)
```



```bash
phpmem
```



```bash
;;
```



```bash
phpcpu)
```



```bash
phpcpu
```



```bash
;;
```



```bash
phpnum)
```



```bash
phpnum
```



```bash
;;
```



```bash
*)
```



```bash
echo
```

```bash
"Usage: $0 {nginxmem|nginxcpu|nginxnum|redismem|rediscpu|redisnum|mysqlmem|mysqlcpu|mysqlnum|mongodbnum|tomcatmem|tomcatcpu|tomcatnum|sentinelmem|sentinelcpu|sentinelnum|mongodbmem|mongodbcpu|mongodbnum|openfiremem|openfirecpu|openfirenum|kafkamem|kafkacpu|kafkanum|zookeepermem|zookeepercpu|zookeepernum|twemproxymem|twemproxycpu|twemproxynum|mycatmem|mycatcpu|mycatnum|httpdmem|httpdcpu|httpdnum|memcachedmem|memcachedcpu|memcachednum|phpmem|phpcpu|phpnum}"
```



```bash
esac
```
```





然后修改脚本的权限，使用：

chmod +x processstatus.sh

在zabbix_agentd.con.d/下面的配置文件中增加如下代码：



```
```bash
[9kgame@monitor zabbix_agentd.conf.d]$
```

```bash
cat
```

```bash
/data/zabbix/etc/zabbix_agentd
```

```bash
.conf.d
```

```bash
/process_num_cpu_mem
```

```bash
.conf
```



```bash
#monitor process
```



```bash
UserParameter=process.nginx.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh nginxmem
```



```bash
UserParameter=process.nginx.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh nginxcpu
```



```bash
UserParameter=process.nginx.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh nginxnum
```



```bash
UserParameter=process.redis.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh redismem
```



```bash
UserParameter=process.redis.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh rediscpu
```



```bash
UserParameter=process.redis.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh redisnum
```



```bash
UserParameter=process.mysql.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mysqlmem
```



```bash
UserParameter=process.mysql.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mysqlcpu
```



```bash
UserParameter=process.mysql.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mysqlnum
```



```bash
UserParameter=process.tomcat.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh tomcatmem
```



```bash
UserParameter=process.tomcat.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh tomcatcpu
```



```bash
UserParameter=process.tomcat.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh tomcatnum
```



```bash
UserParameter=process.sentinel.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh sentinelmem
```



```bash
UserParameter=process.sentinel.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh sentinelcpu
```



```bash
UserParameter=process.sentinel.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh sentinelnum
```



```bash
UserParameter=process.mongodb.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mongodbmem
```



```bash
UserParameter=process.mongodb.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mongodbcpu
```



```bash
UserParameter=process.mongodb.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mongodbnum
```



```bash
UserParameter=process.openfire.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh openfiremem
```



```bash
UserParameter=process.openfire.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh openfirecpu
```



```bash
UserParameter=process.openfire.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh openfirenum
```



```bash
UserParameter=process.kafka.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh kafkamem
```



```bash
UserParameter=process.kafka.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh kafkacpu
```



```bash
UserParameter=process.kafka.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh kafkanum
```



```bash
UserParameter=process.zookeeper.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh zookeepermem
```



```bash
UserParameter=process.zookeeper.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh zookeepercpu
```



```bash
UserParameter=process.zookeeper.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh zookeepernum
```



```bash
UserParameter=process.twemproxy.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh twemproxymem
```



```bash
UserParameter=process.twemproxy.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh twemproxycpu
```



```bash
UserParameter=process.twemproxy.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh twemproxynum
```



```bash
UserParameter=process.mycat.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mycatmem
```



```bash
UserParameter=process.mycat.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mycatcpu
```



```bash
UserParameter=process.mycat.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh mycatnum
```



```bash
UserParameter=process.httpd.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh httpdmem
```



```bash
UserParameter=process.httpd.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh httpdcpu
```



```bash
UserParameter=process.httpd.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh httpdnum
```



```bash
UserParameter=process.memcached.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh memcachedmem
```



```bash
UserParameter=process.memcached.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh memcachedcpu
```



```bash
UserParameter=process.memcached.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh memcachednum
```



```bash
UserParameter=process.php.memory,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh phpmem
```



```bash
UserParameter=process.php.cpu,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh phpcpu
```



```bash
UserParameter=process.php.number,
```

```bash
/data/zabbix/sbin/processstatus
```

```bash
.sh phpnum
```
```





最后重启zabbix_agentd服务

service zabbix_agentd restart

然后在zabbix服务端使用zabbix_get看能否取到相应的数据，像下面这样就是成功获取到了数据。




```bash
[root@localhost zabbix]
```

```bash
# bin/zabbix_get -s 172.16.1.20 -p 10050 -k process.nginx.memory
```



```bash
184876
```




最后，需要在zabbix中定义模板。模板附件链接在下面。

[zabbix模板下载](http://www.xianglinhu.com/resource/template_process)

如果模板无法下载可以在附件中下载模板

需要注明的是内存取到的值得单位是KB，所以定义item的时候使用自定义倍数乘以1000，单位改成Byte，另外CPU占用率的值是带有小数点的一个数，所以在定义item的时候需要定义值得类型是浮点型，并且该值是占用逻辑单核的CPU占用率，所以需要定义自定义倍数，我实验中的服务器是2颗CPU，每颗CPU是8核16线程，所以自定义倍数是原来的基础上除以32，单位改成%就好。

![wKiom1V-bluw3jXfAAGuQ1sx50A716.jpg](http://s3.51cto.com/wyfs02/M01/6E/80/wKiom1V-bluw3jXfAAGuQ1sx50A716.jpg)

![wKiom1V-bx_zMpagAAGZw-GzZBU009.jpg](http://s3.51cto.com/wyfs02/M00/6E/80/wKiom1V-bx_zMpagAAGZw-GzZBU009.jpg)

![wKiom1V-bs_QsEOBAAGS67C2qDc024.jpg](http://s3.51cto.com/wyfs02/M02/6E/80/wKiom1V-bs_QsEOBAAGS67C2qDc024.jpg)



下面是做好之后的显示效果：

![wKiom1V-baWxFO4LAAIC1oDItsQ527.jpg](http://s3.51cto.com/wyfs02/M02/6E/80/wKiom1V-baWxFO4LAAIC1oDItsQ527.jpg)

注意这里MySQL占用cpu为0.0031%，表示占用1个核心的0.0031%，因为有32核心，所以mysql占总的cpu应该为

0.0031%*32=0.0992%

![wKioL1V-b12zrX0UAAI-AE6sUBo325.jpg](http://s3.51cto.com/wyfs02/M00/6E/7C/wKioL1V-b12zrX0UAAI-AE6sUBo325.jpg)

[http://xianglinhu.blog.51cto.com/5787032/1657570](http://xianglinhu.blog.51cto.com/5787032/1657570)



附件：[http://down.51cto.com/data/2365812](http://down.51cto.com/data/2365812)









