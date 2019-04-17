# MapReduce基础开发之十二ChainMapper和ChainReducer使用 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月27日 11:31:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2075
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
1、需求场景：

   过滤无意义的单词后再进行文本词频统计。处理流程是：

1）第一个Map使用无意义单词数组过滤输入流；

2）第二个Map将过滤后的单词加上出现一次的标签；

3）最后Reduce输出词频；

MapReduce适合高吞吐高延迟的批处理，对于数据集迭代支持比较弱，唯有这个Chain具备。



2、具体代码如下：



```java
package com.word;

import java.io.IOException;
import java.util.HashSet;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.chain.ChainMapper;
import org.apache.hadoop.mapreduce.lib.chain.ChainReducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class ChainWordCount {
	//过滤无意义单词的第一个Map
	public static class FilterMapper extends Mapper<Object, Text, Text, Text>{
		private final static String[] StopWord = {"a","an","the","of","in","to","and","at","as","with"};
		private HashSet<String> StopWordSet;
		private Text word = new Text();
		//setup函数在Map task启动之后立即执行
	    public void setup(Context context) throws IOException,InterruptedException{
	    	StopWordSet=new HashSet<String>();
	    	for(int i=0;i<StopWord.length;i++){
	    		StopWordSet.add(StopWord[i]);
	    	}
	    }		
		//将 输入流中无意义的单词过滤掉
		public void map(Object key, Text value, Context context)throws IOException, InterruptedException {
			StringTokenizer itr = new StringTokenizer(value.toString());
			while (itr.hasMoreTokens()) {
				String aword=itr.nextToken();//获取字符
				if(!StopWordSet.contains(aword)){//不包含无意义单词
					word.set(aword);
					context.write(word, new Text(""));
				}
			}
		}
	}
	//记录单词标签第二个Map
	public static class TokenizerMapper extends Mapper<Text, Text, Text, IntWritable>{
		private final static IntWritable one = new IntWritable(1);	
		public void map(Text key, Text value, Context context)throws IOException, InterruptedException {
			context.write(key, one);
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
			System.err.println("Usage: ChainWordCount <in> <out>");
			System.exit(2);
		}
		
		Job job = new Job(conf, "ChainWordCount");
		job.setJarByClass(ChainWordCount.class);
	    //第一个map加入作业流
		JobConf map1Conf=new JobConf(false);
		ChainMapper.addMapper(job, FilterMapper.class, Object.class, Text.class, Text.class, Text.class, map1Conf);
		//第二个map加入作业流
		JobConf map2Conf=new JobConf(false);
		ChainMapper.addMapper(job, TokenizerMapper.class, Text.class, Text.class, Text.class, IntWritable.class, map2Conf);
	    //将词频统计的Reduce设置成作业流唯一的Reduce
		JobConf redConf=new JobConf(false);
		ChainReducer.setReducer(job, IntSumReducer.class, Text.class, IntWritable.class, Text.class, IntWritable.class, redConf);
				
		job.setNumReduceTasks(1);//设置reduce输出文件数
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
/*
 * 统计的输入文件：hadoop fs -put /var/log/boot.log /tmp/fjs/
 * 结果输出文件：/tmp/fjs/cwcout
 * 执行命令：hadoop jar /mnt/ChainWordCount.jar /tmp/fjs/boot.log /tmp/fjs/cwcout
 */
```





