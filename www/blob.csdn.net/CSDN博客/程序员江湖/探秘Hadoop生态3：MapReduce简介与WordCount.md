# 探秘Hadoop生态3：MapReduce简介与WordCount - 程序员江湖 - CSDN博客





2017年03月08日 09:40:49[黄小斜](https://me.csdn.net/a724888)阅读数：2380
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









No.10 MapReduce 编程模型极简篇

dantezhao木东居士2017-09-23

**0x00 前言**

回想自己最初学 Hadoop 的时候，初衷是写MapReduce程序，但是搭建单机环境折腾一周，搭建分布式环境折腾一周，跑个Demo解决一下Bug又一周过去了。最后都忘了自己是想学 MapReduce 的。

感觉自己虽然是搞Hadoop的，但是写MR比自己想的要少很多。初期是花了很多精力在安装以及集群的各种日常维护，熟悉Hive后就经常用Hive来解决问题，然后逐渐地各种任务过度到了Spark上，因此对MapReduce的重视就少了很多。 细想起来，MapReduce本身是很简洁易学的，因此这次抛开各种MapReduce背后的实现原理，来专门回顾一下它的编程模型。

**0x01 编程模型**

MapReduce计算提供了简洁的编程接口，对于某个计算任务来说，其输入是Key/Value数据对，输出也以Key/Value数据对方式表示。

对于应用开发者来说，只需要根据业务逻辑实现Map和Reduce两个接口函数内的具体操作内容，即可完成大规模数据的并行批处理任务。

**Map** 函数以Key/Value数据对作为输入，将输入数据经过业务逻辑计算产生若干仍旧以Key/Value形式表达的中间数据。MapReduce计算框架会自动将中间结果中具有相同Key值的记录聚合在一起，并将数据传送给Reduce函数内定义好的处理逻辑作为其输入值。

**Reduce** 函数接收到Map阶段传过来的某个Key值及其对应的若干Value值等中间数据，函数逻辑对这个Key对应的Value内容进行处理，一般是对其进行累加、过滤、转换等操作，生成Key/Value形式的结果，这就是最终的业务计算结果。

**0x02 举个栗子**

**1. 问题描述**

举个MapReduce最经典的例子——WordCount。假设给你100亿的文本内容，如何统计每个单词的总共出现次数？或者说统计出统计出现次数最多的N个单词?

这个任务看似简单，但如果在单机环境下快速完成还是需要实现技巧的，主要原因在于数据规模巨大。在MapReduce框架中实现的话就会简单很多，只要实现相应的和Map和Reduce函数即可。

**2. 代码实现**

我们用Python实现一下大致的逻辑：

**def****map**(key, value):    values = value.split(" ")    **for** v **in** values:        **print** (v, "1")**def****reduce**(key, value):    int result=0**for** v **in** value:        result += int(v)    **print** (key, result)

Map操作的key是一个文件的ID或者一个网页的ID，value是它的正文内容，即由一系列单词组成。**Map函数的主要作用是把文本内容解析成一个个单词和单词出现的次数，比如****<w,1>**。一般我们不太关注Map中的key，只解析其中的value即可。

**Reduce操作的key值为某个单词，对应的Value为出现次数列表，通过遍历相同Key的次数列表并累加其出现次数，即可获得某个单词在网页集合中总共出现的次数。**

**3. 分析**

画个整体的图，来解释一下MapReduce的过程都做了什么。

![](https://img-blog.csdn.net/20180705103705237?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 最左边是我们需要统计的文本内容，假设我们现在与三个问题，每个里面都有一些内容。
- Map阶段会有我们的Map函数来读取相应的文本，并解析出其中的单词，然后输出dantezhao 1这种结构，其中key是dantezhao，value是出现次数1。
- 然后这里会经过一个Shuffle过程，把相同key的数据放到同一个Reduce中来处理，比如我们这里会把所有key为dantezhao的数据发送到第二个Reduce中。
- Reduce阶段，根据相同的key，计算出它出现的次数，即将value值相加。

**0xFF 总结**

单纯的MapReduce编程模型其实还是不难的，当然想深入学还是有很多细节的，比如Partitioner的设计、Shuffle阶段的设计，Map和Reduce的一些优化。这里暂且做一个最基本的回顾。





Hadoop的“Hello world”---WordCount

2016-02-14

薛彬

[Hadoop](http://axuebin.com/blog/category/#Hadoop)

[Hadoop](http://axuebin.com/blog/tag/#Hadoop)[MapReduce](http://axuebin.com/blog/tag/#MapReduce)[Wordcount](http://axuebin.com/blog/tag/#Wordcount)

在安装并配置好Hadoop环境之后，需要运行一个实例来验证配置是否正确，Hadoop就提供了一个简单的wordcount程序，其实就是统计单词个数的程序，这个程序可以算是Hadoop中的“Hello World”了。

MapReduce

原理

MapReduce其实就是采用分而治之的思想，将大规模的数据分成各个节点共同完成，然后再整合各个节点的结果，得到最终的结果。这些分节点处理数据都可以做到并行处理，大大缩减了工作的复杂度。

过程

MapReduce可以分成两个阶段，其实就是单词拆成map和reduce，这其实是两个函数。map函数会产生一个中间输出，然后reduce函数接受多个map函数产生的一系列中间输出然后再产生一个最终输出。

WordCount展示

前期工作

启动hadoop

cd /usr/hadoop/hadoop-2.6.2/sbin/start-dfs.shsbin/start-yarn.sh

创建本地数据文件

cd ~/mkdir ~/filecd fileecho "Hello World" > test1.txtecho "Hello Hadoop" > test2.txt

这样就创建了两个txt文件，里面分别有一个字符串：Hello World，Hello Hadoop。我们通过wordcount想要得到的结果是这样的：Hello 2，World 1,Hadoop 1。

在HDFS上创建输入文件夹

hadoop fs -mkdir /input

创建好我们可以通过

hadoop fs -ls /

来查看结果：

将数据文件传到input目录下

hadoop fs -put ~/file/test*.txt /input

同样，我们可以通过

hadoop fs -ls /input 

来查看是否上传成功：

如果看不到任何结果，说明在hadoop的配置中存在问题，或者是防火墙没有关闭，导致节点连接不通。

运行程序

运行wordcount

hadoop jar /你的hadoop根目录/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.6.2.jar wordcount /input /output

运行这条命令后，Hadoop会启动一个JVM来运行MapReduce程序，而且会在集群上创建一个output文件夹，将结果存在其中。

我们来看看结果：

注意点：
- 这个目录一定要填对，要不然会报jar不存在。
- 输出文件夹一定要是空文件夹。

查看结果

output文件夹中现在有两个文件，我们需要的结果在part-r-00000这个文件夹中。

hadoop fs -cat /output/part-r-00000

我们就可以看到最终的wordcount结果了：

WordCount源码分析

Map过程

源码：

importjava.io.IOException;importjava.util.StringTokenizer;importorg.apache.hadoop.io.IntWritable;importorg.apache.hadoop.io.Text;importorg.apache.hadoop.mapreduce.Mapper;publicclassTokenizerMapperextendsMapper<Object,Text,Text,IntWritable>{IntWritableone=newIntWritable(1);Textword=newText();publicvoidmap(Objectkey,Textvalue,Contextcontext)throwsIOException,InterruptedException{StringTokenizeritr=newStringTokenizer(value.toString());while(itr.hasMoreTokens()){word.set(itr.nextToken());context.write(word,one);}}}

继承Mapper类，重写map方法。

我们了解到mapreduce中数据都是通过<key,value>传递的。我们可以通过控制台来看看其中的value值和key值是什么样的。在map方法中加入以下代码：

System.out.println("key= "+key.toString());//查看key值System.out.println("value= "+value.toString());//查看value值

运行程序后控制台输出如下：

我们可以看出，map方法中的value值存储的是文本文件中的一行，而key值为该行的首字符相对于文本文件的首地址的偏移量。

程序中的StringTokenizer这个类的功能是将每一行拆分成一个一个的单词，并将<word,1>作为map方法的结果输出。

Reduce过程

源码：

importjava.io.IOException;importorg.apache.hadoop.io.IntWritable;importorg.apache.hadoop.io.Text;importorg.apache.hadoop.mapreduce.Reducer;publicclassIntSumReducerextendsReducer<Text,IntWritable,Text,IntWritable>{IntWritableresult=newIntWritable();publicvoidreduce(Textkey,Iterable<IntWritable>values,Contextcontext)throwsIOException,InterruptedException{intsum=0;for(IntWritableval:values){sum+=val.get();}result.set(sum);context.write(key,result);}}

同样，Reduce过程也需要继承一个Reducer类，并重写reduce方法。

我们可以看到reduce的输入参数是Text key和Iterable<Intrable>。我们知道reduce方法的输入参数key是一个单词，而values是由各个Mapper上对应单词的计数值所组成的列表，我们可以看到values实现了一个Iterable接口，可以理解成values里面包含了多个IntWritable整数，其实也就是计数值。

然后我们只要遍历values并且求和，就可以得到各单词的总次数了。

执行MapReduce

我们已经写好了map函数和reduce函数，现在就是要执行mapreduce了。

源码：

importorg.apache.hadoop.conf.Configuration;importorg.apache.hadoop.fs.Path;importorg.apache.hadoop.io.IntWritable;importorg.apache.hadoop.io.Text;importorg.apache.hadoop.mapreduce.Job;importorg.apache.hadoop.mapreduce.lib.input.FileInputFormat;importorg.apache.hadoop.mapreduce.lib.output.FileOutputFormat;importorg.apache.hadoop.util.GenericOptionsParser;publicclassWordCount{publicstaticvoidmain(String[]args)throwsException{Configurationconf=newConfiguration();String[]otherArgs=newGenericOptionsParser(conf,args).getRemainingArgs();if(otherArgs.length!=2){System.err.println("Usage: wordcount <in> <out>");System.exit(2);}Jobjob=newJob(conf,"wordcount");job.setJarByClass(WordCount.class);job.setMapperClass(TokenizerMapper.class);job.setCombinerClass(IntSumReducer.class);job.setReducerClass(IntSumReducer.class);job.setOutputKeyClass(Text.class);job.setOutputValueClass(IntWritable.class);FileInputFormat.addInputPath(job,newPath(otherArgs[0]));FileOutputFormat.setOutputPath(job,newPath(otherArgs[1]));System.exit(job.waitForCompletion(true)?0:1);}}

代码中的job.set*()方法是为对任务的参数进行相关的设置，然后调用job.waitForCompletion()方法执行任务。

**微信公众号【Java技术江湖】一位阿里 Java 工程师的技术小站。作者黄小斜，专注 Java 相关技术：SSM、SpringBoot、MySQL、分布式、中间件、集群、Linux、网络、多线程，偶尔讲点Docker、ELK，同时也分享技术干货和学习经验，致力于Java全栈开发！**

![](https://img-blog.csdnimg.cn/20190319165741304.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=,size_16,color_FFFFFF,t_70)



