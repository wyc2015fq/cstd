# spark算子运算 - LC900730的博客 - CSDN博客
2017年12月10日 13:46:04[lc900730](https://me.csdn.net/LC900730)阅读数：221
使用spark-shell进入scala命令行中 
./bin/spark-shell – master spark://centos701:7077 
这样才能拿到sc对象。
### 运行
sc.textFile(“hdfs://centos701:9000/wc”).flatMap(*.split(” “)).map((*,1)).reduceByKey(*+*).sortBy(_._2,false).collect
### 运行后保存到文件
sc.textFile(“hdfs://node-1.itcast.cn:9000/wc”).flatMap(*.split(” “)).map((*,1)).reduceByKey(*+*).sortBy(_._2,false).saveAsTextFile(“hdfs://centos701:9000/out”)
sc.parallelize。
val rdd1=sc.parallelize(Array(1,2,3,4,5,6,7,8,9,10)) 
rdd1:org.apache.spark.rdd.RDD[int]=ParallelcollectorRDD[22] at …
#### 乘以10
val rdd2=rdd1.map(_*10) 
rdd2:org.apache.spark.rdd.RDD[Int]=MapPartitionsRDD[23] at map at … 
#### 过滤
val rdd3=rdd2.filter(_<50) 
rdd3: org.apache.spark.rdd.RDD[Int]=MapPartitionsRDD[24] at filter at … 
以上数据此时都是放在内存的，如果需要计算 
rdd3.collect
## spark中算子分为两类
### 一类叫做Transformation(转换)
### 一类叫做Action(动作)
Transformation延迟执行，Transformation会记录元数据信息，当计算任务触发Action时才会真正开始计算。
## 创建RDD有2种方式
1.通过HDFS支持的文件系统创建RDD，RDD里面没有真正要计算的数据，只是记录了一下元数据 
2.通过scala集合或者数组以并行化的方式创建RDD 
    如sc.parallelize。
### 在SparkContext对象是driver program，它与节点管理者通信管理任务的运行。
each RDD has the following 5 main characters
- a list of partitions
- a function for computing each split
- a list of dependencies on other RDDS
- optionally ,a partitioner for key-value RDDs(key-value类型的才会有，默认的hash-partition)
- optionally, a list of preferred locations to compute each split
```php
val rdd1=sc.parallelize(List(5,6,4,7,3,8,2,9,1,10))
//排序
rdd1.map(_*10).sortBy(x=>x,true);
//转成字符串排序
rdd1.map(_*10).sortBy(x=>x+"",true); //输出字典顺序
```
```
val rdd4=sc.parallelize(Array("a b c","d e f","h i j"))
rdd4.flatMap(_.split(' ').collect)
```
```
val rdd5=sc.parallelize(List("a b c","a b b",List("e f g","a f g"),List("h i j","a b b" )
rdd5.flatMap(_.flatMap(_.split(" "))).collect
```
## union求并集
```
val rdd6=sc.parallelize(List(5,6,4,7))
val rdd7=sc.parallelize(List(1,2,3,4))
var rdd8=rdd6.union(rdd7)
rdd8.disttinct.sortBy(x=>x).collect
```
## intersection求交集
```
val rdd9=rdd6.intersection(rdd7)  //4
```
## 元组交集
```
val rdd1=sc.parallelize(List(("tom",1),("jerry",2),("kitty",3)))
val rdd2=sc.parallelize(List(("jerry",9),("tom",8),("shuke",7)))
```
rdd1.intersection(rdd2)  //空，因为上述元组没有一样的 
简单的元组intersection不一样
### join
#### case1
```
val rdd3=rdd1.join(rdd2)
//Array[(String,(Int,Int))]=Array((tom,(1,8)),(jerry,(2,9)))
```
#### case2
```
val rdd1=sc.parallelize(List(("tom",1),("jerry",2),("kitty",3)))
val rdd2=sc.parallelize(List(("jerry",9),("tom",8),("shuke",7),("tom",2)))
val rdd3=rdd1.join(rdd2)
rdd3.collect
//res27:Array[(String,(Int,Int)))]=Array((tom,(1,8)),(tom,(1,2)),(jerry,(2,9))
```
### leftOuterJoin左外
//如果左边没有，也会显示出来
```
val rdd3=rdd1.leftOuterJoin(rdd2);
//Array(tom,(1,some(8))),(tom,(1,some(2))),(jerry,(2,some(9))),(kitty,(3,none))
//左右join上的话，就会显示some，右边没有的话就会显示none
```
### rightOuterJoin左外
```
//如果右边没有，也会显示出来
val rdd3=rdd1.rightOuterJoin(rdd2)
Array((tom,(Some(1),8)), (tom,(Some(1),2)), (jerry,(Some(2),9)), (shuke,(None,7)))
//右边存在tom8，tom2，jerry9，shuke7，左边存在tom1，jerry2及其他
```
### List 之间union
val rdd3=rdd1 union rdd2 
rdd3.collect 
//Array((tom,1),(jerry,2),(kitty,3),(jerry,9),(tom,8),(shuke,7),(tom,2))
### groupByKey
```
val rdd3=rdd1 union rdd2
val rdd4=rdd3.groupByKey
//Array((tom,CompactBuffer(1,8,2)),(jerry,CompactBuffer(9,2)),(shuke,CompactBuffer(7)),(kitty,CompactBuffer(3)))
```
### 分区
分区数量：rdd1.partitions.lenth
