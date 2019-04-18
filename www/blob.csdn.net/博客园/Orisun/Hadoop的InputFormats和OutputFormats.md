# Hadoop的InputFormats和OutputFormats - Orisun - 博客园







# [Hadoop的InputFormats和OutputFormats](https://www.cnblogs.com/zhangchaoyang/articles/2649660.html)





### InputFormat

InputFormat类用来产生InputSplit，并把它切分成record。

```
public interface InputFormat<K, V> {
        InputSplit[] getSplits(JobConf job, int numSplits) throws IOException;
        RecordReader<K, V> getRecordReader(InputSplit split,JobConf job,Reporter reporter) throws IOException;
}
```

有时候一个文件虽然大于一个block size，但是你不希望它被切分，一种办法上把mapred.min.split.size提高到比该文件的长度还要大，另一个办法是自定义FileInputFormat的子类，让isSplitable()方法返回false。

InputSplit是由客户端计算出来提交给JobTracker的，JobTracker把它存放在Configuration中，mapper可以从中获取split的信息。

map.input.file　　　String　　　　被map处理的文件的路径

map.input.start　　 long　　　　   从split的开头偏移的字节量

map.input.length　　long　　　　split的长度

### FileInputFormat

有3个类实现了InputFormat接口中：DBInputFormat, DelegatingInputFormat, FileInputFormat。FileInputFormat是所有数据来自文件的InputFormat的基类。

通过FileInputFormat可以指定输入文件有哪些，而且FileInputFormat实现了InputFormat中的getSplits()方法，getRecordReader()则需要由FileInputFormat的子类来实现。

Path可以代表一个文件、一个目录、文件或目录的集合（通过使用glob）。

如果把一个目录传递给FileInputFormat.addPath()，这不是递归的模式，即该目录下的目录是不会作为输入数据源的，并且这种情况下会引发错误。解决办法是使用glob或filter来仅选择目录下的文件。下面看一下如何使用FileInputFormat的静态方法设置filter:

```
static void setInputPathFilter(Job job, Class<? extends PathFilter> filter)
```

即使你不设置filter，默认情况下FileInputFormat也会把隐藏文件过滤掉。

你或许会问，通过调用FileInputFormat.addPath()得到了很多输入文件，FileInputFormat如何把它们切分成input split呢？事实上，FileInputFormat只切分大文件，对于小于一个HDFS block的文件它独自产生一个input split。HDFS block的大小可以通过ds.block.size来设置。

### CombineFileInputFormat

前文已经说过很多小文件带来2个麻烦：namenode上要维护很多文件的metadata，容易造成namenode内存溢出；很多小文件就会产生很多inputsplit，从而产生很多map task，而map task的启动、销毁、监控都会带来额外的开销。使用Hadoop Archive(HAR)可以解决第一个问题，但不能解决第2个问题，因为对MapReduce来说看到的还归档前的文件。使用SequenceFile不仅可以合并文件，还有压缩的作用，节省磁盘空间。但是如果你现在已经有了诸多小文件，在进行MapReduce之前把它们合并成一个SequenceFile可能还得不偿失，这时候你可以用CombineFileInputFormat。CombineFileInputFormat并没的把原文件合并，它只是对于MapReduce来说产生了较少的InputSplit（通过把多个文件打包到一个inputsplit中--当然是把邻近的文件打包进同一个split，这样map task离它要处理的文件都比较近）。使用CombineFileInputFormat时你可能需要调节split的大小，一个split的最大（默认为Long.MAX_VALUE）和最小长度(默认为1B)可以通过mapred.min.split.size和mapred.max,split.size来设置。

CombineFileInputFormat是抽象类，它里面有一个抽象方法需要子类来实现：

```
abstract  RecordReader<K,V>    createRecordReader(InputSplit split, TaskAttemptContext context)
```

### 自定义InputFormat

TextInputFormat是默认的InputFormat，一行为一个record，key是该行在文件中的偏移量，value是该行的内容。

经常我们需要自定义InputFormat。我们希望一行为一个record，但key不是该在文件中的偏移量，而是行号，下面的代码是读取存储了一个二维矩阵的文件，我们需要知道每行的行号，从而知道矩阵中的行索引。

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


package basic;

import java.io.IOException;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.JobContext;
import org.apache.hadoop.mapreduce.RecordReader;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;

public class MatrixInputFormat extends FileInputFormat<IntWritable,Text>{

    @Override
    public RecordReader<IntWritable, Text> createRecordReader(
            InputSplit split, TaskAttemptContext context)
            throws IOException, InterruptedException {
        return new MatrixLineRecordReader();
    }
    
    /*因为读入时要记录行号，所以要保证中有一个mapper，这样行号才是一致的*/
    @Override
    protected boolean isSplitable(JobContext context, Path filename){
        return false;
    }

}
```

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


package basic;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.RecordReader;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.util.LineReader;

public class MatrixLineRecordReader extends RecordReader<IntWritable, Text> {

    private LineReader in;
    private int lineno=0;
    private boolean more=true;
    private IntWritable key=null;
    private Text value=null;
    
    private Log LOG=LogFactory.getLog(MatrixLineRecordReader.class);
    
    @Override
    public void initialize(InputSplit inputSplit,
            TaskAttemptContext context) throws IOException, InterruptedException {
        FileSplit split=(FileSplit)inputSplit;
        Configuration conf=context.getConfiguration();
        Path file=split.getPath();
        FileSystem fs=file.getFileSystem(conf);
        FSDataInputStream fileIn=fs.open(file);
        in=new LineReader(fileIn,conf);
    }

    @Override
    public boolean nextKeyValue() throws IOException, InterruptedException {
        LOG.info("line number is "+lineno);
        if(key==null)
            key=new IntWritable();
        if(value==null)
            value=new Text();
        int readsize=in.readLine(value);
        LOG.info("line content is "+value.toString());
        if(readsize==0){
            more=false;
            return false;
        }
        key.set(lineno);
        lineno++;
        return true;
    }
    
    @Override
    public IntWritable getCurrentKey() throws IOException, InterruptedException {
        return key;
    }

    @Override
    public Text getCurrentValue() throws IOException,
            InterruptedException {
        return value;
    }

    @Override
    public float getProgress() throws IOException, InterruptedException {
        if(more)
            return 0.0f;
        else
            return 100f;
    }
    
    @Override
    public void close() throws IOException {
        in.close();
    }
}
```

有一个细节问题：如果一个split刚好从一个record的中间切开怎么办？放心，这种情况下split会自动扩容以容纳下最后一条record，也就是说split会比block size略长。

### Multiple Inputs

有时数据来源于不同的文件，它们包含的信息相同，但格式（InputFormat）不同，我们需要用不同的RecordReader来解析。这时可以调用MultipleInputs的静态方法：

```
static void    addInputPath(Job job, Path path, Class<? extends InputFormat> inputFormatClass)
```

甚至有时候对不同的数据来源需要使用不同的map来处理：

```
static void    addInputPath(Job job, Path path, Class<? extends InputFormat> inputFormatClass, Class<? extends Mapper> mapperClass)
```

注意不同的map output要有相同的类型，因为它们要被同一个reducer处理，且这些mapper Emit(key,value)的顺序是不可预知的。这时候就不要再调用FileInputFormat.addInputPath()和job.setMpperClass()了。

### 自定义OutputFormat

TextOutputFormat是默认的OutputFormat，每条record占一行，其key 和value都是Text类型，用tab隔开（这个分隔符可以通过mapred.textoutputformat.separator设置）。你还可设置key或者value为空，只要设为类型NullWritable（这是一个单例模式）就可以了，此时分隔符也就不会被输出了。

一般我们并不需要实现一个FileOutputFormat的子类（这之间还要实现一个RecordWriter的子类），而只需要实现一个Writable的子类，重写它的toString()方法就可以了--这种方法确实更简单一些。

### Multiple Outputs

一般情况下一个reducer产生一个输出文件（文件大小与block size无关），命名为part-r-xxxxx，xxxxx是从0开始的序号。FileOutputFormat及其子类产生的输出文件都在同一个目录下。有时候你可能想控制输出文件的名称，或者想让一个reducer产生多个输出文件。

**MultipleOutputFormat（按行划分到不同的文件）**

假如MapReduce输出了如下的一些record：

```
Jim   Class1    male
Lili     Class2    female
Tom   Class1   male
Mei     Class2    female
```

我们想按class把record输出到不同的文件，只需要自定义一个MultipleOutputFormat类，然后把设置为OutputFormat就可以了。

```
public class PertitionByClass extends MultipleTextOutputFormat<NullWritable,Text>{
    @Override
    protected String generateFileNameForKeyValue(NullWritable key,Text value,String filenmae){
        String []arr=value.toString().split("\\s+");
        return arr[1];
    }
}
```

```
job.setOutputFormat(PertitionByClass);
```

MultipleTextOutputFormat继承自MultipleOutputFormat。

写这篇博客时间比较久了，MultipleTextOutputFormat只在hadoop0.21之前的版本能用，在新版中需要用MultipleOutputs，具体参见[军哥的博客](http://my.oschina.net/leejun2005/blog/94706)。

**MultipleOutputs（**按列分到不同的文件**）**

如果想输出为2个文件，一个文件存放“姓名，班级”，另一个文件存放“姓名，性别”。

在Reducer中

```
public class MyReducer extends Reducer<NullWritable, Text, NullWritable, Text> {

    private MultipleOutputs<NullWritable, Text>  mos;
    @Override
    public void setup(Context context){
        mos=new MultipleOutputs<NullWritable, Text>(context);
    }
    @Override
    public void reduce(NullWritable key,Iterable<Text> values,Context context)throws IOException,InterruptedException{
        while(values.iterator().hasNext()){
            Text text=values.iterator().next();
            String []arr=text.toString().split("\\s+");
            String nc=arr[0]+","+arr[1];　　　　//姓名，班级
            String nf=arr[0]+","+arr[2];　　　　//姓名，性别
            mos.write("nc", NullWritable.get(), new Text(nc));
            mos.write("nf", NullWritable.get(), new Text(nf));
        }
    }
    @Override
    public void cleanup(Context context)throws IOException,InterruptedException{
        mos.close();
    }
}
```

然后在driver中调用MultipleOutputs的静态方法addNamedOutput()

```
MultipleOutputs.addNamedOutput(job,"nc",TextOutputFormat.class,NullWritable.class,Text.class);
MultipleOutputs.addNamedOutput(job,"nf",TextOutputFormat.class,NullWritable.class,Text.class);
```

输出文件名中包含"nc"或"nf"作为区别。

解释一下上面用到的几个函数

```
static void    addNamedOutput(Job job, String namedOutput, Class<? extends OutputFormat> outputFormatClass, Class<?> keyClass, Class<?> valueClass) 
Adds a named output for the job.

<K,V> void  write(String namedOutput, K key, V value) 
Write key and value to the namedOutput.

<K,V> void  write(String namedOutput, K key, V value, String baseOutputPath) 
Write key and value to baseOutputPath using the namedOutput.
```












