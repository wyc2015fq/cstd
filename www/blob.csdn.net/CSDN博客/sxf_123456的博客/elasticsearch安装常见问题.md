# elasticsearch安装常见问题 - sxf_123456的博客 - CSDN博客
2018年01月11日 20:09:16[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：710
                
# 1 max file descriptors [65535] for elasticsearch process likely too low, increase to at least [65536]
    vim /etc/security/limits.conf
    添加如下内容:
    elasticsearch  -  nofile  65536
# 2 elasticsearch dead but subsys locked
    rm /var/lock/subsys/elasticsearch
# 3 system call filters failed to install; check the logs and fix your configuration or disable system call filters at your own risk
    centos6 不支持 bootstrap.system_call_filter
    修改elasticsearch.yml设置bootstrap.system_call_filter: false
# 4 安装X-pack证书
    curl -XPUT -u elastic 'http://<host>:<port>/_xpack/license' -H "Content-Type: application/json" -d @license.json
# 5  max number of threads [1024] for user [elasticsearch] is too low, increase to at least [2048]
    vim /etc/security/limits.conf
    添加如下内容:
    elasticsearch - nproc 2048
# 6 error: "net.bridge.bridge-nf-call-ip6tables" is an unknown key 
    先执行
    modprobe bridge
# 7 memory locking requested for elasticsearch process but memory is not locked
    vim /etc/security/limits.conf
    添加如下内容
    elasticsearch soft memlock unlimited
    elasticsearch hard memlock unlimited
# 8 如果怎么也连接不上主节点
    修改防火墙配置或者关闭防火墙
    service iptables stop
# 9 detected index data in default.path.data [/var/lib/elasticsearch] where there should not be any
    这是因为在配置自定义datapath前启动了es, 确认之前的启动是误启动，删除留下的文件即可
    rm -rf /var/lib/elasticsearch/nodes
