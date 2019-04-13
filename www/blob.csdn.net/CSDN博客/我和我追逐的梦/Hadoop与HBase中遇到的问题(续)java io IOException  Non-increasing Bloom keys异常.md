
# Hadoop与HBase中遇到的问题(续)java.io.IOException: Non-increasing Bloom keys异常 - 我和我追逐的梦~~~ - CSDN博客


2014年05月30日 09:32:04[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：4452


在使用Bulkload向HBase导入数据中, 自己编写Map与使用KeyValueSortReducer生成HFile时, 出现了下面的异常:
java.io.IOException: Non-increasing Bloom keys: 201301025200000000000003520000000000000500 after 201311195100000000000000010000000000001600
at org.apache.hadoop.hbase.regionserver.StoreFile$Writer.appendGeneralBloomfilter(StoreFile.java:869)
at org.apache.hadoop.hbase.regionserver.StoreFile$Writer.append(StoreFile.java:905)
at org.apache.hadoop.hbase.mapreduce.HFileOutputFormat$1.write(HFileOutputFormat.java:180)
at org.apache.hadoop.hbase.mapreduce.HFileOutputFormat$1.write(HFileOutputFormat.java:136)
at org.apache.hadoop.mapred.ReduceTask$NewTrackingRecordWriter.write(ReduceTask.java:586)
at org.apache.hadoop.mapreduce.TaskInputOutputContext.write(TaskInputOutputContext.java:80)
at org.apache.hadoop.hbase.mapreduce.KeyValueSortReducer.reduce(KeyValueSortReducer.java:53)
at org.apache.hadoop.hbase.mapreduce.KeyValueSortReducer.reduce(KeyValueSortReducer.java:36)
at org.apache.hadoop.mapreduce.Reducer.run(Reducer.java:177)
at org.apache.hadoop.mapred.ReduceTask.runNewReducer(ReduceTask.java:649)
at org.apache.hadoop.mapred.ReduceTask.run(ReduceTask.java:418)
at org.apache.hadoop.mapred.Child$4.run(Child.java:255)
at java.security.AccessController.doPrivileged(Native Method)
at javax.security.auth.Subject.doAs(Subject.java:396)
at org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.java:1190)
at org.apache.hadoop.mapred.Child.main(Child.java:249)

该异常在源码的StoreFile类中, 即在使用StoreFile类生成HFile文件时抛出异常, 根据控制台异常信息可以知道异常出现在源码StoreFile.java:905行处,此处是append方法,该方法调用appendGeneralBloomfilter方法,生成Bloom key, 源码为:

```python
public static class HFileGenerateMapper extends
			Mapper<LongWritable, Text, ImmutableBytesWritable, KeyValue> {
		private static int familyIndex = 0;
		private static Configuration conf = null;
		private static MyMD5 md5 = new MyMD5();
		@Override
		protected void setup(Context context) throws IOException,
				InterruptedException {
			conf = context.getConfiguration();
			familyIndex = conf.getInt("familyIndex",0);
		}
		@Override
		protected void map(LongWritable key, Text value, Context context)
				throws IOException, InterruptedException {
			ImmutableBytesWritable mykey = new ImmutableBytesWritable(
					value.toString().split(",")[0].getBytes());
			List<KeyValue> list = null;
			list = createKeyValue(value.toString());
			Iterator<KeyValue> it = list.iterator();
			while (it.hasNext()) {
				KeyValue kv = new KeyValue();
				kv = it.next();
				if (kv != null) {
					  context.write(mykey, kv);
				}
			}
		}
		/**
		 * a.CITY_NO,to_char(DT,'yyyy-MM-dd'),DATA_TYPE,E0,E1,E2,E3,E4,E5,
		 * MEASUREPOINTID,TRANSFORMERID,ZONEID,CAPACITY
		 * @param str
		 * @return
		 */
		private List<KeyValue> createKeyValue(String str) {
			List<KeyValue> list = new ArrayList<KeyValue>(CONSTANT_HBASE.TB2_FNColNames[familyIndex].length);
			String[] values = str.toString().split(",");
			String[] qualifiersName = CONSTANT_HBASE.TB2_FNColNames[familyIndex];
			for (int i = 0; i < qualifiersName.length; i++) {
				//需要作为rowKey的各个字段字符串组成RowKey
				String rowkey = values[1]+values[0]+values[11]+values[12];
				//加上32位的MD5
				rowkey += md5.getMD5Code(rowkey);
				String family = CONSTANT_HBASE.TB2_FamilyNames[familyIndex];
				String qualifier = qualifiersName[i];
			    String value_str = values[i+CONSTANT_HBASE.TB2_FNColIndex[familyIndex]-1];
				KeyValue kv = new KeyValue(Bytes.toBytes(rowkey),
						Bytes.toBytes(family), Bytes.toBytes(qualifier),
						CONSTANT_HBASE.timeStamp, Bytes.toBytes(value_str));
				list.add(kv);
			}
			return list;
		}
	}
```
关键出错的那一句在
```python
ImmutableBytesWritable rowkey = new ImmutableBytesWritable(value.toString().split(",")[0].getBytes());
```
因为最终导入RowKey的是由多个字段的字符串+32位的MD5值拼接而成的,但是生成ImmutableBytesWritable mykey却只用到第一个字段的字符串,而这个key是用来全局排序用的,所以需要mykey与KeyValue kv 的rowkey相等, 于是更改方法便是将map方法代码改成如下:

```python
@Override
		protected void map(LongWritable key, Text value, Context context)
				throws IOException, InterruptedException {
			List<KeyValue> list = null;
			list = createKeyValue(value.toString());
			Iterator<KeyValue> it = list.iterator();
			while (it.hasNext()) {
				KeyValue kv = new KeyValue();
				kv = it.next();
				if (kv != null) {
					  <span style="color:#FF0000;">context.write(new ImmutableBytesWritable(kv.getKey()), kv);</span>
				}
			}
		}
```
运行之后成功了,可以通过http://localhost:50030/jobtracker.jsp查看任务运行状态.



