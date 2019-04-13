
# elasticsearch外场分片找回-UNASSIGNED - 3-Number - CSDN博客


置顶2017年09月06日 12:11:27[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：3451


# 0x01 缘由
产品开发过程中没有专人去深入理解elasticsearch相关原理，导致在产品生产部署时，没有做到合理的物理架构部署，导致后期问题不断出现。
当外场出现服务器资源瓶颈时，紧急调整相关结构，忙中出错，调整主节点时，导致某个索引无法找回相关分片。类似：
![](https://img-blog.csdn.net/20170906120916283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
1、3分片 “UNASSIGNED”
# 0x02 场景描述
软件：elasticsearch 5.1.1 版本 5个节点 1个主节点
运行软件： 上面跑各种程序，导致资源使用需要非常珍惜。
a.尝试修改elasticsearch/config/elasticsearch.yml 中相关参数，即作为数据节点，又作为主节点如下：
node.master: true
node.data: true
b.重启es
重启es几分钟后，重新分片还未完成，就开始重启其他节点。
c.过了几个小时后，发现一个索引分片无法找回，状态变为RED；
后台日志报错：
017-08-31T15:37:06,018][WARN ][o.e.g.DanglingIndicesState] [node8] [[wide_protocols_215a_201707/j7yRa0RoT6eQPZzl67wv3g]] can not be importe
d as a dangling index, as index with same name already exists in cluster metadata
# 0x03 问题分析
数据对于现网运行环境来说，比较重要。所以得想办法去恢复索引分片。
a.查看索引分片的uuuid:
![](https://img-blog.csdn.net/20170906120931403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
b.然后进入后台数据存储路径，查看，发现7月份索引两个分片信息，这也是导致es后台日志有如下警告的原因：
017-08-31T15:37:06,018][WARN ][o.e.g.DanglingIndicesState] [node8] [[wide_protocols_215a_201707/j7yRa0RoT6eQPZzl67wv3g]] can not be importe
d as a dangling index, as index with same name already exists in cluster metadata
c.导致es状态总是RED，影响到数据的检索速度等。
# 0x04 解决思路和办法
解决问题的过程中尝试了很多做法：
1、强制重新分片---无用
2、尝试修改分片文件信息---无用
最后，可行的方法是：
1、将所有节点新生成的UUID对象的文件备份移走；
2、通过head等工具，直接删除该索引；
3、ES立马把老的索引信息恢复；
# 0x05 总结
解决问题是一个逻辑推理的过程，只有根据相关信息和理论去找原因，然后不断在开发环境下尝试，最后总会找到解决问题的方法。

