# Hadoop Mapper 示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月20日 14:32:31[boonya](https://me.csdn.net/boonya)阅读数：1304








This  Article Is From：[https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/hadoop-mapper-example/](https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/hadoop-mapper-example/)





### About Raman Jhajj


![Raman Jhajj](https://examples.javacodegeeks.com/wp-content/uploads/2015/11/Raman-Jhajj-60x60.jpg)Ramaninder毕业于德国Georg-August大学计算机科学与数学系，目前与奥地利的大数据研究中心合作。他拥有应用计算机科学硕士学位，专业应用系统工程和商业信息学。他也是一名微软认证过程，在Java，C＃，Web开发和相关技术方面有超过5年的经验。目前，他的主要兴趣是大数据生态系统，包括批处理和流处理系统，机器学习和Web应用程序。





建议先看英文再看翻译：翻译使用的是Google翻译。


在这个例子中，我们将讨论和理解Hadoop Mappers，这是Hadoop MapReduce框架的前半部分。映射器是任何MapReduce应用程序的最明显的部分，并且需要对Mappers的良好理解才能充分利用MapReduce功能。

# 1.介绍（Introduction）

Mapper是用于在Hadoop MapReduce中实现Map任务的基类。映射是在reduce之前运行的各个任务，并将输入转换为一组输出值。这些输出值是作为Reduce任务输入的中间值。

我们将在示例中详细了解映射器（Mapers）。


# 2.背景（Background）

在了解Mappers的细节之前，我们需要做一些背景阅读，并理解一些术语，我们将在后面的文章中使用。以下是我们需要了解的术语：


- 输入格式（Input Format）：输入格式是定义Hadoop
 MapReduce作业的输入规范的类。它对输入数据执行分割，以便可以将数据逻辑分配给多个映射器。例如，在文件是MapReduce任务的输入的情况下。输入格式通常根据文件的总大小和根据使用正在被视为单个输入拆分的最大限制的文件的块大小运行的可用映射任务的总数来确定的拆分数来拆分输入。
- 输入拆分(Input
 Split)：输入拆分包含要由单个映射器处理的数据。如上所述的输入格式负责产生输入分割并将每个输入分割分配给映射器以用于进一步处理。


现在有了对Mappers使用的术语和类的背景理解，我们将在下一节中继续研究Mapper的工作流程。

# 3.工作流（Workflow）

任何单独的映射器的工作流程不是那么复杂，并且很容易理解。
给定的输入对可以生成零到多个输出对。**mapper的工作流如下：**


1.Hadoop MaReduce框架首先调用setup方法来执行所有必需的活动来启动map任务。

2.设置(setup)后，调用map以执行实际任务。

3.最后，需要执行清理(cleanup)来执行关闭功能。

4.映射器的输出值被组合在一起用于特定键，然后被传递到reducer以进一步处理，其提供MapReduce任务的最终输出


![](https://img-blog.csdn.net/20170120140229543)


注意：如果作业具有零缩减器并且只有映射器，那么输出将直接写入OutputFormat。


# 4.映射内部（Mapper Internals)

在本节中，我们将介绍Mapper的内部结构，什么是可用的方法，如何使用它们以及流如何工作。

以下是Mapper类中提供的方法：


- setup
- map
- run
- cleanup



让我们看看每一个细节：


## 4.1setup

设置方法作为名称指示用于设置地图任务，并且在任务开始时只使用一次。在此方法中初始化运行任务所需的所有逻辑。大多数时候，不需要修改或覆盖此方法。

它使用Mapper.Context对象作为参数，并抛出IOException和InteruptedException。

**mapper.java**



```java
protected void setup(Mapper.Context context)
       throws IOException, 
              InterruptedException
```





## 4.2map

这是最重要的方法，是一个开发人员应该最了解的。
对于映射器任务的输入中的每个键/值对，调用一次映射方法。这是我们需要在任何我们开发的应用程序中覆盖的方法，它应该包含需要在map任务中执行的所有逻辑。

**mapper.java**



```java
protected void map(KEYIN key, VALUIN value, Mapper.Context context)
       throws IOException, 
              InterruptedException
```

此方法从输入拆分和上下文中的键值对获取键和值。map方法可以抛出两种异常IOException和InterruptedException



## 4.3run

如果我们想要更多地控制map任务的运行/执行，这是我们需要覆盖的方法。

Run是在没有适当知识的情况下不应修改的方法，它是负责运行/执行映射任务的方法。这就是为什么开发人员想要覆盖这个方法时需要非常小心的原因。

**mapper.java**



```java
public void run(Mapper.Context context)
       throws IOException, 
              InterruptedException
```
run方法只需要一个参数，即Mapper.Context，并且可以抛出与其他方法类似的两个excetions，即IOException或InterruptedException。



## 4.4cleanup

清理是在任务结束时只调用一次的方法。此方法作为名称表示负责清除任务残留（如果有）。

**mapper.java**



```java
public void cleanup(Mapper.Context context)
       throws IOException, 
              InterruptedException
```

与所有其他方法类似，此方法也使用Mapper.Context作为参数，并可能抛出IOException和InterruptedException


## 4.5其他常用方法

除了Mapper类的四个主要方法之外，还有一些从java.lang.Object类继承的常见方法，因为Mapper继承自Object类。这些方法如下：


- clone
- equals
- finalize
- getClass
- hashCode
- notify
- notifyAll
- toString
- wait



这些是在java中使用的相当常见的方法，如果你想更多地了解这些方法，java.lang.Object是一个很好的开始。


# 5.Hadoop Mapper 代码

在本节中，我们将编写和了解一个集中在Mapper上的MapReduce应用程序。我们不会为这个应用程序实现任何Reducer类。因此应用程序将是“Mapper Only Hadoop Application”。


## 5.1设置

我们将使用Maven来设置一个新项目。在Eclipse中设置一个maven项目，并向pom.xml添加以下Hadoop依赖项。这将确保我们具有对Hadoop核心库的所需访问权限。

**pom.xml**




```
<dependency>
    <groupId>org.apache.hadoop</groupId>
    <artifactId>hadoop-core</artifactId>
    <version>1.2.1</version>
</dependency>
```

添加依赖之后，我们准备好写我们的实际应用程序代码。


## 5.2Mapper 类

在这个仅映射器应用程序中，我们将创建一个从输入文件中读取行的映射器，每行包含在网站上访问的页面的ID，用空格分隔。因此，线被分割，并且该线中的总页数被计数。如果它们超过500，那些行被写入上下文。

这不是一个大的用例，但足以澄清映射器的工作原理。

**MapClass.java**



```java
package com.javacodegeeks.examples.hadoopMapper;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;


/**
 * Map Class which extends MaReduce.Mapper class
 * Map is passed a single line at a time, it splits the line based on space
 * and calculates the number of page visits(each line contains the number corresponding to page number)
 * So total length of the split array are the no. of pages visited in that session
 * If pages are more then 500 then write the line to the context.
 * 
 * @author Raman
 */
public class MapClass extends Mapper{
	 
    private Text selectedLine = new Text();
    private IntWritable noOfPageVisited = new IntWritable();
    
    /**
     * map function of Mapper parent class takes a line of text at a time
     * performs the operation and passes to the context as word along with value as one
     */
	@Override
	protected void map(LongWritable key, Text value,
			Context context)
			throws IOException, InterruptedException {
		
		String line = value.toString();
		String[] pagesVisited = line.split(" ");
		
		if(pagesVisited.length > 500) {
			selectedLine.set(line);
			noOfPageVisited.set(pagesVisited.length);
			context.write(selectedLine, noOfPageVisited);
		}
	}
}
```
行22-23，定义输出键值对的数据类型。
第29-42行，重写Mapper类的map方法，并包含主逻辑。
行35拆分输入，然后检查条件是否满足行40在上下文中写入输出


## 5.3Driver类

驱动程序类是每个MapReduce应用程序的入口点。这是设置MapReduce作业配置的类。


**Driver.java**




```java
package com.javacodegeeks.examples.hadoopMapper;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

/**
 * The entry point for the WordCount example,
 * which setup the Hadoop job with Map and Reduce Class
 * 
 * @author Raman
 */
public class Driver extends Configured implements Tool{
	
	/**
	 * Main function which calls the run method and passes the args using ToolRunner
	 * @param args Two arguments input and output file paths
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception{
		int exitCode = ToolRunner.run(new Driver(), args);
		System.exit(exitCode);
	}
 
	/**
	 * Run method which schedules the Hadoop Job
	 * @param args Arguments passed in main function
	 */
	public int run(String[] args) throws Exception {
		if (args.length != 2) {
			System.err.printf("Usage: %s needs two arguments   files\n",
					getClass().getSimpleName());
			return -1;
		}
	
		//Initialize the Hadoop job and set the jar as well as the name of the Job
		Job job = new Job();
		job.setJarByClass(Driver.class);
		job.setJobName("LogProcessor");
		
		//Add input and output file paths to job based on the arguments passed
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
	
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		job.setOutputFormatClass(TextOutputFormat.class);
		
		//Set the MapClass and ReduceClass in the job
		job.setMapperClass(MapClass.class);
		
		//Setting the number of reducer tasks to 0 as we do not 
		//have any reduce tasks in this example. We are only concentrating on the Mapper
		job.setNumReduceTasks(0);
		
		
		//Wait for the job to complete and print if the job was successful or not
		int returnValue = job.waitForCompletion(true) ? 0:1;
		
		if(job.isSuccessful()) {
			System.out.println("Job was successful");
		} else if(!job.isSuccessful()) {
			System.out.println("Job was not successful");			
		}
		
		return returnValue;
	}
}
```

Driver类的所有设置和配置都使用注释进行了正确解释，但我们应该仔细查看第no行。57，它将MapClass设置为Mapper和行号。61将reducers的数量设置为零，这使得它成为仅有映射器的应用程序而没有任何reducer。



## 5.4数据集(Dataset)

用于此示例的数据集在[UCI机器学习库](http://archive.ics.uci.edu/ml/datasets/MSNBC.com+Anonymous+Web+Data)中可用

该数据集描述了1999年9月28日访问过msnbc.com的用户的网页访问次数。访问次数记录在URL类别级别，并按时间顺序记录。
每一行包含用户在一个会话中访问的页面/网址。数据是匿名的，只包含整数，一个整数表示一页。


## 5.5运行应用程序

为了测试的目的，可以在Eclipse本地运行应用程序，我们也将这样做。在Eclipse中，传递项目参数中的输入文件和输出文件名。以下是参数的样子。在这种情况下，输入文件位于项目的根目录中，这就是为什么只需要filename，但如果输入文件位于其他位置，则应提供完整的路径。

![](https://img-blog.csdn.net/20170120142515412)


注意：确保输出文件不存在。如果存在，程序将抛出一个错误。

设置参数后，只需运行应用程序。成功完成应用程序后，控制台将显示输出为“Job Successful”。
然后我们可以检查输出目录的输出结果。

Hadoop应用程序被设计为在集群上实际运行。如果你想知道如何在Hadoop集群上运行应用程序，无论是单节点集群还是多节点，请参考文章[Apache Hadoop Wordcount示例](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-wordcount-example/)


# 6.总结（Conclusion）

本示例解释了Mapper类和关于Apache Hadoop MapReduce框架的概念。我们研究了Mapper类及其工作流程的细节。然后了解Mapper类中可用的方法，以便在应用程序中实现。然后我们写了一个mapper only应用程序的实践经验，然后学习如何执行的应用程序在Eclipse本身为测试目的。我希望，本文的目的是解释Mapper类的Hadoop MapReduce框架，并为您提供了解Apache Hadoop和MapReduce的坚实基础。

# 7.下载代码（Download the code）

下载文章中讨论的代码和数据集文件。



**Download**您可以在这里下载此示例的完整源代码：**[HadoopMapper](http://examples.javacodegeeks.com/wp-content/uploads/2016/05/hadoopMapper.zip)**



注：为了代码持续可下载，已备份至百度网盘：[http://pan.baidu.com/s/1nvfQk21](http://pan.baidu.com/s/1nvfQk21)




