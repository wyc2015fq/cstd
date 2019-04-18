# 【Spark Mllib】逻辑回归——垃圾邮件分类器与maven构建独立项目 - Soul Joy Hub - CSDN博客

2016年06月13日 10:05:34[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3213
所属专栏：[Spark机器学习](https://blog.csdn.net/column/details/15014.html)



                
http://blog.csdn.net/u011239443/article/details/51655469**使用SGD算法逻辑回归的垃圾邮件分类器**
```
1 package com.oreilly.learningsparkexamples.scala
 2 
 3 import org.apache.spark.{SparkConf, SparkContext}
 4 import org.apache.spark.mllib.classification.LogisticRegressionWithSGD
 5 import org.apache.spark.mllib.feature.HashingTF
 6 import org.apache.spark.mllib.regression.LabeledPoint
 7 
 8 object MLlib {
 9 
10   def main(args: Array[String]) {
11     val conf = new SparkConf().setAppName(s"MLlib example")
12     val sc = new SparkContext(conf)
13 
14     // Load 2 types of emails from text files: spam and ham (non-spam).
15     // Each line has text from one email.
16     val spam = sc.textFile("files/spam.txt")
17     val ham = sc.textFile("files/ham.txt")
18 
19     // Create a HashingTF instance to map email text to vectors of 100 features.
20     val tf = new HashingTF(numFeatures = 100)
21     // Each email is split into words, and each word is mapped to one feature.
22     val spamFeatures = spam.map(email => tf.transform(email.split(" ")))
23     val hamFeatures = ham.map(email => tf.transform(email.split(" ")))
24 
25     // Create LabeledPoint datasets for positive (spam) and negative (ham) examples.
26     val positiveExamples = spamFeatures.map(features => LabeledPoint(1, features))
27     val negativeExamples = hamFeatures.map(features => LabeledPoint(0, features))
28     val trainingData = positiveExamples ++ negativeExamples
29     trainingData.cache() // Cache data since Logistic Regression is an iterative algorithm.
30 
31     // Create a Logistic Regression learner which uses the SGD.
32     val lrLearner = new LogisticRegressionWithSGD()
33     // Run the actual learning algorithm on the training data.
34     val model = lrLearner.run(trainingData)
35 
36     // Test on a positive example (spam) and a negative one (ham).
37     // First apply the same HashingTF feature transformation used on the training data.
38     val posTestExample = tf.transform("O M G GET cheap stuff by sending money to ...".split(" "))
39     val negTestExample = tf.transform("Hi Dad, I started studying Spark the other ...".split(" "))
40     // Now use the learned model to predict spam/ham for new emails.
41     println(s"Prediction for positive test example: ${model.predict(posTestExample)}")
42     println(s"Prediction for negative test example: ${model.predict(negTestExample)}")
43 
44     sc.stop()
45   }
46 }
```

spam.txt
```
Dear sir, I am a Prince in a far kingdom you have not heard of.  I want to send you money via wire transfer so please ...
Get Viagra real cheap!  Send money right away to ...
Oh my gosh you can be really strong too with these drugs found in the rainforest. Get them cheap right now ...
YOUR COMPUTER HAS BEEN INFECTED!  YOU MUST RESET YOUR PASSWORD.  Reply to this email with your password and SSN ...
THIS IS NOT A SCAM!  Send money and get access to awesome stuff really cheap and never have to ...
```

ham.txt

```
Dear Spark Learner, Thanks so much for attending the Spark Summit 2014!  Check out videos of talks from the summit at ...
Hi Mom, Apologies for being late about emailing and forgetting to send you the package.  I hope you and bro have been ...
Wow, hey Fred, just heard about the Spark petabyte sort.  I think we need to take time to try it out immediately ...
Hi Spark user list, This is my first question to this list, so thanks in advance for your help!  I tried running ...
Thanks Tom for your email.  I need to refer you to Alice for this one.  I haven't yet figured out that part either ...
Good job yesterday!  I was attending your talk, and really enjoyed it.  I want to try out GraphX ...
Summit demo got whoops from audience!  Had to let you know. --Joe
```

- **maven打包scala程序**

```
├── pom.xml
├── README.md
├── src
│   └── main
│       └── scala
│           └── com
│                   └── learningsparkexamples
│                           └── scala
│                               └── MLlib.scala
```

MLlib.scala 就是上面写的scala代码，pom.xml 是 maven 编译时候的 配置 文件：

```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" 
   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
   xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>my.demo</groupId>
    <artifactId>sparkdemo</artifactId>
    <version>1.0-SNAPSHOT</version>
    <properties>
  <!--编译时候 java版本 
  <maven.compiler.source>1.7</maven.compiler.source> 
  <maven.compiler.target>1.7</maven.compiler.target> 
  -->
  <encoding>UTF-8</encoding>
  <scala.tools.version>2.10</scala.tools.version>
  <!-- Put the Scala version of the cluster -->
  <scala.version>2.10.5</scala.version>
    </properties>
    <dependencies>
  <dependency> <!-- Spark dependency -->
      <groupId>org.apache.spark</groupId>
      <artifactId>spark-core_2.10</artifactId>
      <version>1.6.1</version>
      <scope>provided</scope>
  </dependency>
  <dependency> <!-- Spark dependency -->
      <groupId>org.apache.spark</groupId>
      <artifactId>spark-mllib_2.10</artifactId>
      <version>1.6.1</version>
      <scope>provided</scope>
  </dependency>
  <dependency>
      <groupId>org.scala-lang</groupId>
      <artifactId>scala-library</artifactId>
      <version>2.10.5</version>
  </dependency>
    </dependencies>
    <build>
  <pluginManagement>
      <plugins>
    <plugin>
        <!--用来编译scala的-->
        <groupId>net.alchim31.maven</groupId>
        <artifactId>
        scala-maven-plugin</artifactId>
        <version>3.1.5</version>
    </plugin>
       </plugins>
  </pluginManagement>
  <plugins>
      <plugin>
    <groupId>net.alchim31.maven</groupId>
    <artifactId>scala-maven-plugin</artifactId>
    <executions>
        <execution>
      <id>scala-compile-first</id>
      <phase>process-resources</phase>
  <goals>
          <goal>add-source</goal>
          <goal>compile</goal>
      </goals>
        </execution>
        <execution>
      <id>scala-test-compile</id>
      <phase>
       process-test-resources</phase>
      <goals>
          <goal>testCompile</goal>
      </goals>
        </execution>
    </executions>
      </plugin>
      </plugins>
    </build>
</project>
```




其中：

```
import org.apache.spark.{SparkConf, SparkContext}
```

所需要的依赖包配置是：

```
```
<dependency> <!-- Spark dependency -->
      <groupId>org.apache.spark</groupId>
      <artifactId>spark-core_2.10</artifactId>
      <version>1.6.1</version>
      <scope>provided</scope>
  </dependency>
```
```

```
import org.apache.spark.mllib.classification.LogisticRegressionWithSGD
import org.apache.spark.mllib.feature.HashingTF
import org.apache.spark.mllib.regression.LabeledPoint
```

所需要的依赖包配置是：

```
```
<dependency> <!-- Spark dependency -->
      <groupId>org.apache.spark</groupId>
      <artifactId>spark-mllib_2.10</artifactId>
      <version>1.6.1</version>
      <scope>provided</scope>
  </dependency>
```
```


配置的时候要注意spark 和 scala 的版本，可以打开spark-shell 观察：

![](http://images2015.cnblogs.com/blog/709693/201606/709693-20160603140340930-307314451.png)

配置完成后，在pom.xml 所在的目录运行命令：

```
mvn clean && mvn compile && mvn package
```

如果mvn 下载 有问题，可以参考这篇博文：http://www.cnblogs.com/xiaoyesoso/p/5489822.html 的 3. Bulid GitHub Spark Runnable Distribution

- spark运行项目

mvn编译打包完成后会pom.xml所在目录下出现一个target文件夹：

```
├── target
│   ├── classes
│   │   └── com
│   │       └── oreilly
│   │           └── learningsparkexamples
│   │               └── scala
│   │                   ├── MLlib$$anonfun$1.class
│   │                   ├── MLlib$$anonfun$2.class
│   │                   ├── MLlib$$anonfun$3.class
│   │                   ├── MLlib$$anonfun$4.class
│   │                   ├── MLlib.class
│   │                   └── MLlib$.class
│   ├── classes.-475058802.timestamp
│   ├── maven-archiver
│   │   └── pom.properties
│   ├── maven-status
│   │   └── maven-compiler-plugin
│   │       └── compile
│   │           └── default-compile
│   │               ├── createdFiles.lst
│   │               └── inputFiles.lst
│   └── sparkdemo-1.0-SNAPSHOT.jar
```

最后 运行命令，提交执行任务（注意两个test文件所对应的位置）：

```
${SPARK_HOME}/bin/spark-submit --class ${package.name}.${class.name} ${PROJECT_HOME}/target/*.jar
```

运行结果：

```
caizhenwei@caizhenwei-Inspiron-3847:~/桌面/learning-spark$ vim mini-complete-example/src/main/scala/com/oreilly/learningsparkexamples/mini/scala/MLlib.scala caizhenwei@caizhenwei-Inspiron-3847:~/桌面/learning-spark$ ../bin-spark-1.6.1/bin/spark-submit --class com.oreilly.learningsparkexamples.scala.MLlib ./mini-complete-example/target/sparkdemo-1.0-SNAPSHOT.jar 
16/06/03 13:23:23 WARN NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable
16/06/03 13:23:23 WARN Utils: Your hostname, caizhenwei-Inspiron-3847 resolves to a loopback address: 127.0.1.1; using 172.16.111.93 instead (on interface eth0)
16/06/03 13:23:23 WARN Utils: Set SPARK_LOCAL_IP if you need to bind to another address
16/06/03 13:23:24 WARN Utils: Service 'SparkUI' could not bind on port 4040. Attempting port 4041.
16/06/03 13:23:26 WARN BLAS: Failed to load implementation from: com.github.fommil.netlib.NativeSystemBLAS
16/06/03 13:23:26 WARN BLAS: Failed to load implementation from: com.github.fommil.netlib.NativeRefBLAS
Prediction for positive test example: 1.0
Prediction for negative test example: 0.0
```

![](https://img-blog.csdn.net/20170509141535695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

