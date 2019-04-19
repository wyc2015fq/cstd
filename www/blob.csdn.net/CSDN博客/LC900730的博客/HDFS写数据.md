# HDFS写数据 - LC900730的博客 - CSDN博客
2017年11月15日 09:36:49[lc900730](https://me.csdn.net/LC900730)阅读数：407
### 上传流程
1.首先客户端向namenode请求上传文件  /aaa/cls.avi 
    元数据存储在内存中 
    namenode检查该路径是否有文件等，返回给客户端
2.响应，可以上传。rpc请求，请求上传一个block，请求返回datanode。 
3.返回datanode列表，(dn1/dn3/dn4..), 
     namenode给这些datanode的讲究是： 
     1.datanode空间大小(datanode定期会汇报，namenode知道datanode空间大小) 
     2.同一个机架上一般上一个路由器连接，比 跨2级的距离短 
     3.第二台是跨机架的 
     4.第三个副本就在第一个副本同机架另外一台datanode存放。
4.socket nio去传文件(先跟目标机器建立socket流，socket流要写入本地流) 
先建立一台如dn1，发起建立连接请求 
dn1会向dn3发起请求，建立连接 
dn3向dn4发起请求，建立连接。dn4应答成功，然后dn3应答成功，然后dn2应答客户。 
通道成功流，客户就开始写，pipeline(管道流水线)。 
5.客户端文件channel写入socket channel，写入dn1(一个一个packet写入，一个数据包64k，写的时候校验)。写入dn1缓冲，缓冲可以写入dn1文件，也可以往dn3的socket写 
只要上传一个成功，客户端就认为成功。（namenode会异步复制）
### 下载流程
1.客户端请求下载文件 /aa/cis.avi 
namenode存储的元数据信息， 
如/aa/cls.avi (BLK_1 BLK_2 BLK_3) (BLK_1:DN1,DN3,DN4  BLK_2:DN1,DN4,DN5  ) 
2.namenode返回文件元数据 
3.请求读取数据BLK_1 
4.客户端与DN1建立流，DN1文件流写入socket流，写入客户端socketInputStream流，写入客户端文件流 
5.然后读取BLK_2(如与DN4建立通道，追加到文件流)；
### NAMENODE元数据管理机制
namenode元数据 
客户端查询元数据请求，更新元数据请求等操作 
放入内存，还会做持久化(fsimage) 
刷新机制： 
secondary namenode 
首先更新内存，然后记录到操作日志edits(追加)。原来/aa/cls.avi，修改之后在内存中好改，但是在edits文件中修改就会很慢。
//创建文件  /aa/cls 
//创建文件  /aa/a.txt
//修改 /aa/cls   /aa/good.txt 
记录操作的历史记录，这样重放日志 
即使挂流，那么老的数据在fsimage，新的在edits。
定期合并然后替换掉secondary namenode。请求namenode是否满足条件 
checkpoint，触发条件： 1.定时；2.edits中的记录信息 
条件满足就会请求checkpoint 
namenode中edits_inprogress 
secondaryname会合并已经产生的，可以触发日志立马滚动一次，多合并一些。
secondary namenode下载这些edits文件，然后加载到内存运算一遍，合并更新后的生成fsimage.chkpoint上传。重命名为fsimage，老的直接覆盖。
namenode工作目录被删除了，启动时候会启动不起来。查看日志发现文件夹不存在。 
namenode与name文件内容是一致的，因此可以直接复制namenode文件夹给name。
namenode中工作目录参数时，有什么注意点。namenode工作目录放在多个磁盘中。 
修改配置文件：hdfs-site.xml中
```xml
<property>
<name>dfs.name.dir</name>
<value>/home/hadoop/name1,/home/hadoop/name2</value>
//edits文件同时往这2个写
</property>
```
