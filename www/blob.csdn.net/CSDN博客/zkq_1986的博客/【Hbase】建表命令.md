# 【Hbase】建表命令 - zkq_1986的博客 - CSDN博客





2017年11月20日 11:03:54[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：210








# Hbase 建表基本命令




1. create 't1','c1','c2'

表示创建一个t1表，列簇为c1和c2两个。




**2. 预分区**






TTL的单位为秒




create 't2',{NAME=>'f1',COMPRESSION=>'SNAPPY',TTL=>63072000},SPLITS=>['0001','0002','0003','0004']


一个表只能有一个预分区策略，不能每个列簇单独一个




下面，我们登陆一下master的web页面<Hmaster:60010>，查看一下hbase的表信息，找到刚刚新建的预分区表，进入查看region信息：

![](https://img-blog.csdn.net/20150605145011779)


我们看到第一个region是没有startkey的，最后一个region是没有stopkey的。





