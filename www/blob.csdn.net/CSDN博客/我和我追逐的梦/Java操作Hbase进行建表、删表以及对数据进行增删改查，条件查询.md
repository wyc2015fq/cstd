
# Java操作Hbase进行建表、删表以及对数据进行增删改查，条件查询 - 我和我追逐的梦~~~ - CSDN博客


2014年04月08日 13:02:31[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：990个人分类：[Hadoop																](https://blog.csdn.net/heyongluoyao8/article/category/1773173)


转载自: http://javacrazyer.iteye.com/blog/11868811、搭建环境
新建JAVA项目，添加的包有:
有关Hadoop的hadoop-core-0.20.204.0.jar
有关Hbase的hbase-0.90.4.jar、hbase-0.90.4-tests.jar以及Hbase资源包中lib目录下的所有jar包
2、主要程序
Java代码![收藏代码](http://javacrazyer.iteye.com/images/icon_star.png)
package com.wujintao.hbase.test;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.MasterNotRunningException;
import org.apache.hadoop.hbase.ZooKeeperConnectionException;
import org.apache.hadoop.hbase.client.Delete;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.HTablePool;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.ResultScanner;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.filter.Filter;
import org.apache.hadoop.hbase.filter.FilterList;
import org.apache.hadoop.hbase.filter.SingleColumnValueFilter;
import org.apache.hadoop.hbase.filter.CompareFilter.CompareOp;
import org.apache.hadoop.hbase.util.Bytes;
public class JinTaoTest {
public static Configuration configuration;
static {
configuration = HBaseConfiguration.create();
configuration.set("hbase.zookeeper.property.clientPort", "2181");
configuration.set("hbase.zookeeper.quorum", "192.168.1.100");
configuration.set("hbase.master", "192.168.1.100:600000");
}
public static void main(String[] args) {
// createTable("wujintao");
// insertData("wujintao");
// QueryAll("wujintao");
// QueryByCondition1("wujintao");
// QueryByCondition2("wujintao");
//QueryByCondition3("wujintao");
//deleteRow("wujintao","abcdef");
deleteByCondition("wujintao","abcdef");
}
/**
* 创建表
* @param tableName
*/
public static void createTable(String tableName) {
System.out.println("start create table ......");
try {
HBaseAdmin hBaseAdmin = new HBaseAdmin(configuration);
if (hBaseAdmin.tableExists(tableName)) {// 如果存在要创建的表，那么先删除，再创建
hBaseAdmin.disableTable(tableName);
hBaseAdmin.deleteTable(tableName);
System.out.println(tableName + " is exist,detele....");
}
HTableDescriptor tableDescriptor = new HTableDescriptor(tableName);
tableDescriptor.addFamily(new HColumnDescriptor("column1"));
tableDescriptor.addFamily(new HColumnDescriptor("column2"));
tableDescriptor.addFamily(new HColumnDescriptor("column3"));
hBaseAdmin.createTable(tableDescriptor);
} catch (MasterNotRunningException e) {
e.printStackTrace();
} catch (ZooKeeperConnectionException e) {
e.printStackTrace();
} catch (IOException e) {
e.printStackTrace();
}
System.out.println("end create table ......");
}
/**
* 插入数据
* @param tableName
*/
public static void insertData(String tableName) {
System.out.println("start insert data ......");
HTablePool pool = new HTablePool(configuration, 1000);
HTable table = (HTable) pool.getTable(tableName);
Put put = new Put("112233bbbcccc".getBytes());// 一个PUT代表一行数据，再NEW一个PUT表示第二行数据,每行一个唯一的ROWKEY，此处rowkey为put构造方法中传入的值
put.add("column1".getBytes(), null, "aaa".getBytes());// 本行数据的第一列
put.add("column2".getBytes(), null, "bbb".getBytes());// 本行数据的第三列
put.add("column3".getBytes(), null, "ccc".getBytes());// 本行数据的第三列
try {
table.put(put);
} catch (IOException e) {
e.printStackTrace();
}
System.out.println("end insert data ......");
}
/**
* 删除一张表
* @param tableName
*/
public static void dropTable(String tableName) {
try {
HBaseAdmin admin = new HBaseAdmin(configuration);
admin.disableTable(tableName);
admin.deleteTable(tableName);
} catch (MasterNotRunningException e) {
e.printStackTrace();
} catch (ZooKeeperConnectionException e) {
e.printStackTrace();
} catch (IOException e) {
e.printStackTrace();
}
}
/**
* 根据 rowkey删除一条记录
* @param tablename
* @param rowkey
*/
public static void deleteRow(String tablename, String rowkey)  {
try {
HTable table = new HTable(configuration, tablename);
List list = new ArrayList();
Delete d1 = new Delete(rowkey.getBytes());
list.add(d1);
table.delete(list);
System.out.println("删除行成功!");
} catch (IOException e) {
e.printStackTrace();
}

}
/**
* 组合条件删除
* @param tablename
* @param rowkey
*/
public static void deleteByCondition(String tablename, String rowkey)  {
//目前还没有发现有效的API能够实现 根据非rowkey的条件删除 这个功能能，还有清空表全部数据的API操作
}

/**
* 查询所有数据
* @param tableName
*/
public static void QueryAll(String tableName) {
HTablePool pool = new HTablePool(configuration, 1000);
HTable table = (HTable) pool.getTable(tableName);
try {
ResultScanner rs = table.getScanner(new Scan());
for (Result r : rs) {
System.out.println("获得到rowkey:" + new String(r.getRow()));
for (KeyValue keyValue : r.raw()) {
System.out.println("列：" + new String(keyValue.getFamily())
+ "====值:" + new String(keyValue.getValue()));
}
}
} catch (IOException e) {
e.printStackTrace();
}
}
/**
* 单条件查询,根据rowkey查询唯一一条记录
* @param tableName
*/
public static void QueryByCondition1(String tableName) {
HTablePool pool = new HTablePool(configuration, 1000);
HTable table = (HTable) pool.getTable(tableName);
try {
Get scan = new Get("abcdef".getBytes());// 根据rowkey查询
Result r = table.get(scan);
System.out.println("获得到rowkey:" + new String(r.getRow()));
for (KeyValue keyValue : r.raw()) {
System.out.println("列：" + new String(keyValue.getFamily())
+ "====值:" + new String(keyValue.getValue()));
}
} catch (IOException e) {
e.printStackTrace();
}
}
/**
* 单条件按查询，查询多条记录
* @param tableName
*/
public static void QueryByCondition2(String tableName) {
try {
HTablePool pool = new HTablePool(configuration, 1000);
HTable table = (HTable) pool.getTable(tableName);
Filter filter = new SingleColumnValueFilter(Bytes
.toBytes("column1"), null, CompareOp.EQUAL, Bytes
.toBytes("aaa")); // 当列column1的值为aaa时进行查询
Scan s = new Scan();
s.setFilter(filter);
ResultScanner rs = table.getScanner(s);
for (Result r : rs) {
System.out.println("获得到rowkey:" + new String(r.getRow()));
for (KeyValue keyValue : r.raw()) {
System.out.println("列：" + new String(keyValue.getFamily())
+ "====值:" + new String(keyValue.getValue()));
}
}
} catch (Exception e) {
e.printStackTrace();
}
}
/**
* 组合条件查询
* @param tableName
*/
public static void QueryByCondition3(String tableName) {
try {
HTablePool pool = new HTablePool(configuration, 1000);
HTable table = (HTable) pool.getTable(tableName);
List<Filter> filters = new ArrayList<Filter>();
Filter filter1 = new SingleColumnValueFilter(Bytes
.toBytes("column1"), null, CompareOp.EQUAL, Bytes
.toBytes("aaa"));
filters.add(filter1);
Filter filter2 = new SingleColumnValueFilter(Bytes
.toBytes("column2"), null, CompareOp.EQUAL, Bytes
.toBytes("bbb"));
filters.add(filter2);
Filter filter3 = new SingleColumnValueFilter(Bytes
.toBytes("column3"), null, CompareOp.EQUAL, Bytes
.toBytes("ccc"));
filters.add(filter3);
FilterList filterList1 = new FilterList(filters);
Scan scan = new Scan();
scan.setFilter(filterList1);
ResultScanner rs = table.getScanner(scan);
for (Result r : rs) {
System.out.println("获得到rowkey:" + new String(r.getRow()));
for (KeyValue keyValue : r.raw()) {
System.out.println("列：" + new String(keyValue.getFamily())
+ "====值:" + new String(keyValue.getValue()));
}
}
rs.close();
} catch (Exception e) {
e.printStackTrace();
}
}
}
注意：可能大家没看到更新数据的操作，其实更新的操作跟添加完全一致，只不过是添加呢rowkey不存在，更新呢rowkey已经存在，并且timstamp相同的情况下，还有就是目前好像还没办法实现hbase数据的分页查询，不知道有没有人知道怎么做
**HBase性能优化建议：**
针对前面的代码，有很多不足之处，在此我就不修改上面的代码了，只是提出建议的地方，大家自己加上
1)配置
当你调用create方法时将会加载两个配置文件:hbase-default.xml and hbase-site.xml,利用的是当前的java类路径， 代码中configuration设置的这些配置将会覆盖hbase-default.xml和hbase-site.xml中相同的配置,如果两个配置文件都存在并且都设置好了相应参上面的属性下面的属性即可
2)关于建表
public void createTable(HTableDescriptor desc)

HTableDescriptor 代表的是表的schema, 提供的方法中比较有用的有
setMaxFileSize，指定最大的region size
setMemStoreFlushSize 指定memstore flush到HDFS上的文件大小
增加family通过 addFamily方法
public void addFamily(final HColumnDescriptor family)

HColumnDescriptor代表的是column的schema，提供的方法比较常用的有
setTimeToLive:指定最大的TTL,单位是ms,过期数据会被自动删除。
setInMemory:指定是否放在内存中，对小表有用，可用于提高效率。默认关闭
setBloomFilter:指定是否使用BloomFilter,可提高随机查询效率。默认关闭
setCompressionType:设定数据压缩类型。默认无压缩。
setMaxVersions:指定数据最大保存的版本个数。默认为3。
注意的是，一般我们不去setInMemory为true,默认是关闭的
3)关于入库
官方建议
table.setAutoFlush(false); //数据入库之前先设置此项为false
table.setflushCommits();//入库完成后，手动刷入数据
注意：
在入库过程中，put.setWriteToWAL(true/flase);
关于这一项如果不希望大量数据在存储过程中丢失，建议设置为true,如果仅是在测试演练阶段，为了节省入库时间建议设置为false
4)关于获取表实例
HTablePool pool = new HTablePool(configuration, Integer.MAX_VALUE);
HTable table = (HTable) pool.getTable(tableName);
建议用表连接池的方式获取表，具体池有什么作用，我想用过数据库连接池的同学都知道，我就不再重复
不建议使用new HTable(configuration,tableName);的方式获取表
5)关于查询
建议每个查询语句都放入try catch语句块，并且finally中要进行关闭ResultScanner实例以及将不使用的表重新放入到HTablePool中的操作，具体做法如下
Java代码![收藏代码](http://javacrazyer.iteye.com/images/icon_star.png)
public static void QueryAll(String tableName) {
HTablePool pool = new HTablePool(configuration, Integer.MAX_VALUE);
HTable table = null;
ResultScanner rs = null;
try {
Scan scan = new Scan();
table = (HTable) pool.getTable(tableName);
rs = table.getScanner(scan);
for (Result r : rs) {
System.out.println("获得到rowkey:" + new String(r.getRow()));
for (KeyValue keyValue : r.raw()) {
System.out.println("列：" + new String(keyValue.getFamily())
+ "====值:" + new String(keyValue.getValue()));
}
}
} catch (IOException e) {
e.printStackTrace();
}finally{
rs.close();// 最后还得关闭
pool.putTable(table); //实际应用过程中，pool获取实例的方式应该抽取为单例模式的，不应在每个方法都重新获取一次(单例明白？就是抽取到专门获取pool的逻辑类中，具体逻辑为如果pool存在着直接使用，如果不存在则new)
}
}

所以，以上代码有缺陷的地方，感兴趣的同学可以针对优化建议作出相应修改

