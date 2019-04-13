
# spark 通过打散热点key解决数据倾斜问题 - bitcarmanlee的博客 - CSDN博客


2017年12月22日 16:06:11[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1172



## 1.热点key的数据倾斜
在大数据相关的统计与处理中，热点key造成的数据倾斜非常常见也非常讨厌，经常会造成job运行时间变长或者造成job的OOM最后导致任务失败。例如在wordcount任务中，如果有一个word是热点词，出现的次数很多，那么最后这个job的运行时间就是由这个热点词所在的task运行时间决定的。因此遇到这种热点问题，我们需要想办法改进代码，优化任务，提高最终的运行效率。
## 2.实际case
现在有这么一个简单的实际例子：
hdfs上有一个名为”xxx”的路径，此路径下的数据量比较大，有几百G之多。现在我们想统计一下这个路径下所有文件的行数。
如果数据量不大，在spark-shell中，可以用一行简单的代码解决问题：
```python
scala> sc
```
```python
.textFile
```
```python
(
```
```python
"xxx"
```
```python
)
```
```python
.count
```
```python
()
```
但是数据量大了以后，运行的速度很慢很慢，慢到不可接受；而且最后程序会报OOM退出，得不到最终的结果。那怎么办呢？
## 3.通过将热点key打算做计算
我们将上述需求稍微做一下转型：
统计所有数据的行数，假设每一行对应的一个key就是”all”,每一行的输出是”all, 1”，最后需要做的就是简单的wordcount，针对all这个热点key，然后求和！
这种我们明确知道热点key是啥的case，一般的做法是将热点key先打散，然后再聚回来！
直接上代码：
```python
def linestats(
```
```python
sc
```
```python
: SparkContext) = {
        val inputpath =
```
```python
"xxx"
```
```python
sc.textFile(inputpath)
            .map(x
```
```python
=>
```
```python
{
                val randomNum = (
```
```python
new
```
```python
java.util.Random).nextInt(
```
```python
2000
```
```python
)
                val allkey = randomNum +
```
```python
"_all"
```
```python
(allkey,
```
```python
1
```
```python
)
            })
            .reduceByKey
```
```python
((x, y) => x + y)
```
```python
.
```
```python
map
```
```python
(x => {
                val (keywithrandom, num) = (x._1, x._2)
                val key = StringUtils.split(keywithrandom,
```
```python
"_"
```
```python
)(
```
```python
1
```
```python
)
                (key, num.toLong)
            })
```
```python
.
```
```python
reduceByKey
```
```python
((x, y) => x + y)
```
```python
.
```
```python
map
```
```python
(x =>
```
```python
"%s\t%s"
```
```python
.format(x._1, x._2))
```
```python
.
```
```python
repartition
```
```python
(
```
```python
1
```
```python
)
```
```python
}
```
上面代码的思路如下：
1.第一步先将key打算，给所有”all”加上一个随机前缀。
2.然后对带有随机前缀的key做第一次聚合，即reduceByKey操作，得出第一次聚合的结果。
3.再将随机前缀去掉，做第二次聚合，即reduceByKey操作，得到最终的结果！

