# Kryo：快速、高效的序列化框架 - z69183787的专栏 - CSDN博客
2017年09月21日 17:00:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2024
[http://hao.jobbole.com/kryo/](http://hao.jobbole.com/kryo/)
Kryo是一个快速高效的Java序列化框架，旨在提供快速、高效和易用的API。无论文件、数据库或网络数据Kryo都可以随时完成序列化。Kryo还可以执行自动深拷贝（克隆）、浅拷贝（克隆）。这是对象到对象的直接拷贝，非对象->字节->对象的拷贝。
![kryo](http://jbcdn1.b0.upaiyun.com/2016/01/7de2c7954bd8a8e358c6970560311d23.png)
## 安装
Kryo JAR可以在[发布页面](https://github.com/EsotericSoftware/kryo/releases)和[Maven中央仓库](http://search.maven.org/#browse%7C1975274176)下载。Kryo最新镜像可以在[Sonatype仓库](https://oss.sonatype.org/content/repositories/snapshots/com/esotericsoftware/kryo/kryo)找到，包括对master分支的镜像构建。
### Maven集成
要使用Kryo的官方发布版本，请将下面脚本添加到pom.xml中：

XHTML
```
```
如果遇到问题，可能是因为你的classpath上已经安装了不同版本的asm。可以使用kryo-shaded jar中的asm版本，重新定位到不同的package：

XHTML
``
如果要测试最新的Kryo镜像，请在pom.xml中使用下面代码：

XHTML
```
```
## 快速上手
下面展示了如何使用Kryo：

Java
``
Kryo类负责协调了对象序列化。Output和Input类负责缓存字节并flush到流中（这一步可选）。下面展示了序列化过程的细节和Kryo的优势。
### 非Maven环境使用Kryo
如果在非Maven环境下使用Kryo，请注意Kryo jar有一些外部依赖，这些JAR也需要添加到你的classpath中。包括[MinLog日志库](https://github.com/EsotericSoftware/minlog/)和[Objenesis库](https://code.google.com/p/objenesis/)。
## 主要特性
- [IO](http://hao.jobbole.com/kryo/#io)
- [基于Unsafe的IO](http://hao.jobbole.com/kryo/#unsafe-based-io)
- [Serializer](http://hao.jobbole.com/kryo/#serializers)
- [Registration](http://hao.jobbole.com/kryo/#registration)
- [默认serializer](http://hao.jobbole.com/kryo/#default-serializers)
- [FieldSerializer](http://hao.jobbole.com/kryo/#fieldserializer)
- [KryoSerializable](http://hao.jobbole.com/kryo/#kryoserializable)
- [Class字段注解](http://hao.jobbole.com/kryo/#class-fields-annotations)
- [Java序列化](http://hao.jobbole.com/kryo/#using-standard-java-serialization)
- [读写](http://hao.jobbole.com/kryo/#reading-and-writing)
- [引用](http://hao.jobbole.com/kryo/#references)
- [创建对象](http://hao.jobbole.com/kryo/#object-creation)
- [拷贝/克隆](http://hao.jobbole.com/kryo/#copyingcloning)
- [Context](http://hao.jobbole.com/kryo/#context)
- [压缩和加密](http://hao.jobbole.com/kryo/#compression-and-encryption)
- [区块编码](http://hao.jobbole.com/kryo/#chunked-encoding)
- [兼容性](http://hao.jobbole.com/kryo/#compatibility)
- [互操作能力](http://hao.jobbole.com/kryo/#interoperability)
- [堆栈大小](http://hao.jobbole.com/kryo/#stack-size)
- [线程](http://hao.jobbole.com/kryo/#threading)
- [Kryo实例池](http://hao.jobbole.com/kryo/#pooling-kryo-instances)
- [日志](http://hao.jobbole.com/kryo/#logging)
- [Scala](http://hao.jobbole.com/kryo/#scala)
- [Objective-C](http://hao.jobbole.com/kryo/#objective-c)
## 开发资源
- 上面介绍的功能针对Kryo v2及更高版本。了解v1.x请参阅[V1文档](https://github.com/EsotericSoftware/kryo/wiki/Documentation-for-Kryo-version-1.x)。
- 使用Kryo进行网络通信[KryoNet](https://github.com/EsotericSoftware/kryonet)是不错的选择。
- [kryo邮件列表](https://groups.google.com/forum/#!forum/kryo-users)
- 使用Kryo的项目：
- [KryoNet](http://code.google.com/p/kryonet/)（NIO网络通信）
- [Twitter’s Scalding](https://github.com/twitter/scalding)（级联Scala API）
- [Twitter’s Chill](https://github.com/twitter/chill)（Scala版的Kryo Serializer）
- [Apache Hive](http://hive.apache.org/) （查询计划序列化）
- [DataNucleus](https://github.com/datanucleus/type-converter-kryo) （JDO/JPA持久化框架）
- [CloudPelican](http://www.cloudpelican.com/)
- [Yahoo’s S4](http://www.s4.io/) （分布式流式计算）
- [Storm](https://github.com/nathanmarz/storm/wiki/Serialization) （分布式实时计算系统，反过来被许多[其他工具](https://github.com/nathanmarz/storm/wiki/Powered-By)使用）
- [Cascalog](https://github.com/nathanmarz/cascalog) （Clojure/Java数据处理和查询[细节](https://groups.google.com/d/msg/cascalog-user/qgwO2vbkRa0/UeClnLL5OsgJ)）
- [memcached-session-manager](https://code.google.com/p/memcached-session-manager/)（Tomcat高可用性会话）
- [Mobility-RPC](http://code.google.com/p/mobility-rpc/) （启用RPC的分布式应用程序）
- [akka-kryo-serialization](https://github.com/romix/akka-kryo-serialization) （Akka版的Kryo Serializer）
- [Groupon](https://code.google.com/p/kryo/issues/detail?id=67)
- [Jive](http://www.jivesoftware.com/jivespace/blogs/jivespace/2010/07/29/the-jive-sbs-cache-redesign-part-3)
- [DestroyAllHumans](https://code.google.com/p/destroyallhumans/) （[机器人控制](http://www.youtube.com/watch?v=ZeZ3R38d3Cg)！）
- [kryo-serializers](https://github.com/magro/kryo-serializers) （其它Serializer）
