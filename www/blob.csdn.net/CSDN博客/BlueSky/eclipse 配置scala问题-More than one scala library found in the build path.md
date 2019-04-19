# eclipse 配置scala问题-More than one scala library found in the build path - BlueSky - CSDN博客
2015年11月16日 18:55:36[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：11973
## 配置eclipse出错
按照这篇博客[http://dongxicheng.org/framework-on-yarn/spark-eclipse-ide/](http://dongxicheng.org/framework-on-yarn/spark-eclipse-ide/)安装scalaIDE插件，但是并没有成功，当引入如下spark-assembly-1.5.1-hadoop2.6.0.jar（对应我自己的spark下的jar包）会报错，提示这个jar包与源环境中的jar包冲突（提示scala环境冲突）—-未解决！！！！ 
错误提示如下： 
```
Description    Resource    Path    Location    Type 
More than one scala library found in the build path (/home/hadoop/eclipse/plugins/org.scala-lang.scala-library_2.11.7.v20150622-112736-1fbce4612c.jar, /usr/local/spark/spark-1.5.1-bin-hadoop2.6/lib/spark-assembly-1.5.1-hadoop2.6.0.jar).At least one has an incompatible version. Please update the project build path so it contains only one compatible scala library.    hello-test      Unknown Scala Classpath Problem
```
## 配置eclipse出错解决
山重水复疑无路，柳暗花明又一村！经过大量的验证。。。终于make it。 
参考博客：[http://blog.csdn.net/wankunde/article/details/41843217](http://blog.csdn.net/wankunde/article/details/41843217)
其中四个步骤： 
Scala 工程版本 
该方法和Java 工程类似。
新建scala工程 
1.在工程中移除自带的scala版本库 
1.添加spark 库spark-assembly-1.1.0-cdh5.2.0-hadoop2.5.0-cdh5.2.0.jar 
3.修改工程中的scala编译版本 
4.右击 –> Scala –> set the Scala Installation
也可以
右击工程–> Properties –> Scala Compiler –> Use project Setting 中选择spark对应的scala版本，此处选择Lastest2.10 bundle
编写scala代码 
export jar 包 
**对于第一个步骤没什么用或者自己没找到怎么去移除；对于第四部选择scala版本这个还是解决问题的最终的方法，选择Scala library container->Classpath Container中选择Fixed..2.10.5；the error就解决了**
**最后又发现真正的问题是我自己spark文件夹下的jar包scala是2.10.4而下的eclipse ide是2.17.0所以上面的解决办法才奏效。**
## 安装sbt出错
按照sbt官网教程安装sbt，链接[http://www.scala-sbt.org/0.13/tutorial/zh-cn/Installing-sbt-on-Linux.html](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Installing-sbt-on-Linux.html)，安装完毕后，以为一切都OK了，但是出现
```
Getting org.scala-sbt sbt 0.13.9 ...
You probably access the destination server through a proxy server that is not well configured.
```
又是不成功。。。去查找资料看到说法是：**网速，DNS设置！！**
stackflow上有个答案说是在.bashrc中添加：
```bash
export JAVA_OPTS="$JAVA_OPTS -Dhttp.proxyHost=proxy -Dhttp.proxyPort=port -Dhttps.proxyHost=proxy -Dhttps.proxyPort=port"
```
然而，试了下仍无效，然而在之前在ubuntu下配置是正确的。可能也可系统有关。。。
哎，两个问题都没有得到解决。。。 
The road is too long for me！！！
## 解决
**第一个eclipse问题**是插件与自己spark中的scala版本不对，spark中对应的是2.10.5，这个具体怎么看出来的，可以在eclipse中先删除scala container然后添加spark 中的jar，提示错误信息就会看到了。。。 
**第二个问题**：竟然是网速（或者是网络dns设置）问题！！！回来再高能所这边虽然下了一会但还是make it！ 
目前sbt的安装需要联网，离线好像不能安装，因为在第一次sbt -version时它要初始化并会自动联网下载好多jar包。
总的来说，还算是把问题解决了，虽然进度有点慢，也有点不顺各种问题，但还算有点结果。继续 go on！
明天把spark应用程序编译，跑通。scala语言学习！
