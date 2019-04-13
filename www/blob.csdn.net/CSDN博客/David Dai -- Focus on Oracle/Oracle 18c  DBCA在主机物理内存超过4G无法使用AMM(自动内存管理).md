
# Oracle 18c  DBCA在主机物理内存超过4G无法使用AMM(自动内存管理) - David Dai -- Focus on Oracle - CSDN博客


2018年07月26日 09:56:09[Dave](https://me.csdn.net/tianlesoftware)阅读数：959


*原文链接：**[https://www.cndba.cn/Expect-le/article/2913](https://www.cndba.cn/Expect-le/article/2913)*
###### 1. 错误说明
在DBCA创建数据库时，选择AMM时出现报错：
> [INS-35178]The Automatic Memory Management option is not allowed when the total physical memory is greater than 4GB.
![https://www.cndba.cn](https://www.cndba.cn:8080/cndba/20180724/jpg/1532426952822.jpg)
###### 2.原因
对此MOS文档做了说明
> 12.2:[INS-35178]The Automatic Memory Management Option Is Not Allowed During RunInstaller or Using DBCA If Physical Memory Is Greater Than 4G. (文档 ID 2244817.1)
在12.2之前版本中也会出现当主机内存超过4G时，会出现告警提示不建议使用AMM。但是从12.2开始，当主机内存大于4G就直接报错无法继续安装数据库了。对此Oracle解释了具体原因：Because database creation fails some times and in some cases database wont be functional after some times .大概意思就是如果数据库创建失败几次后，可能会出现数据库创建后无法正常运行。
###### 3.  解决办法
就是使用ASMM(自动共享内存管理)。

