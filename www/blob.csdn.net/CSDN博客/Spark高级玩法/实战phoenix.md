# 实战phoenix - Spark高级玩法 - CSDN博客
2017年11月25日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：303

![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXR9gDaCNpJmopRlb7jViaYLxufcb0KCytxRWKJSFjA3dxN3AUWiaZibJ9MZhGrgQ62FnJbEcnetDicTg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**一 安装部署**
**1， 下载**
http://archive.apache.org/dist/phoenix/
本文下载的是apache-phoenix-4.12.0-HBase-1.2-bin.tar.gz
**2， 安装**
解压之后将phoenix-4.12.0-HBase-1.2-server.jar复制到hbase/lib目录下。
在hbase-site.xml中，添加如下配置
<property>
<name>hbase.regionserver.wal.codec</name>
<value>org.apache.hadoop.hbase.regionserver.wal.IndexedWALEditCodec</value>
</property>
<property>
<name>hbase.coprocessor.master.classes</name>
<value>org.apache.phoenix.hbase.index.master.IndexMasterObserver</value>
</property>
<property>
<name>hbase.rpc.timeout</name>
<value>300000</value>
</property>
<property>
<name>hbase.region.server.rpc.scheduler.factory.class</name>
<value>org.apache.hadoop.hbase.ipc.PhoenixRpcSchedulerFactory</value>
<description>Factory to create the Phoenix RPC Scheduler that usesseparate queues for index and metadata updates</description>
</property>
<property>
<name>hbase.rpc.controllerfactory.class</name>
<value>org.apache.hadoop.hbase.ipc.controller.ServerRpcControllerFactory</value>
<description>Factory to create the Phoenix RPCScheduler that uses separate queues for index and metadataupdates</description>
</property>
重启hbase
关于hadoop及hbase的安装部署，请参考：
[Hadoop伪分布式集群安装部署](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484091&idx=1&sn=00d93b91642a6aaa103de2731066410f&chksm=9f38e193a84f68858e1d906967f0b7b3221320d82fa95905b9a854ebe39fc96837820a42bc22&scene=21#wechat_redirect)
[HBase的安装部署](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484117&idx=2&sn=b5cbd15e1259d5727f7c36ee83c60989&chksm=9f38e1fda84f68ebf4a0e52b888dc7b8885d1c899426067a6fa4b2ca3b903b1594d566efab2f&scene=21#wechat_redirect)
**3， 测试**
测试的方法有很多，本文采用更加简单的测试方法。
在phoenix的bin目录下执行: sqlline.py [zookeeper]
./sqlline.py localhost:2181
创建表，并插入两条数据：
create table test (mykey integer not null primary key, mycolumn varchar);
upsert into test values (1,'Hello');
upsert into test values (2,'World!');
select * from test;
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXR9gDaCNpJmopRlb7jViaYLmibUQTT9Ga77ibibkbib7MYNzYZ5rlxMmibiaozI6MwSjDnpSEiahQpGEyTdg/0?wx_fmt=png)
**二 通过java的jdbc**
创建表test1，并插入数据，然后查询
在这之前，要先将依赖：phoenix-4.12.0-HBase-1.2-client.jar加到工程的依赖里面
具体代码如下：
import
java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
importjava.sql.PreparedStatement;
import java.sql.Statement;
/**
* Created by spark on 2017/11/25.
*/
public class PhoenixTest {
public static void
main(String[] args)
throws SQLException {
       Statement stmt =
null;
ResultSet rset =
null;
Connection con = DriverManager.getConnection("jdbc:phoenix:192.168.1.103:2181");
stmt = con.createStatement();
stmt.executeUpdate("create table test1 (mykey integer
 not null primary key, mycolumn varchar)");
stmt.executeUpdate("upsert into test1 values (1,'Hello')");
stmt.executeUpdate("upsert into test1 values (2,'World!')");
con.commit();
PreparedStatement statement
 = con.prepareStatement("select * from test1");
rset = statement.executeQuery();
       while (rset.next()) {
           System.out.println(rset.getString("mycolumn"));
}
       statement.close();
con.close();
}
}
输出结果如下：
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXR9gDaCNpJmopRlb7jViaYLsmic72zoZibhk3OiczSkaCtmvvqiaCjBHG5hWqh4RaZns8EQIsJ90QA9bw/0?wx_fmt=png)
**三 映射hbase表**
针对预先存在的HBase表，可以通过CREATE TABLE / CREATE VIEW DDL语句创建一个Phoenix表或视图。在这两种情况下，我们都会保留HBase元数据。对于CREATE TABLE，我们将创建任何不存在的元数据（表格，列族）。我们还将为每行添加一个空的键值，以便查询按预期工作（不需要在扫描期间映射所有列）。
Rowkey是通过使用将值简单拼接形成的，其中在变长类型后使用一个零字节作为分隔符。
**映射hbase表到phoenix：**
比如，在hbase上创建表t1,然后映射到phoenix上，分两步：
**1， 创建hbase表**
create 't1', {NAME => 'f1', VERSIONS => 5}
**2， 创建phoenix视图**
CREATE VIEW "t1" ( pk VARCHAR PRIMARY KEY, "f1".val VARCHAR )
Pk列声明你的Rowkey是字符串类型。而“f1”.val列声明你的HBase表将包含KeyValues列簇和列限定符“f1”：VAL和他们的值将是一个VARCHAR。
**请注意**，如果使用全部大写名称创建HBase表，则不需要双引号（因为这是Phoenix对字符串进行规范化的方法，通过上面的方式对它们进行标准化）。例如，用：
创建'T1'，{NAME =>'F1'，VERSIONS => 5}
你可以创建这个phoenix图：
CREATE VIEW t1（pk VARCHAR PRIMARY KEY，f1.val VARCHAR）
**Hbase表中插入一条数据，phoenix表中查询**
put 't1','1001','f1:VAL','lisi'
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXR9gDaCNpJmopRlb7jViaYLl11Ut1FeDQ8ySibKgD2r9ibEQMNGKzmgPIkb5wmgWujIKV8qTIxZ2naQ/0?wx_fmt=png)
**同时创建hbase和phoenix表：**
也可以直接使用cteate table同时创建phoenix表和hbase表
CREATE TABLE t1（pk VARCHAR PRIMARY KEY，val VARCHAR）
**推荐阅读：**
1，[HBase原理和设计](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484241&idx=1&sn=2f4d59fe83d74a1bc764230a574972dc&chksm=9f38e079a84f696f32c2b823024164ea2c83d4775d24f02ca1d3b3ac6731a1172688001e9666&scene=21#wechat_redirect)
2，[干货:Sqoop导入导出数据练习](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484190&idx=1&sn=943cc19ff5c3be877253b7076884ab46&chksm=9f38e036a84f6920533d742458d94fc9ca0ad2ee864ca0d7024fe79404022fe8a455bc83075b&scene=21#wechat_redirect)
3，[kafka连接器两种部署模式详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484229&idx=1&sn=d5e87da8a05c40b617f343044ba6d808&chksm=9f38e06da84f697b5181c1aedbeb9205e3af0a81dde6d19487cbde803e01d86fb9367b3a3b3b&scene=21#wechat_redirect)
4，[重磅：如何玩转kylin](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484128&idx=1&sn=e0a7328f4b1337bcc68e694b72758d3b&chksm=9f38e1c8a84f68deac7c3d9d6804f5a0f9e3f90f1361e93334920a9b85785b2776908634aacb&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**
