# Mycat源码打包-包含各平台安装包 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月11日 20:34:05[boonya](https://me.csdn.net/boonya)阅读数：1173








使用maven打包即可，打包后会生成相应的安装包和jar包，maven如果打包失败请删除失败jar包的仓库目录重新打包下载jar包。

打包命令：


`mvn clean install -Dmaven.test.skip=true`



打包生成文件：

![](https://img-blog.csdn.net/20170811203206812)

详细打包日志：



```
[INFO] Scanning for projects...
[WARNING] 
[WARNING] Some problems were encountered while building the effective model for io.mycat:Mycat-server:jar:1.6-RELEASE
[WARNING] 'build.plugins.plugin.version' for org.apache.maven.plugins:maven-compiler-plugin is missing. @ line 361, column 12
[WARNING] 'build.plugins.plugin.version' for org.apache.maven.plugins:maven-jar-plugin is missing. @ line 388, column 12
[WARNING] 'build.plugins.plugin.version' for org.apache.maven.plugins:maven-eclipse-plugin is missing. @ line 406, column 12
[WARNING] 
[WARNING] It is highly recommended to fix these problems because they threaten the stability of your build.
[WARNING] 
[WARNING] For this reason, future Maven versions might no longer support building such malformed projects.
[WARNING] 
[INFO] 
[INFO] ------------------------------------------------------------------------
[INFO] Building Mycat-server 1.6-RELEASE
[INFO] ------------------------------------------------------------------------
[INFO] 
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ Mycat-server ---
[INFO] Deleting D:\workspace-sts-3.8.4.RELEASE\mycat\target
[INFO] 
[INFO] --- buildnumber-maven-plugin:1.3:create (default) @ Mycat-server ---
[INFO] Change the default 'git' provider implementation to 'git'.
[INFO] Executing: cmd.exe /X /C "git rev-parse --verify HEAD"
[INFO] Working directory: D:\workspace-sts-3.8.4.RELEASE\mycat
[INFO] Storing buildNumber: null at timestamp: 20170811203730
[WARNING] Cannot get the branch information from the git repository: 
Detecting the current branch failed: fatal: Not a git repository (or any of the parent directories): .git

[INFO] Executing: cmd.exe /X /C "git rev-parse --verify HEAD"
[INFO] Working directory: D:\workspace-sts-3.8.4.RELEASE\mycat
[INFO] Storing buildScmBranch: UNKNOWN_BRANCH
[INFO] 
[INFO] --- replacer:1.5.3:replace (version) @ Mycat-server ---
[INFO] Replacement run on 1 file.
[INFO] 
[INFO] --- replacer:1.5.3:replace (version2) @ Mycat-server ---
[INFO] Replacement run on 1 file.
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ Mycat-server ---
[WARNING] Using platform encoding (UTF-8 actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 44 resources
[INFO] Copying 1 resource
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ Mycat-server ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 793 source files to D:\workspace-sts-3.8.4.RELEASE\mycat\target\classes
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/utils/BytesTools.java:[25,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[23,16] sun.misc.Cleaner是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[24,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[25,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/manager/response/ShowDirectMemory.java:[14,23] sun.rmi.runtime.Log是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/ByteBufferChunk.java:[5,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/util/FastByteOperations.java:[27,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[10,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/utils/BytesTools.java:[25,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[23,16] sun.misc.Cleaner是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[24,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[25,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/manager/response/ShowDirectMemory.java:[14,23] sun.rmi.runtime.Log是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/ByteBufferChunk.java:[5,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/util/FastByteOperations.java:[27,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[10,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/utils/BytesTools.java:[25,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[23,16] sun.misc.Cleaner是��部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[24,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[25,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/manager/response/ShowDirectMemory.java:[14,23] sun.rmi.runtime.Log是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/ByteBufferChunk.java:[5,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/util/FastByteOperations.java:[27,16] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[10,18] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[43,26] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[263,13] sun.misc.Cleaner是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[263,31] sun.misc.Cleaner是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[321,9] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[323,33] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/memory/unsafe/Platform.java:[325,23] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/ByteBufferPage.java:[26,36] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/ByteBufferChunk.java:[49,29] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/ByteBufferChunk.java:[69,26] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/ByteBufferChunk.java:[172,26] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/util/FastByteOperations.java:[134,22] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/util/FastByteOperations.java:[143,26] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/util/FastByteOperations.java:[151,45] sun.misc.Unsafe是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[89,32] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[97,9] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[97,36] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[99,9] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/buffer/DirectByteBufferPool.java:[99,35] sun.nio.ch.DirectBuffer是内部专用 API, 可能会在未来发行版中删除
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/backend/jdbc/mongodb/MongoPreparedStatement.java: 某些输入文件使用或覆盖了已过时的 API。
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/backend/jdbc/mongodb/MongoPreparedStatement.java: 有关详细信息, 请使用 -Xlint:deprecation 重新编译。
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/backend/jdbc/mongodb/MongoPreparedStatement.java: 某些输入文件使用了未经检查或不安全的操作。
[WARNING] /D:/workspace-sts-3.8.4.RELEASE/mycat/src/main/java/io/mycat/backend/jdbc/mongodb/MongoPreparedStatement.java: 有关详细信息, 请使用 -Xlint:unchecked 重新编译。
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ Mycat-server ---
[INFO] Not copying test resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ Mycat-server ---
[INFO] Not compiling test sources
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ Mycat-server ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ Mycat-server ---
[INFO] Building jar: D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE.jar
[INFO] 
[INFO] --- maven-source-plugin:2.1.2:jar-no-fork (attach-sources) @ Mycat-server ---
[INFO] Building jar: D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-sources.jar
[INFO] 
[INFO] --- maven-jar-plugin:2.4:test-jar (default) @ Mycat-server ---
[INFO] Skipping packaging of the test-jar
[INFO] 
[INFO] --- appassembler-maven-plugin:1.7:generate-daemons (generate-jsw) @ Mycat-server ---
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\mongodb\mongo-java-driver\2.11.4\mongo-java-driver-2.11.4.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\mongo-java-driver-2.11.4.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\iq80\leveldb\leveldb\0.7\leveldb-0.7.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\leveldb-0.7.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\iq80\leveldb\leveldb-api\0.7\leveldb-api-0.7.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\leveldb-api-0.7.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\google\guava\guava\19.0\guava-19.0.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\guava-19.0.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\alibaba\druid\1.0.26\druid-1.0.26.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\druid-1.0.26.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\net\sf\ehcache\ehcache-core\2.6.11\ehcache-core-2.6.11.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\ehcache-core-2.6.11.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\slf4j\slf4j-api\1.6.1\slf4j-api-1.6.1.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\slf4j-api-1.6.1.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\mapdb\mapdb\1.0.7\mapdb-1.0.7.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\mapdb-1.0.7.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\velocity\velocity\1.7\velocity-1.7.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\velocity-1.7.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\commons-collections\commons-collections\3.2.1\commons-collections-3.2.1.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\commons-collections-3.2.1.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\lmax\disruptor\3.3.4\disruptor-3.3.4.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\disruptor-3.3.4.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\logging\log4j\log4j-slf4j-impl\2.5\log4j-slf4j-impl-2.5.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\log4j-slf4j-impl-2.5.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\logging\log4j\log4j-api\2.5\log4j-api-2.5.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\log4j-api-2.5.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\logging\log4j\log4j-core\2.5\log4j-core-2.5.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\log4j-core-2.5.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\logging\log4j\log4j-1.2-api\2.5\log4j-1.2-api-2.5.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\log4j-1.2-api-2.5.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\univocity\univocity-parsers\2.2.1\univocity-parsers-2.2.1.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\univocity-parsers-2.2.1.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\sequoiadb\sequoiadb-driver\1.12\sequoiadb-driver-1.12.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\sequoiadb-driver-1.12.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\log4j\log4j\1.2.17\log4j-1.2.17.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\log4j-1.2.17.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\dom4j\dom4j\1.6.1\dom4j-1.6.1.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\dom4j-1.6.1.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\curator\curator-framework\2.11.0\curator-framework-2.11.0.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\curator-framework-2.11.0.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\curator\curator-client\2.11.0\curator-client-2.11.0.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\curator-client-2.11.0.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\curator\curator-recipes\2.11.0\curator-recipes-2.11.0.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\curator-recipes-2.11.0.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\apache\zookeeper\zookeeper\3.4.6\zookeeper-3.4.6.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\zookeeper-3.4.6.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\jline\jline\0.9.94\jline-0.9.94.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\jline-0.9.94.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\netty\netty\3.7.0.Final\netty-3.7.0.Final.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\netty-3.7.0.Final.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\yaml\snakeyaml\1.17\snakeyaml-1.17.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\snakeyaml-1.17.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\alibaba\fastjson\1.2.12\fastjson-1.2.12.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\fastjson-1.2.12.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\joda-time\joda-time\2.9.3\joda-time-2.9.3.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\joda-time-2.9.3.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\github\shyiko\mysql-binlog-connector-java\0.4.1\mysql-binlog-connector-java-0.4.1.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\mysql-binlog-connector-java-0.4.1.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\google\code\findbugs\jsr305\2.0.3\jsr305-2.0.3.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\jsr305-2.0.3.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\esotericsoftware\kryo\kryo\2.10\kryo-2.10.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\kryo-2.10.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\esotericsoftware\reflectasm\reflectasm\1.03\reflectasm-1.03.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\reflectasm-1.03.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\ow2\asm\asm\4.0\asm-4.0.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\asm-4.0.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\esotericsoftware\minlog\minlog\1.2\minlog-1.2.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\minlog-1.2.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\objenesis\objenesis\1.2\objenesis-1.2.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\objenesis-1.2.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\hamcrest\hamcrest-library\1.3\hamcrest-library-1.3.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\hamcrest-library-1.3.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\hamcrest\hamcrest-core\1.3\hamcrest-core-1.3.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\hamcrest-core-1.3.jar
[INFO] Installing artifact D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\commons-lang\commons-lang\2.6\commons-lang-2.6.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\commons-lang-2.6.jar
[INFO] Installing artifact D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE.jar to D:\workspace-sts-3.8.4.RELEASE\mycat\target\generated-resources\appassembler\jsw\mycat\lib\Mycat-server-1.6-RELEASE.jar
[INFO] 
[INFO] --- maven-assembly-plugin:2.2-beta-5:single (make-assembly) @ Mycat-server ---
[INFO] Reading assembly descriptor: src/main/assembly/assembly-win.xml
[INFO] Reading assembly descriptor: src/main/assembly/assembly-linux.xml
[INFO] Reading assembly descriptor: src/main/assembly/assembly-mac.xml
[INFO] Reading assembly descriptor: src/main/assembly/assembly-solaris.xml
[INFO] Reading assembly descriptor: src/main/assembly/assembly-unix.xml
[INFO] Reading assembly descriptor: src/main/assembly/assembly-testtool.xml
[INFO] mycat/conf/ already added, skipping
[INFO] Building tar : D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-win.tar.gz
[INFO] mycat/conf/ already added, skipping
[INFO] mycat/conf/ already added, skipping
[INFO] Building tar : D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-linux.tar.gz
[INFO] mycat/conf/ already added, skipping
[INFO] mycat/conf/ already added, skipping
[INFO] Building tar : D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-mac.tar.gz
[INFO] mycat/conf/ already added, skipping
[INFO] mycat/conf/ already added, skipping
[INFO] Building tar : D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-solaris.tar.gz
[INFO] mycat/conf/ already added, skipping
[INFO] mycat/conf/ already added, skipping
[INFO] Building tar : D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-unix.tar.gz
[INFO] mycat/conf/ already added, skipping
[INFO] Building tar : D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-testtool.tar.gz
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ Mycat-server ---
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE.jar to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE.jar
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\pom.xml to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE.pom
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-sources.jar to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE-sources.jar
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-win.tar.gz to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE-20170811203730-win.tar.gz
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-linux.tar.gz to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE-20170811203730-linux.tar.gz
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-mac.tar.gz to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE-20170811203730-mac.tar.gz
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-solaris.tar.gz to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE-20170811203730-solaris.tar.gz
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-unix.tar.gz to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE-20170811203730-unix.tar.gz
[INFO] Installing D:\workspace-sts-3.8.4.RELEASE\mycat\target\Mycat-server-1.6-RELEASE-20170811203730-testtool.tar.gz to D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\io\mycat\Mycat-server\1.6-RELEASE\Mycat-server-1.6-RELEASE-20170811203730-testtool.tar.gz
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 01:38 min
[INFO] Finished at: 2017-08-11T20:39:04+08:00
[INFO] Final Memory: 34M/540M
[INFO] ------------------------------------------------------------------------
```







