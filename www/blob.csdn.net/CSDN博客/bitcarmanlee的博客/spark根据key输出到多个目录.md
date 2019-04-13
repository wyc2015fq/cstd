
# spark根据key输出到多个目录 - bitcarmanlee的博客 - CSDN博客


2017年06月08日 20:08:56[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：3884


项目中需要将spark的输出按id输出到不同的目录中，即实现在spark中的多路输出。我们可以调用saveAsHadoopFile函数并自定义一个OutputFormat类，就可以达到上述目的。
```python
import
```
```python
org.apache.commons.lang3.StringUtils
```
```python
import
```
```python
org.apache.hadoop.fs.{FileSystem, Path}
```
```python
import
```
```python
org.apache.hadoop.mapred.lib.MultipleTextOutputFormat
```
```python
import
```
```python
org.apache.spark.{SparkConf, SparkContext}
```
```python
/**
  * Created by WangLei on 17-6-2.
  */
```
```python
object
```
```python
genSpecifiedUserData
```
```python
{
```
```python
val
```
```python
inputPath =
```
```python
"xxx"
```
```python
def
```
```python
genData(sc:SparkContext) = {
        sc.textFile(inputPath)
            .filter(x => StringUtils.split(x,
```
```python
"\t"
```
```python
).length ==
```
```python
7
```
```python
)
            .map(x => {
```
```python
val
```
```python
lines = StringUtils.split(x,
```
```python
"\t"
```
```python
)
```
```python
val
```
```python
(id, aid, title, desc) = (lines(
```
```python
0
```
```python
), lines(
```
```python
1
```
```python
), lines(
```
```python
5
```
```python
), lines(
```
```python
6
```
```python
))
                (id, aid, title, desc)
            })
            .filter(x => StringUtils.isNotBlank(x._1))
            .map(x => {
```
```python
val
```
```python
(id, aid, title, desc) = (x._1, x._2, x._3, x._4)
```
```python
val
```
```python
pattern = Array(aid, title, desc).mkString(
```
```python
"\t"
```
```python
)
                (id, pattern)
            })
            .partitionBy(
```
```python
new
```
```python
org.apache.spark.HashPartitioner(
```
```python
10
```
```python
))
    }
```
```python
def
```
```python
main(args: Array[String]): Unit = {
```
```python
val
```
```python
sparkConf =
```
```python
new
```
```python
SparkConf().setAppName(
```
```python
"gen_specified_user_data"
```
```python
)
        sparkConf.set(
```
```python
"spark.serializer"
```
```python
,
```
```python
"org.apache.spark.serializer.KryoSerializer"
```
```python
)
```
```python
val
```
```python
sc =
```
```python
new
```
```python
SparkContext(sparkConf)
```
```python
val
```
```python
output =
```
```python
"/data/xxx/specified/"
```
```python
val
```
```python
fileSystem = FileSystem.get(sc.hadoopConfiguration)
        fileSystem.delete(
```
```python
new
```
```python
Path(output),
```
```python
true
```
```python
)
        findSpecifiedUser(sc).saveAsHadoopFile(
            output,
            classOf[String],
            classOf[String],
            classOf[RDDMultipleTextOutputFormat[_, _]])
        sc.stop()
    }
}
```
```python
class
```
```python
RDDMultipleTextOutputFormat
```
```python
[
```
```python
K
```
```python
,
```
```python
V
```
```python
]
```
```python
()
```
```python
extends
```
```python
MultipleTextOutputFormat
```
```python
[
```
```python
K
```
```python
,
```
```python
V
```
```python
]
```
```python
()
```
```python
{
```
```python
override
```
```python
def
```
```python
generateFileNameForKeyValue(key: K, value: V, name: String) : String = {
        (key +
```
```python
"/"
```
```python
+ name)
    }
}
```
其中，输入的数据是以”\t”分隔一共七列，第一列为用户id。我们希望将输出的时候，相同的用户id输出到同一个目录下面，不同的用户id分开。
RDDMultipleTextOutputFormat类中的generateFileNameForKeyValue函数有三个参数，key和value就是我们RDD的Key和Value，而name参数是每个Reduce的编号。上面的代码中没有使用该参数，而是直接将同一个Key的数据输出到同一个文件中。
最后生成的结果在HDFS上为：
```python
/data/xxx/specified/idA/
```
```python
...
```
```python
/data/xxx/specified/idB/
```
```python
...
```
```python
/data/xxx/specified/idC/
```
```python
...
```

