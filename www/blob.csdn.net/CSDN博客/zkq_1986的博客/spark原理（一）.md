# spark原理（一） - zkq_1986的博客 - CSDN博客





2017年01月25日 16:13:13[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：183








1 惰性计算

利用惰性计算，有两点好处：

1）将真正需要计算的数据集进入shuffle过程，减少带宽IO

2）中间过程的RDD数据要是受损，重新计算一遍较为方便。




2 persist和checkpoint区别

persist(DISK_ONLY)与checkpoint区别为：

persist随着程序结束，被一起删除；checkpoint除非人为，否则一直存储在磁盘。









