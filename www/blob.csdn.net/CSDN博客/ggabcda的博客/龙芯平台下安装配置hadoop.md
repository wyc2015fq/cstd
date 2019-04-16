# 龙芯平台下安装配置hadoop - ggabcda的博客 - CSDN博客





2017年09月05日 16:25:47[ggabcda](https://me.csdn.net/ggabcda)阅读数：328










# 一、       hadoop简介

Hadoop是一个由Apache基金会所开发的分布式系统基础架构。

用户可以在不了解分布式底层细节的情况下，开发分布式程序。充分利用集群的威力进行高速运算和存储。

Hadoop实现了一个分布式文件系统（Hadoop Distributed File System），简称HDFS。HDFS有高容错性的特点，并且设计用来部署在低廉的（low-cost）硬件上；而且它提供高吞吐量（high throughput）来访问应用程序的数据，适合那些有着超大数据集（large data set）的应用程序。HDFS放宽了（relax）POSIX的要求，可以以流的形式访问（streaming access）文件系统中的数据。

Hadoop的框架最核心的设计就是：HDFS和MapReduce。HDFS为海量的数据提供了存储，则MapReduce为海量的数据提供了计算。

# 二、       软件介质及安装环境

软件版本：hadoop-2.6.5.tar.gz

下载地址：

http://mirror.bit.edu.cn/apache/hadoop/common/hadoop-2.6.5/hadoop-2.6.5.tar.gz

安装环境：

Master节点服务器 Sugon L620-G15（192.168.32.182）

Slave节点服务器 长城3A单路（192.168.32.153 ）

以下分别简称为master服务器和slave服务器

操作系统：iSoft Server OS 5.0 beta3 for mips

# 三、       安装配置

**master****服务器、****slave****服务器分别进行如下操作：**

关闭防火墙



`[hadoop@master hadoop]$ sudo service iptables stop``[sudo] password for hadoop: ``iptables``：清除防火墙规则：``                                 [``确定``]``iptables``：将链设置为政策`` ACCEPT``：``filter                    [``确定``]``iptables``：正在卸载模块：``                                   [``确定``]``[hadoop@master hadoop]$ sudo chkconfig iptables off`



修改hostname分别为master和slave

Master服务器``

`[root@isoft182 ~]# cat /etc/sysconfig/network``NETWORKING=yes``HOSTNAME=master``GATEWAY=192.168.32.1`



Slave服务器



` [root@test-153 ~]# cat /etc/sysconfig/network``NETWORKING=yes``HOSTNAME=slave``GATEWAY=192.168.32.1`



分别修改hosts文件添加内容如下：

`192.168.32.182 master``192.168.32.153 slave`



**新建hadoop****用户**

进入hadoop

**设为无密码登录slave****服务器**``

`[hadoop@isoft182 ~]$ ssh-keygen``Generating public/private rsa key pair.``Enter file in which to save the key (/home/hadoop/.ssh/id_rsa): ``Enter passphrase (empty for no passphrase): ``Enter same passphrase again: ``Your identification has been saved in /home/hadoop/.ssh/id_rsa.``Your public key has been saved in /home/hadoop/.ssh/id_rsa.pub.``The key fingerprint is:``9b:e0:7e:bb:b4:7c:9b:66:e7:1f:5f:49:30:22:be:d0 hadoop@isoft182``The key's randomart image is:``+--[ RSA 2048]----+``|                 |``|                 |``|         . . o   |``|        o . . o  |``|      ..SE     . |``|     . ..o.   . .|``|      . +.    ...|``|     . o..+..  o.|``|      ..=*o+... .|`` ``[hadoop@isoft182 ~]$ ssh-copy-id slave``hadoop@slave's password: ``Now try logging into the machine, with "ssh 'slave'", and check in:`` ``  .ssh/authorized_keys`` ``to make sure we haven't added extra keys that you weren't expecting.`



**配置java****环境**

~/.bashrc文件最后加入

`export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk`

然后

`source ~./bashrc`

是设置生效。

确认java版本信息

`[hadoop@isoft182 ~]$ java -version``openjdk version "1.8.0_25"``OpenJDK Runtime Environment (build 1.8.0_25-rc19-b17)``OpenJDK 64-Bit Server VM (build 25.25-b02, mixed mode)`



**安装hadoop**``

`sudo tar -zxf hadoop-2.6.5.tar.gz -C /usr/local    # ``解压到``/usr/local``中````cd /usr/local/``sudo mv ./hadoop-2.6.5/ ./hadoop            # ``将文件夹名改为``hadoop``sudo chown -R hadoop:hadoop ./hadoop        # ``修改文件权限```



检查版本信息``

` [hadoop@isoft182 local]$ ./hadoop/bin/hadoop version``Hadoop 2.6.5``Subversion https://github.com/apache/hadoop.git -r e8c9fe0b4c252caf2ebf1464220599650f119997``Compiled by sjlee on 2016-10-02T23:43Z``Compiled with protoc 2.5.0``From source with checksum f05c9fa095a395faa9db9f7ba5d754``This command was run using /usr/local/hadoop/share/hadoop/common/hadoop-common-2.6.5.jar`



**配置hadoop****环境变量**

~/.bashrc文件下添加

`export PATH=$PATH:/usr/local/hadoop/bin:/usr/local/hadoop/sbin`

source .bashrc使其生效。



**修改hadoop****配置文件**

进入目录/usr/local/hadoop/etc/hadoop

修改slaves文件

`[hadoop@master hadoop]$ cat slaves``slave`



修改core-site.xml

`<configuration>``        <property>``                <name>fs.defaultFS</name>``                <value>hdfs://master:9000</value>``        </property>``        <property>``                <name>hadoop.tmp.dir</name>``                <value>file:/usr/local/hadoop/tmp</value>``                <description>Abase for other temporary directories.</description>``        </property>``</configuration>`



修改hdfs-site.xml``

`[hadoop@master hadoop]$ cat hdfs-site.xml``<?xml version="1.0" encoding="UTF-8"?>``<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>``<!--``  Licensed under the Apache License, Version 2.0 (the "License");``  you may not use this file except in compliance with the License.``  You may obtain a copy of the License at`` ``    http://www.apache.org/licenses/LICENSE-2.0`` ``  Unless required by applicable law or agreed to in writing, software``  distributed under the License is distributed on an "AS IS" BASIS,``  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.``  See the License for the specific language governing permissions and``  limitations under the License. See accompanying LICENSE file.``-->`` ``<!-- Put site-specific property overrides in this file. -->`` ``<configuration>``   <property>``           <name>dfs.namenode.secondary.http-address</name>``           <value>master:50090</value>``   </property>``   <property>``            <name>dfs.replication</name>``            <value>1</value>``   </property>``   <property>``          <name>dfs.namenode.name.dir</name>``          <value>file:/usr/local/hadoop/tmp/dfs/name</value>``   </property>``   <property>``           <name>dfs.datanode.data.dir</name>``           <value>file:/usr/local/hadoop/tmp/dfs/data</value>``    </property>``</configuration>`



将mapred-site.xml.template修改mapred-site.xml，内容如下：``

`[hadoop@master hadoop]$ cat mapred-site.xml``<?xml version="1.0"?>``<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>``<!--``  Licensed under the Apache License, Version 2.0 (the "License");``  you may not use this file except in compliance with the License.``  You may obtain a copy of the License at`` ``    http://www.apache.org/licenses/LICENSE-2.0`` ``  Unless required by applicable law or agreed to in writing, software``  distributed under the License is distributed on an "AS IS" BASIS,``  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.``  See the License for the specific language governing permissions and``  limitations under the License. See accompanying LICENSE file.``-->`` ``<!-- Put site-specific property overrides in this file. -->`` ``<configuration>``<property>``            <name>mapreduce.framework.name</name>``            <value>yarn</value>`` </property>``  <property>``              <name>mapreduce.jobhistory.address</name>``              <value>master:10020</value>``  </property>``  <property>``              <name>mapreduce.jobhistory.webapp.address</name>``              <value>master:19888</value>``   </property>``</configuration>`



修改文件 yarn-site.xml``

`[hadoop@master hadoop]$ cat yarn-site.xml``<?xml version="1.0"?>``<!--``  Licensed under the Apache License, Version 2.0 (the "License");``  you may not use this file except in compliance with the License.``  You may obtain a copy of the License at`` ``    http://www.apache.org/licenses/LICENSE-2.0`` ``  Unless required by applicable law or agreed to in writing, software``  distributed under the License is distributed on an "AS IS" BASIS,``  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.``  See the License for the specific language governing permissions and``  limitations under the License. See accompanying LICENSE file.``-->``<configuration>``            <property>``                        <name>yarn.resourcemanager.hostname</name>``                        <value>master</value>``            </property>``            <property>``                        <name>yarn.nodemanager.aux-services</name>``                        <value>mapreduce_shuffle</value>``            </property>``<!-- Site specific YARN configuration properties -->`` ``</configuration>`



将master服务器的/usr/local/hadoop文件复制到slave服务器相应目录下

并执行

`sudo chown -R hadoop:hadoop hadoop/`



master服务器初始化

 Master 节点执行 NameNode 的格式化：

`[hadoop@master ~]$ hdfs namenode -format`

启动hadoop

`start-dfs.sh``start-yarn.sh``mr-jobhistory-daemon.sh start historyserver`



通过命令 jps 可以查看各个节点所启动的进程。在 Master 节点上可以看到 NameNode、ResourceManager、SecondrryNameNode、JobHistoryServer 进程，如下图所示：



`[hadoop@master ~]$ jps``5176 NameNode``5883 Jps``5836 JobHistoryServer``5548 ResourceManager``5341 SecondaryNameNode`



在 Slave 节点可以看到 DataNode 和 NodeManager 进程，如下图所示：

`[hadoop@slave ~]$ jps``5302 Jps``5179 NodeManager``5070 DataNode`



另外还需要在 Master 节点上通过命令 hdfs dfsadmin -report 查看 DataNode 是否正常启动，如果 Live datanodes 不为 0 ，则说明集群启动成功。



`[hadoop@master ~]$  hdfs dfsadmin -report``17/09/01 14:06:58 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable``Configured Capacity: 51471126528 (47.94 GB)``Present Capacity: 43123650560 (40.16 GB)``DFS Remaining: 43123625984 (40.16 GB)``DFS Used: 24576 (24 KB)``DFS Used%: 0.00%``Under replicated blocks: 0``Blocks with corrupt replicas: 0``Missing blocks: 0`` ``-------------------------------------------------``Live datanodes (1):`` ``Name: 192.168.32.153:50010 (slave)``Hostname: slave``Decommission Status : Normal``Configured Capacity: 51471126528 (47.94 GB)``DFS Used: 24576 (24 KB)``Non DFS Used: 8347475968 (7.77 GB)``DFS Remaining: 43123625984 (40.16 GB)``DFS Used%: 0.00%``DFS Remaining%: 83.78%``Configured Cache Capacity: 0 (0 B)``Cache Used: 0 (0 B)``Cache Remaining: 0 (0 B)``Cache Used%: 100.00%``Cache Remaining%: 0.00%``Xceivers: 1``Last contact: Fri Sep 01 14:07:06 CST 2017`



# 四、       执行分布式实例

**执行****WordCount****实例**

进入/usr/local/hadoop目录

创建文件夹data_input，并新建两个文本文件，内容任意

`mkdir data_input``touch file1.txt ``touch file2.txt`



执行以下命令

`./bin/hadoop fs -mkdir /data``./bin/hadoop fs -put -f ./data_input/* /data`



执行WordCount命令，并查看结果：



`./bin/hadoop jar ./share/hadoop/mapreduce/sources/hadoop-mapreduce-examples-2..6.5-sources.jar org.apache.hadoop.examples.WordCount /data /output``17/09/02 12:06:01 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable``17/09/02 12:06:08 INFO client.RMProxy: Connecting to ResourceManager at master/192.168.32.182:8032``17/09/02 12:06:13 INFO input.FileInputFormat: Total input paths to process : 2``17/09/02 12:06:13 INFO mapreduce.JobSubmitter: number of splits:2``17/09/02 12:06:15 INFO mapreduce.JobSubmitter: Submitting tokens for job: job_1504324771910_0002``17/09/02 12:06:18 INFO impl.YarnClientImpl: Submitted application application_1504324771910_0002``17/09/02 12:06:18 INFO mapreduce.Job: The url to track the job: http://master:8088/proxy/application_1504324771910_0002/``17/09/02 12:06:18 INFO mapreduce.Job: Running job: job_1504324771910_0002``17/09/02 12:07:09 INFO mapreduce.Job: Job job_1504324771910_0002 running in uber mode : false``17/09/02 12:07:09 INFO mapreduce.Job:  map 0% reduce 0%``17/09/02 12:07:44 INFO mapreduce.Job:  map 100% reduce 0%``17/09/02 12:08:15 INFO mapreduce.Job:  map 100% reduce 100%``17/09/02 12:08:16 INFO mapreduce.Job: Job job_1504324771910_0002 completed successfully``17/09/02 12:08:18 INFO mapreduce.Job: Counters: 49``            File System Counters``                        FILE: Number of bytes read=122``                        FILE: Number of bytes written=322095``                        FILE: Number of read operations=0``                        FILE: Number of large read operations=0``                        FILE: Number of write operations=0``                        HDFS: Number of bytes read=278``                        HDFS: Number of bytes written=81``                        HDFS: Number of read operations=9``                        HDFS: Number of large read operations=0``                        HDFS: Number of write operations=2``            Job Counters ``                        Launched map tasks=2``                        Launched reduce tasks=1``                        Data-local map tasks=2``                        Total time spent by all maps in occupied slots (ms)=63108``                        Total time spent by all reduces in occupied slots (ms)=27632``                        Total time spent by all map tasks (ms)=63108``                        Total time spent by all reduce tasks (ms)=27632``                        Total vcore-milliseconds taken by all map tasks=63108``                        Total vcore-milliseconds taken by all reduce tasks=27632``                        Total megabyte-milliseconds taken by all map tasks=64622592``                        Total megabyte-milliseconds taken by all reduce tasks=28295168``            Map-Reduce Framework``                        Map input records=10``                        Map output records=8``                        Map output bytes=112``                        Map output materialized bytes=128``                        Input split bytes=196``                        Combine input records=8``                        Combine output records=7``                        Reduce input groups=6``                        Reduce shuffle bytes=128``                        Reduce input records=7``                        Reduce output records=6``                        Spilled Records=14``                        Shuffled Maps =2``                        Failed Shuffles=0``                        Merged Map outputs=2``                        GC time elapsed (ms)=1846``                        CPU time spent (ms)=20900``                        Physical memory (bytes) snapshot=699613184``                        Virtual memory (bytes) snapshot=5563170816``                        Total committed heap usage (bytes)=603979776``            Shuffle Errors``                        BAD_ID=0``                        CONNECTION=0``                        IO_ERROR=0``                        WRONG_LENGTH=0``                        WRONG_MAP=0``                        WRONG_REDUCE=0``            File Input Format Counters ``                        Bytes Read=82``            File Output Format Counters ``                        Bytes Written=81`



上面的日志显示出了wordCount的详细情况，然后执行查看结果命令查看统计结果：

` `

`[hadoop@master hadoop]$ ./bin/hadoop fs -cat /output/part-r-00000``17/09/02 12:11:19 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable``--file1.txt 1``--file2.txt 1``dddd        2``ddddd       2``dddeeeeeeeeeeeee        1``dddkkkkkkkkkkkkk        1`



wordCount统计结果完成。



关闭 Hadoop 集群也是在 Master 节点上执行的：

`stop-yarn.sh``stop-dfs.sh``mr-jobhistory-daemon.sh stop historyserver`



# 五、       问题与解决方法

1)         启动hadoop服务时会提示：17/09/01 14:42:01 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable

解决办法：原来系统预装的glibc库是2.12版本，而hadoop期望是2.14版本，所以打印警告信息。

方法一：重新编译glibc.2.14版本，安装后专门给hadoop使用。

方法二：直接在log4j日志中去除告警信息。在etc/hadoop/log4j.properties文件中添加

`            log4j.logger.org.apache.hadoop.util.NativeCodeLoader=ERROR`

         第一种方法风险比较大，本文档使用第二种方法。

2)         Master 节点执行 NameNode 的格式化后，在slave节点无法启动DataNode服务

解决方法：这是由于格式化后，会导致master节点的tmp/dfs/name/current/VERSION文件中的clusterID与slave节点下的tmp/dfs/data/current/VERSION文件中的clusterID不一致，所以DataNode无法启动。将slave下的clusterID替换为master下的clusterID即可解决问题。



3)         执行WordCount实例时，有超时现象，remote无法到达，而且使用hdfs dfsadmin –report命令，可以看到datanodes的hostname为一个ip地址，172.16.0.1

解决方法：这是由于之前有同事在slave节点服务器上安装了docker服务，使用过网桥设备，使得系统中多了一个无用的网卡设备，关闭docker服务重启主机即可。





