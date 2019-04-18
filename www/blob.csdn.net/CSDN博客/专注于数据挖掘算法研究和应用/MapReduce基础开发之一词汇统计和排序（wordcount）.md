# MapReduce基础开发之一词汇统计和排序（wordcount） - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月22日 17:13:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4630
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
统计/var/log/boot.log中含k的字符的数量，并对含k的字符按照数量排序。需分两个job完成，一个用来统计，一个用来排序。



一、统计



1、上传文件到hadoop：

   1)新建文件夹：hadoop fs -mkdir /tmp/fjs

   2)上传文件：hadoop fs -put /var/log/boot.log /tmp/fjs



2、编写wordcount代码并导出jar和上传到namenode

   1)挂载共享文件夹，上传jar包：mount -t cifs //ip/tmp /mnt -o username=xxx,password=xxx

   2)移动jar包到tmp目录下：cp -R /mnt/wordcount.jar /tmp

   3)jar包是root权限，更改给hadoop用户：chown -R hdfs:hdfs /tmp/wordcount.jar

   代码如下：



```java
package com;

import java.io.IOException;
import java.util.StringTokenizer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class WordCount {
	
	public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable>{
		private final static IntWritable one = new IntWritable(1);
		private Text word = new Text();
		
		public void map(Object key, Text value, Context context)throws IOException, InterruptedException {
			StringTokenizer itr = new StringTokenizer(value.toString());
			while (itr.hasMoreTokens()) {
				String strVal=itr.nextToken();//获取字符
				//if(strVal.contains("k")){//如果字符包含k，则统计
					word.set(strVal);
					context.write(word, one);
				//}
			}
		}
	}
	
	public static class IntSumReducer extends Reducer<Text,IntWritable,Text,IntWritable> {
		private IntWritable result = new IntWritable();
		public void reduce(Text key, Iterable<IntWritable> values,Context context) throws IOException, InterruptedException {
			int sum = 0;
			for (IntWritable val : values) {
				sum += val.get();
			}
			result.set(sum);
			context.write(key, result);
		}
	}
	
	public static void main(String[] args) throws Exception {
	
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: wordcount <in> <out>");
			System.exit(2);
		}
		
		Job job = new Job(conf, "word count");
		job.setJarByClass(WordCount.class);
		job.setMapperClass(TokenizerMapper.class);
		job.setCombinerClass(IntSumReducer.class);
		job.setReducerClass(IntSumReducer.class);
		
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}

}
```






3、执行wordcount.jar并查看结果

   1)执行：yarn jar /tmp/wordcount.jar /tmp/fjs /tmp/fjs/out

   2)查看：hadoop fs -text /tmp/fjs/out/part-r-0000.bz2  



二、排序



1、编写wordsort代码并导出jar和上传namenode，对wordcount执行的结果进行排序；

   排序就是利用mapreduce本身的key排序功能，主要是互换key和value。

   代码如下：

```java
package com;
import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class WordSort {
	
	public static class SortIntValueMapper extends Mapper<LongWritable, Text, IntWritable, Text>{
		
		private final static IntWritable wordCount = new IntWritable(1);
		private Text word = new Text();
		
		public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
			StringTokenizer tokenizer = new StringTokenizer(value.toString());
			while (tokenizer.hasMoreTokens()) {
				word.set(tokenizer.nextToken().trim());
				wordCount.set(Integer.valueOf(tokenizer.nextToken().trim()));
				context.write(wordCount, word);//<k,v>互换
			}
		}
	}
	
	public static class SortIntValueReduce extends  Reducer<IntWritable, Text, Text, IntWritable> {
		private Text result = new Text();
		
		public void reduce(IntWritable key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			for (Text val : values) {
				result.set(val.toString());
				context.write(result, key);//<k,v>互换
			}
		}
	}
	
	public static void main(String[] args) throws Exception {
		
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: wordsort <in> <out>");
			System.exit(2);
		}
		
		Job job = new Job(conf, "word sort");
		job.setJarByClass(WordSort.class);
		
		job.setMapperClass(SortIntValueMapper.class);
		job.setReducerClass(SortIntValueReduce.class);
		
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(Text.class);
		
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
```




2、执行wordsort.jar并查看结果

   1)执行：yarn jar /tmp/wordsort.jar /tmp/fjs/out /tmp/fjs/out1

   2)查看：hadoop fs -text /tmp/fjs/out1/part-r-0001.bz2
            


