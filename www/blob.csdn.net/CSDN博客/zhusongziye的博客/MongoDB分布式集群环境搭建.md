# MongoDB分布式集群环境搭建 - zhusongziye的博客 - CSDN博客





2018年11月10日 22:18:59[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：129








**第一节分片的概念**

　　分片（sharding）是指根据片键，将数据进行拆分，使其落在不同的机器上的过程。如此一来，不需要功能，配置等强大的机器，也能储存大数据量，处理更高的负载。



**第二节分片的原理和思想**

　　MongoDB分片的基本思想就是将集合切分成小块。这些块分散到若干片里面，每个片只负责总数据的一部分。

　　对于客户端来说，无需知道数据被拆分了，也无需知道服务端哪个分片对应哪些数据。数据在分片之前需要运行一个路由进程，进程名为mongos。这个路由器知道所有数据的存放位置，知道数据和片的对应关系。对客户端来说，它仅知道连接了一个普通的mongod，在请求数据的过程中，通过路由器上的数据和片的对应关系，路由到目标数据所在的片上，如果请求有了回应，路由器将其收集起来回送给客户端

![](https://img-blog.csdnimg.cn/20181110221822855.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)





**第三节分片的简单实现**

1.片键的概念

　　设置分片时，需要从集合里面选一个键，用该键的值作为数据拆分的依据。这个键称为片键(shard key)。{department:"IT",name:"zhangsan"},{department:"HR",name:"lisi"},{department:"SUPPORT",name:"zhaowu"}

　　以该数据为例，表示的是职员名字以及所在的部门，假若我们设置部门（department）为片键，那么第一片可能存放名称以字母A-F开头的部门，第二片存放名称以G~P开头的部门，第三片存Q~Z，如此类推。随着添加或者删除片，MongoDB会重新平衡数据，使每片的流量都比较均衡，数据量也在合理范围内。



2.本地模拟实现

　 a.首先mongod开启服务端3个节点，端口分别为8080,8081,8082

　 b.开启config服务器 。mongos要把mongod之间的配置放到config服务器里面，所以首先开启它，这里就使用8083端口。 命令为：mongod --dbpath E:\sharding\config_node --port 8083

　 c、开启mongos服务器 。端口8084，同时指定下config服务器。命令为：mongos --port 8084 --configdb=127.0.0.1:8083

　 d.路由指定服务端节点。客户端直接跟mongos打交道，也就说明我们要连接mongos服务器，然后将8080,8081,8082的mongod交给mongos,添加分片也就是addshard()。需要进到路由节点的admin数据库进行配置。allowLocal表示该服务端节点，可被客户端直接连接而无需经过路由。

　　　　db.runCommand({"addshard":"127.0.0.1:8080",allowLocal:true})

　　　　　　　　db.runCommand({"addshard":"127.0.0.1:8081",allowLocal:true})

　　　　　　　　db.runCommand({"addshard":"127.0.0.1:8082",allowLocal:true})

　 e.开启数据库分片功能，命令为enablesharding()，并指定数据库名称，如下指定每个mongod都有的test数据库

db.runCommand({"enablesharding":"test"})

　 f.指定集合中分片的片键，这里就指定为company.department键.　　　　　　db.runCommand({"shardcollection":"company.department","key":{"depatment":1}})

　 g.通过向mongos节点插入100万条数据，查看各节点保存的数据。

　 h.通过向mongos插入一条数据，查看数据被分配到哪个片





