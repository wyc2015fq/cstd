# Netty Failure to transfer io.netty:netty-tcnative:jar:${os.detected.classifier}:2.0.0.Final 问题解决 - z69183787的专栏 - CSDN博客
2017年10月16日 16:27:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1574
Netty框架中，pom文件中引用netty-alljar发生错误，解决办法。
错误现象：
Failure to transfer io.netty:netty-tcnative:jar:${os.detected.classifier}:2.0.0.Final from http://maven.aliyun.com/nexus/content/repositories/central/ was cached in the local repository, resolution will not be reattempted until the update interval of alimaven
 has elapsed or updates are forced. Original error: Could not transfer artifact io.netty:netty-tcnative:jar:${os.detected.classifier}:2.0.0.Final from/to alimaven (http://maven.aliyun.com/nexus/content/repositories/central/): Illegal character in path at index
 123: http://maven.aliyun.com/nexus/content/repositories/central/io/netty/netty-tcnative/2.0.0.Final/netty-tcnative-2.0.0.Final-${os.detected.classifier}.jar
解决办法：下列黑体字的添加。
<dependency>
<groupId>io.netty</groupId>
<artifactId>netty-all</artifactId>
<version>4.1.9.Final</version>
<exclusions>
 <exclusion>
  <groupId>io.netty</groupId>
  <artifactId>netty-tcnative</artifactId>
 </exclusion>
</exclusions>
</dependency>
<dependency>
<groupId>io.netty</groupId>
<artifactId>netty-example</artifactId>
<version>4.1.9.Final</version>
<exclusions>
 <exclusion>
  <groupId>io.netty</groupId>
  <artifactId>netty-tcnative</artifactId>
 </exclusion>
</exclusions>
</dependency>
