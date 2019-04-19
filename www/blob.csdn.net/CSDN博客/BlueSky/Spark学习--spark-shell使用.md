# Spark学习--spark-shell使用 - BlueSky - CSDN博客
2015年11月18日 21:13:27[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：497
```java
<span style="font-family: Arial, Helvetica, sans-serif; font-size: 12px; background-color: rgb(255, 255, 255);">Spark-shell使用</span>
```
配置好Spark集群，测试Sparkwordcount程序，可以通过基本的Spark-shell来进行交互式的代码提交，比如：
```python
<span style="font-size:18px;">val textFile = sc.textFile("words/test.txt")</span>
```
![](https://img-blog.csdn.net/20151118210954104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
`<span style="font-size:18px;">val result = textFile.flatMap(line => line.split("\\s+")).map(word => (word,1)).reduceByKey(_+_)</span>``<span style="font-size:18px;">#上面的为基本先把文件的每行进行分割，再把字符串进行计数，接着进行累加</span>``<span style="color:#ff0000;"><span style="font-size:18px;">自己在测试的时候在第一步传一个不存在的文件，则当时第一步不提示出错，等到第二步，执行reduceByKey（_+_）来进行统计的时候会报错，这就应了RDD在transformation的时候不做任何操作，只是基本的做个转换，这就体现了RDD的“惰性”。</span></span>`
```
<pre name="code" class="python"><span style="font-size:18px;">result.saveAsTextFile("words/result1")
</span>
```
#第三步是将文件保存在hdfs集群的相应位置下。`<span style="font-size:18px;">好了，第一步结束了，也许你想自己编写一个比较长的scala程序来一次性执行，ok，也可以满足你！来看：</span>``<span style="font-size:18px;">创建myFile.scala 代码如下：</span>`
```
<h3><pre name="code" class="java"><pre name="code" class="python">import org.apache.spark._
import SparkContext._
val sc = new SparkContext()
val textFile = sc.textFile("words/test.txt")
val result = textFile.flatMap(line => line.split("\\s+")).map(word => (word,1)).reduceByKey(_+_)
result.saveAsTextFile("words/gaga")
stty echo
```
#最后一句是设置终端的，如果不加执行玩程序你将会发现，终端命令显示不出来而且不换行
`<span style="font-size:18px;color:#000099;">然后执行./spark-shell < myFile.scala</span>``<span style="font-size:18px;color:#000099;">就ok了，网上还有说写个脚本如下：</span>``<span style="font-size:18px;color:#000099;"><span style="font-family: Helvetica, Tahoma, Arial, sans-serif; line-height: 24px; background-color: rgb(245, 245, 245);">#!/usr/lib/spark/bin/spark-shell</span><br style="box-sizing: content-box; font-family: Helvetica, Tahoma, Arial, sans-serif; line-height: 24px; background-color: rgb(245, 245, 245);" /><span style="font-family: Helvetica, Tahoma, Arial, sans-serif; line-height: 24px; background-color: rgb(245, 245, 245);">scala 代码</span></span>`
可以在下图看到执行两次保存的文件，Make it！
![](https://img-blog.csdn.net/20151118211023031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
