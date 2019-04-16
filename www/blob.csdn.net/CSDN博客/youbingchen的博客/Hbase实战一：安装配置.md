# Hbase实战一：安装配置 - youbingchen的博客 - CSDN博客





2016年06月21日 16:44:11[youbingchen](https://me.csdn.net/youbingchen)阅读数：707








# HBase 的安装和配置

前面的博客[大数据处理架构Hadoop配置](http://blog.csdn.net/youbingchen/article/details/51707792)已经详细介绍如何安装Hadoop,但是安装完Hadoop，只包含HDFS和MapReduce等核心组件，并不包含HBase，因此,HBase需要独立安装。[HBase下载地址](http://mirrors.cnnic.cn/apache/hbase/1.2.1/)

> 
友情提示：前面的博客[大数据处理架构Hadoop配置](http://blog.csdn.net/youbingchen/article/details/51707792) Hadoop安装配置有三种方式：单机、伪分布、分布式，相对应的Hbase也有三种安装方式。如果是单机直接将二进制的压缩包解压就可以使用了


### Hbase安装
- 解压压缩包
`tar -zxvf hbase-1.2.1-bin.tar.gz -C /usr/local`
2.更改解压缩文件夹名（仅仅为了方便）
`mv /usr/local/hbase-1.2.1 /usr/local/hbase`
3.配置环境变量，将Hbase路径添加到/etc/profile

```
vim  /etc/profile
# 添加以下内容
HBASE_HOME=/usr/local/hbase/bin
export PATH=$PATH:$HADOOP_HOME:$HBASE_HOME
```

4.查看Hbase的版本
`hbase -version`
5.添加Hbase的权限
`chown -R hadoop:hadoop ./hbase`
### Hbase 配置

#### 单机模式配置
- 配置/usr/local/hbase/conf/hbase-env.sh。配置JAVA环境变量，并添加配置HBASE_MANAGES_ZK(这个参数是要Zookeeper来管理)为true。(单机模式建议用Hbase自带的zookeeper)

```bash
vim ./hbase/conf/hbase-env.sh
# 添加下面两句（或者修改文件原有的参数，并去掉注释）
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 # 根据自己的版本，不要照抄
export HBASE_MANAGES_ZK=true
```
- 配置/usr/local/hbase/conf/hbase-site.xml,在启动HBase前需要设置属性hbase.rootdir，用于指定HBase数据的存储位置，因为如果不设置的话，hbase.rootdir默认为/tmp/hbase-${user.name},这意味着每次重启系统都会丢失数据。此处设置为HBase安装目录下的hbase-tmp文件夹即（/usr/local/hbase/hbase-tmp）,添加配置如下：
`vim  /usr/local/hbase/conf/hbase-site.xml`
修改前： 
![这里写图片描述](https://img-blog.csdn.net/20160621164202111)

修改后： 
![这里写图片描述](https://img-blog.csdn.net/20160621164218882)

3.测试
```
cd /usr/local/hbase/
bin/start-hbase.sh
bin/hbase shell
```

安装成功截图: 
![这里写图片描述](https://img-blog.csdn.net/20160621164300446)

4.停止运行Hbase`/usr/local/hbase/bin/stop-hbase.sh`
#### 伪分布配置hbase
- 配置/usr/local/hbase/conf/hbase-env.sh，配置JAVA_HOME，HBASE_CLASSPATH，HBASE_MANAGES_ZK,HBASE_CLASSPATH设置为本机Hadoop安装目录下的conf目录（即/usr/local/hadoop/conf）

```
# 添加下面三句（或者修改文件原有的参数，并去掉注释）
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 # 根据自己的版本，不要照抄
export HBASE_CLASSPATH=/usr/local/hadoop/conf #
export HBASE_MANAGES_ZK=true
```

2.配置/usr/local/hbase/conf/hbase-site.xml，修改hbase.rootdir，指定HBase数据在HDFS上的存储路径；将属性hbase.cluter.distributed设置为true。假设当前Hadoop集群运行在伪分布式模式下，在本机上运行，且NameNode运行在9000端口。
`vim /usr/local/hbase/conf/hbase-site.xml`
修改前: 
![这里写图片描述](https://img-blog.csdn.net/20160621164318830)

修改后： 
![这里写图片描述](https://img-blog.csdn.net/20160621164339737)

3. 接下来测试运行HBase。 

第一步：首先登陆ssh，之前设置了无密码登陆，因此这里不需要密码；再切换目录至/usr/local/hadoop；再启动hadoop，如果已经启动hadoop请跳过此步骤。命令如下：
```
ssh localhost
cd /usr/local/hadoop
./sbin/start-dfs.sh
```

第二步:切换目录至/usr/local/hbase;再启动HBase.命令如下

```
cd /usr/local/hbase/
bin/start-hbase.sh
bin/hbase shell
```

**启动Hadoop—>启动HBase—>关闭HBase—>关闭Hadoop**






