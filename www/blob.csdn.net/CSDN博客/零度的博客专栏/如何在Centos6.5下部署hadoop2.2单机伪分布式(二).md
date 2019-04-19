# 如何在Centos6.5下部署hadoop2.2单机伪分布式(二) - 零度的博客专栏 - CSDN博客
2016年10月13日 17:07:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：300
hadoop2.x的稳定版本，已经发布多时了，散仙一直都用的hadoop1.x的版本，最近有项目用到hadoop2.x，所以就开始折腾hadoop2.x了，在hadoop2.x的版本里，目前最新版本已经到hadoop2.4了，但散仙在这里用的是hadoop2.2.0的版本，为什么用它？ 因为这是一个稳定的版本，所以相对来说，问题比较少。
散仙在上篇博客，已经讲了如何在编译hadoop2.2.0的64位的版本，如果有不清楚怎么编译，可以参考散仙上篇博文，在这里就不涉及了。 
下面，散仙以表格的形式描述下伪分布式安装 
系统环境如下： 
|序号|名称|描述|
|----|----|----|
|1|系统环境|Centos6.5|
|2|JAVA环境|JDK1.7|
|3|Haoop版本|hadoop2.2.0|
安装步骤如下： 
|序号|步骤|
|----|----|
|1|解压hadoop，并配置环境变量|
|2|使用which hadoop命令，测试是否成功|
|3|配置core-site.xml|
|4|配置hdfs-site.xml|
|5|配置yarn-site.xml（非必须，使用默认也可）|
|6|配置mapred-site.xml|
|7|配置mapred-env.sh里面的JAVA路径|
|8|如上的配置完成后，需要新建几个文件夹，来提供HDFS的格式化底层一个是hadoop的tmp文件夹，另外的是namenode和datanode的文件夹，来分别存储各自的信息|
|9|上面一切完成后，即可执行hadoop namenode -format 进行格式化|
|10|然后启动伪分布式集群，执行命令sbin/start-all.sh 最后使用JPS查看JAVA进程|
|11|配置本机的hosts文件，映射主机名信息|
下面来详细说下，各个步骤的具体操作： 
在这之前，最好配置好本机的SSH无密码登陆操作，避免hadoop进程之间通信时，需要验证。 
下面是各个xml文件的配置 
Xml代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- <!-- core-site.xml配置 -->
- <configuration>
- <property>
- <name>fs.default.name</name>
- <value>hdfs://192.168.46.28:9000</value>
- </property>
- <property>
- <name>hadoop.tmp.dir</name>
- <value>/root/hadoop/tmp</value>
- </property>
- 
- </configuration>
<span style="font-family:Arial;font-size:12px;"><!-- core-site.xml配置 -->
<configuration>
    <property>  
        <name>fs.default.name</name>  
        <value>hdfs://192.168.46.28:9000</value>  
    </property>  
  <property>
    <name>hadoop.tmp.dir</name>
    <value>/root/hadoop/tmp</value>
  </property>
</configuration></span>
Xml代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- <!-- hdfs-site.xml配置 -->
- 
- <configuration>
- <property>
- <name>dfs.replication</name>
- <value>1</value>
- </property>
- 
- <property>
- <name>dfs.namenode.name.dir</name>
- <value>/root/hadoop/nddir</value>
- </property>
- 
- 
- <property>
- <name>dfs.datanode.data.dir</name>
- <value>/root/hadoop/dddir</value>
- </property>
- 
- <property>
- <name>dfs.permissions</name>
- <value>false</value>
- </property>
- 
- </configuration>
<span style="font-family:Arial;font-size:12px;"><!-- hdfs-site.xml配置 -->
 
<configuration>
<property>  
   <name>dfs.replication</name>  
   <value>1</value>  
 </property>  
 <property>  
   <name>dfs.namenode.name.dir</name>  
   <value>/root/hadoop/nddir</value>  
 </property>  
 <property>  
   <name>dfs.datanode.data.dir</name>  
   <value>/root/hadoop/dddir</value>  
 </property>  
<property>  
  <name>dfs.permissions</name>  
  <value>false</value>  
</property>
</configuration>
</span>
Xml代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- <!--    yarn-site.xml  不用配置，使用默认属性即可 -->
- <configuration>
- 
- 
- </configuration>
<span style="font-family:Arial;font-size:12px;"><!--    yarn-site.xml  不用配置，使用默认属性即可 -->
<configuration>
</configuration>
</span>
Xml代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- <!--  mapred-site.xml的配置 -->
- <configuration>
- <property>
- <name>mapreduce.cluster.temp.dir</name>
- <value></value>
- <description>No description</description>
- <final>true</final>
- </property>
- 
- <property>
- <name>mapreduce.cluster.local.dir</name>
- <value></value>
- <description>No description</description>
- <final>true</final>
- </property>
- </configuration>
<span style="font-family:Arial;font-size:12px;"><!--  mapred-site.xml的配置 -->
<configuration>
<property>  
    <name>mapreduce.cluster.temp.dir</name>  
    <value></value>  
    <description>No description</description>  
    <final>true</final>  
  </property>  
  
  <property>  
    <name>mapreduce.cluster.local.dir</name>  
    <value></value>  
    <description>No description</description>  
    <final>true</final>  
  </property>  
</configuration>
</span>
mapred-env.sh里面的配置
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- # Licensed to the Apache Software Foundation (ASF) under one or more  
- # contributor license agreements.  See the NOTICE file distributed with  
- # this work for additional information regarding copyright ownership.  
- # The ASF licenses this file to You under the Apache License, Version 2.0
- # (the "License"); you may not use this file except in compliance with  
- # the License.  You may obtain a copy of the License at  
- #  
- #     http://www.apache.org/licenses/LICENSE-2.0
- #  
- # Unless required by applicable law or agreed to in writing, software  
- # distributed under the License is distributed on an "AS IS" BASIS,  
- # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
- # See the License for the specific language governing permissions and  
- # limitations under the License.  
- 
- export JAVA_HOME=/usr/local/jdk  
- 
- export HADOOP_JOB_HISTORYSERVER_HEAPSIZE=1000
- 
- export HADOOP_MAPRED_ROOT_LOGGER=INFO,RFA  
- 
- #export HADOOP_JOB_HISTORYSERVER_OPTS=  
- #export HADOOP_MAPRED_LOG_DIR="" # Where log files are stored.  $HADOOP_MAPRED_HOME/logs by default.  
- #export HADOOP_JHS_LOGGER=INFO,RFA # Hadoop JobSummary logger.  
- #export HADOOP_MAPRED_PID_DIR= # The pid files are stored. /tmp by default.  
- #export HADOOP_MAPRED_IDENT_STRING= #A string representing this instance of hadoop. $USER by default
- #export HADOOP_MAPRED_NICENESS= #The scheduling priority for daemons. Defaults to 0.  
```java
<span style="font-family:Arial;font-size:12px;"># Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
export JAVA_HOME=/usr/local/jdk
export HADOOP_JOB_HISTORYSERVER_HEAPSIZE=1000
export HADOOP_MAPRED_ROOT_LOGGER=INFO,RFA
#export HADOOP_JOB_HISTORYSERVER_OPTS=
#export HADOOP_MAPRED_LOG_DIR="" # Where log files are stored.  $HADOOP_MAPRED_HOME/logs by default.
#export HADOOP_JHS_LOGGER=INFO,RFA # Hadoop JobSummary logger.
#export HADOOP_MAPRED_PID_DIR= # The pid files are stored. /tmp by default.
#export HADOOP_MAPRED_IDENT_STRING= #A string representing this instance of hadoop. $USER by default
#export HADOOP_MAPRED_NICENESS= #The scheduling priority for daemons. Defaults to 0.
</span>
```
然后，建对应的几个文件夹即可，路径都在hadoop2.2的根目录下即可,
然后执行格式化命令bin/hadoop namenode -format即可； 
最后使用jps命令，查看进程，如果有以下几个进程名称，就代表部署伪分布式成功 
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- 4887 NodeManager  
- 4796 ResourceManager  
- 4661 SecondaryNameNode  
- 4524 DataNode  
- 4418 NameNode  
- 6122 Jps  
```java
<span style="font-family:Arial;font-size:12px;">4887 NodeManager
4796 ResourceManager
4661 SecondaryNameNode
4524 DataNode
4418 NameNode
6122 Jps</span>
```
然后，访问界面端口，注意与之前的配置文件里一致，namenode的端口号仍为50070,原来1.x的50030端口，已经没有了，可以访问8088的yarn的端口，来查看，具体的截图如下：
![](http://dl2.iteye.com/upload/attachment/0097/9850/acc16e13-2b11-3312-9bb3-6bf9a2eb3083.jpg)
![](http://dl2.iteye.com/upload/attachment/0097/9852/fd8da272-1c90-38a2-9088-16abeca7af53.jpg)
OK，此时，我们已经成功的完成伪分布式的部署，下一步我们要做的，就是跑一个经典的MR程序Hellow World，来测试我们的集群了。
