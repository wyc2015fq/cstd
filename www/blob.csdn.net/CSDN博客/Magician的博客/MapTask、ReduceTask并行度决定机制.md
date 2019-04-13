
# MapTask、ReduceTask并行度决定机制 - Magician的博客 - CSDN博客


2018年03月28日 16:03:47[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：630所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言:](#前言)
[1、mapTask并行度的决定机制](#1maptask并行度的决定机制)
[2、ReduceTask并行度的决定](#2reducetask并行度的决定)
[总结：](#总结)


# 目录
## 前言:
MapTask的并行度决定map阶段的任务处理并发度，进而影响到整个job的处理速度。那么，MapTask并行实例是否越多越好呢？其并行度又是如何决定呢？
## 1、mapTask并行度的决定机制
一个job的map阶段并行度由客户端在提交job时决定，而客户端对map阶段并行度的规划的基本逻辑为：
将待处理数据执行逻辑切片（即按照一个特定切片大小，将待处理数据划分成逻辑上的多个split），然后每一个split分配一个mapTask并行实例处理。(注意切片和分块的区别)
这段逻辑及形成的切片规划描述文件，由FileInputFormat实现类的getSplits()方法完成，其过程如下图：
![这里写图片描述](https://img-blog.csdn.net/20180328155235376?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**FileInputFormat切片机制**
1、切片定义在InputFormat类中的getSplit()方法
2、FileInputFormat中默认的切片机制：
a)         简单地按照文件的内容长度进行切片
b)         切片大小，默认等于block大小
c)         切片时不考虑数据集整体，而是逐个针对每一个文件单独切片
比如待处理数据有两个文件：
file1.txt 320M
file2.txt 10M
经过FileInputFormat的切片机制运算后，形成的切片信息如下：
file1.txt.split1– 0~128
file1.txt.split2– 128~256
file1.txt.split3– 256~320
file2.txt.split1– 0~10M
**FileInputFormat中切片的大小的参数配置**（了解）
通过分析源码，在FileInputFormat中，计算切片大小的逻辑：Math.max(minSize, Math.min(maxSize, blockSize));  切片主要由这几个值来运算决定
minsize：默认值：1    配置参数： mapreduce.input.fileinputformat.split.minsize
maxsize：默认值：Long.MAXValue    配置参数：mapreduce.input.fileinputformat.split.maxsize
blocksize
因此，默认情况下，切片大小=blocksize
maxsize（切片最大值）：
参数如果调得比blocksize小，则会让切片变小，而且就等于配置的这个参数的值
minsize （切片最小值）：
参数调的比blockSize大，则可以让切片变得比blocksize还大
**选择并发数的影响因素：**
1、运算节点的硬件配置
2、运算任务的类型：CPU密集型还是IO密集型
3、运算任务的数据量
## 2、ReduceTask并行度的决定
reducetask的并行度同样影响整个job的执行并发度和执行效率，但与maptask的并发数由切片数决定不同，**Reducetask数量的决定是可以直接手动设置**：
//默认值是1，手动设置为4
job.setNumReduceTasks(4);
如果数据分布不均匀，就有可能在reduce阶段产生数据倾斜
注意： reducetask数量并不是任意设置，还要考虑业务逻辑需求，有些情况下，需要计算全局汇总结果，就只能有1个reducetask
尽量不要运行太多的reduce task。对大多数job来说，最好reduce的个数最多和集群中的reduce持平，或者比集群的 reduce slots小。这个对于小集群而言，尤其重要。
## 总结：
MapTask并行度由两种情况：
1、当一个文件比较大时，每次切片128M为一个MapTask的任务量。被切分多少片就能由多少个MapTask。
2、当有多个小文件时，每个小文件可以看做是一个MapTask的任务量，有多少个小文件就有多少个MapTask。
PS：当然默认切片的大小可以自定义设置。
ReduceTask并行度可以在代码中手动设置。

