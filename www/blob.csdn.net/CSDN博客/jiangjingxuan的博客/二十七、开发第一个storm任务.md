# 二十七、开发第一个storm任务 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:51:01[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1367












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_117_default_big.jpeg)



每门语言的学习都要经历最经典的helloworld过程，学习storm框架也一样，本节我们先来运行storm为我们做好的storm-starter例子，来确定storm服务搭建是正常的，然后我们自己来从零开始写一个简单的storm任务

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 运行storm-starter



首先要下载storm源代码，我们服务部署的是1.0.1版本，那么我就下载同样的版本，在github.com上找到[https://github.com/apache/storm/releases/tag/v1.0.1](https://github.com/apache/storm/releases/tag/v1.0.1)的源代码（注意，这里的源代码和上一节中降到的部署用的包是不同的，这个是未编译的原始代码，上节中的是编译好的直接可运行的包，当然你也可以用本节下载的源代码重新编译来部署），下载：
`wget https://github.com/apache/storm/archive/v1.0.1.tar.gz`
解压后编译代码中的样例start-storm（位于源代码的examples/storm-starter），编译方法如下（也可以加载到eclipse，用maven编译）：

```java
mvn -D maven.test.skip=true clean package
```

注：如果没有安装maven则先安装，具体方法百度一下



编译好了之后会生成target/storm-starter-1.0.1.jar文件，下面我们来在我们部署好的storm上运行这个jar包里的storm.starter.StatefulTopology类



执行：
`storm jar examples/storm-starter/storm-starter-topologies-*.jar storm.starter.StatefulTopology statetopology`
这里的storm命令是在storm部署目录的bin目录下的脚本，如果没有配置PATH环境变量，可以用绝对路径执行，如果我的storm部署在/data/apache-storm-1.0.1/，那么就执行：
`/data/apache-storm-1.0.1/bin/storm jar storm-starter-1.0.1.jar storm.starter.StatefulTopology statetopology`


如果运行成功说明你的storm是正常的，输出如下：

```java
[root@centos7vm storm-1.0.1]# /data/apache-storm-1.0.1/bin/storm jar storm-starter-1.0.1.jar storm.starter.StatefulTopology statetopology
Running: java -client -Ddaemon.name= -Dstorm.options= -Dstorm.home=/data/apache-storm-1.0.1 -Dstorm.log.dir=/data/apache-storm-1.0.1/logs -Djava.library.path=/usr/local/lib:/opt/local/lib:/usr/lib -Dstorm.conf.file= -cp /data/apache-storm-1.0.1/lib/storm-core-1.0.1.jar:/data/apache-storm-1.0.1/lib/kryo-3.0.3.jar:/data/apache-storm-1.0.1/lib/reflectasm-1.10.1.jar:/data/apache-storm-1.0.1/lib/asm-5.0.3.jar:/data/apache-storm-1.0.1/lib/minlog-1.3.0.jar:/data/apache-storm-1.0.1/lib/objenesis-2.1.jar:/data/apache-storm-1.0.1/lib/clojure-1.7.0.jar:/data/apache-storm-1.0.1/lib/disruptor-3.3.2.jar:/data/apache-storm-1.0.1/lib/log4j-api-2.1.jar:/data/apache-storm-1.0.1/lib/log4j-core-2.1.jar:/data/apache-storm-1.0.1/lib/log4j-slf4j-impl-2.1.jar:/data/apache-storm-1.0.1/lib/slf4j-api-1.7.7.jar:/data/apache-storm-1.0.1/lib/log4j-over-slf4j-1.6.6.jar:/data/apache-storm-1.0.1/lib/servlet-api-2.5.jar:/data/apache-storm-1.0.1/lib/storm-rename-hack-1.0.1.jar:storm-starter-1.0.1.jar:/data/apache-storm-1.0.1/conf:/data/apache-storm-1.0.1/bin -Dstorm.jar=storm-starter-1.0.1.jar storm.starter.StatefulTopology statetopology
1329 [main] INFO  o.a.s.StormSubmitter - Generated ZooKeeper secret payload for MD5-digest: -6806752055047040892:-6736308748043757911
1492 [main] INFO  o.a.s.s.a.AuthUtils - Got AutoCreds []
1687 [main] INFO  o.a.s.StormSubmitter - Uploading topology jar storm-starter-1.0.1.jar to assigned location: /data/apache-storm-1.0.1/storm-local/nimbus/inbox/stormjar-89ac13d0-dc05-4aec-ab4c-0c6fc6b7f5e0.jar
Start uploading file 'storm-starter-1.0.1.jar' to '/data/apache-storm-1.0.1/storm-local/nimbus/inbox/stormjar-89ac13d0-dc05-4aec-ab4c-0c6fc6b7f5e0.jar' (62385346 bytes)
[==================================================] 62385346 / 62385346
File 'storm-starter-1.0.1.jar' uploaded to '/data/apache-storm-1.0.1/storm-local/nimbus/inbox/stormjar-89ac13d0-dc05-4aec-ab4c-0c6fc6b7f5e0.jar' (62385346 bytes)
2992 [main] INFO  o.a.s.StormSubmitter - Successfully uploaded topology jar to assigned location: /data/apache-storm-1.0.1/storm-local/nimbus/inbox/stormjar-89ac13d0-dc05-4aec-ab4c-0c6fc6b7f5e0.jar
2992 [main] INFO  o.a.s.StormSubmitter - Submitting topology statetopology in distributed mode with conf {"storm.zookeeper.topology.auth.scheme":"digest","storm.zookeeper.topology.auth.payload":"-6806752055047040892:-6736308748043757911","topology.workers":1,"topology.debug":false}
3682 [main] INFO  o.a.s.StormSubmitter - Finished submitting topology: statetopology
```

web界面如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/e9cf8e1d5685e3d14536260eda40b0ec9e6912d8.png)



stateopology详细运行情况可以点击进去



我们用eclipse（你可以用其他IDE）导入源代码，找到main函数如下：

```java
public static void main(String[] args) throws Exception {
        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout("spout", new RandomIntegerSpout());
        builder.setBolt("partialsum", new StatefulSumBolt("partial"), 1).shuffleGrouping("spout");
        builder.setBolt("printer", new PrinterBolt(), 2).shuffleGrouping("partialsum");
        builder.setBolt("total", new StatefulSumBolt("total"), 1).shuffleGrouping("printer");
        Config conf = new Config();
        conf.setDebug(false);

        if (args != null && args.length > 0) {
            conf.setNumWorkers(1);
            StormSubmitter.submitTopologyWithProgressBar(args[0], conf, builder.createTopology());
        } else {
            LocalCluster cluster = new LocalCluster();
            StormTopology topology = builder.createTopology();
            cluster.submitTopology("test", conf, topology);
            Utils.sleep(40000);
            cluster.killTopology("test");
            cluster.shutdown();
        }
    }
```

这个storm任务是由一个Spout（随机生成整数值）和三个Bolt（主要对数字做汇总并打印一些信息）组成，为了能看到运行的效果，我们找到部署目录里的log4j2/worker.xml配置文件，找到appenders配置，如果没有自己指定log目录，那么这里面默认应该是部署目录里的logs/workers-artifacts下

我们进入logs/workers-artifacts目录，这里面就是每个storm任务的日志目录

点网页里的stateopology

![](http://www.shareditor.com/uploads/media/my-context/0001/01/cad3b0a1dfea0081fc2d02a378f252563e3d7487.png)



这里的statetopology-2-1463051043就是我们的任务id，那么在logs/workers-artifacts目录中就会有statetopology-2-1463051043目录

```bash
cd statetopology-2-1463051043
```

会看到又有多个目录，继续点web里的spout进入到spout的状态页面，看到如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/eb489a9b262e7381ff2bd480e8e33a50b9b7e6a8.png)



这是由supervisor启动的executor的主机地址和port，那么我们进入到刚才的statetopology-2-1463051043目录的6700目录就是这个spout的日志啦，执行
`[root@centos7vm 6700]# tail -f worker.log`
日志在定期输出随机生成的数字



以上就是storm-starter的运行，在storm-starter项目里还有很多功能的演示，在真正使用时会经常参考







## 从零开始开发storm任务



下面我们准备不依赖storm的源代码，而是自己从零开始写一个storm任务，来理解一下storm的开发要点



打开eclipse（需要预安装mvn插件，没装的请百度一下），我们新建一个maven工程：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/a88e84c431b04969a2976c325d40d2f673f752ab.png)



group id设置为：com.shareditor，artifact id设置为：myfirststormpro，其他都默认下一步，最终我们的工程如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/4dfbefa1485e0a2a93b83ac49b09fdbea4902147.png)



这是maven为我们自动生成的helloworld，我们用maven编译一下，项目上点右键，选择Run as -> Maven build，在Goals中填clean package，点Run编译，如果编译成功会看到：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/7be7fca8be5c6825f28465be8946844f633ff6ab.png)



以上如果成功，说明maven编译没有问题

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

下一步我们来编辑storm项目依赖，打开pom.xml源文件如下：

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.shareditor</groupId>
  <artifactId>myfirststormpro</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>myfirststormpro</name>
  <url>http://maven.apache.org</url>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>

  <dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>3.8.1</version>
      <scope>test</scope>
    </dependency>
  </dependencies>
</project>
```



在dependencies标签中添加如下依赖：

```xml
<dependency>
            <groupId>org.apache.storm</groupId>
            <artifactId>storm-core</artifactId>
            <version>1.0.1</version>
        </dependency>
```

然后在com.shareditor.myfirststormpro包下创建MySpout.java，如下：

```java
package com.shareditor.myfirststormpro;

import java.util.Map;

import org.apache.storm.spout.SpoutOutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichSpout;
import org.apache.storm.tuple.Fields;
import org.apache.storm.utils.Utils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class MySpout extends BaseRichSpout{
    
    private static final Logger LOG = LoggerFactory.getLogger(MySpout.class);
    
    private SpoutOutputCollector collector;

    public void open(Map conf, TopologyContext context,
            SpoutOutputCollector collector) {
        this.collector = collector;
    }

    public void nextTuple() {
        Utils.sleep(1000);
        LOG.info("MySpout nextTuple");
    }


    public void declareOutputFields(OutputFieldsDeclarer declarer) {
        declarer.declare(new Fields("value"));
    }
}
```



创建MyBolt.java，如下：

```java
package com.shareditor.myfirststormpro;

import org.apache.storm.topology.BasicOutputCollector;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseBasicBolt;
import org.apache.storm.tuple.Tuple;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class MyBolt extends BaseBasicBolt {
    
    private static final Logger LOG = LoggerFactory.getLogger(MyBolt.class);

    public void execute(Tuple input, BasicOutputCollector collector) {
        LOG.info("MyBolt execute");
    }

    public void declareOutputFields(OutputFieldsDeclarer declarer) {
        // TODO Auto-generated method stub

    }
}
```



修改App.java，如下：

```java
package com.shareditor.myfirststormpro;

import org.apache.storm.Config;
import org.apache.storm.StormSubmitter;
import org.apache.storm.topology.TopologyBuilder;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main(String[] args) throws Exception {
        
        System.out.println("main");
        
        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout("myspout", new MySpout());
        builder.setBolt("mybolt", new MyBolt()).shuffleGrouping("myspout");
        
        Config conf = new Config();
        conf.setDebug(false);
        conf.setNumWorkers(1);
        StormSubmitter.submitTopologyWithProgressBar(args[0], conf, builder.createTopology());
    }
}
```

重新用maven编译项目，没问题会输出[INFO] BUILD SUCCESS，并且在myfirststormpro/target中会生成：myfirststormpro-0.0.1-SNAPSHOT.jar



用storm启动它，执行：
`/data/apache-storm-1.0.1/bin/storm jar myfirststormpro-0.0.1-SNAPSHOT.jar com.shareditor.myfirststormpro.App myfirststormproname`


像上面找storm-starter的方法一样找到日志，看到：

```
[root@centos7vm 6700]# tail -f worker.log
2016-05-13 08:48:59.100 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:49:00.112 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:49:02.308 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:49:03.310 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:49:04.312 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:49:05.314 c.s.m.MySpout [INFO] MySpout nextTuple
……
```

每隔一秒钟打印一行MySpout nextTuple



下面我们做一些修改，让他真正射点东西：



```java
public void nextTuple() {
        Utils.sleep(1000);
        LOG.info("MySpout nextTuple");
        
        collector.emit(new Values(10));
    }
```



重新编译并部署后查看日志如下：

```
[root@centos7vm 6700]# tail -f worker.log
2016-05-13 08:52:13.558 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:52:13.566 c.s.m.MyBolt [INFO] MyBolt execute
2016-05-13 08:52:14.563 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:52:14.565 c.s.m.MyBolt [INFO] MyBolt execute
2016-05-13 08:52:15.564 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:52:15.568 c.s.m.MyBolt [INFO] MyBolt execute
2016-05-13 08:52:16.565 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:52:16.567 c.s.m.MyBolt [INFO] MyBolt execute
2016-05-13 08:52:17.566 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:52:17.569 c.s.m.MyBolt [INFO] MyBolt execute
2016-05-13 08:52:18.567 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:52:18.569 c.s.m.MyBolt [INFO] MyBolt execute
```

我们看到Bolt开始工作了



那么我们怎么知道Bolt有没有收到数据呢，继续修改Bolt如下：

```java
public void execute(Tuple input, BasicOutputCollector collector) {
        
        int value = (Integer)input.getValueByField("value");
        LOG.info("MyBolt execute receive " + value);
    }
```



重新编译并部署后查看日志如下：

```
[root@centos7vm 6700]# tail -f worker.log
2016-05-13 08:58:02.565 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:58:02.567 c.s.m.MyBolt [INFO] MyBolt execute receive 10
2016-05-13 08:58:03.566 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:58:03.568 c.s.m.MyBolt [INFO] MyBolt execute receive 10
2016-05-13 08:58:04.567 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:58:04.570 c.s.m.MyBolt [INFO] MyBolt execute receive 10
2016-05-13 08:58:05.567 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:58:05.570 c.s.m.MyBolt [INFO] MyBolt execute receive 10
2016-05-13 08:58:06.569 c.s.m.MySpout [INFO] MySpout nextTuple
2016-05-13 08:58:06.573 c.s.m.MyBolt [INFO] MyBolt execute receive 10
```



## 试验一下storm的ack机制

在我们的Spout中添加：

```java
@Override
	public void ack(Object msgId) {
		LOG.info("MySpout arc");
		super.ack(msgId);
	}
```

重新编译部署后查看log发现没什么变化，这是为什么呢？

因为我们的bolt没有显式地调用ack函数，所以继续修改bolt，但是只有OutputCollector才能调用ack函数，因此我们再尝试一个bolt基类：BaseRichBolt，把整个代码修改成如下：

```java
package com.shareditor.myfirststormpro;

import java.util.Map;

import org.apache.storm.task.OutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichBolt;
import org.apache.storm.tuple.Tuple;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class MyBolt extends BaseRichBolt {
	
	private static final Logger LOG = LoggerFactory.getLogger(MyBolt.class);
	
	 private OutputCollector collector;

	public void declareOutputFields(OutputFieldsDeclarer declarer) {
		// TODO Auto-generated method stub
		
	}

	public void prepare(Map stormConf, TopologyContext context,
			OutputCollector collector) {
		this.collector = collector;
	}

	public void execute(Tuple input) {
		int value = (Integer)input.getValueByField("value");
		LOG.info("MyBolt execute receive " + value);
		collector.ack(input);
	}

}
```

再次编译部署查看log，看到了打印ack的日志啦

总结一下：storm就是帮我们管理了流式计算的服务部署和数据流传递，我们只需要实现具体的业务逻辑即可




