# 如何在CentOS6.5下编译64位的Hadoop2.x(一) - 零度的博客专栏 - CSDN博客
2016年10月13日 16:54:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：458
hadoop2.x在apache官网直接下载的并没有64位直接能用的版本，如果我们想在64位系统使用，那么就需要重新编译hadoop，否则直接使用32位的hadoop运行在64位的系统上，将会出现一些库不兼容的异常。如下图所示，最直接的一个异常:
![](http://dl2.iteye.com/upload/attachment/0097/9474/0f2d2143-d0c2-33c6-98b8-fed60901a798.jpg)
在这之前，先用一个表格来描述下散仙的编译的环境的状况: 
|序号|描述|备注|
|----|----|----|
|1|centos6.5系统64位|linux环境|
|2|Apache Ant1.9|ant编译|
|3|Apache Maven3.2.1|maven打包部署|
|4|gcc,gcc-c++,make|依赖库|
|5|protobuf-2.5.0|序列化库|
|6|JDK1.7|JAVA 环境|
|7|Hadoop2.2.0源码包|官网下载|
|8|屌丝工程师一名|主角|
|9|hadoop交流群376932160|技术交流|
下面进入正题，散仙的环境是在centos下，所以大部分安装编译依赖库，都可以很方便的使用yum命令来完成。
1,安装gcc，执行如下的几个yum命令即可 
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- yum -y install gcc  
- yum install -y  bzip2-devel  
- yum -y install gcc-c++  
- yum install make  
- yum install autoconf automake libtool cmake ncurses-devel openssl-devel gcc*  
```java
<span style="font-family:Arial;font-size:12px;">yum -y install gcc
yum install -y  bzip2-devel
yum -y install gcc-c++
yum install make
yum install autoconf automake libtool cmake ncurses-devel openssl-devel gcc*
</span>
```
2,安装JDK,并设置环境变量，完成后测试安装成功否
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [root@ganglia ~]# java -version  
- java version "1.5.0"
- gij (GNU libgcj) version 4.4.720120313 (Red Hat 4.4.7-4)  
- 
- Copyright (C) 2007 Free Software Foundation, Inc.  
- This is free software; see the source for copying conditions.  There is NO  
- warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
- [root@ganglia ~]#   
```java
<span style="font-family:Arial;font-size:12px;">[root@ganglia ~]# java -version
java version "1.5.0"
gij (GNU libgcj) version 4.4.7 20120313 (Red Hat 4.4.7-4)
Copyright (C) 2007 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
[root@ganglia ~]# </span>
```
3， 安装Maven，安装完成后测试安装与否
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [root@ganglia ~]# mvn -v  
- Apache Maven 3.2.1 (ea8b2b07643dbb1b84b6d16e1f08391b666bc1e9; 2014-02-15T01:37:52+08:00)  
- Maven home: /usr/local/maven  
- Java version: 1.7.0_25, vendor: Oracle Corporation  
- Java home: /usr/local/jdk1.7.0_25/jre  
- Default locale: zh_CN, platform encoding: UTF-8
- OS name: "linux", version: "2.6.32-431.el6.x86_64", arch: "amd64", family: "unix"
- [root@ganglia ~]#   
```java
<span style="font-family:Arial;font-size:12px;">[root@ganglia ~]# mvn -v
Apache Maven 3.2.1 (ea8b2b07643dbb1b84b6d16e1f08391b666bc1e9; 2014-02-15T01:37:52+08:00)
Maven home: /usr/local/maven
Java version: 1.7.0_25, vendor: Oracle Corporation
Java home: /usr/local/jdk1.7.0_25/jre
Default locale: zh_CN, platform encoding: UTF-8
OS name: "linux", version: "2.6.32-431.el6.x86_64", arch: "amd64", family: "unix"
[root@ganglia ~]# </span>
```
4, 安装Ant， 安装完成后，依旧测试成功与否
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [root@ganglia ~]# ant -version  
- Apache Ant(TM) version 1.9.4 compiled on April 292014
- [root@ganglia ~]#   
```java
<span style="font-family:Arial;font-size:12px;">[root@ganglia ~]# ant -version
Apache Ant(TM) version 1.9.4 compiled on April 29 2014
[root@ganglia ~]# </span>
```
5,安装protobuf，安装方式，从官网下载tar.gz的包[点击下载](https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.bz2)，并上传到linux上解压，然后进入根目录下，执行如下的几个命令:
wget https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.bz2 
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- ./configure   
- make   
- make check  
- make install  
```java
<span style="font-family:Arial;font-size:12px;">./configure 
make 
make check
make install</span>
```
然后，执行如下命令，进行测试安装成功与否
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [root@ganglia protobuf-2.5.0]# protoc  
- Missing input file.  
- [root@ganglia protobuf-2.5.0]#   
```java
<span style="font-family:Arial;font-size:12px;">[root@ganglia protobuf-2.5.0]# protoc
Missing input file.
[root@ganglia protobuf-2.5.0]# </span>
```
6,从hadoop官网下载hadoop2.2.0的版本的源码的src的包，并查看目录
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [root@ganglia ~]# cd hadoop-2.2.0-src  
- [root@ganglia hadoop-2.2.0-src]# ll  
- 总用量 108
- -rw-r--r--.  167974 users  996810月  72013 BUILDING.txt  
- drwxr-xr-x.  267974 users  409610月  72013 dev-support  
- drwxr-xr-x.  467974 users  40966月   917:05 hadoop-assemblies  
- drwxr-xr-x.  367974 users  40966月   917:27 hadoop-client  
- drwxr-xr-x.  967974 users  40966月   917:14 hadoop-common-project  
- drwxr-xr-x.  367974 users  40966月   917:26 hadoop-dist  
- drwxr-xr-x.  767974 users  40966月   917:20 hadoop-hdfs-project  
- drwxr-xr-x. 1167974 users  40966月   917:25 hadoop-mapreduce-project  
- drwxr-xr-x.  467974 users  40966月   917:06 hadoop-maven-plugins  
- drwxr-xr-x.  367974 users  40966月   917:27 hadoop-minicluster  
- drwxr-xr-x.  467974 users  40966月   917:03 hadoop-project  
- drwxr-xr-x.  367974 users  40966月   917:05 hadoop-project-dist  
- drwxr-xr-x. 1267974 users  40966月   917:26 hadoop-tools  
- drwxr-xr-x.  467974 users  40966月   917:24 hadoop-yarn-project  
- -rw-r--r--.  167974 users 1516410月  72013 LICENSE.txt  
- -rw-r--r--.  167974 users   10110月  72013 NOTICE.txt  
- -rw-r--r--.  167974 users 1656910月  72013 pom.xml  
- -rw-r--r--.  167974 users  136610月  72013 README.txt  
- [root@ganglia hadoop-2.2.0-src]#   
```java
<span style="font-family:Arial;font-size:12px;">[root@ganglia ~]# cd hadoop-2.2.0-src
[root@ganglia hadoop-2.2.0-src]# ll
总用量 108
-rw-r--r--.  1 67974 users  9968 10月  7 2013 BUILDING.txt
drwxr-xr-x.  2 67974 users  4096 10月  7 2013 dev-support
drwxr-xr-x.  4 67974 users  4096 6月   9 17:05 hadoop-assemblies
drwxr-xr-x.  3 67974 users  4096 6月   9 17:27 hadoop-client
drwxr-xr-x.  9 67974 users  4096 6月   9 17:14 hadoop-common-project
drwxr-xr-x.  3 67974 users  4096 6月   9 17:26 hadoop-dist
drwxr-xr-x.  7 67974 users  4096 6月   9 17:20 hadoop-hdfs-project
drwxr-xr-x. 11 67974 users  4096 6月   9 17:25 hadoop-mapreduce-project
drwxr-xr-x.  4 67974 users  4096 6月   9 17:06 hadoop-maven-plugins
drwxr-xr-x.  3 67974 users  4096 6月   9 17:27 hadoop-minicluster
drwxr-xr-x.  4 67974 users  4096 6月   9 17:03 hadoop-project
drwxr-xr-x.  3 67974 users  4096 6月   9 17:05 hadoop-project-dist
drwxr-xr-x. 12 67974 users  4096 6月   9 17:26 hadoop-tools
drwxr-xr-x.  4 67974 users  4096 6月   9 17:24 hadoop-yarn-project
-rw-r--r--.  1 67974 users 15164 10月  7 2013 LICENSE.txt
-rw-r--r--.  1 67974 users   101 10月  7 2013 NOTICE.txt
-rw-r--r--.  1 67974 users 16569 10月  7 2013 pom.xml
-rw-r--r--.  1 67974 users  1366 10月  7 2013 README.txt
[root@ganglia hadoop-2.2.0-src]# </span>
```
7，修改/root/hadoop-2.2.0-src/hadoop-common-project/hadoop-auth/pom.xml文件，增加，补丁内容，这部分是hadoop2.2.0的bug，如果是其他的2.x的版本，可以视情况而定，内容如下：
Xml代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- <dependency>
- <groupId>org.mockito</groupId>
- <artifactId>mockito-all</artifactId>
- <scope>test</scope>
- </dependency>
- <!--新增的内容开始 -->
- <dependency>
- <groupId>org.mortbay.jetty</groupId>
- <artifactId>jetty-util</artifactId>
- <scope>test</scope>
- </dependency>
- <!--新增的内容结束 -->
- <dependency>
- <groupId>org.mortbay.jetty</groupId>
- <artifactId>jetty</artifactId>
- <scope>test</scope>
- </dependency>
<span style="font-family:Arial;font-size:12px;">  <dependency>
      <groupId>org.mockito</groupId>
      <artifactId>mockito-all</artifactId>
      <scope>test</scope>
    </dependency>
	<!--新增的内容开始 -->
	<dependency>
		<groupId>org.mortbay.jetty</groupId>
		    <artifactId>jetty-util</artifactId>
		    <scope>test</scope>
	</dependency>
	<!--新增的内容结束 -->
    <dependency>
      <groupId>org.mortbay.jetty</groupId>
      <artifactId>jetty</artifactId>
      <scope>test</scope>
    </dependency></span>
8，修改完毕后，回到hadoop-2.2.0-src的跟目录下执行编译打包命令:
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- mvn clean  
- mvn package -Pdist,native -DskipTests -Dtar  
```java
<span style="font-family:Arial;font-size:12px;">mvn clean
mvn package -Pdist,native -DskipTests -Dtar
</span>
```
然后等待半个小时左右的编译时间，网速快的话，时间可能会更短，编译完成后，输出的打包信息如下：
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [INFO]   
- [INFO] --- maven-resources-plugin:2.2:resources (default-resources) @ hadoop-minicluster ---  
- [INFO] Using default encoding to copy filtered resources.  
- [INFO]   
- [INFO] --- maven-compiler-plugin:2.5.1:compile (default-compile) @ hadoop-minicluster ---  
- [INFO] No sources to compile  
- [INFO]   
- [INFO] --- maven-resources-plugin:2.2:testResources (default-testResources) @ hadoop-minicluster ---  
- [INFO] Using default encoding to copy filtered resources.  
- [INFO]   
- [INFO] --- maven-compiler-plugin:2.5.1:testCompile (default-testCompile) @ hadoop-minicluster ---  
- [INFO] No sources to compile  
- [INFO]   
- [INFO] --- maven-surefire-plugin:2.12.3:test (default-test) @ hadoop-minicluster ---  
- [INFO] Tests are skipped.  
- [INFO]   
- [INFO] --- maven-jar-plugin:2.3.1:jar (default-jar) @ hadoop-minicluster ---  
- [WARNING] JAR will be empty - no content was marked for inclusion!  
- [INFO] Building jar: /root/hadoop-2.2.0-src/hadoop-minicluster/target/hadoop-minicluster-2.2.0.jar  
- [INFO]   
- [INFO] --- maven-source-plugin:2.1.2:jar-no-fork (hadoop-java-sources) @ hadoop-minicluster ---  
- [INFO] No sources in project. Archive not created.  
- [INFO]   
- [INFO] --- maven-source-plugin:2.1.2:test-jar-no-fork (hadoop-java-sources) @ hadoop-minicluster ---  
- [INFO] No sources in project. Archive not created.  
- [INFO]   
- [INFO] --- maven-site-plugin:3.0:attach-descriptor (attach-descriptor) @ hadoop-minicluster ---  
- [INFO]   
- [INFO] --- maven-javadoc-plugin:2.8.1:jar (module-javadocs) @ hadoop-minicluster ---  
- [INFO] Building jar: /root/hadoop-2.2.0-src/hadoop-minicluster/target/hadoop-minicluster-2.2.0-javadoc.jar  
- [INFO] ------------------------------------------------------------------------  
- [INFO] Reactor Summary:  
- [INFO]   
- [INFO] Apache Hadoop Main ................................ SUCCESS [01:43 min]  
- [INFO] Apache Hadoop Project POM ......................... SUCCESS [01:21 min]  
- [INFO] Apache Hadoop Annotations ......................... SUCCESS [ 42.256 s]  
- [INFO] Apache Hadoop Assemblies .......................... SUCCESS [  0.291 s]  
- [INFO] Apache Hadoop Project Dist POM .................... SUCCESS [ 41.053 s]  
- [INFO] Apache Hadoop Maven Plugins ....................... SUCCESS [ 44.283 s]  
- [INFO] Apache Hadoop Auth ................................ SUCCESS [01:49 min]  
- [INFO] Apache Hadoop Auth Examples ....................... SUCCESS [ 18.950 s]  
- [INFO] Apache Hadoop Common .............................. SUCCESS [05:31 min]  
- [INFO] Apache Hadoop NFS ................................. SUCCESS [ 40.498 s]  
- [INFO] Apache Hadoop Common Project ...................... SUCCESS [  0.050 s]  
- [INFO] Apache Hadoop HDFS ................................ SUCCESS [03:43 min]  
- [INFO] Apache Hadoop HttpFS .............................. SUCCESS [ 26.962 s]  
- [INFO] Apache Hadoop HDFS BookKeeper Journal ............. SUCCESS [ 47.056 s]  
- [INFO] Apache Hadoop HDFS-NFS ............................ SUCCESS [  4.237 s]  
- [INFO] Apache Hadoop HDFS Project ........................ SUCCESS [  0.029 s]  
- [INFO] hadoop-yarn ....................................... SUCCESS [01:25 min]  
- [INFO] hadoop-yarn-api ................................... SUCCESS [ 40.841 s]  
- [INFO] hadoop-yarn-common ................................ SUCCESS [ 31.228 s]  
- [INFO] hadoop-yarn-server ................................ SUCCESS [  0.161 s]  
- [INFO] hadoop-yarn-server-common ......................... SUCCESS [ 12.289 s]  
- [INFO] hadoop-yarn-server-nodemanager .................... SUCCESS [ 19.271 s]  
- [INFO] hadoop-yarn-server-web-proxy ...................... SUCCESS [  3.586 s]  
- [INFO] hadoop-yarn-server-resourcemanager ................ SUCCESS [ 14.674 s]  
- [INFO] hadoop-yarn-server-tests .......................... SUCCESS [  1.153 s]  
- [INFO] hadoop-yarn-client ................................ SUCCESS [  7.861 s]  
- [INFO] hadoop-yarn-applications .......................... SUCCESS [  0.106 s]  
- [INFO] hadoop-yarn-applications-distributedshell ......... SUCCESS [  4.540 s]  
- [INFO] hadoop-mapreduce-client ........................... SUCCESS [  0.168 s]  
- [INFO] hadoop-mapreduce-client-core ...................... SUCCESS [ 29.360 s]  
- [INFO] hadoop-yarn-applications-unmanaged-am-launcher .... SUCCESS [  3.353 s]  
- [INFO] hadoop-yarn-site .................................. SUCCESS [  0.128 s]  
- [INFO] hadoop-yarn-project ............................... SUCCESS [ 29.610 s]  
- [INFO] hadoop-mapreduce-client-common .................... SUCCESS [ 19.908 s]  
- [INFO] hadoop-mapreduce-client-shuffle ................... SUCCESS [  3.357 s]  
- [INFO] hadoop-mapreduce-client-app ....................... SUCCESS [ 12.116 s]  
- [INFO] hadoop-mapreduce-client-hs ........................ SUCCESS [  5.807 s]  
- [INFO] hadoop-mapreduce-client-jobclient ................. SUCCESS [  6.713 s]  
- [INFO] hadoop-mapreduce-client-hs-plugins ................ SUCCESS [  2.001 s]  
- [INFO] Apache Hadoop MapReduce Examples .................. SUCCESS [  7.684 s]  
- [INFO] hadoop-mapreduce .................................. SUCCESS [  3.664 s]  
- [INFO] Apache Hadoop MapReduce Streaming ................. SUCCESS [  5.645 s]  
- [INFO] Apache Hadoop Distributed Copy .................... SUCCESS [ 29.953 s]  
- [INFO] Apache Hadoop Archives ............................ SUCCESS [  2.277 s]  
- [INFO] Apache Hadoop Rumen ............................... SUCCESS [  7.743 s]  
- [INFO] Apache Hadoop Gridmix ............................. SUCCESS [  5.608 s]  
- [INFO] Apache Hadoop Data Join ........................... SUCCESS [  3.385 s]  
- [INFO] Apache Hadoop Extras .............................. SUCCESS [  3.509 s]  
- [INFO] Apache Hadoop Pipes ............................... SUCCESS [  8.266 s]  
- [INFO] Apache Hadoop Tools Dist .......................... SUCCESS [  2.073 s]  
- [INFO] Apache Hadoop Tools ............................... SUCCESS [  0.025 s]  
- [INFO] Apache Hadoop Distribution ........................ SUCCESS [ 23.928 s]  
- [INFO] Apache Hadoop Client .............................. SUCCESS [  6.876 s]  
- [INFO] Apache Hadoop Mini-Cluster ........................ SUCCESS [  0.514 s]  
- [INFO] ------------------------------------------------------------------------  
- [INFO] BUILD SUCCESS  
- [INFO] ------------------------------------------------------------------------  
- [INFO] Total time: 26:04 min  
- [INFO] Finished at: 2014-06-09T17:27:26+08:00
- [INFO] Final Memory: 96M/239M  
- [INFO] ------------------------------------------------------------------------  
```java
<span style="font-family:Arial;font-size:12px;">[INFO] 
[INFO] --- maven-resources-plugin:2.2:resources (default-resources) @ hadoop-minicluster ---
[INFO] Using default encoding to copy filtered resources.
[INFO] 
[INFO] --- maven-compiler-plugin:2.5.1:compile (default-compile) @ hadoop-minicluster ---
[INFO] No sources to compile
[INFO] 
[INFO] --- maven-resources-plugin:2.2:testResources (default-testResources) @ hadoop-minicluster ---
[INFO] Using default encoding to copy filtered resources.
[INFO] 
[INFO] --- maven-compiler-plugin:2.5.1:testCompile (default-testCompile) @ hadoop-minicluster ---
[INFO] No sources to compile
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.3:test (default-test) @ hadoop-minicluster ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-jar-plugin:2.3.1:jar (default-jar) @ hadoop-minicluster ---
[WARNING] JAR will be empty - no content was marked for inclusion!
[INFO] Building jar: /root/hadoop-2.2.0-src/hadoop-minicluster/target/hadoop-minicluster-2.2.0.jar
[INFO] 
[INFO] --- maven-source-plugin:2.1.2:jar-no-fork (hadoop-java-sources) @ hadoop-minicluster ---
[INFO] No sources in project. Archive not created.
[INFO] 
[INFO] --- maven-source-plugin:2.1.2:test-jar-no-fork (hadoop-java-sources) @ hadoop-minicluster ---
[INFO] No sources in project. Archive not created.
[INFO] 
[INFO] --- maven-site-plugin:3.0:attach-descriptor (attach-descriptor) @ hadoop-minicluster ---
[INFO] 
[INFO] --- maven-javadoc-plugin:2.8.1:jar (module-javadocs) @ hadoop-minicluster ---
[INFO] Building jar: /root/hadoop-2.2.0-src/hadoop-minicluster/target/hadoop-minicluster-2.2.0-javadoc.jar
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary:
[INFO] 
[INFO] Apache Hadoop Main ................................ SUCCESS [01:43 min]
[INFO] Apache Hadoop Project POM ......................... SUCCESS [01:21 min]
[INFO] Apache Hadoop Annotations ......................... SUCCESS [ 42.256 s]
[INFO] Apache Hadoop Assemblies .......................... SUCCESS [  0.291 s]
[INFO] Apache Hadoop Project Dist POM .................... SUCCESS [ 41.053 s]
[INFO] Apache Hadoop Maven Plugins ....................... SUCCESS [ 44.283 s]
[INFO] Apache Hadoop Auth ................................ SUCCESS [01:49 min]
[INFO] Apache Hadoop Auth Examples ....................... SUCCESS [ 18.950 s]
[INFO] Apache Hadoop Common .............................. SUCCESS [05:31 min]
[INFO] Apache Hadoop NFS ................................. SUCCESS [ 40.498 s]
[INFO] Apache Hadoop Common Project ...................... SUCCESS [  0.050 s]
[INFO] Apache Hadoop HDFS ................................ SUCCESS [03:43 min]
[INFO] Apache Hadoop HttpFS .............................. SUCCESS [ 26.962 s]
[INFO] Apache Hadoop HDFS BookKeeper Journal ............. SUCCESS [ 47.056 s]
[INFO] Apache Hadoop HDFS-NFS ............................ SUCCESS [  4.237 s]
[INFO] Apache Hadoop HDFS Project ........................ SUCCESS [  0.029 s]
[INFO] hadoop-yarn ....................................... SUCCESS [01:25 min]
[INFO] hadoop-yarn-api ................................... SUCCESS [ 40.841 s]
[INFO] hadoop-yarn-common ................................ SUCCESS [ 31.228 s]
[INFO] hadoop-yarn-server ................................ SUCCESS [  0.161 s]
[INFO] hadoop-yarn-server-common ......................... SUCCESS [ 12.289 s]
[INFO] hadoop-yarn-server-nodemanager .................... SUCCESS [ 19.271 s]
[INFO] hadoop-yarn-server-web-proxy ...................... SUCCESS [  3.586 s]
[INFO] hadoop-yarn-server-resourcemanager ................ SUCCESS [ 14.674 s]
[INFO] hadoop-yarn-server-tests .......................... SUCCESS [  1.153 s]
[INFO] hadoop-yarn-client ................................ SUCCESS [  7.861 s]
[INFO] hadoop-yarn-applications .......................... SUCCESS [  0.106 s]
[INFO] hadoop-yarn-applications-distributedshell ......... SUCCESS [  4.540 s]
[INFO] hadoop-mapreduce-client ........................... SUCCESS [  0.168 s]
[INFO] hadoop-mapreduce-client-core ...................... SUCCESS [ 29.360 s]
[INFO] hadoop-yarn-applications-unmanaged-am-launcher .... SUCCESS [  3.353 s]
[INFO] hadoop-yarn-site .................................. SUCCESS [  0.128 s]
[INFO] hadoop-yarn-project ............................... SUCCESS [ 29.610 s]
[INFO] hadoop-mapreduce-client-common .................... SUCCESS [ 19.908 s]
[INFO] hadoop-mapreduce-client-shuffle ................... SUCCESS [  3.357 s]
[INFO] hadoop-mapreduce-client-app ....................... SUCCESS [ 12.116 s]
[INFO] hadoop-mapreduce-client-hs ........................ SUCCESS [  5.807 s]
[INFO] hadoop-mapreduce-client-jobclient ................. SUCCESS [  6.713 s]
[INFO] hadoop-mapreduce-client-hs-plugins ................ SUCCESS [  2.001 s]
[INFO] Apache Hadoop MapReduce Examples .................. SUCCESS [  7.684 s]
[INFO] hadoop-mapreduce .................................. SUCCESS [  3.664 s]
[INFO] Apache Hadoop MapReduce Streaming ................. SUCCESS [  5.645 s]
[INFO] Apache Hadoop Distributed Copy .................... SUCCESS [ 29.953 s]
[INFO] Apache Hadoop Archives ............................ SUCCESS [  2.277 s]
[INFO] Apache Hadoop Rumen ............................... SUCCESS [  7.743 s]
[INFO] Apache Hadoop Gridmix ............................. SUCCESS [  5.608 s]
[INFO] Apache Hadoop Data Join ........................... SUCCESS [  3.385 s]
[INFO] Apache Hadoop Extras .............................. SUCCESS [  3.509 s]
[INFO] Apache Hadoop Pipes ............................... SUCCESS [  8.266 s]
[INFO] Apache Hadoop Tools Dist .......................... SUCCESS [  2.073 s]
[INFO] Apache Hadoop Tools ............................... SUCCESS [  0.025 s]
[INFO] Apache Hadoop Distribution ........................ SUCCESS [ 23.928 s]
[INFO] Apache Hadoop Client .............................. SUCCESS [  6.876 s]
[INFO] Apache Hadoop Mini-Cluster ........................ SUCCESS [  0.514 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 26:04 min
[INFO] Finished at: 2014-06-09T17:27:26+08:00
[INFO] Final Memory: 96M/239M
[INFO] ------------------------------------------------------------------------</span>
```
编译好的hadoop包，路径在：
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [root@ganglia target]# pwd  
- /root/hadoop-2.2.0-src/hadoop-dist/target  
- [root@ganglia target]# ll  
- 总用量 282348
- drwxr-xr-x. 2 root root      40966月   917:26 antrun  
- -rw-r--r--. 1 root root      16186月   917:26 dist-layout-stitching.sh  
- -rw-r--r--. 1 root root       6356月   917:26 dist-tar-stitching.sh  
- drwxr-xr-x. 9 root root      40966月   917:26 hadoop-2.2.0
- -rw-r--r--. 1 root root  961838336月   917:27 hadoop-2.2.0.tar.gz  
- -rw-r--r--. 1 root root      27456月   917:26 hadoop-dist-2.2.0.jar  
- -rw-r--r--. 1 root root 1929034726月   917:27 hadoop-dist-2.2.0-javadoc.jar  
- drwxr-xr-x. 2 root root      40966月   917:27 javadoc-bundle-options  
- drwxr-xr-x. 2 root root      40966月   917:26 maven-archiver  
- drwxr-xr-x. 2 root root      40966月   917:26 test-dir  
- [root@ganglia target]#   
```java
<span style="font-family:Arial;font-size:12px;">[root@ganglia target]# pwd
/root/hadoop-2.2.0-src/hadoop-dist/target
[root@ganglia target]# ll
总用量 282348
drwxr-xr-x. 2 root root      4096 6月   9 17:26 antrun
-rw-r--r--. 1 root root      1618 6月   9 17:26 dist-layout-stitching.sh
-rw-r--r--. 1 root root       635 6月   9 17:26 dist-tar-stitching.sh
drwxr-xr-x. 9 root root      4096 6月   9 17:26 hadoop-2.2.0
-rw-r--r--. 1 root root  96183833 6月   9 17:27 hadoop-2.2.0.tar.gz
-rw-r--r--. 1 root root      2745 6月   9 17:26 hadoop-dist-2.2.0.jar
-rw-r--r--. 1 root root 192903472 6月   9 17:27 hadoop-dist-2.2.0-javadoc.jar
drwxr-xr-x. 2 root root      4096 6月   9 17:27 javadoc-bundle-options
drwxr-xr-x. 2 root root      4096 6月   9 17:26 maven-archiver
drwxr-xr-x. 2 root root      4096 6月   9 17:26 test-dir
[root@ganglia target]# 
</span>
```
编译完成后的本地库，位于如下位置，并查看本地库支持位数：
Java代码 ![复制代码](http://qindongliang.iteye.com/images/icon_copy.gif)![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)![](http://qindongliang.iteye.com/images/spinner.gif)
- [root@ganglianative]# pwd  
- /root/hadoop-2.2.0-src/hadoop-dist/target/hadoop-2.2.0/lib/native
- [root@ganglianative]# ll  
- 总用量 3596
- -rw-r--r--. 1 root root  7331146月   917:26 libhadoop.a  
- -rw-r--r--. 1 root root 14872366月   917:26 libhadooppipes.a  
- lrwxrwxrwx. 1 root root      186月   917:26 libhadoop.so -> libhadoop.so.1.0.0
- -rwxr-xr-x. 1 root root  4118706月   917:26 libhadoop.so.1.0.0
- -rw-r--r--. 1 root root  5819446月   917:26 libhadooputils.a  
- -rw-r--r--. 1 root root  2733306月   917:26 libhdfs.a  
- lrwxrwxrwx. 1 root root      166月   917:26 libhdfs.so -> libhdfs.so.0.0.0
- -rwxr-xr-x. 1 root root  1810426月   917:26 libhdfs.so.0.0.0
- [root@ganglianative]# file libhadoop.so  
- libhadoop.so: symbolic link to `libhadoop.so.1.0.0'  
- [root@ganglianative]# file libhadoop.so.1.0.0
- libhadoop.so.1.0.0: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, not stripped  
- [root@ganglianative]#   
```java
<span style="font-family:Arial;font-size:12px;">[root@ganglia native]# pwd
/root/hadoop-2.2.0-src/hadoop-dist/target/hadoop-2.2.0/lib/native
[root@ganglia native]# ll
总用量 3596
-rw-r--r--. 1 root root  733114 6月   9 17:26 libhadoop.a
-rw-r--r--. 1 root root 1487236 6月   9 17:26 libhadooppipes.a
lrwxrwxrwx. 1 root root      18 6月   9 17:26 libhadoop.so -> libhadoop.so.1.0.0
-rwxr-xr-x. 1 root root  411870 6月   9 17:26 libhadoop.so.1.0.0
-rw-r--r--. 1 root root  581944 6月   9 17:26 libhadooputils.a
-rw-r--r--. 1 root root  273330 6月   9 17:26 libhdfs.a
lrwxrwxrwx. 1 root root      16 6月   9 17:26 libhdfs.so -> libhdfs.so.0.0.0
-rwxr-xr-x. 1 root root  181042 6月   9 17:26 libhdfs.so.0.0.0
[root@ganglia native]# file libhadoop.so
libhadoop.so: symbolic link to `libhadoop.so.1.0.0'
[root@ganglia native]# file libhadoop.so.1.0.0 
libhadoop.so.1.0.0: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, not stripped
[root@ganglia native]# 
</span>
```
至此，我们的编译已经，成功完成，然后，我们就可以使用在target目录下，编译生成的hadoop新的tar.gz包，来部署我们的hadoop集群。
![](http://dl2.iteye.com/upload/attachment/0097/9492/1b35ce2f-6ee4-3adf-b392-8d815a4bf95b.jpg)
