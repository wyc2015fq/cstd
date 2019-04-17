# MapReduce基础开发之十读写ORC File - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月18日 16:43:17[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6144
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、ORC File

Orc是Hive特有的一种列式存储的文件格式，它有着非常高的压缩比和读取效率，因此很快取代了之前的RCFile，成为Hive中非常常用的一种文件格式。


2、编译ORC Jar包

    http://orc.apache.org/ 下载源代码orc-1.2.1.tar.gz编译jar包

   用ubuntu14编译，安装jdk1.8、cmake3.2.2、Maven3.0.5。

  解压orc-1.2.1.tar.gz，进入目录orc-1.2.1/java下，执行mvn package编译生成jar文件。

  编译完成后进入

orc-1.2.1/java/mapreduce/target获取orc-mapreduce-1.2.1.jar

orc-1.2.1/java/core/target获取orc-core-1.2.1.jar

orc-1.2.1/java/tools/target获取orc-tools-1.2.1.jar

orc-1.2.1/java/tools/target获取orc-tools-1.2.1-uber.jar

orc-1.2.1/java/ storage-api /target获取hive-storage-api-2.1.1-pre-orc.jar

四个jar包导入到MapReduce工程lib目录下。

注意提交到hadoop集群时，第三方独立jar要一并打包。



3、MR读写ORC File基础代码




```java
import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.orc.mapred.OrcStruct;
import org.apache.orc.mapreduce.OrcInputFormat;
import org.apache.orc.mapreduce.OrcOutputFormat;
import org.apache.orc.TypeDescription;


public class ORCSample {

	public static class ORCMapper extends Mapper<NullWritable, OrcStruct, Text, Text> {
		private Text oKey=new Text();
		private Text oValue=new Text();
		public void map(NullWritable key, OrcStruct value, Context context) throws IOException, InterruptedException {
			//要知道OrcStruct存储的结构
			StringBuffer bf = new StringBuffer();			
			if(value.getNumFields()==3){
				Text valAcount=(Text)value.getFieldValue(0);
				Text valDomain=(Text)value.getFieldValue(1);
				Text valPost=(Text)value.getFieldValue(2);
				bf.append(valAcount.toString()).append("|").append(valDomain.toString()).append("|").append(valPost.toString());
			}
	  	    if (bf.length() > 0) oValue.set(bf.toString());
	  	    else oValue.set("");
	  	  oKey.set("");
	  	  context.write(oKey,oValue);
		}
	}
	/*
	public static class ORCReducer extends Reducer<Text, Text, Text, Text> {
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			for(Text value:values){
				String strVal=value.toString();
				Text okey=new Text();
				okey.set(strVal);
				context.write(okey,null);
			}
		}
	}*/
	
	public static class ORCReducer extends Reducer<Text, Text, NullWritable, OrcStruct> {
		//具体OrcStruct字段对应hadoop的定义参考https://orc.apache.org/docs/mapreduce.html
		private TypeDescription schema = TypeDescription.fromString("struct<account:string,domain:string,post:string>");
		private OrcStruct orcs = (OrcStruct) OrcStruct.createValue(schema);
		private final NullWritable nw = NullWritable.get();
		public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			for (Text val : values) {
				if(val.toString() ==null) continue;
                String[] strVals=val.toString().split("\\|");
				if(strVals.length==3){
					Text txtAccount=new Text();
					txtAccount.set(strVals[0]);
					orcs.setFieldValue(0, txtAccount);
					Text txtDomain=new Text();
					txtAccount.set(strVals[1]);
					orcs.setFieldValue(1, txtDomain);
					Text txtPost=new Text();
					txtAccount.set(strVals[2]);
					orcs.setFieldValue(2, txtPost);
					context.write(nw, orcs);
				}				
			}
		}
	}
    
	public static void main(String args[]) throws Exception {
		Configuration conf = new Configuration();
		//要设置结构，否则reduce会提示输入空值
		conf.set("orc.mapred.output.schema","struct<account:string,domain:string,post:string>");
		Job job = new Job(conf, "ORCSample");	
		job.setJarByClass(ORCSample.class);
		job.setMapperClass(ORCMapper.class);
		job.setReducerClass(ORCReducer.class);
		//map类型设置
		job.setInputFormatClass(OrcInputFormat.class);
		job.setMapOutputKeyClass(Text.class);//优先于setOutputKeyClass生效于map
		job.setMapOutputValueClass(Text.class);
		//reduce类型设置	
		job.setNumReduceTasks(1);
		job.setOutputFormatClass(OrcOutputFormat.class);
		job.setOutputKeyClass(NullWritable.class);
		job.setOutputValueClass(OrcStruct.class);
		//job.setOutputKeyClass(Text.class);//对map和reduce输出都生效
		//job.setOutputValueClass(Text.class);
		//输入输出路径
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
```







