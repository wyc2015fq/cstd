# copyFromLocal异常DataNode启动不了 - BlueSky - CSDN博客
2015年11月17日 17:17:46[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：347标签：[异常																[hdfs																[hadoop](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=hdfs&t=blog)](https://so.csdn.net/so/search/s.do?q=异常&t=blog)
个人分类：[Spark Learning](https://blog.csdn.net/ASIA_kobe/article/category/5912555)
copyFromLocal: File /user/apple/test.txt.*COPYING* could only be replicated to 0 nodes instead of minReplication (=1). There are 0 datanode(s) running and no node(s) are excluded in this operation.
刚才执行 hdfs dfs -copyFromLocal 命令的时候报上面的这个异常，用jps命令一看才知道datanode线程没有启动。 重启也没有用，重新格式化namenode 也不行。 原来需要先将hdfs根目录里面的dfs/data 这个文件夹删除，然后重新对namenode格式化才行。
