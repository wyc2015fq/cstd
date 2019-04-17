# Hadoop-Mapreduce经典习题：字数统计 - Machine Learning with Peppa - CSDN博客





2018年07月03日 14:21:22[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：342
所属专栏：[JavaSE](https://blog.csdn.net/column/details/21699.html)









源数据与代码也放在我的GITHUB：[Hadoop-mapreduce](https://github.com/Y1ran/Hadoop-MapReduce)，欢迎fork与star

## Mapper

```java
package hadoops;

import java.io.*;
import javax.servlet.*;

import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;

import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.*;
import org.apache.hadoop.mapreduce.lib.output.*;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.fs.Path;  
import org.apache.hadoop.fs.FSDataInputStream;  
import org.apache.hadoop.fs.FSDataOutputStream;  

public class WordCountMapper extends Mapper<Object, Text, Text, IntWritable> {
    
    private static final IntWritable one = new IntWritable(1);
    private Text word = new Text();
    
    public void map(Object key, Text value, Context context) 
        throws IOException, InterruptedException{
        String[] words = value.toString().split(" ");
        
        for (String str: words)
        {
            word.set(str);
            context.write(word, one);
        }
    }
}
```

## Reducer

```java
package hadoops;

import java.io.*;
import javax.servlet.*;

import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.*;
import org.apache.hadoop.mapreduce.lib.output.*;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.fs.Path;  
import org.apache.hadoop.fs.FSDataInputStream;  
import org.apache.hadoop.fs.FSDataOutputStream;  

public class WordCountReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
    
    public void reduce(Text key, Iterable<IntWritable> values, Context context)
            throws IOException, InterruptedException{
        int total = 0;
        for(IntWritable val: values) {
            total++;
        }
        
        context.write(key, new IntWritable(total));
    }
    
}
```



## Main_Driver

```java
package hadoops;

import java.io.*;
import javax.servlet.*;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.*;
import org.apache.hadoop.mapreduce.lib.output.*;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.fs.Path;  
import org.apache.hadoop.fs.FSDataInputStream;  
import org.apache.hadoop.fs.FSDataOutputStream;  

public class WordCountDriver {

    public static void main(String[] args)  throws Exception{
        // TODO Auto-generated method stub
        if(args.length != 2) {
            
            String str = "Usage: MaxTemp<input path> <output path>";
            System.err.println(str);
            System.exit(-1);
            //exit 0;
        }
        
        Configuration conf = new Configuration();
        Job job = new Job(conf, "Word count");
        job.setJarByClass(WordCountDriver.class);
        //job.setJobName("Max Temprature");  !replaced by new job(...
        

        //private static void print(String strings) {
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        
        //setup the MepRed
        job.setMapperClass(WordCountMapper.class);
        job.setReducerClass(WordCountReducer.class);
        
        //setup the output format
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        
        //exit when job finished
        System.exit(job.waitForCompletion(true) ? 0 : 1);

     }
    }
}
```



