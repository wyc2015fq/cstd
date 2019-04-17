# Linux Ubuntu server 15.04 amd-64编译Apache Hadoop 2.7.2源码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月20日 11:51:39[boonya](https://me.csdn.net/boonya)阅读数：927标签：[apache																[hadoop																[源码](https://so.csdn.net/so/search/s.do?q=源码&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/boonya/article/category/6559496)

所属专栏：[Hadoop学习实践](https://blog.csdn.net/column/details/14484.html)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)




为解决Unable to load native-hadoop library for your platform 异常需要修改/etc/profile文件下的Hadoop配置：

export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native

export HADOOP_OPTS="-Djava.library.path=$HADOOP_HOME/lib"

注：主要问题是对应的lib下没有64位编译的包。


# 1.环境准备

请参考：[Linux Ubuntu server 15.04 安装JDK、Maven、findbugs、protobuf](http://blog.csdn.net/boonya/article/details/55506386)

# 2.编译Apache Hadoop 2.7.2源码

## 2.1下载Apache Hadoop源码

从hadoop官网下载，下载地址是：[http://hadoop.apache.org/releases.html](http://hadoop.apache.org/releases.html)，下载hadoop-2.7.2-src.tar.gz ，放置于/usr/local/路径下。

## 2.2解压源码

在/usr/local/下加压，命令如下：



`tar -zxvf  hadoop-2.7.2-src.tar.gz`
解压后的全路径是：/usr/local/hadoop-2.7.2-src。


## 2.3编译源码

进入/usr/local/hadoop-2.7.2-src目录，输入如下命令：


`mvn package -DskipTests-Pdist,native,docs`
注：如果没有安装findbugs，把上面命令中的docs去掉即可，就不必生成文档了。


该命令会从外网下载依赖的jar，编译hadoop源码，需要花费很长时间。

## 2.4编译完成后查看

编译后的代码和jar包在/usr/local/hadoop-2.7.2-src/hadoop-dist/target下面。](https://so.csdn.net/so/search/s.do?q=apache&t=blog)




