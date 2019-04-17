# WFDB库命令整理 - 工作笔记 - CSDN博客





2012年01月16日 14:35:17[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6935标签：[matlab																[file																[存储																[网络																[服务器																[磁盘](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)





**wfdbcat**

**wfdbcat***file *[ *file* ... ] 

找到WFDB库中的任何文件，然后复制将其标准输出。如果连接同一个版本的WFDB库已经被支持的网络文件编译过，wfdbcat尤其有用于从遥远的网络服务器 FTP等上面检索文件。
  for R in `wfdbcat mitdb/RECORDS`
  do
    echo -n "Record mitdb/$R ..."
    sigamp -r mitdb/$R -a atr -p
  done



**sigamp**
**sigamp -r***[record](http://physionet.org/physiotools/wag/intro.htm#record)* [*options* ... ]

测量指定记录文件的所有信号的经过基线校准的幅度的均方根，或者（适当的ECG信号的注释文件）正常QRS正负峰值差。它能达到个信号300个测量结果，并且计算出标准值（通过舍弃测量结果中最大最小值5%，取剩下的90%）。

**rdsamp**

**rdsamp -r***[record](http://physionet.org/physiotools/wag/intro.htm#record)* [*options* ... ]


随意读指定记录文件，并以十进制标准输出。无参，则rdsamp默认开始为0时刻，输出所有样品。默认情况下，每一行输出包含样品数量和每个导联的样品。起始为0导联起点

**wfdb2mat**

**wfdb2mat -i***[record](http://physionet.org/physiotools/wag/intro.htm#record)* [*options* ... ]


讲任何一种格式的信号转换成一条任何版本的Matlab都可以用的.mat记录文件和一个包含这个文件各种信息的头文件。另外，wfdb2mat同时输出一个这些信息的简短摘要。

**wrsamp** [ *options* ... ] [ *column* ... ] 

wrsamp读取文本格式的输入文件，然后将指定条目以‘16’存储格式保存在标准输出中，或者磁盘文件中。如果不指定，则全部都存储

**wfdbwhich**

找出任何WFDB文件的路径，并输出

**rdann**

输出ASCII格式的注释文件

rr2ann

从一个RR间期的文本文件生成一个注释文件](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)




