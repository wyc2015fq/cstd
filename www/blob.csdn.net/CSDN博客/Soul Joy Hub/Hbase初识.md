# Hbase初识 - Soul Joy Hub - CSDN博客

2016年07月16日 23:11:07[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：761


# 模块

![这里写图片描述](https://img-blog.csdn.net/20161212120241208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
hmaster、hregionserver、zookeeper、hregion、root表、meta表、hfile、hstore、memstore、blockcache

- hmaster 
- 启动时HRegion的分配，以及负载均衡和修复时HRegion的重新分配。
- 监控集群中所有HRegionServer的状态(通过Heartbeat和监听ZooKeeper中的状态)。
- 创建、删除、修改Table的定义

- 
hregionserver

主要负责响应用户的I/O请求，向HDFS文件系统中读写数据。

- 
zookeeper

root表的父节点，记录root表的Lacation

- 
hregion

HBase使用RowKey将表水平切割成多个HRegion，从HMaster的角度，每个HRegion都纪录了它的StartKey和EndKey，由于RowKey是排序的，因而Client可以通过HMaster快速的定位每个RowKey在哪个HRegion中。

- 
root表

mata表的父节点，记录meta表的Region信息，只有一个Region

- 
meta表

UserTable的父节点，记录UserTable的Region信息，可以有多个region。

- 
hfile

是StoreFile的底层实现，对应LSM tree中的大树

- 
hstore

hstore是Hbase存储的核心，其中有两部分组成，一部分是MenStore，一部分是StoreFile

- 
memstore

Sorted Memory Buffer，用户写入的数据首先会放入menstore,对应LSM tree中的小树，当menstore满了以后会执行flush操作变成一个storefile

- 
blockcache

blockcache是一个读缓存，即“引用局部性”原理将数据预读取到内存中，以提升读的性能。

# 基本API

## DDL
- 生成conf -> 生成admin

```java
Configuration conf = HBaseConfiguration.create();
HBaseAdmin admin = new HBaseAdmin(conf);
```
- 生成tableName -> 生成tableDescriptor

```java
TableName tableName = TableName.valueOf("test");
HTableDescriptor tableDescriptor = new HTableDescriptor(tableName);
```
- 生成columnDescriptor（列族）-> 加入到tableDescriptor

```java
tableDescriptor.addFamily(columnDescriptor);
```
- tableDescriptor -> admin生成table

```java
admin.createTable(tableDescriptor);
```

## DML
- conf & tableName -> 生成 table ，用于 DML 和 DQL

```java
HTable table = new HTable(conf, tableName);
```
- 生成 row -> 生成put ，Hbase中传入传出数据库中的值用的都是byte[] , 要注意与String类型转换

```java
byte[] row = Bytes.toBytes("row1");
Put put = new Put(row);
```
- 生成 想添加的val 以及它所在 列族 和 列 -> 加入到 put

```java
byte[] colfam = Bytes.toBytes("data");
byte[] col = Bytes.toBytes(String.valueOf(1));
byte[] val = Bytes.toBytes("value1");
put.add(colfam, col, val);
```
- put -> table put

```java
table.put(put);
```

## DQL

### get
- 生成 row -> 生成get

```java
byte[] row = Bytes.toBytes("row1");
Get get = new Get(row);
```
- get -> table getResult 生成 result

```java
Result result = table.get(get);
```
- 生成 查询的 列族 和 列 -> result getValue

```java
byte[] colfam = Bytes.toBytes("data");
byte[] col = Bytes.toBytes(String.valueOf(1));
System.out.println("get value is " + Bytes.toString(result.getValue(colfam, col)));
```

### scan
- 生成scan -> table getScanner 生成 ResultScanner。

```java
Scan scan = new Scan();
ResultScanner scanner = table.getScanner(scan);
```
- ResultScanner可以理解为Result数组，遍历生成要查询的列族和列 -> result getValue

```java
int i = 0;
for (Result scanresult : scanner) {
byte[] scancol = Bytes.toBytes(String.valueOf(++i));
System.out.println("scan value is " + Bytes.toString(scanresult.getValue(colfam, scancol)));
}
```

***注意： HBaseAdmin,HTable,ResultScanner 对象最后都要close()***

## Example

```java
package ExampleClient;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.MasterNotRunningException;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.ZooKeeperConnectionException;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.ResultScanner;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.util.Bytes;

public class ExampleClient {

    public static void main(String[] args) throws MasterNotRunningException, ZooKeeperConnectionException, IOException {
        // TODO Auto-generated method stub
        Configuration conf = HBaseConfiguration.create();
        HBaseAdmin admin = new HBaseAdmin(conf);
        try {

            TableName tableName = TableName.valueOf("test");
            HTableDescriptor tableDescriptor = new HTableDescriptor(tableName);
            HColumnDescriptor columnDescriptor = new HColumnDescriptor("data");
            tableDescriptor.addFamily(columnDescriptor);
            admin.createTable(tableDescriptor);

            HTable table = new HTable(conf, tableName);
            try {
                for (int i = 1; i <= 3; ++i) {
                    byte[] row = Bytes.toBytes("row" + i);
                    Put put = new Put(row);
                    byte[] colfam = Bytes.toBytes("data");
                    byte[] col = Bytes.toBytes(String.valueOf(i));
                    byte[] val = Bytes.toBytes("value" + i);
                    put.add(colfam, col, val);
                    table.put(put);
                }
                byte[] row = Bytes.toBytes("row1");
                Get get = new Get(row);
                Result result = table.get(get);
                byte[] colfam = Bytes.toBytes("data");
                byte[] col = Bytes.toBytes(String.valueOf(1));
                System.out.println("get result is " + Bytes.toString(result.getValue(colfam, col)));

                Scan scan = new Scan();
                ResultScanner scanner = table.getScanner(scan);
                try {
                    int i = 0;
                    for (Result scanresult : scanner) {
                        byte[] scancol = Bytes.toBytes(String.valueOf(++i));
                        System.out.println("scan result is " + Bytes.toString(scanresult.getValue(colfam, scancol)));
                    }
                } finally {
                    // TODO: handle finally clause
                    scanner.close();
                }
            } finally {
                // TODO: handle finally clause
                table.close();
            }
        } finally {
            // TODO: handle finally clause
            admin.close();
        }
    }

}
```

# B+ tree与LSM tree

## B+树

根节点和枝节点分别记录每个叶子节点的最小值，并用一个指针指向叶子节点。

B+树对读友好。叶子节点里每个键值都指向真正的数据块，每个叶子节点都有前指针和后指针，这是为了做范围查询时，叶子节点间可以直接跳转。

B+树对写不友好。最大的性能问题是会产生大量的随机IO，随着新数据的插入，叶子节点会慢慢分裂，逻辑上连续的叶子节点在物理上往往不连续，甚至分离的很远，但做范围查询时，会产生大量读随机IO。

![这里写图片描述](https://img-blog.csdn.net/20161212120443267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

关系数据库中常用B+树组织数据。如上图所示，内部节点已经存满，再插入一个新记录时，需要在B+树中插入一个新的内部节点，再链到B+树中。这里的问题是新的内部节点在磁盘上可能存放在很远的地方，在顺序扫描数据时，不得不seek磁盘。

## LSM树

LSM树本质上就是在读写之间取得平衡，和B+树相比，它牺牲了部分读性能，用来大幅提高写性能。

它的原理是把一颗大树拆分成N棵小树， 它首先写入到内存中（内存没有寻道速度的问题，随机写的性能得到大幅提升），在内存中构建一颗有序小树，随着小树越来越大，内存的小树会flush到磁盘上。当读时，由于不知道数据在哪棵小树上，因此必须遍历所有的小树，但在每颗小树内部数据是有序的。

## Hbase中的LSM树
- insert/update写入log后，再写入内存（memory store）
- memory store写满后，flush到磁盘上
- 后台进程/线程对磁盘上多个文件进行合并，组成排序后的B+树，同时处理删除、更新、TTL等 

查询时先查内存中的数据，再查磁盘

![这里写图片描述](https://img-blog.csdn.net/20161212120742847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
WAL

因为数据是先写到内存中，如果断电，内存中的数据会丢失，因此为了保护内存中的数据，需要在磁盘上先记录logfile，当内存中的数据flush到磁盘上时，就可以抛弃相应的Logfile。

- 
memstore,          

LSM树就是一堆小树，在内存中的小树即memstore，每次flush，内存中的memstore变成磁盘上一个新的storefile。

- 
compact

随着小树越来越多，读的性能会越来越差，因此需要在适当的时候，对磁盘中的小树进行merge，多棵小树变成一颗大树。

