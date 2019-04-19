# Spark编译安装 - LC900730的博客 - CSDN博客
2017年12月10日 22:09:52[lc900730](https://me.csdn.net/LC900730)阅读数：122
1.下载并安装maven 
2.安装protubuf(需要解决依赖yum install -y gcc gcc-c++ make) 
3.解压安装protobuf 
tar zxvf protubuf-2.5.0.tar.gz -C /usr/local/src/ 
cd /usr/local/src/protobuf-2.5.0 
./configure –prefix=/usr/local/protobuf 
make && make install
4.编译其他安装 
yum install -y cmake openssl-devel ncurses-devel zlib-devel 
yum install -y snappy snappy-devel  bzip2 bzip2-devel lzo 
 lzo-devel lzop openssl openssl-devel  
5.编译hadoop 
tar zxvf hadoop-2.6.4-src.tar.gz -C /usr/local/src/ 
cd /usr/local/src/hadoop-2.6.4-src
mvn package -Pdist,native -DskipTests -Dtar
6.查看编译好的安装包 
cd /usr/local/src/hadoop-2.6.4-src/hadoop-dist/target 
然后 
export HADOOP_OPTS=”-Djava.library.path=$HADOOP_HOME/lib/native” 
6.查看可用的本地库 
hadoop checknative -a
