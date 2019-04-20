# 重要 | mr使用hcatalog读写hive表 - Spark高级玩法 - CSDN博客
2018年08月16日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1277
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
企业中，由于领导们的要求，hive中有数据存储格式很多时候是会变的，比如为了优化将tsv，csv格式改为了parquet或者orcfile。那么这个时候假如是mr作业读取hive的表数据的话，我们又要重新去写mr并且重新部署。这个时候就很蛋疼。hcatalog帮我们解决了这个问题，有了它我们不用关心hive中数据的存储格式。详细信息请仔细阅读本文。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
本文主要是讲mapreduce使用HCatalog读写hive表。
hcatalog使得hive的元数据可以很好的被其它hadoop工具使用，比如pig，mr和hive。
HCatalog的表为用户提供了（HDFS）中数据的关系视图，并确保用户不必担心他们的数据存储在何处或采用何种格式，因此用户无需知道数据是否以RCFile格式存储， 文本文件或sequence 文件。
它还提供通知服务，以便在仓库中有新数据可用时通知工作流工具（如Oozie）。
HCatalog提供HCatInputFormat / HCatOutputFormat，使MapReduce用户能够在Hive的数据仓库中读/写数据。 它允许用户只读取他们需要的表和列的分区。 返回的记录格式是方便的列表格式，用户无需解析它们。
下面我们举个简单的例子。
在mapper类中，我们获取表schema并使用此schema信息来获取所需的列及其值。
下面是map类。
```
public class onTimeMapper extends Mapper {
    @Override
    protected void map(WritableComparable key, HCatRecord value,
     org.apache.hadoop.mapreduce.Mapper.Context context)
     throws IOException, InterruptedException {
     // Get table schema
     HCatSchema schema = HCatBaseInputFormat.getTableSchema(context);
     Integer year = new Integer(value.getString("year", schema));
     Integer month = new Integer(value.getString("month", schema));
     Integer DayofMonth = value.getInteger("dayofmonth", schema);
     context.write(new IntPair(year, month), new IntWritable(DayofMonth));
    }
}
```
在reduce类中，会为将要写入hive表中的数据创建一个schema。
```
public class onTimeReducer extends Reducer {
public void reduce (IntPair key, Iterable value, Context context) 
  throws IOException, InterruptedException{
  
  int count = 0; // records counter for particular year-month
  for (IntWritable s:value) {
   count++;
  }
  
  // define output record schema
  List columns = new ArrayList(3);
  columns.add(new HCatFieldSchema("year", HCatFieldSchema.Type.INT, ""));
  columns.add(new HCatFieldSchema("month", HCatFieldSchema.Type.INT, ""));
  columns.add(new HCatFieldSchema("flightCount", HCatFieldSchema.Type.INT,""));
  HCatSchema schema = new HCatSchema(columns);
  HCatRecord record = new DefaultHCatRecord(3);
  
  record.setInteger("year", schema, key.getFirstInt()); 
  record.set("month", schema, key.getSecondInt());
  record.set("flightCount", schema, count);
  context.write(null, record);
 }
}
```
最后，创建driver类，并且表明输入输出schema和表信息。
```
public class onTimeDriver extends Configured implements Tool{
    private static final Log log = LogFactory.getLog( onTimeDriver.class );
    public int run( String[] args ) throws Exception{
     Configuration conf = new Configuration();
     Job job = new Job(conf, "OnTimeCount");
     job.setJarByClass(onTimeDriver.class);
     job.setMapperClass(onTimeMapper.class);
     job.setReducerClass(onTimeReducer.class);
     HCatInputFormat.setInput(job, "airline", "ontimeperf");
     job.setInputFormatClass(HCatInputFormat.class);
     job.setMapOutputKeyClass(IntPair.class);
     job.setMapOutputValueClass(IntWritable.class);
     
     job.setOutputKeyClass(Text.class);
     job.setOutputValueClass(DefaultHCatRecord.class);
     job.setOutputFormatClass(HCatOutputFormat.class);
     HCatOutputFormat.setOutput(job, OutputJobInfo.create("airline", "flight_count", null));
     HCatSchema s = HCatOutputFormat.getTableSchema(job);
     HCatOutputFormat.setSchema(job, s);
     
     return (job.waitForCompletion(true)? 0:1);
    }
    
    public static void main(String[] args) throws Exception{
  int exitCode = ToolRunner.run(new onTimeDriver(), args);
  System.exit(exitCode);
 }
}
```
当然，在跑上面写的代码之前，应该先在hive中创建输出表。
```
create table airline.flight_count
(Year INT ,
Month INT ,
flightCount INT)
ROW FORMAT DELIMITED FIELDS TERMINATED BY ',' 
STORED AS TEXTFILE;
```
可能会引起错误的地方是没有设置$HIVE_HOME.
**[完]**
欢迎点击**阅读原文**或**扫二维码**加入浪尖**知识星球**，获取更多优质的大数据学习资源和指导。
**推荐阅读：**
[Hive性能优化（全面）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485140&idx=1&sn=dd8d05309b8e2e86b3bde6728c6932ec&chksm=9f38e5fca84f6ceae8eb4791337ccfe81fc6764890100bb2cb7f7aec2ad23b1a78e1e25f56c4&scene=21#wechat_redirect)
[Hive高级优化](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484413&idx=1&sn=1573cc39a44d65407223f08a6315c1e4&chksm=9f38e0d5a84f69c373b971283b905f291ebcdf28cd3d686fbf25316d2ac4de6d5c356f270a96&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
