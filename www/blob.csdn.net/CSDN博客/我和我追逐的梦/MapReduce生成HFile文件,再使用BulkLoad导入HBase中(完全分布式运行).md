
# MapReduce生成HFile文件,再使用BulkLoad导入HBase中(完全分布式运行) - 我和我追逐的梦~~~ - CSDN博客


2014年05月09日 21:16:16[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：23634


**声明: 若要转载, 请标明出处.**
前提: 在对于大量的数据导入到HBase中, 如果一条一条进行插入, 则太耗时了, 所以可以先采用MapReduce生成HFile文件, 然后使用BulkLoad导入HBase中.
引用:
一、这种方式有很多的优点：
1. 如果我们一次性入库hbase巨量数据，处理速度慢不说，还特别占用Region资源， 一个比较高效便捷的方法就是使用 “Bulk Loading”方法，即HBase提供的HFileOutputFormat类。
2. 它是利用hbase的数据信息按照特定格式存储在hdfs内这一原理，直接生成这种hdfs内存储的数据格式文件，然后上传至合适位置，即完成巨量数据快速入库的办法。配合mapreduce完成，高效便捷，而且不占用region资源，增添负载。
二、这种方式也有很大的限制：
1. 仅适合初次数据导入，即表内数据为空，或者每次入库表内都无数据的情况。
2. HBase集群与Hadoop集群为同一集群，即HBase所基于的HDFS为生成HFile的MR的集群.
本文代码采用Eclipse编辑器(Linux环境下)
一. 网上的大部分代码都是或多或少有问题, 比如他们或者不是运行在集群上,或者运行时有问题, 后面会对产生哪些问题进行说明, 先不说这么多了,先上代码吧.
二. 源代码(注: 作者亲测运行在集群上成功,集群基于Ubuntu12.04, Hadoop-1.2.1与HBase-0.98,使用自带的ZooKeeper)
1.  MapReduce生产HFile文件
首先, 需要导入的数据的表格(BigClientEnergyInfo表)有四个列族, 每个列族下面有一些列, 这些信息都使用常量配置类CONSTANT_HADOOP与CONSTANT_HBASE进行说明,如下:
```python
package cn.hey.loaddata2hbase;
/**
 * 
 * @author HeYong
 * @version 1
 * @time 2014-05-09
 *
 */
public class CONSTANT_HADOOP {
	//大客户表BigClientEnergyInfo的HFile生成Job名字
	public static final String BigClientEnergyInfo_JobName = "BigClientEnergyInfo_HFileGenerator_Job";
	//大客户表BigClientEnergyInfo的输入原始文本信息的HDFS路径
	public static final String BigClientEnergyInfo_inDir = "hdfs://node1:49000/user/hadoop/input/BigClientEnergyInfo/";
	//大客户表BigClientEnergyInfo的HFile文件的输出HDFS路径
	public static final String BigClientEnergyInfo_HFile_outDir = "hdfs://node1:49000/user/hadoop/output/BigClientEnergyInfo/";
	
	//说明: 因为在创建HBase表的时候,默认只有一个Region,只有等到这个Region的大小超过一定的阈值之后,才会进行split
	//所以为了利用完全分布式加快生成HFile和导入HBase中以及数据负载均衡,所以需要在创建表的时候预先进行分区,
	//而进行分区时要利用startKey与endKey进行rowKey区间划分(因为导入HBase中,需要rowKey整体有序),所以在导入之前,自己先写一个MapReduce的Job求最小与最大的rowKey
	//即startKey与endKey
	//获取最大rowKey与最小rowKey的Job名字
	public static final String GetMaxAndMinRowKey_JobName = "GetMaxAndMinRowKey_Job";
	//大客户表BigClientEnergyInfo的输入原始文本信息的HDFS路径
	public static final String GetMaxAndMinRowKey_inDir = "hdfs://node1:49000/user/hadoop/input/BigClientEnergyInfo/";
	//最大rowKey与最小rowKey的输出HDFS路径
	public static final String GetMaxAndMinRowKey_outDir = "hdfs://node1:49000/user/hadoop/output/GetMaxAndMinRowKey/";
}
```

```python
package cn.hey.loaddata2hbase;
import java.util.LinkedList;
import java.util.List;
import org.apache.hadoop.hbase.client.HTable;
/**
 * 
 * @author HeYong
 * @version 1
 * @time 2014-05-09
 *
 */
public class CONSTANT_HBASE {
	public static final long timeStamp = System.currentTimeMillis();
	//表集合
	public static List<HTable> htables = new LinkedList<HTable>();
	public static final String[] TableNames = {"BigClientEnergyInfo"};
	
	/**
	 * 大客户表信息
	 */
	//列族信息
	public static final String[] TB0_FamilyNames = {"DateTime","MeterEnergy","ObjInfo","ClientInfo"};
	//第1个列族中的列
	public static final String[] TB0_FN0ColNames ={"DATETIME"}; 
	//第2个列族中的列
	public static final String[] TB0_FN1ColNames ={"DT","OBJ_ID","E0","E1","E2","E3","E4","E5"}; 
	//第3个列族中的列
	public static final String[] TB0_FN2ColNames ={"STAT_TYPE","CITY_NO","OBJ_ID","OBJ_NAME","LAYER","LAYER_ID","OBJ_TYPE","TYPE_VALUE",
		"TYPE_VALUE_GROUP","SORT","SYS_ID","STATION_NO","FLAG"}; 
	//第4个列族中的列
	public static final String[] TB0_FN3ColNames ={"CITY_NO","CONSUMERID","CONSUMERNAME","CUSTOMERTYPE","USERSTATUS","USERADDR","ZONEID","INDUSTRYTYPE",
		"LINKMAN","LINKPHONE","USETYPE","LINEID"}; 
	//列族信息集合
	public static final String[][] TB0_FNColNames={TB0_FN0ColNames,TB0_FN1ColNames,TB0_FN2ColNames,TB0_FN3ColNames};
	//每个列族的列索引
	public static final int[] FNColIndex={1,2,10,23};
	
}
```
接着, 使用创建一个生成四个列族的HFile的MapRed Job,每个列族一个Job, 源代码如下(类BigClientEnergyInfoHFileGenerator):
其中有三点需要特别注意:
(1)

```python
//特别注意: 一定要设置,不然会报cannot read partitioner file错误
  conf.set("fs.default.name","node1:49000");
```
(2)
```python
//特别注意: 一定要设置,不然不会运行在集群上
  conf.set("mapred.job.tracker","node1:49001");
```
(3)
```python
//特别注意: 对相关Class文件以及依赖的jar包(如HBase的jar,)进行打包,这是运行在集群上必须要做的一步,不然集群找不到相关的Mapper等类文件
 		 File jarpath;
		 try {
			 jarpath = JarTools.makeJar("bin");
			 conf.set("mapred.jar", jarpath.toString());
		 } catch (Exception e) {
			 logger.error("进行jar打包出错!");
			 e.printStackTrace();
			 return;
		 }
```
特别注意:  因为我这里是对工程下的bin目录里面的内容进行打包,所以需要把依赖的jar包先放入bin文件夹中, 再Bulid Path->Add to Build Path, 不然会出现在运行时, 依赖的包中的类找不到, 如HBase包中的ImmutableBytesWritable类等.  当然你也可以放在别的目录下,然后进行打包, 反正需要将相关Class文件与依赖的jar包进行打包. 这里自己写了一个JarTools类进行对指定文件夹下面的内容进行打包


```python
package cn.hey.loaddata2hbase;
import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.mapreduce.HFileOutputFormat;
import org.apache.hadoop.hbase.mapreduce.KeyValueSortReducer;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import cn.hey.file.FileOperation;
import cn.hey.hbase.HbaseOperation;
import cn.hey.utils.JarTools;
/**
 * 
 * @author HeYong
 * @version 1
 * @time 2014-05-09
 *
 */
public class BigClientEnergyInfoHFileGenerator {
	public static Logger logger = LogManager.getLogger(BigClientEnergyInfoHFileGenerator.class);
	/**
	 * 
	 * @param args 第一个元素表示第几个表,第二个元素表示该表的列族个数
	 * @throws IOException
	 * @throws InterruptedException
	 * @throws ClassNotFoundException
	 * @throws Exception
	 */
	public static void main(String[] args) throws  IOException, InterruptedException, ClassNotFoundException, Exception{
		if(args.length<2){
			logger.error("参数个数不对!");
			return;
		}
		int tableIndex = Integer.parseInt(args[0]);
		int familyNum = Integer.parseInt(args[1]);
		int index = 0;
		long beginTime=0,endTime=0;
		while(index<familyNum){
			beginTime = System.currentTimeMillis();
			GeneratorJob(tableIndex,index);
			endTime = System.currentTimeMillis();
			FileOperation.append2File(System.getProperty("user.dir")+File.separator+"file"+File.separator+"runTime1.txt",(((endTime-beginTime)/(1.0*60*1000)))+"\n");
			++index;
		}
		FileOperation.append2File(System.getProperty("user.dir")+File.separator+"file"+File.separator+"runTime1.txt","-----------------------------");
	}
	
	public static class HFileGenerateMapper extends
			Mapper<LongWritable, Text, ImmutableBytesWritable, KeyValue> {
		private static int familyIndex = 0;
		private static Configuration conf = null;
		@Override
		protected void setup(Context context) throws IOException,
				InterruptedException {
			conf = context.getConfiguration();
			familyIndex = conf.getInt("familyIndex",0);
		}
		@Override
		protected void map(LongWritable key, Text value, Context context)
				throws IOException, InterruptedException {
			ImmutableBytesWritable rowkey = new ImmutableBytesWritable(
					value.toString().split(",")[0].getBytes());
			List<KeyValue> list = null;
			list = createKeyValue(value.toString());
			Iterator<KeyValue> it = list.iterator();
			while (it.hasNext()) {
				KeyValue kv = new KeyValue();
				kv = it.next();
				if (kv != null) {
					context.write(rowkey, kv);
				}
			}
		}
		private List<KeyValue> createKeyValue(String str) {
			List<KeyValue> list = new ArrayList<KeyValue>(CONSTANT_HBASE.TB0_FNColNames[familyIndex].length);
			String[] values = str.toString().split(",");
			String[] qualifiersName = CONSTANT_HBASE.TB0_FNColNames[familyIndex];
			for (int i = 0; i < qualifiersName.length; i++) {
				String rowkey = values[0];
				String family = CONSTANT_HBASE.TB0_FamilyNames[familyIndex];
				String qualifier = qualifiersName[i];
				String value_str = values[i+CONSTANT_HBASE.FNColIndex[familyIndex]];
				KeyValue kv = new KeyValue(Bytes.toBytes(rowkey),
						Bytes.toBytes(family), Bytes.toBytes(qualifier),
						CONSTANT_HBASE.timeStamp, Bytes.toBytes(value_str));
				list.add(kv);
			}
			return list;
		}
	}
	//测试Mapper,用来进行测试的, 后面没有用到
    public static class HFileMapper extends Mapper<LongWritable, Text,ImmutableBytesWritable,KeyValue> {  
   
        protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {  
            String[] values = value.toString().split(" ", -1);  
            
            
            byte[] rkey = Bytes.toBytes(values[0]);     //rowkey
            byte[] family = Bytes.toBytes("info");      //列族
            byte[] column = Bytes.toBytes("name");      //列
            byte[] val = Bytes.toBytes(values[1]);      //值
            //Put tmpPut=new  Put(subject);
            ImmutableBytesWritable rowKey = new ImmutableBytesWritable(rkey);  
           
            KeyValue kvProtocol = new KeyValue(rkey , family, column, val);                                                                                  
            context.write(rowKey,  kvProtocol );    
        }  
  
    }  
   /**
   * 
   * @param tableIndex 表示第几个表(从0开始),具体参见CONSTANT_HBASE类
   * @param familyIndex 表示该表的第几个列族(从0开始),具体参见CONSTANT_HBASE类
   * @throws IOException
   */
    public static void GeneratorJob(int tableIndex,int familyIndex) throws IOException{  
         Configuration conf = HbaseOperation.HBASE_CONFIG; 
         //特别注意: 一定要设置,不然会爆cannot read partitioner file错误
         conf.set("fs.default.name","node1:49000");
         //特别注意: 一定要设置,不然不会运行在集群上
         conf.set("mapred.job.tracker","node1:49001");
         //特别注意: 对相关Class以及依赖的jar包(如HBase的jar)进行打包,这是运行在集群上必须要做的一步,不然集群找不到相关的Mapper等类文件
 		 File jarpath;
		 try {
			 jarpath = JarTools.makeJar("bin");
			 conf.set("mapred.jar", jarpath.toString());
		 } catch (Exception e) {
			 logger.error("进行jar打包出错!");
			 e.printStackTrace();
			 return;
		 }
 		 
		 //设置job
         Job job = new Job(conf, CONSTANT_HADOOP.BigClientEnergyInfo_JobName);  
         job.setJarByClass(BigClientEnergyInfoHFileGenerator.class);  
   
         //设置Map任务输出Key-Value类型,一定要为该类型,Value可以改为HBase的Put类型
         job.setOutputKeyClass(ImmutableBytesWritable.class);  
         job.setOutputValueClass(KeyValue.class);  
   
         //设置Mapper与Reducer类
         job.setMapperClass(HFileGenerateMapper.class);  
         job.setReducerClass(KeyValueSortReducer.class);  
         // 不需要设置,系统会根据相关信息调用 HFileOutputFormat
         // job.setOutputFormatClass(HFileOutputFormat.class);
         // 不需要设置, 系统会根据表的Region数创建多少Reducer
         // job.setNumReduceTasks(4);  
         // job.setPartitionerClass(org.apache.hadoop.hbase.mapreduce.SimpleTotalOrderPartitioner.class);  
          HTable table = new HTable(conf, CONSTANT_HBASE.TableNames[tableIndex]);  
          HFileOutputFormat.configureIncrementalLoad(job, table);  
   
          //设置数据输入输出目录
          String str_inPath = CONSTANT_HADOOP.BigClientEnergyInfo_inDir;
          String str_outPath = CONSTANT_HADOOP.BigClientEnergyInfo_HFile_outDir+CONSTANT_HBASE.TB0_FamilyNames[familyIndex];
          
          //创建HDFS对象
          FileSystem fs = FileSystem.get(URI.create(str_inPath),conf);
		  // 如果输出路径存在就先删掉，因为不允许输出路径事先存在
	  	  Path outPath = new Path(str_outPath);
	  	  if (fs.exists(outPath))
	  		  fs.delete(outPath, true);
	      FileInputFormat.addInputPath(job, new Path(str_inPath));  
	      FileOutputFormat.setOutputPath(job, new Path(str_outPath));  
	   
	      try {
				job.waitForCompletion(true);
		  } catch (InterruptedException e) {
			  logger.info(CONSTANT_HADOOP.BigClientEnergyInfo_JobName+" 任务运行出错!");
				e.printStackTrace();
		  } catch (ClassNotFoundException e) {
			    logger.info(CONSTANT_HADOOP.BigClientEnergyInfo_JobName+" 任务运行出错!");
				e.printStackTrace();
		  }
    }  
}
```

生成HFile程序说明：
①. 最终输出结果，无论是map还是reduce，输出部分key和value的类型必须是： < ImmutableBytesWritable, KeyValue>或者< ImmutableBytesWritable, Put>。
②. 最终输出部分，Value类型是KeyValue 或Put，对应的Sorter分别是KeyValueSortReducer或PutSortReducer。
③. MR例子中job.setOutputFormatClass(HFileOutputFormat.class); HFileOutputFormat只适合一次对单列族组织成HFile文件。好像最新的版本可以多个列族.
④. MR例子中HFileOutputFormat.configureIncrementalLoad(job, table);自动对job进行配置。TotalOrderPartitioner是需要先对key进行整体排序，然后划分到每个reduce中，保证每一个reducer中的的key最小最大值区间范围，是不会有交集的。因为入库到HBase的时候，作为一个整体的Region，key是绝对有序的。
⑤. MR例子中最后生成HFile存储在HDFS上，输出路径下的子目录是各个列族。如果对HFile进行入库HBase，相当于move HFile到HBase的Region中，HFile子目录的列族内容没有了。
然后, 使用BulkLoad工具将HFile文件导入HBase中,  源代码如下(类BigClientEnergyInfoHFileLoader):

```python
package cn.hey.loaddata2hbase;
import java.io.File;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.mapreduce.LoadIncrementalHFiles;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import cn.hey.file.FileOperation;
import cn.hey.hbase.HbaseOperation;
/**
 * 
 * @author HeYong
 * @version 1
 * @time 2014-05-09
 *
 */
public class BigClientEnergyInfoHFileLoader {
	public static Logger logger = LogManager.getLogger(HFileLoader.class);
	public static void main(String[] args) throws Exception {
     
		
		if(args.length<2){
			logger.error("参数个数不对!");
			return;
		}
        LoadIncrementalHFiles loader = new LoadIncrementalHFiles(
        		HbaseOperation.HBASE_CONFIG);
      
        int tableIndex = Integer.parseInt(args[0]);
        int familyNum = Integer.parseInt(args[1]);
        int i = 0;
        long beginTime=0,endTime=0;
        while(i<familyNum){
        	beginTime = System.currentTimeMillis();
            String str_outPath = CONSTANT_HADOOP.str_outPath+CONSTANT_HBASE.TB0_FamilyNames[i];
            loader.doBulkLoad(new Path(str_outPath),CONSTANT_HBASE.htables.get(tableIndex));
    		endTime = System.currentTimeMillis();
                //将用时相关写入文件
                FileOperation.append2File(System.getProperty("user.dir")+File.separator+"file"+File.separator+"runTime2.txt",(((endTime-beginTime)/(1.0*60*1000)))+"\n");
    		++i;
        }
        FileOperation.append2File(System.getProperty("user.dir")+File.separator+"file"+File.separator+"runTime2.txt","------------------------");
	}
	
}
```
最后,使用一个Driver类, 先创建HTable,然后调用上面的两个类,源代码如下(类BigClientEnergyInfoLoadDriver):
说明: 因为在创建HBase表的时候,默认只有一个Region,只有等到这个Region的大小超过一定的阈值之后,才会进行split, 所以为了利用完全分布式加快生成HFile和导入HBase中以及数据负载均衡,所以需要在创建表的时候预先创建分区,可以查阅相关资料(关于HBase调优的资料), 而进行分区时要利用startKey与endKey进行rowKey区间划分(因为导入HBase中,需要rowKey整体有序),所以在导入之前,自己先写一个MapReduce的Job求最小与最大的rowKey,
 即startKey与endKey.

```python
package cn.hey.loaddata2hbase;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import cn.hey.hbase.HbaseOperation;
import cn.hey.hdfs.HDFSOperation;
/**
 * 
 * @author HeYong
 * @version 1
 * @time 2014-05-09
 *
 */
public class BigClientEnergyInfoLoadDriver {
	protected static Logger logger = LogManager.getLogger(BigClientEnergyInfoLoadDriver.class);
	/**
	 * @param args
	 * @throws ClassNotFoundException 
	 * @throws InterruptedException 
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
		//首先删除在CONSTANT_HBASE类中的第0个表,即BigClientEnergyInfo表
		dropHTable(0);
		/**
		 * 说明: 因为在创建HBase表的时候,默认只有一个Region,只有等到这个Region的大小超过一定的阈值之后,才会进行split,
		 * 所以为了利用完全分布式加快生成HFile和导入HBase中以及数据负载均衡,所以需要在创建表的时候预先创建分区,可以查阅相关资料(关于HBase调优的资料),
		 * 而进行分区时要利用startKey与endKey进行rowKey区间划分(因为导入HBase中,需要rowKey整体有序),所以在导入之前,自己先写一个MapReduce的Job求最小与最大的rowKey,
		 * 即startKey与endKey.
		 * 
		 */
		//调用GetMaxAndMinRowKeyDriver.获取startKey与endKey
		GetMaxAndMinRowKeyDriver.main(null);
		//读取startKey与endKey,readHDFSFile方法即读取指定HDFS文件中的内容,每一行作为一个字符串
		List<String> strList = HDFSOperation.readHDFSFile(CONSTANT_HADOOP.GetMaxAndMinRowKey_outDir+"part-r-00000");
		if(strList==null||strList.size()<2){
			logger.info("startKey与endKey读取失败!");
			return;
		}
		String startKey = strList.get(0);
		String endKey = strList.get(1);
		if(startKey==null||"".equals(startKey)||endKey==null||"".equals(endKey)){
			logger.info("startKey或endKey为空!");
			return;
		}
		args = new String[2];
		//第0个表,表的索引,即表BigClientEnergyInfo
		args[0]="0";
		//该表所拥有的列族的数目
		args[1]= ""+CONSTANT_HBASE.TB0_FamilyNames.length;
		//创建第0个表,即大客户表
		boolean flag = false;
		try {
			//创建表时预先创建的Region个数
			int numPreRegions = 7;
			flag = createHTable(0,startKey,endKey,numPreRegions);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		if(flag){
			//产生该表的HFile文件
			try {
				BigClientEnergyInfoHFileGenerator.main(args);
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			//将HFile导入HBase中
			try {
				HFileLoader.main(args);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	/**
	 * 
	 * @param index 第几个表
	 * @param startKey 创建预先分区的startKey
	 * @param endKey 创建预先分区的endKey
	 * @param numRegions 创建预先分区个数
	 * @return 是否创建成功
	 * @throws IOException
	 */
	public static boolean createHTable(int index,String startKey,String endKey,int numRegions) throws IOException{
		if(index<0||index>=CONSTANT_HBASE.TableNames.length){
			logger.error("表下标越界!");
			return false;
		}
		if(startKey==null||"".equals(startKey)){
			logger.error("startKey不能为空!");
			return false;
		}
        if(endKey==null||"".equals(endKey)){
        	logger.error("endKey不能为空!");
			return false;
		}
        if(numRegions<0){
        	logger.error("分区个数<0!");
			return false;
        }
		List<String> list = new ArrayList<String>();
		String tableName = CONSTANT_HBASE.TableNames[index];
		for(String familyName:CONSTANT_HBASE.TB0_FamilyNames){
			list.add(familyName);
		}
		if(HbaseOperation.createTable(tableName, list,startKey,endKey,numRegions)){
			logger.info("创建HTable :"+tableName+"成功");
		}
		HTable table = new HTable(HbaseOperation.HBASE_CONFIG,tableName);
		CONSTANT_HBASE.htables.add(table);
		return true;
	}
	public static void dropHTable(int index){
		String tableName = CONSTANT_HBASE.TableNames[index];
		HbaseOperation.dropTable(tableName);
	}
	
}
```

```python
注: HbaseOperation.createTable方法, 即创建表, HbaseOperation.dropTable方法,即删除表, 源代码如下:
```
```python
/**
	 * 创建表
	 * 
	 * @param tableName
	 * @param family 列族集名称
	 * @param String startKey,String endKey,int numRegions 预先分区相关信息
	 */
	public static boolean createTable(String tableName,List<String> family,String startKey,String endKey,int numRegions) {
		try {
			hBaseAdmin = new HBaseAdmin(HBASE_CONFIG);
			//如果表已存在,则返回
			if (hBaseAdmin.tableExists(tableName)) {
				//hBaseAdmin.disableTable(tableName);
				//hBaseAdmin.deleteTable(tableName);
				logger.info("表: "+tableName+"已经存在!");
				return false;
			}
			HTableDescriptor tableDescriptor = new HTableDescriptor(tableName);
			for(String name:family){
				tableDescriptor.addFamily(new HColumnDescriptor(name));
			}
			hBaseAdmin.createTable(tableDescriptor,Bytes.toBytes(startKey),Bytes.toBytes(endKey),numRegions);
		} catch (MasterNotRunningException e) {
			e.printStackTrace();
		} catch (ZooKeeperConnectionException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return true;
	}
```

```python
/**
	 * 删除一张表
	 * 
	 * @param tableName 表名
	 */
	public static void dropTable(String tableName) {
		if(tableName==null||"".equals(tableName)){
			logger.error("表名不能为空!");
			return;
		}
		try {
			hBaseAdmin = new HBaseAdmin(HBASE_CONFIG);
			hBaseAdmin.disableTable(tableName);
			hBaseAdmin.deleteTable(tableName);
		} catch (MasterNotRunningException e) {
			e.printStackTrace();
		} catch (ZooKeeperConnectionException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
```
特别注意: 对HBase进行操作时, 在获取HBase conf时, 即public static Configuration HBASE_CONFIG = HBaseConfiguration.create();的时候, 一定要进行如下设置:
```python
static {            
                 //设置HMaster
                  HBASE_CONFIG.set("hbase.zookeeper.master","node1:60000");
		 //设置Zookeeper集群
		  HBASE_CONFIG.set("hbase.zookeeper.quorum", "node2,node3,node4,node5,node6,node7,node8");	
	 }
```
```python
不然会出现RegionServer的Zookeeper连接不上HMaster, 千万要注意.
```
到这里就基本大功告成了. 可以通过node1:50030查看job的运行情况, 通过node1:60010查看HBase的相关情况.
下一篇将讲述中间遇到的问题以及解决办法.



