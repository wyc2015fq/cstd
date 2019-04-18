# 入门Hadoop的WordCount程序 - bigfacesafdasgfewgf - CSDN博客





2014年11月19日 10:46:42[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1279








# 入门Hadoop的WordCount程序

    本篇文章主要说两部分：简单介绍MapReduce的工作原理；详细解释WordCount程序。

## 1. MapReduce的工作原理

    在《Hadoop in action》一书中，对MapReduce计算模型有了很好的描述，在这里我们直接引用过来：

“

在 Hadoop
中，用于执行 MapReduce
任务的机器角色有两个：一个是 JobTracker；另一个是 TaskTracker， JobTracker是用于调度工作的，
 TaskTracker是用于执行工作的。一个 Hadoop集群中只有一台 JobTracker。

在分布式计算中， MapReduce
框架负责处理了并行编程中分布式存储、工作调度、负载均衡、容错均衡、容错处理以及网络通信等复杂问题，把处理过程高度抽象为两个函数：map
和 reduce，map
负责把任务分解成多个任务， reduce
负责把分解后多任务处理的结果汇总起来。

在 Hadoop
中，每个 MapReduce
任务都被初始化为一个 Job，每个 Job
又可以分为两种阶段： map
阶段和 reduce 阶段。这两个阶段分别用两个函数表示，即 map
函数和 reduce 函数。 map 函数接收一个<key,value>形式的输入，然后同样产生一个<key,value>形式的中间输出，
 Hadoop 函数接收一个如<key,(list ofvalues)>形式的输入，然后对这个 value
集合进行处理，每个 reduce
产生 0 或 1 个输出，reduce
的输出也是<key,value>形式的。

”

    从上面的解释我们可以看出：MapReduce把对大规模数据集的操作，分发给一个主节点（master）管理下的各个分节点（slaves）共同完成，然后通过整合各个节点的中间结果，得到最终结果。用 MapReduce 来处理的数据集（或任务）必须具备这样的特点：待处理的数据集可以分解成许多小的数据集，而且每一个小数据集都可以完全并行地进行处理。整个过程都是按照<key, value>的形式来输入输出。

    下图很好的描述了MapReduce的工作过程：

![](https://img-blog.csdn.net/20141119104750424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


下面我们结合一个简单的实例来说明MapReduce的内部运行流程，首先给出一个WordCount的数据流程图：

![](https://img-blog.csdn.net/20141119104850593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







    Step1：输入文件file1和file2先按照TextInputFormat形式被处理成两个InputSplit，然后输入到两个map中。此时map的输入格式为<key, value>的，需要指出的是此时的key为当前的行号（位移量），value为对应行的内容；

    Step 2：Map对每行的内容进行切词，每切下一个词就将其组织成<word, 1>的形式输出，注意每个word的value均为1；

    Step 3：把map的输出进入Reduce阶段，TaskTracker会接收到<word, {1,1,1,…}>形式的数据，reduce会进行统计频数，组织成<word, sum>的形式直接输出。

    在以上过程初始的输入文件和最终输出结果都是放在HDFS上的，但是中间的map程序只是写到本地磁盘中，而不是写到HDFS中。这是因为Map的输出在Jpb完成后即可删除了，因此不需要存储到HDFS上。虽然存储到HDFS上会比较安全，但是因为网络传输会降低MapReduce任务的执行效率，因此Map的输出文件时写在本地磁盘的。如果Map程序在没来得及将数据传送给Reduce时就崩毁了，那么JobTracker只需要另选一台机器重新执行这个Task就可以了（JobTracker是要有这个功能的，JobTracker调度任务给TaskTracker，TaskTracker执行任务的时候，会返还进行报告，JobTracker则会记录进度的进行状况，如果某个TaskTracker上的任务执行失败了，那么JobTracker会把这个任务分配给另一个TaskTracker，直到任务执行完成）。

## 2. 详解WordCount程序

WordCount程序是学习Hadoop的入门程序，我们有必要详解一下。能够完整的运行WordCount程序需要如下结果步骤：本地的文本文件上传到HDFS上，WordCount程序实现MapReduce过程，输出结果到HDFS上。

    Step 1：进入centos系统（上篇博文已经介绍过在centos6.0上搭建hadoop框架），在本地新建一个file文件夹。在file文件夹中，新建两个文本文件file1和file2. file1的内容为Hello World，file2的内容为Hello Hadoop。

    Step 2：在HDFS上创建输入文件夹，并把本地的file中文件上传到集群的input目录上；

    Step 3：在集群上运行WordCount程序，以input为输入目录，以output为输出目录；

    以上过程可以再centos的终端完成：




```
mkdir ~/file  在本地创建file文件夹
cd file

echo "Hello World" > file1.txt  在file1和file2文件中存放文本，
echo "Hello Hadoop" > file2.txt   echo命令的作用是输出内容，> file1/2就是输出到文件中

hadoop fs -mkdir input  在HDFS上创建输入目录文件夹

hadoop fs -put ~file/file*.txt input  上传本地file文件夹中的文件到HDFS上


运行WordCount程序
hadoop jar /usr/local/hadoop/hadoop-0.20.2/hadoop-0.20.2-examples.jar wordcount input output
“hadoop jar”——执行jar命令；
“/usr/local/hadoop/hadoop-0.20.2/hadoop-0.20.2-examples.jar”——WordCount所在的jar包的地址
"wordcount"  程序主类名
"input output"  输入、输出文件夹

hadoop fs -cat output/part-r-00000  查看输出文件中的内容
```


    细心的读者会发现，hadoop框架下操作命令都是以hadoop fs -*的形式。下面我们就列举一下hadoop fs -*命令：






```
1，hadoop fs –fs [local | <file system URI>]：声明hadoop使用的文件系统，如果不声明的话，使用当前配置文件配置的，按如下顺序查找：hadoop jar里的hadoop-default.xml->$HADOOP_CONF_DIR下的hadoop-default.xml->$HADOOP_CONF_DIR下的hadoop-site.xml。使用local代表将本地文件系统作为hadoop的DFS。如果传递uri做参数，那么就是特定的文件系统作为DFS。
2，hadoop fs –ls <path>：等同于本地系统的ls，列出在指定目录下的文件内容，支持pattern匹配。输出格式如filename(full path)   <r n>  size.其中n代表replica的个数，size代表大小（单位bytes）。
3，hadoop fs –lsr <path>：递归列出匹配pattern的文件信息，类似ls，只不过递归列出所有子目录信息。
4，hadoop fs –du <path>：列出匹配pattern的指定的文件系统空间总量（单位bytes），等价于unix下的针对目录的du –sb <path>/*和针对文件的du –b <path> ，输出格式如name(full path)  size(in bytes)。
5，hadoop fs –dus <path>：等价于-du，输出格式也相同，只不过等价于unix的du -sb。
<span style="color:#ff0000;">6，hadoop fs –mv <src> <dst>：将制定格式的文件 move到指定的目标位置。当src为多个文件时，dst必须是个目录。
7，hadoop fs –cp <src> <dst>：拷贝文件到目标位置，当src为多个文件时，dst必须是个目录。</span>
8<span style="color:#ff0000;">，hadoop fs –rm [-skipTrash] <src>：删除匹配pattern的指定文件，等价于unix下的rm <src>。</span>
9，hadoop fs –rmr [skipTrash] <src>：递归删掉所有的文件和目录，等价于unix下的rm –rf <src>。
10，hadoop fs –rmi [skipTrash] <src>：等价于unix的rm –rfi <src>。
<span style="color:#ff0000;">11，hadoop fs –put <localsrc> … <dst>：从本地系统拷贝文件到DFS。</span>
12，hadoop fs –copyFromLocal <localsrc> … <dst>：等价于-put。
13，hadoop fs –moveFromLocal <localsrc> … <dst>：等同于-put，只不过源文件在拷贝后被删除。
<span style="color:#ff0000;">14，hadoop fs –get [-ignoreCrc] [-crc] <src> <localdst>：从DFS拷贝文件到本地文件系统，文件匹配pattern，若是多个文件，则dst必须是目录。</span>
15，hadoop fs –getmerge <src> <localdst>：顾名思义，从DFS拷贝多个文件、合并排序为一个文件到本地文件系统。
<span style="color:#ff0000;">16，hadoop fs –cat <src>：展示文件内容。</span>
17，hadoop fs –copyToLocal [-ignoreCrc] [-crc] <src> <localdst>：等价于-get。
<span style="color:#ff0000;">18，hadoop fs –mkdir <path>：在指定位置创建目录。</span>
19，hadoop fs –setrep [-R] [-w] <rep> <path/file>：设置文件的备份级别，-R标志控制是否递归设置子目录及文件。
20，hadoop fs –chmod [-R] <MODE[,MODE]…|OCTALMODE> PATH…：修改文件的权限，-R标记递归修改。MODE为a+r,g-w,+rwx等，OCTALMODE为755这样。
21，hadoop fs -chown [-R] [OWNER][:[GROUP]] PATH…：修改文件的所有者和组。-R表示递归。
22，hadoop fs -chgrp [-R] GROUP PATH…：等价于-chown … :GROUP …。
23，hadoop fs –count[-q] <path>：计数文件个数及所占空间的详情，输出表格的列的含义依次为：DIR_COUNT,FILE_COUNT,CONTENT_SIZE,FILE_NAME或者如果加了-q的话，还会列出QUOTA,REMAINING_QUOTA,SPACE_QUOTA,REMAINING_SPACE_QUOTA。
```


    程序以及详细的注释如下：



```java
package test;
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

  public static class TokenizerMapper 
       extends Mapper<Object, Text, Text, IntWritable>{
//显然这里的Mapper<Object,Text,Text,IntWritable>是范型，其实是
  //Mapper<input_Key_Type,input_Value_Type,output_key_type,output_value_type>
  //也就是借此规定map中用到的数据类型
//这几种类型除Object之外，其它是jdk中没有的，这是hadoop对它相应的jdk中数据类型的封装，
//这里的Text相当于jdk中的String,IntWritable相当于jdk的int类型，
//这样做的原因主要是为了hadoop的数据序化而做的。  
    private final static IntWritable one = new IntWritable(1);
	//声时一个IntWritable变量，作计数用，每出现一个key，给其一个value=1的值
    private Text word = new Text();  //用来暂存map输出中的key值，Text类型的，故有此声明

	//这里就是map函数，也用到了范型，它是和Mapper抽象类中的相对应的，
	//此处的Object key,Text value的类型和上边的Object,Text是相对应的，而且最好一样，
	//不然的话，多数情况运行时会报错。
    public void map(Object key, Text value, Context context) throws IOException, 
	InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());
	  //Hadoop读入的value是以行为单位的，其key为该行所对应的行号
//因为我们要计算每个单词的数目，默认以空格作为间隔，故用StringTokenizer辅助做一下字符串的拆分，
//也可以用string.split("")来作。
      while (itr.hasMoreTokens()) {//遍历一下每行字符串中的单词，
        word.set(itr.nextToken());//出现一个单词就给它设成一个key并将其值设为1
        context.write(word, one);//输出设成的key/value值。
//以上就是打散的过程
      }
    }
  }

  public static class IntSumReducer   //reduce所在的静态类
       extends Reducer<Text,IntWritable,Text,IntWritable> {
		   //这里和Map中的作用是一样的，设定输入/输出的值的类型
    private IntWritable result = new IntWritable();
    public void reduce(Text key, Iterable<IntWritable> values, 
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
		  //由于map的打散，这里会得到如，{key,values}={"hello",{1,1,1,1,1,1,....}},这样的集合
        sum += val.get(); //这里需要逐一将它们的value取出来予以相加，取得总的出现次数，即为汇和
      }
      result.set(sum);                  //将values的和取得，并设成result对应的值
      context.write(key, result);  
	  //此时的key即为map打散之后输出的key，没有变化，
	  //变化的是result,以前得到的是一个数字的集合，此时已经//给算出和了，并做为key/value输出。
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration(); //取得系统的参数
    if (args.length != 2) {//判断一下命令行输入路径/输出路径是否齐全，即是否为两个参数
      System.err.println("Usage: wordcount <in> <out>");
      System.exit(2);  //若非两个参数，即退出
    }

	//此程序的执行，在hadoop看来是一个Job，故进行初始化job操作
    Job job = new Job(conf, "Word Count");

	//配置作业名，此程序要执行WordCount.class这个字节码文件
    job.setJarByClass(WordCount.class); 

	//配置作业的各个类
	//此处设置了使用 TokenizerMapper 完成 Map 过程中的处理
	//使用 IntSumReducer 完成 Combine 和 Reduce 过程中的处理。
	//在这个job中，用TokenizerMapper这个类的map函数
    job.setMapperClass(TokenizerMapper.class);

	//在这个job中，用IntSumReducer这个类的reduce函数
    job.setReducerClass(IntSumReducer.class); 

	 //在reduce的输出时，key的输出类型为Text
    job.setOutputKeyClass(Text.class); 

	 //在reduce的输出时,value的输出类型为IntWritable
    job.setOutputValueClass(IntWritable.class); 

//任务的输出和输入路径则由命令行参数指定，并由 FileInputFormat 和 FileOutputFormat 分别设定

	//初始化要计算word的文件的路径
    FileInputFormat.addInputPath(job, new Path(args[0]));

	//初始化要计算word的文件的之后的结果的输出路径
    FileOutputFormat.setOutputPath(job, new Path(args[1]));

	//这里就是真正的去提交job到hadoop上去执行了，
	//完成相应任务的参数设定后，即可调用 job.waitForCompletion()方法执行任务。
	//意思是指如果这个job真正的执行完了则主函数退出了，若没有真正的执行完就退出了，则为非法退出
    System.exit(job.waitForCompletion(true) ? 0 : 1);          

  }
}
```
































