# InfluxDB 数据库实例存储策略设置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月23日 11:08:00[boonya](https://me.csdn.net/boonya)阅读数：317








原文地址：[https://www.linuxdaxue.com/retention-policies-in-influxdb.html](https://www.linuxdaxue.com/retention-policies-in-influxdb.html)

## 一、InfluxDB 数据保留策略 说明

InfluxDB的数据保留策略（RP） 用来定义数据在InfluxDB中存放的时间，或者定义保存某个期间的数据。

一个数据库可以有多个保留策略，但每个策略必须是独一无二的。

## 二、InfluxDB 数据保留策略 目的

InfluxDB本身不提供数据的删除操作，因此用来控制数据量的方式就是定义数据保留策略。

因此定义数据保留策略的目的是让InfluxDB能够知道可以丢弃哪些数据，从而更高效的处理数据。

## 三、InfluxDB 数据保留策略 操作

1）查询策略

可以通过如下语句查看数据库的现有策略：

```
> SHOW RETENTION POLICIES ON telegraf
name    duration    shardGroupDuration    replicaN    default
default    0        168h0m0s        1        true
```

可以看到，telegraf只有一个策略，各字段的含义如下：

name--名称，此示例名称为 default

duration--持续时间，0代表无限制

shardGroupDuration--shardGroup的存储时间，shardGroup是InfluxDB的一个基本储存结构，应该大于这个时间的数据在查询效率上应该有所降低。

replicaN--全称是REPLICATION，副本个数

default--是否是默认策略

2）新建策略

```
> CREATE RETENTION POLICY "2_hours" ON "telegraf" DURATION 2h REPLICATION 1 DEFAULT
> SHOW RETENTION POLICIES ON telegraf
name    duration    shardGroupDuration    replicaN    default
default    0        168h0m0s        1        false
2_hours    2h0m0s        1h0m0s            1        true
```

通过上面的语句可以添加策略，本例在 telegraf 库添加了一个2小时的策略，名字叫做 2_hours， duration为2小时，副本为1，设置为默认策略。

因为名为default的策略不再是默认策略，因此，在查询使用default策略的表时要显式的加上策略名 “default”。

```
> select * from "default".cpu limit 2
name: cpu
---------
time            cpu        host                host_id    usage_guest    usage_guest_nice    usage_idle   usage_iowait        usage_irq    usage_nice        usage_softirq    usage_steal    usage_system        usage_user
1467884670000000000    cpu-total    ResourcePool-0246-billing07        0        0            99.79994164175388    0            0        0.06251823446523729    0        0        0.12920435125646068    0.008335764603451727
1467884670000000000    cpu9        billing07                0        0            97.79338014069532    1.8054162487519367    0        0            0        0        0.10030090272883943    0.3009027081135398
```

3）修改策略

修改策略使用如下语句修改

```
> ALTER RETENTION POLICY "2_hours" ON "telegraf" DURATION 4h DEFAULT
> show retention POLICIES on telegraf
name    duration    shardGroupDuration    replicaN    default
default    0        168h0m0s        1        false
2_hours    4h0m0s        1h0m0s            1        true
```

可以看到，修改后的策略发生了变化。

4）删除策略

InfluxDB中策略的删除操作如下所示：

```
> drop retention POLICY "2_hours" ON "telegraf"
> show retention POLICIES on telegraf
name    duration    shardGroupDuration    replicaN    default
default    0        168h0m0s        1        false
```

可以看到，名为2_hours的策略已经被删除了。

## 四、其他说明

策略这个关键词“POLICY”在使用是应该大写，小写应该会出粗。

当一个表使用的策略不是默认策略时，在进行操作时一定要显式的指定策略名称，否则会出现错误。

## 五、练习示例

```bash
>  CREATE RETENTION POLICY "20_days" ON "statistics" DURATION 20d REPLICATION 1 DEFAULT
ERR: database not found: statistics
>  CREATE RETENTION POLICY "20_days" ON "rtvsweb" DURATION 20d REPLICATION 1 DEFAULT
> SHOW RETENTION POLICIES ON rtvsweb
name    duration shardGroupDuration replicaN default
----    -------- ------------------ -------- -------
autogen 0s       168h0m0s           1        false
20_days 480h0m0s 24h0m0s            1        true
>
```





