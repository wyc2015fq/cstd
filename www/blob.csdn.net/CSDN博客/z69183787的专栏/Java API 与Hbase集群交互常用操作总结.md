# Java API 与Hbase集群交互常用操作总结 - z69183787的专栏 - CSDN博客
2016年10月22日 21:38:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2755
HBase 与 [Hadoop](http://lib.csdn.net/base/hadoop) 一样，都是用 [Java](http://lib.csdn.net/base/javaee) 编写的，所以
 HBase 对 java 支持是必需的，下面看看怎么使用java 语言对 HBase 进行操作。[Java](http://lib.csdn.net/base/java)API
 核心类总结如下： 
1、 HBaseConfiguration 类 
　　 HBaseConfiguration 是每一个 HBase Client 都会使用到的对象，它代表 HBase 配置信息。有两种构造方式。
```java
public HBaseConfiguration()
public HBaseConfiguration(final Configuration c)
```
　默认构造方式会尝试从 hbase-default.xml 和 HBase-site.xml 文件中读取配置。如果CLASSPATH 没有这两个文件，就需要自己配置。
```java
Configuration HBASE_CONFIG = new Configuration();
HBASE_CONFIG.set("hbase.ZooKeeper.quorum","zkServer");  //hbase 服务地址
HBASE_CONFIG.set("hbase.ZooKeeper.property.clientPort","2181"); //端口号
HBaseConfiguration cfg = new HBaseConfiguration(HBASE_CONFIG);  //读取配置文件
```
2、 创建表
　　创建表通过 HBaseAdmin 对象操作。HBaseAdmin 负责META 表信息的处理。HBaseAdmin 提供了 createTable 方法。
```java
public void createTable(HTableDescriptor desc)
```
HTableDescriptor 表示表的 Schema，提供常用方法有以下两个。
```
1)setMaxFileSize：指定最大 Region 的大小。
    2)setMemStoreFlushSize：指定 MemStore Flush 到 HDFS 上的文件大小。
```
3、 增加 Family
使用 addFamily 方法实现 Family 的添加。
```java
public void addFamily(final HColumnDescriptor family)
```
HColumnDescriptor 代表 Column 的 Schema，提供的常用方法有以下几个。
```
1、setTimeToLive：指定最大的 TTL（单位是 ms），过期数据会被自动删除。
    2、setInMemory：指定是否放在内存中，对小表有用，可用于提高效率。默认关闭。
    3、setBloomFilter：指定是否使用 BloomFilter，可提高随机查询效率。默认关闭。
    4、setCompressionType：设定数据压缩类型。默认无压缩。
    5、setMaxVersions：指定数据最大保存的版本个数。默认为3。
```
举个简单的例子，创建
 4 个 Family 表，命令如下。
```java
HBaseAdmin hAdmin = new HBaseAdmin(hbaseConfig);
HTableDescriptor table = new HTableDescriptor(tableName);
table.addFamily(new HColumnDescriptor("f1"));
table.addFamily(new HColumnDescriptor("f2"));
table.addFamily(new HColumnDescriptor("f3"));
table.addFamily(new HColumnDescriptor("f4"));
hAdmin.createTable(table);
```
4、 删除表
　　删除表也是通过 HBaseAdmin 来操作，删除表之前首先要 disable 表。这是一个非常耗时的操作，所以不建议频繁删除表。
disableTable 和 deleteTable 分别用来执行 disable 和 delete 操作。使用方法如下。
```java
HBaseAdmin hAdmin = new HBaseAdmin(hbaseConfig);
if(hAdmin.tableExists(tableName)){
    hAdmin.disableTable(tableName);
    hAdmin.deleteTable(tableName);
}
```
5、查询数据
　　查询分为单条随机查询和批量查询。单条查询通过 Row Key 在Table 中查询某一行的数据，HTable 提供了get 方法完成单条查询。批量查询通过制定一段 Row Key 的范围来查询，HTable 提供了 getScanner 方法完成批量查询。
```java
public Result get(final Get get)
public ResultScanner getScanner(final Scan scan)
```
Get 对象包含一个 Get 查询需要的信息，它的构造方法有两种。
```java
public Get(byte [] row)
public Get(byte [] row，RowLock rowLock)
```
Row Lock 为了保证读写的原子性，可以传递一个已经存在 Row Lock，否则 HBase 会自动生成一个新的 Row Lock。
　　Scan 对象提供了默认构造函数，一般使用默认构造函数。
1) Get 和 Scan 的常用方法有以下几个。
```
addFamily/addColumn：指定需要的 Family 或者 Column，如果没有调用任何 Family 或者 Column，会返回所有的 Column。
    setMaxVersions：指定最大的版本个数。如果不带任何参数调用 setMaxVersions，表示取所有的版本。如果不调用 setMaxVersions，只会取到最新的版本。
    setTimeRange：指定最大的时间戳和最小的时间戳，只有在此范围内的 Cell 才能被获取。
    setTimeStamp：指定时间戳。
    setFilter：指定 Filter 过滤不需要的信息。
```
2) Scan 特有的方法如下。
```
setStartRow：指定开始的行。如果不调用，从表头开始。
    setStopRow：指定结束的行（不含此行）。
    setBatch：指定最多返回的 Cell 数目。防止一行中有过多的数据，导致 OOM 错误。
```
3)
 Result 代表是一行的数据。常用方法有以下几个。
```
getRow：返回 Row Key。
    raw：返回所有的 KeyValue 数组。
    getValue：按照 Column 来获取 Cell 的值。
    ResultScanner 是 Result 的一个容器，每次调用ResultScanner 的next 方法会返回Result。
```
```java
public Result next() throws IOException;
public Result [] next(int nbRows) throws IOException;
```
示例代码如下：
```java
Scan scan = new Scan();
scan.setMaxVersions();
ResultScanner ss = table.getScanner(scan);
for(Result r:ss){
    System.out.println(new String(r.getRow()));
    for(KeyValue kv:r.raw){
        System.out.println(new String(kv.getColumn()));
    }
}
```
6、 插入数据
　　HTable 通过 put 方法插入数据，可以传递单个 put 对象 或 List put 对象分别实现单条插入和批量插入。
```java
public void put(final Put put) throws IOException
public void put(final List< Put> puts) throws IOException
```
Put 提供3 种构造方式。
```java
public Put(byte [] row)
public Put(byte [] row)
public Put(byte [] row,RowLock rowLock)
public Put(Put putToCopy)
```
Put
 常用的方法有以下几个。
```
1)add：增加一个 Cell。
    2)setTimeStamp：指定所有 Cell 默认的 timestamp，如果一个 Cell 没有指定 timestamp，就会用到这个值。如果没有调用，HBase 会将当前时间作为未指定 timestamp 的Cell 的 timestamp。
    3)setWriteToWAL：WAL 是 Write Ahead Log 的缩写，指的是 HBase 在插入操作前是否写 Log。默认是打开，关掉会提高性能，但是如果系统出现故障（负责插入的Region Server 挂掉），数据可能会丢失。
    另外 HTable 也有两个方法会影响插入的性能。
    1)setAutoFlash：AutoFlush 指的是在每次调用 HBase 的 Put 操作，是否提交到 HBase Server。默认是 true，每次会提交。如果此时是单条插入，就会有更多的I/O，从而降低其性能。
    2)setWriteBufferSize：Write Buffer Size 在 AutoFlush 为false 的时候起作用，默认是 2MB，也就是插入数据超过 2MB，就会自动提交到 Server。
```
示例代码如下：
```java
HTable table = new HTable(hbaseConfig, tableName);
table.setAutoFlush(autoFlush);
List< Put> lp = new ArrayList< Put>();
int count = 10000;
byte[] buffer = new byte[1024];
Random r = new Random();
for(int i = 1;i <= count;++i){
    Put p = new Put(String.format("row%09d",i).getBytes());
    r.nextBytes(buffer);
    p.add("f1".getBytes(), null, buffer);
    p.add("f2".getBytes(), null, buffer);
    p.add("f3".getBytes(), null, buffer);
    p.add("f4".getBytes(), null, buffer);
    p.setWriteToWAL(wal);
    lp.add(p);
    if(i%1000==0){
        table.put(lp);
        lp.clear();
    }
}
```
7、 删除数据
HTable 通过 delete 方法删除数据。
```java
public void delete(final Delete delete)
```
Delete 构造方法如下：
```java
public Delete(byte [] row)
public Delete(byte [] row, long timestamp, RowLock rowLock)
public Delete(final Delete d)
```
 Delete 常用方法有 deleteFamily/deleteColumn，用来指定要删除的 Family 或者 Column 的数据。 如果不调用任何这样的方法，将会删除整行。 
　 注意： 如果某个 Cell 的 timestamp 高于当前时间，这个 Cell 将不会被删除，仍然可以查出来。
示例代码如下：
```java
HTable table = new HTable(hbaseConfig,"mytest");
Delete d = new Delete("row1".getBytes());
table.delete(d)
```
8、 切分表
HBaseAdmin 提供 split 方法将 table 进行切分。
```java
public void split(final String tableNameOrRegionName)
```
如果提供的是 tableName，会将 table 所有 Region 进行切分；如果提供的是 RegionName，只会切分这个Region。Split 是一个异步操作，因此它并不能确切控制 Region 的个数。
示例代码如下：
```java
public void split(String tableName,int number,int timeout) throws Exception{
    Configuration HBASE_CONFIG = new Configuration();
    HBASE_CONFIG.set("hbase.ZooKeeper.quorum",GlobalConf.ZOOKEEPER_QUORUM);
    HBaseConfiguration cfg = new HBaseConfiguration(HBASE_CONFIG);
    HBaseAdmin hAdmin = new HBaseAdmin(cfg);
    HTable hTable = new HTable(cfg,tableName);
    int oldsize = 0;
    long time = System.currentTimeMillis();
    while(true){
        int size = hTable.getRegionsInfo().size();
        logger.info("the region number="+size);
        if(size>=number) break;
        if(size !=oldsize){
            hAdmin.split(hTable.getTableName());
            oldsize = size;
        }else if(System.currentTimeMillis()-time>timeout){
            break;
        }
        Thread.sleep(1000*10);
    }
}
```
