# 用SBT编译Spark的WordCount程序 - BlueSky - CSDN博客
2015年11月19日 11:15:24[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：703
转自：http://www.aboutyun.com/thread-8587-1-1.html
sbt介绍
sbt是一个代码编译工具，是scala界的mvn，可以编译scala，java等，需要java1.6以上。
编译生成package的时候
```java
sbt package  -Dsbt.ivy.home=/root/.ivy2
```
sbt项目环境建立
sbt编译需要固定的目录格式，并且需要联网，sbt会将依赖的jar包下载到用户home的.ivy2下面，目录结构如下：
- 
|--build.sbt
- 
|--lib
- 
|--project
- 
|--src
- 
|   |--main
- 
|   |    |--scala
- 
|   |--test
- 
|         |--scala
- 
|--sbt
- 
|--target
复制代码
以上建立目录如下：
- 
mkdir -p ~/spark_wordcount/lib
- 
mkdir -p ~/spark_wordcount/project
- 
mkdir -p ~/spark_wordcount/src/main/scala
- 
mkdir -p ~/spark_wordcount/src/test/scala
- 
mkdir -p ~/spark_wordcount/target
复制代码
然后拷贝spark安装目录的sbt目录的 sbt脚本和sbt的jar包
- 
cp /path/to/spark/sbt/sbt* ~/spark_wordcount/
复制代码
由于spark的sbt脚本默认查找./sbt目录，修改如下
- 
JAR=sbt/sbt-launch-${SBT_VERSION}.jar
- 
to
- 
JAR=sbt-launch-${SBT_VERSION}.jar
复制代码
拷贝spark的jar包到，sbt的lib目录
- 
cp /path/to/spark/assembly/target/scala-2.10/spark-assembly_2.10-0.9.0-incubating-hadoop2.2.0.jar \
- 
> ~/spark_wordcount/lib/
复制代码
建立build.sbt配置文件,各行需要有一个空行分割
- 
name := "WordCount"
- 
[this is bank line]
- 
version := "1.0.0"
- 
[this is bank line]
- 
scalaVersion := "2.10.3"
复制代码
由于spark的sbt脚本需要到project的build.properties文件找sbt的版本号，我们建立该文件，增加如下内容：
- 
sbt.version=0.12.4
复制代码
Spark WordCount程序编写及编译
建立WordCount.scala源文件，假设需要包为spark.example
- 
mkdir -p ~/spark_wordcount/src/main/scala/spark/example
- 
vi -p ~/spark_wordcount/src/main/scala/spark/example/WordCount.scala
复制代码
添加具体的程序代码，并保存
- 
package spark.example
- 
- 
import org.apache.spark._
- 
import SparkContext._
- 
- 
object WordCount {
- 
  def main(args: Array[String]) {
- 
    //命令行参数个数检查
- 
    if (args.length == 0) {
- 
      System.err.println("Usage: spark.example.WordCount <input> <output>")
- 
      System.exit(1)
- 
    }
- 
    //使用hdfs文件系统
- 
    val hdfsPathRoot = "hdfshost:9000"
- 
    //实例化spark的上下文环境
- 
    val spark = new SparkContext(args(0), "WordCount",
- 
      System.getenv("SPARK_HOME"),SparkContext.jarOfClass(this.getClass))
- 
    //读取输入文件
- 
    val inputFile = spark.textFile(hdfsPathRoot + args(1))
- 
    //执行WordCount计数
- 
    //读取inputFile执行方法flatMap，将每行通过空格分词
- 
    //然后将该词输出该词和计数的一个元组，并初始化计数
- 
    //为 1，然后执行reduceByKey方法，对相同的词计数累
- 
    //加
- 
    val countResult = inputFile.flatMap(line => line.split(" "))
- 
                      .map(word => (word, 1))
- 
                      .reduceByKey(_ + _)
- 
    //输出WordCount结果到指定目录
- 
    countResult.saveAsTextFile(hdfsPathRoot + args(2))
- 
  }
- 
}
复制代码
到spark_wordcount目录，执行编译：
- 
cd ~/spark_wordcount/
- 
./sbt compile
复制代码
打成jar包
- 
./sbt package
复制代码
编译过程，sbt需要上网下载依赖工具包，jna，scala等。编译完成后可以在target/scala-2.10/目录找到打包好的jar
- 
[root@bd001 scala-2.10]# pwd
- 
/usr/local/hadoop/spark_wordcount/target/scala-2.10
- 
[root@bd001 scala-2.10]# ls
- 
cache  classes  wordcount_2.10-1.0.0.jar
复制代码
WordCount执行
可以参考Spark分布式运行于hadoop的yarn上的方法，写一个执行脚本
- 
#!/usr/bin/env bash
- 
- 
SPARK_JAR=./assembly/target/scala-2.10/spark-assembly_2.10-0.9.0-incubating-hadoop2.2.0.jar \
- 
    ./bin/spark-class org.apache.spark.deploy.yarn.Client \
- 
      --jar ~/spark_wordcount/target/scala-2.10/wordcount_2.10-1.0.0.jar \
- 
      --class  spark.example.WordCount \
- 
      --args yarn-standalone \
- 
      --args /testWordCount.txt \
- 
      --args /resultWordCount \
- 
      --num-workers 3 \
- 
      --master-memory 4g \
- 
      --worker-memory 2g \
- 
      --worker-cores 2
复制代码
然后，拷贝一个名为testWordCount.txt的文件进hdfs
- 
hdfs dfs -copyFromLocal ./testWordCount.txt /testWordCount.txt
复制代码
然后执行脚本，过一会就可以看到结果了
