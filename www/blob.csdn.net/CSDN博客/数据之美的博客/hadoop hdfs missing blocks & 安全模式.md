# hadoop hdfs missing blocks & 安全模式 - 数据之美的博客 - CSDN博客
2018年11月19日 11:31:28[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：523
近期在做磁盘有关的一些测试时，最先是发现habse出现了RIT而且还是meta表的RIT，查看hadoop，出现了hdfs  missing blocks的问题，hadoop master上一上来就能看见报该问题。
```
There are 88 missing blocks. The following files may be corrupted:
blk_1076662996	/**/default/usertable/c31c955d868aa590ee08776c4e47f648/cf/e04822ace0e348e38d09b55e5124cfdc
blk_1076663064	/**/default/usertable/c31c955d868aa590ee08776c4e47f648/cf/e04822ace0e348e38d09b55e5124cfdc
blk_1076743273	/**/default/********1029_201810/bdfb0c7d8651f9ba99eaec22dc42cb13/f/afc59021d518457b9152e4a1c057fce4
blk_1076663044	/**/default/usertable/c31c955d868aa590ee08776c4e47f648/cf/e04822ace0e348e38d09b55e5124cfdc
blk_1076743287	/**/default/*****1029_201810/bdfb0c7d8651f9ba99eaec22dc42cb13/f/afc59021d518457b9152e4a1c057fce4
blk_1076663096	/**/default/usertable/c31c955d868aa590ee08776c4e47f648/cf/e04822ace0e348e38d09b55e5124cfdc
blk_1076743312	/**/default/******1029_201810/bdfb0c7d8651f9ba99eaec22dc42cb13/f/afc59021d518457b9152e4a1c057fce4
blk_1073743276
```
在网上查看了一些博客，missing block的意思是hdfs上的块不存在了，并不是少于3份，很难恢复。但是， the "blocks missing" it means that those blocks are not available in the live data nodes of the cluster, please verify any data node is down/unhealthy and bring it back - might rectify the missing blocks.
解决方案：
1. 参考 [https://stackoverflow.com/questions/19205057/how-to-fix-corrupt-hdfs-files](https://stackoverflow.com/questions/19205057/how-to-fix-corrupt-hdfs-files)
2. 参考博客的一些做法  先用 hdfs fsck /  检查missing block，再用 hdfs fsck -delete + block 可以移除 missing block
这里要注意，如果datanode丢失的数据块过多，达到一定的比例，会使得hadoop进入安全模式，也就是除了读不能进行其他任何操作，从而hbaseRS也处于有问题的状态。如果进入安全模式要先解决安全模式的问题才能使用 hdfs fsck delete。
安全模式的问题可以参考下文第三个博客链接。有两种办法：
 1. hadoop dfsadmin -safemode leave命令，强制离开
 2. 修改dfs.safemode.threshold.pct，缺省是0.999，修改为一个比较小的值。
此外分享给小可爱们一个我自己写的蹩脚脚本用来删除MISSING BLOCK：
```
for (( i=0; i<100; i++ ))
do
    hdfs fsck / | grep 'MISSING' | head -n 1 | cut -d : -f 1 | xargs hdfs fsck -delete
done
```
效率不高，我自己用着够用了。可能有不对的地方，小可爱们水平高的发现问题自助修改哈。
参考博客：
[http://julyme.com/20180202/99.html](http://julyme.com/20180202/99.html)
[https://blog.csdn.net/slx_2011/article/details/19634473](https://blog.csdn.net/slx_2011/article/details/19634473)
[https://blog.csdn.net/hongweigg/article/details/7185328](https://blog.csdn.net/hongweigg/article/details/7185328)
