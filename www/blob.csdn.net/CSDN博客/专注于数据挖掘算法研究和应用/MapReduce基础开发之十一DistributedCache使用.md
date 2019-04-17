# MapReduce基础开发之十一DistributedCache使用 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月27日 10:48:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2102
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
1、需求场景：

   过滤无意义的单词后再进行文本词频统计。处理流程是：

1）预定义要过滤的无意义单词保存成文件，保存到HDFS中；

2）程序中将该文件定位为作业的缓存文件，使用DistributedCache类；

3）Map中读入缓存文件，对文件中的单词不做词频统计。

该场景主要解决文件在Hadoop各task之间共享的问题，用conf传递参数不能传输大文件，于是通过DistributedCache派发文件到各节点。



2、具体代码如下：



```java
package com.word;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.net.URI;
import java.util.HashSet;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.filecache.DistributedCache;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class FilterWordCount {
	public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable>{
		private final static IntWritable one = new IntWritable(1);
		private Text word = new Text();
		private HashSet<String> keyWord;
	    private Path[] localFiles;
	    //setup函数在Map task启动之后立即执行
	    public void setup(Context context) throws IOException,InterruptedException{
	    	keyWord=new  HashSet<String>();
	    	Configuration conf=context.getConfiguration();
	    	localFiles=DistributedCache.getLocalCacheFiles(conf);
	    	//将缓存文件内容读入到当前Map Task的全局变量中
	    	for(int i=0;i<localFiles.length;i++){
	    		String aKeyWord;
	    		BufferedReader br=new BufferedReader(new FileReader(localFiles[i].toString()));
	    		while((aKeyWord=br.readLine())!=null){
	    			keyWord.add(aKeyWord);
	    		}
	    		br.close();
	    	}
	    }
	    //根据缓存文件中缓存的无意义单词对输入流进行过滤
		public void map(Object key, Text value, Context context)throws IOException, InterruptedException {
			StringTokenizer itr = new StringTokenizer(value.toString());
			while (itr.hasMoreTokens()) {
				String aword=itr.nextToken();//获取字符
				if(!keyWord.contains(aword)){//不包含无意义单词
					word.set(aword);
					context.write(word, one);
				}
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
			System.err.println("Usage: FilterWordCount <in> <out>");
			System.exit(2);
		}
		//将HDFS上的文件设置成当前作业的缓存文件
		DistributedCache.addCacheFile(new URI("/tmp/fjs/kw.txt"), conf);
		
		Job job = new Job(conf, "FilterWordCount");
		job.setJarByClass(FilterWordCount.class);
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
/*
 * 统计的输入文件：hadoop fs -put /var/log/boot.log /tmp/fjs/
 * 无意义单词缓存文件：/tmp/fjs/kw.txt
 * 结果输出文件：/tmp/fjs/fwcout
 * 执行命令：hadoop jar /mnt/FilterWordCount.jar /tmp/fjs/boot.log /tmp/fjs/fwcout
 */
```





