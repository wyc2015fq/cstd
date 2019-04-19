# flink learn ：1 flink mac 安装及启动 - BlueSky - CSDN博客
2019年01月28日 20:17:42[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：46
个人分类：[flink](https://blog.csdn.net/ASIA_kobe/article/category/8647494)
## 1、安装
一键安装
```bash
$ brew install apache-flink
...
$ flink --version
Version: 1.2.0, Commit ID: 1c659cf
```
查看flink安装位置：
```bash
~ brew info apache-flink
apache-flink: stable 1.7.1, HEAD
Scalable batch and stream data processing
https://flink.apache.org/
/usr/local/Cellar/apache-flink/1.7.0 (158 files, 308.2MB) *
```
快速启动本地flink，一条命令搞定。
```bash
libexec/bin/start-cluster.sh
```
![](https://img-blog.csdnimg.cn/20190128200151130.png)
访问[http://localhost:8081](http://localhost:8081) 可查看相应的集群情况。
![](https://img-blog.csdnimg.cn/20190128201708773.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FTSUFfa29iZQ==,size_16,color_FFFFFF,t_70)
至此，安装启动成功。
## 2、运行Demo
下面运行下flink提供的一个demo，直接感受下实时计算。SocketWindowWordCount 为监听本地端口，通过滑动时间窗口为5s，滑动时间间隔为1s，去实时监听端口内容，并计算输出至本地。详细程序见[scala版](https://github.com/apache/flink/blob/master/flink-examples/flink-examples-streaming/src/main/scala/org/apache/flink/streaming/scala/examples/socket/SocketWindowWordCount.scala)或[java版](https://github.com/apache/flink/blob/master/flink-examples/flink-examples-streaming/src/main/java/org/apache/flink/streaming/examples/socket/SocketWindowWordCount.java)。
```bash
# 开启监听端口
nc -l 9000
# 提交flink作业
./bin/flink run examples/streaming/SocketWindowWordCount.jar --port 9000
```
通过flink web页面，可以查看相应的flink作业运行情况。
![](https://img-blog.csdnimg.cn/20190128205054856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FTSUFfa29iZQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190128205112540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FTSUFfa29iZQ==,size_16,color_FFFFFF,t_70)
在开启的端口输入字符，flink会进行统计并输出至本地文件：flink-*-.out
![](https://img-blog.csdnimg.cn/20190128205406806.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FTSUFfa29iZQ==,size_16,color_FFFFFF,t_70)
至此，初步体验Done！
