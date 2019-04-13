
# 安装Hbase（分布式）遇到一些问题及解决方法 - Magician的博客 - CSDN博客


2017年11月17日 01:19:25[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：233所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



问题一：安装完成后在Hbase shell 命令行执行list命令时，爆出如下错误：
```python
hbase(main):
```
```python
001
```
```python
:
```
```python
0
```
```python
> list
TABLE                                                                                                                                                                                          
SLF4J: Class path contains multiple SLF4J bindings.
SLF4J: Found binding
```
```python
in
```
```python
[jar:
```
```python
file
```
```python
:/home/hadoop/hbase-
```
```python
0.98
```
```python
.9
```
```python
-hadoop2/lib/phoenix-
```
```python
4.2
```
```python
.2
```
```python
-client.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding
```
```python
in
```
```python
[jar:
```
```python
file
```
```python
:/home/hadoop/hbase-
```
```python
0.98
```
```python
.9
```
```python
-hadoop2/lib/slf4j-log4j12-
```
```python
1.6
```
```python
.4
```
```python
.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding
```
```python
in
```
```python
[jar:
```
```python
file
```
```python
:/home/hadoop/hadoop-
```
```python
2.6
```
```python
.0
```
```python
/share/hadoop/common/lib/slf4j-log4j12-
```
```python
1.7
```
```python
.5
```
```python
.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: See http://www.slf4j.org/codes.html#multiple_bindings
```
```python
for
```
```python
an explanation.
```
```python
2017
```
```python
-
```
```python
07
```
```python
-
```
```python
14
```
```python
15
```
```python
:
```
```python
06
```
```python
:
```
```python
48
```
```python
,
```
```python
032
```
```python
WARN  [main] util.NativeCodeLoader: Unable
```
```python
to
```
```python
load native-hadoop
```
```python
library
```
```python
for
```
```python
your platform... using builtin-java classes where applicable
ERROR: Can
```
```python
't
```
```python
get master address from ZooKeeper; znode data ==
```
```python
null
```
```python
Here
```
```python
is
```
```python
some help
```
```python
for
```
```python
this command:
List
```
```python
all
```
```python
tables
```
```python
in
```
```python
hbase. Optional regular expression parameter could
be used
```
```python
to
```
```python
filter the output. Examples:
  hbase> list
  hbase> list
```
```python
'abc
```
```python
.*'
  hbase> list
```
```python
'ns
```
```python
:abc.*'
  hbase> list
```
```python
'ns
```
```python
:.*'
```
对应的部分log内容：
![这里写图片描述](https://img-blog.csdn.net/20171117005648957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
由以上的信息得出问题是不能从zookeeper中得到master地址。用hbase zkcli命令查看
```python
zk: xxxx:
```
```python
2181
```
```python
(CONNECTED)
```
```python
0
```
```python
]
```
```python
ls /
```
```python
[
```
```python
hbase, zookeeper
```
```python
]
```
```python
[
```
```python
zk: xxxx:
```
```python
2181
```
```python
(CONNECTED)
```
```python
1
```
```python
]
```
```python
ls /hbase
```
```python
[
```
```python
meta
```
```python
-region
```
```python
-server
```
```python
, backup
```
```python
-masters
```
```python
, table, draining, region
```
```python
-in
```
```python
-transition
```
```python
, table
```
```python
-lock
```
```python
, balancer, namespace, hbaseid, online
```
```python
-snapshot
```
```python
, replication, splitWAL, recovering
```
```python
-regions
```
```python
, rs
```
```python
]
```
确实没有master文件夹。然后就关闭hbase和hadoop，清空zookeeper的data目录（不清楚data目录在哪，可在zookeeper/conf下的zoo.cfg文件中查看如下图所示），在重启就好了。
![这里写图片描述](https://img-blog.csdn.net/20171117004318288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117004318288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
出现了master文件夹
[

](https://img-blog.csdn.net/20171117004318288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
[
```
```python
zk: xxxx:
```
```python
2181
```
```python
(CONNECTED)
```
```python
0
```
```python
]
```
```python
ls /hbase
```
```python
[
```
```python
meta
```
```python
-region
```
```python
-server
```
```python
, backup
```
```python
-masters
```
```python
, table, draining, region
```
```python
-in
```
```python
-transition
```
```python
, table
```
```python
-lock
```
```python
, running, master, namespace, hbaseid, online
```
```python
-snapshot
```
```python
, replication, splitWAL, recovering
```
```python
-regions
```
```python
, rs
```
```python
]
```
[
](https://img-blog.csdn.net/20171117004318288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)但问题的具体原因还不清除。
[
](https://img-blog.csdn.net/20171117004318288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)问题二：
同样在执行list命令时，又报错ERROR:org.apache.hadoop.hbase.PleaseHoldException: Master is initializing。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20171117010639070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117010639070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
进一步查看关键log信息如下:
![这里写图片描述](https://img-blog.csdn.net/20171117010948643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117010948643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
发现好像是有关host配置的问题造成节点间的链接出现了异常，于是又去检查了host的配置信息以及hbase-site的配置，最终发现原来是在配置hbase-site时将hbase.rootdir这个属性配置错了，更改后就OK了（如果在安装Hbase时，是一个节点配置错误后将配置文件传到其他的各个节点，记得要更改所有的节点的配置文件）
至于其他的问题想起来再接着更新吧。
但是不管是什么问题，如果不能一下搞定记得去查看LOG，根据log去分析问题，你就能找到问题的解决办法。

[
](https://img-blog.csdn.net/20171117010948643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
