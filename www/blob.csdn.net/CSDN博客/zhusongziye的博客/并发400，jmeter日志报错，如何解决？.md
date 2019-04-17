# 并发400，jmeter日志报错，如何解决？ - zhusongziye的博客 - CSDN博客





2018年01月21日 13:34:09[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1897








                
   hi 大家好，最近有点忙，还是抽空坚持分享，这是性能测试中遇到的一个问题，下面我们一起看看：



问题背景：

还是之前做的某教育项目Web场景压测，通过对压力机端口资源的配置调优之后，单台压力机并发数可达到300，在此基础上逐渐加大并发，当并发数达到400时，部分接口出现概率性报错，且服务端无报错日志，Jmeter错误日志：

![](https://img-blog.csdn.net/20171026215129047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







分析：根据Jmeter接口返回的错误日志，对压力机的其它参数做配置调优，发现调优效果并不显著，于是将问题原因定位到服务端，可能是服务端有较多请求正在处理（且处理时间较长），导致Jmeter不能连接上服务器而产生的。通过对服务端进行配置调优，单台压力机并发数可增加到500并发，下面给大家分享下我在解决此问题过程中对服务端做的几个调优项,刚入门的新手，欢迎大家交流指教。



方案：服务端的配置调优

调优项1、对web服务器系统的参数进行配置调优，修改/etc/sysctl.conf和/etc/security/limits.conf，具体修改如下：

在/etc/sysctl.conf文件末尾添加：

vm.swappiness= 0

net.ipv4.neigh.default.gc_stale_time=120

net.ipv4.conf.all.rp_filter=0

net.ipv4.conf.default.rp_filter=0

net.ipv4.conf.default.arp_announce= 2

net.ipv4.conf.all.arp_announce=2

net.ipv4.tcp_max_tw_buckets= 5000

net.ipv4.tcp_syncookies= 1

net.ipv4.tcp_max_syn_backlog= 1024

net.ipv4.tcp_synack_retries= 2

net.ipv4.conf.lo.arp_announce=2

net.ipv4.tcp_fin_timeout= 60

net.ipv4.tcp_tw_reuse= 1

net.ipv4.tcp_tw_recycle= 1

net.ipv4.tcp_syncookies= 1

net.ipv4.tcp_max_syn_backlog= 16384

net.ipv4.tcp_max_tw_buckets= 36000

net.ipv4.route.gc_timeout= 100

net.ipv4.tcp_syn_retries= 2

net.ipv4.tcp_synack_retries= 1

执行sysctl -p是配置生效

在/etc/security/limits.conf文件末尾添加：

*        soft  nproc  65535

*        hard  nproc  65535

*        soft  nofile  65535

*        hard  nofile  65535

执行service sshdrestart使配置生效



调优项2、将tomcat的server.xml 配置文件，将http连接池数调大，重启tomcat，继续进行压测





调优项3、对数据库的连接数进行调整，修改tomcat/conf/context.xml配置文件，新增  maxActive="300"maxIdle="30" maxWait="1000"，重启服务，保证配置生效
            


