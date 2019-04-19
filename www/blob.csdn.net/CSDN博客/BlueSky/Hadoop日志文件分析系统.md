# Hadoop日志文件分析系统 - BlueSky - CSDN博客
2016年03月05日 00:11:07[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：643
个人分类：[Hadoop](https://blog.csdn.net/ASIA_kobe/article/category/6119583)
项目需求：
   需要统计一下线上日志中某些信息每天出现的频率，举个简单的例子，统计线上每天的请求总数和异常请求数。线上大概几十台
服务器，每台服务器大概每天产生4到5G左右的日志，假设有30台，每台5G的，一天产生的日志总量为150G。
处理方案：
   方案1：传统的处理方式，写个JAVA日志分析代码，部署到每台服务器进行处理，这种方式部署起来耗时费力，又不好维护。
   方案2：采用Hadoop分布式处理，日志分析是Hadoop集群系统的拿手好戏。150G每天的日志也算是比较大的数据量了，搭个简
单的Hadoop集群来处理这些日志是再好不过的了。
Hadoop集群的搭建：
      参见这两篇文章：[http://www.cnblogs.com/cstar/archive/2012/12/16/2820209.html](http://www.cnblogs.com/cstar/archive/2012/12/16/2820209.html)
[http://www.cnblogs.com/cstar/archive/2012/12/16/2820220.html](http://www.cnblogs.com/cstar/archive/2012/12/16/2820220.html)
我们这里的集群就采用了两台机器，配置每台8核，32G内存，500G磁盘空间。
日志准备工作：
     由于日志分散在各个服务器，所以我们先需要将所有的日志拷贝到我们的集群系统当中，这个可以通过linux服务器下rsync或者scp
服务来执行。这里我们通过scp服务来拷贝，由于都是内网的机器，所以拷贝几个G的日志可以很快就完成。下面是拷贝日志的脚本，脚本
还是有一些需要注意的地方，我们只需要拷贝前一天的数据，实际保存的数据可能是好几天的，所以我们只要把我们需要的这一天的数据
SCP过去就可以了。
```java
#!/bin/sh
```
```java
workdir=/home/myproj/bin/log/
```
```java
files=`ls $workdir`
```
```java
pre1date=`date  +
```
```java
"%Y%m%d"
```
```java
-d
```
```java
"-1 days"
```
```java
`
```
```java
pre1date1=`date  +
```
```java
"%Y-%m-%d"
```
```java
-d
```
```java
"-1 days"
```
```java
`
```
```java
curdate=`date  +
```
```java
"%Y%m%d"
```
```java
`
```
```java
hostname=`uname -n`
```
```java
echo $pre1date $curdate
```
```java
uploadpath=
```
```java
"/home/hadoop/hadoop/mytest/log/"
```
```java
$pre1date1
```
```java
"/"
```
```java
$hostname
```
```java
echo $uploadpath
```
```java
cd $workdir
```
```java
mintime=
```
```java
240000
```
```java
secondmintime=
```
```java
0
```
```java
for
```
```java
file in $files;
```
```java
do
```
```java
```
```java
filedate=`stat $file | grep Modify| awk
```
```java
'{print $2}'
```
```java
|sed -e
```
```java
's/-//g'
```
```java
`
```
```java
```
```java
filetime=`stat $file | grep Modify| awk
```
```java
'{print $3}'
```
```java
|cut -d
```
```java
"."
```
```java
-f1 | sed -e
```
```java
's/://g'
```
```java
| sed
```
```java
's/^0+//'
```
```java
`
```
```java
```
```java
if
```
```java
[ $filedate -eq $curdate ]; then
```
```java
```
```java
if
```
```java
[ $filetime -lt $mintime ]; then
```
```java
```
```java
secondmintime=$mintime
```
```java
```
```java
mintime=$filetime
```
```java
```
```java
fi
```
```java
```
```java
fi
```
```java
done
```
```java
echo
```
```java
"mintime:"
```
```java
$mintime
```
```java
step=
```
```java
1000
```
```java
mintime=`expr $mintime + $step`
```
```java
echo
```
```java
"mintime+1000:"
```
```java
$mintime
```
```java
for
```
```java
file in $files;
```
```java
do
```
```java
```
```java
filedate=`stat $file | grep Modify| awk
```
```java
'{print $2}'
```
```java
|sed -e
```
```java
's/-//g'
```
```java
`
```
```java
```
```java
filetime=`stat $file | grep Modify| awk
```
```java
'{print $3}'
```
```java
|cut -d
```
```java
"."
```
```java
-f1 | sed -e
```
```java
's/://g'
```
```java
| sed
```
```java
's/^0+//'
```
```java
`
```
```java
```
```java
filename=`echo $file | cut -c
```
```java
1
```
```java
-
```
```java
8
```
```java
`
```
```java
```
```java
startchars=
```
```java
"info.log"
```
```java
```
```java
#echo $filename
```
```java
```
```java
if
```
```java
[ $filename == $startchars ]; then
```
```java
```
```java
if
```
```java
[ $filedate -eq $pre1date ]; then
```
```java
```
```java
scp -rp $file dir
```
```java
@antix2
```
```java
:$uploadpath
```
```java
```
```java
#echo $file
```
```java
```
```java
elif [ $filedate -eq $curdate ]; then
```
```java
```
```java
if
```
```java
[ $filetime -lt $mintime ]; then
```
```java
```
```java
scp -rp $file dir
```
```java
@antix2
```
```java
:$uploadpath
```
```java
```
```java
#echo $file
```
```java
```
```java
fi
```
```java
```
```java
fi
```
```java
```
```java
fi
```
```java
```
```java
#echo $filedate $filetime
```
```java
done
```
MapReduce代码
接下来就是编写MapReduce的代码了。使用Eclipse环境来编写，需要安装hadoop插件，我们hadoop机器采用的是1.1.1版本，所以插
件使用hadoop-eclipse-plugin-1.1.1.jar，将插件拷贝到eclipse的plugins目录下就可以了。然后新建一个MapReduce项目：
![](http://images.cnitblog.com/blog/65340/201307/14093931-bfb3e1461fca4d76bc88fb8c636438f5.x-png)
工程新建好了然后我们就可以编写我们的MapReduce代码了。
```
importjava.io.IOException;
importjava.text.SimpleDateFormat;
importjava.util.Date;
importorg.apache.hadoop.conf.Configuration;
importorg.apache.hadoop.fs.FileSystem;
importorg.apache.hadoop.fs.Path;
importorg.apache.hadoop.io.IntWritable;
importorg.apache.hadoop.io.LongWritable;
importorg.apache.hadoop.io.Text;
importorg.apache.hadoop.mapreduce.Job;
importorg.apache.hadoop.mapreduce.Mapper;
importorg.apache.hadoop.mapreduce.Reducer;
importorg.apache.hadoop.mapreduce.lib.input.FileInputFormat;
importorg.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
importorg.apache.hadoop.util.GenericOptionsParser;
publicclass LogAnalysis {
publicstaticclassLogMapper
extends Mapper<LongWritable, Text, Text, IntWritable>{
privatefinalstatic IntWritable one = newIntWritable(1);
private Text word = newText();
private Text hourWord = newText();
publicvoid map(LongWritable key, Text value, Context context
)throws IOException, InterruptedException {
 String line =value.toString();
 SimpleDateFormat formatter2 =newSimpleDateFormat("yy-MM-dd");
 java.util.Date d1 =newDate();
d1.setTime(System.currentTimeMillis()-1*24*3600*1000);
 String strDate =formatter2.format(d1);
if(line.contains(strDate)){
 String[] strArr = line.split(",");
int len = strArr[0].length();
 String time = strArr[0].substring(1,len-1);
 String[] timeArr = time.split(":");
 String strHour = timeArr[0];
 String hour = strHour.substring(strHour.length()-2,strHour.length());
 String hourKey ="";
if(line.contains("StartASocket")){
word.set("SocketCount");
 context.write(word, one);
hourKey="SocketCount:"+hour;
hourWord.set(hourKey);
 context.write(hourWord, one);
word.clear();
hourWord.clear();
}
if(line.contains("SocketException")){
word.set("SocketExceptionCount");
 context.write(word, one);
hourKey="SocketExceptionCount:"+hour;
hourWord.set(hourKey);
 context.write(hourWord, one);
word.clear();
hourWord.clear();
}
}
}
publicstaticclassLogReducer
extendsReducer<Text,IntWritable,Text,IntWritable>{
private IntWritable result = newIntWritable();
publicvoid reduce(Text key, Iterable<IntWritable>values,
 Context context
)throws IOException, InterruptedException {
int sum = 0;
for (IntWritable val : values) {
sum+=val.get();
}
result.set(sum);
 context.write(key, resul
```

