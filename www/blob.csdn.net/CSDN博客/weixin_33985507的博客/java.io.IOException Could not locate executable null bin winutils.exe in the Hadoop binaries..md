# java.io.IOException: Could not locate executable null\bin\winutils.exe in the Hadoop binaries. - weixin_33985507的博客 - CSDN博客
2018年03月03日 13:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
问题描述：在idea中执行scala的spark程序，出现如下异常：
```
18/03/03 10:40:44 ERROR Shell: Failed to locate the winutils binary in the hadoop binary path
java.io.IOException: Could not locate executable null\bin\winutils.exe in the Hadoop binaries.
    at org.apache.hadoop.util.Shell.getQualifiedBinPath(Shell.java:356)
    at org.apache.hadoop.util.Shell.getWinUtilsPath(Shell.java:371)
    at org.apache.hadoop.util.Shell.<clinit>(Shell.java:364)
    at org.apache.hadoop.util.StringUtils.<clinit>(StringUtils.java:80)
    at org.apache.hadoop.security.SecurityUtil.getAuthenticationMethod(SecurityUtil.java:611)
    at org.apache.hadoop.security.UserGroupInformation.initialize(UserGroupInformation.java:272)
    at org.apache.hadoop.security.UserGroupInformation.ensureInitialized(UserGroupInformation.java:260)
    at org.apache.hadoop.security.UserGroupInformation.loginUserFromSubject(UserGroupInformation.java:790)
    at org.apache.hadoop.security.UserGroupInformation.getLoginUser(UserGroupInformation.java:760)
    at org.apache.hadoop.security.UserGroupInformation.getCurrentUser(UserGroupInformation.java:633)
    at org.apache.spark.util.Utils$$anonfun$getCurrentUserName$1.apply(Utils.scala:2464)
    at org.apache.spark.util.Utils$$anonfun$getCurrentUserName$1.apply(Utils.scala:2464)
    at scala.Option.getOrElse(Option.scala:121)
    at org.apache.spark.util.Utils$.getCurrentUserName(Utils.scala:2464)
    at org.apache.spark.SparkContext.<init>(SparkContext.scala:292)
    at com.spark.ruizhe.WordCount$.main(WordCount.scala:16)
    at com.spark.ruizhe.WordCount.main(WordCount.scala)
```
解决方案如下：
[Here](https://link.jianshu.com?t=http%3A%2F%2Fteknosrc.com%2Fspark-error-java-io-ioexception-could-not-locate-executable-null-bin-winutils-exe-hadoop-binaries%2F) is a good explanation of your problem with the solution.
- 
Download winutils.exe from [http://public-repo-1.hortonworks.com/hdp-win-alpha/winutils.exe](https://link.jianshu.com?t=http%3A%2F%2Fpublic-repo-1.hortonworks.com%2Fhdp-win-alpha%2Fwinutils.exe).
- 
SetUp your HADOOP_HOME environment variable on the OS level or programmatically:
System.setProperty("hadoop.home.dir", "C:\ProgramFiles\hadoop-3.0.0")
- 
Enjoy
