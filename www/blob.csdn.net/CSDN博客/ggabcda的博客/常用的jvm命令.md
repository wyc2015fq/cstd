# 常用的jvm命令 - ggabcda的博客 - CSDN博客





2017年08月23日 14:05:14[ggabcda](https://me.csdn.net/ggabcda)阅读数：319








（1）jps -lv

列出所有java进程

第一个参数是pid；

第二个为jvm参数和完整的执行类名

（2）jinfo <pid>

描述：输出给定 java 进程所有的配置信息。包括 java 系统属性和 jvm 命令行标记等。

（3）jstack <pid>

为指定的线程输出 java 的线程堆栈信息，包括了进程里的所有线程。每一个线程 frame ，包括类全名，方法名，代码行。

（4）jstat -gcutil <pid> <interval>

查看java进程的gc情况

以百分比显示每个区域的内存使用情况；

参数interval表示每多少毫秒刷新一次

（5）jmap -head <pid>

查看java进程的对堆信息

（6）jmap -dump:[live,] format=b,file=<filename> <pid>

生成heap dump文件，live为可选参数，表示先做一次full gc


```java
C:\Users\baimq>jmap -dump:live,format=b,file=d:heap2.bin 6660
Dumping heap to D:\heap2.bin ...
Heap dump file created
```


 （7）jmap -histo[:live] <pid>

打印每个class的实例数目，内存占用，类全名信息，可用于强制full gc。

ps:这个打印出来的有点多。。。可以加上more或者grep，用来查找大内存对象


```java
C:\Users\baimq>jmap -histo:live 6660|more

 num     #instances         #bytes  class name
----------------------------------------------
   1:         42124        6946688  [C
   2:          9489        2092888  [I
   3:          7314         698064  java.lang.Class
   4:         40802         652832  java.lang.String
   5:          6432         566016  java.lang.reflect.Method
   6:         17779         426696  java.util.concurrent.ConcurrentHashMap$Node
   7:          7432         416192  java.util.LinkedHashMap
   8:         12889         412448  java.util.LinkedHashMap$Entry
   9:          5833         400296  [Ljava.util.HashMap$Node;
  10:          6651         334752  [Ljava.lang.Object;
  11:          9282         222768  java.util.HashMap$Node
  12:          1235         208128  [B
  13:           158         179024  [Ljava.util.concurrent.ConcurrentHashMap$Nod
e;
```




（8）dump文件分析工具

使用VisualVM或者memory analyzer tool可以分析





