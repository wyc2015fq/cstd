# hbase region move - 数据之美的博客 - CSDN博客
2018年11月14日 14:13:52[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：132
hbase使用过程中，由于负载不均衡，或者出于某种需要，要将某些区域移动至指定的RS，即区域移动，方法很简单，hbase shell中输入移动回车查看移动用法：
```
hbase(main):001:0> move
ERROR: wrong number of arguments (0 for 1)
Here is some help for this command:
Move a region.  Optionally specify target regionserver else we choose one
at random.  NOTE: You pass the encoded region name, not the region name so
this command is a little different to the others.  The encoded region name
is the hash suffix on region names: e.g. if the region name were
TestTable,0094429456,1289497600452.527db22f95c8a9e0116f0cc13c680396. then
the encoded region name portion is 527db22f95c8a9e0116f0cc13c680396
A server name is its host, port plus startcode. For example:
host187.example.com,60020,1289493121758
Examples:
  hbase> move 'ENCODED_REGIONNAME'
  hbase> move 'ENCODED_REGIONNAME', 'SERVER_NAME'
```
解释说的很清楚，但是这里面涉及到一个起始码，知不道这是什么啊。查了一下提到这个的真不多。
去RS 60030上查看，tdh05,60020,1542163054306，最后这串数应该就是startcode了。
![](https://img-blog.csdnimg.cn/20181114135916842.png)
近日发现。。。还可以直接去master的60010主界面直接查看所有节点的信息
![](https://img-blog.csdnimg.cn/20181119193309537.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
另外通过查看博客，参考[https://stackoverflow.com/questions/11203147/how-to-get-regionservers-startcode-in-a-hbase-cluster](https://stackoverflow.com/questions/11203147/how-to-get-regionservers-startcode-in-a-hbase-cluster)
起始码可以通过如下方式获得：
```
hbase(main):001:0> status 'simple'
SLF4J: Class path contains multiple SLF4J bindings.
SLF4J: Found binding in [jar:file:/root/TDH-Client/hyperbase/lib/slf4j-log4j12-1.7.10.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding in [jar:file:/root/TDH-Client/hadoop/hadoop/lib/slf4j-log4j12-1.7.10.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding in [jar:file:/root/TDH-Client/zookeeper/lib/slf4j-log4j12-1.6.1.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: See http://www.slf4j.org/codes.html#multiple_bindings for an explanation.
2018-11-14 13:19:13,346 INFO util.KerberosUtil: Using principal pattern: HTTP/_HOST
2018-11-14 13:19:14,353 INFO Configuration.deprecation: io.bytes.per.checksum is deprecated. Instead, use dfs.bytes-per-checksum
3 live servers
    tdh05:60020 1542163054306
        requestsPerSecond=0.0, numberOfOnlineRegions=581, usedHeapMB=948, maxHeapMB=23400, numberOfStores=581, numberOfStorefiles=1530, storefileUncompressedSizeMB=35549281, storefileSizeMB=8544796, compressionRatio=0.2404, memstoreSizeMB=0, storefileIndexSizeMB=0, readRequestsCount=0, writeRequestsCount=0, rootIndexSizeKB=35340, totalStaticIndexSizeKB=38074359, totalStaticBloomSizeKB=1111557794, totalCompactingKVs=0, currentCompactedKVs=0, compactionProgressPct=NaN, coprocessors=[]
    tdh07:60020 1542163055490
        requestsPerSecond=0.0, numberOfOnlineRegions=578, usedHeapMB=1060, maxHeapMB=23400, numberOfStores=578, numberOfStorefiles=1586, storefileUncompressedSizeMB=36320289, storefileSizeMB=8767011, compressionRatio=0.2414, memstoreSizeMB=0, storefileIndexSizeMB=0, readRequestsCount=8, writeRequestsCount=0, rootIndexSizeKB=37217, totalStaticIndexSizeKB=38839670, totalStaticBloomSizeKB=1135920048, totalCompactingKVs=0, currentCompactedKVs=0, compactionProgressPct=NaN, coprocessors=[]
    tdh06:60020 1542163054369
        requestsPerSecond=0.0, numberOfOnlineRegions=580, usedHeapMB=875, maxHeapMB=23400, numberOfStores=580, numberOfStorefiles=1609, storefileUncompressedSizeMB=35828293, storefileSizeMB=8646905, compressionRatio=0.2413, memstoreSizeMB=0, storefileIndexSizeMB=0, readRequestsCount=320265, writeRequestsCount=1129, rootIndexSizeKB=39721, totalStaticIndexSizeKB=38353161, totalStaticBloomSizeKB=1119650592, totalCompactingKVs=52694, currentCompactedKVs=52694, compactionProgressPct=1.0, coprocessors=[]
30 dead servers
    tdh01,60020,1527746297076
    tdh08,60020,1540193139117
    tdh03,60020,1527746079267
    tdh08,60020,1527745284974
    tdh02,60020,1540182188221
    tdh08,60020,1527745732279
    tdh01,60020,1527745283401
    tdh03,60020,1542163054677
    tdh04,60020,1527745727995
    tdh02,60020,1542163058601
    tdh04,60020,1540193137506
    tdh01,60020,1542163054614
    tdh03,60020,1527746297224
    tdh01,60020,1527730457808
    tdh08,60020,1542163057616
    tdh04,60020,1540175857760
    tdh03,60020,1527745283754
    tdh04,60020,1527746298654
    tdh02,60020,1527730457889
    tdh03,60020,1540182188106
    tdh01,60020,1540193137448
    tdh02,60020,1540193137442
    tdh04,60020,1542163054490
    tdh01,60020,1540175856508
    tdh04,60020,1540182188213
    tdh03,60020,1527745727622
    tdh02,60020,1527746079010
    tdh02,60020,1540175856656
    tdh01,60020,1527746078978
    tdh03,60020,1527730457862
Aggregate load: 0, regions: 1739
```
可以看到提供的tdh05的起始码码为**1542163054306，**
来测试一下，测试表现在情况是这样：![](https://img-blog.csdnimg.cn/20181114134240102.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
现在我们尝试把他移动到tdh07上去。先状态'simple'一下：
![](https://img-blog.csdnimg.cn/20181114134458540.png)
![](https://img-blog.csdnimg.cn/20181114134617637.png)
现在查看一下60010表的信息
![](https://img-blog.csdnimg.cn/20181114134655981.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
PS：一个有趣的发现：在上面移动完成以后，我还需要将区域移回去至tdh05，查看一下60030下面的详细信息，也会有一个类似的码，试一下：
![](https://img-blog.csdnimg.cn/2018111413572639.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20181114135320189.png)
再查看表的信息：
![](https://img-blog.csdnimg.cn/20181114135529300.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
移到了tdh06上.....所以说用这个作为起始码竟然也可以移动。但是没移对地方....
