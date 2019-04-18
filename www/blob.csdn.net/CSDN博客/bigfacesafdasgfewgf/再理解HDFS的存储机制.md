# 再理解HDFS的存储机制 - bigfacesafdasgfewgf - CSDN博客





2014年12月15日 09:03:55[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2525








**再理解HDFS的存储机制**



1. HDFS开创性地设计出一套文件存储方式，即对文件分割后分别存放；




2. HDFS将要存储的大文件进行分割，分割后存放在既定的存储块（Block）中，并通过预先设定的优化处理，模式对存储的数据进行预处理，从而解决了大文件储存与计算的需求；



3. 一个HDFS集群包括两大部分，即NameNode与DataNode。一般来说，**一个集群中会有一个NameNode和多个DataNode共同工作；**



4. **NameNode是集群的主服务器**，主要是用于对HDFS中所有的文件及内容数据进行维护，并不断读取记录集群中DataNode主机情况与工作状态，并通过读取与写入镜像日志文件的方式进行存储；



5. **DataNode在HDFS集群中担任任务具体执行角色**，是集群的工作节点。文件被分成若干个相同大小的数据块，分别存储在若干个DataNode上，DataNode会定期向集群内NameNode发送自己的运行状态与存储内容，并根据NameNode发送的指令进行工作；



6. NameNode负责接受客户端发送过来的信息，然后将文件存储位置信息发送给提交请求的客户端，由客户端直接与DataNode进行联系，从而进行部分文件的运算与操作。


**7. Block是HDFS的基本存储单元，默认大小是64M；**



8. HDFS还可以对已经存储的Block进行多副本备份，将**每个Block至少复制到3个相互独立的硬件上，这样可以快速恢复损坏的数据；**



9. 用户可以使用既定的API接口对HDFS中的文件进行操作；


**10. 当客户端的读取操作发生错误的时候，客户端会向NameNode报告错误，并请求NameNode排除错误的DataNode后后重新根据距离排序，从而获得一个新的DataNode的读取路径。如果所有的DataNode都报告读取失败，那么整个任务就读取失败；**

**11. 对于写出操作过程中出现的问题，FSDataOutputStream并不会立即关闭。客户端向NameNode报告错误信息，并直接向提供备份的DataNode中写入数据。备份DataNode被升级为首选DataNode，并在其余2个DataNode中备份复制数据。NameNode对错误的DataNode进行标记以便后续对其进行处理。**








