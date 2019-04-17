# Apache Hadoop FS命令示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月11日 15:40:21[boonya](https://me.csdn.net/boonya)阅读数：9449








原文来自：[https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-fs-commands-example/](https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-fs-commands-example/)

==========本文采用谷歌翻译，请参照中英文学习===========


在本示例中，我们将介绍您可能需要知道的最重要的命令来处理Hadoop文件系统（FS）。

我们假设以前知道Hadoop是什么以及Hadoop可以做什么？它如何以分布式方式工作，Hadoop分布式文件系统（HDFS）是什么？这样我们可以继续检查一些如何处理Hadoop文件系统的例子，以及什么是最重要的命令。以下是两个例子，如果你不清楚Apache Hadoop可以帮助你：
- [Hadoop“Hello World”示例](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/hadoop-hello-world-example/)
- [如何使用虚拟机设置Hadoop集群](http://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-cluster-setup-example-virtual-machines/)


让我们开始，如在这个例子中所说，我们将看到顶部和最常用的Hadoop文件系统（fs）命令，这将有助于管理HDFS集群中的文件和数据。

# 1.介绍

Hadoop文件系统（FS）默认提供了各种shell命令，可用于与Hadoop分布式文件系统（HDFS）或任何其他使用Hadoop Shell的支持文件系统进行交互。一些最常用的命令是曾经用于诸如创建目录，复制文件，查看文件内容，更改文件的所有权或权限等操作。


# 2.公共命令

在本节中，我们将看到最常见的Hadoop FS命令的用法和示例。


## 2.1创建目录

用法：



```java
hadoop fs -mkdir <paths>
```


示例：



```java
hadoop fs -mkdir /user/root/dir1
```
第二行中的命令用于列出特定路径的内容。我们将在下一小节中看到此命令。我们可以在截图中看到dir1被创建
![](https://img-blog.csdn.net/20170211150927813)

使用单个命令创建多个目录



```java
hadoop fs -mkdir /user/root/dir1 /user/root/dir2
```

如上例所示，要一次创建多个目录，只需传递多个路径和目录名称，以空格分隔。
![](https://img-blog.csdn.net/20170211151126419)

## 2.2列出目录的内容

用法：



```java
hadoop fs -ls <paths>
```


示例：



```java
hadoop fs -ls /user/root/
```

该命令类似于unix shell的ls命令。
![](https://img-blog.csdn.net/20170211151322534)


## 2.3在HDFS中上传文件

命令用于将一个或多个文件从本地系统复制到Hadoop文件系统。

用法：



```java
hadoop fs -put <local_files> ... <hdfs_path>
```


示例：



```java
hadoop fs -put Desktop/testfile.txt /user/root/dir1/
```

在下面的屏幕截图中，我们将文件testfile.txt从本地文件系统的Desktop放置到目标  / user / root / dir1下的Hadoop文件系统
![](https://img-blog.csdn.net/20170211151558348)


## 2.4从HDFS下载文件

将文件从HDFS下载到本地文件系统。

用法：



```java
hadoop fs -get <hdfs_paths> <local_path>
```


示例：



```java
hadoop fs -get /user/root/dir1/testfile.txt Downloads/
```

与put命令一样，get命令从Hadoop文件系统获取或下载文件到Downloads文件夹中的本地文件系统。
![](https://img-blog.csdn.net/20170211151835257)


## 2.5查看文件内容

为了查看文件的内容，cat命令在Hadoop文件系统中可用。它再次类似于unix shell中可用的。

以下是上载到Hadoop文件系统的文件的内容，位于路径/ user / root / dir1 /上面的步骤中。

![](https://img-blog.csdn.net/20170211151945133)


用法：



```java
hadoop fs -cat <paths>
```


示例：



```java
hadoop fs -cat /user/root/dir1/testfile.txt
```

我们可以看到下面截图中显示的内容与testfile.txt中的内容相同
![](https://img-blog.csdn.net/20170211152125820)

## 2.6复制文件

在Hadoop文件系统中将文件从一个地方复制到另一个地方与unix shell中的cp命令语法相同。

用法：



```java
hadoop fs -cp <source_path> ... <destination_path>
```


示例：



```java
hadoop fs -cp /user/root/dir1/testfile.txt /user/root/dir2
```

在将文件从源复制到目标时，我们还可以在源中提供多个文件

![](https://img-blog.csdn.net/20170211152335993)


## 2.7将文件从源移动到目标

以下是在Hadoop文件系统中将文件从一个目录移动到另一个目录的语法和示例。

用法：



```java
hadoop fs -mv <source_path> <destination_path>
```


示例：



```java
hadoop fs -mv /user/root/dir1/testfile.txt /user/root/dir2
```

![](https://img-blog.csdn.net/20170211152514999)

## 2.8从HDFS中删除文件或目录

从Hadoop文件系统中删除文件或目录与unix shell类似。它还有两个选项，-rm和-rm -r

用法：



```java
hadoop fs -rm <path>
```


示例：



```java
hadoop fs -rm /user/root/dir2/testfile.txt
```

上述命令只会删除特定文件或目录下的文件，只有当它是空的。
但是如果我们要删除一个包含其他文件的目录，我们还有一个递归版本的remove命令。

![](https://img-blog.csdn.net/20170211152659517)


在情况下，我们要删除一个包含文件的目录，-rm将无法删除该目录。在这种情况下，我们可以使用recursive选项从目录中删除所有的文件，当它是空的删除目录。下面是递归操作的示例：

用法：




```java
hadoop fs -rm -r <path>
```


示例：



```java
hadoop fs -rm -r /user/root/dir2
```

![](https://img-blog.csdn.net/20170211152838386)

## 2.9显示文件的尾部

该命令与unix tail命令完全相似。

用法：



```java
hadoop fs -tail <path>
```


示例：



```java
hadoop fs -tail /user/root/dir1/testfile.txt
```

![](https://img-blog.csdn.net/20170211153008663)

## 2.10显示特定文件的总长度

为了检查文件中内容的总长度，我们可以使用-du。命令如下。如果路径是文件的路径，则显示文件的长度，如果它是目录的路径，则显示的内容的聚合大小显示为包括所有文件和目录。

用法：



```java
hadoop fs -du <path>
```


示例：



```java
hadoop fs -du /user/root/dir1/testfile.txt
```

![](https://img-blog.csdn.net/20170211153202869)

## 2.11计数目录和文件

此命令用于计算指定路径下的文件和目录数。如下面的截图所示，输出显示目录数，即2，文件数，即1，总内容大小为159字节，以及这些统计信息所属的路径。

用法：



```java
hadoop fs -count <path>
```
示例：



```java
hadoop fs -count /user/root/
```

![](https://img-blog.csdn.net/20170211153356794)

## 2.12文件系统中的空间的详细信息

要获取Hadoop文件系统的所有空间相关详细信息，我们可以使用df命令。它提供有关当前安装的文件系统使用的空间量和可用空间量的信息


用法：



```java
hadoop fs -df <path>
```

命令可以在没有路径URI或路径URI的情况下使用，当不使用路径URI时，它提供关于整个文件系统的信息。当提供路径URI id时，它提供特定于路径的信息。

示例：



```java
hadoop fs -df
hadoop fs -df /user/root
```

以下屏幕截图显示文件系统，文件系统大小，已用空间，可用空间和已使用百分比。

![](https://img-blog.csdn.net/20170211153628061)


# 3.结论

这使我们得出了这个例子的结论。这些Hadoop文件系统命令将帮助您开始处理Hadoop生态系统中的文件和目录。







