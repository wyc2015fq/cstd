# 记录一次Ubuntu16.04上安装Elasticsearch踩的坑，memory lock问题 - weixin_33985507的博客 - CSDN博客
2018年05月16日 02:02:25[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
一直在用centos6，ES也安装过n多次了。这次公司新加了3台Ubuntu16.04的机器，作为ES的master节点。
直接从官网下载了5.6分支最新的DEB包，安装完成，然后执行systemctl start elasticsearch.service，
之后问题就来了，报错：
```
[1] bootstrap checks failed
[1]: memory locking requested for elasticsearch process but memory is not locked
```
很清晰么，要么是bootstrap.memory_lock: true这个没设置，要么就是max locked memory这个没配置么。
然后根据以往经验，打卡/etc/security/limits.conf。哐哐哐一顿修改:
```
* soft nofile 65536
* hard nofile 65536
* soft nproc 32000
* hard nproc 32000
* hard memlock unlimited
* soft memlock unlimited
```
之后重启，还是报上面的错误。以为没配好，之后又改了n次。还是不行。
然后查资料，果然，配置文件不一样了。。
limits.conf这里的配置，只适用于通过PAM认证登录用户的资源限制，它对systemd的service的资源限制不生效。登录用户的限制，通过 /etc/security/limits.conf 来配置
对于systemd service的资源限制，现在放在 /etc/systemd/system.conf 和 /etc/systemd/user.conf
这两个文件里面了。
主要就是/etc/systemd/system.conf这个文件
```
[Manager]
#LogLevel=info
#LogTarget=journal-or-kmsg
#LogColor=yes
#LogLocation=no
#DumpCore=yes
#ShowStatus=yes
#CrashChangeVT=no
#CrashShell=no
#CrashReboot=no
#CPUAffinity=1 2
#JoinControllers=cpu,cpuacct net_cls,net_prio
#RuntimeWatchdogSec=0
#ShutdownWatchdogSec=10min
#CapabilityBoundingSet=
#SystemCallArchitectures=
#TimerSlackNSec=
#DefaultTimerAccuracySec=1min
#DefaultStandardOutput=journal
#DefaultStandardError=inherit
#DefaultTimeoutStartSec=90s
#DefaultTimeoutStopSec=90s
#DefaultRestartSec=100ms
#DefaultStartLimitInterval=10s
#DefaultStartLimitBurst=5
#DefaultEnvironment=
#DefaultCPUAccounting=no
#DefaultBlockIOAccounting=no
#DefaultMemoryAccounting=no
#DefaultTasksAccounting=no
#DefaultTasksMax=
#DefaultLimitCPU=
#DefaultLimitFSIZE=
#DefaultLimitDATA=
#DefaultLimitSTACK=
#DefaultLimitCORE=
#DefaultLimitRSS=
#DefaultLimitNOFILE=
#DefaultLimitAS=
#DefaultLimitNPROC=
#DefaultLimitMEMLOCK=
#DefaultLimitLOCKS=
#DefaultLimitSIGPENDING=
#DefaultLimitMSGQUEUE=
#DefaultLimitNICE=
#DefaultLimitRTPRIO=
#DefaultLimitRTTIME=
```
然后修改下面几项:
```
DefaultLimitNOFILE=65536
DefaultLimitNPROC=32000
DefaultLimitMEMLOCK=infinity
```
之后继续操作:
```
/bin/systemctl daemon-reload
/bin/systemctl enable elasticsearch.service
systemctl start elasticsearch.service
systemctl status elasticsearch.service
```
到此，搞定。
后来centos又遇到这个问题了。
[https://www.elastic.co/guide/...](https://www.elastic.co/guide/en/elasticsearch/reference/5.6/setting-system-settings.html#sysconfig)
