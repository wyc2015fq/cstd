# Java和Maven的Cloud Bigtable HBase 客户端配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 18:08:37[boonya](https://me.csdn.net/boonya)阅读数：784








原文地址：[https://cloud.google.com/bigtable/docs/using-maven](https://cloud.google.com/bigtable/docs/using-maven)


本文采用Google翻译。

[Cloud Bigtable](https://cloud.google.com/bigtable/)用于Java的HBase客户端在[Maven存储库](http://mvnrepository.com/)中可用，使得在[Apache
 Maven](http://maven.apache.org/)项目中轻松使用Cloud BigTable。



## Maven工件概述


Cloud Bigtable Java的Java客户端包括三个您可以在项目中使用的Maven工件：

[bigtable-hbase-1.0](http://mvnrepository.com/artifact/com.google.cloud.bigtable/bigtable-hbase-1.0)：基于HBase 1.0.x的客户端。
[bigtable-hbase-1.1](http://mvnrepository.com/artifact/com.google.cloud.bigtable/bigtable-hbase-1.1)：基于HBase 1.1.x的客户端。
[bigtable-hbase-1.2](http://mvnrepository.com/artifact/com.google.cloud.bigtable/bigtable-hbase-1.2)：基于HBase 1.2.x的客户端。
[bigtable-hbase-mapreduce](http://mvnrepository.com/artifact/com.google.cloud.bigtable/bigtable-hbase-mapreduce)：用于将数据导入到Cloud BigTable的MapReduce作业。
每个工件都跟踪不同版本的HBase的API。这些说明的其余部分假设您使用bigtable-hbase-1.2。

注意：Maven存储库包括Cloud BigTable的其他工件。不要在应用程序中使用这些附加工件。
您还必须添加适当的工件以支持对数据进行加密。有关详细信息，请参阅“设置加密”。



## 将Cloud Bigtable工件添加到项目


要将Cloud Bigtable工件添加到Maven项目，只需将工件添加到pom.xml文件的<dependencies>部分以及HBase客户端的工件。

要添加所需的工件：



```
<dependencies>
  <groupId> com.google.cloud.bigtable </ groupId>
  <artifactId> bigtable-hbase-1.2 </ artifactId>
  <version> 0.9.4 </ version>
</ dependency>
<dependencies>
  <groupId> org.apache.hbase </ groupId>
  <artifactId> hbase-client </ artifactId>
  <version> 1.2.1 </ version>
</ dependency>
```

## 设置加密


为了加密数据，Cloud Bigtable HBase客户端使用[netty-tcnative](http://netty.io/wiki/forked-tomcat-native.html)库。为了简化安装，您应该使用静态链接到[BoringSSL](https://boringssl.googlesource.com/boringssl/)（OpenSSL的一个分支）的网络工件。这个工件被称为netty-tcnative-boringssl-static。

要使用netty-tcnative-boringssl-static，您的系统必须满足以下要求：


- netty-tcnative-boringssl-static 1.1.33.Fork19必须在Java类路径上。
（其他版本与Cloud Bigtable HBase客户端不兼容。）下面的示例显示如何将此依赖项添加到pom.xml文件中。
- 您必须在x86
 64位处理器上使用Linux，OS X或Windows。

要将所需的工件添加到pom.xml文件中：



```
<dependencies>
  <groupId> io.netty </ groupId>
  <artifactId> netty-tcnative-boringssl-static </ artifactId>
  <version> 1.1.33.Fork19 </ version>
</ dependency>
```





