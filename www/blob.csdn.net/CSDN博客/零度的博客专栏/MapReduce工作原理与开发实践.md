# MapReduce工作原理与开发实践 - 零度的博客专栏 - CSDN博客
2017年06月22日 09:45:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：503
                
下面我一张图,便于理解MapReduce得整个工作原理
![](https://img-blog.csdn.net/20160428165418734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下面对上面出现的一些名词进行介绍
**ResourceManager：**是YARN资源控制框架的中心模块，负责集群中所有的资源的统一管理和分配。它接收来自NM(NodeManager)的汇报，建立AM，并将资源派送给AM(ApplicationMaster)。
**NodeManager**:简称NM，NodeManager是ResourceManager在每台机器的上代理，负责容器的管理，并监控他们的资源使用情况（cpu，内存，磁盘及网络等），以及向 ResourceManager提供这些资源使用报告。
**ApplicationMaster**:以下简称AM。YARN中每个应用都会启动一个AM，负责向RM申请资源，请求NM启动container，并告诉container做什么事情。
**Container**：资源容器。YARN中所有的应用都是在container之上运行的。AM也是在container上运行的，不过AM的container是RM申请的。
1.  Container是YARN中资源的抽象，它封装了某个节点上一定量的资源（CPU和内存两类资源）。
2.  Container由ApplicationMaster向ResourceManager申请的，由ResouceManager中的资源调度器异步分配给ApplicationMaster；
3. Container的运行是由ApplicationMaster向资源所在的NodeManager发起的，Container运行时需提供内部执行的任务命令（可以是任何命令，比如java、Python、C++进程启动命令均可）以及该命令执行所需的环境变量和外部资源（比如词典文件、可执行文件、jar包等）。
另外，一个应用程序所需的Container分为两大类，如下：
       （1） 运行ApplicationMaster的Container：这是由ResourceManager（向内部的资源调度器）申请和启动的，用户提交应用程序时，可指定唯一的ApplicationMaster所需的资源；
       （2） 运行各类任务的Container：这是由ApplicationMaster向ResourceManager申请的，并由ApplicationMaster与NodeManager通信以启动之。
以上两类Container可能在任意节点上，它们的位置通常而言是随机的，即ApplicationMaster可能与它管理的任务运行在一个节点上。
**整个MapReduce的过程大致分为 Map-->Shuffle（排序）-->Combine（组合）-->Reduce**
下面通过一个单词计数案例来理解各个过程
1）将文件拆分成splits(片)，并将每个split按行分割形成<key,value>对，如图所示。这一步由MapReduce框架自动完成，其中偏移量即key值
![](https://img-blog.csdn.net/20160428165500313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                    分割过程
将分割好的<key,value>对交给用户定义的map方法进行处理，生成新的<key,value>对，如下图所示。
![](https://img-blog.csdn.net/20160428165513626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                    执行map方法
得到map方法输出的<key,value>对后，Mapper会将它们按照key值进行Shuffle（排序），并执行Combine过程，将key至相同value值累加，得到Mapper的最终输出结果。如下图所示。
![](https://img-blog.csdn.net/20160428165527530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                     Map端排序及Combine过程
Reducer先对从Mapper接收的数据进行排序，再交由用户自定义的reduce方法进行处理，得到新的<key,value>对，并作为WordCount的输出结果，如下图所示。
![](https://img-blog.csdn.net/20160428165545924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                    Reduce端排序及输出结果
下面看怎么用Java来实现WordCount单词计数的功能
首先看Map过程
Map过程需要继承org.apache.hadoop.mapreduce.Mapper包中 Mapper 类，并重写其map方法。
**[java]**[view plain](http://blog.csdn.net/tanggao1314/article/details/51275812#)[copy](http://blog.csdn.net/tanggao1314/article/details/51275812#)[print](http://blog.csdn.net/tanggao1314/article/details/51275812#)[?](http://blog.csdn.net/tanggao1314/article/details/51275812#)
- /**
-      *
-      * @author 
-      *  Mapper<LongWritable, Text, Text, IntWritable>中  LongWritable,IntWritable是Hadoop数据类型表示长整型和整形
-      *
-      *  LongWritable, Text表示输入类型 (比如本应用单词计数输入是 偏移量(字符串中的第一个单词的其实位置),对应的单词(值))
-      * Text, IntWritable表示输出类型  输出是单词  和他的个数
-      *  注意：map函数中前两个参数LongWritable key, Text value和输出类型不一致
-      *      所以后面要设置输出类型 要使他们一致
-      */
- //Map过程
- publicstaticclass WordCountMapper extends Mapper<LongWritable, Text, Text, IntWritable> {  
- /***
-          *
-          */
- @Override
- protectedvoid map(LongWritable key, Text value, Mapper<LongWritable, Text, Text, IntWritable>.Context context)  
- throws IOException, InterruptedException {  
- //默认的map的value是每一行,我这里自定义的是以空格分割
-             String[] vs = value.toString().split("\\s");  
- for (String v : vs) {  
- //写出去
-                 context.write(new Text(v), ONE);  
-             }  
- 
-         }  
-     }  
```java
/**
     *
     * @author 汤高
     *    Mapper<LongWritable, Text, Text, IntWritable>中  LongWritable,IntWritable是Hadoop数据类型表示长整型和整形
     *
     *    LongWritable, Text表示输入类型 (比如本应用单词计数输入是 偏移量(字符串中的第一个单词的其实位置),对应的单词(值))
     *    Text, IntWritable表示输出类型  输出是单词  和他的个数
     *  注意：map函数中前两个参数LongWritable key, Text value和输出类型不一致
     *      所以后面要设置输出类型 要使他们一致
     */
    //Map过程
    public static class WordCountMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
        /***
         *
         */
        @Override
        protected void map(LongWritable key, Text value, Mapper<LongWritable, Text, Text, IntWritable>.Context context)
                throws IOException, InterruptedException {
            //默认的map的value是每一行,我这里自定义的是以空格分割
            String[] vs = value.toString().split("\\s");
            for (String v : vs) {
                //写出去
                context.write(new Text(v), ONE);
            }
        }
    }
```
Reduce过程
Reduce过程需要继承org.apache.hadoop.mapreduce包中 Reducer 类，并 重写 其reduce方法。Map过程输出<key,values>中key为单个单词，而values是对应单词的计数值所组成的列表，Map的输出就是Reduce的输入，所以reduce方法只要遍历values并求和，即可得到某个单词的总次数。
**[java]**[view plain](http://blog.csdn.net/tanggao1314/article/details/51275812#)[copy](http://blog.csdn.net/tanggao1314/article/details/51275812#)[print](http://blog.csdn.net/tanggao1314/article/details/51275812#)[?](http://blog.csdn.net/tanggao1314/article/details/51275812#)
- //Reduce过程
- /***
-      * @author 
-      * Text, IntWritable输入类型,从map过程获得 既map的输出作为Reduce的输入
-      * Text, IntWritable输出类型
-      */
- publicstaticclass WordCountReducer extends Reducer<Text, IntWritable, Text, IntWritable>{  
- @Override
- protectedvoid reduce(Text key, Iterable<IntWritable> values,  
-                 Reducer<Text, IntWritable, Text, IntWritable>.Context context) throws IOException, InterruptedException {  
- int count=0;  
- for(IntWritable v:values){  
-                 count+=v.get();//单词个数加一
-             }  
- 
-             context.write(key, new IntWritable(count));  
-         }  
- 
-     }  
```java
//Reduce过程
    /***
     * @author 汤高
     * Text, IntWritable输入类型,从map过程获得 既map的输出作为Reduce的输入
     * Text, IntWritable输出类型
     */
    public static class WordCountReducer extends Reducer<Text, IntWritable, Text, IntWritable>{
        @Override
        protected void reduce(Text key, Iterable<IntWritable> values,
                Reducer<Text, IntWritable, Text, IntWritable>.Context context) throws IOException, InterruptedException {
            int count=0;
            for(IntWritable v:values){
                count+=v.get();//单词个数加一
            }
            
            context.write(key, new IntWritable(count));
        }
        
    }
```
最后执行MapReduce任务
**[java]**[view plain](http://blog.csdn.net/tanggao1314/article/details/51275812#)[copy](http://blog.csdn.net/tanggao1314/article/details/51275812#)[print](http://blog.csdn.net/tanggao1314/article/details/51275812#)[?](http://blog.csdn.net/tanggao1314/article/details/51275812#)
- publicstaticvoid main(String[] args) {  
- 
-         Configuration conf=new Configuration();  
- try {  
- //args从控制台获取路径 解析得到域名
-             String[] paths=new GenericOptionsParser(conf,args).getRemainingArgs();  
- if(paths.length<2){  
- thrownew RuntimeException("必須输入文件输入和输出路径参数");  
-             }  
- //得到一个Job 并设置名字
-             Job job=Job.getInstance(conf,"wordcount");  
- //设置Jar 使本程序在Hadoop中运行
-             job.setJarByClass(WordCount.class);  
- //设置Map处理类
-             job.setMapperClass(WordCountMapper.class);  
- //设置map的输出类型,因为不一致,所以要设置
-             job.setMapOutputKeyClass(Text.class);  
-             job.setOutputValueClass(IntWritable.class);  
- //设置Reduce处理类
-             job.setReducerClass(WordCountReducer.class);  
- //设置输入和输出目录
-             FileInputFormat.addInputPath(job, new Path(paths[0]));  
-             FileOutputFormat.setOutputPath(job, new Path(paths[1]));  
- //启动运行
-             System.exit(job.waitForCompletion(true) ? 0:1);  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         } catch (ClassNotFoundException e) {  
-             e.printStackTrace();  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
-     }  
```java
public static void main(String[] args) {
        
        Configuration conf=new Configuration();
        try {
            //args从控制台获取路径 解析得到域名
            String[] paths=new GenericOptionsParser(conf,args).getRemainingArgs();
            if(paths.length<2){
                throw new RuntimeException("必須輸出 輸入 和输出路径");
            }
            //得到一个Job 并设置名字
            Job job=Job.getInstance(conf,"wordcount");
            //设置Jar 使本程序在Hadoop中运行
            job.setJarByClass(WordCount.class);
            //设置Map处理类
            job.setMapperClass(WordCountMapper.class);
            //设置map的输出类型,因为不一致,所以要设置
            job.setMapOutputKeyClass(Text.class);
            job.setOutputValueClass(IntWritable.class);
            //设置Reduce处理类
            job.setReducerClass(WordCountReducer.class);
            //设置输入和输出目录
            FileInputFormat.addInputPath(job, new Path(paths[0]));
            FileOutputFormat.setOutputPath(job, new Path(paths[1]));
            //启动运行
            System.exit(job.waitForCompletion(true) ? 0:1);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
```
即可求得每个单词的个数
下面把整个过程的源码附上,有需要的朋友可以拿去测试
**[java]**[view plain](http://blog.csdn.net/tanggao1314/article/details/51275812#)[copy](http://blog.csdn.net/tanggao1314/article/details/51275812#)[print](http://blog.csdn.net/tanggao1314/article/details/51275812#)[?](http://blog.csdn.net/tanggao1314/article/details/51275812#)
- package hadoopday02;  
- 
- import java.io.IOException;  
- 
- import org.apache.hadoop.conf.Configuration;  
- import org.apache.hadoop.fs.Path;  
- import org.apache.hadoop.io.IntWritable;  
- import org.apache.hadoop.io.LongWritable;  
- 
- import org.apache.hadoop.io.Text;  
- import org.apache.hadoop.mapreduce.Job;  
- import org.apache.hadoop.mapreduce.Mapper;  
- import org.apache.hadoop.mapreduce.Reducer;  
- import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;  
- import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;  
- import org.apache.hadoop.util.GenericOptionsParser;  
- 
- publicclass WordCount {  
- //计数变量
- privatestaticfinal IntWritable ONE = new IntWritable(1);  
- /**
-      *
-      * @author 
-      *  Mapper<LongWritable, Text, Text, IntWritable>中  LongWritable,IntWritable是Hadoop数据类型表示长整型和整形
-      *
-      * LongWritable, Text表示输入类型 (比如本应用单词计数输入是 偏移量(字符串中的第一个单词的其实位置),对应的单词(值))
-      * Text, IntWritable表示输出类型  输出是单词  和他的个数
-      *  注意：map函数中前两个参数LongWritable key, Text value和输出类型不一致
-      *      所以后面要设置输出类型 要使他们一致
-      */
- //Map过程
- publicstaticclass WordCountMapper extends Mapper<LongWritable, Text, Text, IntWritable> {  
- /***
-          *
-          */
- @Override
- protectedvoid map(LongWritable key, Text value, Mapper<LongWritable, Text, Text, IntWritable>.Context context)  
- throws IOException, InterruptedException {  
- //默认的map的value是每一行,我这里自定义的是以空格分割
-             String[] vs = value.toString().split("\\s");  
- for (String v : vs) {  
- //写出去
-                 context.write(new Text(v), ONE);  
-             }  
- 
-         }  
-     }  
- //Reduce过程
- /***
-      * @author 
-      * Text, IntWritable输入类型,从map过程获得 既map的输出作为Reduce的输入
-      * Text, IntWritable输出类型
-      */
- publicstaticclass WordCountReducer extends Reducer<Text, IntWritable, Text, IntWritable>{  
- @Override
- protectedvoid reduce(Text key, Iterable<IntWritable> values,  
-                 Reducer<Text, IntWritable, Text, IntWritable>.Context context) throws IOException, InterruptedException {  
- int count=0;  
- for(IntWritable v:values){  
-                 count+=v.get();//单词个数加一
-             }  
- 
-             context.write(key, new IntWritable(count));  
-         }  
- 
-     }  
- 
- publicstaticvoid main(String[] args) {  
- 
-         Configuration conf=new Configuration();  
- try {  
- //args从控制台获取路径 解析得到域名
-             String[] paths=new GenericOptionsParser(conf,args).getRemainingArgs();  
- if(paths.length<2){  
- thrownew RuntimeException("必須输入文件输入和输出路径参数");  
-             }  
- //得到一个Job 并设置名字
-             Job job=Job.getInstance(conf,"wordcount");  
- //设置Jar 使本程序在Hadoop中运行
-             job.setJarByClass(WordCount.class);  
- //设置Map处理类
-             job.setMapperClass(WordCountMapper.class);  
- //设置map的输出类型,因为不一致,所以要设置
-             job.setMapOutputKeyClass(Text.class);  
-             job.setOutputValueClass(IntWritable.class);  
- //设置Reduce处理类
-             job.setReducerClass(WordCountReducer.class);  
- //设置输入和输出目录
-             FileInputFormat.addInputPath(job, new Path(paths[0]));  
-             FileOutputFormat.setOutputPath(job, new Path(paths[1]));  
- //启动运行
-             System.exit(job.waitForCompletion(true) ? 0:1);  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         } catch (ClassNotFoundException e) {  
-             e.printStackTrace();  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- 
- }  
```java
package hadoopday02;
import java.io.IOException;
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
public class WordCount {
    //计数变量
    private static final IntWritable ONE = new IntWritable(1);
    /**
     *
     * @author 汤高
     *    Mapper<LongWritable, Text, Text, IntWritable>中  LongWritable,IntWritable是Hadoop数据类型表示长整型和整形
     *
     *    LongWritable, Text表示输入类型 (比如本应用单词计数输入是 偏移量(字符串中的第一个单词的其实位置),对应的单词(值))
     *    Text, IntWritable表示输出类型  输出是单词  和他的个数
     *  注意：map函数中前两个参数LongWritable key, Text value和输出类型不一致
     *      所以后面要设置输出类型 要使他们一致
     */
    //Map过程
    public static class WordCountMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
        /***
         *
         */
        @Override
        protected void map(LongWritable key, Text value, Mapper<LongWritable, Text, Text, IntWritable>.Context context)
                throws IOException, InterruptedException {
            //默认的map的value是每一行,我这里自定义的是以空格分割
            String[] vs = value.toString().split("\\s");
            for (String v : vs) {
                //写出去
                context.write(new Text(v), ONE);
            }
        }
    }
    //Reduce过程
    /***
     * @author 汤高
     * Text, IntWritable输入类型,从map过程获得 既map的输出作为Reduce的输入
     * Text, IntWritable输出类型
     */
    public static class WordCountReducer extends Reducer<Text, IntWritable, Text, IntWritable>{
        @Override
        protected void reduce(Text key, Iterable<IntWritable> values,
                Reducer<Text, IntWritable, Text, IntWritable>.Context context) throws IOException, InterruptedException {
            int count=0;
            for(IntWritable v:values){
                count+=v.get();//单词个数加一
            }
            
            context.write(key, new IntWritable(count));
        }
        
    }
    
    public static void main(String[] args) {
        
        Configuration conf=new Configuration();
        try {
            //args从控制台获取路径 解析得到域名
            String[] paths=new GenericOptionsParser(conf,args).getRemainingArgs();
            if(paths.length<2){
                throw new RuntimeException("必須輸出 輸入 和输出路径");
            }
            //得到一个Job 并设置名字
            Job job=Job.getInstance(conf,"wordcount");
            //设置Jar 使本程序在Hadoop中运行
            job.setJarByClass(WordCount.class);
            //设置Map处理类
            job.setMapperClass(WordCountMapper.class);
            //设置map的输出类型,因为不一致,所以要设置
            job.setMapOutputKeyClass(Text.class);
            job.setOutputValueClass(IntWritable.class);
            //设置Reduce处理类
            job.setReducerClass(WordCountReducer.class);
            //设置输入和输出目录
            FileInputFormat.addInputPath(job, new Path(paths[0]));
            FileOutputFormat.setOutputPath(job, new Path(paths[1]));
            //启动运行
            System.exit(job.waitForCompletion(true) ? 0:1);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
}
```
好了，整个MapReduce的工作流程就分析到这里了,上面全是个人学习归纳的,如果有什么需要改善的地方,欢迎大家指正,一起进步
            
