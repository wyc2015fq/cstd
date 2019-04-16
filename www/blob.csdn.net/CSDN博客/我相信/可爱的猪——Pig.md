# 可爱的猪——Pig - 我相信...... - CSDN博客





2014年12月13日 17:16:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1634
所属专栏：[大数据修炼道场](https://blog.csdn.net/column/details/12856.html)









大数据和云计算涉及的技术众多，数据流处理涉及的技术也很多，如storm，spark等，之所以看看pig，是因为我自己是属猪的。




pig 提供了一个基于hadoop并行执行数据流处理的引擎,以Pig Latin来描述数据流，相当于一个有向无环图（DAG），节点代表处理数据的操作符，节点间的向量代表数据流。
pig 提供了类SQL的标准操作，比直接写mapreduce代码更容易维护，主要用于ETL,原生数据研究和迭代处理。

pig 什么数据都吃，也能在非hadoop上并行运算，容易控制和修改，性能较快。

用户提交hadoop任务的那台机器，可以提取出来作为网关机。

Pig核心是Java写的，所以要注意JAVA_HOME环境变量的设置，启动pig的脚本是bash，既可以运行在本地，也可以运行在hadoop集群上，这时需要知道集群的NameNode和JobTracker所在的位置，还可以运行在云服务上（例如AWS的EMR）。

Grunt是Pig的命令行交互工具，注意PIG_CLASSPATH的设定，除了Pig Latin的交互输入，还可以作为访问HDFS的shell，同时提供了控制pig和mapreduce的命令 kill， exec和run。            


