# java可视化监控工具 - z69183787的专栏 - CSDN博客
2017年09月25日 12:52:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1475
### 1.JConsole
> 
 JConsole工具在JDK/bin目录下，启动JConsole后，将自动搜索本机运行的jvm进程，不需要jps命令来查询指定。双击其中一个jvm进程即可开始监控，也可使用“远程进程”来连接远程服务器。
> 
![](https://img-my.csdn.net/uploads/201210/09/1349788757_5022.jpg)
> 
进入JConsole主界面，有“概述”、“内存”、“线程”、“类”、“VM摘要”和"Mbean"六个页签：
![](https://img-my.csdn.net/uploads/201210/09/1349788933_7362.jpg)
> 
内存页签相当于jstat命令，用于监视收集器管理的虚拟机内存(Java堆和永久代)变化趋势，还可在详细信息栏观察全部GC执行的时间及次数。
![](https://img-my.csdn.net/uploads/201210/09/1349789188_2247.jpg)
> 
线程页签
![](https://img-my.csdn.net/uploads/201210/09/1349789603_4029.jpg)
> 
最后一个常用页签，VM页签，可清楚的了解显示指定的JVM参数及堆信息。
![](https://img-my.csdn.net/uploads/201210/09/1349789749_1161.jpg)
### 2.VisualVM
> 
VisualVM是一个集成多个JDK命令行工具的可视化工具。VisualVM基于NetBeans平台开发，它具备了插件扩展功能的特性，通过插件的扩展，可用于显示虚拟机进程及进程的配置和环境信息(jps，jinfo)，监视应用程序的CPU、GC、堆、方法区及线程的信息(jstat、jstack)等。VisualVM在JDK/bin目录下。
安装插件： 工具- 插件
![](https://img-my.csdn.net/uploads/201210/09/1349791432_7616.jpg)
> 
VisualVM主界面
![](https://img-my.csdn.net/uploads/201210/09/1349791520_9359.jpg)
> 
在VisualVM中生成dump文件：
![](https://img-my.csdn.net/uploads/201210/09/1349791895_7348.jpg)
### 3.jprofiler
> 
参见另一篇[jprofiler安装图解](http://blog.csdn.net/java2000_wl/article/details/7012998)
