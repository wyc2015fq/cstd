# 【Spark】worker、executor、stage、task、partition关系 - zkq_1986的博客 - CSDN博客





2017年12月07日 16:45:45[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：2871








一个物理节点可以有一个或多个worker。

一个worker中可以有一个或多个executor，一个executor拥有多个cpu core和memory。

只有shuffle操作时才算作一个stage。

一个partition对应一个task。




如下示例，总共有4个stage（包括最后一个count），分区数10个，从而task也为10。假如总共的core数为2，那个这10个task会以每次2个的并行度执行。


var a = new Array[(Int,Int)](10000)

    for (i <- 0 until 10000){

        a(i) = (i,i)

    }


    val rdd1 = sc.parallelize(a).coalesce(10,true)

    val rdd2= rdd1.map(x=>x)

    val rdd3 = rdd2.map(x=>x)

    val rdd4 = rdd3.reduceByKey((x,y)=>x+y)


    val rdd5 = rdd4.map(x=>x)

    val rdd6=rdd5.reduceByKey((x,y)=>x+y)


    val rdd7 = rdd6.map(x=>x)

    val rdd8=rdd7.reduceByKey((x,y)=>x+y)

    rdd8.count()




