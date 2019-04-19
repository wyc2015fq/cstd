# Why we choose Couchbase - 三少GG - CSDN博客
2014年08月01日 16:58:11[三少GG](https://me.csdn.net/scut1135)阅读数：656
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

# Why we choose Couchbase?
发表于[2013/11/11](http://windwrite.com/archives/760)由[风河](http://windwrite.com/archives/author/admin)
We ([YY Game](http://game.yy.com/)) have launched a product that requires the distributed storage
 with high TPS (3000+ is the least). Each object is about 10KB. Firstly we consider the object storage system Swift (an openstack child project). We tested it for many times with 5 storage nodes, 1-3 proxy nodes, setup all Account, Container, Object servers
 with SSD storage. But due to the structure limit, it never reaches the wanted performance. In fact we tested the best TPS with about 800. Why I said the structure limit, because Swift proxy communicates to the storage servers via network, and all files read/write
 must be passed through the proxy. So proxy becomes the performance bottle, all IO operations to the backend storage via network are really slow.
We have been using Redis in our production environment widely, for a memory cache, Redis is fast. But why not Redis? The primary reason is that Redis is not cluster oriented. For our product, the capability should be increasing over the time. But until now
 Redis doesn’t handle this well, it is hard to keep the capability and performance increase linearly by adding more nodes into the cluster. So we gave up Redis.
Finally we choose Couchbase. It has the advantages which are very suitable for us:
- 
Pretty good performance, we tested it getting the TPS as good as 20K+
- 
Very good scalability, scaling up or down the cluster is honestly easy
- 
Performance and capability can be growing linearly by adding nodes into the cluster
- 
No single point of failure or performance bottle
- 
Data can be saved with replicas and auto failover to be setup
- 
The amazing web management and realtime statistics systems
We have 8 nodes, each with 48 GB memory, 17 TB disk (Raid5) mounted as a separated partition. So we have the cluster with 320 GB memory, 130 TB disk totally (should keep some resources for the system).
Until now the cluster run well for our situation, thanks to Couchbase Server and the development team.
