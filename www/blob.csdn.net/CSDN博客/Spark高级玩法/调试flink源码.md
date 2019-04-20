# 调试flink源码 - Spark高级玩法 - CSDN博客
2018年11月22日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：505
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vfcBibDicgGnXN7oZiapC8tF5HE2D0WfBFXrqTddZZhcpSWo1a1sRvrAoA/640)本文主要是讲讲flink的源码编译，案例运行，flink源码调试过程。调试flink的源码及案例，需要先clone工程，编一下源码，去掉规范检查，修改工程，最后才是调试运行。
**1. clone工程**
首先复制flink的github地址
`git@github.com:apache/flink.git`
接着在idea点击路径
`File--->New--->Project from Version Control--->git`
弹出窗口
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1v4Ec9cM06XXVQJ6bSicmMzQHTexLuic3JpzBgp7JxgrmIjTc0qOWBGibnA/640)
把刚刚复制的flink的github地址粘贴到url输入栏，点击clone按钮，然后等待构建完成，工程有点大需要点时间。
工程clone完成之后，可以在idea 的右下角切换到自己所用的分支，我的分支是1.6.
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1v3UUa9ApibG8hTDiaK3ibHBJWJiaibvicLpp9eSNMgUXVYhLKgCpDV6uM1qDw/640)
切换完成之后，分支显示为：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vqLhfcA1PARqEiakeXZCZreRkgDItGHuG9AwYiblA77CYUoxibjya2dv5w/640)
**2. 编译源码**
源码编译可以直接用idea的maven插件。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1v1JiawskGyibj9MRJGzRcqN5VaoqA6lvPiabXCMXrq0QLvrRiclCTewhcVA/640)
报错如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1v9IKU9JzexmeNIshosxkRmRFFOhRdQEsTFqcAmrRcDDK7HypOf7biaRg/640)
修改一下根目录下的pom.xml文件
去掉代码风格检查，注释掉这个的主要原因是我们要改源码，不注释掉无法编译通过。
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-checkstyle-plugin</artifactId>
<version>2.17</version>
<dependencies>
<dependency>
<groupId>com.puppycrawl.tools</groupId>
<artifactId>checkstyle</artifactId>
<!-- Note: match version with docs/internals/ide_setup.md -->
<version>8.4</version>
</dependency>
</dependencies>
<executions>
<execution>
<id>validate</id>
<phase>validate</phase>
<goals>
<goal>check</goal>
</goals>
</execution>
</executions>
<configuration>
<suppressionsLocation>/tools/maven/suppressions.xml</suppressionsLocation>
<includeTestSourceDirectory>true</includeTestSourceDirectory>
<configLocation>/tools/maven/checkstyle.xml</configLocation>
<logViolationsToConsole>true</logViolationsToConsole>
<failOnViolation>true</failOnViolation>
</configuration>
</plugin>
再次编译，即可。
**3. 运行kafka案例**
点开工程栏，找到flink-examples模块，然后找到kafka案例，如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vfAy1YJoKRbjUCPiczKC4h6BnKjLDmkhkVN3ta1TFyCtjdXTNroIXKGw/640)
将kafka的example修改为可运行的案例，官方demo是通过打包提交到集群的方式运行，需要传参的，而我们直接在idea中运行，不需要穿参数。代码修改如下：
Properties props = new Properties();
    props.put("bootstrap.servers", "mt-mdh.local:9093");
    props.put("zookeeper.connect","localhost:2181");
    props.put("group.id","test");
    props.put("metadata.fetch.timeout.ms","10000");
    props.put("metadata.max.age.ms","30000");
    StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();
    env.getConfig().disableSysoutLogging();
    env.getConfig().setRestartStrategy(RestartStrategies.fixedDelayRestart(4, 10000));
    env.enableCheckpointing(5000); // create a checkpoint every 5 seconds
    env.setStreamTimeCharacteristic(TimeCharacteristic.EventTime);
    DataStream<KafkaEvent> input = env
        .addSource(
new FlinkKafkaConsumer010<>(
"",
new KafkaEventSchema(),
            props)
          .assignTimestampsAndWatermarks(new CustomWatermarkExtractor()))
        .keyBy("word")
        .map(new RollingAdditionMapper());
    input.addSink(
new FlinkKafkaProducer010<>(
"bar",
new KafkaEventSchema(),
            props));
    env.execute("Kafka 0.10 Example");
然后，右键，run。发现，并不能顺心如意的运行，还是报了一堆错误。。。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1v3oicZYzoOXnfSqG8OQ5GJPSLVOKdLFeuicKwKt3HeDrWUVJF31dCiboicQ/640)
实际上，只需要改一些run的运行配置即可避免该错误。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1v6h1FsChWLheFLL7nZprhcX8vkib1uPMJcTrUjcibh4ral3zicQXjb1ujg/640)
在导航栏，run---> Edit Configurations
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vk55XFsCuF0OJJj2ugv6BGdxE8ibHGMVWp1foFiaOJy5dpT3oCqWMNPPw/640)
修改为
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vRHUicprVWNEOcFJAm8HhYopaCGDe7mgxCM4Ygz6ozyb2pF8PUZ1HPPA/640)
再运行，就ok了。
关于debug，只要run运行成功之后，直接可以debug的。。。
flink的源码调试debug及阅读经验，敬请期待后续，文章，也可以点击原文阅读加入浪尖知识星球。
推荐阅读：
[Flink异步IO第一讲](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485776&idx=1&sn=bb78ed1f45c7509688526cbd29686f4f&chksm=9f38ea78a84f636e468765b2f870e5f78afe5063f6a94b96cca540c9116fc39d7e4a123b6b6e&scene=21#wechat_redirect)
[flink 有状态udf 引起血案一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485723&idx=1&sn=6900dfd8e86318b13deabb247d8f83bc&chksm=9f38ea33a84f6325d62df7f984018bf5f16b18f185d83a5c6dd4ba91808356eb5e69183234df&scene=21#wechat_redirect)
[结合Spark讲一下Flink的runtime](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485964&idx=1&sn=224f6cc89d9e824055673f5ac313d28b&chksm=9f38e924a84f603203311d7c561bfcb3d5fca7241adf3540c4b50e0efca63040add04b2b64a1&scene=21#wechat_redirect)
[一文精通kafka 消费者的三种语义](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485744&idx=1&sn=42cdc89f5318519ec07fb81c2bd28f54&chksm=9f38ea18a84f630e448c0eb5fe1a43eaac6ee227ef85d063f66a378e1293923740797a8a3ec5&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
点赞，然后分享给小伙伴吧～
