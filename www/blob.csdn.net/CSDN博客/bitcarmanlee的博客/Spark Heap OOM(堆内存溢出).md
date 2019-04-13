
# Spark Heap OOM(堆内存溢出) - bitcarmanlee的博客 - CSDN博客


2017年12月13日 11:11:33[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：4086


spark任务在调试过程中，OOM是非常讨厌的一种情况。本文针对Heap OOM的情况先做一定分析，告诉大家如何调参。
## 1.Heap OOM的现象
如果在Spark UI或者在spark.log中看到如下日志：
```python
java.lang.OutOfMemoryError
```
```python
:
```
```python
GC overhead limit exceeded
```
```python
java.lang.OutOfMemoryError
```
```python
:
```
```python
java heap space
```
或者在gc.log中能看到类似的Full GC日志：
```python
[
```
```python
Full
```
```python
GC (Ergonomics) [PSYoungGen:
```
```python
285001
```
```python
K
```
```python
->
```
```python
52041
```
```python
K
```
```python
(
```
```python
465920
```
```python
K
```
```python
)] [ParOldGen:
```
```python
1398048
```
```python
K
```
```python
->
```
```python
1398049
```
```python
K
```
```python
(
```
```python
1398272
```
```python
K
```
```python
)]
```
```python
1683050
```
```python
K
```
```python
->
```
```python
1450091
```
```python
K
```
```python
(
```
```python
1864192
```
```python
K
```
```python
), [Metaspace:
```
```python
35798
```
```python
K
```
```python
->
```
```python
35798
```
```python
K
```
```python
(
```
```python
1081344
```
```python
K
```
```python
)],
```
```python
3.1598485
```
```python
secs] [
```
```python
Times
```
```python
: user=
```
```python
12.17
```
```python
sys=
```
```python
0.09
```
```python
, real=
```
```python
3.16
```
```python
secs]
```
此时就很有可能发生了Heap OOM
## 2.Spark 内存模型
Spark框架主要有两处消耗heap的地方, Spark内部将其分成2个区: Storage和Execution(Execution部分主要用于ShuffleRead和ShuffleWrite).
1.Storage: 主要存RDD, Broadcast等. 涉及的Spark操作: persist/cache/sc.broadcast等
2.Execution: 主要用于Shuffle阶段, read shuffle/write shuffle阶段需要开buffer来做一些merge操作或者防止shuffle数据放内存原地爆炸. 一般涉及的操作: XXXXByKey(reduceByKey,combineByKey等)/coGroup/join类等.
## 3.Driver heap
Driver heap OOM的三大原因:
(1).用户在Driver端口生成大对象, 比如创建了一个大的集合数据结构
解决思路:
1.1. 考虑将该大对象转化成Executor端加载. 例如调用sc.textFile/sc.hadoopFile等
1.2. 如若无法避免, 自我评估该大对象占用的内存, 相应增加driver-memory的值
(2).从Executor端收集数据回Driver端
比如Collect. 某个Stage中Executor端发回的所有数据量不能超过spark.driver.maxResultSize，默认1g. 如果用户增加该值, 请对应增加2delta increase到Driver Memory, resultSize该值只是数据序列化之后的Size, 如果是Collect的操作会将这些数据反序列化收集, 此时真正所需内存需要膨胀2-5倍, 甚至10倍. *解决思路:
2.1. 本身不建议将大的数据从Executor端, collect回来. 建议将Driver端对collect回来的数据所做的操作, 转化成Executor端RDD操作.
2.2. 如若无法避免, 自我评collect需要的内存, 相应增加driver-memory的值
(3)Spark本身框架的数据消耗.
现在在Spark1.6版本之后主要由Spark UI数据消耗, 取决于作业的累计Task个数.
解决思路: 3.1. 考虑缩小大numPartitions的Stage的partition个数, 例如从HDFS load的partitions一般自动计算, 但是后续用户的操作中做了过滤等操作已经大大减少数据量, 此时可以缩小Parititions。
3.2. 通过参数spark.ui.retainedStages(默认1000)/spark.ui.retainedJobs(默认1000)控制.
3.3. 实在没法避免, 相应增加内存.
## 4.Executor heap
UI 表现形式:
UI Task的失败原因显示: java.lang.OutOfMemoryError
![这里写图片描述](https://img-blog.csdn.net/20171213110812079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
UI Task的失败原因显示: ExecutorLostFailure 和Executor exit code 为143.
![这里写图片描述](https://img-blog.csdn.net/20171213110851961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
UI Task失败的原因显示: ExecutorLostFailure 和Executor Lost的原因是Executor heartbeat timed out Spark heap heart beat
![这里写图片描述](https://img-blog.csdn.net/20171213110910592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
OOM原因
(1)数据相关
例如用户单key对应的Values过多, 调用groupByKey/对RDD[K, V: List/Array]做集合操作.
解决思路:
1.1. 控制单key的Value的个数, 做个数截断或者做过滤. 很多情况是用户自身有异常数据导致.
1.2. 考虑对业务逻辑的RDD操作, 考虑其他方式的RDD实现, 避免统一处理所有的Values. 比如对Key做且分,类似keyA_1, keyA_2操作.
1.3. 降低spark.memory.fraction的值, 以此提高用户可用的内存空间. 注意spark.memory.fraction的至少保证在0.1. 降低该值会影响Spark的执行效率, 酌情减少。
1.4 增加 Exeutor-memory
(2)用户代码
在RDD操作里创建了不容易释放的大对象**, 例如集合操作中产生不易释放的对象。
解决思路:
2.1. 优化逻辑. 避免在一个RDD操作中实现大量集合操作, 可以尝试转化成多个RDD操作.
2.2. 降低spark.memory.fraction的值, 以此提高用户可用的内存空间. 注意spark.memory.fraction的至少保证在0.1, 降低该值会影响Spark的执行效率, 酌情减少。
2.3. 增加Executor-memory.

