
# Hadoop配置文件与HBase配置文件 - 我和我追逐的梦~~~ - CSDN博客


2014年05月09日 21:55:02[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：7266


本Hadoop与HBase集群有1台NameNode, 7台DataNode
1. /etc/hostname文件
NameNode:
node1
DataNode 1:
node2
DataNode 2:
node3.......
DataNode 7:
node8
2. /etc/hosts文件
NameNode:

```python
127.0.0.1	localhost
#127.0.1.1	node1
#-------edit by HY(2014-05-04)--------
#127.0.1.1	node1
125.216.241.113 node1
125.216.241.112 node2
125.216.241.96 node3
125.216.241.111 node4
125.216.241.114 node5
125.216.241.115 node6
125.216.241.116 node7
125.216.241.117 node8
#-------end edit--------
# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
```
DataNode 1:

```python
127.0.0.1	localhost
#127.0.0.1	node2
#127.0.1.1	node2
#--------eidt by HY(2014-05-04)--------
125.216.241.113 node1
125.216.241.112 node2
125.216.241.96 node3
125.216.241.111 node4
125.216.241.114 node5
125.216.241.115 node6
125.216.241.116 node7
125.216.241.117 node8
#-------end eidt---------

# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
```
其他的DataNode类似,只是注意要保持hostname与hosts中的域名要一样, 如果不一样, 在集群上跑任务时会出一些莫名奇妙的问题, 具体什么问题忘记了.
3. 在hadoop-env.sh中注释
\# export JAVA_HOME=/usr/lib/j2sdk1.5-sun
增加
JAVA_HOME=/usr/lib/jvm/java-6-sun
4. core-site.xml

```python
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<!-- Put site-specific property overrides in this file. -->
<configuration>
<property>  
 <name>fs.default.name</name>  
  <value>hdfs://node1:49000</value>  
</property>  
<property>  
  <name>hadoop.tmp.dir</name>  
 <value>/home/hadoop/newdata/hadoop-1.2.1/tmp</value>  
</property> 
<property>
<name>io.compression.codecs</name>
<value>org.apache.hadoop.io.compress.DefaultCodec,org.apache.hadoop.io.compress.GzipCodec,org.apache.hadoop.io.compress.BZip2Codec,com.hadoop.compression.lzo.LzoCodec,com.hadoop.compression.lzo.LzopCodec</value>
</property>
<property>
<name>io.compression.codec.lzo.class</name>
<value>com.hadoop.compression.lzo.LzoCodec</value>
</property>
   <property>
     <name>dfs.datanode.socket.write.timeout</name>
     <value>3000000</value>
   </property>
 
   <property>
     <name>dfs.socket.timeout</name>
     <value>3000000</value>
   </property>
</configuration>
```
5. hdfs-site.xml

```python
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<!-- Put site-specific property overrides in this file. -->
<configuration>
<property>  
<name>dfs.name.dir</name>  
<value>/home/hadoop/newdata/hadoop-1.2.1/name1,/home/hadoop/newdata/hadoop-1.2.1/name2</value>
<description>数据元信息存储位置</description>  
</property>  
<property>  
<name>dfs.data.dir</name>  
<value>/home/hadoop/newdata/hadoop-1.2.1/data1,/home/hadoop/newdata/hadoop-1.2.1/data2</value>  
<description>数据块存储位置</description>  
</property>  
<property>  
  <name>dfs.replication</name>  
  <!-- 这里备份两份 -->  
  <value>2</value>  
</property>  
</configuration>
```

6. mapred-site.xml

```python
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<!-- Put site-specific property overrides in this file. -->
<configuration>
<property>  
  <name>mapred.job.tracker</name>  
  <value>node1:49001</value>  
</property>  
<property>  
  <name>mapred.local.dir</name>  
 <value>/home/hadoop/newdata/hadoop-1.2.1/tmp</value>  
</property>
<property>
<name>mapred.compress.map.output</name>
<value>true</value>
<!-- map 和 reduce 输出中间文件默认开启压缩 -->
</property>
<property>
<name>mapred.map.output.compression.codec</name>
<value>com.hadoop.compression.lzo.LzoCodec</value>
<!-- 使用 Lzo 库作为压缩算法 -->
</property>
</configuration>
```

7. masters

```python
node1
```

8. slaves

```python
node2
node3
node4
node5
node6
node7
node8
```

9. 在hbase-env.sh
增加
JAVA_HOME=/usr/lib/jvm/java-6-sun
并启用export HBASE_MANAGES_ZK=true //为true表示使用自带的Zookeeper, 如果需要独立的Zookeeper,则设置为false, 并且安装Zookeeper
10. hbase-site.xml

```python
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<!--
/**
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
-->
<configuration>
    <property>
        <name>hbase.rootdir</name>
        <value>hdfs://node1:49000/hbase</value>
        <description>The directory shared by RegionServers.</description>
    </property>
    <property>
        <name>hbase.cluster.distributed</name>
        <value>true</value>
        <description>The mode the cluster will be in. Possible values are
            false: standalone and pseudo-distributed setups with managed Zookeeper
            true: fully-distributed with unmanaged Zookeeper Quorum (see hbase-env.sh)
        </description>
    </property>
    <property>
        <name>hbase.master</name>
        <value>node1:60000</value>
        <description>
        </description>
    </property>
    <property>
        <name>hbase.tmp.dir</name>
        <value>/home/hadoop/newdata/hbase/tmp</value>
        <description>
            Temporary directory on the local filesystem.
            Change this setting to point to a location more permanent than '/tmp',
            the usual resolve for java.io.tmpdir,
            as the '/tmp' directory is cleared on machine restart.
            Default: ${java.io.tmpdir}/hbase-${user.name}
        </description>
    </property>
    <property>
        <name>hbase.zookeeper.quorum</name>
        <value>node2,node3,node4,node5,node6,node7,node8</value>
        <description>
            要单数台,Comma separated list of servers in the ZooKeeper ensemble (This config.
            should have been named hbase.zookeeper.ensemble).
            For example, "host1.mydomain.com,host2.mydomain.com,host3.mydomain.com".
            By default this is set to localhost for local and pseudo-distributed
            modes of operation.
            For a fully-distributed setup,
            this should be set to a full list of ZooKeeper ensemble servers.
            If HBASE_MANAGES_ZK is set in hbase-env.sh this is the list of servers
            which hbase will start/stop ZooKeeper on as part of cluster start/stop.
            Client-side, we will take this list of ensemble members and put it
            together with the hbase.zookeeper.clientPort config.
            and pass it into zookeeper constructor as the connectString parameter.
            Default: localhost
        </description>
    </property>
    <property>
        <name>hbase.zookeeper.property.dataDir</name>
        <value>/home/hadoop/newdata/zookeeper</value>
        <description>
            Property from ZooKeeper's config zoo.cfg.
            The directory where the snapshot is stored.
            Default: ${hbase.tmp.dir}/zookeeper
        </description>
    </property>
    <property>
        <name></name>
        <value></value>
    </property>
</configuration>
```

11. regionservers

```python
node2
node3
node4
node5
node6
node7
node8
```

每台机器配置都要一样


