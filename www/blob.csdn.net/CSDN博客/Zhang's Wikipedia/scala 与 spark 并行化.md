
# scala 与 spark 并行化 - Zhang's Wikipedia - CSDN博客


2019年01月30日 19:06:13[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：114



## 1.
## .par
## 普通集合转换为并行集合
scala.collection:
scala> (1 to 5).foreach(println(_))
12345
scala> (1 to 5).par.foreach(println(_))
13254注意既然是并行化处理，即一个集合不同的元素由不同的线程操作，不同线程执行的顺序是无法保证的；
获取线程名：
scala> (0 to 10000).collect { case _ => Thread.currentThread.getName}.distinct
scala.collection.immutable.IndexedSeq[String] = Vector(main)
scala> (0 to 10000).par.collect { case _ => Thread.currentThread.getName}.distinct
scala.collection.parallel.immutable.ParSeq[String] = ParVector(ForkJoinPool-1-worker-15, ForkJoinPool-1-worker-19, ForkJoinPool-1-worker-23, ForkJoinPool-1-worker-3, ForkJoinPool-1-worker-17, ForkJoinPool-1-worker-21
...
.par.fold
scala> (0 to 1000).fold(0)(_+_)
500500		// fold 参数为 0 时，等价于 reduce
scala> (0 to 1000).par.fold(10)(_+_)
501200		// 分配多少个线程，便加了多少次初始值‘

