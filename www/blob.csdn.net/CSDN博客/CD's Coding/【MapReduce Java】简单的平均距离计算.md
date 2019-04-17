# 【MapReduce Java】简单的平均距离计算 - CD's Coding - CSDN博客





2017年05月14日 15:03:01[糖果天王](https://me.csdn.net/okcd00)阅读数：443








### 要求

没什么太多要说的，以前的MapR in Python用熟练了，Java要不是作业感觉也不会太常用它了…… 就贴一下以备忘（防止以后突然要用java写的时候可以来参考一下）
- 输入文件：文本文件 
- 每行格式 
- source _ destination _ time
- 3个部分由空格隔开
- 其中source和destination为两个字符串，内部没有空格
- time为一个浮点数，代表时间（秒为单位）
- 涵义：可以表示一次电话通话，或表示一次网站访问等

- 输入可能有噪音 
- 如果一行不符合上述格式，应该被丢弃，程序需要正确执行

- MapReduce计算：统计每对source-destination的信息
- 输出 
- source _ destination _ count _ average time
- 每一个source-destination组合输出一行（注意：顺序相反按不同处理）
- 每行输出通话次数和通话平均时间(保留3位小数，例如2.300)


### Source Code

```
/* 3, 201618013229031, ChenDian */
// Based on WordCount.java:
// Modified by Shimin Chen to demonstrate functionality for Homework 2
// April-May 2015

import java.io.IOException;
import java.util.StringTokenizer;


import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.conf.Configuration;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.DoubleWritable;

import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Reducer.Context;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import org.apache.hadoop.mapred.TextInputFormat;
import org.apache.hadoop.mapred.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class Hw2Part1 {

    // This is the Mapper class
    // reference: http://hadoop.apache.org/docs/r2.6.0/api/org/apache/hadoop/mapreduce/Mapper.html
    //
    public static class CDMapper 
        extends Mapper<Object, Text, Text, DoubleWritable>{

        private Text pair = new Text(); 
        private DoubleWritable time = new DoubleWritable();

        // (src, dest)=key, (time)=value
        private String keyStr = new String();

        public void map(Object key, Text value, Context context
                        ) throws IOException, InterruptedException {
            String delimiters = "\\s+";
            String[] tokens = value.toString().split(delimiters);

            keyStr = tokens[0] + " " + tokens[1];
            pair.set(keyStr); // Key
            time.set(Double.parseDouble(tokens[2])); // Value
            context.write(pair, time);
        }
    }

    // This is the Reducer class
    // reference http://hadoop.apache.org/docs/r2.6.0/api/org/apache/hadoop/mapreduce/Reducer.html
    //
    public static class CDReducer
        extends Reducer<Text, DoubleWritable, Text, Text> {

        private Text retKey= new Text();
        private Text retValue= new Text();

        public void reduce(Text key, Iterable<DoubleWritable> values, Context context
                           ) throws IOException, InterruptedException {
            int cnt = 0; double sum = 0.0;
            for (DoubleWritable val : values) {
                sum += val.get();
                cnt += 1;
            }

            // calculate for return value
            String vStr = new String();
            double ans = sum / (double)cnt;
            vStr = Integer.toString(cnt) + " " + String.format("%.3f", ans);

            // generate result key
            retKey.set(key);

            // generate result value
            retValue.set(vStr);

            // write answer
            context.write(retKey, retValue);
        }
    }

    public static void deletePath(String path) throws IOException{
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(conf);
        Path f = new Path(path);
        if(fs.exists(f)) fs.delete(f, true);
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
        if (otherArgs.length < 2) {
            System.err.println("Usage: Hw2Part1 <in> <out>");
            System.exit(2);
        }

        // deletePath(otherArgs[otherArgs.length - 1]) 
        // Perhaps... Teacher's script has done this...

        Job job = Job.getInstance(conf, "Mapr_Average");

        job.setJarByClass(Hw2Part1.class);
        job.setMapperClass(CDMapper.class);
        job.setReducerClass(CDReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(DoubleWritable.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        FileInputFormat.setInputPaths(job, new Path(otherArgs[0]));
        FileOutputFormat.setOutputPath(job,
            new Path(otherArgs[otherArgs.length - 1]));

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
```




