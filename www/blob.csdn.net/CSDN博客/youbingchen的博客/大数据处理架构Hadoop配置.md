# 大数据处理架构Hadoop配置 - youbingchen的博客 - CSDN博客





2016年06月18日 23:28:30[youbingchen](https://me.csdn.net/youbingchen)阅读数：3232








title: 大数据处理架构Hadoop 

date: 2016-06-02 

tags: Hardoop
# Hadoop简介
- 
Hadoop是Apache软件基金会旗下的一个开源分布式计算平台，为用户提供了系统底层细节透明的分布式基础架构

- 
Hadoop是基于Java语言开发的，具有很好的跨平台特性，并且可以部署在廉价的计算机集群中（但是具体在应用层不一定只用java）

- 
Hadoop的核心是分布式文件系统HDFS（Hadoop Distributed File System）和MapReduce

- 
Hadoop被公认为行业大数据标准开源软件，在分布式环境下提供了海量数据的处理能力

- 
几乎所有主流厂商都围绕Hadoop提供开发工具、开源软件、商业化工具和技术服务，如谷歌、雅虎、微软、思科、淘宝等，都支持Hadoop


# hardoop的特性
- 高可靠性
- 高效性
- 高扩展性
- 高容错性
- 成本低
- 运行在linux平台上
- 支持多种编程语言

# hadoop 安装(Ubuntu为例)

针对在单机的学习主要采用的是单机/伪分布式配置。
- 第一步（可选） 可以创建一个新的用户

```
# 创建hadoop用户并使用/bin/bash作为shell
sudo useradd -m haddoop -s /bin/shell
# 设置密码
sudo passwd hadoop 
# 可为hadoop用户增加管理员权限
#  sudo adduser hadoop sudo
```
- 第二步(可选） 更新源 

Ubuntu可以在[Ubuntu源列表](http://wiki.ubuntu.org.cn/%E6%BA%90%E5%88%97%E8%A1%A8)获取最新的源。

```
# 备份系统自带的源
sudo cp /etc/apt/sources.list /etc/apt/sources.list_backup
# 用编辑器编辑(一定要选对版本)
vim /etc/apt/sources.list
# 执行刷新
sudo apt-get update
```
- 第三步 安装SSH、配置SSH无密码登录 

集群、单节点模式都需要用到SSH登录（类似远程登录）,Ubuntu默认安装了SSH client,此外还需要安装SSH server

```bash
sudo apt-get install openssh-server
# ssh安装后测试一下
ssh -l hadoop localhost
# 如果每次登录都要重新输入密码不方便的话，可以利用ssh-keygen生成密钥，并将密钥加入授权中
exit # 先退出
cd ~/.ssh
ssh-keygen -t rsa
cat ./id_rsa.pub >> ./authorized_keys
```
- 第四步（必须的）安装Java环境 

java环境可选择Oracle的JDK，或是OpenJDK，这边直接通过命令安装OpenJDK7

```
sudo apt-get install openjdk-8-jre 
sudo apt-get install openjdk-8-jdk
# 安装完OpenJDK，找到相应的路径，这个路径就是用于配置
dpkg -L openjdk-8-jdk | grep '/bin/javac'
# 输出的结果，去除末尾的"/bin/javac",剩下的就是正确的路径，将这个路径写入到~/.bashrc
vim ~/.bashrc
# 在文件的末尾加入（注意=两边不能有空格）
export JAVA_HOME=JDK路径
# 让环境变量立即生效
source ~/.bashrc
# 测试一下java 是否成功
java -version
```
- 第五步（必须的）安装Hadoop2 

Hadoop可以通过[hadoop 下载](http://mirrors.cnnic.cn/apache/hadoop/common/),一般选择稳定的版本(stable)。建议选择下载编译好的(因为是在自己的电脑)，如果要应用现实的集群中可以选择源码进行编译

```
# 将Hadoop安装至/usr/local/中
sudo tar -zxvf hadoop-x.x.x.tar.gz -C /usr/local
cd /usr/local
sudo mv ./hadoop-x.xcd.x ./hadoop # 为了方便
sudo chown -R hadoop ./hadoop # 修改文件权限
cd /usr/local/hadoop
./bin/hadoop version #验证是否正确安装hadoop
```
- 第六步 单机配置（非分布式） 
**到这一步只是单机配置，并不是分布式**

```
cd /usr/local/hadoop
mkdir ./input
cp ./etc/hadoop/*.xml ./input # 将配置文件作为输入文件
./bin/hadoop jar ./share/hadoop/mapreduce/hadoop-mapreduce-examples-*.jar grep ./input ./output 'dfs[a-z.]+'
cat ./output/* # 查看运行结果
```

![这里写图片描述](https://img-blog.csdn.net/20160618231743969)

注意:Hadoop默认不会覆盖文件，因此再次运行实例提示会出错的，需要将./output文件删除

+ 第七步Hadoop伪分布配置 

Hadoop 可以在单点上以分布式的方式运行，Hadoop进程以分离的Java进程运行，节点即作为NameNode也作为DataNode,同时读取HDFS的文件。 

Hadoop的配置文件位于/usr/local/hadoop/etc/hadoop/中。伪分布式主要需要修改2个配置文件**core-site.xml**和**hdfs-site.xml**。Hadoop的配置文件是xml格式的，每个配置以声明property的name和value方式实现的。 

在这之前先把Hadoop加入环境变量
```bash
vim  ~/.bashrc
   # 添加以下内容
export HADOOP_HOME=/usr/local/hadoop
export HADOOP_INSTALL=$HADOOP_HOME
export HADOOP_MAPRED_HOME=$HADOOP_HOME
export HADOOP_COMMON_HOME=$HADOOP_HOME
export HADOOP_HDFS_HOME=$HADOOP_HOME
export YARN_HOME=$HADOOP_HOME
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
export PATH=$PATH:$HADOOP_HOME/sbin:$HADOOP_HOME/bin
#　关闭 文件，用source命令使配置文件立马生效
source ~/.bashrc
```

> 
主要步骤有：

- 修改配置文件:core-site.xml,hdfs-site.xml,mapred-site.xml
- 初始化文件系统hadoop namenode-format
- 启动所有进程start-all.sh

### 配置文件修改

#### core-site.xml文件修改
`vim ./etc/hadoop/core-site.xml`
![这里写图片描述](https://img-blog.csdn.net/20160618232459708)

修改成: 
![这里写图片描述](https://img-blog.csdn.net/20160618232516333)
#### hdfs-site.xml文件修改

![这里写图片描述](https://img-blog.csdn.net/20160618232533349)


dfs.replication表示副本，伪分布设置为1 

dfs.namenode.name.dir表示本地磁盘目录，是存储fsimage的地方 

dfs.datanode.data.dir表示本地磁盘目录，HDFS数据存放block的地方 

![这里写图片描述](https://img-blog.csdn.net/20160618232557350)
#### Hadoop配置文件说明

> 
1.Hadoop 的运行方式是由配置文件决定的（运行 Hadoop 时会读取配置文件），因此如果需要从伪分布式模式切换回非分布式模式，需要删除 core-site.xml 中的配置项。 

  2.此外，伪分布式虽然只需要配置 fs.defaultFS 和 dfs.replication就可以运行（官方教程如此），不过若没有配置 hadoop.tmp.dir 参数，则默认使用的临时目录为 /tmp/hadoo-hadoop，而这个目录在重启时有可能被系统清理掉，导致必须重新执行format才行。所以我们进行了设置，同时也指定 dfs.namenode.name.dir和dfs.datanode.data.dir，否则在接下来的步骤中可能会出错。


#### 执行NameNode的格式化
`./bin/hdfs namenode -format # 若出现了successfullyu formatted“和”exittng with status 0“,则是成功，否则出现"Exitting with status1",则是出错。`
#### 开启NameNode和DataNode守护进程
`./sbin/start-dfs.sh  # start-dfs.sh 是个完整的可执行文件。中间没有空格`
> 
1.这一步可能会出现JAVA_HOME没设置，这时候要在 /etc/hadoop/hadoop-env.sh添加 export JAVA_HOME 路径

2.也可能会出现启动 Hadoop 时提示 Could not resolve hostname，这个并不是ssh的问题，在~/.bashrc中添加两行


```bash
export HADOOP_HOME=/usr/local/hadoop
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native # 不要忘了 source ~/.bashrc
```

#### jps命令判断是否正常

![这里写图片描述](https://img-blog.csdn.net/20160618232621866)

+ 第八步运行Hadoop伪分布式实例 

上面的单机模式，grep例子读取的是本地数据，伪分布式读取的则是 HDFS 上的数据。要使用 HDFS，首先需要在 HDFS中创建用户目录：`./bin/hdfs dfs -mkdir -p /user/hadoop`
注意:

> 
这里有三张shell 命令


```
./bin/hadoop fs
./bin/hadoop dfs
./hdfs dfs
```

说明:
- hadoop fs适用于任何不同的系统，比如本地文件系统和HDFS文件系统
- hadoop dfs只能适用于HDFS文件系统
- hdfs dfs跟hadoop dfs的命令作用是一样的，也只能适用于HDFS文件系统

接下来将./etc/hadoop中的xml文件作为输入文件复制到分布式文件系统中，即将/usr/local/hadoop/etc/hadoop复制到分布式文件系统中的/user/hadoop/input中。我们使用的是hadoop用户,并且已创建用户目录/user/hadoop。因此在命令中就可以使用相对路径如:input,其对应的绝对路径就是/user/hadoop/input:

```
./bin/hdfs dfs -mkdir input
./bin/hdfs dfs -put ./etc/hadoop/*.xml input
```

复制完成之后，通过命令可以查看情况了
`./bin/hdfs dfs -ls input`
伪分布式运行 MapReduce作业的方式跟单机模式相同，区别在于伪分布式读取的是HDFS中的文件（可以将单机步骤中创建的本地 input 文件夹，输出结果output文件夹都删掉来验证这一点）。 

查看结果

```
./bin/hdfs dfs -cat output/*
```

我们也可以将运行结果取回到本地：
`./bin/hdfs dfs -get output ./output`
Hadoop 运行程序时，输出目录不能存在，否则会提示错误 “org.apache.hadoop.mapred.FileAlreadyExistsException: Output directoryhdfs://localhost:9000/user/hadoop/output already exists” ，因此若要再次执行，需要执行如下命令删除 output 文件夹:
`./bin/hdfs dfs -rm -r output`
若要关闭 Hadoop，则运行
`./sbin/stop-dfs.sh`
注意:下次启动hadoop时候，无需NameNode的初始化，只需要./sbin/start-dfs.sh即可。

+ 第九步YARN的安装（可选） 

因为伪分布式不启动YARN也可以的，一般不会影响程序执行 

YARN 是从MapReduce中分离出来的，负责资源管理和任务调度。上述步骤只是Hadoop中的MapReduce环境，针对启动YARN步骤如下： 

首先配置文件 mapred-site.xml`mv ./etc/hadoop/mapred-site.xml.template ./etc/hadoop/mapred-site.xml`
并添加内容 
![这里写图片描述](https://img-blog.csdn.net/20160618232650507)

接下来修改yarn-site.xml 
![这里写图片描述](https://img-blog.csdn.net/20160618232705054)

然后就可以启动 YARN了（需要先执行过./sbin/start-dfs.sh
```
./sbin/start-yarn.sh #启动yarn
./sbin/mr-jobhistory-daemon.sh start historyserver #开启历史服务器

开启后通过 jps 查看，可以看到多了 NodeManager 和 ResourceManager 两个后台进程。
```

同样的，关闭 YARN 的脚本如下：

```
./sbin/stop-yarn.sh #启动yarn
./sbin/mr-jobhistory-daemon.sh stop historyserver
```
- 最后一步 搭建真正Hadoop集群 
[Hadoop集群搭建教程](http://dblab.xmu.edu.cn/blog/install-hadoop-cluster/)










