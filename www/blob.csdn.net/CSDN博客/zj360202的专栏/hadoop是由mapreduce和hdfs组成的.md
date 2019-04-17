# hadoop是由mapreduce和hdfs组成的 - zj360202的专栏 - CSDN博客





2013年01月26日 10:11:00[zj360202](https://me.csdn.net/zj360202)阅读数：556
个人分类：[hbase																[Hadoop](https://blog.csdn.net/zj360202/article/category/1298102)](https://blog.csdn.net/zj360202/article/category/1298894)








1、mapreduce就是hadoop的api，如果在使用hadoop的mapper和reducer的api的时候就是在使用mapreduce，就会启动hadoop集群，利用hadoop集群解决问题，如果在开发中没有使用hdfs://master:9000这样的输入输出等，就不会使用到hdfs，换言之，就是hdfs就是一个存储系统，在不安装hadoop的情况下，只使用hadoop的api也就是在使用mapreduce,但是想使用Hdfs就必须安装hadoop，这样在使用的时候，就可以使用hdfs://master:9000/的方式使用hdfs

2、在运行hadoop的wordcount的时候，run as  在java application的情况下也是可以使用的，无论你有没有按hadoop，有没有启hadoop服务，但是在输入输出的时候，不能用hdfs://master:9000的，因为这是使用到hdfs，可以直接写系统路径

3、如果测试是在windows下的eclipse下面的话，一定要安装cygwin，并且配置cygwin的bin，到环境变量，才能运行，因为有些命令是要在linux下环境运行的，wordcount可以不用，如果想操作hbase，是要的。




