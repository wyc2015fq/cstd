
# 向InfluxDB写入数据 - leofionn的博客 - CSDN博客


2018年05月01日 22:19:41[leofionn](https://me.csdn.net/qq_36142114)阅读数：1801


官网例子：https://docs.influxdata.com/influxdb/v1.2/query_language/data_download/
在InfluxDB中的数据是通过“时间序列组织的“，包含一个测量变量，比如cpu_load或者温度。时间序列有无数的点，每个点都是metric的离散采样。点包含time（一个时间戳）、一个measurement测量变量（比如是cpu_load），至少一个kv键值对`field`（测量变量的值，比如"value=0.64"，或者"temperature=21.2"），很多的kv键值对`tags`（包含了任何关于变量的元数据信息，比如"host=server01",“region=EMEA”, “dc=Frankfurt”） 。
在概念上，你可以把一个`measurement`看做是一个SQL表，主索引一直都是时间。`tags`和`fields`实际上是表中的列。`tags`会被索引，但是`fields`不会。区别是，使用InfluxDB，你可以有百万个测量变量，但是不需要设计前面的架构，空值不会被存储。
点会被写入到InfluxDB，使用行协议（Line Protocol），格式如下：
```python
<
```
```python
measurement
```
```python
>
```
```python
[,
```
```python
<
```
```python
tag-key
```
```python
>
```
```python
=
```
```python
<
```
```python
tag-value
```
```python
>
```
```python
...]
```
```python
<
```
```python
field-key
```
```python
>
```
```python
=
```
```python
<
```
```python
field-value
```
```python
>
```
```python
[,
```
```python
<
```
```python
field2-key
```
```python
>
```
```python
=
```
```python
<
```
```python
field2-value
```
```python
>
```
```python
...] [unix-nano-timestamp]
```
下面是可以写入到InfluxDB的点的所有例子：
```python
cpu,host=serverA,region=us_west value=0.64
payment,device=mobile,product=Notepad,method=credit billed=33,licenses=3i 1434067467100293230
stock,symbol=AAPL bid=127.46,ask=127.48
temperature,machine=unit42,
```
```python
type
```
```python
=assembly external=25,internal=37 1434067467000000000
```
使用influx命令行，可以插入一个个时间序列点，比如使用如下的语句：
`> INSERT cpu,host=serverA,region=us_west value=0.64
>`这样，一个点——测量变量是`cpu`，tags是`host`和`region`，测量值`value`为0.64，被写入到数据库中了。
然后，我们可以通过如下语句查到这条数据记录：
```python
> SELECT
```
```python
"host"
```
```python
,
```
```python
"region"
```
```python
,
```
```python
"value"
```
```python
FROM
```
```python
"cpu"
```
```python
name: cpu
---------
time                                     host       region   value
2015-10-21T19:28:07.580664347Z  serverA   us_west    0.64
```
我们尝试存储另外一种数据，在一个测量变量里含有两种fields：
```python
> INSERT temperature,machine=unit42,
```
```python
type
```
```python
=assembly external=25,internal=37
>
```
一次查询操作中，可以使用`*`操作符获取所有的fields和tags：
```python
> SELECT * FROM
```
```python
"temperature"
```
```python
name: temperature
-----------------
time                                     external     internal   machine
```
```python
type
```
```python
2015-10-21T19:28:08.385013942Z  25              37          unit42  assembly
>
```
## 从外部文件导入数据
导入的文件包含以下的两段：
DLL(数据定义语言)：包含创建相关数据库和管理retention policy的InfluxQL命令。如果你的数据库和retention policy已经存在，你的文件可以跳过这个段。
DML（数据管理语言）：列出相关的数据库、retention policy（如果需要的话），并包含行协议的数据
比如，文件`datarrr.txt`：
```python
# DDL
```
```python
CREATE DATABASE pirates
CREATE RETENTION POLICY oneday ON pirates DURATION 1d REPLICATION 1
```
```python
# DML
```
```python
# CONTEXT-DATABASE: pirates
```
```python
# CONTEXT-RETENTION-POLICY: oneday
```
```python
treasures,captain_id=dread_pirate_roberts value=801 1439856000
treasures,captain_id=flint value=29 1439856000
treasures,captain_id=sparrow value=38 1439856000
treasures,captain_id=tetra value=47 1439856000
treasures,captain_id=crunch value=109 1439858880
```
导入命令如下：
```python
$influx
```
```python
-import -path=datarrr.txt -precision=s
```
结果如下：
`2015/12/22 12:25:06 Processed 2 commands
2015/12/22 12:25:06 Processed 5 inserts
2015/12/22 12:25:06 Failed 0 inserts`
## 参考
[query_language](https://link.jianshu.com?t=https://docs.influxdata.com/influxdb/v1.2/query_language/spec/)
[tools-influxdb](https://link.jianshu.com?t=https://docs.influxdata.com/influxdb/v1.2/tools/)
[line-protocol](https://link.jianshu.com?t=https://docs.influxdata.com/influxdb/v1.2/concepts/glossary/#line-protocol)
[launch](https://link.jianshu.com?t=https://docs.influxdata.com/influxdb/v1.2/tools/shell/#launch-influx)-[influx](https://link.jianshu.com?t=https://docs.influxdata.com/influxdb/v1.2/tools/shell/#launch-influx)
[database_management](https://link.jianshu.com?t=https://docs.influxdata.com/influxdb/v1.2/query_language/database_management/)
[retention-policy](https://link.jianshu.com?t=https://docs.influxdata.com/influxdb/v1.2/concepts/glossary/#retention-policy-rp)

作者：itfanr
链接：https://www.jianshu.com/p/097f8fcb0a78
來源：简书

