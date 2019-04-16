# hadoop集成RHive - littlely_ll的博客 - CSDN博客





2017年05月03日 09:56:54[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：260








**注：集群中的每个节点都需要安装**

## 将相关tar包和rpm包拷贝至/usr/R3.2/

cd /usr/R3.2

tar zxvf R-3.2.0.tar.gz

## Rpm包更新

rpm  -Uvh  libgcc-4.4.7-17.el6.x86_64.rpm 

rpm  -Uvh  libgomp-4.4.7-17.el6.x86_64.rpm

rpm  -Uvh  mpfr-2.4.1-6.el6.x86_64.rpm

rpm  -Uvh  cpp-4.4.7-17.el6.x86_64.rpm

yum install cloog-ppl

rpm  -Uvh  gcc-4.4.7-17.el6.x86_64.rpm

rpm  -Uvh  libstdc++-4.4.7-17.el6.x86_64.rpm

rpm  -Uvh  libstdc++-devel-4.4.7-17.el6.x86_64.rpm

rpm  -Uvh  gcc-c++-4.4.7-17.el6.x86_64.rpm

rpm  -Uvh  libgfortran-4.4.7-17.el6.x86_64.rpm

rpm  -Uvh  gcc-gfortran-4.4.7-17.el6.x86_64.rpm

## 安装依赖包

yum install  libXt-devel  openssl-devel  readline-devel

## R编译安装

cd R-3.2.0/

./configure –disable-nls –enable-R-shlib

make

make install

cd ../

## 执行R 命令进行rJAVA 、RHive 等模块的安装

R CMD INSTALL rJava_0.9-6.tar.gz

R CMD INSTALL Rserve_1.8-3.tar.gz

R CMD INSTALL RHive_2.0-0.10.tar.gz

## 新建RHIVE 数据存储路径（本地的非HDFS）

```
/code/RHive/data    (master及各节点)
```

## 新建Rserv.conf 文件并写入 “remote enable” 保存到指定的目录

/code/RHive/cfg/Rserv.conf   (master及各节点)

## 修改各个节点以及master 的 /etc/profile 新增环境变量

export RHIVE_DATA=/code/RHive/data

## 将R目录下的lib目录中所有文件上传至HDFS 中的/rhive/lib 目录下

（如果目录不存在手工新建一下即可）

cd /usr/local/lib64/R/lib

hadoop fs -put ./* /rhive/lib

## 在所有节点和master 上执行

R CMD Rserve –RS-conf /code/RHive/cfg/Rserv.conf

telnet master 6311

然后在Master节点telnet所有slave节点，显示 Rsrv0103QAP1 则表示连接成功

## 启动hive远程服务

```
hive --service hiveserver2
```



