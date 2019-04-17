# MapReduce基础开发之二数据去重和排序 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月23日 11:14:31[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4238
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









因Hadoop集群平台网络限制，只能在eclipse里先写好代码再提交jar到集群平台namenode上执行，不能实时调试，所以没有配置eclipse的hadoop开发环境，只是引入了hadoop的lib包。

eclipse的hadoop开发环境配置可参考：http://www.cnblogs.com/xia520pi/archive/2012/05/20/2510723.html

MapReduce的基础开发也是通过该博客系列学习到，很感谢！


1、数据去重

   在MapReduce流程中，map的输出<key，value>经过shuffle过程聚集成<key，value-list>后会交给reduce。 map将输入的value复制到输出数据的key上，并直接输出；

   经过shuffle，相同key形成<key，value-list>，作为reduce的输入；reduce将输入中的key复制到输出数据的key上，并直接输出。利用MapReduce对key的汇聚机制将重复的数据去掉。


   1）在namenode系统tmp目录下新建输入文件file1.txt和file2.txt，上传到hadoop

         命令：hadoop fs -put /tmp/file1.txt /tmp/fjs/in

         注意：file1.txt和file2.txt中要有重复的行，可以看出去重效果

   2）编码DataDedup并导出mr.jar和上传到namenode系统tmp目录下

         注意：通过eclipse工程导出Runnable JAR file包含hadoop lib包，如通过Fatjar不要勾选one-JAR。

        代码如下：



```java
package com.data;

import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class DataDedup {
	
	public static class Map extends Mapper<Object,Text,Text,Text>{
		private static Text line=new Text();//每行数据
		public void map(Object key,Text value,Context context) throws IOException,InterruptedException{
			line=value;
			context.write(line, new Text(""));
		}
	}
	
	public static class Reduce extends Reducer<Text,Text,Text,Text>{
		public void reduce(Text key,Iterable<Text> values,Context context) throws IOException,InterruptedException{
			context.write(key, new Text(""));
		}
	}
	
public static void main(String[] args) throws Exception {
		
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: datadedup <in> <out>");
			System.exit(2);
		}
		
		Job job = new Job(conf, "data dedup");
		job.setJarByClass(DataDedup.class);
		job.setNumReduceTasks(1);//设置reduce输入文件一个，方便查看结果，如设置为0就是不执行reduce，map就输出结果
		
		job.setMapperClass(Map.class);
		job.setCombinerClass(Reduce.class);
		job.setReducerClass(Reduce.class);
		
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
```



   3）执行jar并查看结果

         执行：yarn jar /tmp/mr.jar /tmp/fjs/in /tmp/fjs/out 

         注意：out目录不能先创建，hadoop会自己创建

         查看：hadoop fs -text /tmp/fjs/out/part-r-00000.bz2

        注意：这里执行的Hadoop集群平台reduce输出文件是bz2压缩

                   为方便观察结果，设置reduce数量为1，job.setNumReduceTasks(1)



2、数据排序

   MapReduce过程中按照key值排序的，如果key为封装int的IntWritable类型则按照数字大小对key排序，如果key为封装为String的Text类型，则按照字典顺序对字符串排序。

   在map中将读入的数据转化成IntWritable型，然后作为key值输出（value任意）。reduce拿到<key，value-list>后，将输入的key作为value输出，并根据value-list中元素的个数决定输出的次数。输出的key（即代码中的linenum）是一个全局变量，它统计当前key的位次。MapReduce过程中map和reduce即完成任务，不需要配置Combiner任务。

   1）在namenode系统tmp目录下新建输入文件file3.txt和file4.txt，上传到hadoop

        命令：hadoop fs -put /tmp/file3.txt /tmp/fjs/in1

        注意：file3.txt和file4.txt中每行输入整数，最好有重复；

   2）编码DataSort并导出mr.jar和上传到namenode系统tmp目录下

        注意：和DataDedup放在同一工程下，导出时，要选择DataSort作为主类

        代码如下：



```java
package com.data;

import java.io.IOException;
import java.util.Iterator;

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

public class DataSort {
	//map将输入中的value化成IntWritable类型，作为输出的key
	public static class Map extends Mapper<Object,Text,IntWritable,IntWritable>{
		private static IntWritable data=new IntWritable();
		public void map(Object key,Text value,Context context) throws IOException,InterruptedException{
			String line=value.toString();
			data.set(Integer.parseInt(line));
			context.write(data, new IntWritable(1));
        }
	}

    //reduce将输入中的key复制到输出数据的key上，
    //然后根据输入的value-list中元素的个数决定key的输出次数
    //用全局linenum来代表key的位次
    public static class Reduce extends Reducer<IntWritable,IntWritable,IntWritable,IntWritable>{
    	private static IntWritable linenum = new IntWritable(1);
    	public void reduce(IntWritable key,Iterable<IntWritable> values,Context context)throws IOException,InterruptedException{
    		Iterator<IntWritable> itVal=values.iterator();
    		while(itVal.hasNext()){//for(IntWritable val:values){			
    			context.write(linenum, key);
    			linenum = new IntWritable(linenum.get()+1);
    		}
    	}
    }
    
public static void main(String[] args) throws Exception {
		
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: datasort <in> <out>");
			System.exit(2);
		}
		
		Job job = new Job(conf, "data sort");
		job.setJarByClass(DataSort.class);
		job.setNumReduceTasks(1);//设置reduce输入文件一个，方便查看结果，如设置为0就是不执行reduce，map就输出结果
		
		job.setMapperClass(Map.class);
		job.setReducerClass(Reduce.class);
		
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(IntWritable.class);
		
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
```



   3）执行jar并查看结果

        执行：yarn jar /tmp/mr.jar /tmp/fjs/in1 /tmp/fjs/out1 

       查看：hadoop fs -text /tmp/fjs/out1/part-r-00000.bz2



3、总结：

   通过MapReduce开发之词汇统计和排序以及数据去重和排序，进一步理解Map-Reduce的计算框架，其代码处理流程通俗描述为：

   1）MapReduce对输入的文件分片并按照行提取<key,value>，作为Map的输入，其中key是每行的偏移量，value就是每一行的数据；

   2）MapReduce调用自定义的map函数来处理输入的<key,value>并以<key,value>输出加工后的数据；

   3）MapReduce开始shuffle处理map输出的<key,value>，形成<key,value-list>，汇集相同key的值为list；

   4）最后MapReduce调用自定义的reduce函数来处理输入的<key,value-list>，并以<key,value>输出结果；

   整个步骤就是：分片-map-汇聚-reduce，在框架之上定义自己的map和reduce函数来加工数据。
            


