# 大数据学习笔记(十四)-- hadoop介绍 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月29日 11:40:30[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：305


**Table of Contents**

[Hadoop简介](#Hadoop%E7%AE%80%E4%BB%8B%C2%A0)

[HDFS](#HDFS)

[存储模型](#%E5%AD%98%E5%82%A8%E6%A8%A1%E5%9E%8B)

[架构模型](#%E6%9E%B6%E6%9E%84%E6%A8%A1%E5%9E%8B%C2%A0)

[NameNode](#NameNode)

[DataNode](#DataNode%C2%A0)

[HDFS的优缺点](#HDFS%E7%9A%84%E4%BC%98%E7%BC%BA%E7%82%B9)

[副本布置](#%E5%89%AF%E6%9C%AC%E5%B8%83%E7%BD%AE)

[HDFS读写流程](#HDFS%E8%AF%BB%E5%86%99%E6%B5%81%E7%A8%8B)

[总结](#%E6%80%BB%E7%BB%93)

[​](#%E2%80%8B)

## Hadoop简介 

# ![](https://img-blog.csdn.net/20180927132711872?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## HDFS

### 存储模型

![](https://img-blog.csdn.net/20180927141324606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**解释 **： 已上传的Block大小不可改变的原因是每个Block大小一致，改变一个其他也会跟着改变，并且改变一个其它的偏移量也会改变，这是一个很消耗cpu的过程，Hadoop的目的就是节省出存储模型对cpu的消耗，从而更多用于文件计算上面。

至于可以追加数据的原因是 追加会导致 原本文件增加，又由于每块大小已经定了下来，所以只需增加一个或多个block即可解决。

其中默认的 Block 大小为 128MB ， 副本数为3 ，其中每个副本分别在不同的计算机上，每台计算机上不允许出现相同的副本。

### 架构模型 

![](https://img-blog.csdn.net/20180927143457169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

看下图更好理解

![](https://img-blog.csdn.net/2018092714361965?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

** 解释** :  我们原本有一个很大的文件 ， 然后将它分成很多部分，每一个部分被称为一个块，每一个块又存储在不同的计算机上，这些存储块的计算机就被称为DataNode，此时还有一台计算机并没有存储块，而是存储了哪个块存储在了哪台计算机上面，这个被称为NameNode。当客户端存储数据或者获取数据时都会先去访问 NameNode ，获得 将数据存储到哪台计算机或 从哪台计算机上获取数据 的信息，这个信息被称为元数据，其实就是属性包括块的存储位置、偏移量等等。然后会根据这个信息。HDFSClient 会去找指定的 DataNode。

![](https://img-blog.csdn.net/20180927144758505?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### NameNode

![](https://img-blog.csdn.net/20180927151245752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可能有人会认为 不会和磁盘发生交换又怎么持久化呢？

运行时有 内存和磁盘的双向交互才叫交换。

持久化：运行时往磁盘写，down机以后再重新启动会从磁盘读回，从而恢复内存。

![](https://img-blog.csdn.net/2018092715413356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

当我们分配角色 在启动服务之前会有一次格式化，在这次格式化时，会第一次创建 名为fsimage的空目录，然后当开启服务以后，会分别从fsimage 和 edits 中读取内容， 显然由于第一次启动服务，这两个都是空目录。

当开启服务以后，NameNode会去读取fsimage和edits ， 读到内存当中进行合并，然后重新写入一个fsimage到磁盘当中。 

然后每隔指定的时间 会自动将 fsimage 和 edits 合并，并重新写入新的fsimage。这里 的 fsimage 与 edits 合并实际上是在另外一台机器上完成的，并不会影响 NameNode这台机器的工作。当 NameNode   down机后，再次开启服务又会重新 读取fsimage，恢复最近一次的 metadata信息，同时 读取edits文件执行 最近一次保存metadata与down机这段时间的用户进行的操作。 刚才说的在 另外一台机器上完成的 这台机器就是 **Secondary  NameNode**

### DataNode 

![](https://img-blog.csdn.net/20180927163604968?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图的 其上 含义 为：其它副本上 cp 一份到 其他DN

### HDFS的优缺点

![](https://img-blog.csdn.net/2018092716530579?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180927165929381?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### **副本布置**

官方推荐3个副本，场景是有多个机架，第一个副本在一个机架，第二个副本在另外一个机架上，第三个副本与第二个副本同机架，目的是预防整个机架瘫痪。官方说明是  第一个和第二个在同一个机架，第三个在另外一个机架，但源码与其描述不符。

### HDFS读写流程

![](https://img-blog.csdn.net/2018092910571098?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Pipeline : 上图中的 4 ，这条路线是一个管道，每一个block默认是128M，在传输的过程中会被切成更小的块64k，然后进行传输，很快一个64k的块就能够从FSData OutputStream 传递到 DataNode，此时会继续向下传递，创建副本，从而试第一个DataNode空闲出来，这时FSData OutputStream 又可以发送第二个64k的块。

第一个block传输完毕以后，DataNode会向namenode发送信息，告知自身的存储信息，之后client再上传其他block时就可以进行分配。

![](https://img-blog.csdn.net/20180929105728777?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 读取数据时，优先级为  本机datanode  > 本机架 datanode > 其他机架   ， 一般情况如果在其它机架都会经历3个交换机。

## **总结**

![](https://img-blog.csdn.net/20180929113158220?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### ![](https://img-blog.csdn.net/20180929113410877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180929113906109?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

