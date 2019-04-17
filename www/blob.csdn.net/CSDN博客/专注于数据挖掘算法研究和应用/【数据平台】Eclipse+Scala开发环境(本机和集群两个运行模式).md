# 【数据平台】Eclipse+Scala开发环境(本机和集群两个运行模式) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月11日 16:07:21[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：214








1）环境：

集群部署centos+jdk1.8+spark-1.5.0+Scala-2.11.8；

开发端windows+Eclipse Mars.1 Release (4.5.1)

2）安装eclipse-scala-plugin插件

[http://scala-ide.org/download/prev-stable.html](http://scala-ide.org/download/prev-stable.html)下载版本：

![](https://img-blog.csdn.net/20170911160627248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




解压后将features和plugins中的文件放到eclipse对应的目录中。

3）引入SparkJar包，工程示例如下：

![](https://img-blog.csdn.net/20170911160638642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




加入后Run时提示找不到类，不知道是否是对应的版本问题，在工程属性上调整：

![](https://img-blog.csdn.net/20170911160647457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




4） 单词计数本机运行：新建Scala Object

![](https://img-blog.csdn.net/20170911160657444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5）本机开发提交集群运行



![](https://img-blog.csdn.net/20170911160708916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



