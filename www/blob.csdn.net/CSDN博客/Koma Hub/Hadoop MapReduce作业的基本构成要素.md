# Hadoop MapReduce作业的基本构成要素 - Koma Hub - CSDN博客
2019年03月02日 14:10:08[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：7
**Table of Contents**
[MapReduce模型](#MapReduce%E6%A8%A1%E5%9E%8B)
[我的第一个MapReduce应用程序的架构](#%E6%88%91%E7%9A%84%E7%AC%AC%E4%B8%80%E4%B8%AAMapReduce%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E7%9A%84%E6%9E%B6%E6%9E%84)
[Hadoop的核心MapReduce](#Hadoop%E7%9A%84%E6%A0%B8%E5%BF%83MapReduce)
[MapReduce任务的构成要素](#MapReduce%E4%BB%BB%E5%8A%A1%E7%9A%84%E6%9E%84%E6%88%90%E8%A6%81%E7%B4%A0)
[MapReduceIntro.java](#MapReduceIntro.java)
[输入分割块](#%E8%BE%93%E5%85%A5%E5%88%86%E5%89%B2%E5%9D%97)
[一个简单的Map任务：IdentityMapper](#%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84Map%E4%BB%BB%E5%8A%A1%EF%BC%9AIdentityMapper)
[IdentityMapper.java](#IdentityMapper.java)
[一个简单的Reduce任务：IdentityReducer](#%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84Reduce%E4%BB%BB%E5%8A%A1%EF%BC%9AIdentityReducer)
[IdentityReducer.java](#IdentityReducer.java)
## MapReduce模型
![](https://img-blog.csdnimg.cn/20190302140552332.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
一个Map任务可以执行在集群中的任何一个计算机节点上。多个Map任务可以并行的执行在集群中的多个节点上。Map任务负责转换输入记录成为名值对。所有Map任务的输出会被重新组合成多个排序的集合，这里面的每一个排序的集合会被派发给一个单独的Reduce任务。Reduce任务会对集合中排序的关键字和关联在关键字的多个数据值进行处理。Reduce任务也是并行的运行在集群中的不同节点上的。
应用程序开发人员仅仅需要提供4项输入信息给Hadoop框架:读取和转换输入记录到键值对的作业类，一个Map方法，一个Reduce方法和一个转换键值对到输出记录的Reduce任务类。
我的第一个MapReduce应用程序是一个专业的网络爬虫。这个爬虫接受大量的网页地址，然后读取和处理网页地址的内容。因为这个应用要处理大量的网页地址，所以获取他们的内容是极其浪费时间和资源的。整个处理流程包含一下几个步骤，
1.输入网页地址和获得网页地址关联的元数据。
2.规格化网页地址。
3.排除重复的网页地址。
4.通过预定义的排除和包含过滤器过滤网页地址。
5.通过预定义的非取内容列表过滤网页地址。
6.通过预定义最近已看列表过滤网页地址。
7.获取网页地址内容。
8.标志网页地址内容。
9.更新最近已看列表。
10.为下一个应用程序准备工作列表。
在这个项目中，我有20个机器可以使用。这个应用程序原来的实现是非常复杂的，它使用了一个开源分布式队列框架，它的效率非常低。因为，我花费了大量的时间在开发应用程序和对应用程序进行调优。因此，这个项目濒临失败。随后，其他团队的一个成员建议我使用Hadoop。
我花费了一整天的时间建立了一个具有20台机器的Hadoop集群，接下来，试验性的执行了它的样例程序以后，我的团队花了几个小时的时间想出了一个解决方案，在这个解决方案中包含了九个Map方法和三个Reduce方法。目标是每一个Map和Reduce方法不能超过100行的代码量。那一周结束后，我们实现的基于Hadoop的应用程序就已经比原来的实现更快和更稳定。图表1-2阐述了它的架构。标志网页内容的步骤使用了第三方类库，不幸的是，它偶尔的会出现问题并且引起整个集群瘫痪。
### 我的第一个MapReduce应用程序的架构
![](https://img-blog.csdnimg.cn/20190302140710123.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
使用Hadoop能够很容易的把分布式应用程序并行的运行在集群上，集群上的一个节点的失败不会影响其他节点的操作，一个作业在一个节点上的失败，Hadoop会分配其他的节点进行重试，因为这些优点，Hadoop已经成为我最喜欢的开发工具之一。
谷歌和雅虎都是用MapReduce集群来处理以P数量级计算的大数据集。在2008年初，谷歌宣布它每天通过MapReduce处理20P的数据，请参考http://googleblog.blogspot.com/2008/11/sorting-1pb-with-mapreduce。
## Hadoop的核心MapReduce
Hadoop MapReduce环境为用户提供了一个在集群上管理和执行Map和Reduce任务的复杂的框架。用户需要向框架输入如下信息：
- 分布式文件系统中作业输入的位置
- 分布式文件系统中作业输出的位置
- 输入格式
- 输出格式
- 包含Map方法的类
- 包含Reduce方法的类，这是可选的。
- 包含Map和Reduce方法和其他支持类的Jar文件的位置
如果一个作业不需要一个Reduce方法，那么，用户不需指定一个Reducer类，它也不需要执行作业的Reduce阶段。框架会分割输入数据，在集群里调度和执行Map任务。如果需要，它将Map任务的输出进行排序，然后，将排序的结果输出给Reduce任务。然后，将Reduce任务输出数据输出到输出目录，最后，将工作状况反馈给用户。
MapReduce任务是用来对键值对进行处理的系统实现。该框架转换每个输入的记录成一个键值对，每对数据会被输入给Map任务。Map任务的输出是一套键值对，原则上，输入是一个键值对，但是，输出可以是多个键值对。然后，它对Map输出键值对分组和排序。然后，对排序的每个键值对调用一次Reduce方法，它的输出是一个关键字和一套和此关键字关联的数据值。Reduce方法可以输出任意数量的键值对，这将被写入作业输出目录下的输出文件。如果Reduce输出关键字保持和Reduce输入关键字一致，最终的输出仍然保持排序。
## MapReduce任务的构成要素
|要素|由谁处理|
|----|----|
|配置作业|用户|
|输入分割和派遣|Hadoop框架|
|接受分割的输入后，每个Map任务的启动|Hadoop框架|
|Map函数，对于每个键值对被调用一次|用户|
|混淆，分割和排序Map的的输出并得到快|Hadoop框架|
|排序，将混淆的块进行组合和排序||
|接受排序快后，每个Reduce任务的启动|Hadoop框架|
|Reduce函数，对于每一个关键字和对象的所有数据值被调用一次|用户|
|收集输出结果，在输出目录存储输出结果，输出结果分为N个部分，N是Reduce任务的号码|Hadoop框架|

![](https://img-blog.csdnimg.cn/20190302135711353.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### MapReduceIntro.java
```java
package com.apress.hadoopbook.examples.ch2;
import java.io.IOException;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.FileOutputFormat;
import org.apache.hadoop.mapred.JobClient;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.KeyValueTextInputFormat;
import org.apache.hadoop.mapred.RunningJob;
import org.apache.hadoop.mapred.lib.IdentityMapper;
import org.apache.hadoop.mapred.lib.IdentityReducer;
import org.apache.log4j.Logger;
/**
* A very simple MapReduce example that reads textual input where each record is
* a single line, and sorts all of the input lines into a single output file.
*
* The records are parsed into Key and Value using the first TAB character as a
* separator. If there is no TAB character the entire line is the Key. *
*
* @author Jason Venner
*
*/
public class MapReduceIntro {
    protected static Logger logger = Logger.getLogger(MapReduceIntro.class);
    /**
    * Configure and run the MapReduceIntro job.
    *
    * @param args
    * Not used.
    */
    public static void main(final String[] args) {
        try {
        /**
        * Construct the job conf object that will be used to submit this
        * job to the Hadoop framework. ensure that the jar or directory
        * that contains MapReduceIntroConfig.class is made available to all
        * of the Tasktracker nodes that will run maps or reduces for this
        * job.
        */
        final JobConf conf = new JobConf(MapReduceIntro.class);
        /**
        * Take care of some housekeeping to ensure that this simple example
        * job will run
        */
        MapReduceIntroConfig.exampleHouseKeeping(conf, MapReduceIntroConfig.getInputDirectory(),
        MapReduceIntroConfig.getOutputDirectory());
        /**
        * This section is the actual job configuration portion /**
        * Configure the inputDirectory and the type of input. In this case
        * we are stating that the input is text, and each record is a
        * single line, and the first TAB is the separator between the key
        * and the value of the record.
        */
        conf.setInputFormat(KeyValueTextInputFormat.class);
        FileInputFormat.setInputPaths(conf, MapReduceIntroConfig.getInputDirectory());
        /**
        * Inform the framework that the mapper class will be the
        * {@link IdentityMapper}. This class simply passes the input Key
        * Value pairs directly to its output, which in our case will be the
        * shuffle.
        */
        conf.setMapperClass(IdentityMapper.class);
        /**
        * Configure the output of the job to go to the output directory.
        * Inform the framework that the Output Key and Value classes will
        * be {@link Text} and the output file format will
        * {@link TextOutputFormat}. The TextOutput format class joins
        * produces a record of output for each Key,Value pair, with the
        * following format. Formatter.format( "%s\t%s%n", key.toString(),
        * value.toString() );.
        *
        * In addition indicate to the framework that there will be 1
        * reduce. This results in all input keys being placed into the
        * same, single, partition, and the final output being a single
        * sorted file.
        */
        FileOutputFormat.setOutputPath(conf, MapReduceIntroConfig.getOutputDirectory());
        conf.setOutputKeyClass(Text.class);
        conf.setOutputValueClass(Text.class);
        conf.setNumReduceTasks(1);
        /**
        * Inform the framework that the reducer class will be the
        * {@link IdentityReducer}. This class simply writes an output
        * record key, value record for each value in the key, valueset it
        * receives as input. The value ordering is arbitrary.
        */
        conf.setReducerClass(IdentityReducer.class);
        logger.info("Launching the job.");
        /**
        * Send the job configuration to the framework and request that the
        * job be run.
        */
        final RunningJob job = JobClient.runJob(conf);
        logger.info("The job has completed.");
        if (!job.isSuccessful()) {
        logger.error("The job failed.");
        System.exit(1);
        }
        logger.info("The job completed successfully.");
        System.exit(0);
        } catch (final IOException e) {
        logger.error("The job has failed due to an IO Exception", e);
        e.printStackTrace();
        }
    }
}
```
## 输入分割块
框架能够在多台机器上派遣作业的不同部分，所以，它需要把输入分割成不同的部分，然后它把输入的每一个部分传递给一个独立的分布式作业。输入的每一部分称为输入分割块。一套配置参数的组合和事实上读取输入记录的类的功能决定了框架是如何根据事实的输入文件构造输入分割块的。我们将在第六章介绍这些参数。
一个输入分割块通常来自一个输入文件的连续的记录组，在这种情况下，至少会存在N个输入分割，N是输入文件的个数。如果设置的Map任务数多于输入文件数，或者某个文件大小超过了输入分割块的最大尺寸，那么，框架就会从一个输入文件中构造多个输入分割块。输入分割块的数量和大小会极大的影响整体作业效率。
### 一个简单的Map任务：IdentityMapper
Hadoop框架提供了一个简单的map功能，称为IdentityMapper。它可以被应用在仅仅需要Reduce任务的作业，它不需要转换原始输入的数据。在这一小节，我们研究IdentityMapper类的代码，如列表2-2所示。如果你下载了Hadoop核心安装程序，然后，根据第一章的指令，你能够在安装目录下找到这份代码，它位于${HADOOP_HOME}/src/mapred/org/apache/hadoop/mapred/lib/IdentityMapper.java。
### IdentityMapper.java
```java
/**
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements. See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership. The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License. You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
package org.apache.hadoop.mapred.lib;
import java.io.IOException;
import org.apache.hadoop.mapred.Mapper;
import org.apache.hadoop.mapred.OutputCollector;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.mapred.MapReduceBase;
/** Implements the identity function, mapping inputs directly to outputs. */
public class IdentityMapper<K, V> extends MapReduceBase implements Mapper<K, V, K, V> {
    /**
    * The identify function. Input key/value pair is written directly to
    * output.
    */
    public void map(K key, V val, OutputCollector<K, V> output, Reporter reporter) throws IOException {
        output.collect(key, val);
    }
}
```
这份代码的关键在于这一行，output.collect(key,val)，这一行的作用是传递键值对给框架，框架将做进一步的处理。
所有的map功能必须实现Mapper接口，这确保map功能被调用时总是传递进入一个关键字（关键字是一个WritableComparable类的实例），数据值（数据值是一个Writable类的一个实例），一个输出对象和一个报表对象。到现在为止，只需要记得报表对象是有用的，我们会在本章中后面“创建一个客户化的Mapper和Reducer”小节中详细讨论报表对象。
请注意，你可以在Apress网站(http://www.apress.com)中这本书的下载页面中找到Mapper.java和Reducer.java接口的代码，这些代码和其他的样例代码是在一起的。
框架针对输入中的每一个记录调用一次你的map任务。你的map任务可能在多个Java虚拟机下有多个实例在运行，这些Java虚拟机也可能运行在多台分布式机器上。框架会为你调度这些操作。
## 一个简单的Reduce任务：IdentityReducer
Hadoop框架对于每一个关键字调用一次Reduce功能。在每次调用时，框架都会提供关关键字和关键字所对应的所有数据值。
框架提供的类IdentityReducer为每个输入数据值产生一个输出记录的样例实现。
### IdentityReducer.java
```java
/**
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements. See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership. The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License. You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
package org.apache.hadoop.mapred.lib;
import java.io.IOException;
import java.util.Iterator;
import org.apache.hadoop.mapred.Reducer;
import org.apache.hadoop.mapred.OutputCollector;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.mapred.MapReduceBase;
/** Performs no reduction, writing all input values directly to the output. */
public class IdentityReducer<K, V> extends MapReduceBase implements Reducer<K, V, K, V> {
    /** Writes all keys and values directly to output. */
    public void reduce(K key, Iterator<V> values, OutputCollector<K, V> output, Reporter reporter) throws IOException {
        while (values.hasNext()) {
            output.collect(key, values.next());
        }
    }
}
```
如果你需要排序你的作业的输入，reducer功能必须按照顺序传递数据值对象给output.collect()。然而，reduce阶段可以输出具有同一个关键字和不同数据值的任意数量的记录。这就是为什么Map任务是多线程的，而Reduce任务是单线程的。
> 
精通HADOOP（在云中构建可扩展的分布式应用程序）
翻译：罗伯特.李
邮件：robertleepeak@gmail.com
博客：http://blog.csdn.net/robertleepeak
