# IDEAJ中Scala打包提交Spark集群运行 - LC900730的博客 - CSDN博客
2017年12月11日 22:27:33[lc900730](https://me.csdn.net/LC900730)阅读数：608
最简单一个demo 
//读取hdfs，然后写入hdfs
```
package com.spark.FirstApp
import org.apache.spark.SparkContext
import org.apache.spark.SparkConf
object WordCountTest {
  def main(args: Array[String]) {
    val inputFile =  "hdfs://192.168.1.105:9000/word.txt"
    val conf = new SparkConf().setAppName("WordCount111").setMaster("spark://centos701:7077")
    val sc = new SparkContext(conf)
    val textFile = sc.textFile(inputFile,1)
 textFile.saveAsTextFile("hdfs://192.168.1.105:9000/test/hello.txt")
  }
}
```
IDEAJ中Build->BuidProject->Build Artifacts->Build,然后找到jar包。
在客户机中提交 
 ./spark-submit –class com.spark.FirstApp.WordCountTest /Users/liuchao/Desktop/20171211.jar 
然后查看hdfs发现文件成功写入
