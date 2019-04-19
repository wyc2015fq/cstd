# RHadoop : Reading CSV using rhdfs - 三少GG - CSDN博客
2014年03月12日 18:49:54[三少GG](https://me.csdn.net/scut1135)阅读数：2479
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

# [RHadoop:#104] How to read hdfs file into data frame
[http://grokbase.com/t/gg/rhadoop/125qyh30m3/104-how-to-read-hdfs-file-into-data-frame](http://grokbase.com/t/gg/rhadoop/125qyh30m3/104-how-to-read-hdfs-file-into-data-frame)
# [RHadoop : Reading CSV using rhdfs](http://girishkathalagiri.blogspot.com/2012/11/rhadoop-reading-csv-using-rhdfs.html)
Here is a small code snippet on how to read the csv data from
 HDFS using rhdfs (RHadoop)
rhdfs uses rJava and the buffersize is limited by the heapsize. By default the size of the buffer is set to 5Mb in rhdfs. The source code for rhdfs can be found [here.](https://github.com/RevolutionAnalytics/RHadoop/tree/master/rhdfs)
HADOOP_CMD environment should point to the hadoop.
Sys.setenv(HADOOP_CMD="/bin/hadoop")
library(rhdfs)
hdfs.init()
f = hdfs.file("fulldata.csv","r",buffersize=104857600)
m = hdfs.read(f)
c = rawToChar(m)
data = read.table(textConnection(c), sep =",")## Alternatively You can use hdfs.line.reader()
reader = hdfs.line.reader("fulldata.csv")
 
x = reader$read()
typeof(x)## [1] "character"
