# elasticsearch docker安装 - 三少GG - CSDN博客
2018年06月17日 16:08:01[三少GG](https://me.csdn.net/scut1135)阅读数：688
必看：
https://www.elastic.co/guide/en/elasticsearch/reference/current/docker.html
http://www.xiaozhugrape.com/2018/03/29/elasticsearch%E6%9C%AC%E5%9C%B0%E5%AE%89%E8%A3%85docker%E5%AE%89%E8%A3%85/
1. 在配置阶段： 
` - "ES_JAVA_OPTS=-Xms2g -Xmx2g"`
2.在虚拟机内:  docker exec -it  {CONTAINER ID} bash
- vm.max_map_count 报错
The `vm.max_map_count` setting should be set permanently in /etc/sysctl.conf:
$ grep vm.max_map_count /etc/sysctl.conf
vm.max_map_count=262144
To apply the setting on a live system type: `sysctl -w vm.max_map_count=262144`
```
```
但留意可能报错： setting key "vm.max_map_count": Read-only file system
原因解释(中文):   [https://blog.csdn.net/Gamer_gyt/article/details/52960941](https://blog.csdn.net/Gamer_gyt/article/details/52960941)
[https://github.com/spujadas/elk-docker/issues/99](https://github.com/spujadas/elk-docker/issues/99)
参考：
https://github.com/elastic/elasticsearch/issues/25067
