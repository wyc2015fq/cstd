# Java深入 - Java虚拟机性能问题监控和排查 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年06月15日 21:34:08[initphp](https://me.csdn.net/initphp)阅读数：1833
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









#### 1. jps 查看Java进程ID



```java
$ jps
9788 Jps
7504 Bootstrap
3820 org.eclipse.equinox.launcher_1.3.0.v20120522-1813.jar
```




#### 2. jstat 查看Java状态

查看Java gc状态




```java
$sudo jstat -gc 18963
 S0C    S1C    S0U    S1U      EC       EU        OC         OU       PC     PU    YGC     YGCT    FGC    FGCT     GCT   
218432.0 218432.0  0.0   3648.1 2184576.0 1130415.9 1572864.0   62074.9   154388.0 103149.4    127    3.047   2      0.623    3.670
```



S0C：年轻代中第一个survivor（幸存区）的容量 (字节)

S1C：年轻代中第二个survivor（幸存区）的容量 (字节)

S0U ：年轻代中第一个survivor（幸存区）目前已使用空间 (字节)

S1U：年轻代中第二个survivor（幸存区）目前已使用空间 (字节)

EC ：年轻代中Eden（伊甸园）的容量 (字节)

EU ：年轻代中Eden（伊甸园）目前已使用空间 (字节)

OC ：Old代的容量 (字节)

OU ：Old代目前已使用空间 (字节)

PC：Perm(持久代)的容量 (字节)

PU：Perm(持久代)目前已使用空间 (字节)

YGC  ：从应用程序启动到采样时年轻代中gc次数

YGCT ：从应用程序启动到采样时年轻代中gc所用时间(s)

FGC ：从应用程序启动到采样时old代(全gc)gc次数

FGCT ：从应用程序启动到采样时old代(全gc)gc所用时间(s)

GCT：从应用程序启动到采样时gc用的总时间(s)




查看Java 内存空间使用情况，主要查看交换区、Eden、老生代、常量池的使用百分比



```java
sudo jstat -gcutil 18963
  S0     S1     E      O      P     YGC     YGCT    FGC    FGCT     GCT   
  1.45   0.00  86.09   4.00  66.82    168    3.650     2    0.623    4.273
```


查看加载的class的数量



```java
$sudo jstat -class 18963
Loaded  Bytes  Unloaded  Bytes     Time   
 16850 33388.9        0     0.0      26.21
```


查看编译的数量





```java
$sudo jstat -compiler 18963
Compiled Failed Invalid   Time   FailedType FailedMethod
    5857      0       0    92.43          0
```




#### 3. jinfo 打印java进程的配置信息



```java
sudo jinfo 18963
```



#### 4. jmap 生成虚拟机的内存快照



```java
sudo jmap 18963
```




#### 5. jstack 显示虚拟机的线程快照



```java
jstack 14298     #查看线程 -F 强制查看
top -p 14292 -H    查看线程占用CPU
sudo jstack -F 4284 | grep -A 10 5017  #查看进程中某一个线程，显示10行
```












