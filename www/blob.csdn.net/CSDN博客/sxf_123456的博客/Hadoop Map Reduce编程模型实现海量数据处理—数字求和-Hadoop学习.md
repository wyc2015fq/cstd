# Hadoop Map/Reduce编程模型实现海量数据处理—数字求和-Hadoop学习 - sxf_123456的博客 - CSDN博客
2017年04月25日 13:27:15[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：583
## Hadoop Map/Reduce编程模型实现海量数据处理—数字求和
  Map/Reduce编程模型型的原理是：利用一个输入key/value pair 集合来产生一个输出的key/value pair 集合。MapReduce
库的用户用两个函数表达这个计算：Map 和Reduce。Hadoop Map/Reduce实现主要是通过继承Mapper和Reducer两个抽象类，并实现map和reduce两个方法实现的。
#### Mapper
[Mapper](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/Mapper.html) 将输入键值对(key/value pair)映射到一组中间格式的键值对集合。
Map是一类将输入记录集转换为中间格式记录集的独立任务。 这种转换的中间格式记录集不需要与输入记录集的类型一致。一个给定的输入键值对可以映射成0个或多个输出键值对。
输出键值对不需要与输入键值对的类型一致。一个给定的输入键值对可以映射成0个或多个输出键值对。
Mapper 的输出被排序后，就被划分给每个Reducer 。分块的总数目和一个作业的reduce任务的数目是一样的。用户可以通过实现自定义的
Partitioner 来控制哪个key被分配给哪个 Reducer 。
用户可选择通过[JobConf.setCombinerClass(Class)](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/JobConf.html#setCombinerClass%28java.lang.Class%29) 指定一个combiner ，它负责对中间过程的输出进行本地的聚集，这会有助于降低从Mapper
 到 Reducer 数据传输量。
这些被排好序的中间过程的输出结果保存的格式是(key-len, key, value-len, value)，应用程序可以通过JobConf 控制对这些中间结果是否进行压缩以及怎么压缩，使用哪种[CompressionCodec](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/io/compress/CompressionCodec.html)
 。 
##### 需要多少个Map？
Map的数目通常是由输入数据的大小决定的，一般就是所有输入文件的总块（block）数。
Map正常的并行规模大致是每个节点（node）大约10到100个map，对于CPU 消耗较小的map任务可以设到300个左右。由于每个任务初始化需要一定的时间，因此，比较合理的情况是map执行的时间至少超过1分钟。
这样，如果你输入10TB的数据，每个块（block）的大小是128MB，你将需要大约82,000个map来完成任务，除非使用 [setNumMapTasks(int)](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/JobConf.html#setNumMapTasks%28int%29) （注意：这里仅仅是对框架进行了一个提示(hint)，实际决定因素见[这里](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/JobConf.html#setNumMapTasks%28int%29) ）将这个数值设置得更高。
#### Reducer
[Reducer](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/Reducer.html) 将与一个key关联的一组中间数值集归约（reduce）为一个更小的数值集。
用户可以通过[JobConf.setNumReduceTasks(int)](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/JobConf.html#setNumReduceTasks%28int%29) 设定一个作业中reduce任务的数目。
概括地说，对Reducer 的实现者需要重写 [JobConfigurable.configure(JobConf)](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/JobConfigurable.html#configure%28org.apache.hadoop.mapred.JobConf%29) 方法，这个方法需要传递一个JobConf 参数，目的是完成Reducer的初始化工作。然后，框架为成组的输入数据中的每个<key, (list of values)> 对调用一次
[reduce(WritableComparable, Iterator, OutputCollector, Reporter)](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/Reducer.html#reduce%28K2,%20java.util.Iterator,%20org.apache.hadoop.mapred.OutputCollector,%20org.apache.hadoop.mapred.Reporter%29) 方法。之后，应用程序可以通过重写[Closeable.close()](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/io/Closeable.html#close%28%29) 来执行相应的清理工作。
Reducer 有3个主要阶段：shuffle、sort和reduce。 
##### Shuffle
Reducer 的输入就是Mapper已经排好序的输出。在这个阶段，框架通过HTTP为每个Reducer获得所有Mapper输出中与之相关的分块。
##### Sort
这个阶段，框架将按照key的值对Reducer 的输入进行分组 （因为不同mapper的输出中可能会有相同的key）。
Shuffle和Sort两个阶段是同时进行的；map的输出也是一边被取回一边被合并的。
##### 需要多少个Reduce？
Reduce的数目建议是0.95 或1.75 乘以 (<*no. of nodes* > *
mapred.tasktracker.reduce.tasks.maximum )。 
用0.95，所有reduce可以在maps一完成时就立刻启动，开始传输map的输出结果。用1.75，速度快的节点可以在完成第一轮reduce任务后，可以开始第二轮，这样可以得到比较好的负载均衡的效果。
增加reduce的数目会增加整个框架的开销，但可以改善负载均衡，降低由于执行失败带来的负面影响。
上述比例因子比整体数目稍小一些是为了给框架中的推测性任务（speculative-tasks） 或失败的任务预留一些reduce的资源。
##### 无Reducer
如果没有归约要进行，那么设置reduce任务的数目为*零* 是合法的。
这种情况下，map任务的输出会直接被写入由 [setOutputPath(Path)](http://hadoop.apache.org/core/docs/r0.18.2/api/org/apache/hadoop/mapred/FileOutputFormat.html#setOutputPath%28org.apache.hadoop.mapred.JobConf,%20org.apache.hadoop.fs.Path%29) 指定的输出路径。框架在把它们写入FileSystem 之前没有对它们进行排序。
知道了Map/Reduce相关基础知识，现在我们要做的事，就是对一个包含有海量数字的文本文件进行统计，并求出所有数字的和。
例子：对包含有1*10^6(100000)个数字文件，进行分析并求和。
文件格式：
-50 43 20 58 40 64 -95 28 61 55
38 78 -28 96 35 2 3 4 -87 22
-22 63 40 93 -58 81 72 63 93 94
-48 77 40 42 35 86 -66 43 26 70
-21 45 -14 6 21 73 96 31 -90 57
解决思路：
第一种方法是用Mapper读取文本文件用StringTokenizer对读取文件内的每一行的数字（Hadoop处理文本文件时，处理时是一行一行记取的）进行分隔，获取每一个数字，然后求和,再将求得的值按Key/Value格式写入Context，最后用Reducer对求得中间值进行汇总求和，得出整个文件所有数字的和。
第二种方法是用Mapper读取文本文件用StringTokenizer对文件内的数字进行分隔，获取每一个数字，并救出文件中该数字有多少个，在合并过程中，求出每个数字在文件中的和，最后用Reducer对求得每个数字求得的和进行汇总求和，得出整个文件所有数字的和。
下面就是实现的具体代码>>
第一种实现代码：
package com.metarnet.hadoop;
import java.io.IOException;
import java.util.StringTokenizer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;
public class NumberSum {
	
	
	//对每一行数据进行分隔，并求和
	public static class SumMapper extends
			Mapper<Object, Text, Text, LongWritable> {
		private Text word = new Text("sum");
		private static LongWritable numValue = new LongWritable(1);
		public void map(Object key, Text value, Context context)
				throws IOException, InterruptedException {
			StringTokenizer itr = new StringTokenizer(value.toString());
			long sum = 0;
			while (itr.hasMoreTokens()) {
				String s = itr.nextToken();
				long val = Long.parseLong(s);
				sum += val;
			}
			numValue.set(sum);
			context.write(word, numValue);
		}
	}
	
	// 汇总求和，输出
	public static class SumReducer extends
			Reducer<Text, LongWritable, Text, LongWritable> {
		private LongWritable result = new LongWritable();
		private Text k = new Text("sum");
		public void reduce(Text key, Iterable<LongWritable> values,
				Context context) throws IOException, InterruptedException {
			long sum = 0;
			for (LongWritable val : values) {
				long v = val.get();
				sum += v;
			}
			result.set(sum);
			context.write(k, result);
		}
	}
	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args)
				.getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: numbersum <in> <out>");
			System.exit(2);
		}
		Job job = new Job(conf, "number sum");
		 job.setJarByClass(NumberSum.class);
		job.setMapperClass(SumMapper.class);
		job.setReducerClass(SumReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(LongWritable.class);
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
		System.out.println("ok");
	}
}
第二种实现代码：
package com.metarnet.hadoop;
import java.io.IOException;
import java.util.StringTokenizer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;
public class NumberSum {
	
	//对每一个数字进行分隔
	public static class NumSumMapper extends
			Mapper<Object, Text, Text, LongWritable> {
		private Text word = new Text();
		private static LongWritable numValue = new LongWritable(1);
		public void map(Object key, Text value, Context context)
				throws IOException, InterruptedException {
			StringTokenizer itr = new StringTokenizer(value.toString());
			long sum = 0;
			while (itr.hasMoreTokens()) {
				String s = itr.nextToken();
				word.set(s);
				context.write(word, numValue);
			}
		}
	}
	
	//对每一个数字进行汇总求和
	public static class SumCombiner extends
			Reducer<Text, LongWritable, Text, LongWritable> {
		private LongWritable result = new LongWritable();
		private Text k = new Text("midsum");
		public void reduce(Text key, Iterable<LongWritable> values,
				Context context) throws IOException, InterruptedException {
			long sum = 0;
			if (!key.toString().startsWith("midsum")) {
				for (LongWritable val : values) {
					sum += val.get();
				}
				long kval = Long.parseLong(key.toString());
				long v = kval * sum;
				result.set(v);
				context.write(k, result);
			} else {
				for (LongWritable val : values) {
					context.write(key, val);
				}
			}
		}
	}
	// 汇总求和，输出
	public static class SumReducer extends
			Reducer<Text, LongWritable, Text, LongWritable> {
		private LongWritable result = new LongWritable();
		private Text k = new Text("sum");
		public void reduce(Text key, Iterable<LongWritable> values,
				Context context) throws IOException, InterruptedException {
			long sum = 0;
			for (LongWritable val : values) {
				long v = val.get();
				sum += v;
			}
			result.set(sum);
			context.write(k, result);
		}
	}
	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args)
				.getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: numbersum <in> <out>");
			System.exit(2);
		}
		Job job = new Job(conf, "number sum");
		 job.setJarByClass(NumberSum.class);
		job.setMapperClass(NumSumMapper.class);
		job.setCombinerClass(SumCombiner.class);
		job.setReducerClass(SumReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(LongWritable.class);
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
		System.out.println("ok");
	}
}
第一种实现的方法相对简单，第二种实现方法用到了Combiner类，Hadoop 在
对中间求的的数值进行Combiner时，是通过递归的方式不停地对Combiner方法进行调用，进而合并数据的。
从两种方法中，我们可以看出Map/Reduce的核心是在怎样对输入的数据按照何种方式是进行Key/Value分对的，分的合理对整个结果输出及算法实现有很大的影响。
