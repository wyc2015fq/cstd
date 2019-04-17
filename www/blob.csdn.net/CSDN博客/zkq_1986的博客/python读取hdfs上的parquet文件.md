# python读取hdfs上的parquet文件 - zkq_1986的博客 - CSDN博客





2018年11月28日 14:55:32[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：115








python读取hdfs上的parquet文件

在使用python做大数据和机器学习处理过程中，首先需要读取hdfs数据，对于常用格式数据一般比较容易读取，parquet略微特殊。从hdfs上使用python获取parquet格式数据的方法(当然也可以先把文件拉到本地再读取也可以)：

1、安装anaconda环境。

2、安装hdfs3。

        conda install hdfs3

3、安装fastparquet。

        conda install fastparquet

4、安装python-snappy。

        conda install python-snappy

如果是无网环境，需要把依赖包下载下来，配置 .condarc 文件，从指定包或者内网服务器安装。

namenode mode:

from hdfs3 import HDFileSystem

from fastparquet import ParquetFile

hdfs = HDFileSystem(host='172.16.6.32', port=8020)

sc = hdfs.open

pf = ParquetFile(filename, open_with=sc)

df = pf.to_pandas()



HA mode:

from hdfs3 import HDFileSystem

from fastparquet import ParquetFile

host = "nameservice1"

conf = {

        "dfs.nameservices":"nameservice1",

        ......

}

hdfs = HDFileSystem(host = host, pars = conf)

......

返回pandas的DataFrame类型。





转载自：[https://blog.csdn.net/haveanybody/article/details/79928203](https://blog.csdn.net/haveanybody/article/details/79928203)



