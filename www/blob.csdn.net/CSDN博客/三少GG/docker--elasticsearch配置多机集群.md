# docker--elasticsearch配置多机集群 - 三少GG - CSDN博客
2015年09月06日 18:27:24[三少GG](https://me.csdn.net/scut1135)阅读数：2851
【如何构建elasticsearch】
添加如下语句：
RUN chmod 755 /docker-entrypoint.sh
# Elasticsearch 权威指南（中文版）
## [http://es.xiaoleilu.com/](http://es.xiaoleilu.com/)

# Elasticsearch不同网段自动发现集群配置
创建时间：2014-05-02 20:33:55.0   阅读次数：1151
[](http://www.ruizhishu.com/overt/article/view/38#)
Discovery(发现)
在配置文件（elasticsearch.yml）中加入discovery.zen.ping.unicast.hosts属性，值等于被发现的Elasticsearch主机IP地址和Transport通讯端口，多个主机用逗号分隔。
配置格式有多种，如：
discovery.zen.ping.unicast.hosts: ["host1", "host2:port", "host3[portX-portY]"]
discovery.zen.ping.unicast.hosts: host1:port,host2:port
实例：
discovery.zen.ping.unicast.hosts: 10.111.125.3:9300,10.111.125.4:9300,10.111.125.5:9300,10.111.125.6:9300
已解决镜像，但不完美：
[https://hub.docker.com/r/itzg/elasticsearch/](https://hub.docker.com/r/itzg/elasticsearch/)
lang groovy are disabled
https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-scripting.html
