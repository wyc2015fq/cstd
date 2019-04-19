# hadoop-eclipse-plugin的正确使用(对应0.20.1版本) - 三少GG - CSDN博客
2013年08月28日 15:13:40[三少GG](https://me.csdn.net/scut1135)阅读数：1836

1.下载hadoop-0.20.1-eclipse-plugin.jar包 (地址：[http://code.google.com/p/hadoop-eclipse-plugin/](http://code.google.com/p/hadoop-eclipse-plugin/))
2.拷贝上面下的包到eclipse的plugins文件夹下
3.重启eclipse，点击主菜单上的window-->preferences，在左边栏中找到Hadoop Map/Reduce，点击后在右边对话框里设置hadoop的安装路径即主目录，
   eg: /usr/local/hadoop
4.选择Map/Reduce视图:eclipse--->window---->open perspective---->other---->map/reduce
  window ->Show View -> 搜索“map/reduce Locations”视图 选上 -> 点击“OK”
5. 增加DFS Locations：点击Map/Reduce Locations—> New Hadoop Loaction，填写对应的host和port
![b92e9add-c337-3de0-8f62-e9f0707b0260.png](http://f.dataguru.cn/farattach/forum/201210/23/213451hwbwb9b53z38x6wn.png)
Location Name ： 此处为参数设置名称，可以任意填写
Map/Reduce Master (此处为Hadoop集群的Map/Reduce地址，应该和mapred-site.xml中的mapred.job.tracker设置相同)
DFS Master  (此处为Hadoop的master服务器地址，应该和core-site.xml中的 fs.default.name 设置相同)
一般情况下为 
Map/Reduce Master 
Host： localhost 
Port： 9001 
DFS Master 
Host： localhost 
Port： 9000 
配置完成后，点击Finish。如配置成功，在DFS Locations中将显示出新配置的文件夹。
![4445078c-b725-3351-91b1-a3038517aaef.png](http://f.dataguru.cn/farattach/forum/201210/23/213450vutzlktftfxktd5t.png)
![e344abb7-fd70-39a6-99f1-010532fc0750.png](http://f.dataguru.cn/farattach/forum/201210/23/2134525eeddcexqcp2eqm0.png)
在左边project explorer 列表框中找到“DFS Locations”右键refresh，就会看到
一个目录树，eclipse提供了一个目录树来管理HDFS系统，右键可以创建/删除目录，上传/
查看文件，操作起来很简单，内容与命令 hadoop dfs -ls 结果一致
load     加载
unload   卸载
upload   上传
download 下载
6. 新建Map/Reduce任务
File->New->project->Map/Reduce Project->Next
Project name:example ->Next->Finish 
-> src右键->New->class Name:example->Finish -> 
左侧窗口就显示一个example.java 类文件了
**详细解释（必看）**：（[http://www.cnblogs.com/shitouer/archive/2012/05/29/2522860.html](http://www.cnblogs.com/shitouer/archive/2012/05/29/2522860.html)）
Location Name：
这个不用在意，就是对这个MapReduce的标示，只要能帮你记忆即可
Map/Reduce Master 部分相关定义：
Host：上一节搭建的集群中JobTracker所在的机器的IP地址
port：JobTracker的端口
两个参数就是 mapred-site.xml中mapred.job.tracker的ip和端口
DFS Master部分：
Host：就是上一节集群搭建中Namenode所在机器IP
Port：就是namenode的端口
这两个参数是在 core-site.xml里fs.default.name里面的ip和端口
User Name：
就是搭建Hadoop集群是所用的用户名，我这里用的是root
⑥ 填写完以上信息以后，关闭Eclipse，然后重新启动。
为什么这么做呢？因为如果不重新启动，Advanced Parameters里，有些重要的选项不能显示，重启后，来配制Advanced Parameters信息：
可看到大部分选项都已经预设值好了，其中有一下几个选项，必须注意填写，其他的选项，要么默认，要么会根据我们填写的以下几个选项自动修改：
![](http://pic002.cnblogs.com/images/2012/79891/2012052822342690.jpg)
dfs.replication:
这个这里默认是3，但是要根据你的Datanode的个数来定，如果你仅有2个datanode，那么就写2，如果只有一个，就写1，大于或者等于3个，写3
hadoop.tmp.dir:
这个要跟上节建立的Hadoop集群配置中core-site.xml 中的hadoop.tmp.dir设置一样，上节填写的是/tmp/hadoop-root，这里也如是填写
hadoop.job.ugi:
填成root,Tardis，其中root是我们的用户名，后边的",Tardis"一定要这么写，不能改
这些都填写完了之后，点击保存，重新启动Eclipse，会发现，很多Advanced Parameters里的好多选项都根据hadoop.tmp.dir重新进行了设置。
(2012.10.26修改：现在又用hadoop1.0.3，此选项没有了)
⑦ 重新启动后，在Project Explorer中，会有如下显示：
![](http://pic002.cnblogs.com/images/2012/79891/2012052822365495.jpg)
 注意，第一次初始化打开，并不会有user, input, output 这些文件夹，之后红圈圈出的部分，也没有job_local_0001(1).如果是这样显示说明你已经连接成功，否则，展开所有tmp文件夹，会发现最后有错误信息提示。
2012.10.26修改：
hadoop1.0.3此时还会出错，是plugin中jar包不够，可做如下修改：
1.打开Eclipse Plugins中的hadoop-eclipse-plugin-1.0.0.jar，发现只有发现只有commons-cli-1.2.jar和hadoop-core.jar两个包，将%HADOOP_HOME%/lib下的 commons-configuration-1.6.jar
 , commons-httpclient-3.0.1.jar , commons-lang-2.4.jar , jackson-core-asl-1.8.8.jar 和 jackson-mapper-asl-1.0.8.jar五个jar包打入hadoop-eclipse-plugin-1.0.0.jar中。
2.修改hadoop-eclipse-plugin-1.0.0.jar中META-INF目录下的MANIFEST.MF，将classpath修改为以下内容：Bundle-ClassPath: classes/,lib/hadoop-core.jar,lib/commons-cli-1.2.jar,lib/commons-httpclient-3.0.1.jar,lib/jackson-core-asl-1.0.1.jar,lib/jackson-mapper-asl-1.0.1.jar,lib/commons-configuration-1.6.jar,lib/commons-lang-2.4.jar
3.重启Eclipse即可。
⑧ 在Eclipse中编写调试第一个Hadoop的HelloWorld程序：wordcount
建立Map/Reduce 工程
![](http://pic002.cnblogs.com/images/2012/79891/2012052822405298.jpg)
定义WordCount.Java类
这里，我下载了Hadoop源码，直接拷贝了其WordCount.java文件，代码如下：
```
package org.apache.hadoop.examples;
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
    public static class TokenizerMapper extends
            Mapper<Object, Text, Text, IntWritable> {
        private final static IntWritable one = new IntWritable(1);
        private Text word = new Text();
        public void map(Object key, Text value, Context context)
                throws IOException, InterruptedException {
            String line = value.toString();
            StringTokenizer itr = new StringTokenizer(line);
            while (itr.hasMoreTokens()) {
                word.set(itr.nextToken().toLowerCase());
                context.write(word, one);
            }
        }
    }
    public static class IntSumReducer extends
            Reducer<Text, IntWritable, Text, IntWritable> {
        private IntWritable result = new IntWritable();
        public void reduce(Text key, Iterable<IntWritable> values,
                Context context) throws IOException, InterruptedException {
            int sum = 0;
            for (IntWritable val : values) {
                sum += val.get();
            }
            result.set(sum);
            context.write(key, new IntWritable(sum));
        }
    }
    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        String[] otherArgs = new GenericOptionsParser(conf, args)
                .getRemainingArgs();
        if (otherArgs.length != 2) {
            System.err.println("Usage: wordcount <in> <out>");
            System.exit(2);
        }
        Job job = new Job(conf, "word count");
        job.setJarByClass(WordCount.class);
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
```
为了使程序成功运行，我们需要做一下工作
设置输入：
在命令行中，向hadoop中添加一个文本文件：
hadoop fs -mkdir /input  #在HDFS用户根目录下建立input文件夹
hadoop fs -put test.txt /input/ #向input文件夹里放入所在路径的test.txt文件
该步骤也可以在Eclipse中建立，首先右键点击Project Explorer中大象图标下第一个没有名字的文件夹，点击 Create New Directory，创建input文件夹，后再右键该文件夹，上传test.txt文件。此时到hadoop DFS中查看，也会发现创建成功。
Eclipse中设置输入参数
配置参数 
点击Run-->Run Configurations，在弹出的对话框中左边栏选择Java Application，点击右键New，在右边栏中对Arguments进行配置。 
在Program arguments中配置输入输入、输出目录参数 
**hdfs://localhost:9000/**user/hadoop/input **hdfs://localhost:9000/**user/hadoop/output
程序的运行结果可在out目录下进行查看。 
点击Run运行程序。 
注意： 
input文件夹是需要在程序运行前创建的，output文件夹是不能提前创建的，要由系统自动生成，否则运行时会出现错误。
设置WordCount.java的执行参数如图
![](http://pic002.cnblogs.com/images/2012/79891/2012052823245211.jpg)
在Eclipse中执行
点击**Run或者run as Java Application运行程序。(Run on Hadoop没反应)**
如图，点击 Run on Hadoop
![](http://pic002.cnblogs.com/images/2012/79891/2012052823264530.png)
程序的运行结果可在output目录下进行查看。 
在控制台，会有以下输出
![](http://pic002.cnblogs.com/images/2012/79891/2012052823352719.png)
OK，至此，Congratulations，你已经建立起自己在Windows中Eclipse下进行Hadoop MapReduce开发的基本环境设置工作，并且输出了一个属于自己的HelloWorld程序，并且稍微了解了一些hadoop日志分析的信息，你已经开始进入Hadoop的世界，剩下的，就是要不断地深入学习了。
***************************************************************************************************************************************************************************

編譯 hadoop 0.20.1 的 eclipse plugin
Eclipse 3.3 3.4 3.5 適用
[http://trac.nchc.org.tw/cloud/wiki/waue/2010/0211](http://trac.nchc.org.tw/cloud/wiki/waue/2010/0211)
# 0. 前言[¶](http://trac.nchc.org.tw/cloud/wiki/waue/2010/0211#a0.%E5%89%8D%E8%A8%80)
- 從 hadoop 0.20.1 之後，contrib 資料夾下就在也找不到 eclipse-plugin 可以用了，因為作者群認為大家不是小孩子了，要自己種出東西來吃，而不是伸手要來之後又一直抱怨有瑕疵...（在這之前一直抱怨有瑕疵的就是我）。
- 此篇可以幫助大家編譯出適用於 hadoop 0.20 的 各版本 eclipse plugin （hadoop-0.20.1-eclipse-plugin.jar）出來
- 參考 [http://wiki.apache.org/hadoop/EclipsePlugIn](http://wiki.apache.org/hadoop/EclipsePlugIn)
- 同 [http://sites.google.com/site/waue0920/Home/hadoop/bian-yi-hadoop-eclipse-plugin](http://sites.google.com/site/waue0920/Home/hadoop/bian-yi-hadoop-eclipse-plugin)
