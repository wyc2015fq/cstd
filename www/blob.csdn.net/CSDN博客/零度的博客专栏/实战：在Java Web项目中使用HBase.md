# 实战：在Java Web项目中使用HBase - 零度的博客专栏 - CSDN博客
2017年02月10日 13:44:40[零度anngle](https://me.csdn.net/zmx729618)阅读数：3085标签：[mysql																[hbase																[java web																[数据																[迁移](https://so.csdn.net/so/search/s.do?q=迁移&t=blog)](https://so.csdn.net/so/search/s.do?q=数据&t=blog)](https://so.csdn.net/so/search/s.do?q=java web&t=blog)](https://so.csdn.net/so/search/s.do?q=hbase&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[HBase](https://blog.csdn.net/zmx729618/article/category/6452147)
在此之前我们使用[MySQL](http://lib.csdn.net/base/14)作为数据源，但发现这数据增长速度太快，并且由于种种原因，因此必须使用HBase，所以我们要把Mysql表里面的数据迁移到HBase中，在这里我就不讲解、不争论为什么要使用HBase，HBase是什么了，喜欢的就认真看下去，总有些地方是有用的。
我们要做的3大步骤：
- 
新建HBase表格。
- 
把MYSQL数据迁移到HBase中。
- 
在[Java Web](http://lib.csdn.net/base/13)项目中读取HBase的数据。
先介绍一下必要的一些环境：HBase的版本：0.98.8-hadoop2
所需的依赖包：
**[html]**[view plain](http://blog.csdn.net/z69183787/article/details/52909819#)[copy](http://blog.csdn.net/z69183787/article/details/52909819#)[print](http://blog.csdn.net/z69183787/article/details/52909819#)[?](http://blog.csdn.net/z69183787/article/details/52909819#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1945578/fork)
- commons-codec-1.7.jar  
- commons-collections-3.2.1.jar  
- commons-configuration-1.6.jar  
- commons-lang-2.6.jar  
- commons-logging-1.1.3.jar  
- guava-12.0.1.jar  
- hadoop-auth-2.5.0.jar  
- hadoop-common-2.5.0.jar  
- hbase-client-0.98.8-hadoop2.jar  
- hbase-common-0.98.8-hadoop2.jar  
- hbase-protocol-0.98.8-hadoop2.jar  
- htrace-core-2.04.jar  
- jackson-core-asl-1.9.13.jar  
- jackson-mapper-asl-1.9.13.jar  
- log4j-1.2.17.jar  
- mysql-connector-java-5.1.7-bin.jar  
- netty-3.6.6.Final.jar  
- protobuf-java-2.5.0.jar  
- slf4j-api-1.7.5.jar  
- slf4j-log4j12-1.7.5.jar  
- zookeeper-3.4.6.jar  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
commons-codec-1.7.jar
commons-collections-3.2.1.jar
commons-configuration-1.6.jar
commons-lang-2.6.jar
commons-logging-1.1.3.jar
guava-12.0.1.jar
hadoop-auth-2.5.0.jar
hadoop-common-2.5.0.jar
hbase-client-0.98.8-hadoop2.jar
hbase-common-0.98.8-hadoop2.jar
hbase-protocol-0.98.8-hadoop2.jar
htrace-core-2.04.jar
jackson-core-asl-1.9.13.jar
jackson-mapper-asl-1.9.13.jar
log4j-1.2.17.jar
mysql-connector-java-5.1.7-bin.jar
netty-3.6.6.Final.jar
protobuf-java-2.5.0.jar
slf4j-api-1.7.5.jar
slf4j-log4j12-1.7.5.jar
zookeeper-3.4.6.jar
```
如果在你的web项目中有些包已经存在，保留其中一个就好了，免得报奇怪的错误就麻烦了。
步骤1：建表
在此之前，我在Mysql中的业务数据表一共有6个，其结构重复性太高了，首先看看我在HBase里面的表结构：
|表名|kpi||||||||||||||||
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|key|fid+tid+date||||||||||||||||
|簇(family)|base|gpower|userate|consum|time||||||||||||
||||||||||||||||||
|描述|基础信息|发电量相关指标|可利用率|自耗电量|累计运行小时数|检修小时数|利用小时数||||||||||
|列(qualifier)|fid|tid|date|power|windspeed|unpower|theory|coup|time|power|num|cpower|gpower|runtime|checktime|usetime|
|描述|风场ID|风机号|日期|发电量|风速|弃风电量|理论电量|耦合度|故障时间|故障损失电量|故障台次|当天自耗电量|当天发电量|当天并网秒数|当天检修秒数|当天利用秒数|
这个表中我们有5个family，其中base Family是对应6个mysql表中的key列，gpower、userate、consum分别对应一个表，time对应3个表。
这个kpi表的rowkey设计是base中的3个qualifier，分别从3个维度查询数据，这样的设计已经可以满足我们的需求了。
具体在HBase中如何建表如何搭建环境自己参考我写的【手把手教你配置HBase完全分布式环境】这篇文章吧。
步骤2：把MySQL数据迁移到HBase
这时我用gpower对应的mysql表来做演示吧，其他表的道理都一样。（这里可能有人会说为什么不用第三方插件直接数据库对数据库迁移，这里我统一回答一下，我不会，我也不需要。）
okay,首先我们来看看代码先吧：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52909819#)[copy](http://blog.csdn.net/z69183787/article/details/52909819#)[print](http://blog.csdn.net/z69183787/article/details/52909819#)[?](http://blog.csdn.net/z69183787/article/details/52909819#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1945578/fork)
- import java.io.File;  
- import java.sql.Connection;  
- import java.sql.DriverManager;  
- import java.sql.PreparedStatement;  
- import java.sql.ResultSet;  
- import java.text.SimpleDateFormat;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.apache.hadoop.conf.Configuration;  
- import org.apache.hadoop.hbase.HBaseConfiguration;  
- import org.apache.hadoop.hbase.client.HBaseAdmin;  
- import org.apache.hadoop.hbase.client.HTable;  
- import org.apache.hadoop.hbase.client.Put;  
- import org.apache.hadoop.hbase.util.Bytes;  
- import org.apache.log4j.BasicConfigurator;  
- import org.apache.log4j.Level;  
- import org.apache.log4j.Logger;  
- 
- publicclass GpowerTransfer{  
- 
- privatestaticfinal String QUOREM = "192.168.103.50,192.168.103.51,192.168.103.52,192.168.103.53,192.168.103.54,192.168.103.55,192.168.103.56,192.168.103.57,192.168.103.58,192.168.103.59,192.168.103.60";//这里是你HBase的分布式集群结点，用逗号分开。
- privatestaticfinal String CLIENT_PORT = "2181";//端口
- privatestatic Logger log = Logger.getLogger(GpowerTransfer.class);  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         BasicConfigurator.configure();  
-         log.setLevel(Level.DEBUG);  
-         String tableName = "kpi";//HBase表名称
-         Configuration conf = HBaseConfiguration.create();  
-         conf.set("hbase.zookeeper.quorum", QUOREM);     
-         conf.set("hbase.zookeeper.property.clientPort", CLIENT_PORT);  
- try { File workaround = new File(".");  
-             System.getProperties().put("hadoop.home.dir", workaround.getAbsolutePath());  
- new File("./bin").mkdirs();  
- new File("./bin/winutils.exe").createNewFile();//这几段奇怪的代码在windows跑的时候不加有时候分报错，在web项目中可以不要,但单独的java程序还是加上去吧，知道什么原因的小伙伴可以告诉我一下，不胜感激。
-             HBaseAdmin admin = new HBaseAdmin(conf);  
- if(admin.tableExists(tableName)){  
-                 Class.forName("com.mysql.jdbc.Driver");//首先将mysql中的数据读取出来，然后再插入到HBase中
-                 String url = "jdbc:mysql://192.168.***.***:3306/midb?useUnicode=true&characterEncoding=utf-8";  
-                 String username = "********";  
-                 String password = "********";  
-                 Connection con = DriverManager.getConnection(url, username, password);  
-                 PreparedStatement pstmt = con.prepareStatement("select * from kpi_gpower");  
-                 ResultSet rs = pstmt.executeQuery();  
-                 HTable table = new HTable(conf, tableName);  
-                 log.debug(tableName + ":start copying data to hbase...");  
-                 List<Put> list = new ArrayList<Put>();  
-                 SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");  
-                 String base = "base";//family名称
-                 String gpower = "gpower";//family名称
-                 String[] qbase = {"fid","tid","date"};//qualifier名称
-                 String[] qgpower = {"power","windspeed","unpower","theory","coup"};//qualifier名称
- while(rs.next()){  
-                     String rowKey = rs.getString("farmid") + ":" + (rs.getInt("turbineid")<10?("0"+rs.getInt("turbineid")):rs.getInt("turbineid")) + ":" + sdf.format(rs.getDate("vtime"));//拼接rowkey
-                     Put put = new Put(Bytes.toBytes(rowKey));//新建一条记录，然后下面对相应的列进行赋值
-                     put.add(base.getBytes(), qbase[0].getBytes(), Bytes.toBytes(rs.getString("farmid")));//base:fid
-                     put.add(base.getBytes(), qbase[1].getBytes(), Bytes.toBytes(rs.getInt("turbineid")+""));//base:tid
-                     put.add(base.getBytes(), qbase[2].getBytes(), Bytes.toBytes(rs.getDate("vtime")+""));//base:date
-                     put.add(gpower.getBytes(), qgpower[0].getBytes(), Bytes.toBytes(rs.getFloat("value")+""));//gpower:power
-                     put.add(gpower.getBytes(), qgpower[1].getBytes(), Bytes.toBytes(rs.getFloat("windspeed")+""));//gpower:windspeed
-                     put.add(gpower.getBytes(), qgpower[2].getBytes(), Bytes.toBytes(rs.getFloat("unvalue")+""));//gpower:unvalue
-                     put.add(gpower.getBytes(), qgpower[3].getBytes(), Bytes.toBytes(rs.getFloat("theory")+""));//gpower:theory
-                     put.add(gpower.getBytes(), qgpower[4].getBytes(), Bytes.toBytes(rs.getFloat("coup")+""));//gpower:coup
-                     list.add(put);  
-                 }  
-                 table.put(list);//这里真正对表进行插入操作
-                 log.debug(tableName + ":completed data copy!");  
-                 table.close();//这里要非常注意一点，如果你频繁地对表进行打开跟关闭，性能将会直线下降，可能跟集群有关系。
-             }else{  
-                 admin.close();  
-                 log.error("table '" + tableName + "' not exisit!");  
- thrownew IllegalArgumentException("table '" + tableName + "' not exisit!");  
-             }  
-             admin.close();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }   
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;
public class GpowerTransfer{
	
	private static final String QUOREM = "192.168.103.50,192.168.103.51,192.168.103.52,192.168.103.53,192.168.103.54,192.168.103.55,192.168.103.56,192.168.103.57,192.168.103.58,192.168.103.59,192.168.103.60";//这里是你HBase的分布式集群结点，用逗号分开。
	private static final String CLIENT_PORT = "2181";//端口
	private static Logger log = Logger.getLogger(GpowerTransfer.class);
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		BasicConfigurator.configure();
		log.setLevel(Level.DEBUG);
		String tableName = "kpi";//HBase表名称
		Configuration conf = HBaseConfiguration.create();
        conf.set("hbase.zookeeper.quorum", QUOREM);   
        conf.set("hbase.zookeeper.property.clientPort", CLIENT_PORT);
        try { File workaround = new File(".");
            System.getProperties().put("hadoop.home.dir", workaround.getAbsolutePath());
            new File("./bin").mkdirs();
            new File("./bin/winutils.exe").createNewFile();//这几段奇怪的代码在windows跑的时候不加有时候分报错，在web项目中可以不要,但单独的java程序还是加上去吧，知道什么原因的小伙伴可以告诉我一下，不胜感激。
			HBaseAdmin admin = new HBaseAdmin(conf);
			if(admin.tableExists(tableName)){
				Class.forName("com.mysql.jdbc.Driver");//首先将mysql中的数据读取出来，然后再插入到HBase中
				String url = "jdbc:mysql://192.168.***.***:3306/midb?useUnicode=true&characterEncoding=utf-8";
				String username = "********";
				String password = "********";
				Connection con = DriverManager.getConnection(url, username, password);
				PreparedStatement pstmt = con.prepareStatement("select * from kpi_gpower");
				ResultSet rs = pstmt.executeQuery();
				HTable table = new HTable(conf, tableName);
				log.debug(tableName + ":start copying data to hbase...");
				List<Put> list = new ArrayList<Put>();
				SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
				String base = "base";//family名称
				String gpower = "gpower";//family名称
				String[] qbase = {"fid","tid","date"};//qualifier名称
				String[] qgpower = {"power","windspeed","unpower","theory","coup"};//qualifier名称
				while(rs.next()){
				    String rowKey = rs.getString("farmid") + ":" + (rs.getInt("turbineid")<10?("0"+rs.getInt("turbineid")):rs.getInt("turbineid")) + ":" + sdf.format(rs.getDate("vtime"));//拼接rowkey
				    Put put = new Put(Bytes.toBytes(rowKey));//新建一条记录，然后下面对相应的列进行赋值
				    put.add(base.getBytes(), qbase[0].getBytes(), Bytes.toBytes(rs.getString("farmid")));//base:fid
				    put.add(base.getBytes(), qbase[1].getBytes(), Bytes.toBytes(rs.getInt("turbineid")+""));//base:tid
				    put.add(base.getBytes(), qbase[2].getBytes(), Bytes.toBytes(rs.getDate("vtime")+""));//base:date
				    put.add(gpower.getBytes(), qgpower[0].getBytes(), Bytes.toBytes(rs.getFloat("value")+""));//gpower:power
				    put.add(gpower.getBytes(), qgpower[1].getBytes(), Bytes.toBytes(rs.getFloat("windspeed")+""));//gpower:windspeed
				    put.add(gpower.getBytes(), qgpower[2].getBytes(), Bytes.toBytes(rs.getFloat("unvalue")+""));//gpower:unvalue
				    put.add(gpower.getBytes(), qgpower[3].getBytes(), Bytes.toBytes(rs.getFloat("theory")+""));//gpower:theory
				    put.add(gpower.getBytes(), qgpower[4].getBytes(), Bytes.toBytes(rs.getFloat("coup")+""));//gpower:coup
				    list.add(put);
				}
				table.put(list);//这里真正对表进行插入操作
				log.debug(tableName + ":completed data copy!");
				table.close();//这里要非常注意一点，如果你频繁地对表进行打开跟关闭，性能将会直线下降，可能跟集群有关系。
			}else{
				admin.close();
				log.error("table '" + tableName + "' not exisit!");
				throw new IllegalArgumentException("table '" + tableName + "' not exisit!");
			}
			admin.close();
		} catch (Exception e) {
			e.printStackTrace();
		} 
	}
}
```
在put语句进行add的时候要特别注意：对于int、float、Date等等非String类型的数据，要记得将其转换成String类型，这里我直接用+""解决了，否则在你读取数据的时候就会遇到麻烦了。
步骤3：Java Web项目读取HBase里面的数据
ok,我们成功地把数据迁移到HBase，我们剩下的任务就是在Web应用中读取数据了。
首先我们要确保Web项目中已经把必要的Jar包添加到ClassPath了，下面我对一些HBase的连接做了小封装：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52909819#)[copy](http://blog.csdn.net/z69183787/article/details/52909819#)[print](http://blog.csdn.net/z69183787/article/details/52909819#)[?](http://blog.csdn.net/z69183787/article/details/52909819#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1945578/fork)
- import org.apache.hadoop.conf.Configuration;  
- import org.apache.hadoop.hbase.HBaseConfiguration;  
- import org.apache.hadoop.hbase.client.HBaseAdmin;  
- 
- /**
-  * @author a01513
-  *
-  */
- publicclass HBaseConnector {  
- 
- privatestaticfinal String QUOREM = "192.168.103.50,192.168.103.51,192.168.103.52,192.168.103.53,192.168.103.54,192.168.103.55,192.168.103.56,192.168.103.57,192.168.103.58,192.168.103.59,192.168.103.60";  
- privatestaticfinal String CLIENT_PORT = "2181";  
- private HBaseAdmin admin;  
- private Configuration conf;  
- 
- 
- public HBaseAdmin getHBaseAdmin(){  
-         getConfiguration();  
- try {  
-             admin = new HBaseAdmin(conf);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
- return admin;   
-     }  
- 
- public Configuration getConfiguration(){  
- if(conf == null){  
-             conf = HBaseConfiguration.create();  
-             conf.set("hbase.zookeeper.quorum", QUOREM);     
-             conf.set("hbase.zookeeper.property.clientPort", CLIENT_PORT);     
-         }  
- return conf;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.HBaseAdmin;
/**
 * @author a01513
 *
 */
public class HBaseConnector {
	
	private static final String QUOREM = "192.168.103.50,192.168.103.51,192.168.103.52,192.168.103.53,192.168.103.54,192.168.103.55,192.168.103.56,192.168.103.57,192.168.103.58,192.168.103.59,192.168.103.60";
	private static final String CLIENT_PORT = "2181";
	private HBaseAdmin admin;
	private Configuration conf;
	
	
	public HBaseAdmin getHBaseAdmin(){
		getConfiguration();
        try {
			admin = new HBaseAdmin(conf);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return admin; 
	}
	
	public Configuration getConfiguration(){
		if(conf == null){
			conf = HBaseConfiguration.create();
	        conf.set("hbase.zookeeper.quorum", QUOREM);   
	        conf.set("hbase.zookeeper.property.clientPort", CLIENT_PORT);   
		}
		return conf;
	}
}
```
这里的代码基本上跟迁移的那部分代码一样，由于我在其他地方都要重用这些代码，就装在一个地方免得重复写了。
我在Service层做了一下测试，下面看看具体的读取过程：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52909819#)[copy](http://blog.csdn.net/z69183787/article/details/52909819#)[print](http://blog.csdn.net/z69183787/article/details/52909819#)[?](http://blog.csdn.net/z69183787/article/details/52909819#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1945578/fork)
- privatefinal String tableName = "kpi";  
- @Override
- public List<GenPowerEntity> getGenPower(String farmid,int ltb,int htb,String start,String end) {  
-         List<GenPowerEntity> list = new ArrayList<GenPowerEntity>();  
-         HBaseConnector hbaseConn = new HBaseConnector();  
-         HBaseAdmin admin = hbaseConn.getHBaseAdmin();  
- try {  
- if(admin.tableExists(tableName)){  
-                 HTable table = new HTable(hbaseConn.getConfiguration(), tableName);  
-                 Scan scan = new Scan();  
-                 scan.addFamily(Bytes.toBytes("base"));  
-                 scan.addFamily(Bytes.toBytes("gpower"));  
-                 scan.addFamily(Bytes.toBytes("userate"));  
-                 String startRowKey = new String();  
-                 String stopRowKey = new String();  
- if("".equals(start) && !"".equals(end)){  
-                     stopRowKey = farmid + ":" + Tools.addZero(htb) + ":" + end;  
-                     scan.setStopRow(Bytes.toBytes(stopRowKey));  
-                 }elseif(!"".equals(start) && "".equals(end)){  
-                     startRowKey = farmid + ":" + Tools.addZero(ltb) + ":" + start;  
-                     scan.setStartRow(Bytes.toBytes(startRowKey));  
-                 }elseif(!"".equals(start) && !"".equals(end)){  
-                     startRowKey = farmid + ":" + Tools.addZero(ltb) + ":" + start;  
-                     stopRowKey = farmid + ":" + Tools.addZero(htb) + ":" + end;  
-                     scan.setStartRow(Bytes.toBytes(startRowKey));  
-                     scan.setStopRow(Bytes.toBytes(stopRowKey));  
-                 }else{  
-                     table.close();  
-                     admin.close();  
- returnnull;  
-                 }  
-                 ResultScanner rsc = table.getScanner(scan);  
-                 Iterator<Result> it = rsc.iterator();  
-                 List<GenPowerEntity> slist = new ArrayList<GenPowerEntity>();  
-                 List<UseRateEntity> ulist = new ArrayList<UseRateEntity>();  
-                 String tempRowKey = "";//这个临时rowkey是用来判断一行数据是否已经读取完了的。
-                 GenPowerEntity gpower = new GenPowerEntity();  
-                 UseRateEntity userate = new UseRateEntity();  
- while(it.hasNext()){  
- for(Cell cell: it.next().rawCells()){  
-                         String rowKey = new String(cell.getRowArray(),cell.getRowOffset(),cell.getRowLength(),"UTF-8");  
-                         String family = new String(cell.getFamilyArray(),cell.getFamilyOffset(),cell.getFamilyLength(),"UTF-8");  
-                         String qualifier = new String(cell.getQualifierArray(),cell.getQualifierOffset(),cell.getQualifierLength(),"UTF-8");  
-                         String value = new String(cell.getValueArray(),cell.getValueOffset(),cell.getValueLength(),"UTF-8");//假如我们当时插入HBase的时候没有把int、float等类型的数据转换成String，这里就会乱码了，并且用Bytes.toInt()这个方法还原也没有用，哈哈
-                         System.out.println("RowKey=>"+rowKey+"->"+family+":"+qualifier+"="+value);  
- if("".equals(tempRowKey))  
-                             tempRowKey = rowKey;  
- if(!rowKey.equals(tempRowKey)){  
-                             slist.add(gpower);  
-                             ulist.add(userate);  
-                             gpower = null;  
-                             userate = null;  
-                             gpower = new GenPowerEntity();  
-                             userate = new UseRateEntity();  
-                             tempRowKey = rowKey;  
-                         }  
- switch(family){  
- case"base":  
- switch(qualifier){  
- case"fid":  
-                                 gpower.setFarmid(value);  
-                                 userate.setFarmid(value);  
- break;  
- case"tid":  
-                                 gpower.setTurbineid(Integer.parseInt(value));  
-                                 userate.setTurbineid(Integer.parseInt(value));  
- break;  
- case"date":  
-                                 SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");  
-                                 Date date = null;  
- try {  
-                                     date = sdf.parse(value);  
-                                 } catch (ParseException e) {  
-                                     e.printStackTrace();  
-                                 }  
-                                 gpower.setVtime(date);  
-                                 userate.setVtime(date);  
- break;  
-                             }  
- break;  
- case"gpower":  
- switch(qualifier){  
- case"power":  
-                                 gpower.setValue(Float.parseFloat(value));  
- break;  
- case"windspeed":  
-                                 gpower.setWindspeed(Float.parseFloat(value));  
- break;  
- case"unpower":  
-                                 gpower.setUnvalue(Float.parseFloat(value));  
- break;  
- case"theory":  
-                                 gpower.setTvalue(Float.parseFloat(value));  
- break;  
- case"coup":  
-                                 gpower.setCoup(Float.parseFloat(value));  
- break;  
-                             }  
- break;  
- case"userate":  
- switch(qualifier){  
- case"num":  
-                                 userate.setFnum(Integer.parseInt(value));  
- break;  
- case"power":  
-                                 userate.setFpower(Float.parseFloat(value));  
- break;  
- case"time":  
-                                 userate.setFvalue(Float.parseFloat(value));  
- break;  
-                             }  
- break;  
-                         }  
- 
-                     }  
-                 }  
-                 rsc.close();  
-                 table.close();  
-                 admin.close();  
-                 ......  
-             }  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
- return list;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
private final String tableName = "kpi";
@Override
	public List<GenPowerEntity> getGenPower(String farmid,int ltb,int htb,String start,String end) {
		List<GenPowerEntity> list = new ArrayList<GenPowerEntity>();
		HBaseConnector hbaseConn = new HBaseConnector();
		HBaseAdmin admin = hbaseConn.getHBaseAdmin();
		try {
			if(admin.tableExists(tableName)){
				HTable table = new HTable(hbaseConn.getConfiguration(), tableName);
				Scan scan = new Scan();
				scan.addFamily(Bytes.toBytes("base"));
				scan.addFamily(Bytes.toBytes("gpower"));
				scan.addFamily(Bytes.toBytes("userate"));
				String startRowKey = new String();
				String stopRowKey = new String();
				if("".equals(start) && !"".equals(end)){
					stopRowKey = farmid + ":" + Tools.addZero(htb) + ":" + end;
					scan.setStopRow(Bytes.toBytes(stopRowKey));
				}else if(!"".equals(start) && "".equals(end)){
					startRowKey = farmid + ":" + Tools.addZero(ltb) + ":" + start;
					scan.setStartRow(Bytes.toBytes(startRowKey));
				}else if(!"".equals(start) && !"".equals(end)){
					startRowKey = farmid + ":" + Tools.addZero(ltb) + ":" + start;
					stopRowKey = farmid + ":" + Tools.addZero(htb) + ":" + end;
					scan.setStartRow(Bytes.toBytes(startRowKey));
					scan.setStopRow(Bytes.toBytes(stopRowKey));
				}else{
					table.close();
					admin.close();
					return null;
				}
				ResultScanner rsc = table.getScanner(scan);
				Iterator<Result> it = rsc.iterator();
				List<GenPowerEntity> slist = new ArrayList<GenPowerEntity>();
				List<UseRateEntity> ulist = new ArrayList<UseRateEntity>();
				String tempRowKey = "";//这个临时rowkey是用来判断一行数据是否已经读取完了的。
				GenPowerEntity gpower = new GenPowerEntity();
				UseRateEntity userate = new UseRateEntity();
				while(it.hasNext()){
					for(Cell cell: it.next().rawCells()){
						String rowKey = new String(cell.getRowArray(),cell.getRowOffset(),cell.getRowLength(),"UTF-8");
						String family = new String(cell.getFamilyArray(),cell.getFamilyOffset(),cell.getFamilyLength(),"UTF-8");
						String qualifier = new String(cell.getQualifierArray(),cell.getQualifierOffset(),cell.getQualifierLength(),"UTF-8");
						String value = new String(cell.getValueArray(),cell.getValueOffset(),cell.getValueLength(),"UTF-8");//假如我们当时插入HBase的时候没有把int、float等类型的数据转换成String，这里就会乱码了，并且用Bytes.toInt()这个方法还原也没有用，哈哈
						System.out.println("RowKey=>"+rowKey+"->"+family+":"+qualifier+"="+value);
						if("".equals(tempRowKey))
							tempRowKey = rowKey;
						if(!rowKey.equals(tempRowKey)){
							slist.add(gpower);
							ulist.add(userate);
							gpower = null;
							userate = null;
							gpower = new GenPowerEntity();
							userate = new UseRateEntity();
							tempRowKey = rowKey;
						}
						switch(family){
						case "base":
							switch(qualifier){
							case "fid":
								gpower.setFarmid(value);
								userate.setFarmid(value);
								break;
							case "tid":
								gpower.setTurbineid(Integer.parseInt(value));
								userate.setTurbineid(Integer.parseInt(value));
								break;
							case "date":
								SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
								Date date = null;
								try {
									date = sdf.parse(value);
								} catch (ParseException e) {
									e.printStackTrace();
								}
								gpower.setVtime(date);
								userate.setVtime(date);
								break;
							}
							break;
						case "gpower":
							switch(qualifier){
							case "power":
								gpower.setValue(Float.parseFloat(value));
								break;
							case "windspeed":
								gpower.setWindspeed(Float.parseFloat(value));
								break;
							case "unpower":
								gpower.setUnvalue(Float.parseFloat(value));
								break;
							case "theory":
								gpower.setTvalue(Float.parseFloat(value));
								break;
							case "coup":
								gpower.setCoup(Float.parseFloat(value));
								break;
							}
							break;
						case "userate":
							switch(qualifier){
							case "num":
								userate.setFnum(Integer.parseInt(value));
								break;
							case "power":
								userate.setFpower(Float.parseFloat(value));
								break;
							case "time":
								userate.setFvalue(Float.parseFloat(value));
								break;
							}
							break;
						}
						
					}
				}
				rsc.close();
				table.close();
				admin.close();
				......
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return list;
	}
```
这是我在Service层中用作测试的一个方法，业务逻辑代码可以直接无视（已经用.....代替了，哈哈），至此我们的所有工作完成，对于更深入的应用，还要靠自己去认真挖掘学习了。
