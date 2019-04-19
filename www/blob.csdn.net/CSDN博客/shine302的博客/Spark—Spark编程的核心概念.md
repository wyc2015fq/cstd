# Spark—Spark编程的核心概念 - shine302的博客 - CSDN博客
2018年04月18日 08:50:19[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：84
通过一个驱动器程序创建一个SparkContext 和一系列RDD，然后进行并行操作。
    从上层来看，每个Spark 应用都由一个驱动器程序（driver program）来发起集群上的各种
并行操作。驱动器程序包含应用的main 函数，并且定义了集群上的分布式数据集，还对这
些分布式数据集应用了相关操作。
    驱动器程序通过一个SparkContext 对象来访问Spark。这个对象代表对计算集群的一个连
接。shell 启动时已经自动创建了一个SparkContext 对象，是一个叫作sc 的变量。
一旦有了SparkContext，你就可以用它来创建RDD。在例2-1 和例2-2 中，我们调用了
sc.textFile() 来创建一个代表文件中各行文本的RDD。我们可以在这些行上进行各种操
作，比如count()。
要执行这些操作，驱动器程序一般要管理多个执行器（executor）节点。
![](https://img-blog.csdn.net/20180418084855195?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后，我们有很多用来传递函数的API，可以将对应操作运行在集群上
Spark 会自动将函数（比如line.contains("Python")）发到各个执行器节点上。这样，你就可以在单一的
驱动器程序中编程，并且让代码自动运行在多个节点上。
