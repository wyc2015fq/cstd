# nutch 1.7 修改代码后如何编译发布，并集群采集攻略 - weixin_33985507的博客 - CSDN博客
2013年09月18日 16:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
nutch 1.3之后，分布式的可执行文件与单机可执行文件进行了分离
接上篇，[nutch 1.7 导入 eclipse](http://www.cnblogs.com/i80386/p/3324068.html)
本篇所要解决的问题：
nutch下载下来经过简单的配置即可进行采集，但有时候我们需要修改nutch的源码
（比如[不遵守机器人协议](http://www.cnblogs.com/i80386/p/3223027.html)，[比如我要保存网页的编码](http://www.cnblogs.com/i80386/p/3253035.html)），这个时候如何编译为可执行程序呢？
tar zxvf apache-nutch-1.7-src.tar.gz -C .
mv apache-nutch-1.7 apache-nutch-1.7-src
cp -rv libreback/lib/ apache-nutch-1.7-src/
查看job工作状态
http://10.0.0.5:50030/jobtracker.jsp
第一步
将apache-nutch-1.7-bin中的 plugins 拷贝到 apache-nutch-1.7-src 目录下
修改apache-nutch-1.7-src/conf/nutch-default.xml 
将plugin.folders 选项 将./src/plugin 该为 ./plugins
第二步：
ant
然后将 runtime/deploy/apache-nutch-1.7.job  scp 到 hadoop master节点的
hadoop-1.2.1的根目录下。
然后chmod 777 apache-nutch-1.7.job 更改权限为可执行
第三步；
将urls上传到dfs中
第四步 运行：
bin/hadoop jar apache-nutch-1.7.job org.apache.nutch.crawl.Crawl urls -dir crawl -depth 7 -topN 50
其他：
采集完后，查询采集统计：
bin/hadoop jar apache-nutch-1.7.job org.apache.nutch.crawl.CrawlDbReader crawl/crawldb  -stats -sort
会发现好多unfetched，原因是：
nutch-default.xml对generate的时候进行了，分数限制，只有大于0分的才会被generate。所以会有很多分数低的没被采集。
（将数据url dump 出来你会发现 unfetched的URL都是负分，而且负的挺大的）
考虑到注释掉：
      // consider only entries with a score superior to the threshold
      if (scoreThreshold != Float.NaN && sort < scoreThreshold) return;
最后还是觉得修改：
<property>
  <name>generate.min.score</name>
  <value>0</value>
  <description>Select only entries with a score larger than
  generate.min.score.</description>
</property>
value 为 -1
```
