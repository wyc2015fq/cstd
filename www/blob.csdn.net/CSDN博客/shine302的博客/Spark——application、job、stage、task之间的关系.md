# Spark——application、job、stage、task之间的关系 - shine302的博客 - CSDN博客
2018年05月04日 11:33:42[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：397
Application：通俗讲，用户每次提交的所有的代码为一个application。
Job：一个application可以分为多个job。如何划分job？通俗讲，出发一个final RDD的实际计算为一个job
Stage：一个job可以分为多个stage。根据一个job中的RDD的依赖关系进行划分
Task：task是最小的基本的计算单位。一般是一个块为一个task，大约是128M
